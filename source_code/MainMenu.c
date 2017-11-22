//******************************************************************************
#define     _IN_MENU
//------------------------------------------------------------------------------
#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "Resource.h"
#include "KeyMacro.h"
#include "KeyStruct.h"
#include "BrowserMacro.h"
#include "BrowserGlobal.h"

#include "Global.h"
#include "KeyGlobal.h"


#ifdef GAME_MODULE
    #include "Block.h"
#endif
#include "lcd.h"
#include "key.h"
#include "LcdChar.h"

void MainMenuKey(void);
void ManiMenuDisplay(void);

//unsigned int MenuId;
//unsigned int PrevMenuId;
//unsigned int MenuDispFlag;
//unsigned int MainMenuState;

/*------------------------------------------------------------------------------
Function: 按键扫描程序
Input   : 
Output  : 
说明：  1、确定按键状态及键值
        2、判断Hold状态及其处理
------------------------------------------------------------------------------*/
void MainMenuInit(void)
{
    MenuId = 0;
    PrevMenuId = 0;
    MenuDispFlag = 0;
    MainMenuState = 0;
}
/*------------------------------------------------------------------------------
Function: 按键扫描程序
Input   : 
Output  : 
说明：  1、确定按键状态及键值
        2、判断Hold状态及其处理
------------------------------------------------------------------------------*/
void MainMenuProc(void)
{
    switch (DisplayFlag) {
    case DISP_MENU:
    case DISP_HOLD:
    case DISP_USB:
    case DISP_BATTERY:
    case DISP_UPGRADE_DIALOG:
        if (SystemInfo.bc.bBackLightOffStart) {
            //read_creg(%imask,ImaskTemp);
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
            //write_creg(%imask,ImaskTemp);
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 1) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 0;
            bitset_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart1 = 1;
            LCD_SDRAM_Standby_Return();
        }
        
        if (DisplayFlag == DISP_MENU) {
            if (MainMenuState == 0) {
                KeyReset();
                MenuDispFlag = 1;
                MainMenuState = 1;
            }
            
            if (SysBatteryFlag.bc.bBatteryDisplay) {
                SysBatteryFlag.bc.bBatteryDisplay = 0;
                DisplayPicture(MMENU_BATT_PIC_X, MMENU_BATT_PIC_Y, MMENU_BATT_PIC_XSIZE, MMENU_BATT_PIC_YSIZE, ((unsigned long)(MMENU_BATT_PIC_1 + (unsigned long)(BatteryLevel) * MMENU_BATT_PIC_SIZE)));
            }
            
            if (SystemInfo.bc.bBackLightOnStart1 == 0) {
                MainMenuKey();
            }
            
            if (SystemInfo.bc.bDisplayRecovery) {
                SystemInfo.bc.bDisplayRecovery = 0;
                MenuDispFlag = 1;
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
            } else  {
                ManiMenuDisplay();
            }
        } else if (DisplayFlag == DISP_BATTERY) {
            if (SystemInfo.bc.bBackLightOnStart1 != 1) {
                if (KeyFlag.bc.bKeyProssed) {
                    KeyFlag.bc.bKeyProssed = 0;
                    if (KeyStatus == KEY_STATUS_DOWN) {
                        KeyReset();
                        if (SysBatteryFlag.bc.BatteryChargeEn == 0) {
                            SysBatteryFlag.bc.bBatteryDisplay = 1;
                            BatteryLevel = 3;
                        }
                        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                        Fun1ModuleChange();
                        
                        AmplifierOn();
                        
                        if (PreDisplayFlag == DISP_BATTERY) {
                            MainMenuState = 0;
                            DisplayFlag = DISP_MENU;
                            PreDisplayFlag = DisplayFlag;
                        } else {
                            DisplayFlag = PreDisplayFlag;
                            SystemInfo.bc.bDisplayRecovery = 1;
                        }
                    }
                }
            }
        } else {
            if (KeyFlag.bc.bKeyProssed) {
                KeyFlag.bc.bKeyProssed = 0;
            }
        }
        
        if (SystemInfo.bc.bBackLightOnStart1) {
            SystemInfo.bc.bBackLightOnStart1 = 0;
            DmaFillImage2Lcd();
            LcdBackLightOn();
            KeyReset();
        }
        break;
        
    default:
        break;
    }
}

/*------------------------------------------------------------------------------
Function:MODE键功能处理程序
Input   :
Output  :
------------------------------------------------------------------------------*/
void MainMenuModeKey(void)
{
    switch(MenuId){
#ifdef  MUSIC_MODULE
        case MUSIC_MODULE_ID:
            DisplayFlag = DISP_MUSIC;
            //PreDisplayFlag = DisplayFlag;
            MusicDisplayInterface = 1;
//test---------------------------------
//进音乐播放的时候先初始化以下浏览器
            if(DirTreeInfo.CurPickFileType[0] != FIND_FILE_TYPE_MUSIC)
            {
                DirValueInit();
                BrowserFileTypeInit(FIND_FILE_TYPE_MUSIC);
                BrowserDirInit();
            }
//-------------------------------------         
            if (Sys1InitFlag.bc.bMusicModuleInitEnable){//add by zyz
                Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                Fun1ModuleChange();
                Fun2ModuleChange();
                PostFun1Message(MSG_FUNCTION1_MUSIC_CHECK,0,0,0,0/*,0,0,0*/);
            } else {
                if (Fun1MsgArray[0] == MSG_FUNCTION1_MUSIC_MP3) {
                    PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_PLAY,MSG_FUNCTION1_MP3_DISPLAY_INIT,0,0/*,0,0,0*/);
                } else if (Fun1MsgArray[0] == MSG_FUNCTION1_MUSIC_WMA) {
                    PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_PLAY,MSG_FUNCTION1_WMA_DISPLAY_INIT,0,0/*,0,0,0*/);
                } else {
                    PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_PLAY,MSG_FUNCTION1_WAV_DISPLAY_INIT,0,0/*,0,0,0*/);
                }
            }
            break;
#endif    

#ifdef  AVI_DECODE_MODULE         
        case AVI_DECODE_MODULE_ID:     /*  视频播放    */
            DisplayFlag = DISP_BROWSER;
            PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_INIT,FIND_FILE_TYPE_AVI,0,0/*,0,0,0*/);
            break;
#endif   

#ifdef  FM_MODULE     
        case FM_MODULE_ID:     /*  FM          */
        	 DisplayFlag = DISP_FM;
             if (Sys1InitFlag.bc.bFmModuleInitEnable) {//add by zyz
                 Fun1ModuleChange();
                 PostSche1Message(MSG_SCHEDULER1_FM/*,0,0,0,0,0,0,0*/);
             }
        	 PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_INIT,0,0,0);
//        	 KeyVal = KEY_VAL_NONE;
           break;
#endif  
 
#ifdef  JPEG_DECODE_MODULE            
        case JPEG_DECODE_MODULE_ID:     /* 图片浏览 */
            DisplayFlag = DISP_BROWSER;
            PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_INIT,FIND_FILE_TYPE_JPEG,0,0/*,0,0,0*/);
            break;
#endif

#ifdef  TXT_MODULE            
        case TXT_MODULE_ID:     /*文本*/
            DisplayFlag = DISP_BROWSER;
            PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_INIT,FIND_FILE_TYPE_TXT,0,0/*,0,0,0*/);
            break;
#endif            
      
#ifdef  ENCODE_MODULE            
        case ENCODE_MODULE_ID:     /*录音*/
            DisplayFlag = DISP_ENCODE;
            if (Sys1InitFlag.bc.bEncodeModuleInitEnable) {//add by zyz
                Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                Fun1ModuleChange();
                Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                Fun2ModuleChange();
                PostSche1Message(MSG_SCHEDULER1_ENCODE/*,0,0,0,0,0,0,0*/);
            }
            PostFun1Message(MSG_FUNCTION1_ENCODE_PROC , MSG_FUNCTION1_ENC_SELINIT , ENCODE_MIC_REC ,0,0/*,0,0,0*/);
            break;
#endif        
      
#ifdef  BROWSER_MODULE        
        case BROWSER_MODULE_ID: //BROWSER
            DisplayFlag = DISP_BROWSER;
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
            Fun2ModuleChange();
            PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_INIT,FIND_FILE_TYPE_ALL,0,0/*,0,0,0*/);
            break; 
#endif            
 
#ifdef  GAME_MODULE            
        case GAME_MODULE_ID: //GAME
            DisplayFlag = DISP_GAME;
            Fun2ModuleChange();
            PostSche2Message(MSG_SCHEDULER2_GAME,0,0,0,0,0,0,0);
            PostFun2Message(MSG_FUNCTION2_GAME,GAME_BLOCK_INIT,0,0,0,0,0,0);
            break;
#endif         
      
#ifdef  SETMENU_MODULE         
        case SETMENU_MODULE_ID:
            DisplayFlag = DISP_SETTING;
            //PreDisplayFlag = DisplayFlag;
            PostSche2Message(MSG_SCHEDULER2_SETMENU/*,0,0,0,0,0,0,0*/);
            PostFun2Message(MSG_FUNCTION2_MENU,MSG_FUN2_MENU_INIT,(unsigned int)(SETUP_MENU_TXT>>16),(unsigned int)(SETUP_MENU_TXT&0xFFFF),0/*,0,0,0*/);
            break;
#endif         
        default:
            break;
    }
    PreDisplayFlag = DisplayFlag;
}
/*------------------------------------------------------------------------------
Function: 按键扫描程序
Input   : 
Output  : 
说明：  1、确定按键状态及键值
        2、判断Hold状态及其处理
------------------------------------------------------------------------------*/
void MainMenuKey(void)
{
    if(KeyFlag.bc.bKeyProssed == 0) return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
    
    switch (KeyVal) {
    /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_SHORT_UP:
    case KEY_VAL_PLAY_LONG_UP:
        break;
    */
    /*
    case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS_START:
    case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_SHORT_UP:
    case KEY_VAL_REC_LONG_UP:
        break;
    */
    /*case KEY_VAL_MENU_PRESS_START:
    case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_SHORT_UP:
    case KEY_VAL_MENU_LONG_UP:*/
    case KEY_VAL_MENU_DOWN:
        MainMenuModeKey();
        break;
    /*    
    case KEY_VAL_UP_PRESS_START:
    case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_LONG_UP:
    case KEY_VAL_UP_PRESS:
    case KEY_VAL_UP_DOWN:
        break;*/
    /*    
    case KEY_VAL_DOWN_PRESS_START:
    case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_LONG_UP:
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_DOWN_DOWN:
        break;
    */    
    
    /*case KEY_VAL_FFD_PRESS_START:
    case KEY_VAL_FFD_SHORT_UP:
    case KEY_VAL_FFD_LONG_UP:
        break;*/
    case KEY_VAL_FFD_DOWN:
    case KEY_VAL_FFD_PRESS:
        MenuDispFlag = 1;
            if (++ MenuId > (MODULE_MAX_NUM-1))
            MenuId = 0;
        break;
        
    /*case KEY_VAL_FFW_PRESS_START:
    case KEY_VAL_FFW_SHORT_UP:
    case KEY_VAL_FFW_LONG_UP:*/
    case KEY_VAL_FFW_DOWN:
    case KEY_VAL_FFW_PRESS:
        MenuDispFlag = 1;
        if (MenuId == 0)
                MenuId = (MODULE_MAX_NUM-1);
        else 
            MenuId -- ;
        break;
        
    default:
        break;
    }
}

/*------------------------------------------------------------------------------
Function: 按键扫描程序
Input   : 
Output  : 
说明：  1、确定按键状态及键值
        2、判断Hold状态及其处理
------------------------------------------------------------------------------*/
void ManiMenuDisplay(void)
{
    unsigned int color_tmp;
    
    if (MenuDispFlag == 0) return;
    
    MenuDispFlag = 0;;
    
    MusicSetFreq51();
    DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, ((unsigned long)(MAIN_MENU_MUSIC + (unsigned long) (MenuId)  *MAX_PIC_SIZE)));
    
    DisplayPicture(MMENU_HOLD_PIC_X, MMENU_HOLD_PIC_Y, MMENU_HOLD_PIC_XSIZE, MMENU_HOLD_PIC_YSIZE, (MMENU_HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MMENU_HOLD_PIC_SIZE));
    
    DisplayPicture(MMENU_BATT_PIC_X, MMENU_BATT_PIC_Y, MMENU_BATT_PIC_XSIZE, MMENU_BATT_PIC_YSIZE, (MMENU_BATT_PIC_1 + (unsigned long)(BatteryLevel) * MMENU_BATT_PIC_SIZE));
    
    color_tmp = LCD_GetColor();
    LCD_SetColor(COLOR_BLACK);
    DisplayMenuStr(MAIN_MENU_TXT_X, MAIN_MENU_TXT_Y, MAIN_MENU_TXT_XSIZE, MAIN_MENU_TXT_YSIZE, LCD_TEXTALIGN_CENTER, ((unsigned long)(MAIN_MENU_MUSIC_TXT + (unsigned long) (MenuId)  *MENU_PER_BYTE)));
    LCD_SetColor(color_tmp);
    
    LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
    MusicSetFreq51Return();
}
//******************************************************************************
