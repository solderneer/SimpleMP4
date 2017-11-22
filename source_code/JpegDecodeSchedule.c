/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : JpegDecodeSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#define _IN_JPEGDECODESCHEDULE

#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "Global.h"
#include "Resource.h"
#include "BrowserMacro.h"
#include "JpegDecodeGlobal.h"
#include "JpgDec.h"

#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "Lcd.h"

void JpegDecodeInitial(void);
void JpegKeyProc(void);


/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".jpeg_text, \"ax\"")))
void JpegTimeProc(void *parg)
{
    if(JpegAutoPlayFlag != 0){
        TimerReset(JPEG_TMR_CHANNEL);
        TimerStart(JPEG_TMR_CHANNEL);
        JpegAutoBrowserTimeFlag++;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".jpeg_text, \"ax\"")))
void JpegTimeReset(void *parg)
{
    if(JpegAutoPlayFlag != 0){
        TimerReset(JPEG_TMR_CHANNEL);
        TimerStart(JPEG_TMR_CHANNEL);
        JpegAutoBrowserTimeFlag = 0;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".jpeg_text, \"ax\"")))
void JpegAutoPlaySetup(void)
{
    unsigned int JpegAutoPlayTimeTable[5] = {1*40,2*40,3*40,4*40,5*40};
    
    if (JpegAutoPlayFlag != 0) {
        TimerCfgFunc(JPEG_TMR_CHANNEL, JpegTimeProc, 0);
        TimerSetTimer(JPEG_TMR_CHANNEL, JpegAutoPlayTimeTable[gJpegAutoBrowserTime]);
        TimerStart(JPEG_TMR_CHANNEL);
        JpegAutoBrowserTimeFlag = 1;
    } else {
        JpegAutoBrowserTimeFlag = 0;
        TimerStop(JPEG_TMR_CHANNEL);
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".jpeg_text, \"ax\"")))
void JpegModuleProc(void)
{
    switch (Fun2MsgArray[1])
    {
    case MSG_FUNCTION2_JPEG_DECODE_INIT:
        JpegDecodeInitial();
        break;
        
    case MSG_FUNCTION2_JPEG_DECODE_HEAD:
#if(FLASHROM_TYPE == _MLC_)
        if(Sys1InitFlag.word == 0xffff) {
            MlcDataRefresh();           //如果单独JPEG工作而Function1不工作的话，需要在适当的时候添加该语句
        }
#endif
        JpegDecodeHead();
        break;
        
    case MSG_FUNCTION2_JPEG_DECODE_PLAY:
        if ((SysFile2Info.Fdt.Name[8] == 0x42/*'B'*/) &&
            (SysFile2Info.Fdt.Name[9] == 0x4d/*'M'*/) &&
            (SysFile2Info.Fdt.Name[10] == 0x50/*'P'*/)) {
            BmpDecoder();
        } else {
            JpegDecodePlay();
        }
        KeyReset();//解决快速按FFD出现解码变慢的问题
        break;
        
    case MSG_FUNCTION2_JPEG_DECODE_END:
        JpegDecodeEnd();
        break;
        
    case MSG_FUNCTION2_JPEG_AUTO_PLAY:
    default:
    {
        if((JpegAutoPlayFlag != 0) && (JpegAutoBrowserTimeFlag != 0)){
            JpegAutoBrowserTimeFlag = 0;
            
            if (JpegFileFormatError == 1) {
                JpegFileFormatError = 0;
                KeyReset();
            }
            PostFun2Message(MSG_FUNCTION2_FILE_FIND,MSG_FIND_JPEG_FILE,
                            MSG_FIND_FILE_NEXT,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
        }
        break;
    }
    }
    
    if (JpegFileFormatError == 1) {
        if (/*(DisplayFlag != DISP_JPEG) || */(SystemInfo.bc.bDisplayRecovery)/* || (SystemInfo.bc.bBackLightStatus == 2)*/) {
            //文件错误，显示对话框结束，进入下一首歌曲
            SystemInfo.bc.bDisplayRecovery = 0;
            //JpegDecodeEnd();
            PostFun2Message(MSG_FUNCTION2_FILE_FIND,MSG_FIND_JPEG_FILE,MSG_FIND_FILE_NEXT,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
            return;
        }
    }
    
    if (DisplayFlag == DISP_JPEG) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            JpegKeyProc();
            
            if (SystemInfo.bc.bBackLightOnStart1) {
                SystemInfo.bc.bBackLightOnStart1 = 0;
                DmaFillImage2Lcd();
                LcdBackLightOn();
                KeyReset();
            }
        } else {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 0;
            bitset_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart1 = 1;
            LCD_SDRAM_Standby_Return();
        }
        
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            MusicDisplayBlock.DisplayFlag.bit.bBatt = 1;
        }
        
        if (SystemInfo.bc.bHoldDisplay) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bHoldDisplay = 0;
            bitset_creg(%imask,5);
            
            DisplayRecoveryTime = 0;
            PreDisplayFlag = DisplayFlag;
            DisplayFlag = DISP_HOLD;
            
            MusicSetFreq51();
            LCD_Init(0/*LCD_INIT_NORMAL*/);
            LCD_SetDiaplayMode(0);
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MAX_PIC_SIZE);
            MusicSetFreq51Return();
            return;
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {
            SystemInfo.bc.bDisplayRecovery = 0;
            //恢复显示图片(重新解码)
            PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_DECODE_INIT,0,0,0/*,0,0,0*/);
        }
        
//        if (SystemInfo.bc.bBackLightOnStart1) {
//            SystemInfo.bc.bBackLightOnStart1 = 0;
//            DmaFillImage2Lcd();
//            LcdBackLightOn();
//            KeyReset();
//        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".jpeg_text, \"ax\"")))

void JpegDecodeInitial(void)
{
    if (Sys2InitFlag.bc.bJpegSysInitEnable){
        while(DmaFillImage2LcdFlag);
        
        bitclear_creg(%imask,15);
        
        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0010);
        write_mem32(DMAR_ClearBlock,0x00000007);
        
        bitset_creg(%imask,7);
        bitset_creg(%imask,15);
        
        Sys2InitFlag.word = 0xffff;
        Sys2InitFlag.bc.bJpegSysInitEnable = 0;
        Sys2InitFlag.bc.bJpegModuleInitEnable = 0;
        
        JpegAutoPlayFlag = 0;
        JpegAutoBrowserTimeFlag = 0;
    }
    KeyReset();
    JpeaDmaTanFlag = 0;
    JpegTanCont    = 0;
    JpegFileFormatError = 0;
    PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_DECODE_HEAD,0,0,0/*,0,0,0*/);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".jpeg_text, \"ax\"")))

void JpegKeyProc(void)
{
    unsigned int KeyTemp;
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
    
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
        
    if (JpegFileFormatError == 1) {
        KeyTemp = KeyVal & 0x1fff;
        if (KeyTemp = KEY_VAL_MENU) {
            DisplayRecoveryTime = 120;//300;
            SystemInfo.bc.bDisplayRecovery = 0;
        } else if (KeyTemp == KEY_VAL_FFW) {
            KeyVal = KEY_VAL_FFW_SHORT_UP;
        } else /*if (KeyTemp == KEY_VAL_FFD)*/ {
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
        JpegAutoPlayFlag = 1 - JpegAutoPlayFlag;
        JpegAutoPlaySetup();
        break;
    }
//--------------------- REC KEY ---------------------------
    /*
    case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS_START:
    case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_SHORT_UP:
    case KEY_VAL_REC_LONG_UP:
        break;
    */
//--------------------- MENU KEY ---------------------------
    /*&case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_DOWN:
    case KEY_VAL_MENU_LONG_UP:*/
    case KEY_VAL_MENU_PRESS_START:
    {
        TimerStop(JPEG_TMR_CHANNEL);
        
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        LCD_Init(0/*LCD_INIT_NORMAL*/);
        //PostFun2Message(MSG_FUNCTION2_MAIN_MENU,MSG_FUNCTION2_MAIN_MENU_INIT,0,0,0,0,0,0);
        break;
    }
    case KEY_VAL_MENU_SHORT_UP:
    {
        TimerStop(JPEG_TMR_CHANNEL);
        
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        BrowserDirInit();//zyf  2006-01-13
        DisplayFlag = DISP_BROWSER;
        //PostKeyMessage(MSG_KEY_RUN, MENU_BROWSER_INTERFACE_ID, 1, 0, 0, 0, 0, 0);
        PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,FIND_FILE_TYPE_JPEG,0,0/*,0,0,0*/);
        LCD_Init(0/*LCD_INIT_NORMAL*/);
        break;
    }
//--------------------- UP KEY ---------------------------
    /*    
    case KEY_VAL_UP_DOWN:
    case KEY_VAL_UP_PRESS_START:
    case KEY_VAL_UP_PRESS:
    case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_LONG_UP:
        break;*/
//--------------------- DOWN KEY ---------------------------
    /*case KEY_VAL_DOWN_DOWN:
    case KEY_VAL_DOWN_PRESS_START:
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_LONG_UP:
        break;*/
//--------------------- FFD KEY ---------------------------
    /*case KEY_VAL_FFD_PRESS_START:
    case KEY_VAL_FFD_SHORT_UP:
    case KEY_VAL_FFD_PRESS:
    case KEY_VAL_FFD_LONG_UP:
        break;*/
    case KEY_VAL_FFD_DOWN:
    {
        if (JpegFileFormatError == 1) {
            JpegFileFormatError = 0;
            KeyReset();
        }
        PostFun2Message(MSG_FUNCTION2_FILE_FIND,MSG_FIND_JPEG_FILE,MSG_FIND_FILE_NEXT,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
        break;
    }
//--------------------- FFW KEY ---------------------------
    /*case KEY_VAL_FFW_PRESS_START:
    case KEY_VAL_FFW_SHORT_UP:
    case KEY_VAL_FFW_PRESS:
    case KEY_VAL_FFW_LONG_UP:*/
    case KEY_VAL_FFW_DOWN:
    {
        if (JpegFileFormatError == 1) {
            JpegFileFormatError = 0;
            KeyReset();
        }
        PostFun2Message(MSG_FUNCTION2_FILE_FIND,MSG_FIND_JPEG_FILE,MSG_FIND_FILE_PREV,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
        break;
    }
    default:
        break;
    }
}
//******************************************************************************
