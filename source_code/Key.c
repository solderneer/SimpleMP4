/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Key.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
#define _IN_KEY

#include "Global.h"
#include "MsgDef.h"
#include "Macro.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"

/*------------------------------------------------------------------------------
Function: 按键处理全局变量初始化
Input   : 
Output  : 
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void KeyInit(void)
{
    KeyVal = 0;
    PrevKeyVal = 0;
    KeyStatus = 0;
    KeyScanCnt = 0;
    KeyFlag.word = 0;
}

/*------------------------------------------------------------------------------
Function: 按键处理全局变量初始化
Input   : 
Output  : 
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void KeyReset(void)
{
    //KeyScanCnt = 0;
    KeyStatus = KEY_STATUS_NONE;
    KeyVal = KEY_VAL_NONE;
    KeyFlag.word = 0;
}

/*------------------------------------------------------------------------------
Function: 读取键值
Input   : 
Output  : 
------------------------------------------------------------------------------*/
//01001100
//00001110 -- > 00000111
//01000000 -- > 00001111
SYSTEM_TEXT_SECTION
unsigned int GetKeyVal(void)
{
    unsigned int KeyTemp;
    
    //KeyTemp = (~read_mem(KEY_PORT)) & 0x7f;
    KeyTemp = read_mem(KEY_PORT);
    KeyTemp = ((~KeyTemp) & 0x3f) |(KeyTemp & 0x40);
    
    return (KeyTemp);
}

/*------------------------------------------------------------------------------
Function: 按键扫描程序
Input   : 
Output  : 
说明：  1、确定按键状态及键值
        2、判断Hold状态及其处理
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void KeyScan(void)
{
    unsigned int TempKeyVal;
    
    if (KeyFlag.bc.bKeyProssed == 0){
        TempKeyVal = GetKeyVal();
        
        if (TempKeyVal != KEY_VAL_NONE) {
            
            LightOffCounter = 0;
            SysIdleCounter = 0;
                
            if (TempKeyVal != PrevKeyVal) {
                //-----------------------------------------
                if (KeyScanCnt > KEY_TIME_PRESS_START) {
                    KeyFlag.bc.bKeyProssed = 1;
                    KeyVal = (KEY_STATUS_LONG_UP << 13) | (KeyVal & 0x1fff);
                } else if (KeyScanCnt > KEY_TIME_DOWN) {
                    KeyFlag.bc.bKeyProssed = 1;
                    KeyVal = (KEY_STATUS_SHORT_UP << 13) | (KeyVal & 0x1fff);
                }
                //-----------------------------------------
                KeyScanCnt = 0;
                KeyStatus = KEY_STATUS_DOWN;
            } else {
                if (++KeyScanCnt == 0) {
                    KeyScanCnt = KEY_TIME_PRESS_START + KEY_TIME_PRESS_STEP;
                } else if (KeyScanCnt == KEY_TIME_DOWN) { 
                    if (SystemInfo.bc.bSysIdleStatus) {
                        SystemInfo.bc.bSysIdleStatus = 0;
            
                        SysCpuSetFreq(PrevCpuFreq);
                        SysTimer0Set(PrevCpuFreq,PrevTimer0Val);
                    }
                    
                    if (SystemInfo.bc.bBackLightStatus != 0) {
                        SystemInfo.bc.bBackLightOnStart = 1;
                    } else {
                        //如果背光关闭，按键之后应该先开背光
                        if (SystemInfo.bc.bHoldOn) {
                            if (DisplayRecoveryTime >= 80/*200*/) {
                                SystemInfo.bc.bHoldDisplay = 1;
                            }
                            DisplayRecoveryTime = 0;
                        }
                    }
                }
            }

            switch (KeyStatus){
            case KEY_STATUS_DOWN:
                if (KeyScanCnt == KEY_TIME_DOWN) { 
                    KeyFlag.bc.bKeyProssed = 1;
                    KeyVal = (KEY_STATUS_DOWN << 13) | TempKeyVal;
                    
                } else if (KeyScanCnt == KEY_TIME_PRESS_START) {
                    KeyStatus = KEY_STATUS_PRESS;
                    KeyFlag.bc.bKeyProssed = 1;
                    KeyVal = (KEY_STATUS_PRESS_START << 13) | TempKeyVal;
                }
                break;
                
            case KEY_STATUS_PRESS:
                if (((KeyScanCnt - KEY_TIME_PRESS_START) % KEY_TIME_PRESS_STEP) == 0 ) {
                    KeyFlag.bc.bKeyProssed = 1;
                    KeyVal = (KEY_STATUS_PRESS << 13) | TempKeyVal;
                }
                break;
                
            default:
                //;KeyScanCnt = 0;
                break;
            }
        } else {
            
            if (KeyScanCnt > KEY_TIME_PRESS_START) {
                KeyFlag.bc.bKeyProssed = 1;
                KeyVal = (KEY_STATUS_LONG_UP << 13) | (KeyVal & 0x1fff);
            } else if (KeyScanCnt > KEY_TIME_DOWN) {
                KeyFlag.bc.bKeyProssed = 1;
                KeyVal = (KEY_STATUS_SHORT_UP << 13) | (KeyVal & 0x1fff);
            }
            
            KeyScanCnt = 0;
            KeyStatus = KEY_STATUS_NONE;
            
        }
        PrevKeyVal = TempKeyVal;
    }
}

///*------------------------------------------------------------------------------
//Function: 按键扫描程序
//Input   : 
//Output  : 
//说明：  1、确定按键状态及键值
//        2、判断Hold状态及其处理
//------------------------------------------------------------------------------*/
//void KeyModuleProc(void)
//{
//    switch (DisplayFlag) {
//    case DISP_MENU:
//        if (MainMenuState == 1) {
//            MainMenuKey();
//        }
//        break;
//    
//    case DISP_MUSIC:
//        if (Fun1MsgArray[0] == MSG_FUNCTION1_MUSIC_MP3) {
//            Mp3KeyProc();
//        } else if (Fun1MsgArray[0] == MSG_FUNCTION1_MUSIC_WMA) {
//            WmaKeyProc();
//        } else if (Fun1MsgArray[0] == MSG_FUNCTION1_MUSIC_WAV) {
//            WavKeyProc();
//        }
//        break;
//        
//    case DISP_AVI:
//        AviKeyProc();
//        break;
//        
//    case DISP_ENCODE:
//        break;
//        
//    case DISP_BROWSER:
//        break;
//        
//    case DISP_SETTING:
//        break;
//        
//    case DISP_JPEG:
//        break;
//        
//    case DISP_TEXT:
//        break;
//    /*
//    case DISP_GAME:
//    case DISP_FM:
//    case DISP_BATTERY:
//    case DISP_USB:
//    case DISP_HOLD:
//    */
//    default:
//        if (KeyFlag.bc.bKeyProssed) {
//            KeyFlag.bc.bKeyProssed = 0;
//        }
//        break;
//    }
//}
//******************************************************************************
