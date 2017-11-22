//******************************************************************************
//SysUSB.c          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#define _IN_FUNMODULE
//******************************************************************************
#include <Creg.h>
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Resource.h"

#include "Config.h"
#include "Message.h"
#include "FunUSB.h"
#include "FileModule.h"
#include "Lcd.h"
#include "LcdChar.h"

#ifdef MP3_DECODE_MODULE
#include "Mp3DecodeSchedule.h"
#include "Mp3Lrc.h"
#endif

#ifdef WMA_DECODE_MODULE
#include "WmaDecodeSchedule.h"
#include "WmaLrc.h"
#endif

#ifdef AVI_DECODE_MODULE
#include "AviSchedule.h"
#endif

#ifdef ENCODE_MODULE
#include "EncSchedule.h"
#endif

#ifdef JPEG_DECODE_MODULE
#include "JpegDecodeSchedule.h"
#endif

#ifdef BROWSER_MODULE
#include "BrowserSchedule.h"
#endif

#ifdef SETMENU_MODULE
#include "SetMenuSchedule.h"
#endif

#ifdef TXT_MODULE
#include "TxtSchedule.h"
#endif

#ifdef FM_MODULE
#include "FmSchedule.h"
#endif

#ifdef GAME_MODULE
#include "Block.h"
#endif


#ifdef MUSIC_MODULE
    void MusicModuleInit(void);
    void LrcModuleInit(void);
#endif

#ifdef AVI_DECODE_MODULE
    void AviModuleInit(void);
#endif

#ifdef JPEG_DECODE_MODULE
    void JpegModuleInit(void);
#endif    

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void Function1Module(void)
{
    unsigned int FunReturnValue;
    
    switch(Fun1MsgArray[0])
    {
        case MSG_FUNCTION1_FILE_FIND:           /*查找音乐、视频文件*/
            FindMusicFile(Fun1MsgArray[1],Fun1MsgArray[2],Fun1MsgArray[3],Fun1MsgArray[4]);
            break;
#ifdef MUSIC_MODULE     
       
        case MSG_FUNCTION1_MUSIC_CHECK:         /*音乐播放模块系统检测*/
            MusicModuleInit();
            break;
            
    #ifdef MP3_DECODE_MODULE             
        case MSG_FUNCTION1_MUSIC_MP3:           /*MP3 播放功能模块*/
            Mp3DecodeProc();
            break;
    #endif
    
    #ifdef WMA_DECODE_MODULE
        case MSG_FUNCTION1_MUSIC_WMA:           /*WMA 播放功能模块*/
            WMADecodeProc();
            break;
    #endif
    
    #ifdef WAV_DECODE_MODULE
        case MSG_FUNCTION1_MUSIC_WAV:           /*WAV 播放功能模块*/
    	    WavDecodeProc();	
            break;
    #endif
    
#endif

#ifdef AVI_DECODE_MODULE
        case MSG_FUNCTION1_AVI_CHECK:           /*视频播放模块系统检测*/
            //AviModuleInit();                
            break;
         case MSG_FUNCTION1_AVI_DECODE:         /*AVI 播放功能模块*/
            AviModuleProc();
            break;
#endif            
         
#ifdef ENCODE_MODULE            
        case MSG_FUNCTION1_ENCODE_PROC:          /*ENCODE Module Proc*/
            EncModuleProc();
            break;
#endif            

#ifdef FM_MODULE            
        case MSG_FUNCTION1_FM:                  /*FM ????*/
            FmModuleProc();
            break;
#endif
            
        case MSG_FUNCTION1_USB_INIT:            /*USB 系统初始化*/
            FunUSBInit();
            break;
        case MSG_FUNCTION1_USB_TRANS:           /*USB 数据传输*/
            FunUSBTrans();
            break;
        case MSG_FUNCTION1_UPGRADE_INIT:        /*UPGRADE 固件升级系统初始化*/
            FunUpgradeInit();
            break;
        case MSG_FUNCTION1_UPGRADE_TRANS:       /*UPGRADE 数据传输*/
            UpgradeUSBTrans();
            break;
        default:
            break;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void Function2Module(void)
{    
    switch(Fun2MsgArray[0])
    { 
        case MSG_FUNCTION2_FILE_FIND:
            FindDisplayFile(Fun2MsgArray[1],Fun2MsgArray[2],Fun2MsgArray[3],Fun2MsgArray[4]);
            break;
            
#ifdef MUSIC_MODULE
        case MSG_FUNCTION2_LRC:
            if (Sys1InitFlag.bc.bMusicModuleInitEnable == 0) {
                if (Sys1InitFlag.bc.bMp3SysInitEnable == 0) {
#ifdef MP3_DECODE_MODULE
                    Mp3LrcModuleProc();
#endif
                } else if (Sys1InitFlag.bc.bWmaSysInitEnable == 0) {
#ifdef WMA_DECODE_MODULE
                    WmaLrcModuleProc();
#endif
                }
            }
            break;
#endif

#ifdef JPEG_DECODE_MODULE
        case MSG_FUNCTION2_JPEG_DECODE:
            JpegModuleProc();
            break;
#endif

#ifdef BROWSER_MODULE         
        case MSG_FUNCTION2_BROWSER:
            BrowserModule();
            break;
#endif            
            
#ifdef SETMENU_MODULE    
        case MSG_FUNCTION2_MENU:
            SetMenuModule();
            break;
#endif            
      
#ifdef TXT_MODULE            
        case MSG_FUNCTION2_TXT:
            TextModule();
            break;
#endif            
     
#ifdef GAME_MODULE            
        case MSG_FUNCTION2_GAME:
            GameBlockModule();
            break;
#endif            
        default:
            break;
    }
}
//***************************************************************************************************
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void GetMusicPlayModeInfo(unsigned int *pRange, unsigned int *pOrder, unsigned int *pRepMode)
{
    if(MusicPlayInfo.Mode == MUSIC_DECODE_ONE || MusicPlayInfo.Mode == MUSIC_DECODE_ONE_REP)
        *pRange = MSG_FIND_FILE_ONE;
    else if((MusicPlayInfo.Mode == MUSIC_DECODE_DIR) || (MusicPlayInfo.Mode == MUSIC_DECODE_DIR_REP))
        *pRange = MSG_FIND_FILE_DIR;
    else if((MusicPlayInfo.Mode == MUSIC_DECODE_ALL) || (MusicPlayInfo.Mode == MUSIC_DECODE_ALL_REP) || (MusicPlayInfo.Mode == MUSIC_DECODE_BROWSE))
        *pRange = MSG_FIND_FILE_ALL;
    
//    if((MusicPlayInfo.Mode == MUSIC_DECODE_DIR_RANDOM) || (MusicPlayInfo.Mode == MUSIC_DECODE_ALL_RANDOM))
//        *pOrder = MSG_FIND_FILE_RANDOM;
//    else
//        *pOrder = MSG_FIND_FILE_ORDER;
    *pOrder = MusicPlayInfo.Order;
    
    if ((MusicPlayInfo.Mode == MUSIC_DECODE_ONE_REP) || (MusicPlayInfo.Mode == MUSIC_DECODE_DIR_REP) || (MusicPlayInfo.Mode == MUSIC_DECODE_ALL_REP)) {
        *pRepMode = MUSIC_REPMODE_REPEAT;
    } else {
        *pRepMode = MUSIC_REPMODE_NORMAL;
    }
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void MusicModuleInit(void)
{
    unsigned int temp1,temp2;
    
#ifdef SYSTEM_SD_CARD_SP      
    if(0 == (SysFile1Info.TotalFiles = BuildDirInfo(MusicFileExtString))){
        if (MemorySelect == 1) {
            MemorySelect = 0;
            MediaSetup(MemorySelect);
            FileInit();
        } else {
            if (SDCARD_INSERT()) {
                write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0004);
                Delay1Ms(10);
                MemorySelect = 1;
                if(MediaSetup(MemorySelect)) {
                    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffb);
                    MediaSetup(0);
                    MemorySelect = 0;
                }
                FileInit();
            }
        }
        if(0 == (SysFile1Info.TotalFiles = BuildDirInfo(MusicFileExtString))){
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            PreDisplayFlag = DISP_MENU;
            DisplayFlag = DISP_DIALOG;
            DisplayRecoveryTime = 0;
            SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_NOFILE_TXT);
            return;
        }
    }
#else 
    if(0 == (SysFile1Info.TotalFiles = BuildDirInfo(MusicFileExtString))){
       Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
       PreDisplayFlag = DISP_MENU;
       DisplayFlag = DISP_DIALOG;
       DisplayRecoveryTime = 0;
       SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_NOFILE_TXT);
       return;
    }
#endif 
    
    uiTotalMusicFileNum = SysFile1Info.TotalFiles;  // hjl@2006-10-27 16:28
    if (SysFile1Info.TotalFiles < GlobalMusicVar.MusicCurFileNum){
        GlobalMusicVar.MusicCurFileNum = SysFile1Info.TotalFiles;
    }
    SysFile1Info.CurrentFileNum = GlobalMusicVar.MusicCurFileNum;
    
    MusicDisplayInterface = 1;
    MusicPlayInfo.PlayState = MUSIC_DECODE_PLAY;
    //MusicPlayInfo.DispState = MUSIC_DISPLAY;
    
    GetMusicPlayModeInfo(&MusicPlayInfo.Range,&MusicPlayInfo.Order,&MusicPlayInfo.RepMode);
    
//    if (MusicPlayInfo.Range == MSG_FIND_FILE_DIR) {
//        SysFile1Info.CurrentFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString);
//        GetDirPath(SysFile1Info.Path);
//        SysFile1Info.TotalFiles = GetTotalFiles(SysFile1Info.Path,MusicFileExtString);
//    }
//------------------------------------------------------------------------------
//(2006-12-29 16:22 Hjl)
    unsigned int tempFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
    GetDirPath(SysFile1Info.Path);  
    if (MusicPlayInfo.Range == MSG_FIND_FILE_DIR) {
        SysFile1Info.CurrentFileNum = tempFileNum;
        SysFile1Info.TotalFiles = GetTotalFiles(SysFile1Info.Path,MusicFileExtString);
    }
    unsigned int FindFileResult = FindFirst(&SysFile1Info.Fdt, &FindDataPlay, SysFile1Info.Path, MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
    for(temp1=1; temp1<tempFileNum; temp1++){
        FindFileResult = FindNext(&SysFile1Info.Fdt, &FindDataPlay, MusicFileExtString);
    }
//------------------------------------------------------------------------------
        
    CreatRandomFileList(SysFile1Info.CurrentFileNum,SysFile1Info.TotalFiles,File1RandomBuffer);
    
    PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_MUSIC_FILE,MSG_FIND_FILE_CURRENT,MusicPlayInfo.Range,MusicPlayInfo.Order/*,0,0,0*/);
}

//***************************************************************************************************
