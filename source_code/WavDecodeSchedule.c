/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WavDecodeSchedule.c
* Author     : zyf
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            zyf          2005-12-6      1.0            ORG
*
********************************************************************************
*/
#define _IN_WAVDECODESCHEDULE

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "WavDecodeMacro.h"
#include "KeyMacro.h"
#include "Global.h"
#include "WavDecodeGlobal.h"
#include "Resource.h"    
#include "BrowserMacro.h"
#include "BrowserGlobal.h"
#include "Key.h"
#include "Lcd.h"

void WavDecodeInitial(void);
void WavDecodeCPUInit(void);
void WavDecodeVariableInit(void);
void WavDecodeCodecInit(void);
void WavKeyProc(void);
void WavFfdFfwKeyStart(void);
void WavFfdFfwKeyEnd(void);
void WavCpuFreqSet(unsigned long SampleFreq);
void WavRepeatModeSet(void);
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeProc(void)
{    
    // 解决当在一首歌切换到另一首歌时，这时如果进入音乐播放界面时会死机的问题。
    if (Fun1MsgArray[2] == MSG_FUNCTION1_WAV_DISPLAY_INIT) {
        Fun1MsgArray[2] = MSG_FUNCTION1_WAV_NONE;
        KeyReset();
        SystemInfo.bc.bDisplayRecovery = 1;
    }
        
    switch(Fun1MsgArray[1])
    {
    case MSG_FUNCTION1_WAV_INIT:
        WavDecodeInitial();
        break;
        
    case MSG_FUNCTION1_WAV_HEAD:
        WavDecodeHead();
        break;
        
    case MSG_FUNCTION1_WAV_START:
        WavDecodeStart();
        AVI_MP3_DECODE_ENABLE;
        break;
        
    case MSG_FUNCTION1_WAV_PLAY:
        /*
        if (Fun1MsgArray[2] == MSG_FUNCTION1_WAV_DISPLAY_INIT) {
            Fun1MsgArray[2] = MSG_FUNCTION1_WAV_NONE;
            KeyReset();
            SystemInfo.bc.bDisplayRecovery = 1;
        }
        */
        WavDecodeTimeUpdate();
        break;
        
    case MSG_FUNCTION1_WAV_END:
        AVI_MP3_DECODE_DISABLE;
        WavDecodeEnd();
        break;
        
    default:
        break;
    }
    
    if (SystemInfo.bc.bMusicMenuSet == 1) {
        SystemInfo.bc.bMusicMenuSet = 0;
        WavRepeatModeSet();
    }
            
    if (WavFileFormatError == 1) {
        if ((DisplayFlag != DISP_MUSIC) || (SystemInfo.bc.bDisplayRecovery) || (SystemInfo.bc.bBackLightStatus == 2)) {
            //文件错误，显示对话框结束，进入下一首歌曲
            WavPlayFlag.bc.bPcmBufEmpty = 1;
            MusicDisplayInterface = 1;
            WavDecodeEnd();
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
                WavCpuFreqSet(MusicPlayInfo.Freq);
            }
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            WavKeyProc();
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
            WavCpuFreqSet(MusicPlayInfo.Freq);
            return;
        }
            
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            MusicDispFlag |= MUSIC_DISPFLAG_BATT;
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {           //add by zyz
            SystemInfo.bc.bDisplayRecovery = 0;
            
            MusicSetFreq51();
            MusicDispFlag = (MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |
                             MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | MUSIC_DISPFLAG_EQ | 
                             MUSIC_DISPFLAG_AB | MUSIC_DISPFLAG_SPECTRUM | MUSIC_DISPFLAG_FILENAME | 
                             MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | MUSIC_DISPFLAG_VOL | 
                             MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS | MUSIC_DISPFLAG_ORDER);
        }
        if ((MusicDispFlag != MUSIC_DISPFLAG_NULL) && (SystemInfo.bc.bBackLightStatus != 2)) {  //Display Recovery
            DISP_WavPlay();
            if (WavFileFormatError == 1) {
                SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
                PreDisplayFlag = DISP_MUSIC;
                DisplayRecoveryTime = 0;
            }
            WavCpuFreqSet(MusicPlayInfo.Freq);
        }
        
        if (SystemInfo.bc.bBackLightOnStart1) {             //Back Light On 
            SystemInfo.bc.bBackLightOnStart1 = 0;
            DmaFillImage2Lcd();
            LcdBackLightOn();
            KeyReset();
            WavCpuFreqSet(MusicPlayInfo.Freq);
        }
    }
}
//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeInitial(void)
{
    MusicPlayInfo.DispState = MUSIC_DISPLAY;	// 解决当一首不支持的音乐文件与另一首支持且带LRC的音乐文件相
                                                // 邻时，由LRC文件切到不支持的文件，然后再切回时频谱出现花屏 
    if (Sys1InitFlag.bc.bMusicModuleInitEnable){
        KeyReset();
//        MusicDispFlag = (MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_STATUS | MUSIC_DISPFLAG_VOL | MUSIC_DISPFLAG_BATT | MUSIC_DISPFLAG_BITRATE |
//                             MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_ORDER |MUSIC_DISPFLAG_EQ | MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_SPECTRUM);
    }
    if (Sys1InitFlag.bc.bWavSysInitEnable) {
        WavDecodeCPUInit();
        WavDecodeVariableInit();
        WavDecodeCodecInit();
    }
    WavFileFormatError = 0;
    PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_HEAD,0,0,0/*,0,0,0*/);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeCPUInit(void)
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
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeVariableInit(void)
{
    Sys1InitFlag.word = 0xffff;
    
    Sys1InitFlag.bc.bWavSysInitEnable = 0;
    Sys1InitFlag.bc.bMusicModuleInitEnable = 0;
    
    pWavPCMOutputBuf = FirstWavPCMOutAddress;
    pWavDecodeOutputBuf = FirstWavPCMOutAddress + WAVPCMBUFFERSIZE;
    
    MusicDispFlag = MUSIC_DISPFLAG_NULL;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeCodecInit(void)
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
__attribute__((section(".wav_text,\"ax\"")))
void WavSamplingFreqSet(unsigned long Freq)
{
    CodecIicInit();
    
    
    switch (Freq) {
    case 8000:
        write_mem(I2DSP_TXCONF,0x0030);
        CodecCommandSet(P_ADD_CCR, 0x0066);
        break;
        
    case 11025:
        write_mem(I2DSP_TXCONF,0x0028);
        CodecCommandSet(P_ADD_CCR, 0x0055);
        break;
        
    //case 12000:
    //    break;
    case 16000:
        write_mem(I2DSP_TXCONF,0x0020);
        CodecCommandSet(P_ADD_CCR, 0x0044);
        break;
        
    case 22050:
        write_mem(I2DSP_TXCONF,0x0018);
        CodecCommandSet(P_ADD_CCR, 0x0033);
        break;
        
    //case 24000:
    //    break;
        
    case 32000:
        write_mem(I2DSP_TXCONF,0x00010);
        CodecCommandSet(P_ADD_CCR, 0x0022);
        break;
        
    case 48000:
        write_mem(I2DSP_TXCONF,0x0000);
        CodecCommandSet(P_ADD_CCR, 0x0080);
        break;
        
    case 44100:
    default:
        write_mem(I2DSP_TXCONF,0x0008);
        CodecCommandSet(P_ADD_CCR, 0x0011);
        break;
    }
}
/*------------------------------------------------------------------------------
Function: Rock2 Output Volume Set
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavCpuFreqSet(unsigned long SampleFreq)
{
    unsigned int TempFreq;
    
    switch (SampleFreq) {
    /*case 12000:
    case 24000:*/
    case 48000:
        //write_mem(CLOCK_I2SMCLKCON, 0x0005);
        I2sClkConSet(0x000b);
        TempFreq = 34;
        break;
        
  /*case 8000:
    case 11025:
    case 16000:
    case 22050:
    case 32000:
    case 44100:*/
    default:
        //write_mem(CLOCK_I2SMCLKCON, 0x0100);    //I2SMclk = 12MHZ 
        I2sClkConSet(0x0100);
        TempFreq = 24;
        if (SystemInfo.bc.bBackLightStatus != 2) {
            TempFreq = 36;
        }
        break;
    }
    if ((Sys2InitFlag.bc.bGameModuleInitEnable != 0) || (CurrCpuFreq != 51)) {
        if ((CurrCpuFreq != TempFreq) && (SystemInfo.bc.bSysIdleStatus != 1)) {
            SysCpuSetFreq(TempFreq);
            SysTimer0Set(TempFreq,25000);
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavRepeatModeSet(void) 
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
            WavDecodeOutputDisable(1);
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
                unsigned int tempFileNum =GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
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
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavToBrowser(unsigned int SelMode)
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
