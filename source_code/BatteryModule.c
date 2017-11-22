/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : BatteryModule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
#define _IN_BATTERYMODULE

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "Struct.h"
#include "MsgDef.h"
#include "Resource.h"

#include "Global.h"
#include "Lcd.h"
void UsbConnectedHook(void)
{
//    DisplayPicture(0,0,128,160,(unsigned long)(IMAGE_IN_FLASH_BASE_ADDR)+3*PicPerPage);
//    UsbTranFlag = 1;
//    BatteryDetectState = 3;
}

//void SendDisplayBatteryFlag(void);

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void BatteryLevelHcheck(void)
{
    BatteryDetectCountL = 0;
    if (++BatteryDetectCountH > 4) {
        BatteryLevel ++;
        BatteryDetectCountL = 0;
        BatteryDetectCountH = 0;
        SysBatteryFlag.bc.bBatteryDisplay = 1;
        //SendDisplayBatteryFlag();
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void BatteryLevelLcheck(void)
{
    BatteryDetectCountH = 0;
    if (++BatteryDetectCountL > 4){
        BatteryLevel --;
        BatteryDetectCountL = 0;
        BatteryDetectCountH = 0;
        SysBatteryFlag.bc.bBatteryDisplay = 1;
        //SendDisplayBatteryFlag();
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void BatteryLevelDetect(void)
{
    if (BatteryFlag.bit.Rdy) {
        BatteryFlag.bit.Rdy = 0;
        switch (BatteryLevel) {
        case 0:
            if (BatteryAdcData < BATTERY_LEVEL_0) {
                BatteryDetectCountH = 0;
                if (++ BatteryDetectCountL > 20){
                    Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                    Fun1ModuleChange();
                    Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                    Fun2ModuleChange();
//------------------------------------------------------------------------------
//背光关闭时，低压关机时需要开背光提示
                    LCD_SDRAM_Standby_Return();
                    LCD_Init(0);
                    SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_LOWPOWER_TXT);
#ifdef LCD_MEMORY_DEV
                    DmaFillImage2Lcd();
#endif
                    LcdBackLightOn();
//------------------------------------------------------------------------------
                    DisplayFlag = DISP_DIALOG;
                    Delay1Ms(500);
                    SysPowerDown();
                }
            } else if (BatteryAdcData > BATTERY_LEVEL_1 + BATTERY_LEVEL_SETP){
                BatteryLevelHcheck();
            } else {
                BatteryDetectCountL = 0;
                BatteryDetectCountH = 0;
            }
            break;
            
        case 1:
            if (BatteryAdcData < BATTERY_LEVEL_1 - BATTERY_LEVEL_SETP) {
                BatteryLevelLcheck();
            } else if (BatteryAdcData > BATTERY_LEVEL_2 + BATTERY_LEVEL_SETP){
                BatteryLevelHcheck();
            } else {
                BatteryDetectCountL = 0;
                BatteryDetectCountH = 0;
            }
            break;
        
        case 2:
            if (BatteryAdcData < BATTERY_LEVEL_2 - BATTERY_LEVEL_SETP) {
                BatteryLevelLcheck();
            } else if (BatteryAdcData > BATTERY_LEVEL_3 + BATTERY_LEVEL_SETP){
                BatteryLevelHcheck();
            } else {
                BatteryDetectCountL = 0;
                BatteryDetectCountH = 0;
            }
            break;
            
        case 3:
            if (BatteryAdcData < BATTERY_LEVEL_3 - BATTERY_LEVEL_SETP) {
                BatteryLevelLcheck();
            } else {
                BatteryDetectCountL = 0;
                BatteryDetectCountH = 0;
            }
            break;
        default:
            BatteryDetectCountL = 0;
            BatteryDetectCountH = 0;
            break;
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void BatteryChargeStart(void)
{
    BatteryDetectCountL = 0;
    BatteryDetectCountH = 0;
    BatteryDetectCounter = 0;
    
    MusicSetFreq51();
//    bitclear_creg(%imask,15);
    if(BatteryLevel <= 3){
        DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, (unsigned long)(CHARGE_LEVEL_PIC) + BatteryLevel * MAX_PIC_SIZE);
#ifdef LCD_MEMORY_DEV 
        DmaFillImage2Lcd();
#endif
    }
//    bitset_creg(%imask,15);
    MusicSetFreq51Return();
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void BatteryChargeDetect(void)
{
    if (BatteryFlag.bit.Rdy != 1) return;
        BatteryFlag.bit.Rdy = 0;
        
    if (BatteryAdcData <= BATTERY_LEVEL_4){
        if (((BatteryDetectCounter % 20) == 0) && (BatteryDetectCounter > 0)) {
            if (PreDisplayFlag == DISP_BATTERY) {
                MusicSetFreq51();
                DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, (unsigned long)(CHARGE_LEVEL_PIC) + ((BatteryDetectCounter/20) - 1) * MAX_PIC_SIZE);
                MusicSetFreq51Return();
            } else {
                SysBatteryFlag.bc.bBatteryDisplay = 1;
                BatteryLevel = BatteryDetectCounter/20 - 1;
            }
            if (BatteryDetectCounter >= 80) {
                if (BatteryAdcData <= BATTERY_LEVEL_2) {
                    BatteryDetectCounter = 0;
                } else if (BatteryAdcData <= BATTERY_LEVEL_3) {
                    BatteryDetectCounter = 21;
                } else {
                    BatteryDetectCounter = 41;
                }
            } else {
                BatteryDetectCounter ++;
            }
        }
        BatteryDetectCountH = 0;
    }
    
    if (/*(ChargeAdcData > CHARGE_ADC_30) || */(BatteryAdcData > BATTERY_LEVEL_4)) {
        if (++BatteryDetectCountH > 8) {
            //write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0xdf));
            SysBatteryFlag.bc.BatteryChargeEn = 0;
            BatteryLevel = 3;
            if (PreDisplayFlag == DISP_BATTERY) {
                DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, (unsigned long)(CHARGE_LEVEL_PIC) + 3 * MAX_PIC_SIZE);
            } else {
                SysBatteryFlag.bc.bBatteryDisplay = 1;
            }
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void IncBatteryDetectCounter(void)
{
    if (SysBatteryFlag.bc.BatteryChargeEn == 1) {
        if (((BatteryDetectCounter % 20) != 0) || (BatteryDetectCounter == 0))
            BatteryDetectCounter ++ ;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void AdcSampleInit(void)
{
    int i;
    
    write_mem(ADC_ADCFRE, (CurrApbFreq) -1);          //ADC Freq Set
    
    BatteryAdcDataSum = 0;
    for (i = 200; i >= 0; i--) {
	    write_mem(ADC_ADCCON, (0x0000 << 2) | 0x0001);      //ADC0 Sel / ADC Start 
	    Delay1Ms(1);
	    AdcData0 = read_mem(ADC_ADCDAT0);
	    
	    write_mem(ADC_ADCCON, (0x0001 << 2) | 0x0001);      //ADC1 Sel / ADC Start 
	    Delay1Ms(1);
	    AdcData1 = read_mem(ADC_ADCDAT0);
	    
	    if (i < 8) {
		    BatteryAdcDataSum +=  (unsigned int)((unsigned long)(AdcData0) * 1024 / (AdcData1 + 1));
	    }
	}
#ifndef _INSIDE_HPSENSE_USED_
    AdcSampleCount = 2;
    write_mem(ADC_ADCCON, (0x0002 << 2) | 0x0001);
    
    EarAdcData = 0;
    EarAdcDataSum = 0;
#else
    AdcSampleCount = 0;
    write_mem(ADC_ADCCON, (0x0000 << 2) | 0x0001);
#endif
    BatteryFlag.word = 0;
    BatteryDetectState = 0;
    BatteryDetectCounter = 0;
    BatteryDetectCountL = 0;
    BatteryDetectCountH = 0;
    
    BatteryAdcData = BatteryAdcDataSum >> 3;
    BatteryAdcDataSum = BatteryAdcData;
    
    if (BatteryAdcData < BATTERY_LEVEL_1) {
        BatteryLevel = 0;
    } else if (BatteryAdcData < BATTERY_LEVEL_2) {
        BatteryLevel = 1;
    } else if (BatteryAdcData < BATTERY_LEVEL_3) {
        BatteryLevel = 2;
    } else {
        BatteryLevel = 3;
    }
    SysBatteryFlag.bc.bBatteryDisplay = 1;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void AdcDataSample(void)
{
    unsigned int temp,temp1;
    
    temp  = AdcSampleCount & 0x03;
    
    if ((read_mem(ADC_ADCRDY) & 0x0001) == 0) {
        write_mem(ADC_ADCCON, (temp << 2) | 0x0001);
        return;
    }
    
    
//    DisplayTestDecNum(20, 4, TestCount++);
    
    temp1 = AdcSampleCount & 0x0c;
    
    AdcSampleCount ++;
    AdcSampleCount &= 0x0f;
    
    if (temp == 0) {            //Battery Det
        AdcData0 = read_mem(ADC_ADCDAT0);                   //ADC0 Sample
        write_mem(ADC_ADCCON, (0x0001 << 2) | 0x0001);      //ADC1 Start
        
        BatteryAdcDataSum +=  (unsigned int)((unsigned long)(AdcData0) * 1024 / (AdcData1 + 1));
        if (temp1 == 0x0c) {
            BatteryFlag.bit.Rdy = 1;
            BatteryAdcData = BatteryAdcDataSum >> 2;
            BatteryAdcDataSum = 0;
            //DisplayTestDecNum(100, 4, BatteryAdcData);
        }
    } else if (temp == 1) {     //1.8v Det
#ifdef  _INSIDE_HPSENSE_USED_        
        AdcData1 = read_mem(ADC_ADCDAT0);                   //ADC1 Sample
        write_mem(ADC_ADCCON, (0x0000 << 2) | 0x0001);      //ADC2 Start

        AdcSampleCount += 2;
        AdcSampleCount &= 0x0f;
	}
#else
        AdcData1 = read_mem(ADC_ADCDAT0);                   //ADC1 Sample
        write_mem(ADC_ADCCON, (0x0002 << 2) | 0x0001);      //ADC2 Start
    } else if (temp == 2) {     //Ear Det
        AdcData2 = read_mem(ADC_ADCDAT0);                   //ADC2 Sample
        write_mem(ADC_ADCCON, (0x0000 << 3) | 0x0001);      //ADC3 Start
        
        EarAdcDataSum +=  (unsigned int)((unsigned long)(AdcData2) * 1024 / (AdcData1 + 1));
        if (temp1 == 0x0c) {
            PeriphFlag.bc.bEarAdcRdy = 1;
            EarAdcData = EarAdcDataSum >> 2;
            EarAdcDataSum = 0;
            //DisplayTestDecNum(180, 4, EarAdcData);
        }
        AdcSampleCount ++;
        AdcSampleCount &= 0x0f;
    }
#endif
}
//******************************************************************************
