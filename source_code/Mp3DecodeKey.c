/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3DecodeKey.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
#define _IN_MP3DECODE_KEY

#include <creg.h>
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Resource.h"
#include "BrowserMacro.h"
#include "BrowserGlobal.h"
#include "Mp3DecodeGlobal.h"

#include "Message.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "Mp3_Decode.h"
#include "mp3_initial.h"

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3FfdFfwKeyStart(void)
{
    MP3DecodeOutputDisable(0);
    Mp3PlayFlag.bc.bMP3DecodeEnable = 0;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3FfdFfwKeyEnd(void)
{
    Mp3PlayFlag.bc.bMP3DecodeEnable = 0;
    if(MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
        Mp3PlayFlag.bc.bMP3DecodeEnable = 1;
        MP3DecodeOutputEnable();
    }
}

/*------------------------------------------------------------------------------
Function: 
Input   : 
Output  : 
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3KeyProc(void)
{
    unsigned int temp1, temp2, KeyTemp;
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
    
    if (MP3FileFormatError == 1) {
        DisplayRecoveryTime = 120;//300;
        KeyTemp = KeyVal & 0x1fff;
        if (KeyTemp == KEY_VAL_FFW) {
            KeyVal = KEY_VAL_FFW_SHORT_UP;
        } else if (KeyTemp != KEY_VAL_MENU) {
            KeyVal = KEY_VAL_FFD_SHORT_UP;
        }
    }
    
    switch (KeyVal) {
//--------------------- PLAY KEY ---------------------------
    /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_LONG_UP:*/
    case KEY_VAL_PLAY_SHORT_UP:                 //Play Pause
    {
        if(MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY){
            MusicPlayInfo.PlayState = MUSIC_DECODE_PAUSE;
            Mp3PlayFlag.bc.bMP3DecodeEnable = 0;
            MP3DecodeOutputDisable(0);
        } else {
            MusicPlayInfo.PlayState = MUSIC_DECODE_PLAY;
            Mp3PlayFlag.bc.bMP3DecodeEnable = 1;
            CodecPowerOn();
            MP3DecodeOutputEnable();
        }
        Mp3PlayStatus = MusicPlayInfo.PlayState;
        MusicDispFlag |= MUSIC_DISPFLAG_STATUS;
        break;
    }   
//--------------------- REC KEY ---------------------------
    /*case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_LONG_UP:
    case KEY_VAL_REC_DOWN:*/
    case KEY_VAL_REC_PRESS_START:               //ID3 Change
    {
        if (MusicPlayInfo.DispState == MUSIC_DISPLAY) {                    
            MusicPlayInfo.DispMode = ID3_DISPLAY;
        } else {
            MusicPlayInfo.DispMode = MUSIC_DISPLAY;
        }
        MusicPlayInfo.DispState = MusicPlayInfo.DispMode;
        Mp3DisplayModeInit();
        break;
    }    
    case KEY_VAL_REC_SHORT_UP:                  //AB Change
    {
        if (MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
            MusicDispFlag |= MUSIC_DISPFLAG_AB;
        	
        	if(MusicPlayInfo.AB == MUSIC_DECODE_ABOFF) {
        	    MusicPlayInfo.AB = MUSIC_DECODE_A_;
                MP3FileOffSetA = MP3FileOffSet;
            } else if(MusicPlayInfo.AB == MUSIC_DECODE_A_) {
                MusicPlayInfo.AB = MUSIC_DECODE_AB;
                MP3FileOffSetB= MP3FileOffSet;
            } else {
                MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
            }
        }
        break;
    }
//--------------------- MENU KEY ---------------------------
    /*case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_LONG_UP:
    case KEY_VAL_MENU_DOWN:*/
    case KEY_VAL_MENU_PRESS_START:              //Return To Main Menu
    {
        //ScrollStringDispDisable();
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        MainMenuState = 0;
        
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
            
        if (MP3FileFormatError == 1) {
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
        }
        break;
    }   
    case KEY_VAL_MENU_SHORT_UP:                 //Return File List
    {
        Mp3ToBrowser(TO_CURRENT_FILE);
        break;
    }   
//--------------------- UP KEY ---------------------------
    /*case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_LONG_UP:
    case KEY_VAL_UP_PRESS_START:*/
    case KEY_VAL_UP_DOWN:                       //Vol Up
    case KEY_VAL_UP_PRESS:
    {
        MusicDispFlag |= MUSIC_DISPFLAG_VOL;
        OutputVolumeInc();
        break;
    }
//--------------------- DOWN KEY -------------------------
    /*case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_LONG_UP:
    case KEY_VAL_DOWN_PRESS_START:*/
    case KEY_VAL_DOWN_DOWN:                     //Vol Down
    case KEY_VAL_DOWN_PRESS:                
    {
        MusicDispFlag |= MUSIC_DISPFLAG_VOL;
        OutputVolumeDec();
        break;
    }
//--------------------- FFD KEY -------------------------
    /*case KEY_VAL_FFD_DOWN:*/
    case KEY_VAL_FFD_PRESS_START:
    {
        Mp3FfdFfwKeyStart();
        
        Mp3PlayStatus = MUSIC_DECODE_FFD;
        MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
        MusicDispFlag |= MUSIC_DISPFLAG_AB;// | MUSIC_DISPFLAG_STATUS;
        break;
    }
    case KEY_VAL_FFD_LONG_UP:
    {
        Mp3FfdFfwKeyEnd();
        
        Mp3PlayStatus = MusicPlayInfo.PlayState;
        //MusicDispFlag |= MUSIC_DISPFLAG_STATUS;
    }
    case KEY_VAL_FFD_PRESS:
    {
        if (((long)SysFile1Info.Fdt.FileSize > (long)65536) &&          // 解决小于64KB的MP3文件快进不正常的问题 hjl@2006-12-21 15:43
            (MP3FileOffSet < (SysFile1Info.Fdt.FileSize - 65536))) {        
            MP3FileOffSet += 65536;
            FileSeek(MP3FileOffSet, SEEK_SET, hMainFile);
            Mp3PlayFlag.bc.bMP3DecodeInit = 1;
            
            MusicPlayInfo.TimeGuageVal = (unsigned int)(MP3FileOffSet* TIME_GUAGE_CONST / SysFile1Info.Fdt.FileSize);
            MP3TimeGuageCount = (unsigned long)(MP3FileOffSet % (SysFile1Info.Fdt.FileSize / TIME_GUAGE_CONST));
            
            MP3TimeCount = (unsigned long)(MP3FileOffSet % (((unsigned long)(MusicPlayInfo.FirstBitrate) * 125)/20));
            Mp3CurrTimeSec = (unsigned long)(MP3FileOffSet / ((unsigned long)(MusicPlayInfo.FirstBitrate) *125));
            
            MusicDispFlag |= (MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_FREE_TIME | MUSIC_DISPFLAG_GUAGE);
            break;
        } 
        KeyReset();
    }
    case KEY_VAL_FFD_SHORT_UP:
    {
        MP3DecodeOutputDisable(1);
        FileClose(hMainFile);
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        temp1 = MusicPlayInfo.Range;
        if (temp1 == MSG_FIND_FILE_ONE) {
            temp1 = MSG_FIND_FILE_ALL;
        }
        
        if (MP3FileFormatError == 1) {
            MP3FileFormatError = 0;
            MusicDisplayInterface = 1;
            KeyReset();
        }
        //ScrollStringDispDisable();
        PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_MUSIC_FILE,MSG_FIND_FILE_NEXT,temp1,MusicPlayInfo.Order/*,0,0,0*/);
        break;
    }
//--------------------- FFD KEY -------------------------
    /*case KEY_VAL_FFW_DOWN:*/
    case KEY_VAL_FFW_PRESS_START:
    {
        Mp3FfdFfwKeyStart();
        
        Mp3PlayStatus = MUSIC_DECODE_FFW;
        MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
        MusicDispFlag |= MUSIC_DISPFLAG_AB;// | MUSIC_DISPFLAG_STATUS;
        break;
    }
    case KEY_VAL_FFW_LONG_UP:
    {
        Mp3FfdFfwKeyEnd();
        
        Mp3PlayStatus = MusicPlayInfo.PlayState;
        //MusicDispFlag |= MUSIC_DISPFLAG_STATUS;
    }
    case KEY_VAL_FFW_PRESS:
    {
        if (MP3FileOffSet > 65536) {
            MP3FileOffSet -= 65536;
        	FileSeek(MP3FileOffSet, SEEK_SET, hMainFile);
        	Mp3PlayFlag.bc.bMP3DecodeInit = 1;
        	
         	MusicPlayInfo.TimeGuageVal = (unsigned int)(MP3FileOffSet* TIME_GUAGE_CONST / SysFile1Info.Fdt.FileSize);
            MP3TimeGuageCount = (unsigned long)(MP3FileOffSet % (SysFile1Info.Fdt.FileSize / TIME_GUAGE_CONST));
                
            MP3TimeCount = (unsigned long)(MP3FileOffSet % (((unsigned long)(MusicPlayInfo.FirstBitrate) * 125)/20));
            Mp3CurrTimeSec = (unsigned long)(MP3FileOffSet / ((unsigned long)(MusicPlayInfo.FirstBitrate) *125));
            
            MusicDispFlag |= (MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_FREE_TIME | MUSIC_DISPFLAG_GUAGE);
            break;
        }
        KeyReset();
    }
    case KEY_VAL_FFW_SHORT_UP:
    {
        MP3DecodeOutputDisable(1);
        FileClose(hMainFile);
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        temp1 = MusicPlayInfo.Range;
        if (temp1 == MSG_FIND_FILE_ONE) {
            temp1 = MSG_FIND_FILE_ALL;
        }
        
        if (MP3FileFormatError == 1) {
            MP3FileFormatError = 0;
            MusicDisplayInterface = 1;
            KeyReset();
        }
        //ScrollStringDispDisable();
        PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_MUSIC_FILE,MSG_FIND_FILE_PREV,temp1,MusicPlayInfo.Order/*,0,0,0*/);
        break;
    }    
    default:
        break;
    }
}
//******************************************************************************
