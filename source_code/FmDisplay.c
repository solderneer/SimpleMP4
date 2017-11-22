/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FmDisplay.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/11       1.0            ORG
********************************************************************************
*/

#include "Macro.h"
#include "MsgDef.h"
#include "Struct.h"
#include "Resource.h"
#include "Global.h"
#include "Lcd.h"
#ifdef FM_MODULE

    #define _IN_FMDISPLAY

    #include "FmMacro.h"
    #include "FmStruct.h"
    #include "FmGlobal.h"

#include "Global.h"

#include "Lcd.h"
#define DEL_FM_SCREEN_PER_LINE      4

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void DISP_Fm(void)
{
    unsigned int      FmDispBuf[MENU_PER_BYTE];
    unsigned int      temp, temp1, TempColor, TempFlag;
    unsigned int      *pFmDispBuf;
    unsigned int      Temp_DrawMode;
    unsigned long     CharAddrL;
    unsigned long     TempDispFlag, TempDisplayFlag1;
    
    MusicSetFreq51();         //add by zyz
    TempColor = LCD_GetColor();
    
    TempDispFlag = FmDisplayFlag;
    FmDisplayFlag = MUSIC_DISPFLAG_NULL;
    TempFlag = 0x00000001;
    while (TempDispFlag != 0) {
        TempDisplayFlag1 = TempDispFlag & TempFlag;
        TempDispFlag   &= (~TempFlag);
        TempFlag = TempFlag << 1;
        
        if (TempDisplayFlag1 != 0) {
            switch (TempDisplayFlag1) {
            case FM_DISPFLAG_BACKGROUND:
            {
                DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, RADIO_BKGROUND_PIC);
                DisplayPicture(RADIO_FREQDOT_PIC_X, RADIO_FREQDOT_PIC_Y, RADIO_FREQDOT_PIC_XSIZE, RADIO_FREQDOT_PIC_YSIZE, RADIO_FREQDOT_PIC);
                LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
                break;
            }
            case FM_DISPFLAG_FREQ:
            {
                temp1 = GlobalFmVar.FmFreq / 10000;
                temp  = GlobalFmVar.FmFreq % 10000;
                if (temp1 > 0) {
                    DisplayPicture(RADIO_FREQNUM_PIC_X, RADIO_FREQNUM_PIC_Y, RADIO_FREQNUM_PIC_XSIZE, RADIO_FREQNUM_PIC_YSIZE, (NFA_RADIO_FREQNUM_PIC + (unsigned long)(temp1) * RADIO_FREQNUM_PIC_SIZE));
                } else {
                    DisplayPicture(RADIO_FREQNUM_PIC_X, RADIO_FREQNUM_PIC_Y, RADIO_FREQNUM_PIC_XSIZE, RADIO_FREQNUM_PIC_YSIZE, (NFA_RADIO_FREQNUM_PIC + (unsigned long)(10) * RADIO_FREQNUM_PIC_SIZE));
                }
                temp1 = temp / 1000;
                temp  = temp % 1000;
                DisplayPicture(RADIO_FREQNUM_PIC_X + RADIO_FREQNUM_PIC_XSIZE, RADIO_FREQNUM_PIC_Y, RADIO_FREQNUM_PIC_XSIZE, RADIO_FREQNUM_PIC_YSIZE, (NFA_RADIO_FREQNUM_PIC + (unsigned long)(temp1) * RADIO_FREQNUM_PIC_SIZE));
                
                temp1 = temp / 100;
                temp  = temp % 100;
                DisplayPicture(RADIO_FREQNUM_PIC_X + (RADIO_FREQNUM_PIC_XSIZE << 1), RADIO_FREQNUM_PIC_Y, RADIO_FREQNUM_PIC_XSIZE, RADIO_FREQNUM_PIC_YSIZE, (NFA_RADIO_FREQNUM_PIC + (unsigned long)(temp1) * RADIO_FREQNUM_PIC_SIZE));
                
                temp = temp / 10;
                DisplayPicture(RADIO_FREQNUM_PIC_X + (RADIO_FREQNUM_PIC_XSIZE * 3) + RADIO_FREQDOT_PIC_XSIZE, RADIO_FREQNUM_PIC_Y, RADIO_FREQNUM_PIC_XSIZE, RADIO_FREQNUM_PIC_YSIZE, (NFA_RADIO_FREQNUM_PIC + (unsigned long)(temp) * RADIO_FREQNUM_PIC_SIZE));
                break;
            }
            case FM_DISPFLAG_GUAGE:
            {
                DisplayPicture(RADIO_GUAGEBK_PIC_X + FmGuageX, RADIO_GUAGEBK_PIC_Y, RADIO_GUAGEBK_PIC_XSIZE, RADIO_GUAGEBK_PIC_YSIZE, RADIO_GUAGE_BKGROUND_PIC);  
                DisplayPicture(RADIO_GUAGEFRONT_PIC_X + FmGuageVal, RADIO_GUAGEFRONT_PIC_Y, RADIO_GUAGEFRONT_PIC_XSIZE, RADIO_GUAGEFRONT_PIC_YSIZE, RADIO_GUAGE_FRONT_PIC);
                FmGuageX = FmGuageVal;
                break;
            }
            case FM_DISPFLAG_BATT:
            {
                DisplayPicture(RADIO_BATT_PIC_X, RADIO_BATT_PIC_Y, RADIO_BATT_PIC_XSIZE, RADIO_BATT_PIC_YSIZE, (NFA_RADIO_BATT_PIC + (unsigned long)(BatteryLevel) * RADIO_BATT_PIC_SIZE));
                break;
            }
//            case FM_DISPFLAG_SENS:
//            {
//                DisplayPicture(RADIO_SEN_PIC_X, RADIO_SEN_PIC_Y, RADIO_SEN_PIC_XSIZE, RADIO_SEN_PIC_YSIZE, (NFA_RADIO_SEN_PIC + (unsigned long)(FmSensitive) * RADIO_SEN_PIC_SIZE));
//                break;
//            }
//            case FM_DISPFLAG_STEREO:
//            {
//                DisplayPicture(RADIO_SOUND_PIC_X, RADIO_SOUND_PIC_Y, RADIO_SOUND_PIC_XSIZE, RADIO_SOUND_PIC_YSIZE, (NFA_RADIO_SOUND_PIC + (unsigned long)(FmStereoState) * RADIO_SOUND_PIC_SIZE));
//                break;
//            }
            case FM_DISPFLAG_CH:
            {
                DisplayPicture(RADIO_PRESETBK_PIC_X, RADIO_PRESETBK_PIC_Y, RADIO_PRESETBK_PIC_XSIZE, RADIO_PRESETBK_PIC_YSIZE, RADIO_PRESETBK_PIC);
                if ((GlobalFmVar.FmState == FM_STATE_AUTO) || (GlobalFmVar.FmState == FM_STATE_PRESET)) {
                    FmDispBuf[0] = (GlobalFmVar.FmSaveNum + 1)/ 10 + '0';
                    if (FmDispBuf[0] == '0') FmDispBuf[0] = ' ';
                    FmDispBuf[1] = (GlobalFmVar.FmSaveNum + 1) % 10 + '0';
                    FmDispBuf[2] = '/';
                    FmDispBuf[3] = '2';
                    FmDispBuf[4] = '0';
                    FmDispBuf[5] = 0;
                    LCD_SetColor(COLOR_BLACK);
                    LCD_NFDispStringAt(RADIO_PRESETBK_PIC_X, RADIO_PRESETBK_PIC_Y, FmDispBuf);
                }
                break;
            }
            case FM_DISPFLAG_VOL:
            {
                DisplayPicture(RADIO_VOLLEVEL_PIC_X, RADIO_VOLLEVEL_PIC_Y, RADIO_VOLLEVEL_PIC_XSIZE, RADIO_VOLLEVEL_PIC_YSIZE, 
                               (NFA_RADIO_VOLLEVEL_PIC + (unsigned long)(OutputVolume) * RADIO_VOLLEVEL_PIC_SIZE));
                DisplayPicture(RADIO_VOLBK_PIC_X, RADIO_VOLBK_PIC_Y, RADIO_VOLBK_PIC_XSIZE, RADIO_VOLBK_PIC_YSIZE, RADIO_VOLBK_PIC);
                
                FmDispBuf[0] = OutputVolume / 10 + '0';
                FmDispBuf[1] = OutputVolume % 10 +'0';
                FmDispBuf[2] = 0;
                
                LCD_SetColor(COLOR_BLACK);
                LCD_NFDispStringAt(RADIO_VOLBK_PIC_X + 2, RADIO_VOLBK_PIC_Y + 2, FmDispBuf);
                break;
            }
            case FM_DISPFLAG_REGION:
            {
                DisplayPicture(RADIO_REGIONBK_PIC_X, RADIO_REGIONBK_PIC_Y, RADIO_REGIONBK_PIC_XSIZE, RADIO_REGIONBK_PIC_YSIZE, RADIO_REGIONBK_PIC);
                LCD_SetColor(COLOR_BLACK);
                DisplayMenuStr(RADIO_REGIONBK_PIC_X, RADIO_REGIONBK_PIC_Y + 2, RADIO_REGIONBK_PIC_XSIZE, RADIO_REGIONBK_PIC_YSIZE, LCD_TEXTALIGN_CENTER, (SETUP_RADIO_REGION_TXT + (unsigned long)(GlobalFmVar.FmArea + 1) * MENU_PER_BYTE));
                
                FmDispBuf[0] = FmFreqMinVal / 10000 + '0';
                temp         = FmFreqMinVal % 10000;
                if(FmDispBuf[0] == '0') FmDispBuf[0] = ' ';
                
                FmDispBuf[1] = temp / 1000 + '0';
                temp         = temp % 1000;
                
                FmDispBuf[2] = temp / 100 +'0';
                FmDispBuf[4] = temp % 100 + '0';
                
                FmDispBuf[3] = '.';
                FmDispBuf[5] = 'M';
                FmDispBuf[6] = 'H';
                FmDispBuf[7] = 'z';
                FmDispBuf[8] = 0;
                
                DisplayPicture(RADIO_REGIONBK_START_PIC_X, RADIO_REGIONBK_START_PIC_Y, RADIO_REGIONBK_PIC_XSIZE, RADIO_REGIONBK_PIC_YSIZE, RADIO_REGIONBK_START_PIC);
                LCD_NFDispStringAt(RADIO_REGIONBK_START_PIC_X, RADIO_REGIONBK_START_PIC_Y, FmDispBuf);
                
                FmDispBuf[0] = FmFreqMaxVal / 10000 + '0';
                temp         = FmFreqMaxVal % 10000;
                if(FmDispBuf[0] == '0') FmDispBuf[0] = ' ';
                
                FmDispBuf[1] = temp / 1000 + '0';
                temp         = temp % 1000;
                
                FmDispBuf[2] = temp / 100 +'0';
                FmDispBuf[4] = temp % 100 + '0';
                
                DisplayPicture(RADIO_REGIONBK_END_PIC_X, RADIO_REGIONBK_END_PIC_Y, RADIO_REGIONBK_PIC_XSIZE, RADIO_REGIONBK_PIC_YSIZE, RADIO_REGIONBK_END_PIC);
                LCD_NFDispStringAt(RADIO_REGIONBK_END_PIC_X, RADIO_REGIONBK_END_PIC_Y, FmDispBuf);
                break;
            }
            case FM_DISPFLAG_HOLD:
            {
                DisplayPicture(RADIO_HOLD_PIC_X, RADIO_HOLD_PIC_Y, RADIO_HOLD_PIC_XSIZE, RADIO_HOLD_PIC_YSIZE, (NFA_RADIO_HOLD_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * RADIO_HOLD_PIC_SIZE));
                break;
            }
            default:
                break;
            }
        }
    }
    LCD_SetColor(TempColor);
    
    MusicSetFreq51Return();
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void DisplayFmMenu(void)
{
    unsigned int      FmDispBuf[MENU_PER_BYTE];
    unsigned int      temp, TempColor;
    unsigned int      Temp_DrawMode;
    
    MusicSetFreq51();         //add by zyz
    
    TempColor      = LCD_GetColor();
    Temp_DrawMode  = LCD_SetTextMode(LCD_DRAWMODE_TRANS);
    
    if (FmDisplayFlag & FM_DISPFLAG_SUBMENU_BK) {
        //LCD_SetDispRect(RADIO_SUBMENUBK_PIC_X, RADIO_SUBMENUBK_PIC_Y, (RADIO_SUBMENUBK_PIC_X + RADIO_SUBMENUBK_PIC_XSIZE - 1), (RADIO_SUBMENUBK_PIC_Y + RADIO_SUBMENUBK_PIC_YSIZE - 1));
        DisplayPicture(RADIO_SUBMENUBK_PIC_X, RADIO_SUBMENUBK_PIC_Y, RADIO_SUBMENUBK_PIC_XSIZE, RADIO_SUBMENUBK_PIC_YSIZE, RADIO_SUBMENUBK_PIC);
    }
    
    if (FmDisplayFlag & FM_DISPFLAG_SUBMENU_SEL) {
        
        LCD_SetColor(COLOR_BLACK);
        for (temp = 0; temp < 5; temp ++) {
            
            DisplayPicture(RADIO_SUBMENUBK_PIC_X, (RADIO_SUB_MENU_TXT_Y + (temp * RADIO_SUB_MENU_TXT_YSTEP)), 
                           RADIO_SUBMENUBK_PIC_XSIZE, RADIO_SUB_MENU_TXT_YSTEP, 
                           (NFA_RADIO_SUBMENUBK_PIC + ((unsigned long)(((RADIO_SUB_MENU_TXT_Y - RADIO_SUBMENUBK_PIC_Y) + (temp * RADIO_SUB_MENU_TXT_YSTEP)) * RADIO_SUBMENUBK_PIC_XSIZE * 2))));
            
            if(temp == FmFunctionSele) {
                if ((FmFunctionSele == FM_STATE_SAVE) || (FmFunctionSele == FM_STATE_DEL)) {
                    DisplayPicture(RADIO_SUBMENU_ARROW_PIC_X, (RADIO_SUB_MENU_TXT_Y + (temp * RADIO_SUB_MENU_TXT_YSTEP)) + 2, 
                                   RADIO_SUBMENU_ARROW_PIC_XSIZE, RADIO_SUBMENU_ARROW_PIC_YSIZE, RADIO_SUBMENU_ARROW_ARROW);
                }
                LCD_SetTextMode(LCD_DRAWMODE_REV);
            }
            DisplayMenuStr(RADIO_SUB_MENU_TXT_X, (RADIO_SUB_MENU_TXT_Y + (temp * RADIO_SUB_MENU_TXT_YSTEP)), RADIO_SUB_MENU_TXT_XSIZE, RADIO_SUB_MENU_TXT_YSIZE, LCD_TEXTALIGN_LEFT, (NFA_RADIO_TEXT + (unsigned long)(temp) * MENU_PER_BYTE));
            LCD_SetTextMode(LCD_DRAWMODE_TRANS); 
        }
    }
    FmDisplayFlag = FM_DISPFLAG_NULL;
    
    LCD_SetTextMode(Temp_DrawMode);
    LCD_SetColor(TempColor);
    MusicSetFreq51Return();
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmFreq2Ascii(unsigned int chl, unsigned int freq, unsigned int *pDispBuf)
{
    pDispBuf[0] = 'C';
    pDispBuf[1] = 'H';
    pDispBuf[2] = (chl+1)/10 + '0';
    pDispBuf[3] = (chl+1)%10 + '0';
    pDispBuf[4] = ':';
    pDispBuf[5] = ' ';
    pDispBuf[8] = '.';
    pDispBuf[10] = 'M';
    pDispBuf[11] = 'H';
    pDispBuf[12] = 'z';
    pDispBuf[13] = 0;
    if(freq != 0){
        pDispBuf[5]  = freq /10000;
        pDispBuf[5] += (pDispBuf[5]?'0':' ');
        pDispBuf[6]  = (freq%10000)/1000 + '0';
        pDispBuf[7]  = (freq%1000)/100 + '0';
        
        pDispBuf[9] = (freq%100)/10   + '0';
    }else{
        pDispBuf[6]  = 'E';
        pDispBuf[7]  = 'm';
        pDispBuf[8]  = 'p';
        pDispBuf[9]  = 't';
        pDispBuf[10] = 'y';
        pDispBuf[11] = 0;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void DisplayFmList(void)
{
    unsigned int i,j,m;
    unsigned int mode_tmp;
    unsigned int temp, TempColor, Temp_DrawMode;
    
    unsigned int per_id = PrevCh;
    unsigned int cur_id = CurCh;
    
    unsigned int k;
    unsigned int *pFmFreq;
    unsigned int FmDispBuf[MENU_PER_BYTE];
    
    MusicSetFreq51();         //add by zyz
    TempColor      = LCD_GetColor();
    Temp_DrawMode  = LCD_SetTextMode(LCD_DRAWMODE_TRANS);

    if (FmDisplayFlag & FM_DISPFLAG_LIST_BK) {
        
        //LCD_SetDispRect(RADIO_LISTBK_PIC_X, RADIO_LISTBK_PIC_Y, (RADIO_LISTBK_PIC_X + RADIO_LISTBK_PIC_XSIZE - 1), (RADIO_LISTBK_PIC_Y + RADIO_LISTBK_PIC_YSIZE - 1));
        DisplayPicture(RADIO_LISTBK_PIC_X,RADIO_LISTBK_PIC_Y,RADIO_LISTBK_PIC_XSIZE, RADIO_LISTBK_PIC_YSIZE, RADIO_LISTBK_PIC);
        
        k = cur_id / DEL_FM_SCREEN_PER_LINE;
        k = k * DEL_FM_SCREEN_PER_LINE;
        pFmFreq = &GlobalFmVar.FmFreqArray[k];
        
        LCD_SetColor(COLOR_BLACK);
        for (i = 0; i < DEL_FM_SCREEN_PER_LINE; i++, pFmFreq++) {
            
            FmFreq2Ascii(i+k, *pFmFreq, FmDispBuf);
            
            if ((i + k) == cur_id) LCD_SetTextMode(LCD_DRAWMODE_REV);
            LCD_NFDispStringAt(RADIO_SUB_LIST_TXT_X, (RADIO_SUB_LIST_TXT_Y + i * RADIO_SUB_LIST_TXT_YSTEP), FmDispBuf);
            LCD_SetTextMode(LCD_DRAWMODE_TRANS);
        }
        
        if (cur_id < DEL_FM_SCREEN_PER_LINE) {
            DisplayPicture(RADIO_LIST_UPICON_PIC_X,RADIO_LIST_UPICON_PIC_Y,RADIO_LIST_UPICON_PIC_XSIZE, RADIO_LIST_UPICON_PIC_YSIZE, RADIO_LIST_UPICON_ARROWBK);
        } else {
            DisplayPicture(RADIO_LIST_UPICON_PIC_X,RADIO_LIST_UPICON_PIC_Y,RADIO_LIST_UPICON_PIC_XSIZE, RADIO_LIST_UPICON_PIC_YSIZE, RADIO_LIST_UPICON_ARROW);
        }
        
        temp = cur_id / DEL_FM_SCREEN_PER_LINE;
        if (((temp + 1) * DEL_FM_SCREEN_PER_LINE) > FREQMAXNUMBLE) {
            DisplayPicture(RADIO_LIST_DOWNICON_PIC_X,RADIO_LIST_DOWNICON_PIC_Y,RADIO_LIST_DOWNICON_PIC_XSIZE, RADIO_LIST_DOWNICON_PIC_YSIZE, RADIO_LIST_DOWNICON_ARROWBK);
        } else {
            DisplayPicture(RADIO_LIST_DOWNICON_PIC_X,RADIO_LIST_DOWNICON_PIC_Y,RADIO_LIST_DOWNICON_PIC_XSIZE, RADIO_LIST_DOWNICON_PIC_YSIZE, RADIO_LIST_DOWNICON_ARROW);
        }
    }
    
    if (FmDisplayFlag & FM_DISPFLAG_LIST_SEL) {
        
        if (cur_id < DEL_FM_SCREEN_PER_LINE) {
            DisplayPicture(RADIO_LIST_UPICON_PIC_X,RADIO_LIST_UPICON_PIC_Y,RADIO_LIST_UPICON_PIC_XSIZE, RADIO_LIST_UPICON_PIC_YSIZE, RADIO_LIST_UPICON_ARROWBK);
        } else {
            DisplayPicture(RADIO_LIST_UPICON_PIC_X,RADIO_LIST_UPICON_PIC_Y,RADIO_LIST_UPICON_PIC_XSIZE, RADIO_LIST_UPICON_PIC_YSIZE, RADIO_LIST_UPICON_ARROW);
        }
        
        temp = cur_id / DEL_FM_SCREEN_PER_LINE;
        if (((temp + 1) * DEL_FM_SCREEN_PER_LINE) > FREQMAXNUMBLE) {
            DisplayPicture(RADIO_LIST_DOWNICON_PIC_X,RADIO_LIST_DOWNICON_PIC_Y,RADIO_LIST_DOWNICON_PIC_XSIZE, RADIO_LIST_DOWNICON_PIC_YSIZE, RADIO_LIST_DOWNICON_ARROWBK);
        } else {
            DisplayPicture(RADIO_LIST_DOWNICON_PIC_X,RADIO_LIST_DOWNICON_PIC_Y,RADIO_LIST_DOWNICON_PIC_XSIZE, RADIO_LIST_DOWNICON_PIC_YSIZE, RADIO_LIST_DOWNICON_ARROW);
        }
            
        if((per_id / DEL_FM_SCREEN_PER_LINE) != temp) {
            
            DisplayPicture(RADIO_LISTBK_PIC_X, RADIO_SUB_LIST_TXT_Y, 
                           RADIO_LISTBK_PIC_XSIZE, (RADIO_SUB_LIST_TXT_YSTEP * DEL_FM_SCREEN_PER_LINE), 
                           (RADIO_LISTBK_PIC + (unsigned long)(RADIO_SUB_LIST_TXT_Y - RADIO_LISTBK_PIC_Y) * RADIO_LISTBK_PIC_XSIZE * 2));
            
                        
            if(cur_id == (FM_SAVE_BUF_MAX-1) && (per_id == 0)){
                m = cur_id%DEL_FM_SCREEN_PER_LINE;
                k = cur_id - m;
                pFmFreq = &GlobalFmVar.FmFreqArray[k];
            }else if(((per_id % DEL_FM_SCREEN_PER_LINE)==0)&&((cur_id%DEL_FM_SCREEN_PER_LINE)==(DEL_FM_SCREEN_PER_LINE-1))){
                m = cur_id%DEL_FM_SCREEN_PER_LINE;
                k = cur_id - m;
                pFmFreq = &GlobalFmVar.FmFreqArray[k];
            }else{
                pFmFreq = &GlobalFmVar.FmFreqArray[cur_id];
                m = FM_SAVE_BUF_MAX - (cur_id+1);
                k = cur_id;
            }
            
            LCD_SetColor(COLOR_BLACK);
            for (i=0; (i<DEL_FM_SCREEN_PER_LINE && i<=m); i++, pFmFreq++) {
                FmFreq2Ascii(k+i, *pFmFreq, FmDispBuf);
                
                if ((i + k) == cur_id) LCD_SetTextMode(LCD_DRAWMODE_REV);
                LCD_NFDispStringAt(RADIO_SUB_LIST_TXT_X, (RADIO_SUB_LIST_TXT_Y + i * RADIO_SUB_LIST_TXT_YSTEP), FmDispBuf);
                LCD_SetTextMode(LCD_DRAWMODE_TRANS);
            }
        }else{
            j = per_id%DEL_FM_SCREEN_PER_LINE;
            i = cur_id%DEL_FM_SCREEN_PER_LINE;
            
            LCD_SetColor(COLOR_BLACK);
            
            DisplayPicture(RADIO_LISTBK_PIC_X, (RADIO_SUB_LIST_TXT_Y + j * RADIO_SUB_LIST_TXT_YSTEP), 
                           RADIO_LISTBK_PIC_XSIZE, RADIO_SUB_LIST_TXT_YSTEP, 
                           (RADIO_LISTBK_PIC + (unsigned long)((RADIO_SUB_LIST_TXT_Y - RADIO_LISTBK_PIC_Y) + j * RADIO_SUB_LIST_TXT_YSTEP) * RADIO_LISTBK_PIC_XSIZE * 2));
                           
            FmFreq2Ascii(per_id, GlobalFmVar.FmFreqArray[per_id], FmDispBuf);
            LCD_NFDispStringAt(RADIO_SUB_LIST_TXT_X, (RADIO_SUB_LIST_TXT_Y + j * RADIO_SUB_LIST_TXT_YSTEP), FmDispBuf);
            
            DisplayPicture(RADIO_LISTBK_PIC_X, (RADIO_SUB_LIST_TXT_Y + i * RADIO_SUB_LIST_TXT_YSTEP), 
                           RADIO_LISTBK_PIC_XSIZE, RADIO_SUB_LIST_TXT_YSTEP, 
                           (RADIO_LISTBK_PIC + (unsigned long)((RADIO_SUB_LIST_TXT_Y - RADIO_LISTBK_PIC_Y) + i * RADIO_SUB_LIST_TXT_YSTEP) * RADIO_LISTBK_PIC_XSIZE * 2));
                           
            FmFreq2Ascii(cur_id, GlobalFmVar.FmFreqArray[cur_id], FmDispBuf);
            LCD_SetTextMode(LCD_DRAWMODE_REV);
            LCD_NFDispStringAt(RADIO_SUB_LIST_TXT_X, (RADIO_SUB_LIST_TXT_Y + i * RADIO_SUB_LIST_TXT_YSTEP), FmDispBuf);
        }
        LCD_SetColor(TempColor);
    }
    FmDisplayFlag = FM_DISPFLAG_NULL;
    
    LCD_SetTextMode(Temp_DrawMode);
    LCD_SetColor(TempColor);
    MusicSetFreq51Return();
}
#endif
//******************************************************************************
