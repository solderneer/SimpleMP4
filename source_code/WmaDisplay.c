#define _IN_MP3DISPLAY

#include "Macro.h"
#include "MsgDef.h"
#include "Struct.h"
#include "Resource.h"
#include "WmaDecodeMacro.h"
#include "WmaDecodeGlobal.h"
#include "WmaLrcVar.h"
#include "Global.h"

#include "Lcd.h"
#include "LcdChar.h"

unsigned int WmaPlayEqStr[12][8] __attribute__((section(".wma_data, \"aw\""))) =
{
	{"NOR"  },                      // 0         no eq  
    {"3D"   },                      //  1        3d  
	{"ROCK" },                      //  2        rock   
	{"POP"  },                        // 3         pop    
	{"CLASS"},                      // 4         classic
	{"BASS" },                      //  5        bass   
	{"JAZZ" },                      //  6        jazz   
    {"USER" } ,                    //   11       user 
	{"METAL"},               //  7        metal  
	{"DANCE"},                        //  8        dance  
	{"PARTY"},                     //  9        party  
	{"CLUB" },	                  //  10        club  
};

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void DISP_WmaPlay(void){
    unsigned int  i,j,k;
    unsigned int  temp, TempColor;
    unsigned long tempL,TempFlag;
    unsigned long TempDispFlag, TempDisplayFlag1;
    unsigned int  DisplayBuf[3 + MAX_DIR_DEPTH*12 + 1];//[32];
    LCD_RECT      r;
    
    AVI_MP3_DECODE_DISABLE;
    TempDispFlag = MusicDispFlag;
    MusicDispFlag = MUSIC_DISPFLAG_NULL;
    AVI_MP3_DECODE_ENABLE;
    
    TempFlag = 0x00000001;
    while (TempDispFlag != 0) {
        TempDisplayFlag1 = TempDispFlag & TempFlag;
        TempDispFlag   &= (~TempFlag);
        TempFlag = TempFlag << 1;
        
        if (TempDisplayFlag1 != 0) {
            switch (TempDisplayFlag1) {
            case MUSIC_DISPFLAG_BACKGROUND:
            {
                DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, MUSIC_BKGROUND_PIC);
                LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
                break;
            }
            case MUSIC_DISPFLAG_SPECTRUM:
            {
                for(i = 0; i < 17; i ++) {
                    DisplayPicture(MUSIC_SPECTRUM_PIC_X + (i * (MUSIC_SPECTRUM_PIC_XSIZE + 0)), MUSIC_SPECTRUM_PIC_Y, 
                                   MUSIC_SPECTRUM_PIC_XSIZE, MUSIC_SPECTRUM_PIC_YSIZE, 
                                   (NFA_MUSIC_SPECTRUM_PIC + (unsigned long)(WMAStroboscopeArray[i]) * MUSIC_SPECTRUM_PIC_SIZE));
                }
                break;
            }
            case MUSIC_DISPFLAG_GUAGE:
            {
                for (i = 0; i < MusicPlayInfo.TimeGuageVal; i ++) {
                    DisplayPicture(MUSIC_GUAGE_PIC_X + i, MUSIC_GUAGE_PIC_Y, MUSIC_GUAGE_PIC_XSIZE, MUSIC_GUAGE_PIC_YSIZE, MUSIC_GUAGE_FRONT_PIC);
                }
                for (i = MusicPlayInfo.TimeGuageVal; i < TIME_GUAGE_CONST; i ++ ) {
                    DisplayPicture(MUSIC_GUAGE_PIC_X + i, MUSIC_GUAGE_PIC_Y, MUSIC_GUAGE_PIC_XSIZE, MUSIC_GUAGE_PIC_YSIZE, MUSIC_GUAGE_BKGROUND_PIC);
                }
                break;
            }
            //case MUSIC_DISPFLAG_FREE_TIME:
            case MUSIC_DISPFLAG_CUR_TIME:
            {
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                
                GetTimeHMS(cbLastTime,&i,&j,&k);
                ConTime2String(1, i, j, k, DisplayBuf);
                DisplayPicture(MUSIC_TIMECURRBK_PIC_X, MUSIC_TIMECURRBK_PIC_Y, MUSIC_TIMECURRBK_PIC_XSIZE, MUSIC_TIMECURRBK_PIC_YSIZE, MUSIC_TIMECURRBK_PIC);
                LCD_NFDispStringAt(MUSIC_TIMECURRBK_PIC_X,MUSIC_TIMECURRBK_PIC_Y,DisplayBuf);
                
                tempL = 0;
                if (cbTotalTime >= cbLastTime) tempL = cbTotalTime - cbLastTime;
                GetTimeHMS(tempL, &i, &j, &k);
                ConTime2String(-1, i, j, k, DisplayBuf);
                DisplayPicture(MUSIC_TIMETOTALBK_PIC_X, MUSIC_TIMETOTALBK_PIC_Y, MUSIC_TIMETOTALBK_PIC_XSIZE, MUSIC_TIMETOTALBK_PIC_YSIZE, MUSIC_TIMETOTALBK_PIC);
                LCD_NFDispStringAt(MUSIC_TIMETOTALBK_PIC_X,MUSIC_TIMETOTALBK_PIC_Y,DisplayBuf);
                LCD_SetColor(TempColor);
                
//                TempColor = LCD_GetColor();
//                LCD_SetColor(COLOR_BLACK);
//                
//                GetTimeHMS(cbLastTime,&i,&j,&k);
//                ConTime2String(1, i, j, k, DisplayBuf);
//                DisplayPicture(MUSIC_TIMEINCBK_PIC_X, MUSIC_TIMEINCBK_PIC_Y, MUSIC_TIMEINCBK_PIC_XSIZE, MUSIC_TIMEINCBK_PIC_YSIZE, MUSIC_TIMEINCBK_PIC);
//                LCD_NFDispStringAt(MUSIC_TIMEINCBK_PIC_X,MUSIC_TIMEINCBK_PIC_Y,DisplayBuf);
//                
//                tempL = 0;
//                if (cbTotalTime >= cbLastTime) tempL = cbTotalTime - cbLastTime;
//                GetTimeHMS(tempL, &i, &j, &k);
//                ConTime2String(-1, i, j, k, DisplayBuf);
//                DisplayPicture(MUSIC_TIMEDECBK_PIC_X, MUSIC_TIMEDECBK_PIC_Y, MUSIC_TIMEDECBK_PIC_XSIZE, MUSIC_TIMEDECBK_PIC_YSIZE, MUSIC_TIMEDECBK_PIC);
//                LCD_NFDispStringAt(MUSIC_TIMEDECBK_PIC_X,MUSIC_TIMEDECBK_PIC_Y,DisplayBuf);
//                LCD_SetColor(TempColor);
        
                break;
            }
            case MUSIC_DISPFLAG_LRC:
            {
                DisplayPicture(MUSIC_LRCBK_PIC_X, MUSIC_LRCBK_PIC_Y, MUSIC_LRCBK_PIC_XSIZE, MUSIC_LRCBK_PIC_YSIZE, MUSIC_LRCBK_PIC);
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                r.x0 = MUSIC_LRCBK_PIC_X;
                r.y0 = MUSIC_LRCBK_PIC_Y;
                r.x1 = MUSIC_LRCBK_PIC_X + MUSIC_LRCBK_PIC_XSIZE - 1;
                r.y1 = MUSIC_LRCBK_PIC_Y + MUSIC_LRCBK_PIC_YSIZE - 1;
                LCD_DispStringInRect(&r, &r, WmaLrcBuffer, LCD_TEXTALIGN_LEFT);
                LCD_SetColor(TempColor);
                break;
            }
            case MUSIC_DISPFLAG_BATT:
            {
                DisplayPicture(MUSIC_BATT_PIC_X, MUSIC_BATT_PIC_Y, MUSIC_BATT_PIC_XSIZE, MUSIC_BATT_PIC_YSIZE,
                              (MUSIC_BATT_PIC_1 + (unsigned long)(BatteryLevel) * MUSIC_BATT_PIC_SIZE));
                break;
            }
            case MUSIC_DISPFLAG_BITRATE:
            {
                DisplayPicture(MUSIC_BITERATEBK_PIC_X, MUSIC_BITERATEBK_PIC_Y, MUSIC_BITERATEBK_PIC_XSIZE, MUSIC_BITERATEBK_PIC_YSIZE, MUSIC_BITERATEBK_PIC);
                
                DisplayBuf[0] = MusicPlayInfo.Bitrate / 100 + '0';
                if(DisplayBuf[0] == '0') DisplayBuf[0] = ' ';
                DisplayBuf[1] = (MusicPlayInfo.Bitrate % 100) / 10 + '0';
                DisplayBuf[2] = MusicPlayInfo.Bitrate % 10 +'0';
                DisplayBuf[3] = 'K';
                DisplayBuf[4] = 'b';
                DisplayBuf[5] = 'p';
                DisplayBuf[6] = 's';
                DisplayBuf[7] = 0;
                
                r.x0 = MUSIC_BITERATEBK_PIC_X;
                r.y0 = MUSIC_BITERATEBK_PIC_Y;
                r.x1 = MUSIC_BITERATEBK_PIC_X + MUSIC_BITERATEBK_PIC_XSIZE - 1;
                r.y1 = MUSIC_BITERATEBK_PIC_Y + MUSIC_BITERATEBK_PIC_YSIZE - 1;
                
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                LCD_DispStringInRect(&r, &r, DisplayBuf, LCD_TEXTALIGN_CENTER);
                LCD_SetColor(TempColor);
                break;
            }
            case MUSIC_DISPFLAG_STATUS:
            {
                DisplayPicture(MUSIC_PLAY_STATUS_PIC_X, MUSIC_PLAY_STATUS_PIC_Y, MUSIC_PLAY_STATUS_PIC_XSIZE, MUSIC_PLAY_STATUS_PIC_YSIZE, 
                              (MUSIC_STATUS_PLAY_PIC + (unsigned long)(WmaPlayStatus) * MUSIC_PLAY_STATUS_PIC_SIZE));
                break;
            }
            case MUSIC_DISPFLAG_VOL:
            {
                DisplayPicture(MUSIC_VOLLEVEL_PIC_X, MUSIC_VOLLEVEL_PIC_Y, MUSIC_VOLLEVEL_PIC_XSIZE, MUSIC_VOLLEVEL_PIC_YSIZE, 
                               (MUSIC_VOLLEVEL_PIC_00 + (unsigned long)(OutputVolume) * MUSIC_VOLLEVEL_PIC_SIZE));
                DisplayPicture(MUSIC_VOLBK_PIC_X, MUSIC_VOLBK_PIC_Y, MUSIC_VOLBK_PIC_XSIZE, MUSIC_VOLBK_PIC_YSIZE, MUSIC_VOLBK_PIC);
                
                DisplayBuf[0] = OutputVolume / 10 + '0';
                DisplayBuf[1] = OutputVolume % 10 +'0';
                DisplayBuf[2] = 0;
                
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                LCD_NFDispStringAt(MUSIC_VOLBK_PIC_X + 2, MUSIC_VOLBK_PIC_Y + 2, DisplayBuf);
                LCD_SetColor(TempColor);
                break;
            }
            case MUSIC_DISPFLAG_HOLD:
            {
                DisplayPicture(MUSIC_HOLD_PIC_X, MUSIC_HOLD_PIC_Y, MUSIC_HOLD_PIC_XSIZE, MUSIC_HOLD_PIC_YSIZE, 
                              (MUSIC_HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MUSIC_HOLD_PIC_SIZE));
                break;
            }
            case MUSIC_DISPFLAG_AB:
            {
                DisplayPicture(MUSIC_AB_PIC_X, MUSIC_AB_PIC_Y, MUSIC_AB_PIC_XSIZE, MUSIC_AB_PIC_YSIZE, 
                              (MUSIC_AB_OFF_PIC + (unsigned long)(MusicPlayInfo.AB) * MUSIC_AB_PIC_SIZE));
                break;
            }
            /*case MUSIC_DISPFLAG_PATH:*/
            case MUSIC_DISPFLAG_FILENAME:
            {
                DisplayPicture(MUSIC_FILENAMEBK_PIC_X, MUSIC_FILENAMEBK_PIC_Y, MUSIC_FILENAMEBK_PIC_XSIZE, MUSIC_FILENAMEBK_PIC_YSIZE, MUSIC_FILENAMEBK_PIC);
                
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                
                r.x0 = MUSIC_FILENAMEBK_PIC_X;
                r.x1 = MUSIC_FILENAMEBK_PIC_X + MUSIC_FILENAMEBK_PIC_XSIZE - 1;
                
                r.y0 = MUSIC_FILENAMEBK_PIC_Y + 7;
                r.y1 = MUSIC_FILENAMEBK_PIC_Y + 7 + CH_CHAR_YSIZE - 1;
                //TxtGbk2Unicode(SysFile1Info.Path, DisplayBuf, 1);
                //LCD_DispStringInRect(&r, &r, DisplayBuf, LCD_TEXTALIGN_LEFT);
                LCD_DispStringInRect(&r, &r, &SysFile1Info.LongPath[0], LCD_TEXTALIGN_LEFT);
                
                r.y0 = MUSIC_FILENAMEBK_PIC_Y + 11 + CH_CHAR_YSIZE;
                r.y1 = MUSIC_FILENAMEBK_PIC_Y + 11 + (CH_CHAR_YSIZE << 1) - 1;
                LCD_DispStringInRect(&r, &r, SysFile1Info.LongFileName, LCD_TEXTALIGN_LEFT);
                LCD_SetColor(TempColor);
        
                LCD_SetColor(TempColor);
                break;
            }
            case MUSIC_DISPFLAG_ID3:
            {
                DisplayPicture(MUSIC_ID3ALBUMBK_PIC_X, MUSIC_ID3ALBUMBK_PIC_Y, MUSIC_ID3ALBUMBK_PIC_XSIZE, MUSIC_ID3ALBUMBK_PIC_YSIZE, MUSIC_ID3ALBUMBK_PIC);
                
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                
                r.x0 = MUSIC_ID3ALBUMBK_PIC_X;
                r.x1 = MUSIC_ID3ALBUMBK_PIC_X + MUSIC_ID3ALBUMBK_PIC_XSIZE - 1;
                
//                r.y0 = MUSIC_ID3ALBUMBK_PIC_Y;
//                r.y1 = MUSIC_ID3ALBUMBK_PIC_Y + CH_CHAR_YSIZE - 1;
//                if (Id3Info.id3_title[0] == 0) {
//                    LCD_DispStringInRect(&r, &r, Id3NoTitleStr, LCD_TEXTALIGN_LEFT);
//                } else {
//                    LCD_ScrollStringAt(&r, WmaId3Info.id3_title, MUSIC_ID3BK_TITLE_PIC_X, MUSIC_ID3BK_PIC_XSIZE, MUSIC_ID3BK_TITLE_PIC);
//                }
                
                r.y0 = MUSIC_ID3ART_TXT_Y;
                r.y1 = MUSIC_ID3ART_TXT_Y + CH_CHAR_YSIZE - 1;
                LCD_DispStringInRect(&r,&r,WmaId3Info.id3_singer, LCD_TEXTALIGN_LEFT);
                
                r.y0 = MUSIC_ID3ALBUM_TXT_Y;
                r.y1 = MUSIC_ID3ALBUM_TXT_Y + CH_CHAR_YSIZE - 1;
                LCD_DispStringInRect(&r,&r,WmaId3Info.id3_album, LCD_TEXTALIGN_LEFT);
                LCD_SetColor(TempColor);
                break;
            }
            case MUSIC_DISPFLAG_MODE:
            {
                DisplayPicture(MUSIC_REPMODE_PIC_X, MUSIC_REPMODE_PIC_Y, MUSIC_REPMODE_PIC_XSIZE, MUSIC_REPMODE_PIC_YSIZE,
                              (MUSIC_REPMODE_ONE_PIC + (unsigned long)(MusicPlayInfo.Mode) * MUSIC_REPMODE_PIC_SIZE));
                break;
            }
            case MUSIC_DISPFLAG_ORDER:
            {
                DisplayPicture(MUSIC_ORDER_PIC_X, MUSIC_ORDER_PIC_Y, MUSIC_ORDER_PIC_XSIZE, MUSIC_ORDER_PIC_YSIZE,
                              (MUSIC_ORDER_ORDER_PIC + (unsigned long)(MusicPlayInfo.Order) * MUSIC_ORDER_PIC_SIZE));
                break;
            }
            case MUSIC_DISPFLAG_EQ:
            {
                DisplayPicture(MUSIC_EQBK_PIC_X, MUSIC_EQBK_PIC_Y, MUSIC_EQBK_PIC_XSIZE, MUSIC_EQBK_PIC_YSIZE, MUSIC_EQBK_PIC);
                
                r.x0 = MUSIC_EQBK_PIC_X;
                r.y0 = MUSIC_EQBK_PIC_Y;
                r.x1 = MUSIC_EQBK_PIC_X + MUSIC_EQBK_PIC_XSIZE - 1;
                r.y1 = MUSIC_EQBK_PIC_Y + MUSIC_EQBK_PIC_YSIZE - 1;
                
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                LCD_DispStringInRect(&r, &r, &WmaPlayEqStr[MusicPlayInfo.Eq][0], LCD_TEXTALIGN_CENTER);
                LCD_SetColor(TempColor);
                break;
            }
            case MUSIC_DISPFLAG_FILENUM:
            {
                DisplayPicture(MUSIC_FILENUMBK_PIC_X, MUSIC_FILENUMBK_PIC_Y, MUSIC_FILENUMBK_PIC_XSIZE, MUSIC_FILENUMBK_PIC_YSIZE, MUSIC_FILENUMBK_PIC);
                
                TempColor = LCD_GetColor();
                LCD_SetColor(COLOR_BLACK);
                DisplayBuf[0] = SysFile1Info.CurrentFileNum / 100 + 0x30;
                temp = SysFile1Info.CurrentFileNum % 100;
                DisplayBuf[1] = temp / 10 + 0x30;
                DisplayBuf[2] = temp % 10 + 0x30;
                DisplayBuf[3] = '/';
                DisplayBuf[4] = 0;
                for (i = 0; i < 2; i++) {
                    if (DisplayBuf[i] != 0x30) break;
                    DisplayBuf[i] = 0x20;
                }
                LCD_NFDispStringAt(MUSIC_FILENUMBK_PIC_X + 12, MUSIC_FILENUMBK_PIC_Y, DisplayBuf);
                
                DisplayBuf[0] = SysFile1Info.TotalFiles / 100 + 0x30;
                temp = SysFile1Info.TotalFiles % 100;
                DisplayBuf[1] = temp / 10 + 0x30;
                DisplayBuf[2] = temp % 10 + 0x30;
                DisplayBuf[3] = 0;
                for (i = 0; i < 2; i++) {
                    if (DisplayBuf[i] != 0x30) break;
                }
                LCD_NFDispStringAt(MUSIC_FILENUMBK_PIC_X + 12 + 32, MUSIC_FILENUMBK_PIC_Y, &DisplayBuf[i]);
                LCD_SetColor(TempColor);
                break;
            }
            default:
                break;
            }
        }
    }
}
