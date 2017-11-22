/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#define _IN_AVIKEY
#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Global.h"
#include "Resource.h"
#include "AviMacro.h"
#include "AviGlobal.h"

#include "Config.h"
#include "AviFile.h"
#include "AviMp4Decode.h"
#include "AviMp3Decode.h"
#include "BrowserMacro.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"

#include "Lcd.h"
//********************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviStatusBarDispSet(unsigned int DispBits){
    unsigned int imask_tmp;
    
    read_creg(%imask, imask_tmp);
    bitclear_creg(%imask, 15);
    
    if(DispBits != AVI_STATUS_BAR_BATT_BMASK){
        if(uiAviStatusBarDispFlag == AVI_STATUS_BAR_NULL){
            uiAviStatusBarDispBits = AVI_STATUS_BAR_DISP_ALL;
        }else{
            uiAviStatusBarDispBits |= DispBits;
        }
        uiAviStatusBarTimeCont = 0;
    }else{
        if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL)
            uiAviStatusBarDispBits |= DispBits;
    }
    write_creg(%imask, imask_tmp);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviWaitForDmaDone(void)
{
    while(Dma_16x16Block);
    while(Dma_16x8Block);
    while(Dma_E2IBlock);
    while(Dma_E2IBlock_Y);
    while(Dma_E2IBlock_UV);
    while(Dma_I2EBlock);
    while(Dma_Rgb2Lcd);
    while(Dma_RGB_I2E);
    while(Dma_RGB_E2LCD);
    while(DmaFillImage2LcdFlag);
    DmaInit();
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviSpeedHookFunc(unsigned int FileSeekMode)
{
    AviPlayFlag.bc.bAviFindIFrameOK = 1;
    AviPlayFlag.bc.bAviFirstPause   = 1;
    
    AviPlayFlag.bc.bMp4DecodeInit   = 1;
    AviPlayFlag.bc.bMp4DecodeEnable = 1;
    NeedFillDataNum                 = 0;
    AviMp4FrameCount                = 0;
    FileSeek(AviVideoChunkOffset,FileSeekMode,hMainFile); 
    
    while(Dma_RGB_E2LCD);
    if(AviRgbSdramIndex == 0){
        LCDSdramIndex = AVI_RGB_SDRAM_BUF_CONT; 
    }else{
        LCDSdramIndex = AviRgbSdramIndex-1;
    }
    AviMp4FrameCount = 0;
}

__attribute__((section(".avi_v_text, \"ax\"")))
void AviKeyProc(void)
{
    unsigned int imask_tmp;
    
    unsigned int KeyTemp;
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
        
    if (SystemInfo.bc.bHoldOn) {
        KeyFlag.bc.bKeyProssed = 0;
        return;
    }
        
    if (AviFileFormatError == 1) {
        DisplayRecoveryTime = 120;
        
        KeyTemp = KeyVal & 0x1fff;
        if (KeyTemp == KEY_VAL_FFW) {
            KeyVal = KEY_VAL_FFW_SHORT_UP;
        } else if (KeyTemp != KEY_VAL_MENU) {
            KeyVal = KEY_VAL_FFD_SHORT_UP;
        }
    }
    switch (KeyVal) {
//--------------------- PLAY KEY --------------------------
    /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_LONG_UP:*/
    case KEY_VAL_PLAY_SHORT_UP:
    {
        if (AviPlayState == AVI_DECODE_PLAY) {
            AviPlayState = AVI_DECODE_PAUSE;
            AviWaitForDmaDone();
            AviPlayFlag.bc.bMp4DecodeEnable = 0;
            AviMP3DecodeOutputDisable(0);
        } else {
            AviPlayState = AVI_DECODE_PLAY;
            AviPlayFlag.bc.bMp4DecodeEnable = 1;
            CodecPowerOn();
            AviMP3DecodeOutputEnable();
        }
        AviStatusBarDispSet(AVI_STATUS_BAR_STATE_BMASK); 
        break;
    }
//---------------------- REC KEY --------------------------
    /*
    case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS_START:
    case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_SHORT_UP:
    case KEY_VAL_REC_LONG_UP:
        break;
    */
//--------------------- MENU KEY --------------------------
    /*case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_LONG_UP:
    case KEY_VAL_MENU_DOWN:*/
    case KEY_VAL_MENU_PRESS_START:
    {
        AviWaitForDmaDone();
        AviMp3DecodeEnd();
        
        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        Fun1ModuleChange();
        
        uiAviStatusBarDispFlag = AVI_STATUS_BAR_NULL;
        uiAviStatusBarDispBits = 0;
        uiAviLcdDmaDoneFlag    = 0;
        
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        
        SysCpuSetFreq(51);
        SysTimer0Set(51,25000);
        
        LCD_Init(0);
        break;
    }    
    case KEY_VAL_MENU_SHORT_UP:
    {
        AviWaitForDmaDone();
        AviMp3DecodeEnd();

        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        Fun1ModuleChange();

        uiAviStatusBarDispFlag = AVI_STATUS_BAR_NULL;
        uiAviStatusBarDispBits = 0;
        uiAviLcdDmaDoneFlag    = 0;

        BrowserDirInit();
        DisplayFlag = DISP_BROWSER;
        PreDisplayFlag = DisplayFlag;
        PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,FIND_FILE_TYPE_AVI,0,0/*,0,0,0*/);
        
        SysCpuSetFreq(51);
        SysTimer0Set(51,25000);
        
        LCD_Init(0);
        break;
    }
//--------------------- UP KEY ----------------------------
    /*case KEY_VAL_UP_PRESS_START:
    case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_LONG_UP:*/
    case KEY_VAL_UP_DOWN:
    case KEY_VAL_UP_PRESS:
    {
        AviPlayFlag.bc.bAviFirstPause = 1;
        OutputVolumeInc();
        AviStatusBarDispSet(AVI_STATUS_BAR_VOL_BMASK);            
        break;
    }
//------------------- DOWN KEY ----------------------------
    /*case KEY_VAL_DOWN_PRESS_START:
    case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_LONG_UP:*/
    case KEY_VAL_DOWN_DOWN:
    case KEY_VAL_DOWN_PRESS:
    {
        AviPlayFlag.bc.bAviFirstPause = 1;
        OutputVolumeDec();
        AviStatusBarDispSet(AVI_STATUS_BAR_VOL_BMASK);
        break;
    }
//-------------------- FFD KEY ----------------------------
    /*case KEY_VAL_FFD_DOWN:*/
    case KEY_VAL_FFD_LONG_UP:
    {
        SysIdleCounter = 0;
        if((AviPlayFlag.bc.bAviFirstLongDown == 1)&&(AviPlayFlag.bc.bAviFindIFrameOK == 1)){
            while(Dma_RGB_E2LCD);
            if(AviRgbSdramIndex == 0){
                LCDSdramIndex = AVI_RGB_SDRAM_BUF_CONT; 
            }else{
                LCDSdramIndex = AviRgbSdramIndex-1;
            }
            uiAviSpeedFlag   = 2;
            SynAudio2VideoChunk(&AviAudioChunkOffset, &AviAudioChunkLength, &AviCurr_IFrameNum_FrameNum, &AviLastChunkNum, &AviStreamInfo);
            FileSeek(AviAudioChunkOffset,SEEK_CUR,hSlaveFile);
        }
        AviPlayFlag.bc.bMp3DecodeInit = 1;
        break;
    }
    case KEY_VAL_FFD_PRESS_START:
    {
        AviMP3DecodeOutputDisable(0);
        AviLastChunkNum = AviCurrFrameNum;
        AviPlayFlag.bc.bAviFirstLongDown = 1;
    //    break;
    }
    case KEY_VAL_FFD_PRESS:
    {
        SysIdleCounter = 0;
        AviStatusBarDispSet(AVI_STATUS_BAR_JDT_BMASK | AVI_STATUS_BAR_TMR_BMASK);
        AVI_MP3_DECODE_DISABLE;
        if (-1 != GetNextVideoIFrameChunk( &AviVideoChunkOffset, &AviVideoChunkLength)) {
            AviSpeedHookFunc(SEEK_CUR);
            uiAviSpeedFlag   = 1;
            AVI_MP3_DECODE_ENABLE;
            break;
        }
        AviPlayFlag.bc.bAviFindIFrameOK = 0;
        KeyReset();
    }
    case KEY_VAL_FFD_SHORT_UP:
    {
        AviWaitForDmaDone();
        
        AviPlayFlag.bc.bAviFirstPause = 1;
        AviPlayFlag.bc.bAviFirstLongDown = 0;
        
        if (AviFileFormatError == 1) {
            AviFileFormatError = 0;
            KeyReset();
            MP4_LCD_Init();
        }
        
        AviDecodeEnd();
        break;
    }
//-------------------- FFW KEY ----------------------------
    /*case KEY_VAL_FFW_DOWN:*/
    case KEY_VAL_FFW_LONG_UP:
    {
        SysIdleCounter = 0;
        if((AviPlayFlag.bc.bAviFirstLongDown == 1)&&(AviPlayFlag.bc.bAviFindIFrameOK == 1)){
            while(Dma_RGB_E2LCD);
            if(AviRgbSdramIndex == 0){
                LCDSdramIndex = AVI_RGB_SDRAM_BUF_CONT; 
            }else{
                LCDSdramIndex = AviRgbSdramIndex-1;
            }
            uiAviSpeedFlag   = 2;
            SynAudio2VideoChunk(&AviAudioChunkOffset, &AviAudioChunkLength, &AviCurr_IFrameNum_FrameNum, &AviLastChunkNum, &AviStreamInfo);
            FileSeek(AviAudioChunkOffset,SEEK_SET,hSlaveFile);
        }
        AviPlayFlag.bc.bMp3DecodeInit = 1;
        break;
    }
    case KEY_VAL_FFW_PRESS_START:
    {
        AviMP3DecodeOutputDisable(0);   
        AviLastChunkNum = AviCurrFrameNum;
        AviPlayFlag.bc.bAviFirstLongDown = 1;         
        //break;
    }
    case KEY_VAL_FFW_PRESS:
    {
        SysIdleCounter = 0;
        AviStatusBarDispSet(AVI_STATUS_BAR_JDT_BMASK | AVI_STATUS_BAR_TMR_BMASK);
        AVI_MP3_DECODE_DISABLE;
        if (-1 != GetPreVideoIFrameChunk(&AviVideoChunkOffset, &AviVideoChunkLength)) {
            AviSpeedHookFunc(SEEK_SET);
            uiAviSpeedFlag   = 1;
            AVI_MP3_DECODE_ENABLE;
            break;
        }
        AviPlayFlag.bc.bAviFindIFrameOK = 0;
        KeyReset();
    }
    case KEY_VAL_FFW_SHORT_UP:
    {    
        AviWaitForDmaDone();
        
        //uiAviStatusTimeout = 0;
        AviPlayFlag.bc.bAviFirstPause = 1;
        
        if (AviFileFormatError == 1) {
            AviFileFormatError = 0;
            KeyReset();
            MP4_LCD_Init();
        }
        
        AviDecodeEnd();
        PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_AVI_FILE,MSG_FIND_FILE_PREV,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
        break;
    }    
    default:
        break;
    }
    KeyFlag.bc.bKeyProssed = 0;
}

//********************************************************************************
