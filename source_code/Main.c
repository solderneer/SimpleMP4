/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Main.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/12/12       1.0            ORG
*
********************************************************************************
*/
#define _IN_MAIN

extern unsigned int sysareabackupcheck(void);
extern unsigned int sysareabackupread(unsigned int block);
extern void sysareacopy(void);

#include <creg.h>
#include "Macro.h"
#include "KeyMacro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"
#include "Timer.h"
#include "Lcd.h"
#include "BatteryModule.h"

#ifdef  AVI_DECODE_MODULE
#include "mp4decode.h"
#include "aviglobal.h"

#include "AviMp3Decode.h"
#endif

#ifdef  MP3_DECODE_MODULE
#include "Mp3_Decode.h"
#endif

#ifdef  WMA_DECODE_MODULE
#include "WmaDecoder.h"
#endif

#ifdef  WAV_DECODE_MODULE
#include "WavDecoder.h"
#endif

unsigned char MusicFileExtString[16] = "MP1MP2MP3WMAWAV";
unsigned char MovieFileExtString[4] = "AVI";
unsigned char JpegFileExtString[7] = "JPGBMP";
unsigned char TextFileExtString[4] = "TXT";

//unsigned int MusicPlayEqStr[12][8] =
//{
//    {"  NOR  "},
//    {"  3D   "},
//    {" ROCK  "},
//    {"  POP  "},
//    {" CLASS "},
//    {" BASS  "},
//    {" JAZZ  "},
//    {" USER  "},
//    {" METAL "},
//    {" DANCE "},
//    {" PARTY "},
//    {" CLUB  "},
//};

//unsigned int MusicPlaySampeRateStr[9][10] =
//{
////    {"  8Khz   "},
////    {"  11KHz  "},
////    {"  12KHz  "},
////    {"  16KHz  "},
////    {" 22.1KHz "},
////    {"  24KHz  "},
////    {"  32KHz  "},
////    {"  48KHz  "},
////    {" 44.1KHz "},
//};

unsigned int MusicEqTable[] = 
{
    NORMAL,             //MUSIC_DECODE_EQ_NORMAL  
    NORMAL,             //MUSIC_DECODE_EQ_3D      
    ROCK,               //MUSIC_DECODE_EQ_ROCK    
    POP,                //MUSIC_DECODE_EQ_POP     
    CLASSIC,            //MUSIC_DECODE_EQ_CLASSIC 
    BASS,               //MUSIC_DECODE_EQ_BASS    
    JAZZ,               //MUSIC_DECODE_EQ_JAZZ    
    USER,               //MUSIC_DECODE_EQ_USER 	  
    METAL,              //MUSIC_DECODE_EQ_METAL   
    DANCE,              //MUSIC_DECODE_EQ_DANCE   
    PARTY,              //MUSIC_DECODE_EQ_PARTY   
    CLUB,               //MUSIC_DECODE_EQ_CLUB    
};  
    
    
//unsigned int GetSampleRateId(unsigned long Freq)
//{   
//    switch (Freq) {
//    case 8000:      return(0);
//    case 11025:     return(1);
//    case 12000:     return(2);
//    case 16000:     return(3);
//    case 22050:     return(4);
//    case 24000:     return(5);
//    case 32000:     return(6);
//    case 48000:     return(7);
//  /*case 44100:*/
//    default:        return(8);
//    }
//}

/*******************************************************************************
                         ** ISR MODULE  **
* Description: 
* Function   : WdtIsr,Timer0Isr,DmaIsr,UsbIsr,UserIsr
* 
*******************************************************************************/
/*------------------------------------------------------------------------------
Function: Rock2 Mp3 Timer0 Isr
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void WdtIsr(void)
{
   //if(SystemInfo.bit.bRestartWdtEnable)
   //     write_mem(WDT_CRR,0x0076);  //WDT ReStart
}

/*------------------------------------------------------------------------------
Function: Rock2 Mp3 Usb Isr
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void UsbTranIsr(void)
{
    if( SysUsbFlag.bc.TranMode == 0 ) {
        usb_isr();
    } else {
#ifdef  UPGRADE_MODULE
        FWusb_isr();
#endif
    }
}

/*------------------------------------------------------------------------------
Function: Rock2 Mp3 Timer0 Isr
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void Timer0Isr(void)
{
    KeyScan();
    
    TimerModule();
    
    switch(Fun1MsgArray[0]) {
#ifdef AVI_DECODE_MODULE
    case MSG_FUNCTION1_AVI_DECODE:
    if (Sys1InitFlag.bc.bAviModuleInitEnable == 0) {
        AviMp4FrameCountInc();
    }
        break; 
#endif
#ifdef FM_MODULE
    case MSG_FUNCTION1_FM:
        if (Sys1InitFlag.bc.bFmModuleInitEnable == 0) {
            FmTimerDec();
        }
        break;
#endif
        
    default:
        break;
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void Timer1Isr(void)
{
    
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void DmaIsr(void)
{
    DMA_IsrProc();
}


/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void UserIsr(void)
{
    //bitclear_creg(%ireq,0);
    
    switch(Fun1MsgArray[0]) {
#ifdef MP3_DECODE_MODULE
    case MSG_FUNCTION1_MUSIC_MP3:
        MP3DecodePlay();
        break;
#endif
        
#ifdef WMA_DECODE_MODULE
    case MSG_FUNCTION1_MUSIC_WMA:
        WMADecodePlay();
        break;
#endif

#ifdef ENCODE_MODULE
    case MSG_FUNCTION1_ENCODE_PROC:
        EncPlay();
        break;
#endif

#ifdef WAV_DECODE_MODULE
    case MSG_FUNCTION1_MUSIC_WAV:
        WavDecodePlay();
        break;
#endif
        
#ifdef AVI_DECODE_MODULE
    case MSG_FUNCTION1_AVI_DECODE:
        if (AviPlayFlag.bc.bMp3DecodeEnable) {
           AviMp3DecodePlay();      
        } else {
            AviClearPcmData();
        }
        break;
#endif
    default:
        break;
    }
}

/*******************************************************************************
                         ** SYS INITIAL **
* Description: 
* Function   : PowerOnCpuInit,PowerOnVariableInit,LcdPowerOnInit,CodecPowerOnInit
* 
*******************************************************************************/
/*------------------------------------------------------------------------------
Function: Rock2 Mp3 Power On CPU Initial
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".flash_init_text, \"ax\"")))
void PowerOnCpuInit(void)
{
//--------------------------------------
//Zsp400 Register Init
    write_creg(%imask,   0x0000);
    write_creg(%smode,   0x0080);
    
    write_creg(%ip0,     0x0000);
    write_creg(%ip1,     0x0000);
    write_creg(%ireq,    0x0000);
    
    write_creg(%amode,   0x0000);
    write_creg(%fmode,   0x0000);
    write_creg(%hwflag,  0x0000);
    write_creg(%guard,   0x0000);
    
    write_creg(%timer0,  0x0000);
    write_creg(%timer1,  0x0000);
    write_creg(%tc,      0x0000);
//----------  DCDC INIT  ------------
    write_mem32(WDT_TORR,0x0000000);
    write_mem32(WDT_CR,0x00000000);
    
    
    write_mem(CLOCK_PWRCON,     0x001f);    //DMA Power On
//----------  DMA INIT  -----------
    write_mem32(DMAR_DmaCfgReg,0x00000000);
    write_mem32(DMAR_ClearBlock,0x00000007);
    write_mem32(DMAR_MaskBlock,0x0000ff00); //Clear All DMA Int
    write_mem32(DMAR_ChEnReg,0x0000ff00);
    
//----------  GPIO INIT  -----------
    write_mem(GPIO_PCON0,0x6000);           //0b: 01 01 01 01, 00 00 00 00
                                            //P00--p06: KeyScan Input
                                            //P07:Power_Con (1:PowerOn)
                                            
    write_mem(GPIO_PDAT0,0x0080);           //PowerOn
#if(CPU_TYPE == _RK2608_)
    write_mem(GPIO_PCON1,0x607c);           //0b: 01 10 00 00, 01 11 11 00
                                            //P10:      Nor CSN
                                            //P11:      No Use
                                            //P12:      No Use
                                            //P13:      EarCon(0:mute)
                                            //P14:      SD_DET(0:SD Input)
                                            //P15:      HP_SENCE(0:Input)
                                            //P16:      USB_DET(1:Input)
                                            //P17:      SPK_CON(1:mubBattte)
    //write_mem(GPIO_PDAT1,0x0080);           //Ear mute/ SPK mute
    write_mem(GPIO_PDAT1, 0x0008/*0x0000*/);           //Ear mute/ SPK mute
#else
    write_mem(GPIO_PCON1, 0x647c);
    write_mem(GPIO_PDAT1, 0x0028);    
#endif
    write_mem(GPIO_PCON2A,0x0000);          //0b: 00 00 00 00, 00 00 00 00
                                            //P20:      SDDO
                                            //P21:      SDDI
                                            //P22:      SDCLK
                                            //P23:      SDCS
                                            //P24:      A0
                                            //P25:      A1
                                            //P26:      A2
                                            //P27:      A3
                                        
     write_mem(GPIO_PCON2B,0x0400);         //P28:      WEN
                                            //P29:      CASN
                                            //P210:     RASN
                                            //P211:     CSN
                                            //P212:     No Pad
                                            //P213:     No Pad
                                            //P214:     No Pad
                                            //P215:     No Pad
    write_mem(GPIO_PDAT2, 0x0000);
    
    write_mem(GPIO_PCON3,0x0000);           //0b: 00 00 00 00, 00 00 00 00
                                            //P30--P37: D8--D15
}

/*------------------------------------------------------------------------------
Function: Rock2 Mp3 Power On Variable Initial
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".flash_init_text, \"ax\"")))
void PowerOnVariableInit(void)
{
    hMainFile = 0xffff;
    hSlaveFile = 0xffff;
    
    Sys1InitFlag.word = 0xffff;
    Sys2InitFlag.word = 0xffff;
    SystemInfo.word = 0x0000;
    
    PeriphFlag.word = 0;
    SysBatteryFlag.word = 0;
    SysUsbFlag.word = 0;
    
    MusicDisplayInterface = 1;
    
    DisplayRecoveryTime = 0xffff;
    LightOffCounter = 0;
    SysIdleCounter = 0;
    PowerOffCounter = 0;
//------------------------------------------------------------------------------
    BroActFlag = 0;
    SystemDebugFlag = 0;
//------------------------------------------------------------------------------    

    PreviousScheduler1Msg = MSG_SCHEDULER1_NONE;
    PreviousScheduler2Msg = MSG_SCHEDULER2_NONE;
    Sche1MsgArray[0] = MSG_SCHEDULER1_NONE;
    Sche2MsgArray[0] = MSG_SCHEDULER2_NONE;
    Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
    Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
    
//------------------------------------------------------------------------------
    DmaFillImage2LcdFlag = 0;
    Dma_DispFlag         = 0;
    DmaTanData2LcdCont   = 0;
    Dma_ReadLcdFlag      = 0;
//------------------------------------------------------------------------------    
    TestCount = 0;
    TestCount1 = 0;
//------------------------------------------------------------------------------    
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void DisplayPowerOffFlash(void)
{
    unsigned int i,temp;
    unsigned int TimeOut;
    
    //write_creg(%imask,0x8000);
    bitclear_creg(%imask,5);
    
    LCD_Init(0);
    SysCpuSetFreq(24);
    SysTimer0Set(48,62500);
    
    for(i = 0; i < 12; i++)
    {
        DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, NFA_POWER_OFF_PIC + (unsigned long) (i)  *MAX_PIC_SIZE);
#ifdef LCD_MEMORY_DEV 
        DmaFillImage2Lcd();
#endif 
        temp = 0;
        TimeOut = 0;
        while((!(temp & 0x0020)) && ((++TimeOut < 32768))) {
            read_creg(%ireq,temp);
        }
        bitclear_creg(%ireq,5);
    }
    
    LCD_SetColor(0x0000);
    LCD_FillRect(0,0,LCD_MAX_XSIZE,LCD_MAX_YSIZE);
#ifdef LCD_MEMORY_DEV 
    DmaFillImage2Lcd();
#endif 
    bitset_creg(%imask,5);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".sys_init_text, \"ax\"")))
void DisplayPowerOnFlash(void)
{
    unsigned int i,j,temp,block;
    unsigned int TimeOut;
    
    bitclear_creg(%imask,5);
    SysCpuSetFreq(72);
    SysTimer0Set(48,62500);
    
    block = 0;
    for(i = 0; i < 12; i++)
    {
        DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, NFA_POWER_ON_PIC + (unsigned long) (i)  *MAX_PIC_SIZE);
#if (FLASHROM_TYPE == _MLC_)
        for(j=0;j<3;j++){
          if(!SysBackupSta){
          	SysBackupSta = sysareabackupread(block);
          	block++;
          }
        }
#endif  

#ifdef LCD_MEMORY_DEV 
        DmaFillImage2Lcd();
#endif
        temp = 0;
        TimeOut = 0;
        while((!(temp & 0x0020)) && ((++TimeOut < 32768))) {
            read_creg(%ireq,temp);
        }
        bitclear_creg(%ireq,5);
    }
    
#if (FLASHROM_TYPE == _MLC_)
    for (i = 0; i < 12; i++) {
        for(j=0;j<3;j++){
          if(!SysBackupSta){
          	SysBackupSta = sysareabackupread(block);
          	block++;
          }
        }
    }
#endif  
    
    SysCpuSetFreq(24);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".sys_init_text, \"ax\"")))
void USB_Init_Powerless(void)
{
    unsigned int i;
//    
    
//////////////////////////////////////////////////////
//  bEPPflags.value = 0;
//  G_ucBulkPhase=K_CommandPhase;
//  UsbConnected=0;
//  FTLInit();
    DISABLE_USB_INT;
    read_XDATA((USB_BASE+0x02/2));//USB_INT_SRC_IN
    read_XDATA((USB_BASE+0x04/2));//USB_INT_SRC_OUT
    read_XDATA((USB_BASE+0x0a/2));//USB_INT_SRC_USB
    read_XDATA((USB_BASE+0x00/2));//USB_ADDR
//  MUSBHSFC_InitSFR();
    
    
    //write_XDATA(USB_ADDR, read_XDATA(USB_ADDR) | USB_SUSPEND_EN);     //Enable suspend
    write_XDATA((USB_BASE+0x00/2), read_XDATA((USB_BASE+0x00/2)) | (0x0001 << 8));      //Enable suspend
    //write_XDATA(USB_ADDR, read_XDATA(USB_ADDR) | USB_HS_ENABLE);          //HIGH SPEED
    write_XDATA((USB_BASE+0x00/2), read_XDATA((USB_BASE+0x00/2)) | (0x0020 << 8));          //HIGH SPEED
            
//    write_XDATA(USB_INDEX, 1);
    write_XDATA((USB_BASE+0x0e/2), 1);
//  write_XDATA(USB_CSR_IN, CSR_IN_CDT | CSR_IN_FLUSH_FIFO);            //bulk or interrupt
    write_XDATA((USB_BASE+0x12/2), 0x0040 | 0x0008);            //bulk or interrupt
//  write_XDATA(USB_CSR_OUT, CSR_OUT_CDT | CSR_OUT_FLUSH_FIFO);     //bulk or interrupt
    write_XDATA((USB_BASE+0x16/2), 0x0080 | 0x0010);        //bulk or interrupt
    
    //write_XDATA(USB_ADDR, read_XDATA(USB_ADDR) | USB_SOFTCONNECT);        //softconnet
    write_XDATA((USB_BASE+0x00/2), read_XDATA((USB_BASE+0x00/2)) | (0x0040 << 8));      //softconnet
    for(i=0; i<10; i++){
        delay_nops(48000);
    }
    write_XDATA((USB_BASE+0x00/2), read_XDATA((USB_BASE+0x00/2)) & (~(0x0020 << 8)));       //FULL SPEED
    write_XDATA((USB_BASE+0x00/2), read_XDATA((USB_BASE+0x00/2)) & (~(0x0040 << 8)));   //Disconnect    
    read_XDATA((USB_BASE+0x02/2));//USB_INT_SRC_IN
    read_XDATA((USB_BASE+0x04/2));//USB_INT_SRC_OUT
    read_XDATA((USB_BASE+0x0a/2));//USB_INT_SRC_USB
    read_XDATA((USB_BASE+0x00/2));//USB_ADDR
//////////////////////////////////////////////////////
//    write_creg(%ireq,    0x0000);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void FlashErrorHook(uint8 ErrBlkCnt)
{
    
}

/*------------------------------------------------------------------------------
Function:MLC RsEncodeAsmEnable
Input:
Output:
------------------------------------------------------------------------------*/

#if (FLASHROM_TYPE == _MLC_)    
unsigned int RsEncodeAsmEnable(void)
{
    if(/*(MSG_SCHEDULER1_ENCODE == Sche1MsgArray[0]) ||*/(Sys1InitFlag.bc.bUsbModuleInitEnable == 0))
    {
        return 1;//录音和USB的时候数据空间有表格g_Alpha4096_To，返回1
    }else
    {
        return 0;
    }
}
#endif
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".sys_init_text,\"ax\"")))
void SystemInit(void)
{   
    int Temp1,Temp2;
    unsigned int color_tmp;
    
    if((!USB_INSERT()) && (GetKeyVal() != KEY_VAL_PLAY)){
        write_mem(GPIO_PDAT0,read_mem(GPIO_PDAT0) & 0xff7f);
        while(1);
    }
    
#if 1
    extern long IdBlockByteAddr;
    FlashInit(&IdBlockByteAddr);//初始化FLASH时返回系统ID信息地址
#else   
    FlashInit();
#endif

#if (FLASHROM_TYPE == _MLC_)    
    SysCpuSetFreq(72);
    SysBackupSta = sysareabackupcheck(); 
    SysCpuSetFreq(24);
#endif    
    
    LoadSysInformation();
    
    Temp1 = GetRemapTbl();
#if 0
    Temp2 = MediaSetup(FLASH);
#else
    Temp2 = 0;
    MediaSetup(FLASH);//rock@2006-9-21新的文件系统没有高格，所以文件系统检测不通过时不低格
#endif

    write_creg(%imask,0x8080);      //开DMA中断，开全局中断
    
    LcdInit();
    LcdBackLightOn();
    
    if (Temp1 || Temp2) {
        color_tmp = LCD_GetColor();
        LCD_SetColor(COLOR_WHITE);
        LCD_NFDispStringAt(104, 112, "System Init...");
#ifdef LCD_MEMORY_DEV 
        DmaFillImage2Lcd();
#endif
        LCD_SetColor(color_tmp);
        
        LowFormat();
        FileFragCleanFlag = 0;
        SaveSysInformation();
    }
    FileInit();

    CodecPowerOnInitial();

    if(USB_INSERT()){
        SystemInfo.bc.bUsbFirstDisable = 1;
        DisplayFlag = DISP_USB;
        PreDisplayFlag = DisplayFlag;
        SysUsbFlag.bc.TranMode = 0;
        if (GetKeyVal() == KEY_VAL_MENU) {
            SysUsbFlag.bc.TranMode = 1;
        }
        write_mem(CLOCK_PWRCON,0x0019);     //Dma & MemCtr & Usb Clock Power On
    } else {
        USB_Init_Powerless();
        DisplayPowerOnFlash();
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        write_mem(CLOCK_PWRCON,0x0018);     //Dma & MemCtr Clock Power On
        
#ifdef SYSTEM_SD_CARD_SP         
        SysCpuSetFreq(72);
        if (SDCARD_INSERT()) {
            PeriphFlag.bc.SdPlugIn = 1;
            if (MemorySelect == 1) {
                write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0004);        //Sd Clock Power On
                if(MediaSetup(MemorySelect)) {
                    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffb);    //Sd Clock Power Off
                    MediaSetup(FLASH);
                    MemorySelect = FLASH;
                }
            }
        } else {
            PeriphFlag.bc.SdPlugIn = 0;
            SysFreeMemeryCard = 0;
            MediaSetup(FLASH);
            MemorySelect = FLASH;
        }
        PrewMemorySelect = MemorySelect;
#endif 
        SysFreeMemeryFlash = GetFreeMem(0);
    }
    
#if (FLASHROM_TYPE == _MLC_)    
    if(SysBackupSta){
//    	LCDDEV_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
//        LCD_SetColor(0xffff);
//        LCD_NFDispStringAt(84, 112, "System Reloading...");
//#ifdef LCD_MEMORY_DEV 
//        DmaFillImage2Lcd();
//#endif
        SysCpuSetFreq(72);
        sysareacopy();
        //SysCpuSetFreq(24);
     }
     //LCD_SetColor(0x0);
#endif  
    
#if(CPU_TYPE == _RK2608_)
    write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x77) | 0x08);
#endif

    SystemInfo.bc.bSysIdleEn = 1;
    PeriphFlag.bc.bEarStatusCheckEn = 1;
    
    SysCpuSetFreq(24);
    SysTimer0Set(24,25000);
    
    TimerInit();
    bitset_creg(%imask, 5);
}
/*******************************************************************************
                         ** MAIN LOOP **
* Description: 
* Function   : 
* 
*******************************************************************************/
SYSTEM_TEXT_SECTION
int main(void)
{
//-----------------------------异常时，固件升级--------------------------
{
//当固件部分丢失造成没法正常升级时，只要程序能够运行到下面代码中就
//可以固件升级。判断条件不要使用调函数方式，比如GetKeyVal()，因为
//调用的函数可能已经丢失了。注：USB_PLUGIN()为宏。 
    write_mem(GPIO_PCON0,0x6000);//
    if ((!USB_INSERT())&&((~read_XDATA(KEYBOARD) & 0x7F) == 0x08)){
        //p0.6（play）接高 和 p0.4接地
	    void(*fp)(void);
	    asm ("bitc %smode,7");	    //uvt at internal ram 0xf800
	    asm ("mov r12,0xbffe");		//uvt at internal ram 0xf800
	    write_XDATA(0xb006, 0x00); 
	    fp=(void*)0xfef4;			
	    (*fp)();
	    while (1);
    }
}
//---------------------------------------------------------------------
    CurrCpuFreq = 24;
    Delay1Ms(100);
    
    SysCpuSetFreq(48);
    
    PowerOnCpuInit();
    
    PowerOnVariableInit();
    AdcSampleInit();
    
    if(!USB_INSERT()) {
        if (BatteryAdcData < BATTERY_LEVEL_LOW + 20) {
            write_mem(GPIO_PDAT0,read_mem(GPIO_PDAT0) & 0xff7f);
            while(1);
        }
    }
    
    SDRAM_Init();
    DmaInit(); 
       
    KeyInit();
    MainMenuInit();

    SystemInit();
   
#ifdef FM_MODULE
    ModuleOverlay(FM_CODE_MODULE_NUM);
    Tuner_MuteControl(1);
    Tuner_PowerDown();
#endif

    while(1)
    {
        ProgramSchedule1Module();
        ProgramSchedule2Module();
        
        Function1Module();
        Function2Module();
        
        MainMenuProc();
        
        TimerProc();
        
        PeripheralDetectModule();
#ifdef LCD_MEMORY_DEV 
        DmaFillImage2Lcd();
#endif
    }
}
//*******************************************************************************
