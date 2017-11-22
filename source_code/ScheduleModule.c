/*
********************************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  File name :     ScheduleModule.c
*  Author:         zyz, Jiliu Huang
*  Description:    模块调度功能
*  Others:         
*  History:        
*           <author>      <time>     <version>       <desc>
*           zyz          05/10/29      0.9           ORG
*           Jiliu Huang  05/11/01      1.0           添加ModuleOverlay函数，修改大部分函数，
*                                                    修正部分BUG
*           Jiliu Huang  06/02/06      1.1           修改调度方式,适应新的固件升级方式
********************************************************************************************
*/
#define  IN_SCHEDULEMODULE
//******************************************************************************

#include <creg.h>
#include "MsgDef.h"
#include "Macro.h"
#include "Global.h"
#include "Resource.h"
#include "Memap.h"
#include "Global.h"
#include "Config.h"
#include "Audio.h"
#include "FunUSB.h"
#include "Byte2Word.h"
#include "RockReadModule.h"
#include "Lcd.h"
#ifdef  MP3_DECODE_MODULE
    #include "Mp3DecodeGlobal.h"
#endif

#include "KeyStruct.h"
#include "KeyGlobal.h"

void Fun1ModuleChange(void);
void Fun2ModuleChange(void);
//---------------2006-2-5 15:44------------------------


/*------------------------------------------------------------------------------
Function: 主功能模块程序调度
Input   : 
Output  : 
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void ProgramSchedule1Module(void)
{
#ifdef  MODULE_OVERLAY
    unsigned int    ImaskTemp;
    
    if (PreviousScheduler1Msg != Sche1MsgArray[0]) {
        PreviousScheduler1Msg = Sche1MsgArray[0];

        read_creg(%imask,ImaskTemp);            //Disable Interrupt
        bitclear_creg(%imask,15);
        
        switch (Sche1MsgArray[0]) {
            case MSG_SCHEDULER1_MP3_DECODE:
                ModuleOverlay(MP3_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER1_WMA_DECODE:
                ModuleOverlay(WMA_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER1_WAV_DECODE:
                ModuleOverlay(WAV_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER1_AVI:
                ModuleOverlay(AVI_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER1_ENCODE:
                ModuleOverlay(ENC_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER1_FM:
				ModuleOverlay(FM_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER1_USB:
                ModuleOverlay(USB_CODE_MODULE_NUM); 
                break; 
            case MSG_SCHEDULER1_UPGRADE:
                ModuleOverlay(USB_CODE_MODULE_NUM); 
                break;
            default:
                break;
        }
        write_creg(%imask,ImaskTemp);           //Enable Interrupt
    }
#endif
}
/*------------------------------------------------------------------------------
Function: 从功能模块程序调度
Input   : 
Output  : 
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void ProgramSchedule2Module(void)
{
#ifdef  MODULE_OVERLAY
    unsigned int ImaskTemp;
    
    if (PreviousScheduler2Msg != Sche2MsgArray[0]) {
        PreviousScheduler2Msg = Sche2MsgArray[0];

        read_creg(%imask,ImaskTemp);            //Disable Interrupt
        bitclear_creg(%imask,15);
        MusicSetFreq51();
        
        switch (Sche2MsgArray[0]) {
            case MSG_SCHEDULER2_LRC:
                break;
            case MSG_SCHEDULER2_TXT:
                ModuleOverlay(TXT_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER2_JPEG:
                ModuleOverlay(JPEG_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER2_SETMENU:
                ModuleOverlay(SETMENU_CODE_MODULE_NUM);
                break;
            case MSG_SCHEDULER2_GAME:             
                ModuleOverlay(GAME_CODE_MODULE_NUM);
                break;
            default:
            break;
        }
        MusicSetFreq51Return();
        write_creg(%imask, ImaskTemp);           //Enable Interrupt
    }
#endif
}


/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".sys_init_text, \"ax\"")))
void LoadSysInformation(void)
{
    unsigned int i;
    unsigned int TempBuffer[128];
    unsigned long SourceAddr;// = 0x3a0000;

    GetResourceAddress(&ImageLogicAddress, &MenuLogicAddress,
                       &FontLogicAddress,  &GBKLogicAddress,
                       &SysInfomationAddress );

    SourceAddr = SysInfomationAddress;
    FlashReadLogic(SourceAddr, TempBuffer, 128);
//---------------Sys Global Varable-----------------------
//0--15
    OutputVolume = TempBuffer[0];                   //Vol
    if (OutputVolume > 40) OutputVolume = 20;
    
    LineInputVolume = TempBuffer[1];                //Line Vol
    if (LineInputVolume > 4) LineInputVolume = 3;
    
    MicInputVolume = TempBuffer[2];                 //Mic Vol
    if (MicInputVolume > 4) MicInputVolume = 3;
    
    PowerOffTime = TempBuffer[3];                   //Power Off TIme
    if (PowerOffTime > 5) PowerOffTime = 0;
    
    LightOffTime = TempBuffer[4];                   //BackLight Off Time
    if (LightOffTime > 5) LightOffTime = 4;
        
    LigthLevel = TempBuffer[5];                     //BackLigth Level
    if (LigthLevel > 4) LigthLevel = 2;
        
    ContrastLevel = TempBuffer[6];                  //Contrast Level
    if (ContrastLevel > 4) ContrastLevel = 3;
    
    DisplayMode = TempBuffer[7];                    //BackLight Display Mode
    if(DisplayMode > 1) DisplayMode = 0;
        
    Language = TempBuffer[8];                       //Language
    if(Language > (LANGUAGE_MAX_COUNT - 1)) Language = DEFAULT_LANGUE;
        
    MemorySelect = TempBuffer[9];                   //Memory Select
    if (MemorySelect > 1) MemorySelect = 0;
    
    FileFragCleanFlag = TempBuffer[10];             //File Frag Clean Flag
    if (FileFragCleanFlag > 1) FileFragCleanFlag = 1;
        
    ScrollSpeed = TempBuffer[11];                   //Scroll Speed
    if (ScrollSpeed > 4) ScrollSpeed = 3;
    
    gJpegAutoBrowserTime = TempBuffer[12];          //Jpeg Auto Run Speed
    if (gJpegAutoBrowserTime > 4) gJpegAutoBrowserTime = 2;
    
    gTxtAutoBrowserTime = TempBuffer[13];          //Text Auto Run Speed
    if (gTxtAutoBrowserTime > 4) gTxtAutoBrowserTime = 2;
        
//---------------Music Global Varable-----------------
//16--31
    GlobalMusicVar.MusicCurFileNum = TempBuffer[16];
    if (GlobalMusicVar.MusicCurFileNum == 0) GlobalMusicVar.MusicCurFileNum = 1;

    GlobalMusicVar.Mode = TempBuffer[17];
    if (GlobalMusicVar.Mode > MUSIC_DECODE_BROWSE) GlobalMusicVar.Mode = MUSIC_DECODE_ALL_REP;

    GlobalMusicVar.Eq = TempBuffer[18];
    if (GlobalMusicVar.Eq > MUSIC_DECODE_EQ_USER) GlobalMusicVar.Eq = MUSIC_DECODE_EQ_NORMAL;
    
    GlobalMusicVar.Order = TempBuffer[19];
    if (GlobalMusicVar.Order > MUSIC_DECODE_RANDOM) GlobalMusicVar.Order = MUSIC_DECODE_ORDER;
    
    MusicPlayInfo.Mode = GlobalMusicVar.Mode;
    MusicPlayInfo.Order = GlobalMusicVar.Order;
    MusicPlayInfo.Eq = GlobalMusicVar.Eq;
    
    GlobalMusicVar.UserEq[0] = TempBuffer[20];
    if (GlobalMusicVar.UserEq[0] > 7) GlobalMusicVar.UserEq[0] = 3;
    GlobalMusicVar.UserEq[1] = TempBuffer[21];
    if (GlobalMusicVar.UserEq[1] > 7) GlobalMusicVar.UserEq[1] = 3;
    GlobalMusicVar.UserEq[2] = TempBuffer[22];
    if (GlobalMusicVar.UserEq[2] > 7) GlobalMusicVar.UserEq[2] = 3;
    GlobalMusicVar.UserEq[3] = TempBuffer[23];
    if (GlobalMusicVar.UserEq[3] > 7) GlobalMusicVar.UserEq[3] = 3;
    GlobalMusicVar.UserEq[4] = TempBuffer[24];
    if (GlobalMusicVar.UserEq[4] > 7) GlobalMusicVar.UserEq[4] = 3;
    
    MusicPlayInfo.DispState = TempBuffer[25];
    if (MusicPlayInfo.DispState > ID3_DISPLAY) MusicPlayInfo.DispMode = MUSIC_DISPLAY;
//---------------Encode Global Varable-----------------
//32--39
    GlobalEncVar.MicQuality = TempBuffer[32];
    if (GlobalEncVar.MicQuality > 1) GlobalEncVar.MicQuality = 0;
    
    GlobalEncVar.LineInQuality = TempBuffer[33];
    if (GlobalEncVar.LineInQuality > 5) GlobalEncVar.LineInQuality = 2;
    
    GlobalEncVar.AutoDevideSection = TempBuffer[34];
    if (GlobalEncVar.AutoDevideSection > 1) GlobalEncVar.AutoDevideSection = 0;
    
//---------------Fm Global Varable---------------------
//40--127
    GlobalFmVar.FmArea = TempBuffer[40];
    if (GlobalFmVar.FmArea > 3) GlobalFmVar.FmArea = 0;
    
    GlobalFmVar.FmStereo = TempBuffer[41];
    if (GlobalFmVar.FmStereo > 1) GlobalFmVar.FmStereo = 0;
    
    GlobalFmVar.FmState = TempBuffer[42];
    if (GlobalFmVar.FmState > 1) GlobalFmVar.FmState = 0;
    
    GlobalFmVar.FmSaveNum = TempBuffer[43];
    if (GlobalFmVar.FmSaveNum >= FREQMAXNUMBLE) GlobalFmVar.FmSaveNum = 0;
    
    //--------------------Freq------------------------
    GlobalFmVar.FmFreq = (TempBuffer[46] << 8) + TempBuffer[47];
    for (i = 0; i < FREQMAXNUMBLE; i++) {                           //48--127
        GlobalFmVar.FmFreqArray[i] = (TempBuffer[48 + i*2] << 8) + TempBuffer[49 + i*2];
    }
//----------------------------------------------------
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void SaveSysInformation(void)
{
    unsigned int i;
    unsigned int TempBuffer[128];
    unsigned long SourceAddr;
    
//---------------Sys Global Varable-----------------------
//0--15
    TempBuffer[0] = OutputVolume;
    TempBuffer[1] = LineInputVolume;
    TempBuffer[2] = MicInputVolume;
                    
    TempBuffer[3] = PowerOffTime;
    
    TempBuffer[4] = LightOffTime;
    TempBuffer[5] = LigthLevel;
    TempBuffer[6] = ContrastLevel;
    TempBuffer[7] = DisplayMode ;
    
    TempBuffer[8] = Language;   
    
    TempBuffer[9] = MemorySelect;
    
    TempBuffer[10] = FileFragCleanFlag;
    
    TempBuffer[11] = ScrollSpeed;
    
    TempBuffer[12] = gJpegAutoBrowserTime;
    
    TempBuffer[13] = gTxtAutoBrowserTime;
    
//---------------Music Global Varable-----------------
//16--31
    TempBuffer[16] = GlobalMusicVar.MusicCurFileNum;
    TempBuffer[17] = GlobalMusicVar.Mode;
    TempBuffer[18] = GlobalMusicVar.Eq;
    TempBuffer[19] = GlobalMusicVar.Order;
                    
    TempBuffer[20] = GlobalMusicVar.UserEq[0];
    TempBuffer[21] = GlobalMusicVar.UserEq[1];
    TempBuffer[22] = GlobalMusicVar.UserEq[2];
    TempBuffer[23] = GlobalMusicVar.UserEq[3];
    TempBuffer[24] = GlobalMusicVar.UserEq[4];
    
    TempBuffer[25] = MusicPlayInfo.DispMode;

//---------------Encode Global Varable-----------------
//32--39
    TempBuffer[32] = GlobalEncVar.MicQuality;
    TempBuffer[33] = GlobalEncVar.LineInQuality;
    TempBuffer[34] = GlobalEncVar.AutoDevideSection;

//---------------Fm Global Varable---------------------
//40--127
    TempBuffer[40] = GlobalFmVar.FmArea;
    TempBuffer[41] = GlobalFmVar.FmStereo;
    TempBuffer[42] = GlobalFmVar.FmState;
    TempBuffer[43] = GlobalFmVar.FmSaveNum;
    
    TempBuffer[46] = GlobalFmVar.FmFreq >> 8;
    TempBuffer[47] = GlobalFmVar.FmFreq & 0xff;
    
    for (i = 0; i < FREQMAXNUMBLE; i++) {
        TempBuffer[48 + i*2] = GlobalFmVar.FmFreqArray[i] >> 8;
        TempBuffer[49 + i*2] = GlobalFmVar.FmFreqArray[i] & 0xff;
    }
//----------------------------------------------------
    SourceAddr = SysInfomationAddress;
    FLASH_PROTECT_OFF();
    FlashBlockErase(SourceAddr >> 9);
    FlashPageProgLogic(SourceAddr >> 9, 0, TempBuffer, 512);
    FLASH_PROTECT_ON();
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void WaitDamFinish(void)
{
    unsigned int imask_tmp;
    
    read_creg(%imask, imask_tmp);
    bitclear_creg(%imask, 15);  //关全局中断
    
    AVI_MP3_DECODE_REQ_CLR;     //清除 User 中断（音乐）
    imask_tmp &= 0xFFFE;        //    AVI_MP3_DECODE_DISABLE;
    
    write_mem32(DMAR_ChEnReg,0x00000400);  
    write_mem32(DMAR_MaskBlock,0x00000400); 
    write_mem32(DMAR_ClearBlock,0x00000004);
    
    write_creg(%imask, imask_tmp);
    
    MusicCodecMute(1);

    write_mem(I2DSP_TXCOM, 0x0000);         //Tx enable,I2Dsp enable,DMA request Enable 
    write_mem(I2DSP_RXCOM, 0x0000);         //Tx enable,I2Dsp enable,DMA request Enable 
}


SYSTEM_TEXT_SECTION
void Fun1ModuleChange(void)
{
    unsigned int imask_tmp;
    
    Sche1MsgArray[0] = MSG_SCHEDULER1_NONE;
    PreviousScheduler1Msg = MSG_SCHEDULER1_NONE;
  
//------------------------------------------------------------------------------    
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
    read_creg(%imask, imask_tmp);
    bitclear_creg(%imask, 15);
    DmaInit();
    write_creg(%imask, imask_tmp);
#endif
//------------------------------------------------------------------------------      
    WaitDamFinish();
    
    if (Sys1InitFlag.bc.bMusicModuleInitEnable == 0){
        GlobalMusicVar.Mode = MusicPlayInfo.Mode;
        GlobalMusicVar.Order = MusicPlayInfo.Order;
        GlobalMusicVar.Eq = MusicPlayInfo.Eq;
        GlobalMusicVar.MusicCurFileNum = SysFile1Info.CurrentFileNum;
        
        if ((GlobalMusicVar.Mode == MUSIC_DECODE_DIR) || (GlobalMusicVar.Mode == MUSIC_DECODE_DIR_REP)){
            GlobalMusicVar.MusicCurFileNum = GetGlobeFileNum(GlobalMusicVar.MusicCurFileNum,SysFile1Info.Path,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
        }
    
        FileClose(hMainFile);
            
        if (Sys2InitFlag.bc.bLrcModuleInitEnable == 0) {
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
            Fun2ModuleChange();
        }
        
        Sys1InitFlag.bc.bMp3SysInitEnable = 1;
        Sys1InitFlag.bc.bWmaSysInitEnable = 1;
        Sys1InitFlag.bc.bWavSysInitEnable = 1;
        Sys1InitFlag.bc.bMusicModuleInitEnable = 1;
        
        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffd);//~0x0002 //test Dma Lcd
    	    
    } else if (Sys1InitFlag.bc.bAviSysInitEnable == 0){
        
        FileClose(0);
        FileClose(1);
        FileClose(2);
        FileClose(3);

        Sys1InitFlag.bc.bAviSysInitEnable = 1;
        Sys1InitFlag.bc.bAviModuleInitEnable = 1;
        
//        MusicCodecMute(1);
        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffd);//~0x0002 //test Dma Lcd
        
    } else if (Sys1InitFlag.bc.bEncodeModuleInitEnable == 0){
#ifdef  ENCODE_MODULE
        ExitEncode();       //在录音时，有可能插入USB，需要保存录音文件信息
#endif
        Sys1InitFlag.bc.bLineInSysInitEnable = 1;
        Sys1InitFlag.bc.bMicRecSysInitEnable = 1;
        Sys1InitFlag.bc.bEncodeModuleInitEnable = 1;
        
        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffd);//~0x0002   //test Dma Lcd
        
    } else if (Sys1InitFlag.bc.bFmModuleInitEnable == 0){
        Sys1InitFlag.bc.bFmSysInitEnable = 1;
        Sys1InitFlag.bc.bFmModuleInitEnable = 1;
        
#ifdef FM_MODULE
        Tuner_MuteControl(1);
        Tuner_PowerDown();
#endif
        
        bitclear_creg(%imask,5);
        SystemInfo.bc.bPowerOffDisable = 0;
        bitset_creg(%imask,5);

        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffd);//~0x0012 //test Dma Lcd
        
    } else if (Sys1InitFlag.bc.bUsbModuleInitEnable == 0) {
        Sys1InitFlag.bc.bUsbModuleInitEnable = 1;
        
        bitclear_creg(%imask,10);
        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffa); //~0x0015
        write_XDATA(USB_ADDR, read_XDATA(USB_ADDR) & (~USB_HS_ENABLE));		//FULL SPEED
	    write_XDATA(USB_ADDR, read_XDATA(USB_ADDR) & (~USB_SOFTCONNECT));	//Disconnect
    }
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void Fun2ModuleChange(void)
{
    unsigned int imask_tmp;
    
    AVI_MP3_DECODE_DISABLE;
    
    Sche2MsgArray[0] = MSG_SCHEDULER2_NONE;
    PreviousScheduler2Msg = MSG_SCHEDULER2_NONE;

//------------------------------------------------------------------------------    
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
    read_creg(%imask, imask_tmp);
    bitclear_creg(%imask, 15);
    DmaInit();
    write_creg(%imask, imask_tmp);
#endif
//------------------------------------------------------------------------------ 

    if (Sys2InitFlag.bc.bJpegModuleInitEnable == 0){
        
        FileClose(hSlaveFile);
        Delay1Ms(10);
    	
    	Sche1MsgArray[0] = MSG_SCHEDULER1_NONE;
        PreviousScheduler1Msg = MSG_SCHEDULER1_NONE;
    	
        Sys2InitFlag.bc.bJpegSysInitEnable = 1;
        Sys2InitFlag.bc.bJpegModuleInitEnable = 1;
        
    } else if (Sys2InitFlag.bc.bTxtModuleInitEnable == 0){
        FileClose(hSlaveFile);
        Sys2InitFlag.bc.bTxtSysInitEnable = 1;
        Sys2InitFlag.bc.bTxtModuleInitEnable = 1;
    } else if (Sys2InitFlag.bc.bLrcModuleInitEnable == 0){
        FileClose(hSlaveFile);
        Sys2InitFlag.bc.bLrcSysInitEnable = 1;
        Sys2InitFlag.bc.bLrcModuleInitEnable = 1;
    } else if (Sys2InitFlag.bc.bBrowserModuleEnable == 0) {
        Sys2InitFlag.bc.bBrowserModuleEnable = 1;
    } else if (Sys2InitFlag.bc.bSetingMenuModuleEnable == 0) {
        Sys2InitFlag.bc.bSetingMenuModuleEnable = 1;
    } else if (Sys2InitFlag.bc.bGameModuleInitEnable == 0) {
        Sys2InitFlag.bc.bGameModuleInitEnable = 1;
        Sys2InitFlag.bc.bGameSysInitEnable = 1;
    }
    
    if (Sys1InitFlag.word != 0xffff){
        AVI_MP3_DECODE_ENABLE;
    } 
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void SysPowerDown(void)
{
    Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
    Fun1ModuleChange();
    Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
    Fun2ModuleChange();
    SaveSysInformation();
    
    if(USB_INSERT()) {
        PowerOffCounter = 0;
        DisplayFlag = DISP_BATTERY;
        PreDisplayFlag = DisplayFlag;
        SysBatteryFlag.bc.BatteryChargeEn = 1;
        LCD_Init(0);
        LCD_SetColor(0);
        LCD_SetDiaplayMode(0);
        AdcSampleInit();
        BatteryChargeStart();
        KeyReset();
    } else {
        AmplifierOff();
        //Delay1Ms(80);
        CodecPowerOff();

        LCD_Init(0);
        LCD_SetDiaplayMode(0);
        
        DisplayPowerOffFlash();
        SaveSysInformation();
        write_mem(GPIO_PDAT0,read_mem(GPIO_PDAT0) & 0xff7f);
        while(1);
    }
}

//******************************************************************************

