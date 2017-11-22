/*
********************************************************************************
*          Copyright (C),2004-2006, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FMKey.c
* Author     : HuangJiliu
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            HuangJiliu    06/03/24       1.0            ORG
*
********************************************************************************
*/


#include <creg.h>
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Resource.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "BrowserMacro.h"
#ifdef FM_MODULE

    #define _IN_FM_KEY
    #include "FmGlobal.h"



//******************************************************************************
/*
GlobalFmVar.FmState = 0     手动
GlobalFmVar.FmState = 1     自动
GlobalFmVar.FmState = 2     存台
GlobalFmVar.FmState = 3     预设
GlobalFmVar.FmState = 4     删除
GlobalFmVar.FmState = 255   菜单
*/
void FmFunKeyProc(void);
void FmMenuKeyProc(void);
void FmListKeyProc(void);
void FmDialogKeyProc(void);
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmKeyProc(void)
{
    if(KeyFlag.bc.bKeyProssed == 0) return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
    
    if (Fun1MsgArray[1] == MSG_FUNCTION1_FM_MENU) {
        FmMenuKeyProc();
    } else if ((Fun1MsgArray[1] == MSG_FUNCTION1_FM_SAVE) || (Fun1MsgArray[1] == MSG_FUNCTION1_FM_DEL)) {
        FmListKeyProc();
    } else if (Fun1MsgArray[1] == MSG_FUNCTION1_FM_DIALOG) {
        FmDialogKeyProc();
    } else {
        FmFunKeyProc();
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmFunKeyProc(void)
{
    unsigned int i;
    
    switch(KeyVal) {
//---------------------- REC KEY ---------------------------
  /*case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS_START:
    case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_LONG_UP:*/
    case KEY_VAL_REC_SHORT_UP:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        DisplayFlag = DISP_ENCODE;

        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        Sche1MsgArray[0] = MSG_SCHEDULER1_NONE;
        PreviousScheduler1Msg = MSG_SCHEDULER1_NONE;
        Sys1InitFlag.bc.bFmSysInitEnable = 1;
        //Fun1ModuleChange();
        
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        PostSche1Message(MSG_SCHEDULER1_ENCODE/*,0,0,0,0,0,0,0*/);
        PostFun1Message(MSG_FUNCTION1_ENCODE_PROC , MSG_FUNCTION1_ENC_INIT , ENCODE_FM_REC ,0,0/*,0,0,0*/);
        break;
    }
//--------------------- MENU KEY ---------------------------
    /*case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_LONG_UP:
    case KEY_VAL_MENU_DOWN:*/
    case KEY_VAL_MENU_PRESS_START:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        
        break;
    }
    case KEY_VAL_MENU_SHORT_UP:         //进入FM菜单
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        KeyReset();
        FmFunctionSele = GlobalFmVar.FmState;//FM_STATE_MANUAL;
        FmDisplayFlag |= (FM_DISPFLAG_SUBMENU_BK | FM_DISPFLAG_SUBMENU_SEL);
        PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_MENU,0,0,0,0,0,0);
        break;
    }
//--------------------- UP KEY -----------------------------
    case KEY_VAL_UP_DOWN:
    case KEY_VAL_UP_PRESS:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        FmDisplayFlag |= FM_DISPFLAG_VOL;//_INC;
        OutputVolumeInc();
        break;
    }
//--------------------- DOWN KEY ----------------------------
    case KEY_VAL_DOWN_DOWN:
    case KEY_VAL_DOWN_PRESS:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        FmDisplayFlag |= FM_DISPFLAG_VOL;//_DEC;
        OutputVolumeDec();
        break;
    }
//--------------------- FFD KEY ----------------------------
  /*case KEY_VAL_FFD_SHORT_UP:*/
    case KEY_VAL_FFD_DOWN:
    {
        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            bitclear_creg(%imask,5);
            FmTimerCount = 5;
            bitset_creg(%imask,5);
            
            Tuner_MuteControl(1);
            AmplifierOff();
            Tuner_FreqChange(TUN_UP,TUNERFREQSTEP);
            FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
            
            FmDisplayFlag |= (FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0/*,0,0,0*/);
            break;
        } 
        if (GlobalFmVar.FmState == FM_STATE_PRESET) {
            bitclear_creg(%imask,5);
            FmTimerCount = 20;
            bitset_creg(%imask,5);
            
            Tuner_MuteControl(1);
            AmplifierOff();
            GetNextStation();
            FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
            
            FmDisplayFlag |= (FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_CH);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0/*,0,0,0*/);
        }
        if (GlobalFmVar.FmState == FM_STATE_AUTO) {
            GlobalFmVar.FmState = FM_STATE_PRESET;
            
            bitclear_creg(%imask,5);
            FmTimerCount = 20;
            bitset_creg(%imask,5);
            
            GlobalFmVar.FmSaveNum = 0;
            GlobalFmVar.FmFreq = GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum];
            if((GlobalFmVar.FmFreq < FmFreqMinVal) || (GlobalFmVar.FmFreq > FmFreqMaxVal)){
                GlobalFmVar.FmFreq = FmFreqMinVal;   
            }
            
            FmDisplayFlag |= (FM_DISPFLAG_GUAGE | FM_DISPFLAG_FREQ | FM_DISPFLAG_CH);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0,0,0,0);
        }
        break;
    }
    case KEY_VAL_FFD_PRESS_START:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            Tuner_MuteControl(1);
            AmplifierOff();
            FmStereoState = 0;
            FmDisplayFlag |= FM_DISPFLAG_STEREO;
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_NONE,0,0,0/*,0,0,0*/);
        }
        break;
    }
    case KEY_VAL_FFD_PRESS:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            Tuner_FreqChange(TUN_UP,TUNERFREQSTEP);
            Tuner_PLLCount(GlobalFmVar.FmFreq,1);
            FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
            FmDisplayFlag |= (FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE);
        }
        break;
    }
    case KEY_VAL_FFD_LONG_UP:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            bitclear_creg(%imask,5);
            FmTimerCount = 0;
            bitset_creg(%imask,5);
            
            UpDownFlag = TUN_UP;
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_SEND_FREQHSI,0,0/*,0,0,0*/);
        }
        break;
    }
//--------------------- FFW KEY ----------------------------
  /*case KEY_VAL_FFW_SHORT_UP:*/
    case KEY_VAL_FFW_DOWN:
    {
        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            bitclear_creg(%imask,5);
            FmTimerCount = 5;
            bitset_creg(%imask,5);
            
            Tuner_MuteControl(1);
            AmplifierOff();
            
            Tuner_FreqChange(TUN_DOWN,TUNERFREQSTEP);
            FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
            
            FmDisplayFlag |= (FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0/*,0,0,0*/);
            break;
        } 
        if (GlobalFmVar.FmState == FM_STATE_PRESET) {
            bitclear_creg(%imask,5);
            FmTimerCount = 20;
            bitset_creg(%imask,5);
            
            Tuner_MuteControl(1);
            AmplifierOff();
            GetPrevStation();
            
            FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
            
            FmDisplayFlag |= (FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_CH);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0/*,0,0,0*/);
        }
        if (GlobalFmVar.FmState == FM_STATE_AUTO) {
            GlobalFmVar.FmState = FM_STATE_PRESET;
            
            bitclear_creg(%imask,5);
            FmTimerCount = 20;
            bitset_creg(%imask,5);
            
            GlobalFmVar.FmSaveNum = 0;
            GlobalFmVar.FmFreq = GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum];
            if((GlobalFmVar.FmFreq < FmFreqMinVal) || (GlobalFmVar.FmFreq > FmFreqMaxVal)){
                GlobalFmVar.FmFreq = FmFreqMinVal;   
            }
            
            FmDisplayFlag |= (FM_DISPFLAG_GUAGE | FM_DISPFLAG_FREQ | FM_DISPFLAG_CH);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0,0,0,0);
        }
        break;
    }
    case KEY_VAL_FFW_PRESS_START:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;

        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            Tuner_MuteControl(1);
            AmplifierOff();
            FmStereoState = 0;
            FmDisplayFlag |= FM_DISPFLAG_STEREO;
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_NONE,0,0,0/*,0,0,0*/);
        }
        break;
    }
    case KEY_VAL_FFW_PRESS:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
    
        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            Tuner_FreqChange(TUN_DOWN,TUNERFREQSTEP);
            Tuner_PLLCount(GlobalFmVar.FmFreq,1);
            FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
            FmDisplayFlag |= (FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE);
        }
        break;
    }
    case KEY_VAL_FFW_LONG_UP:
    {
        if (GlobalFmVar.FmState == FM_STATE_AUTO) break;
        
        if (GlobalFmVar.FmState == FM_STATE_MANUAL) {
            bitclear_creg(%imask,5);
            FmTimerCount = 0;
            bitset_creg(%imask,5);
            UpDownFlag = TUN_DOWN;
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_SEND_FREQHSI,0,0/*,0,0,0*/);
        }
        break;
    }
//--------------------- OTHER KEY --------------------------
    default:
        break;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmMenuKeyProc(void)
{
    unsigned int i;
    
    switch(KeyVal) {
//--------------------- PLAY KEY ---------------------------
  /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_SHORT_UP:
    case KEY_VAL_PLAY_LONG_UP:*/
//--------------------- MENU KEY ---------------------------
    /*case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_LONG_UP:
    case KEY_VAL_MENU_PRESS_START:
    case KEY_VAL_MENU_DOWN:*/
    case KEY_VAL_MENU_SHORT_UP:
    {
        KeyReset();
        if (FmFunctionSele == FM_STATE_AUTO) {          //自动
            GlobalFmVar.FmState = FM_STATE_AUTO;
            
            GlobalFmVar.FmSaveNum = 0;
            GlobalFmVar.FmFreq = FmFreqMinVal;
            for (i = 0; i < FREQMAXNUMBLE; i ++) {
                GlobalFmVar.FmFreqArray[i] = 0;
            }
            bitclear_creg(%imask,5);
            FmTimerCount = 0;
            bitset_creg(%imask,5);
            UpDownFlag = TUN_UP;
            Tuner_MuteControl(1);
            AmplifierOff();
            
            FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                             FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                             FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
                         
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_SEND_FREQHSI,0,0,0,0,0);
            break;
        } 
        
        if (FmFunctionSele == FM_STATE_MANUAL) {        //手动
            GlobalFmVar.FmState = FM_STATE_MANUAL;
            
            FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                             FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                             FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
                         
            PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_PLAY,0,0,0,0,0,0);
            break;
        } 
        
        if (FmFunctionSele == FM_STATE_PRESET) {        //预设
            GlobalFmVar.FmState = FM_STATE_PRESET;
            
            if (GlobalFmVar.FmSaveNum >= FREQMAXNUMBLE) GlobalFmVar.FmSaveNum = 0;
            
            if (GlobalFmVar.FmFreq == GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum]) {
                PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_PLAY,0,0,0,0,0,0);
            } else {
                GlobalFmVar.FmFreq = GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum];
                if((GlobalFmVar.FmFreq < FmFreqMinVal) || (GlobalFmVar.FmFreq > FmFreqMaxVal)){
                    GlobalFmVar.FmFreq = FmFreqMinVal;   
                }
                FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
                PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0,0,0,0);
            }
            
            FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                             FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                             FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
                         
            break;
        }
        
        CurCh  = GlobalFmVar.FmSaveNum;
        PrevCh = GlobalFmVar.FmSaveNum;
        
        FmDisplayFlag |= FM_DISPFLAG_LIST_BK;
        if (FmFunctionSele == FM_STATE_SAVE) {
            //GlobalFmVar.FmState = FM_STATE_SAVE;
            PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_SAVE,0,0,0,0,0,0);
            break;
        } else {
            //GlobalFmVar.FmState = FM_STATE_DEL;
            PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_DEL,0,0,0,0,0,0);
            break;
        }
        break;
    }
//--------------------- UP KEY ----------------------------
  /*case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_PRESS:
    case KEY_VAL_UP_LONG_UP:
    case KEY_VAL_UP_PRESS_START:*/
    case KEY_VAL_UP_DOWN:
    {
        if (FmFunctionSele == FM_STATE_MANUAL) {
            FmFunctionSele = FM_STATE_AUTO;
        } else {
            FmFunctionSele -- ;
        }
        FmDisplayFlag = FM_DISPFLAG_SUBMENU_SEL;
        break;
    }
//--------------------- DOWN KEY ----------------------------
  /*case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_PRESS_START:
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_DOWN_LONG_UP:*/
    case KEY_VAL_DOWN_DOWN:
    {
        FmFunctionSele ++ ;
        if (FmFunctionSele > FM_STATE_AUTO) FmFunctionSele = FM_STATE_MANUAL;
        FmDisplayFlag = FM_DISPFLAG_SUBMENU_SEL;
        break;
    }
//--------------------- FFW KEY ----------------------------
  /*case KEY_VAL_FFW_PRESS_START:
    case KEY_VAL_FFW_PRESS:
    case KEY_VAL_FFW_SHORT_UP:
    case KEY_VAL_FFW_LONG_UP:*/
    case KEY_VAL_FFW_DOWN:
    {
        KeyReset();
        FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                             FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                             FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
        PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_PLAY,0,0,0,0,0,0);
        break;
    }
//--------------------- OTHER KEY --------------------------
    default:
        break;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmListKeyProc(void)
{
    PrevCh = CurCh;
    
    switch(KeyVal) {
//--------------------- PLAY KEY ---------------------------
    /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_SHORT_UP:
    case KEY_VAL_PLAY_LONG_UP:*/
//--------------------- MENU KEY ---------------------------
    /*case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_LONG_UP:
    case KEY_VAL_MENU_PRESS_START:
    case KEY_VAL_MENU_DOWN:*/
    case KEY_VAL_MENU_SHORT_UP:
    {
        KeyReset();
        FmDialogVal = FM_DEL_SAVE_YES;
        DispSelectDialogBox(0, 0, FmDialogVal, (RADIO_DIALOG_SAVE_TXT + (unsigned long)(FmFunctionSele - FM_STATE_SAVE) * MENU_PER_BYTE));
        PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_DIALOG,0,0,0,0,0,0);
        break;
    }
//--------------------- UP KEY ----------------------------
  /*case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_LONG_UP:
    case KEY_VAL_UP_PRESS_START:*/
    case KEY_VAL_UP_PRESS:
    case KEY_VAL_UP_DOWN:
    {
        if(CurCh > 0){
            CurCh --;
        }else{
            CurCh = FM_SAVE_BUF_MAX-1;
        }
        FmDisplayFlag = FM_DISPFLAG_LIST_SEL;
        break;
    }
//--------------------- DOWN KEY ----------------------------
  /*case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_PRESS_START:
    case KEY_VAL_DOWN_LONG_UP:*/
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_DOWN_DOWN:
    {
        if(CurCh < FM_SAVE_BUF_MAX-1){
            CurCh++;
        }else{
            CurCh = 0;
        }
        FmDisplayFlag = FM_DISPFLAG_LIST_SEL;
        break;
    }
//--------------------- FFW KEY ----------------------------
  /*case KEY_VAL_FFW_DOWN:
    case KEY_VAL_FFW_PRESS_START:
    case KEY_VAL_FFW_PRESS:
    case KEY_VAL_FFW_LONG_UP:*/
    case KEY_VAL_FFW_SHORT_UP:
    {
        KeyReset();
        FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                             FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                             FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
        PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_PLAY,0,0,0,0,0,0);
        break;
    }
//--------------------- OTHER KEY --------------------------
    default:
        break;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmDialogKeyProc(void)
{
    switch (KeyVal) {
//--------------------- PLAY KEY ---------------------------
  /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_SHORT_UP:
    case KEY_VAL_PLAY_LONG_UP:*/
//--------------------- MENU KEY ---------------------------
    /*case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_LONG_UP:
    case KEY_VAL_MENU_PRESS_START:
    case KEY_VAL_MENU_SHORT_UP:*/
    case KEY_VAL_MENU_DOWN:
    {
       KeyReset();
       
       if(FmDialogVal == FM_DEL_SAVE_YES){ 
           if(FmFunctionSele == FM_STATE_SAVE){
               GlobalFmVar.FmFreqArray[CurCh] = GlobalFmVar.FmFreq;
           }else{
               GlobalFmVar.FmFreqArray[CurCh] = 0;
           } 
       }
       FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                        FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                        FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
                         
       PostFun1Message(MSG_FUNCTION1_FM, MSG_FUNCTION1_FM_PLAY,0,0,0,0,0,0);
       break; 
    }
//--------------------- VOL KEY ----------------------------
  /*case KEY_VAL_VOL_PRESS_START:
    case KEY_VAL_VOL_PRESS:
    case KEY_VAL_VOL_SHORT_UP:
    case KEY_VAL_VOL_DOWN:
    case KEY_VAL_VOL_LONG_UP:*/
//--------------------- FFD KEY ----------------------------
  /*case KEY_VAL_FFD_SHORT_UP:
    case KEY_VAL_FFD_PRESS_START:
    case KEY_VAL_FFD_LONG_UP:*/
    case KEY_VAL_FFD_DOWN:
    case KEY_VAL_FFD_PRESS:
//--------------------- FFW KEY ----------------------------
  /*case KEY_VAL_FFW_SHORT_UP:
    case KEY_VAL_FFW_LONG_UP:
    case KEY_VAL_FFW_PRESS_START:*/
    case KEY_VAL_FFW_DOWN:
    case KEY_VAL_FFW_PRESS:
    {
        FmDialogVal = 1 - FmDialogVal;
        DispSelectDialogBox(0, 0, FmDialogVal, (RADIO_DIALOG_SAVE_TXT + (unsigned long)(FmFunctionSele - FM_STATE_SAVE) * MENU_PER_BYTE));
        break;
    }
//--------------------- OTHER KEY --------------------------
    default:
        break;
    }
}
#endif
//***************************************************************************************************
