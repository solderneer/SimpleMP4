/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FmSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/11       1.0            ORG
********************************************************************************
*/


#include <Creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"

#ifdef FM_MODULE
    #define IN_TUNER_DRV
    #include "FmMacro.h"
    #include "FmStruct.h"
    #include "FmGlobal.h"

    void IIC_SysInit(void);

///**************************************************************************************
//*   函数名称：Tuner_DummyWait
//*   功    能：
//*   输入参数：
//*   输出参数：
//*   说    明：  
//**************************************************************************************/
//void Tuner_DummyWait(unsigned long count)
//{
//    while(1){
//        if (--count == 0)
//            break;
//    }
//}
/**************************************************************************************
*   函数名称：IIC_SysInit
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
void IIC_SysInit(void)
{
    IIC_DISABLE;
    
    write_mem(GPIO_PCON1,read_mem(GPIO_PCON1) | 0x003c);//00,00,00,00,00,11,11,00
    delay_nops(100);
    
    bitset_mem(I2C_EXT,0); 
    write_mem(IC_SAR,0x55);             /* IIC Slave address */
    write_mem(IC_TAR,0x60);             /* IIC Target address */
    
    write_mem(IC_CON,0x63);             /* Slave Mode */
    write_mem(IC_SS_HCNT,0x88);
    write_mem(IC_SS_LCNT,0x66);
    IIC_ENABLE;
    Delay1Ms(2);
}

/**************************************************************************************
*   函数名称：Tuner_ReadData
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
unsigned int Tuner_ReadData(unsigned char *data, unsigned int size)
{
    unsigned int status,i;
    unsigned char temp,test[5];
    unsigned int j;
    
    IIC_SysInit();
    delay_nops(24000);
    
    j = 0;
    status = 0;
    for (i = 0; i < size + 1; i++) {
        write_XDATA(IC_DATA_CMD,0x100);
        if((read_XDATA(IC_STATUS) & RFNE) == 1) {
            *(data++) = read_XDATA(IC_DATA_CMD);
            j++;
        }
        temp = 0;
        do {
            if (++ temp == 0xfff0)
                break;
        } while ((read_XDATA(IC_STATUS) & TFNF) == 0);
    }
    
    for ( ; j < size; j ++) {
        temp = 0;
        do {
            if (++ temp == 0xfff0)
                break;
        } while ((read_XDATA(IC_STATUS) & RFNE) == 0);
        *(data++) = read_XDATA(IC_DATA_CMD);
    }
    Delay1Ms(2);
    return(status);
}

/**************************************************************************************
*   函数名称：Tuner_SendCommand
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
unsigned int Tuner_SendCommand(unsigned char *pdata, unsigned int size)
{
    unsigned int  status,i,j;
    unsigned char readdata[5];
    
    IIC_SysInit();
    delay_nops(24000);
    
    status = 0;
    for(i = 0; i < size+1; i++) {
        j = 0;
        do {
            if (++ j == 0xfff0)
                status = 1;
                break;
        } while (read_XDATA(IC_STATUS) & TFNF == 0);
        write_XDATA(IC_DATA_CMD,*(pdata+i));
    }
    
    j = 0;
    do {
        if (++ j == 0xfff0)
            status = 1;
            break;
    } while ((read_XDATA(IC_STATUS) & ACTIVITY) == 1);
    Delay1Ms(2);
    return(status);
}

/**************************************************************************************
*   函数名称：Tuner_MuteControl
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
void Tuner_MuteControl(bool active)
{
    IIC_SysInit();
    delay_nops(24000);
    
    if(active)
        TEA5767ControlData[0] |= TUNER_MUTE;
    else
        TEA5767ControlData[0] &= (unsigned char)~TUNER_MUTE;
    
    if(Tuner_SendCommand(TEA5767ControlData,1) != 0)
        Tuner_SendCommand(TEA5767ControlData,1);
}

///**************************************************************************************
//*   函数名称：Fm Radio Stereo Control
//*   功    能：
//*   输入参数：
//*   输出参数：
//*   说    明：  
//**************************************************************************************/
//__attribute__((section(".fm_text,\"ax\"")))
//void Tuner_StereoControl(void)
//{
//    TEA5767ControlData[2] &= 0xF7; //~TUNER_MS 0x08
//    TEA5767ControlData[3] = 0x00 | TUNER_XTAL | TUNER_HCC ;//| TUNER_SNC;
//    if (GlobalFmVar.FmStereo == 1) { 
//        TEA5767ControlData[2] |= TUNER_MS;
//    } else {
//        TEA5767ControlData[3] |= TUNER_SNC;
//    }
//    if(Tuner_SendCommand(TEA5767ControlData,5) != 0) {
//        Tuner_SendCommand(TEA5767ControlData,5);
//    }
//}

/**************************************************************************************
*   函数名称：Fm Radio Power Down
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
void Tuner_PowerDown(void)
{
    IIC_SysInit();
    delay_nops(24000);
    TEA5767ControlData[3] |= TUNER_STBY;
    if(Tuner_SendCommand(TEA5767ControlData,5) != 0) {
        Tuner_SendCommand(TEA5767ControlData,5);
    }
}
/**************************************************************************************
*   函数名称：Tuner_PLLCount
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\""))) 
void Tuner_PLLCount(unsigned int TunerFrequency, unsigned char hightsideflag)
{
    unsigned long  pll_value;
    unsigned char  temp;
    unsigned int   round_fact;

    pll_value = (unsigned long)TunerFrequency * 10;
    
    if (hightsideflag == 0)
       pll_value -= 225;
    else
       pll_value += 225;
       
    pll_value = pll_value * 4 * 1000;
    round_fact = pll_value % TUNER_REFFREQ;
    pll_value = pll_value / TUNER_REFFREQ;
    
    if(round_fact >=  (TUNER_REFFREQ / 2))
        pll_value ++;
    
    temp  = ((unsigned char)(pll_value >> 8) & 0x3f) | (TEA5767ControlData[0] & 0x80);
    
    TEA5767ControlData[0] = temp | TUNER_MUTE;
    TEA5767ControlData[1] = (unsigned char)(pll_value & 0xff);
    if (hightsideflag == 0) {
        TEA5767ControlData[2] = 0xa0;
    } else {
        TEA5767ControlData[2] = 0xb0;
    }
    TEA5767ControlData[3] = 0x00 | TUNER_XTAL | TUNER_HCC ;//| TUNER_SNC;
    if (GlobalFmVar.FmStereo == 1) { //add by zyz
        TEA5767ControlData[2] |= TUNER_MS;
    } else {
        TEA5767ControlData[3] |= TUNER_SNC;
    }
    if (GlobalFmVar.FmArea == FM_AREA_JAPAN) { //add by zyz
        TEA5767ControlData[3] |= TUNER_BL;
    }
    TEA5767ControlData[4] = 0x00;
        
    if(Tuner_SendCommand(TEA5767ControlData,5) != 0) {
        Tuner_SendCommand(TEA5767ControlData,5);
    }
}    

/**************************************************************************************
*   函数名称：Tuner_FreqChange
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
void Tuner_FreqChange(unsigned char updownflag,unsigned int step)
{   
    if(updownflag == TUN_UP)
        GlobalFmVar.FmFreq += step;
    if(updownflag == TUN_DOWN)
        GlobalFmVar.FmFreq -= step;
    
    if(GlobalFmVar.FmFreq > FmFreqMaxVal)
        GlobalFmVar.FmFreq = FmFreqMinVal;
    if(GlobalFmVar.FmFreq < FmFreqMinVal)
        GlobalFmVar.FmFreq = FmFreqMaxVal;
}

/**************************************************************************************
*   函数名称：GetStereoStatus
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
void GetStereoStatus(void)
{
    unsigned char TempBuffer[5];
    
    Tuner_ReadData(TempBuffer,5);
    if (TempBuffer[2] & 0x80) {
        FmStereoState = 1;
    } else {
        FmStereoState = 0;
    }
    //FmDisplayBlock.DisplayFlag.bit.bFmStereo = 1;
}

/**************************************************************************************
*   函数名称：GetStereoStatus
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
void GetNextStation(void)
{
    unsigned int i;
    
    for (i = 0; i < FREQMAXNUMBLE; i ++) {
        if (++GlobalFmVar.FmSaveNum >= FREQMAXNUMBLE)
            GlobalFmVar.FmSaveNum = 0;
        GlobalFmVar.FmFreq = GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum];
        if ((FmFreqMinVal <= GlobalFmVar.FmFreq) && (GlobalFmVar.FmFreq <= FmFreqMaxVal))
            return;
    }
    GlobalFmVar.FmSaveNum = 0;
    GlobalFmVar.FmFreq = FmFreqMinVal;
}

/**************************************************************************************
*   函数名称：GetStereoStatus
*   功    能：
*   输入参数：
*   输出参数：
*   说    明：  
**************************************************************************************/
__attribute__((section(".fm_text,\"ax\"")))
void GetPrevStation(void)
{
    unsigned int i;
    
    for (i = 0; i < FREQMAXNUMBLE; i ++) {
        if (GlobalFmVar.FmSaveNum == 0)
            GlobalFmVar.FmSaveNum = FREQMAXNUMBLE;
        GlobalFmVar.FmSaveNum --;
        GlobalFmVar.FmFreq = GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum];
        if ((FmFreqMinVal <= GlobalFmVar.FmFreq) && (GlobalFmVar.FmFreq <= FmFreqMaxVal))
            return;
    }
    GlobalFmVar.FmSaveNum = 0;
    GlobalFmVar.FmFreq = FmFreqMinVal;
}

///**************************************************************************************
//*   函数名称：Tuner_FetchFreq
//*   功    能：
//*   输入参数：
//*   输出参数：
//*   说    明：  
//**************************************************************************************/
//__attribute__((section(".fm_text,\"ax\"")))
//void Tuner_FetchFreq(void)
//{
//    
//    TunerFrequency = TunerFreqMemory[FreqNumble];
//    FreqNumble++;
//    if (FreqNumble > FREQMAXNUMBLE)
//        FreqNumble = 0;
//        
//    Tuner_PLLCount(1);
//    Tuner_DummyWait(60000);                        //delay 30mS
//    Tuner_MuteControl(FALSE);
//}

///**************************************************************************************
//*   函数名称：Tuner_StoreFreq
//*   功    能：
//*   输入参数：
//*   输出参数：
//*   说    明：  
//**************************************************************************************/
//__attribute__((section(".fm_text,\"ax\"")))
//void Tuner_StoreFreq(void)
//{
//    TunerFreqMemory[FreqNumble] = TunerFrequency;
//    FreqNumble++;
//    if (FreqNumble > FreqMaxNumble)
//        FreqNumble = 0;
//}
//
///*************************************************************************************/

#endif
