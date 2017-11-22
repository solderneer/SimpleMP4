//******************************************************************************
//SysUSB.c          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#define _IN_USBMODULE
//******************************************************************************

#include <creg.h>
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Lcd.h"
#include "Resource.h"
#include "Config.h"

#include "Message.h"
#include "BatteryModule.h"

extern void DisplayPicture(int x, int y, int xsize, int ysize, unsigned long Flash_Addr);

#ifdef SYSTEM_SD_CARD_SP
    void SDCardStatusDetectModule(void);
#endif
void USBStatusDetectModule(void);
void EarStatusDetectModule(void);
void UsbTranEnd(void);
void AmplifierOff(void);
//void SystemReset(void);
/*------------------------------------------------------------------------------
Function: 外围接口的动态插拔实时检测
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void PeripheralDetectModule(void)
{
#ifdef SYSTEM_SD_CARD_SP
    SDCardStatusDetectModule();
#endif    
    EarStatusDetectModule();
    
    USBStatusDetectModule();
}

//******************************************************************************
/*------------------------------------------------------------------------------
Function: USB 和 充电检测
Input   :
Output  :
------------------------------------------------------------------------------*/
#ifdef SYSTEM_SD_CARD_SP
//__attribute__((section(".sys_text,\"ax\"")))
void SDCardStatusDetectModule(void)
{
//    if (BroActFlag != 0) {
        if(SDCARD_INSERT()){
            if ((SystemInfo.bc.bBackLightStatus != 0) && (PeriphFlag.bc.SdPlugIn == 0)) {
                PeriphFlag.bc.SdPlugIn = 1;
                
                LightOffCounter = 0;
                SysIdleCounter = 0;
                
                bitclear_creg(%imask,5);
                SystemInfo.bc.bBackLightOnStart = 1;
                bitset_creg(%imask,5);
                LCD_SDRAM_Standby_Return();
            }
        } else {
            if ((SystemInfo.bc.bBackLightStatus != 0) && (PeriphFlag.bc.SdPlugIn == 1)/* && (BroActFlag != 0)*/) {
                PeriphFlag.bc.SdPlugIn = 0;
                
                LightOffCounter = 0;
                SysIdleCounter = 0;
                
                bitclear_creg(%imask,5);
                SystemInfo.bc.bBackLightOnStart = 1;
                bitset_creg(%imask,5);
                LCD_SDRAM_Standby_Return();
            }
            if (MemorySelect == 1) {
                MemorySelect = 0;
                PrewMemorySelect =  MemorySelect;
                
                Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                Fun1ModuleChange();
                Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                Fun2ModuleChange();
                
                write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffb);
                
                MediaSetup(0);
                FileInit();
                BroActFlag = 0;
                if (DisplayFlag != DISP_MENU) {
                    MainMenuState = 0;
                    DisplayFlag = DISP_MENU;
                    LCD_Init(0);
                    LCD_SetDiaplayMode(0);
                }
                PreDisplayFlag = DisplayFlag;
            }
        }
//    }
}
#endif

/*------------------------------------------------------------------------------
Function: USB 和 充电检测
Input   :
Output  :
------------------------------------------------------------------------------*/
//P1.3 EAR_CON: 1 mute
//P1.7 SPK_CON: 0 mute
void AmplifierOn(void)
{
    unsigned int TimeOut;
    
    PeriphFlag.bc.bEarStatusCheckEn = 1;
    
#ifndef _INSIDE_HPSENSE_USED_
    bitclear_creg(%imask,5);
    if (PeriphFlag.bc.bEarAdcRdy == 1) PeriphFlag.bc.bEarAdcRdy = 0;
    bitset_creg(%imask,5);
    
    TimeOut = 0;
    do {
        if (++TimeOut > 64000) break;
    } while(PeriphFlag.bc.bEarAdcRdy == 0);
    BatteryFlag.bit.Rdy = 0;
    PeriphFlag.bc.bEarAdcRdy = 0;
    if (/*(EAR_ADC_16 < EarAdcData) && */(EarAdcData < EAR_ADC_20)) {
#else
    if(GetHpState()){
#endif
        PeriphFlag.bc.EarPlugIn = 1;
        PeriphFlag.bc.EarEjectout = 0;
#if(CPU_TYPE == _RK2608_)        
        write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x77));
#else
        write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x7F));    //关功放
#endif
        Delay1Ms(200);
    } else {
        PeriphFlag.bc.EarEjectout = 1;
        PeriphFlag.bc.EarPlugIn = 0;
#if(CPU_TYPE == _RK2608_)
        write_mem(GPIO_PDAT1, (read_mem(GPIO_PDAT1) | 0x88));
#else
        write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1))|0x80);    // 开功放
#endif
    }
    
    OutputVolumeSet(OutputVolume);
}

/*------------------------------------------------------------------------------
Function: USB 和 充电检测
Input   :
Output  :
------------------------------------------------------------------------------*/
void AmplifierOff(void)
{
    PeriphFlag.bc.bEarStatusCheckEn = 0;
    OutputVolumeSet(0);
    delay_nops(100);
#if(CPU_TYPE == _RK2608_)
    write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x77) | 0x08);
#else
    write_mem(GPIO_PDAT1,read_mem(GPIO_PDAT1) & 0x7f);
#endif
}

/*------------------------------------------------------------------------------
Function: USB 和 充电检测
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void EarStatusDetectModule(void)
{
    if (PeriphFlag.bc.bEarStatusCheckEn != 1) return;

#ifndef _INSIDE_HPSENSE_USED_
    if (PeriphFlag.bc.bEarAdcRdy != 1) return;
    PeriphFlag.bc.bEarAdcRdy = 0;
    if (/*(EAR_ADC_16 < EarAdcData) && */(EarAdcData < EAR_ADC_20)) {
#else
    if(GetHpState()){
#endif
        if (PeriphFlag.bc.EarPlugIn == 0) {
            PeriphFlag.bc.EarPlugIn = 1;
            PeriphFlag.bc.EarEjectout = 0;
#if(CPU_TYPE == _RK2608_)
            write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x77));
#else
            write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x7F));            // 关功放
#endif
        }
    } else {
        if (PeriphFlag.bc.EarEjectout == 0) {
            PeriphFlag.bc.EarEjectout = 1;
            PeriphFlag.bc.EarPlugIn = 0;
#if(CPU_TYPE == _RK2608_)
            write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) | 0x88));
#else
            write_mem(GPIO_PDAT1,read_mem(GPIO_PDAT1) | 0x80);       //开功放
#endif
        }
    }
}

/*------------------------------------------------------------------------------
Function: USB 和 充电检测
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void USBStatusDetectModule(void)
{
    unsigned int ImaskTemp;
    
    if(USB_INSERT()) {
        if (PeriphFlag.bc.UsbPlugIn == 0) {
            PeriphFlag.bc.UsbPlugIn = 1;
            //write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) | 0x20));
            ////////////////////////////////////////////////
            read_creg(%imask,ImaskTemp);
            bitclear_creg(%imask,5);
            LightOffCounter = 0;
            if (SystemInfo.bc.bSysIdleStatus) {
                SystemInfo.bc.bSysIdleStatus = 0;
                SysCpuSetFreq(PrevCpuFreq);
                SysTimer0Set(PrevCpuFreq,PrevTimer0Val);
            }
            if (SystemInfo.bc.bBackLightStatus != 0) {
                SystemInfo.bc.bBackLightOnStart = 1;
                LCD_SDRAM_Standby_Return();
            }
            //SystemInfo.bc.bSysIdleEn = 0;
            SysIdleCounter = 0;
            write_creg(%imask,ImaskTemp);
            //////////////////////////////////////////////
            if (SystemInfo.bc.bUsbFirstDisable == 0) {
                Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                Fun1ModuleChange();
                Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                Fun2ModuleChange();
            } else {
                SystemInfo.bc.bUsbFirstDisable = 0;
            }
            FileFragCleanFlag = 1;
            SaveSysInformation();
            CodecPowerDown();
            //AmplifierOff();
#if(CPU_TYPE == _RK2608_)        
            write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x77) | 0x08);
#else
            write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x77));    //关功放
#endif            
            LCD_Init(0);
            LCD_SetDiaplayMode(0);
//            LCD_SetColor(0);
            SysUsbFlag.bc.UsbTran = 0;
            if (SysUsbFlag.bc.TranMode == 0) {
                
                DisplayFlag = DISP_BATTERY;
                PreDisplayFlag = DisplayFlag;
                
                SysBatteryFlag.bc.BatteryChargeEn = 1;
                //AdcSampleInit();
                BatteryChargeStart();
            
                PostSche1Message(MSG_SCHEDULER1_USB/*,0,0,0,0,0,0,0*/);
                PostFun1Message(MSG_FUNCTION1_USB_INIT,0,0,0,0/*,0,0,0*/);
            } else {
                DisplayFlag = DISP_USB;
                PreDisplayFlag = DisplayFlag;
                
                SysBatteryFlag.bc.BatteryChargeEn = 0;
                PostSche1Message(MSG_SCHEDULER1_UPGRADE/*,0,0,0,0,0,0,0*/);
                PostFun1Message(MSG_FUNCTION1_UPGRADE_INIT,0,0,0,0/*,0,0,0*/);
                
                //bitclear_creg(%imask,15);
                bitclear_creg(%imask,5);
                DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, USB_CONNECT_PIC);
#ifdef LCD_MEMORY_DEV 
                DmaFillImage2Lcd();
#endif
                //bitset_creg(%imask,15);
            }
        }
        
        if ((UsbConnected == 1) && (Sys1InitFlag.bc.bUsbModuleInitEnable == 0)) {
            if (SysUsbFlag.bc.UsbTran == 0) {
                SysUsbFlag.bc.UsbTran = 1;
                SysBatteryFlag.bc.BatteryChargeEn = 0;
                
                DisplayFlag = DISP_USB;
                PreDisplayFlag = DisplayFlag;
                
                //bitclear_creg(%imask,15);
                bitclear_creg(%imask,5);
                SystemInfo.bc.bSysIdleEn = 0;
                DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, USB_CONNECT_PIC);
#ifdef LCD_MEMORY_DEV 
                DmaFillImage2Lcd();
#endif
//                LCD_NFDispStringAt(50, 158, "Ready...");
                //bitset_creg(%imask,15);
            }
        } else {
            if (SysUsbFlag.bc.UsbTran == 1) {
                SysUsbFlag.bc.UsbTran = 0; 
                ////////////////////////////////////////////////
                read_creg(%imask,ImaskTemp);
                bitclear_creg(%imask,5);
                
                LightOffCounter = 0;
                if (SystemInfo.bc.bBackLightStatus != 0) {
                    SystemInfo.bc.bBackLightOnStart = 1;
                    LCD_SDRAM_Standby_Return();
                }
                write_creg(%imask,ImaskTemp);
                //////////////////////////////////////////////
                //if (SysUsbFlag.bc.TranMode == 0) {
                    //usb 通讯结束
                    UsbTranEnd();
                    SystemInfo.bc.bSysIdleEn = 1;
                    SysIdleCounter = 0;
                    
                    Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                    Fun1ModuleChange();
                    
                    DisplayFlag = DISP_BATTERY;
                    PreDisplayFlag = DisplayFlag;
                    
                    SysBatteryFlag.bc.BatteryChargeEn = 1;
                    //AdcSampleInit();
                    BatteryChargeStart();
                    
                //} else {
                //    //固件升级结束
                //    SysPowerDown();
                //}
            } else {
                if (SysBatteryFlag.bc.BatteryChargeEn == 1) {
                    BatteryChargeDetect();
                }
            }
        }
    } else {
        if (SystemInfo.bc.bUsbFirstDisable == 1) {
            SystemInfo.bc.bUsbFirstDisable = 0;
            PeriphFlag.bc.UsbPlugIn = 1;
        }
            
        if (PeriphFlag.bc.UsbPlugIn == 1) {
            PeriphFlag.bc.UsbPlugIn = 0;
            //AmplifierOn();
            PeriphFlag.bc.EarPlugIn = 0;
            PeriphFlag.bc.EarEjectout = 0;
            
            if ((DisplayFlag == DISP_USB) || (DisplayFlag == DISP_BATTERY)) {
                if (SysUsbFlag.bc.TranMode == 0) {
                    if (SysUsbFlag.bc.UsbTran == 1) {
                        UsbTranEnd();
                        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                        Fun1ModuleChange();
                    }
                    SystemInfo.bc.bSysIdleEn = 1;
                    SysIdleCounter = 0;
                    MainMenuState = 0;
                    DisplayFlag = DISP_MENU;
                    PreDisplayFlag = DisplayFlag;
                } else {
                    write_mem(GPIO_PDAT0,read_mem(GPIO_PDAT0) & 0xff7f);
                    while(1);
                }
            }
            
            SysBatteryFlag.bc.BatteryChargeEn = 0;
            //write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0xdf));
            ////////////////////////////////////////////////
            read_creg(%imask,ImaskTemp);
            bitclear_creg(%imask,5);
            
            LightOffCounter = 0;
            if (SystemInfo.bc.bSysIdleStatus) {
                SystemInfo.bc.bSysIdleStatus = 0;
                SysCpuSetFreq(PrevCpuFreq);
                SysTimer0Set(PrevCpuFreq,PrevTimer0Val);
            }
            if (SystemInfo.bc.bBackLightStatus != 0) {
                //LcdBackLightOn();
                SystemInfo.bc.bBackLightOnStart = 1;
                LCD_SDRAM_Standby_Return();
            }
            
            write_creg(%imask,ImaskTemp);
            //AdcSampleInit();
            //////////////////////////////////////////////
        }
        BatteryLevelDetect();
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void UsbTranEnd(void)
{
    Delay1Ms(200);
    
    bitclear_creg(%imask,10);
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffe);
    write_XDATA(USB_ADDR, read_XDATA(USB_ADDR) & (~USB_HS_ENABLE));		//FULL SPEED
	write_XDATA(USB_ADDR, read_XDATA(USB_ADDR) & (~USB_SOFTCONNECT));	//Disconnect
        
    Delay1Ms(200);
    
    UsbEnd_SDProcess();//USB退出时关闭SD卡相关操作
    USBGetRemapTbl();
    MediaSetup(FLASH);
    FileInit();
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffa);
    
    MemorySelect = 0;
    PrewMemorySelect = MemorySelect;
    SysFreeMemeryFlash = GetFreeMem(0);

    bitset_creg(%imask,5);
	SysCpuSetFreq(24);
    SysTimer0Set(24,25000);//SysTimer0Set(24,10000);
}
//******************************************************************************