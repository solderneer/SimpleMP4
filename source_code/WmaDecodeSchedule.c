/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaDecodeSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*            HJL           05/12/08       1.1      Modified for being overlaid
********************************************************************************
*/
#define _IN_WMADECODESCHEDULE

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "Global.h"
#include "MsgDef.h"
#include "Resource.h"
#include "WmaDecodeMacro.h"
#include "BrowserMacro.h"
#include "BrowserGlobal.h"
#include "Equ_Config.h"
#include "Global.h"
#include "WmaDecodeGlobal.h"
#include "Message.h"
#include "WmaDecodeKey.h"
#include "Lcd.h"

void WMADecodeInitial(void);
void WMADecodeCPUInit(void);
void WMADecodeVariableInit(void);
void WMADecodeCodecInit(void);
void WMAKeyProc(void);
void WmaFfdFfwKeyStart(void);
void WmaFfdFfwKeyEnd(void);
void WmaDecodeEqSet(void);
void WmaCpuFreqSet(unsigned long SampleFreq);
void WmaSamplingFreqSet(unsigned long Freq);
void WmaRepeatModeSet(void);
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeProc(void)
{    
    // 解决当在一首歌切换到另一首歌时，这时如果进入音乐播放界面时会死机的问题。
    if (Fun1MsgArray[2] == MSG_FUNCTION1_WMA_DISPLAY_INIT) {
        Fun1MsgArray[2] = MSG_FUNCTION1_WMA_NONE;
        KeyReset();
        SystemInfo.bc.bDisplayRecovery = 1;
    }
    switch(Fun1MsgArray[1])
    {
    case MSG_FUNCTION1_WMA_INIT:
//        DisplayTestDecNum(4,4,1);
//        while(1);
        WMADecodeInitial();
        break;
        
    case MSG_FUNCTION1_WMA_HEAD:
        WMADecodeHead();
        break;
        
    case MSG_FUNCTION1_WMA_START:
        WMADecodeStart();
        AVI_MP3_DECODE_ENABLE;
        break;
        
    case MSG_FUNCTION1_WMA_PLAY:
        /*
        if (Fun1MsgArray[2] == MSG_FUNCTION1_WMA_DISPLAY_INIT) {
            Fun1MsgArray[2] = MSG_FUNCTION1_WMA_NONE;
            KeyReset();
            SystemInfo.bc.bDisplayRecovery = 1;
        }
        */
        WMADecodeTimeUpdate();
        break;
        
    case MSG_FUNCTION1_WMA_END:
        AVI_MP3_DECODE_DISABLE;
        WMADecodeEnd();
        break;
        
    default:
        break;
    }
    
    if (SystemInfo.bc.bMusicMenuSet == 1) {
        SystemInfo.bc.bMusicMenuSet = 0;
        
        WmaRepeatModeSet();
        
        memcpy(WMAEQMode.Usr_Def_Tab,GlobalMusicVar.UserEq,5);
        WmaDecodeEqSet();
        WmaCpuFreqSet(MusicPlayInfo.Freq);
    }
            
    if (WmaFileFormatError == 1) {
        if ((DisplayFlag != DISP_MUSIC) || (SystemInfo.bc.bDisplayRecovery) || (SystemInfo.bc.bBackLightStatus == 2)) {
            //文件错误，显示对话框结束，进入下一首歌曲
            WmaPlayFlag.bc.bPcmBufEmpty = 1;
            MusicDisplayInterface = 1;
            WMADecodeEnd();
            return;
        }
    }
    
    if (DisplayFlag == DISP_MUSIC) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
            if (SystemInfo.bc.bSysIdleStatus == 0) {
                WmaCpuFreqSet(MusicPlayInfo.Freq);
            }
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            WmaKeyProc();
        } else {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 0;
            bitset_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart1 = 1;
            LCD_SDRAM_Standby_Return();
        }
        
        if (SystemInfo.bc.bHoldDisplay) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bHoldDisplay = 0;
            bitset_creg(%imask,5);
            
            DisplayRecoveryTime = 0;
            PreDisplayFlag = DisplayFlag;
            DisplayFlag = DISP_HOLD;
            
            MusicSetFreq51();
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MAX_PIC_SIZE);
            MusicDispFlag |= MUSIC_DISPFLAG_HOLD;
            WmaCpuFreqSet(MusicPlayInfo.Freq);
            return;
        }
            
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            MusicDispFlag |= MUSIC_DISPFLAG_BATT;
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {           //add by zyz
            SystemInfo.bc.bDisplayRecovery = 0;
            
            MusicSetFreq51();         //add by zyz
            MusicDispFlag = (MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |
                             MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | MUSIC_DISPFLAG_EQ | MUSIC_DISPFLAG_AB |
                             MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | MUSIC_DISPFLAG_VOL | 
                             MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS | MUSIC_DISPFLAG_ORDER);
            WmaDisplayModeInit();
        }
            
        if ((MusicDispFlag != MUSIC_DISPFLAG_NULL) && (SystemInfo.bc.bBackLightStatus != 2)){  //Display Recovery
            
            DISP_WmaPlay();
            if (WmaFileFormatError == 1) {
                SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
                PreDisplayFlag = DISP_MUSIC;
                DisplayRecoveryTime = 0;
            }
            WmaCpuFreqSet(MusicPlayInfo.Freq);
        }
        
        if (SystemInfo.bc.bBackLightOnStart1) {             //Back Light On 
            SystemInfo.bc.bBackLightOnStart1 = 0;
            DmaFillImage2Lcd();
            LcdBackLightOn();
            KeyReset();
            WmaCpuFreqSet(MusicPlayInfo.Freq);
        }
    }
}
//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeInitial(void)
{
    MusicPlayInfo.DispState = MUSIC_DISPLAY;	// 解决当一首不支持的音乐文件与另一首支持且带LRC的音乐文件相
                                                // 邻时，由LRC文件切到不支持的文件，然后再切回时频谱出现花屏 
    if (Sys1InitFlag.bc.bMusicModuleInitEnable){
        KeyReset();
    }
    if (Sys1InitFlag.bc.bWmaSysInitEnable) {
        WMADecodeCPUInit();
        WMADecodeVariableInit();
        WMADecodeCodecInit();
    }
    WmaFileFormatError = 0;
    MusicGuageBak      = 0;
    PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_HEAD,0,0,0/*,0,0,0*/);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeCPUInit(void)
{
    while(DmaFillImage2LcdFlag);
    
    bitclear_creg(%imask,15);
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0002);
    write_mem32(DMAR_ClearBlock,0x00000007);
    
    delay_nops(48000);
    write_creg(%ip0,0x0000);
    write_creg(%ip1,0x4800);
    write_creg(%ireq,0x0000);
    //bitset_creg(%imask,7);
    bitset_creg(%imask,15);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeVariableInit(void)
{
    Sys1InitFlag.word = 0xffff;
    
    Sys1InitFlag.bc.bWmaSysInitEnable = 0;
    Sys1InitFlag.bc.bMusicModuleInitEnable = 0;
    
    WmaPlayFlag.word = 0;
    
    memcpy(WMAEQMode.Usr_Def_Tab,GlobalMusicVar.UserEq,5);
        
    pWMAInputBuffer = WMABufferForEncode;
    FirstWMAPCMOutAddress = pWMAInputBuffer + INBUFFERSIZE;
    
    pWMAPCMOutputBuf = FirstWMAPCMOutAddress;
    pWMADecodeOutputBuf = FirstWMAPCMOutAddress + WMA_PCM_BUFFER_SIZE;
    
    MusicDispFlag = MUSIC_DISPFLAG_NULL;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeCodecInit(void)
{
    CodecIicInit();
    
#ifndef  _INSIDE_HPSENSE_USED_    
    CodecCommandSet(P_ADD_TR1,  0x0002);
#else 
    CodecCommandSet(P_ADD_TR1,  0x0000);
#endif
    CodecCommandSet(P_ADD_CCR,  0x0011);    //12MHZ,44.1KHZ/44.1KHZ
    CodecCommandSet(P_ADD_CR1,  0x0028);
    Pre_CR1_Value = 0x0008;
    CodecCommandSet(P_ADD_CR2,  0x0000);    //16BIT
//-----------------------------------------------------------------
#if(CPU_TYPE == _RK2608A_)
    CodecCommandSet(P_ADD_PMR1, 0x001f);    //All Power Down
#else
    CodecCommandSet(P_ADD_PMR1, 0x003f);    //All Power Down
#endif
    CodecCommandSet(P_ADD_PMR2, 0x0008);    //All Power Down
//-----------------------------------------------------------------
    Delay1Ms(100);
    write_mem(I2DSP_TXCONF,0x0008);         //Slave Mode, 16 bit, 44.1KHZ
    write_mem(I2DSP_TXCOM, 0x0000);         //Tx enable,I2Dsp enable,DMA request Enable   
    write_mem(I2DSP_DPCTRL,0x0001);         //Tx FIFO Reset
    CodecPowerOn();
}
/*------------------------------------------------------------------------------
Function: Rock2 Output Volume Set
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaSamplingFreqSet(unsigned long Freq)
{
    CodecIicInit();
        
    //write_mem(CLOCK_I2SMCLKCON, 0x0100);    //I2SMclk = 12MHZ 
        
    switch (Freq) {
    /*
    case 12000:
        write_mem(CLOCK_I2SMCLKCON, 0x0017);
        write_mem(I2DSP_TXCONF,0x0000);
        CodecCommandSet(P_ADD_CCR, 0x0080);
        break;
        
    case 24000:
        write_mem(CLOCK_I2SMCLKCON, 0x000b);
        write_mem(I2DSP_TXCONF,0x0000);
        CodecCommandSet(P_ADD_CCR, 0x0080);
        break;
    */
    case 48000:
        //write_mem(CLOCK_I2SMCLKCON, 0x0005);
        write_mem(I2DSP_TXCONF,0x0000);
        CodecCommandSet(P_ADD_CCR, 0x0080);
        break;
    /*
    case 8000:
        write_mem(I2DSP_TXCONF,0x0030);
        CodecCommandSet(P_ADD_CCR, 0x0066);
        break;
        
    case 11025:
        write_mem(I2DSP_TXCONF,0x0028);
        CodecCommandSet(P_ADD_CCR, 0x0055);
        break;
        
    case 16000:
        write_mem(I2DSP_TXCONF,0x0020);
        CodecCommandSet(P_ADD_CCR, 0x0044);
        break;
    */
    case 22050:
        write_mem(I2DSP_TXCONF,0x0018);
        CodecCommandSet(P_ADD_CCR, 0x0033);
        break;
    
    case 32000:
        write_mem(I2DSP_TXCONF,0x00010);
        CodecCommandSet(P_ADD_CCR, 0x0022);
        break;
        
    case 44100:
    default:
        write_mem(I2DSP_TXCONF,0x0008);
        CodecCommandSet(P_ADD_CCR, 0x0011);
        break;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaCpuFreqSet(unsigned long SampleFreq)
{
    unsigned int TempFreq;
    
    switch (SampleFreq) {
    /*
    case 12000:
    case 24000:
    */
    case 48000:
        I2sClkConSet(0x000b);
        if ((SystemInfo.bc.bBackLightStatus != 2) || (MusicPlayInfo.Eq == MUSIC_DECODE_EQ_3D)) {
            TempFreq = 51;
#ifdef PD_DRM
            if(g_WmaHas_DRM) TempFreq = 68;
#endif
        } else {
            TempFreq = 34;  //34
#ifdef PD_DRM
            if(g_WmaHas_DRM) TempFreq = 51;
#endif
        }
        break;
    /*
    case 8000:
    case 11025:
    case 16000:
    */
  /*case 22050:
    case 32000:
    case 44100:*/
    default:
        I2sClkConSet(0x0100);
        if ((SystemInfo.bc.bBackLightStatus != 2) || (MusicPlayInfo.Eq == MUSIC_DECODE_EQ_3D)) {
            TempFreq = 52;
#ifdef PD_DRM
            if(g_WmaHas_DRM) TempFreq = 60;
#endif
        } else {
            TempFreq = 40;
#ifdef PD_DRM
            if(g_WmaHas_DRM) TempFreq = 48;
#endif
        }
        break;
    }

    if ((Sys2InitFlag.bc.bGameModuleInitEnable != 0) || (CurrCpuFreq != 51)) {
        if ((CurrCpuFreq != TempFreq) && (SystemInfo.bc.bSysIdleStatus != 1)) {
            SysCpuSetFreq(TempFreq);
            SysTimer0Set(TempFreq,25000);//SysTimer0Set(TempFreq,10000);
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaRepeatModeSet(void) 
{
    unsigned int PrevRange;
    unsigned int RepeatMode = MusicPlayInfo.Mode;
    
    GetMusicPlayModeInfo(&PrevRange,&MusicPlayInfo.Order,&MusicPlayInfo.RepMode);       //Get PrevRange
    
    MusicPlayInfo.Order = GlobalMusicVar.Order;
    MusicPlayInfo.Eq = GlobalMusicVar.Eq;
    
    MusicPlayInfo.Mode = GlobalMusicVar.Mode;
    GetMusicPlayModeInfo(&MusicPlayInfo.Range,&MusicPlayInfo.Order,&MusicPlayInfo.RepMode);
    
    if ((MusicPlayInfo.Range == MSG_FIND_FILE_DIR) && (PrevRange != MSG_FIND_FILE_DIR)) {
        SysFile1Info.CurrentFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
        GetDirPath(SysFile1Info.Path);
        SysFile1Info.TotalFiles = GetTotalFiles(SysFile1Info.Path,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
        CreatRandomFileList(SysFile1Info.CurrentFileNum,SysFile1Info.TotalFiles,File1RandomBuffer);
    } else if (((MusicPlayInfo.Range == MSG_FIND_FILE_ALL) && (PrevRange != MSG_FIND_FILE_ALL)) || ((MusicPlayInfo.Range == MSG_FIND_FILE_ONE) && (PrevRange != MSG_FIND_FILE_ONE))) {
        SysFile1Info.CurrentFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
        SysFile1Info.CurrentFileNum = GetGlobeFileNum(SysFile1Info.CurrentFileNum,SysFile1Info.Path,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
        //SysFile1Info.TotalFiles = BuildDirInfo(MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
        SysFile1Info.TotalFiles = uiTotalMusicFileNum;//  hjl@2006-10-27 16:33 
        CreatRandomFileList(SysFile1Info.CurrentFileNum,SysFile1Info.TotalFiles,File1RandomBuffer);
    }
//#if 0    
    if (MusicPlayInfo.Mode == MUSIC_DECODE_BROWSE) {
        if (RepeatMode != MUSIC_DECODE_BROWSE) {
            WmaDecodeOutputDisable(1);
            FileClose(hMainFile);
            if (Fun2MsgArray[0] == MSG_FUNCTION2_LRC) {
                Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                Fun2ModuleChange();
            }
            SysFile1Info.TotalFiles = BuildDirInfo(MusicFileExtString);
            uiTotalMusicFileNum = SysFile1Info.TotalFiles;
            SysFile1Info.CurrentFileNum = 1;
            //------------------------------------------------------------------------------
            //(2006-12-29 16:22 Hjl)
                unsigned int tempFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
                GetDirPath(SysFile1Info.Path);  
                unsigned int FindFileResult = FindFirst(&SysFile1Info.Fdt, &FindDataPlay, SysFile1Info.Path, MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
                unsigned int temp1;
                for(temp1=1; temp1<tempFileNum; temp1++){
                    FindFileResult = FindNext(&SysFile1Info.Fdt, &FindDataPlay, MusicFileExtString);
                }
            //------------------------------------------------------------------------------
            CreatRandomFileList(SysFile1Info.CurrentFileNum,SysFile1Info.TotalFiles,File1RandomBuffer);
            PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_MUSIC_FILE,MSG_FIND_FILE_CURRENT,MSG_FIND_FILE_ALL,MSG_FIND_FILE_ORDER/*,0,0,0*/);
        }
    }
//#endif
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaDecodeEqSet(void)
{
    WMAEQMode.MODE = MusicEqTable[MusicPlayInfo.Eq];
    
    if (MusicPlayInfo.Eq == MUSIC_DECODE_EQ_3D) {
        if(WMAEQ3DInit((ghdr_parse.nSamplesPerSec)&0xFFFF , 5 , 0x1666 ) == -1)
        {
            ;//no 3d eq
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaToBrowser(unsigned int SelMode)
{
    unsigned int  FileNum;
    unsigned char DirPath[3 + MAX_DIR_DEPTH*12 + 1];
    
    //ScrollStringDispDisable();
    
    if (Fun2MsgArray[0] == MSG_FUNCTION2_LRC) {
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
    }
        
    if ((Fun2MsgArray[0] == MSG_FUNCTION2_NONE) && (DisplayFlag != DISP_MENU)) {
        DisplayFlag = DISP_BROWSER;
        PreDisplayFlag = DisplayFlag;
        LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
        PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,FIND_FILE_TYPE_MUSIC,0,0/*,0,0,0*/);
    }
    
    if(DirTreeInfo.CurPickFileType[0] != FIND_FILE_TYPE_MUSIC){
        DirValueInit();
        BrowserFileTypeInit(FIND_FILE_TYPE_MUSIC);
    }
    BrowserDirInit();
    
//    if (SelMode == TO_DEV_FIRST_FILE) {         //返回第一首歌曲
//        SysFile1Info.CurrentFileNum = 1;
//        SysFile1Info.TotalFiles = BuildDirInfo(MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
//        GetDirPath(DirPath);
//        BrowserReturnHook(DirPath, SysFile1Info.CurrentFileNum - 1, FIND_FILE_TYPE_MUSIC);
//        CreatRandomFileList(SysFile1Info.CurrentFileNum,SysFile1Info.TotalFiles,File1RandomBuffer);
//    } else {                                    //返回当前歌曲
//        if (MusicPlayInfo.Range != MSG_FIND_FILE_DIR) {
//            FileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
//            GetDirPath(DirPath);
//            BrowserReturnHook(DirPath, FileNum - 1, FIND_FILE_TYPE_MUSIC);
//        } else {
//            BrowserReturnHook(SysFile1Info.Path, SysFile1Info.CurrentFileNum - 1, FIND_FILE_TYPE_MUSIC);
//        }
//    }
}
//******************************************************************************
