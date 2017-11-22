/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3DecodeSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*            tonyzyz       05/10/31       1.1       修改文件查找结构
*            tonyzyz       05/11/14       1.2       添加LRC文件查找
********************************************************************************
*/
#define _IN_SETMENUSCHEDULE

#include "Macro.h"
#include "MsgDef.h"
#include "Global.h"
#include "Lcd.h"
#include "LcdChar.h"
#include "SetMenuMacro.h"
#include "SetMenuStruct.h"
#include "SetMenuGlobal.h"
#include "Resource.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "SetDisplay.h"

void MenuVariableInit(void);
void SetMenuInit(void);
void SysSetDefaultValue(void);
void SetKeyProc(void);
void SettingValueFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue);
void ResumeSystemFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue);
void FirmwareUpgradeFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue);
void FormatDiskFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue);
void UserEQSettintFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue);
//******************************************************************************

unsigned int UserEqBuf[5] __attribute__((section(".setmenu_data, \"aw\""))) = {0,0,0,0,0};

#ifdef		FM_MODULE
#define PLAY_SET_ID             1
#define RADIO_SET_ID            2
#define RECORD_SET_ID           3
#define DISPLAY_SET_ID          4
#define BROWSER_TIME_SET_ID     5
#define SYSTEM_SET_ID           6
#define TIME_SET_ID             7
#define SYS_RESILE_SET_ID       8
#define FORMAT_SET_ID           9
#else
#define PLAY_SET_ID             1
#define RECORD_SET_ID           2
#define DISPLAY_SET_ID          3
#define BROWSER_TIME_SET_ID     4
#define SYSTEM_SET_ID           5
#define TIME_SET_ID             6
#define SYS_RESILE_SET_ID       7

#define RADIO_SET_ID            7
#define FORMAT_SET_ID           8
#endif

#define PLAY_REPEAT_SET_ID      PLAY_SET_ID*100 + 1
#define PLAY_PLAY_SET_ID        PLAY_SET_ID*100 + 2
#define PLAY_EQ_SET_ID          PLAY_SET_ID*100 + 3
#define PLAY_USEREQ_SET_ID      PLAY_SET_ID*100 + 4

#define RADIO_STEREO_SET_ID     RADIO_SET_ID*100 + 1
#define RADIO_REGION_SET_ID     RADIO_SET_ID*100 + 2
#define RADIO_DEL_SET_ID        RADIO_SET_ID*100 + 3

#define RECORD_QUALITY_SET_ID   RECORD_SET_ID*100 + 1
#define RECORD_VOL_SET_ID       RECORD_SET_ID*100 + 2
//#define RECORD_DEL_SET_ID       RECORD_SET_ID*100 + 3

#define DISP_BLIGHT_SET_ID      DISPLAY_SET_ID*100 + 1
#define DISP_LUM_SET_ID         DISPLAY_SET_ID*100 + 2
#define DISP_MODE_SET_ID        DISPLAY_SET_ID*100 + 3

#define TEXT_TIME_SET_ID        BROWSER_TIME_SET_ID*100 + 1
#define JPEG_TIME_SET_ID        BROWSER_TIME_SET_ID*100 + 2

#define SYS_LANGUAGE_SET_ID     SYSTEM_SET_ID*100 + 1 
#define SYS_INFO_SET_ID         SYSTEM_SET_ID*100 + 2 
#define SYS_FIRMWARE_UPGRADE_ID SYSTEM_SET_ID*100 + 3 

#define DIALOG_BOTTEN 

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU SetMenu[SET_MENU_ITEM_TOTAL] = {
#ifdef  	FM_MODULE
    {PLAY_SET_ID,           SETUP_MUSIC_TXT                                                         },  /*  放音设置    */
    {RADIO_SET_ID,          SETUP_RADIO_TXT                                                         },  /*  收音设置    */
    {RECORD_SET_ID,         SETUP_RECORD_TXT                                                        },  /*  录音设置    */
    {DISPLAY_SET_ID,        SETUP_DISPLAY_TXT                                                       },  /*  显示设置    */
    {BROWSER_TIME_SET_ID,   SETUP_BROWSER_TIME_TXT                                                  },  /*  文本及图片浏览设置  */
    {SYSTEM_SET_ID,         SETUP_SYSTEM_TXT                                                        },  /*  系统设置    */
    {TIME_SET_ID,           SETUP_SLEEP_TXT,            &POWEROFFTIMER,         SettingValueFunc    },  /*  时间设置    */
    {SYS_RESILE_SET_ID,     SETUP_RESUME_TXT,           0,                      ResumeSystemFunc    },

//    {FORMAT_SET_ID,         SETUP_FORMAT_TXT,           0,                      FormatDiskFunc      },  /*  格 式 化    */

    {RADIO_STEREO_SET_ID,   SETUP_RADIO_ST_TXT,         &SOLIDSWSTATE,          SettingValueFunc    },  /*  立体声开关  */
    {RADIO_REGION_SET_ID,   SETUP_RADIO_REGION_TXT,     &RADIOREGION,           SettingValueFunc    },  /*  收听地区    */
#else
    {PLAY_SET_ID,           SETUP_MUSIC_TXT                                                         },  /*  放音设置    */
    {RECORD_SET_ID,         SETUP_RECORD_TXT                                                        },  /*  录音设置    */
    {DISPLAY_SET_ID,        SETUP_DISPLAY_TXT                                                       },  /*  显示设置    */
    {BROWSER_TIME_SET_ID,   SETUP_BROWSER_TIME_TXT                                                  },  /*  文本及图片浏览设置  */
    {SYSTEM_SET_ID,         SETUP_SYSTEM_TXT                                                        },  /*  系统设置    */
    {TIME_SET_ID,           SETUP_SLEEP_TXT,            &POWEROFFTIMER,         SettingValueFunc    },  /*  时间设置    */
    {SYS_RESILE_SET_ID,     SETUP_RESUME_TXT,           0,                      ResumeSystemFunc    },

//    {RADIO_SET_ID,          SETUP_RADIO_TXT                                                         },  /*  收音设置    */
//    {FORMAT_SET_ID,         SETUP_FORMAT_TXT,           0,                      FormatDiskFunc      },  /*  格 式 化    */

//    {RADIO_STEREO_SET_ID,   SETUP_RADIO_ST_TXT,         &SOLIDSWSTATE,          SettingValueFunc    },  /*  立体声开关  */
//    {RADIO_REGION_SET_ID,   SETUP_RADIO_REGION_TXT,     &RADIOREGION,           SettingValueFunc    },  /*  收听地区    */
#endif
    
    {PLAY_REPEAT_SET_ID,    SETUP_MUSIC_MODE_TXT,       &REPEATSTATE,           SettingValueFunc    },  /*  重复设置    */                
    {PLAY_PLAY_SET_ID,      SETUP_MUSIC_ORDER_TXT,      &PLAYMODE,              SettingValueFunc    },  /*  播放设置    */
    {PLAY_EQ_SET_ID,        SETUP_MUSIC_EQ_TXT,         &EQSTATE,               SettingValueFunc    },  /*  EQ选择      */
    {PLAY_USEREQ_SET_ID,    SETUP_MUSIC_USEREQ_TXT,     UserEqBuf,              UserEQSettintFunc   },  /*  用户EQ调整  */
    
    {RECORD_QUALITY_SET_ID, SETUP_RECORD_QUALITY_TXT,   &MICINQUALITY,          SettingValueFunc    },  /*  线录输入比特率  */
    {RECORD_VOL_SET_ID,     SETUP_RECORD_VOL_TXT,       &RECORDMICVOL,          SettingValueFunc    },  /*  语音输入质量    */
    //{RECORD_DEL_SET_ID,     NFA_RECORD_AUTO_SETTING,    &AUTOSWSTATE,           SettingValueFunc    },  /*  自动检测开关    */
    
    {DISP_BLIGHT_SET_ID,    SETUP_DISPLAY_TIME_TXT,     &BKLIGHT,               SettingValueFunc    },  /*  背光灯时间      */
    {DISP_LUM_SET_ID,       SETUP_DISPLAY_LUM_TXT,      &LCDLUM,                SettingValueFunc    },  /*  亮度调节        */
    {DISP_MODE_SET_ID,      SETUP_DISPLAY_MODE_TXT,     &LCDDISPMODE,           SettingValueFunc    },  /*  背光显示模式    */
    
    {TEXT_TIME_SET_ID,      SETUP_TEXT_TIME_TXT,        &S_TXTAUTOBROWSERTIME,  SettingValueFunc    },
    {JPEG_TIME_SET_ID,      SETUP_JPEG_TIME_TXT,        &S_JPEGAUTOBROWSERTIME, SettingValueFunc    },
    
    {SYS_LANGUAGE_SET_ID,   SETUP_SYSTEM_LANGUAGE_TXT,  &LANGUAGESTATE,         SettingValueFunc    },  /*  语言选择        */
    {SYS_INFO_SET_ID,       SETUP_SYSTEM_INFO_TXT,      0,                      DispCopyRightFunc   },  /*  固件版本        */
    {SYS_FIRMWARE_UPGRADE_ID, SETUP_SYSTEM_UPGRADE_TXT, 0,                      FirmwareUpgradeFunc }   /*  固件升级        */
//    {SYS_MEMORY_SELECT_ID,  NFA_SYS_MEMORY_SELECT,      &MEMORY_SELECT,         SettingValueFunc    },
//    {SYS_RESILE_SET_ID,     NFA_SYS_RESILE,             0,                      ResumeSystemFunc    }   /*  恢复系统默认值  */
};


/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void SetMenuModule(void)
{
    unsigned long nfa_tmp;
    unsigned int  adjust_tmp;
    
    switch(Fun2MsgArray[1]) {
    case MSG_FUN2_MENU_INIT:
    {
        MenuVariableInit();
        KeyReset();
        SetMenuInit();
        
        PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_INIT,Fun2MsgArray[2],Fun2MsgArray[3],0,0,0,0);
        PostFun2Message(MSG_FUNCTION2_MENU, 0,0,0,0/*0,0,0*/);
        break;
    }
    default:
        break;
    }
    
    if (DisplayFlag == DISP_SETTING) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            SetKeyProc();
            
            if (SystemInfo.bc.bBackLightOnStart1) {
                SystemInfo.bc.bBackLightOnStart1 = 0;
//                DmaFillImage2Lcd();
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
            DisplayPicture(SETUP_BATT_PIC_X, SETUP_BATT_PIC_Y, SETUP_BATT_PIC_XSIZE, SETUP_BATT_PIC_YSIZE, (NFA_SETUP_BATT_PIC + (unsigned long)(BatteryLevel) * SETUP_BATT_PIC_SIZE));
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
            MusicSetFreq51Return();
            return;
        }

        if (SystemInfo.bc.bDisplayRecovery) {
            SystemInfo.bc.bDisplayRecovery = 0;
            if(SetMenuAdjust==1){
                nfa_tmp    = pSetMenu->DispInNFAddr;
                adjust_tmp = AdjustValueTmp;
            }else if(pSetMenu->father != MENU_NULL){
                nfa_tmp = pSetMenu->father->DispInNFAddr;
                adjust_tmp = pSetMenu->id%100-1;
            }else{
                nfa_tmp = SETUP_MENU_TXT;
                adjust_tmp = pSetMenu->id%100-1;
            }
            PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_INIT/*2*/,(unsigned int)(nfa_tmp>>16),
                    (unsigned int)(nfa_tmp&0xFFFF),adjust_tmp,0,0,0); 
        }
        
        DISP_Set();
        
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
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void MenuVariableInit(void)
{
    if (Sys2InitFlag.bc.bSetingMenuModuleEnable) {
        Sys2InitFlag.bc.bSetingMenuModuleEnable = 0;
    }
    SetPrevScrollY = SETUP_SCROLL_PIC_Y;
    SetMenuAdjust = 0;
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void *getSet_menu_p(unsigned int id)
{
	int i;
	
	for(i = 0;i<SET_MENU_ITEM_TOTAL;i ++)
	{
		if(SetMenu[i].id == id)
			return(&SetMenu[i]);
	}
	return MENU_NULL;
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void SetMenuInit(void)
{
    unsigned int i; 
    unsigned int j;
    
    for(i = 0; i < SET_MENU_ITEM_TOTAL; i++)
    {                           
        SetMenu[i].father = getSet_menu_p(SetMenu[i].id/100);
        SetMenu[i].son    = getSet_menu_p((SetMenu[i].id*100)+1);  
        SetMenu[i].per    = getSet_menu_p(SetMenu[i].id-1);
        SetMenu[i].next   = getSet_menu_p(SetMenu[i].id+1);   
    }    
    SetMenuAdjust=0;
    pSetMenu = &SetMenu[0];    
} 
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void SysSetDefaultValue(void)
{
    int i;
    OutputVolume = 20;
    LineInputVolume =  3;
    MicInputVolume =  3;
    PowerOffTime = 0;
    LightOffTime = 4;
    LigthLevel = 2;
    ContrastLevel = 3;
    DisplayMode = 0;
    Language = DEFAULT_LANGUE;
    ScrollSpeed =  3;
    
    gJpegAutoBrowserTime = 2;
    gTxtAutoBrowserTime = 2;
    
    //---------------Music Global Varable-----------------
    GlobalMusicVar.MusicCurFileNum = 1;
    GlobalMusicVar.Mode = MUSIC_DECODE_ALL_REP;   
    GlobalMusicVar.Eq = MUSIC_DECODE_EQ_NORMAL;
    GlobalMusicVar.Order = MUSIC_DECODE_ORDER;
    
    for(i=0; i<5; i++){
        GlobalMusicVar.UserEq[i] =  3;
    }
    
    //---------------Encode Global Varable-----------------
    GlobalEncVar.MicQuality = 0;
    GlobalEncVar.LineInQuality = 2;
    GlobalEncVar.AutoDevideSection =  0;
    //---------------Fm Global Varable---------------------
    GlobalFmVar.FmArea =  0;
    GlobalFmVar.FmStereo =  0;
    GlobalFmVar.FmState =  0;
    GlobalFmVar.FmSaveNum =  0;
    
    SaveSysInformation();
    
//---------------------------------------------------------
//恢复默认值是需要实时处理
    LcdBackLevelSet(LigthLevel);
    SystemInfo.bc.bMusicMenuSet = 1;
}

/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : SetMenuKey.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void SetKeyProc(void)
{
    unsigned long Nfa_tmp;
    unsigned int  pram1=0; 
    unsigned int  pram2=0;
    unsigned int  pram3=0;
    unsigned int  post_msg_flag=0;
    unsigned int key = KeyVal;
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
    
    switch (KeyVal) {
//--------------------- PLAY KEY ---------------------------
//    case KEY_VAL_PLAY_PRESS_START:
//    {
//        SetMenuAdjust = 0;
//        pSetMenu      = &SetMenu[0];
//        break;
//    }
//--------------------- REC KEY ---------------------------
  /*case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS_START:
    case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_SHORT_UP:
    case KEY_VAL_REC_LONG_UP:
        break;*/
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_PRESS_START:
    {
        if(SetMenuAdjust == 1){
            SystemInfo.bc.bMusicMenuSet = 1;
            AdjustValueTmp = *pSetMenu->AdjustValue = SettingValueTmp;      //返回时恢复原来状态
            (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);
        }
        SetMenuAdjust = 0;
       
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
       
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        break;
    }    
    case KEY_VAL_MENU_SHORT_UP:
    {
        if (pSetMenu->son != MENU_NULL) {
            Nfa_tmp       = pSetMenu->DispInNFAddr;
            pSetMenu      = pSetMenu->son;
        } else if(SetMenuAdjust != 1) {
            SettingValueTmp = AdjustValueTmp = *pSetMenu->AdjustValue;
            
            (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);

            SetMenuAdjust = 1;
            if(pSetMenu->id != FORMAT_SET_ID && pSetMenu->id != PLAY_USEREQ_SET_ID && pSetMenu->id != SYS_RESILE_SET_ID && (pSetMenu->id < SYS_INFO_SET_ID || pSetMenu->id > SYS_FIRMWARE_UPGRADE_ID)){
                Nfa_tmp       = pSetMenu->DispInNFAddr;
                pram1         = *pSetMenu->AdjustValue;
            }else{
                break;
            }
        } else if(SetMenuAdjust == 1) {
            if(pSetMenu->id != PLAY_USEREQ_SET_ID && pSetMenu->id != SYS_FIRMWARE_UPGRADE_ID){

                (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);

                SetMenuAdjust = 0;
                pram1         = pSetMenu->id%100-1;
                if(pSetMenu->father->id != MENU_NULL){
                    Nfa_tmp       = pSetMenu->father->DispInNFAddr;
                }else{
                    Nfa_tmp       = SETUP_MENU_TXT;
                }
            }else{
                (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);
                break;
            }
        }
        PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_INIT/*2*/,(unsigned int)(Nfa_tmp>>16),(unsigned int)(Nfa_tmp&0xFFFF),pram1,0,0,0);
        break;
    }
//--------------------- UP KEY ---------------------------
    case KEY_VAL_UP_PRESS:
    case KEY_VAL_UP_SHORT_UP:
    {
        if(SetMenuAdjust == 1)
        {                
            (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);
            break;
        }
        if(pSetMenu->per != MENU_NULL)
        {
            pSetMenu          = pSetMenu->per;
            
            pram1   = pSetMenu->next->id%100-1;
            pram2   = pSetMenu->id%100-1;
            
            if(pSetMenu->father->id != MENU_NULL){
                Nfa_tmp = pSetMenu->father->DispInNFAddr;
                pram3   = pSetMenu->id%100-1;
            }else{
                Nfa_tmp = SETUP_MENU_TXT;
                pram3   = pSetMenu->id-1;
            }
            PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_SELECT,(unsigned int)(Nfa_tmp>>16),
                        (unsigned int)(Nfa_tmp&0xFFFF),pram1,pram2,pram3,0);
        }
        break;
    }
//--------------------- DOWN KEY ---------------------------
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_DOWN_SHORT_UP:
    {
       if(SetMenuAdjust == 1)
       {                
           (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);
           break;
       }
       if(pSetMenu->next != MENU_NULL)
       {
           pSetMenu = pSetMenu->next;
           pram1   = pSetMenu->per->id%100-1;
           pram2   = pSetMenu->id%100-1;
           if(pSetMenu->father->id != MENU_NULL){
               Nfa_tmp = pSetMenu->father->DispInNFAddr;
               pram3   = pSetMenu->id%100-1;
           }else{
               Nfa_tmp = SETUP_MENU_TXT;
               pram3   = pSetMenu->id-1;
           }
           PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_SELECT,(unsigned int)(Nfa_tmp>>16),
                       (unsigned int)(Nfa_tmp&0xFFFF),pram1,pram2,pram3,0);
       }
       break;
    }
//--------------------- FFW KEY ---------------------------
    case KEY_VAL_FFW_SHORT_UP:
    {
        if(SetMenuAdjust == 1)
        {
            if((pSetMenu->id != PLAY_USEREQ_SET_ID) && (pSetMenu->id != SYS_RESILE_SET_ID) && (pSetMenu->id != SYS_FIRMWARE_UPGRADE_ID) && (pSetMenu->id != FORMAT_SET_ID)){
                SetMenuAdjust = 0;
                SystemInfo.bc.bMusicMenuSet = 1;
                AdjustValueTmp = *pSetMenu->AdjustValue = SettingValueTmp;      //返回时恢复原来状态
                
                if (pSetMenu->id == DISP_LUM_SET_ID) {
                    LcdBackLevelSet(LigthLevel);
                }
                (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);
                if(pSetMenu->father != MENU_NULL){
                    Nfa_tmp = pSetMenu->father->DispInNFAddr;
                }else{
                    Nfa_tmp = SETUP_MENU_TXT;
                }
            }else{
                (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);
                break;
            }
        }
        else if(pSetMenu->father != MENU_NULL)
        {
            pSetMenu = pSetMenu->father;
            if(pSetMenu->father != MENU_NULL){
                Nfa_tmp = pSetMenu->father->DispInNFAddr;
            }else{
                Nfa_tmp = SETUP_MENU_TXT;
            }
        }else{   //
            SetMenuAdjust = 0;
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
            Fun2ModuleChange();
            MainMenuState = 0;
            DisplayFlag = DISP_MENU;
            break;
        }
        PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_INIT/*2*/,(unsigned int)(Nfa_tmp>>16),
                        (unsigned int)(Nfa_tmp&0xFFFF),pSetMenu->id%100-1,0,0,0);
        break;
    }
    default:
    {
        if(SetMenuAdjust == 1)
            (*pSetMenu->pFunc)(key, pSetMenu->DispInNFAddr, pSetMenu->AdjustValue);
        break;
    }
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void SettingValueFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue)
{
    unsigned int value_tmp;
    unsigned int value_max;
    unsigned int TmpBuf[8];
    
    LanguageResoureOffset = (unsigned long)(Language) * MENU_LANGUE_PER_BYTE;//2006-2-23 21:03 
    if(Language < LANGUAGE_MAX_COUNT) flash_addr += LanguageResoureOffset;//2006-2-23 21:03
    flash_addr += MenuLogicAddress;
    
    FlashReadLogic(flash_addr, TmpBuf, 8);
    Byte2Word(TmpBuf, TmpBuf, 8);
    
    value_max = TmpBuf[0]-1;
    value_tmp = AdjustValueTmp;
    
    switch(key){
//--------------------- PLAY KEY ---------------------------
  /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_SHORT_UP:
    case KEY_VAL_PLAY_LONG_UP:
    case KEY_VAL_PLAY_PRESS_START:
        break;*/
//--------------------- MENU KEY ---------------------------
  /*case KEY_VAL_MENU_DOWN:
    case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_PRESS_START:
    case KEY_VAL_MENU_LONG_UP:*/
    case KEY_VAL_MENU_SHORT_UP://KEY_MODE:
    {
        *pValue = AdjustValueTmp;
        switch(pSetMenu->id){
            case PLAY_REPEAT_SET_ID:
            case PLAY_PLAY_SET_ID:
            case PLAY_EQ_SET_ID:
            case RADIO_STEREO_SET_ID:           //收音机设置时需要更改收音机状态
            case RADIO_REGION_SET_ID:
                SystemInfo.bc.bMusicMenuSet = 1;
                break;
                
            case DISP_LUM_SET_ID:
                LcdBackLevelSet(*pValue);
                break;
            case TIME_SET_ID:
                PowerOffCounter = 0;
                break;
        }
        break;
    }
    case KEY_VAL_MENU_PRESS_START:
    {
        if(pSetMenu->id == DISP_LUM_SET_ID){
            LcdBackLevelSet(*pValue);
        }
        if(pSetMenu->id == PLAY_EQ_SET_ID){
            *pValue = SettingValueTmp;
            SystemInfo.bc.bMusicMenuSet = 1;
        }
        break;
    }
//--------------------- UP KEY ---------------------------
    case KEY_VAL_UP_PRESS:
    case KEY_VAL_UP_SHORT_UP://KEY_UP:
    {
        if(AdjustValueTmp > 0)
            AdjustValueTmp--;
        else
            AdjustValueTmp = value_max;
            
        if(pSetMenu->id == DISP_LUM_SET_ID){
            LcdBackLevelSet(AdjustValueTmp);
        }
        if(pSetMenu->id == PLAY_EQ_SET_ID){
            SystemInfo.bc.bMusicMenuSet = 1;
            *pValue = AdjustValueTmp;
        }
        break;
    }
//--------------------- DOWN KEY ---------------------------
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_DOWN_SHORT_UP://KEY_DOWN:
    {
        if(AdjustValueTmp < value_max)
            AdjustValueTmp++;
        else
            AdjustValueTmp = 0;
            
        if(pSetMenu->id == DISP_LUM_SET_ID){
            LcdBackLevelSet(AdjustValueTmp);
        }
        if(pSetMenu->id == PLAY_EQ_SET_ID){
            SystemInfo.bc.bMusicMenuSet = 1;
            *pValue = AdjustValueTmp;
        }
        break;
    }
    default:
        break;
    }
    
    if(value_tmp != AdjustValueTmp)
        PostDisplayMessage(10,MSG_DISP_SET_SELECT, (unsigned int)((pSetMenu->DispInNFAddr)>>16),
                                (unsigned int)((pSetMenu->DispInNFAddr)&0xFFFF), value_tmp, AdjustValueTmp, SettingValueTmp/**pSetMenu->AdjustValue*/,0);
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void ResumeSystemFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue)
{
    switch(key) {
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_SHORT_UP://KEY_MODE:
    {
        if((unsigned int)(pSetMenu->AdjustValue) & 0x01){
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            SysSetDefaultValue();//恢复系统默认值
        }else{
            DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_DEFAULT_TXT);
        }
        pSetMenu->AdjustValue = 0;
        break;
    }
    case KEY_VAL_MENU_PRESS_START://KEY_MODE_L:
    {
        pSetMenu->AdjustValue = 0;
        DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_DEFAULT_TXT);
        break;
    }
//--------------------- FFD KEY ---------------------------
//--------------------- FFW KEY ---------------------------
    case KEY_VAL_FFD_SHORT_UP://KEY_UP:
    case KEY_VAL_FFW_SHORT_UP://KEY_DOWN:
    {
        pSetMenu->AdjustValue++;
        DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_DEFAULT_TXT);
        break;
    }
    default:
        break;
    }
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void FirmwareUpgradeFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue)
{
    SysUsbFlag.bc.TranMode = 1;
    switch(key){
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_SHORT_UP://KEY_MODE:
    {
        if((unsigned int)(pSetMenu->AdjustValue) & 0x01){
            pSetMenu->AdjustValue = 0;
            DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_PLUGIN_TXT);
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            SysUsbFlag.bc.TranMode = 1;
        }else{
            DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_UPGRADE_TXT);

            if(SetMenuAdjust == 1)
            {
                SysUsbFlag.bc.TranMode = 0;
                PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_INIT/*2*/,(unsigned int)((pSetMenu->father->DispInNFAddr)>>16),
                        (unsigned int)((pSetMenu->father->DispInNFAddr)&0xFFFF),pSetMenu->id%100-1,0,0,0);
                SetMenuAdjust = 0;
            }
        pSetMenu->AdjustValue = 0;
        }
        break;
    }
    case KEY_VAL_MENU_PRESS_START://KEY_MODE_L:
    {
        SysUsbFlag.bc.TranMode = 0;
        pSetMenu->AdjustValue = 0;
        DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_UPGRADE_TXT);
        break;
    }
//--------------------- FFD KEY ---------------------------
//--------------------- FFW KEY ---------------------------
    case KEY_VAL_FFD_SHORT_UP://KEY_UP:
    case KEY_VAL_FFW_SHORT_UP://KEY_DOWN:
    {
        pSetMenu->AdjustValue++;
        DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_UPGRADE_TXT);
        break;
    }
    default:
        break;
    }

}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void FormatDiskFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue)
{
#if(FLASHROM_TYPE !=  _MLC_ )
    unsigned int tmp;
 
    switch(key){
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_SHORT_UP://KEY_MODE:
    {
        if((unsigned int)(pSetMenu->AdjustValue) & 0x01){
            DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_FORMATING_TXT);
#ifdef LCD_MEMORY_DEV 
            DmaFillImage2Lcd();
#endif 
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            
            MusicSetFreq51();
            
//            LowFormat();
            FileInit();
          	SysFreeMemeryFlash = GetFreeMem(0);
          		
		    FileFragCleanFlag = 0;
            SaveSysInformation();
            
            MusicSetFreq51Return();
        }else{
            DispSelectDialogBox(42, 64, (unsigned int)pSetMenu->AdjustValue, DIALOG_FORMAT_TXT);
        }
        pSetMenu->AdjustValue = 0;
        break;
    }
    case KEY_VAL_MENU_PRESS_START://KEY_MODE_L:
    {
        pSetMenu->AdjustValue = 0;
        DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_FORMATING_TXT);
        break;
    }
//--------------------- FFD KEY ---------------------------
//--------------------- FFW KEY ---------------------------
    case KEY_VAL_FFD_SHORT_UP://KEY_UP:
    case KEY_VAL_FFW_SHORT_UP://KEY_DOWN:
    {
        pSetMenu->AdjustValue++;
        DispSelectDialogBox(0, 0, (unsigned int)pSetMenu->AdjustValue, DIALOG_FORMAT_TXT);
        break;
    }
    default:
        break;
    }
#endif
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void UserEQSettintFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue)
{
    unsigned int MenuStrBuf[MENU_PER_BYTE];
    unsigned int old_mode, color_tmp;
    unsigned int k,m,temp;
    unsigned int poffset;
    unsigned int *pUserEqBuf;
    unsigned long TempL;
    LCD_RECT r;
    
    pUserEqBuf = &UserEqBuf[0];
    
    switch(key){
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_SHORT_UP://KEY_MODE:
    {
        if(SetMenuAdjust == 0){
            MusicSetFreq51();
            
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, SETUP_USEREQBK_PIC);
            DisplayPicture(SETUP_HOLD_PIC_X, SETUP_HOLD_PIC_Y, SETUP_HOLD_PIC_XSIZE, SETUP_HOLD_PIC_YSIZE, (SETUP_HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * SETUP_HOLD_PIC_SIZE));
            DisplayPicture(SETUP_BATT_PIC_X, SETUP_BATT_PIC_Y, SETUP_BATT_PIC_XSIZE, SETUP_BATT_PIC_YSIZE, (NFA_SETUP_BATT_PIC + (unsigned long)(BatteryLevel) * SETUP_BATT_PIC_SIZE));
            
            flash_addr += MenuLogicAddress;
            LanguageResoureOffset = (unsigned long)(Language) * MENU_LANGUE_PER_BYTE;
            if(Language < LANGUAGE_MAX_COUNT) flash_addr += LanguageResoureOffset;//2006-2-23 21:03
            
            FlashReadLogic(flash_addr, MenuStrBuf, MENU_PER_BYTE);
            Byte2Word(MenuStrBuf, MenuStrBuf, MENU_PER_BYTE);
            
            r.x0 = SETUP_MENU_TXT_X;
            r.y0 = SETUP_MENU_TXT_Y;
            r.x1 = SETUP_MENU_TXT_X + SETUP_MENU_TXT_XSIZE - 1;
            r.y1 = SETUP_MENU_TXT_Y + SETUP_MENU_TXT_YSIZE - 1;
            LCD_DispStringInRect(&r, &r, &MenuStrBuf[1], LCD_TEXTALIGN_CENTER);
            
            memcpy(pUserEqBuf,GlobalMusicVar.UserEq,5);//2006-2-23 14:09
            memcpy(UserEqBufTmp,GlobalMusicVar.UserEq,5);//2006-2-23 14:09
            
            UserEqCont = 0;
            DisplayPicture(SETUP_USEREQ_SEL_PIC_X, SETUP_USEREQ_SEL_PIC_Y, SETUP_USEREQ_SEL_PIC_XSIZE, SETUP_USEREQ_SEL_PIC_YSIZE, (NFA_SETUP_USEREQ_SEL_PIC + (unsigned long)(GlobalMusicVar.UserEq[UserEqCont]) * SETUP_USEREQ_SEL_PIC_SIZE));
            for(k = 1; k < 5; k++){
                DisplayPicture((SETUP_USEREQ_NOSEL_PIC_X + k * 50), SETUP_USEREQ_NOSEL_PIC_Y, SETUP_USEREQ_NOSEL_PIC_XSIZE, SETUP_USEREQ_NOSEL_PIC_YSIZE, (NFA_SETUP_USEREQ_NOSEL_PIC + (unsigned long)(GlobalMusicVar.UserEq[k]) * SETUP_USEREQ_NOSEL_PIC_SIZE));
            }
            
            color_tmp = LCD_GetColor();
            LCD_SetColor(COLOR_BLACK);
            DisplayPicture(SETUP_USEREQ_BTN_YES_PIC_X, SETUP_USEREQ_BTN_YES_PIC_Y, SETUP_USEREQ_BTN_PIC_XSIZE, SETUP_USEREQ_BTN_PIC_YSIZE, SETUP_USEREQ_BTN_NOSEL_PIC);
            DisplayMenuStr(SETUP_USEREQ_BTN_YES_PIC_X, (SETUP_USEREQ_BTN_YES_PIC_Y + (SETUP_USEREQ_BTN_PIC_YSIZE >> 1) - 8),
                           SETUP_USEREQ_BTN_PIC_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_YES_TXT);
            DisplayPicture(SETUP_USEREQ_BTN_NO_PIC_X, SETUP_USEREQ_BTN_NO_PIC_Y, SETUP_USEREQ_BTN_PIC_XSIZE, SETUP_USEREQ_BTN_PIC_YSIZE, SETUP_USEREQ_BTN_NOSEL_PIC);
            DisplayMenuStr(SETUP_USEREQ_BTN_NO_PIC_X, (SETUP_USEREQ_BTN_NO_PIC_Y + (SETUP_USEREQ_BTN_PIC_YSIZE >> 1) - 8),
                           SETUP_USEREQ_BTN_PIC_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_NO_TXT);
            LCD_SetColor(color_tmp);
            
            MusicSetFreq51Return();
        }else{
            if(UserEqCont == 5){
                SetMenuAdjust = 0;
                PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_INIT/*2*/,(unsigned int)((pSetMenu->father->DispInNFAddr)>>16),
                        (unsigned int)((pSetMenu->father->DispInNFAddr)&0xFFFF),pSetMenu->id%100-1,0,0,0);
                UserEqCont = 0;
                pSetMenu->AdjustValue = &UserEqBuf[0];
                memcpy(GlobalMusicVar.UserEq,pUserEqBuf,5);//2006-2-23 14:09
                SystemInfo.bc.bMusicMenuSet = 1;
                //MusicEqSet();
            }else if(UserEqCont == 6){
                SetMenuAdjust = 0;
                memcpy(GlobalMusicVar.UserEq,UserEqBufTmp,5);   //RT UserEq
                SystemInfo.bc.bMusicMenuSet = 1;
                PostDisplayMessage(MSG_DISP_SET,MSG_DISP_SET_INIT/*2*/,(unsigned int)((pSetMenu->father->DispInNFAddr)>>16),
                        (unsigned int)((pSetMenu->father->DispInNFAddr)&0xFFFF),pSetMenu->id%100-1,0,0,0);
                UserEqCont = 0;
                pSetMenu->AdjustValue = &UserEqBuf[0];
            } 
        }
        break;
    }
//--------------------- UP KEY ---------------------------
    case KEY_VAL_UP_PRESS:
    case KEY_VAL_UP_SHORT_UP://KEY_UP:
    {
        if(*(pValue+UserEqCont) > 0 && UserEqCont < 5){
            (*(pValue+UserEqCont))--;
            SystemInfo.bc.bMusicMenuSet = 1;
            memcpy(GlobalMusicVar.UserEq,pUserEqBuf,5);   //RT UserEq
            DisplayPicture((SETUP_USEREQ_SEL_PIC_X + UserEqCont * 50), SETUP_USEREQ_SEL_PIC_Y, SETUP_USEREQ_SEL_PIC_XSIZE, SETUP_USEREQ_SEL_PIC_YSIZE, (NFA_SETUP_USEREQ_SEL_PIC + (unsigned long)(GlobalMusicVar.UserEq[UserEqCont]) * SETUP_USEREQ_SEL_PIC_SIZE));
        }
        break;
    }
//--------------------- DOWN KEY ---------------------------
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_DOWN_SHORT_UP://KEY_DOWN:
    {
        if(*(pValue+UserEqCont) < 6 && UserEqCont < 5){
            (*(pValue+UserEqCont))++;
            SystemInfo.bc.bMusicMenuSet = 1;
            memcpy(GlobalMusicVar.UserEq,pUserEqBuf,5);   //RT UserEq
            DisplayPicture((SETUP_USEREQ_SEL_PIC_X + UserEqCont * 50), SETUP_USEREQ_SEL_PIC_Y, SETUP_USEREQ_SEL_PIC_XSIZE, SETUP_USEREQ_SEL_PIC_YSIZE, (NFA_SETUP_USEREQ_SEL_PIC + (unsigned long)(GlobalMusicVar.UserEq[UserEqCont]) * SETUP_USEREQ_SEL_PIC_SIZE));
        }
        break;
    }
//--------------------- FFD KEY ---------------------------
    case KEY_VAL_FFD_SHORT_UP://KEY_FFD:
    {
        temp = UserEqCont;
        if(UserEqCont < 6){
            UserEqCont++;
        }else{ 
            UserEqCont = 0;
        }
        UserEqSettingDisplay(temp, UserEqCont);            
        break;
    }
//--------------------- FFW KEY ---------------------------
    case KEY_VAL_FFW_SHORT_UP://KEY_FFW:
    {
        temp = UserEqCont;
        if(UserEqCont > 0)
            UserEqCont--;
        else
            UserEqCont=6;
        UserEqSettingDisplay(temp, UserEqCont);
        break;
    }
    default:
        break;
    }
}
//------------------------------------------------------------------------------