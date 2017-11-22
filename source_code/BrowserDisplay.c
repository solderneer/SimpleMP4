/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : BrowserDisplay.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#define _IN_BROWSERDISPLAY

#include "Macro.h"
#include "MsgDef.h"
#include "Resource.h"
#include "BrowserMacro.h"
#include "BrowserStruct.h"
#include "BrowserGlobal.h"
#include "KeyMacro.h"

#include "Global.h"

#include "Lcd.h"
#include "LcdChar.h"

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void DISP_Browser(void)
{
    BROWSER_STRUCT  *pBro;
    DIR_TREE_STRUCT *pDirInfo;
    LCD_RECT      r;
    unsigned long BrowserNFAddr;
    int           StrSize;
    unsigned int  i,j,k,m,n;
    unsigned int  color_temp;
    unsigned int  Temp_DrawMode;
    unsigned long FileTypeIconAddr;
    
    if (DispMsgArray[0] != MSG_DISP_BROWSER) return;
    DispMsgArray[0] = MSG_DISP_NONE;
        
    r.x0 = EXPLORER_BAR_PIC_X + 2;
    r.x1 = EXPLORER_BAR_PIC_X + EXPLORER_BAR_PIC_XSIZE - 1;
   
    pDirInfo  = (DIR_TREE_STRUCT *)(DispMsgArray[2]);
    pBro      = pDirInfo->pBrowser;
    
    k         = DispMsgArray[3];
    j         = DispMsgArray[4];
    
    MusicSetFreq51();
    switch(DispMsgArray[1])
    {
        case BROWSER_DISP_INIT:
        {
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, EXPLORER_BKGROUND_PIC);
            DisplayPicture(EXPLORER_HOLD_PIC_X, EXPLORER_HOLD_PIC_Y, EXPLORER_HOLD_PIC_XSIZE, EXPLORER_HOLD_PIC_YSIZE, (EXPLORER_HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * EXPLORER_HOLD_PIC_SIZE));
            DisplayPicture(EXPLORER_BATT_PIC_X, EXPLORER_BATT_PIC_Y, EXPLORER_BATT_PIC_XSIZE, EXPLORER_BATT_PIC_YSIZE, (NFA_EXPLORER_BATT_PIC + (unsigned long)(BatteryLevel) * EXPLORER_BATT_PIC_SIZE ));
            
            if (pDirInfo->CurPickFileType[0] == FIND_FILE_TYPE_TXT) {
                BrowserNFAddr = MAIN_MENU_TEXT_TXT;
            } else if (pDirInfo->CurPickFileType[0] == FIND_FILE_TYPE_AVI) {
                BrowserNFAddr = MAIN_MENU_VEDIO_TXT;
            } else if (pDirInfo->CurPickFileType[0] == FIND_FILE_TYPE_JPEG) {
                BrowserNFAddr = MAIN_MENU_PHOTO_TXT;
            } else if (pDirInfo->CurPickFileType[0] == FIND_FILE_TYPE_MUSIC) {
                BrowserNFAddr = MAIN_MENU_MUSIC_TXT;
            } else {
                BrowserNFAddr = MAIN_MENU_EXPLORER_TXT;
            }
            DisplayMenuStr(EXPLORER_TITLE_TXT_X, EXPLORER_TITLE_TXT_Y, EXPLORER_TITLE_TXT_XSIZE, EXPLORER_TITLE_TXT_YSIZE, LCD_TEXTALIGN_CENTER, BrowserNFAddr);
        }
        case BROWSER_DISP_INIT1:
        {
            LCD_SetDispRect(EXPLORER_AREABK_PIC_X, EXPLORER_AREABK_PIC_Y, EXPLORER_AREABK_PIC_X + EXPLORER_AREABK_PIC_XSIZE - 1, EXPLORER_AREABK_PIC_Y + EXPLORER_AREABK_PIC_YSIZE - 1);
            DisplayPicture(EXPLORER_AREABK_PIC_X, EXPLORER_AREABK_PIC_Y, EXPLORER_AREABK_PIC_XSIZE, EXPLORER_AREABK_PIC_YSIZE, EXPLORER_AREABK_PIC);
        }
        case BROWSER_DISP_INIT2:
        {
            LCD_SetDispRect(EXPLORER_AREABK_PIC_X, EXPLORER_AREABK_PIC_Y, EXPLORER_AREABK_PIC_X + EXPLORER_AREABK_PIC_XSIZE - 1, EXPLORER_AREABK_PIC_Y + EXPLORER_AREABK_PIC_YSIZE - 1);
            
            for(i = 0; ((i < BROWSER_SCREEN_PER_LINE) && ( k > 0)); i++,k--) {
                r.y0 = EXPLORER_BAR_PIC_Y + i * EXPLORER_BAR_PIC_YSIZE;
                
                color_temp = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                DisplayPicture(EXPLORER_AREABK_PIC_X, r.y0, EXPLORER_AREABK_PIC_XSIZE, EXPLORER_BAR_PIC_YSIZE, (EXPLORER_AREABK_PIC + (unsigned long)(r.y0 - EXPLORER_AREABK_PIC_Y) * EXPLORER_AREABK_PIC_XSIZE * 2));
                DisplayPicture(EXPLORER_ICON_PIC_X,   r.y0, EXPLORER_ICON_PIC_XSIZE,   EXPLORER_ICON_PIC_YSIZE, (EXPLORER_ICON_FOLDER_PIC + (unsigned long)(pBro->FileType) * EXPLORER_ICON_PIC_SIZE));
                if(i == j){
                    DisplayPicture(EXPLORER_BAR_PIC_X, r.y0, EXPLORER_BAR_PIC_XSIZE, EXPLORER_BAR_PIC_YSIZE, EXPLORER_BAR_PIC);
                    LCD_SetColor(COLOR_YELLOW);
                }
                
                r.y0 += 2;
                r.y1 =  r.y0 + CH_CHAR_YSIZE - 1;
                LCD_DispStringInRect(&r, &r, pBro->LongFileName, LCD_TEXTALIGN_LEFT);
                LCD_SetColor(color_temp);
                pBro = pBro->pNext;
            }
            LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
            
            i = 0;
            if((pDirInfo->DirNum + pDirInfo->FileNum) > 1)
                i = (unsigned int)(((unsigned long)(pDirInfo->CurId[pDirInfo->DirDeep]) * (BROWSER_SCROLL_BAR_COUNT-1))/(pDirInfo->DirNum + pDirInfo->FileNum-1));
                
            DisplayPicture(EXPLORER_SCROLL_PIC_X, EXPLORER_SCROLL_PIC_Y + PrevScrollY, EXPLORER_SCROLL_PIC_XSIZE, EXPLORER_SCROLL_PIC_YSIZE, EXPLORER_SCROLL_BACKGROUND_PIC);
            DisplayPicture(EXPLORER_SCROLL_PIC_X, (EXPLORER_SCROLL_PIC_Y + i), EXPLORER_SCROLL_PIC_XSIZE, EXPLORER_SCROLL_PIC_YSIZE, EXPLORER_SCROLL_FOREROUND_PIC);       
            PrevScrollY = i;
            break;
        }
        case BROWSER_DISP_SELE:
        {
            for(i = 0; i < BROWSER_SCREEN_PER_LINE; i++)
            {
                if((i == j) || (i == k))
                {
                    r.y0 = EXPLORER_BAR_PIC_Y + i * EXPLORER_BAR_PIC_YSIZE;
                    
                    color_temp = LCD_GetColor();
                    LCD_SetColor(COLOR_BLACK);
                    
                    DisplayPicture(EXPLORER_AREABK_PIC_X, r.y0, EXPLORER_AREABK_PIC_XSIZE, EXPLORER_BAR_PIC_YSIZE, (EXPLORER_AREABK_PIC + (unsigned long)(r.y0 - EXPLORER_AREABK_PIC_Y) * EXPLORER_AREABK_PIC_XSIZE * 2));
                    DisplayPicture(EXPLORER_ICON_PIC_X,   r.y0, EXPLORER_ICON_PIC_XSIZE,   EXPLORER_ICON_PIC_YSIZE, (EXPLORER_ICON_FOLDER_PIC + (unsigned long)(pBro->FileType) * EXPLORER_ICON_PIC_SIZE));
                    if( i == j) {
                        DisplayPicture(EXPLORER_BAR_PIC_X, r.y0, EXPLORER_BAR_PIC_XSIZE, EXPLORER_BAR_PIC_YSIZE, EXPLORER_BAR_PIC);
                        LCD_SetColor(COLOR_YELLOW);
                    }
                    
                    r.y0 += 2;
                    r.y1 =  r.y0 + CH_CHAR_YSIZE - 1;
                    LCD_DispStringInRect(&r, &r, pBro->LongFileName, LCD_TEXTALIGN_LEFT);
                    LCD_SetColor(color_temp);
                    
                    if((pDirInfo->DirNum + pDirInfo->FileNum) > 1)
                        n = (unsigned int)(((unsigned long)(pDirInfo->CurId[pDirInfo->DirDeep]) * (BROWSER_SCROLL_BAR_COUNT-1))/(pDirInfo->DirNum + pDirInfo->FileNum-1));
                    else
                        n = 0;
                    DisplayPicture(EXPLORER_SCROLL_PIC_X, EXPLORER_SCROLL_PIC_Y + PrevScrollY, EXPLORER_SCROLL_PIC_XSIZE, EXPLORER_SCROLL_PIC_YSIZE, EXPLORER_SCROLL_BACKGROUND_PIC);
                    DisplayPicture(EXPLORER_SCROLL_PIC_X, EXPLORER_SCROLL_PIC_Y + n, EXPLORER_SCROLL_PIC_XSIZE, EXPLORER_SCROLL_PIC_YSIZE, EXPLORER_SCROLL_FOREROUND_PIC);
                    PrevScrollY = n;
                }
                pBro = pBro->pNext;
            }  
            break;
        }
        default:
            break;
    }
    LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
    MusicSetFreq51Return();
}

#ifdef SYSTEM_SD_CARD_SP
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserMemSelectDisp(unsigned int mode, unsigned int id)
{
    unsigned int color_temp;
    unsigned int i;
    unsigned long BrowserNFAddr;
    LCD_RECT r;
    
    MusicSetFreq51();
    if(mode == 0){
        DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, EXPLORER_BKGROUND_PIC);
        
        DisplayPicture(EXPLORER_HOLD_PIC_X, EXPLORER_HOLD_PIC_Y, EXPLORER_HOLD_PIC_XSIZE, EXPLORER_HOLD_PIC_YSIZE, (EXPLORER_HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * EXPLORER_HOLD_PIC_SIZE));
        DisplayPicture(EXPLORER_BATT_PIC_X, EXPLORER_BATT_PIC_Y, EXPLORER_BATT_PIC_XSIZE, EXPLORER_BATT_PIC_YSIZE, (NFA_EXPLORER_BATT_PIC + (unsigned long)(BatteryLevel) * EXPLORER_BATT_PIC_SIZE ));
        
        DisplayMenuStr(EXPLORER_TITLE_TXT_X, EXPLORER_TITLE_TXT_Y, EXPLORER_TITLE_TXT_XSIZE, EXPLORER_TITLE_TXT_YSIZE, LCD_TEXTALIGN_CENTER, MAIN_MENU_EXPLORER_TXT);
    }
    
    r.x0 = EXPLORER_BAR_PIC_X + 2;
    r.x1 = EXPLORER_BAR_PIC_X + EXPLORER_BAR_PIC_XSIZE - 1;
    
    for (i = 0; i < DirTreeInfo.MemMedium; i ++) {
        r.y0 = EXPLORER_BAR_PIC_Y + i * EXPLORER_BAR_PIC_YSIZE;
        r.y1 = r.y0 + EXPLORER_BAR_PIC_YSIZE - 1;
        
        color_temp = LCD_GetColor();
        LCD_SetColor(COLOR_BLACK);
                
        DisplayPicture(EXPLORER_AREABK_PIC_X, r.y0, EXPLORER_AREABK_PIC_XSIZE, EXPLORER_BAR_PIC_YSIZE, (EXPLORER_AREABK_PIC + (unsigned long)(r.y0 - EXPLORER_AREABK_PIC_Y) * EXPLORER_AREABK_PIC_XSIZE * 2));
        DisplayPicture(EXPLORER_ICON_PIC_X,   r.y0, EXPLORER_ICON_PIC_XSIZE,   EXPLORER_ICON_PIC_YSIZE, EXPLORER_ICON_FOLDER_PIC);
                
        if(i == id){
            DisplayPicture(EXPLORER_BAR_PIC_X, r.y0, EXPLORER_BAR_PIC_XSIZE, EXPLORER_BAR_PIC_YSIZE, EXPLORER_BAR_PIC);
            LCD_SetColor(COLOR_YELLOW);
        }
        
        r.y0 += 2;
        r.y1 =  r.y0 + CH_CHAR_YSIZE - 1;
        DisplayMenuStr(r.x0, r.y0, r.x1 - r.x0, CH_CHAR_YSIZE, LCD_TEXTALIGN_LEFT, EXPLORER_MEMSEL_FLASH_TXT + (unsigned long)(i) * MENU_PER_BYTE);
        
        LCD_SetColor(color_temp);
    }
    MusicSetFreq51Return();         //add by zyz
}
#endif
//------------------------------------------------------------------------------
