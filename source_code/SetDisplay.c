
#define _IN_SETDISPLAY

#include "Macro.h"
#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"
#include "Lcd.h"
#include "LcdChar.h"
#include "KeyMacro.h"
#include "SetMenuMacro.h"
#include "SetMenuStruct.h"
#include "SetMenuGlobal.h"

//unsigned char SetVerString[]  __attribute__((section(".setmenu_data, \"aw\"")))  = "Ver:      " ;
//unsigned char SetDateString[] __attribute__((section(".setmenu_data, \"aw\"")))  = "Date:     " ;
//unsigned char SetFlasgString[] __attribute__((section(".setmenu_data, \"aw\""))) = "FlashMen: " ;
//unsigned char SetCardString[]  __attribute__((section(".setmenu_data, \"aw\""))) = "CardMen:  " ;
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void DISP_Set(void){
    LCD_RECT r;
    unsigned int  i,j,k,m,x,MaxValTemp;
    unsigned int  cur_id;
    unsigned int  MenuStrBuf[MENU_PER_BYTE];
    unsigned int  color_temp;
    unsigned int  Temp_DrawMode;
    unsigned long MenuNFAddr;
    unsigned long SelIconAddr;
    
    if (DispMsgArray[0] != MSG_DISP_SET) {
        return;
    } else {
        DispMsgArray[0] = MSG_DISP_NONE;
    }
    MusicSetFreq51();
    
    MenuNFAddr =   DispMsgArray[2] & 0x0000FFFF;
    MenuNFAddr <<= 16;
    MenuNFAddr |=  DispMsgArray[3] & 0x0000FFFF; 
    MenuNFAddr += MenuLogicAddress;
    
    LanguageResoureOffset = (unsigned long)(Language) * MENU_LANGUE_PER_BYTE;
    if(Language < LANGUAGE_MAX_COUNT) MenuNFAddr += LanguageResoureOffset;//2006-2-23 21:03

    i = DispMsgArray[4];        //ValPrev
    j = DispMsgArray[5];        //ValNow
    cur_id = DispMsgArray[6];   //ValSel
    
    r.x0 = SETUP_BAR_PIC_X + 2;
    r.x1 = SETUP_BAR_PIC_X + SETUP_BAR_PIC_XSIZE - 1;

    FlashReadLogic(MenuNFAddr, MenuStrBuf, MENU_PER_BYTE);
    Byte2Word(MenuStrBuf, MenuStrBuf, MENU_PER_BYTE);
    k = MenuStrBuf[0];          //MaxVal
    MaxValTemp = MenuStrBuf[0];
    
    switch(DispMsgArray[1])
    {
        case MSG_DISP_SET_INIT:
        {
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, SETUP_BKGROUND_PIC);
            DisplayPicture(SETUP_HOLD_PIC_X, SETUP_HOLD_PIC_Y, SETUP_HOLD_PIC_XSIZE, SETUP_HOLD_PIC_YSIZE, (SETUP_HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * SETUP_HOLD_PIC_SIZE));
            DisplayPicture(SETUP_BATT_PIC_X, SETUP_BATT_PIC_Y, SETUP_BATT_PIC_XSIZE, SETUP_BATT_PIC_YSIZE, (NFA_SETUP_BATT_PIC + (unsigned long)(BatteryLevel) * SETUP_BATT_PIC_SIZE));
        }
        case MSG_DISP_SET_INIT2:
        {
            r.x0 = SETUP_MENU_TXT_X;
            r.y0 = SETUP_MENU_TXT_Y;
            r.x1 = SETUP_MENU_TXT_X + SETUP_MENU_TXT_XSIZE - 1;
            r.y1 = SETUP_MENU_TXT_Y + SETUP_MENU_TXT_YSIZE - 1;
            LCD_DispStringInRect(&r, &r, &MenuStrBuf[1], LCD_TEXTALIGN_CENTER);
            
            LCD_SetDispRect(SETUP_AREABK_PIC_X, SETUP_AREABK_PIC_Y, (SETUP_AREABK_PIC_X + SETUP_AREABK_PIC_XSIZE - 1), (SETUP_AREABK_PIC_Y + SETUP_AREABK_PIC_YSIZE - 1));
            //DisplayPicture(SETUP_AREABK_PIC_X, SETUP_AREABK_PIC_Y, SETUP_AREABK_PIC_XSIZE, SETUP_AREABK_PIC_YSIZE, SETUP_AREABK_PIC); 
            
            r.x0 = SETUP_BAR_PIC_X + 2;
            r.x1 = SETUP_BAR_PIC_X + SETUP_BAR_PIC_XSIZE - 1;
            
            color_temp = LCD_GetColor();
            LCD_SetColor(COLOR_BLACK);
            
            m = i / SET_SCREEN_PER_LINE;
            m = m * SET_SCREEN_PER_LINE;
            k -= m;
            for(j = 0; ((j < k) && (j < SET_SCREEN_PER_LINE)); j++){
                r.y0 = SETUP_BAR_PIC_Y + j * SETUP_BAR_PIC_YSIZE;
                
                DisplayPicture(SETUP_ICON_PIC_X,   r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_OFF);
                
                FlashReadLogic(MenuNFAddr+(unsigned long)((j + 1 + m) * MENU_PER_BYTE), MenuStrBuf, MENU_PER_BYTE);
                Byte2Word(MenuStrBuf, MenuStrBuf, MENU_PER_BYTE);
                r.y0 += 2;
                r.y1 =  r.y0 + CH_CHAR_YSIZE - 1;
                LCD_DispStringInRect(&r, &r, MenuStrBuf, LCD_TEXTALIGN_LEFT);
            }
            
            
            LCD_SetColor(COLOR_YELLOW);
            
            r.y0 = SETUP_BAR_PIC_Y + (i-m) * SETUP_BAR_PIC_YSIZE;
            DisplayPicture(SETUP_BAR_PIC_X, r.y0, SETUP_BAR_PIC_XSIZE, SETUP_BAR_PIC_YSIZE, SETUP_BAR_PIC);
            DisplayPicture(SETUP_ICON_PIC_X,   r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_ON);
            
            FlashReadLogic(MenuNFAddr+(unsigned long)((i+1)*MENU_PER_BYTE), MenuStrBuf, MENU_PER_BYTE);
            Byte2Word(MenuStrBuf, MenuStrBuf, MENU_PER_BYTE);
            r.y0 += 2;
            r.y1 = r.y0 + CH_CHAR_YSIZE - 1;
            LCD_DispStringInRect(&r, &r, MenuStrBuf, LCD_TEXTALIGN_LEFT);
            
            LCD_SetColor(color_temp);
            
            LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
            
            i = 0;
            if(MaxValTemp > 1) i = (unsigned int)(((unsigned long)(i-m) * (SET_SCROLL_BAR_COUNT))/(MaxValTemp - 1));
            
            DisplayPicture(SETUP_SCROLL_PIC_X, (SETUP_SCROLL_PIC_Y + SetPrevScrollY), SETUP_SCROLL_PIC_XSIZE, SETUP_SCROLL_PIC_YSIZE, SETUP_SCROLL_BACKGROUND_PIC);
            DisplayPicture(SETUP_SCROLL_PIC_X, (SETUP_SCROLL_PIC_Y + i), SETUP_SCROLL_PIC_XSIZE, SETUP_SCROLL_PIC_YSIZE, SETUP_SCROLL_FOREROUND_PIC);
            SetPrevScrollY = i;
            break;
        }
        
        case MSG_DISP_SET_SELECT:
        {
            x = j / SET_SCREEN_PER_LINE;
            if((i / SET_SCREEN_PER_LINE) != (j / SET_SCREEN_PER_LINE)) {    //Page Change
                DisplayPicture(SETUP_AREABK_PIC_X, SETUP_AREABK_PIC_Y, SETUP_AREABK_PIC_XSIZE, SETUP_AREABK_PIC_YSIZE, SETUP_AREABK_PIC); 
                k -= x*SET_SCREEN_PER_LINE;
                
                color_temp = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                
                for (m = 0; ((m < k) && (m < SET_SCREEN_PER_LINE)); m++) {
                    
                    r.y0 = SETUP_BAR_PIC_Y + m * SETUP_BAR_PIC_YSIZE;
                    
                    if ((m + x * SET_SCREEN_PER_LINE) == j) {
                        DisplayPicture(SETUP_BAR_PIC_X, r.y0, SETUP_BAR_PIC_XSIZE, SETUP_BAR_PIC_YSIZE, SETUP_BAR_PIC);
                        LCD_SetColor(COLOR_YELLOW);
                    }
                    
                    if ((m + x * SET_SCREEN_PER_LINE) == cur_id) {
                        DisplayPicture(SETUP_ICON_PIC_X, r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_ON);
                    } else {
                        DisplayPicture(SETUP_ICON_PIC_X,   r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_OFF);
                    }
                    
                    r.y0 += 2;
                    r.y1 = r.y0 + CH_CHAR_YSIZE - 1;
                    FlashReadLogic(MenuNFAddr+(unsigned long)((m + 1 + x * SET_SCREEN_PER_LINE) * MENU_PER_BYTE), MenuStrBuf, MENU_PER_BYTE);
                    Byte2Word(MenuStrBuf, MenuStrBuf, MENU_PER_BYTE);
                    LCD_DispStringInRect(&r, &r, MenuStrBuf, LCD_TEXTALIGN_LEFT);
                    
                    if((m+x*SET_SCREEN_PER_LINE) == j) LCD_SetColor(COLOR_BLACK);
                }
                LCD_SetColor(color_temp);
            }else{
                color_temp = LCD_GetColor();
                
                m = i % SET_SCREEN_PER_LINE;
                k = j % SET_SCREEN_PER_LINE;
                
                r.y0 = SETUP_BAR_PIC_Y + m * SETUP_BAR_PIC_YSIZE;
                DisplayPicture(SETUP_AREABK_PIC_X ,r.y0, SETUP_AREABK_PIC_XSIZE, SETUP_BAR_PIC_YSIZE, (SETUP_AREABK_PIC + ((unsigned long)(r.y0 - SETUP_AREABK_PIC_Y) * (SETUP_AREABK_PIC_XSIZE) * 2)));
                if (i == cur_id) {
                    DisplayPicture(SETUP_ICON_PIC_X, r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_ON);
                } else {
                    DisplayPicture(SETUP_ICON_PIC_X,   r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_OFF);
                }
                LCD_SetColor(COLOR_BLACK);
                FlashReadLogic(MenuNFAddr+(unsigned long)((i + 1) * MENU_PER_BYTE), MenuStrBuf, MENU_PER_BYTE);
                Byte2Word(MenuStrBuf, MenuStrBuf, MENU_PER_BYTE);
                r.y0 += 2;
                r.y1 = r.y0 + CH_CHAR_YSIZE - 1;
                LCD_DispStringInRect(&r, &r, MenuStrBuf, LCD_TEXTALIGN_LEFT);
                
                
                r.y0 = SETUP_BAR_PIC_Y + k * SETUP_BAR_PIC_YSIZE;
                DisplayPicture(SETUP_BAR_PIC_X, r.y0, SETUP_BAR_PIC_XSIZE, SETUP_BAR_PIC_YSIZE, SETUP_BAR_PIC);
                if (j == cur_id) {
                    DisplayPicture(SETUP_ICON_PIC_X, r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_ON);
                } else {
                    DisplayPicture(SETUP_ICON_PIC_X,   r.y0, SETUP_ICON_PIC_XSIZE,   SETUP_ICON_PIC_YSIZE, SETUP_ICON_SEL_OFF);
                }
                LCD_SetColor(COLOR_YELLOW);
                FlashReadLogic(MenuNFAddr+(unsigned long)((j + 1) * MENU_PER_BYTE), MenuStrBuf, MENU_PER_BYTE);
                Byte2Word(MenuStrBuf, MenuStrBuf, MENU_PER_BYTE);
                r.y0 += 2;
                r.y1 = r.y0 + CH_CHAR_YSIZE - 1;
                LCD_DispStringInRect(&r, &r, MenuStrBuf, LCD_TEXTALIGN_LEFT);
                
                LCD_SetColor(color_temp);
            }
            
            i = 0;
            if(MaxValTemp > 1) i = (unsigned int)(((unsigned long)(j) * (SET_SCROLL_BAR_COUNT))/(MaxValTemp - 1));
            DisplayPicture(SETUP_SCROLL_PIC_X, (SETUP_SCROLL_PIC_Y + SetPrevScrollY), SETUP_SCROLL_PIC_XSIZE, SETUP_SCROLL_PIC_YSIZE, SETUP_SCROLL_BACKGROUND_PIC);
            DisplayPicture(SETUP_SCROLL_PIC_X, (SETUP_SCROLL_PIC_Y + i), SETUP_SCROLL_PIC_XSIZE, SETUP_SCROLL_PIC_YSIZE, SETUP_SCROLL_FOREROUND_PIC);
            SetPrevScrollY = i;
            break;
        }
        default:
            break;
    }
    
    LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
    
    MusicSetFreq51Return();
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
#define     COPYRIGHT_BOX_X1        74
#define     COPYRIGHT_BOX_Y1        79
#define     COPYRIGHT_BOX_X2        246
#define     COPYRIGHT_BOX_Y2        161
SET_MENU_TEXT
void ConvertMemMB(unsigned int *pBuff, unsigned long *Mem)
{
    unsigned long Frememery;
    unsigned int i;
    
    Frememery = (*Mem) >> 10;
    *(pBuff + 3) = Frememery%10 + '0';
    Frememery = Frememery / 10 ; 
    *(pBuff + 2) = (Frememery%10) + '0';
    Frememery = Frememery / 10 ; 
    *(pBuff + 1) = (Frememery%10) + '0';
//    Frememery = Frememery / 10 ; 
    *(pBuff + 0) = (Frememery/10) + '0';
    for(i = 0; i < 3; i++) {
        if (*(pBuff + i) == '0'){
            *(pBuff + i) = ' ';
        } else {
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
void DispCopyRightFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue)
{
    unsigned int i, color_tmp;
    unsigned int MasterVersion, SlaveVersion, SmallVersion;
    unsigned int FirmwareYear , FirmwareMonth, FirmwareDate;
    unsigned long temp,FlashTotalMem, CardTotalMem;
    unsigned int FillDialogBuf[128];
    unsigned int *pFillDialogBuf = FillDialogBuf;
    LCD_RECT       r;
    
    if(SetMenuAdjust == 1){
        return;
    }
    if(key == KEY_VAL_FFW_SHORT_UP){
        return;
    }
    MusicSetFreq51();
    
    color_tmp = LCD_GetColor();
    LCD_SetColor(0);
    
    LCD_DrawHLine(COPYRIGHT_BOX_X1, COPYRIGHT_BOX_Y1, COPYRIGHT_BOX_X2);
    LCD_DrawHLine(COPYRIGHT_BOX_X1, COPYRIGHT_BOX_Y2, COPYRIGHT_BOX_X2);
    LCD_DrawVLine(COPYRIGHT_BOX_X1, COPYRIGHT_BOX_Y1, COPYRIGHT_BOX_Y2);
    LCD_DrawVLine(COPYRIGHT_BOX_X2,COPYRIGHT_BOX_Y1, COPYRIGHT_BOX_Y2);
    
    LCD_SetColor(0xE79C);
    LCD_FillRect((COPYRIGHT_BOX_X1 + 1), (COPYRIGHT_BOX_Y1 + 1), (COPYRIGHT_BOX_X2 - 1), (COPYRIGHT_BOX_Y2 - 1));
    
    FlashTotalMem = GetTotalMem(0);
    if(FlashTotalMem > 0x3e800000) FlashTotalMem -= 1000000;

#ifdef SYSTEM_SD_CARD_SP    
    LCD_SetColor(0);
    r.x0 = COPYRIGHT_BOX_X1 + 4;
    r.y0 = COPYRIGHT_BOX_Y1 + ((COPYRIGHT_BOX_Y2 - COPYRIGHT_BOX_Y1) >> 1 - 8);
    r.x1 = COPYRIGHT_BOX_X2 - 4;
    r.y1 = r.y0 + 16;
    LCD_DispStringInRect(&r,&r,"Waiting...", LCD_TEXTALIGN_CENTER);
        
    if (SDCARD_INSERT()) {
        MusicCodecMute(1);
        AVI_MP3_DECODE_DISABLE;
        //------------------------------------------------------------
#if(CPU_TYPE == _RK2608_) 
	    write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0x77) | 0x08);
#endif
        PeriphFlag.bc.EarPlugIn = 0;
        PeriphFlag.bc.EarEjectout = 0;
        //------------------------------------------------------------
        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0004);
        SysFreeMemeryCard = GetFreeMem(1);
        CardTotalMem = GetTotalMem(1);
        if (MemorySelect == 0) {
            MediaSetup(0);
        }
        //------------------------------------------------------------
        AVI_MP3_DECODE_ENABLE;
        MusicCodecMute(0);
    } else {
        SysFreeMemeryCard = 0;
        CardTotalMem = 0;
    }
    LCD_SetColor(0xE79C);
    LCD_FillRect((COPYRIGHT_BOX_X1 + 1), (COPYRIGHT_BOX_Y1 + 1), (COPYRIGHT_BOX_X2 - 1), (COPYRIGHT_BOX_Y2 - 1));
#endif

    GetFirmwareVersion(&MasterVersion, &SlaveVersion,&SmallVersion, &FirmwareYear,&FirmwareMonth,&FirmwareDate);
    
//    memcpy(pFillDialogBuf,SetVerString,9);
//    pFillDialogBuf += 9;
//    *pFillDialogBuf++ = MasterVersion/10 + '0';
//    *pFillDialogBuf++ = MasterVersion%10 + '0';
//    *pFillDialogBuf++ = '.';
//    *pFillDialogBuf++ = SlaveVersion/10 + '0';
//    *pFillDialogBuf++ = SlaveVersion%10 + '0';
//    *pFillDialogBuf++ = '.';
//    
//    *pFillDialogBuf++ = SmallVersion / 1000 + '0';
//    SmallVersion = SmallVersion % 1000;
//    *pFillDialogBuf++ = SmallVersion/100 + '0';
//    SmallVersion = SmallVersion % 100;
//    *pFillDialogBuf++ = SmallVersion / 10 + '0';
//    *pFillDialogBuf++ = SmallVersion % 10 + '0';
//    *pFillDialogBuf++ = '\n';
//    
//    memcpy(pFillDialogBuf,SetDateString,9);
//    pFillDialogBuf += 9;
//    
//    *pFillDialogBuf++ = FirmwareYear / 1000 + '0';
//    FirmwareYear = FirmwareYear % 1000;
//    *pFillDialogBuf++ = FirmwareYear/100 + '0';
//    FirmwareYear = FirmwareYear % 100;
//    *pFillDialogBuf++ = FirmwareYear / 10 + '0';
//    *pFillDialogBuf++ = FirmwareYear % 10 + '0';
//    
//    *pFillDialogBuf++ = '-';
//    *pFillDialogBuf++ = FirmwareMonth/10 + '0';
//    *pFillDialogBuf++ = FirmwareMonth%10 + '0';
//    *pFillDialogBuf++ = '-';
//    *pFillDialogBuf++ = FirmwareDate/10 + '0';
//    *pFillDialogBuf++ = FirmwareDate%10 + '0';
//    *pFillDialogBuf++ = '\n';
//    
//    memcpy(pFillDialogBuf,SetFlasgString,9);
//    pFillDialogBuf += 9;
//    ConvertMemMB(pFillDialogBuf, &SysFreeMemeryFlash);
//    pFillDialogBuf += 4;
//    *pFillDialogBuf++ = '/';
//    ConvertMemMB(pFillDialogBuf, &FlashTotalMem);
//    pFillDialogBuf += 4;
//    *pFillDialogBuf++ = 'M';
//    *pFillDialogBuf++ = 'B';
//    
//#ifdef SYSTEM_SD_CARD_SP 
//    *pFillDialogBuf++ = '/';
//    memcpy(pFillDialogBuf,SetCardString,9);
//    pFillDialogBuf += 9;
//    ConvertMemMB(pFillDialogBuf, &SysFreeMemeryCard);
//    pFillDialogBuf += 4;
//    *pFillDialogBuf++ = '/';
//    ConvertMemMB(pFillDialogBuf, &CardTotalMem);
//    pFillDialogBuf += 4;
//    *pFillDialogBuf++ = 'M';
//    *pFillDialogBuf++ = 'B';
//#endif
//    *pFillDialogBuf++ = 0;
//    *pFillDialogBuf++ = 0;
    
    FillDialogBuf[0] = 'V';
    FillDialogBuf[1] = 'e';
    FillDialogBuf[2] = 'r';
    FillDialogBuf[3] = ':';
    FillDialogBuf[4] = ' ';
    FillDialogBuf[5] = MasterVersion/10 + '0';
    FillDialogBuf[6] = MasterVersion%10 + '0';
    FillDialogBuf[7] = '.';
    FillDialogBuf[8] = SlaveVersion/10 + '0';
    FillDialogBuf[9] = SlaveVersion%10 + '0';
    FillDialogBuf[10] = '.';
    
    FillDialogBuf[14] = SmallVersion%10 + '0';
    SmallVersion = SmallVersion / 10 ; 
    FillDialogBuf[13] = SmallVersion % 10 + '0';
    SmallVersion = SmallVersion / 10 ; 
    FillDialogBuf[12] = SmallVersion%10 + '0';    
    FillDialogBuf[11] = SmallVersion/10 + '0';
    FillDialogBuf[15] = '\n';
    
    FillDialogBuf[16] = 'D';
    FillDialogBuf[17] = 'a';
    FillDialogBuf[18] = 't';
    FillDialogBuf[19] = 'e';
    FillDialogBuf[20] = ':';
    
    FillDialogBuf[24] = FirmwareYear%10 + '0';
    FirmwareYear = FirmwareYear / 10 ; 
    FillDialogBuf[23] = FirmwareYear % 10 + '0';
    FirmwareYear = FirmwareYear / 10 ; 
    FillDialogBuf[22] = FirmwareYear%10 + '0';    
    FillDialogBuf[21] = FirmwareYear/10 + '0';
    
    FillDialogBuf[25] = '-';
    FillDialogBuf[26] = FirmwareMonth/10 + '0';
    FillDialogBuf[27] = FirmwareMonth%10 + '0';
    FillDialogBuf[28] = '-';
    FillDialogBuf[29] = FirmwareDate/10 + '0';
    FillDialogBuf[30] = FirmwareDate%10 + '0';
    FillDialogBuf[31] = '\n';
    
    FillDialogBuf[32] = 'F';
    FillDialogBuf[33] = 'l';
    FillDialogBuf[34] = 'a';
    FillDialogBuf[35] = 's';
    FillDialogBuf[36] = 'h';
    FillDialogBuf[37] = 'M';
    FillDialogBuf[38] = 'e';
    FillDialogBuf[39] = 'm';
    FillDialogBuf[40] = ':';
    
    ConvertMemMB(&FillDialogBuf[41], &SysFreeMemeryFlash);
        
    FillDialogBuf[45] = '/';
    
    ConvertMemMB(&FillDialogBuf[46], &FlashTotalMem);
        
    FillDialogBuf[50] = 'M';
    FillDialogBuf[51] = 'B';
    
#ifndef SYSTEM_SD_CARD_SP 
    FillDialogBuf[52] = 0;
#else
    FillDialogBuf[52] = '\n';
    FillDialogBuf[53] = 'C';
    FillDialogBuf[54] = 'a';
    FillDialogBuf[55] = 'r';
    FillDialogBuf[56] = 'd';
    FillDialogBuf[57] = 'M';
    FillDialogBuf[58] = 'e';
    FillDialogBuf[59] = 'm';
    FillDialogBuf[60] = ':';
    FillDialogBuf[61] = ' ';
    ConvertMemMB(&FillDialogBuf[62], &SysFreeMemeryCard);
    FillDialogBuf[66] = '/';
    ConvertMemMB(&FillDialogBuf[67], &CardTotalMem);
    FillDialogBuf[71] = 'M';
    FillDialogBuf[72] = 'B';
    FillDialogBuf[73] = 0;
#endif

    r.x0 = COPYRIGHT_BOX_X1 + 4;
    r.x1 = COPYRIGHT_BOX_X2 - 4;
#ifdef SYSTEM_SD_CARD_SP 
    r.y0 = COPYRIGHT_BOX_Y1 + 8;
    r.y1 = COPYRIGHT_BOX_Y2 - 8;
#else
    r.y0 = COPYRIGHT_BOX_Y1 + 16 ;
    r.y1 = COPYRIGHT_BOX_Y2 - 16;
#endif
    LCD_SetColor(0);
    LCD_DispStringInRect(&r,&r,FillDialogBuf, LCD_TEXTALIGN_CENTER);
        
    LCD_SetColor(color_tmp);
    
    MusicSetFreq51Return();
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
SET_MENU_TEXT
void UserEqSettingDisplay(unsigned int PreVal, unsigned int CurVal)
{
    //unsigned int StrBuf[8];
    unsigned int color_tmp;
    
//    switch(cont){
//        case 0:
//            DisplayPicture(182, 184, EQ_BUTTON_FU_XSIZE, EQ_BUTTON_FU_YSIZE, NFA_EQ_BUTTON_FU);
//            color_tmp = LCD_GetColor();
//            LCD_SetColor(COLOR_WHITE);
//            DisplayMenuStr(182, 192, EQ_BUTTON_FU_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, NFA_DIALOG_NO_STR);
//            LCD_SetColor(color_tmp);
//            DisplayPicture(SELECT_EQ_X, SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_SELECT_EQ + (unsigned long)(SELECT_EQ_XSIZE*SELECT_EQ_YSIZE*2)*(*(pValue)));
//            DisplayPicture(SELECT_EQ_X+(SELECT_EQ_XSIZE+28), SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_EMPTY_EQ + (unsigned long)(EMPTY_EQ_XSIZE*EMPTY_EQ_YSIZE*2)*(*(pValue+1)));
//            break;
//        case 4:
//            DisplayPicture(63, 184, EQ_BUTTON_FU_XSIZE, EQ_BUTTON_FU_YSIZE, NFA_EQ_BUTTON_FU);
//            color_tmp = LCD_GetColor();
//            LCD_SetColor(COLOR_WHITE);
//            DisplayMenuStr(63, 192, EQ_BUTTON_FU_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, NFA_DIALOG_YES_STR);
//            LCD_SetColor(color_tmp);
//            DisplayPicture(SELECT_EQ_X + 3*(SELECT_EQ_XSIZE+28), SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_EMPTY_EQ + (unsigned long)(EMPTY_EQ_XSIZE*EMPTY_EQ_YSIZE*2)*(*(pValue+3)));
//            DisplayPicture(SELECT_EQ_X + 4*(SELECT_EQ_XSIZE+28), SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_SELECT_EQ + (unsigned long)(SELECT_EQ_XSIZE*SELECT_EQ_YSIZE*2)*(*(pValue+4)));
//            break;
//        case 5:
//            DisplayPicture(63, 184, EQ_BUTTON_EM_XSIZE, EQ_BUTTON_EM_YSIZE, NFA_EQ_BUTTON_EM);
//            DisplayPicture(182, 184, EQ_BUTTON_FU_XSIZE, EQ_BUTTON_FU_YSIZE, NFA_EQ_BUTTON_FU);
//            color_tmp = LCD_GetColor();
//            LCD_SetColor(COLOR_YELLOW);
//            DisplayMenuStr(63, 192, EQ_BUTTON_FU_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, NFA_DIALOG_YES_STR);
//            LCD_SetColor(COLOR_WHITE);
//            DisplayMenuStr(182, 192, EQ_BUTTON_FU_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, NFA_DIALOG_NO_STR);
//            LCD_SetColor(color_tmp);
//            DisplayPicture(SELECT_EQ_X + 4*(SELECT_EQ_XSIZE+28), SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_EMPTY_EQ + (unsigned long)(EMPTY_EQ_XSIZE*EMPTY_EQ_YSIZE*2)*(*(pValue+4)));
//            break;
//        case 6:
//            DisplayPicture(63, 184, EQ_BUTTON_FU_XSIZE, EQ_BUTTON_FU_YSIZE, NFA_EQ_BUTTON_FU);
//            DisplayPicture(182, 184, EQ_BUTTON_EM_XSIZE, EQ_BUTTON_EM_YSIZE, NFA_EQ_BUTTON_EM);
//            color_tmp = LCD_GetColor();
//            LCD_SetColor(COLOR_WHITE);
//            DisplayMenuStr(63, 192, EQ_BUTTON_FU_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, NFA_DIALOG_YES_STR);
//            LCD_SetColor(COLOR_YELLOW);
//            DisplayMenuStr(182, 192, EQ_BUTTON_FU_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, NFA_DIALOG_NO_STR);
//            LCD_SetColor(color_tmp);
//            DisplayPicture(SELECT_EQ_X, SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_EMPTY_EQ + (unsigned long)(EMPTY_EQ_XSIZE*EMPTY_EQ_YSIZE*2)*(*(pValue)));
//            break;
//        default:
//            DisplayPicture(SELECT_EQ_X + (cont-1)*(SELECT_EQ_XSIZE+28), SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_EMPTY_EQ + (unsigned long)(EMPTY_EQ_XSIZE*EMPTY_EQ_YSIZE*2)*(*(pValue+(cont-1))));
//            DisplayPicture(SELECT_EQ_X + (cont)*(SELECT_EQ_XSIZE+28), SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_SELECT_EQ + (unsigned long)(SELECT_EQ_XSIZE*SELECT_EQ_YSIZE*2)*(*(pValue+cont)));
//            DisplayPicture(SELECT_EQ_X + (cont+1)*(SELECT_EQ_XSIZE+28), SELECT_EQ_Y, SELECT_EQ_XSIZE, SELECT_EQ_YSIZE, NFA_EMPTY_EQ + (unsigned long)(EMPTY_EQ_XSIZE*EMPTY_EQ_YSIZE*2)*(*(pValue+cont+1)));
//            break;
//    }
    
    color_tmp = LCD_GetColor();
    LCD_SetColor(COLOR_BLACK);
    if (PreVal < 5) {
        DisplayPicture((SETUP_USEREQ_NOSEL_PIC_X + PreVal * 50), SETUP_USEREQ_NOSEL_PIC_Y, SETUP_USEREQ_NOSEL_PIC_XSIZE, SETUP_USEREQ_NOSEL_PIC_YSIZE, (NFA_SETUP_USEREQ_NOSEL_PIC + (unsigned long)(GlobalMusicVar.UserEq[PreVal]) * SETUP_USEREQ_NOSEL_PIC_SIZE));
    } else if (PreVal < 6) {
        DisplayPicture(SETUP_USEREQ_BTN_YES_PIC_X, SETUP_USEREQ_BTN_YES_PIC_Y, SETUP_USEREQ_BTN_PIC_XSIZE, SETUP_USEREQ_BTN_PIC_YSIZE, SETUP_USEREQ_BTN_NOSEL_PIC);
        DisplayMenuStr(SETUP_USEREQ_BTN_YES_PIC_X, (SETUP_USEREQ_BTN_YES_PIC_Y + (SETUP_USEREQ_BTN_PIC_YSIZE >> 1) - 8),
                       SETUP_USEREQ_BTN_PIC_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_YES_TXT);
    } else {
        DisplayPicture(SETUP_USEREQ_BTN_NO_PIC_X, SETUP_USEREQ_BTN_NO_PIC_Y, SETUP_USEREQ_BTN_PIC_XSIZE, SETUP_USEREQ_BTN_PIC_YSIZE, SETUP_USEREQ_BTN_NOSEL_PIC);
        DisplayMenuStr(SETUP_USEREQ_BTN_NO_PIC_X, (SETUP_USEREQ_BTN_NO_PIC_Y + (SETUP_USEREQ_BTN_PIC_YSIZE >> 1) - 8),
                       SETUP_USEREQ_BTN_PIC_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_NO_TXT);
    }
    
    if (CurVal < 5) {
        DisplayPicture((SETUP_USEREQ_SEL_PIC_X + CurVal * 50), SETUP_USEREQ_SEL_PIC_Y, SETUP_USEREQ_SEL_PIC_XSIZE, SETUP_USEREQ_SEL_PIC_YSIZE, (NFA_SETUP_USEREQ_SEL_PIC + (unsigned long)(GlobalMusicVar.UserEq[CurVal]) * SETUP_USEREQ_SEL_PIC_SIZE));
    } else if (CurVal < 6) {
        DisplayPicture(SETUP_USEREQ_BTN_YES_PIC_X, SETUP_USEREQ_BTN_YES_PIC_Y, SETUP_USEREQ_BTN_PIC_XSIZE, SETUP_USEREQ_BTN_PIC_YSIZE, SETUP_USEREQ_BTN_SEL_PIC);
        DisplayMenuStr(SETUP_USEREQ_BTN_YES_PIC_X, (SETUP_USEREQ_BTN_YES_PIC_Y + (SETUP_USEREQ_BTN_PIC_YSIZE >> 1) - 8),
                       SETUP_USEREQ_BTN_PIC_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_YES_TXT);
    } else {
        DisplayPicture(SETUP_USEREQ_BTN_NO_PIC_X, SETUP_USEREQ_BTN_NO_PIC_Y, SETUP_USEREQ_BTN_PIC_XSIZE, SETUP_USEREQ_BTN_PIC_YSIZE, SETUP_USEREQ_BTN_SEL_PIC);
        DisplayMenuStr(SETUP_USEREQ_BTN_NO_PIC_X, (SETUP_USEREQ_BTN_NO_PIC_Y + (SETUP_USEREQ_BTN_PIC_YSIZE >> 1) - 8),
                       SETUP_USEREQ_BTN_PIC_XSIZE, CH_CHAR_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_NO_TXT);
    }
    LCD_SetColor(color_tmp);
}
//------------------------------------------------------------------------------
