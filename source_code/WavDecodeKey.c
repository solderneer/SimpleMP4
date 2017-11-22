/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WavDecodeKey.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
#define _IN_WAVDECODE_KEY

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
#include "KeyMacro.h"
#include "KeyGlobal.h"

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavFfdFfwKeyStart()
{
    WavDecodeOutputDisable(0);
    WavPlayFlag.bc.bWavDecodeEnable = 0;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavFfdFfwKeyEnd()
{
    WavPlayFlag.bc.bWavDecodeEnable = 0;
    if(MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
        WavPlayFlag.bc.bWavDecodeEnable = 1;
        WavDecodeOutputEnable();
    }
}
/*------------------------------------------------------------------------------
Function: 按键扫描程序
Input   : 
Output  : 
说明：  1、确定按键状态及键值
        2、判断Hold状态及其处理
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavKeyProc(void)
{
    unsigned int temp1,temp2,KeyTemp;
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
        
    if (WavFileFormatError == 1) {
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
    case KEY_VAL_PLAY_SHORT_UP:
    {
        if(MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
    	    MusicPlayInfo.PlayState = MUSIC_DECODE_PAUSE;
    	    WavPlayFlag.bc.bWavDecodeEnable = 0;
    	    WavDecodeOutputDisable(0);
    	} else {
    	    MusicPlayInfo.PlayState = MUSIC_DECODE_PLAY;
    	    WavPlayFlag.bc.bWavDecodeEnable = 1;
    	    WavDecodeOutputEnable();
    	}
    	WavPlayStatus = MusicPlayInfo.PlayState;
        MusicDispFlag |= MUSIC_DISPFLAG_STATUS;
        break;
    }
//--------------------- REC KEY ---------------------------
    /*case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_LONG_UP:
    case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS_START:*/
    case KEY_VAL_REC_SHORT_UP:
    {
        if (MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
            MusicDispFlag |= MUSIC_DISPFLAG_AB;
        	if(MusicPlayInfo.AB == MUSIC_DECODE_ABOFF) {
        	    MusicPlayInfo.AB = MUSIC_DECODE_A_;
                WavFileOffSetA = WavFileOffSet;
            } else if(MusicPlayInfo.AB == MUSIC_DECODE_A_) {
                MusicPlayInfo.AB = MUSIC_DECODE_AB;
                WavFileOffSetB= WavFileOffSet;
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
    case KEY_VAL_MENU_PRESS_START:
    {
        //ScrollStringDispDisable();
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        MainMenuState = 0;
        
        if (WavFileFormatError == 1) {
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
        }
        break;
    }
    case KEY_VAL_MENU_SHORT_UP:
    {
        WavToBrowser(TO_CURRENT_FILE);
        break;
    }    
//--------------------- UP KEY ---------------------------
    /*case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_LONG_UP:
    case KEY_VAL_UP_PRESS_START:*/
    case KEY_VAL_UP_DOWN:
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
    case KEY_VAL_DOWN_DOWN:
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
        WavFfdFfwKeyStart();
        
        WavPlayStatus = MUSIC_DECODE_FFD;
        MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
        MusicDispFlag |= MUSIC_DISPFLAG_AB;// | MUSIC_DISPFLAG_STATUS;
        break;
    }
    case KEY_VAL_FFD_LONG_UP:
    {
        WavFfdFfwKeyEnd();
        
        WavPlayStatus = MusicPlayInfo.PlayState;
        //MusicDispFlag |= MUSIC_DISPFLAG_STATUS;
    }
    case KEY_VAL_FFD_PRESS:
    {
        AVI_MP3_DECODE_DISABLE;
        WavFileOffSet+=((long)gWavnAvgBytesPerSec);
        if((WavFileOffSet ) < SysFile1Info.Fdt.FileSize) {
            WavPlayFlag.bc.bWavDecodeInit = 1;
            AVI_MP3_DECODE_ENABLE;
            break;
        } 
        KeyReset();
    }
    case KEY_VAL_FFD_SHORT_UP:
    {
        WavDecodeOutputDisable(1);
    	FileClose(hMainFile);
    	
    	temp1 = MusicPlayInfo.Range;
        if (temp1 == MSG_FIND_FILE_ONE) {
            temp1 = MSG_FIND_FILE_ALL;
        }
	    if (WavFileFormatError == 1) {
            WavFileFormatError = 0;
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
        WavFfdFfwKeyStart();
        
        WavPlayStatus = MUSIC_DECODE_FFW;
        MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
        MusicDispFlag |= MUSIC_DISPFLAG_AB;// | MUSIC_DISPFLAG_STATUS;
        break;
    }
    case KEY_VAL_FFW_LONG_UP:
    {
        WavFfdFfwKeyEnd();
        
        WavPlayStatus = MusicPlayInfo.PlayState;
        //MusicDispFlag |= MUSIC_DISPFLAG_STATUS;
    }
    case KEY_VAL_FFW_PRESS:
    {
        AVI_MP3_DECODE_DISABLE;
        if(WavFileOffSet >=((long)gWavnAvgBytesPerSec + 512))
        {
            WavFileOffSet-=((long)gWavnAvgBytesPerSec );
            WavPlayFlag.bc.bWavDecodeInit = 1;
            AVI_MP3_DECODE_ENABLE;
            break;
        }
        KeyReset();
    }
    case KEY_VAL_FFW_SHORT_UP:
    {
        WavDecodeOutputDisable(1);
    	FileClose(hMainFile);
    	
        temp1 = MusicPlayInfo.Range;
        if (temp1 == MSG_FIND_FILE_ONE) {
            temp1 = MSG_FIND_FILE_ALL;
        }
        
    	if (WavFileFormatError == 1) {
            WavFileFormatError = 0;
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
