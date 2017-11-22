#include "MsgDef.h"
#include "Resource.h"
//#include "Display.h"
#include "Lcd.h"
#include "Macro.h"
#include "global.h"
//#include "NewBrowser.h"
#include "Timer.h"
#include "EncGlobal.h"
#include "key.h"
#include "KeyMacro.h"

#include "Lcd.h"
#include "LcdChar.h"

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void DISP_Record(void)
{
    unsigned int  i,j,k;
    unsigned int  temp, TempColor, Temp_DrawMode;
    unsigned long tempL,TempFlag;
    unsigned long TempDispFlag, TempDisplayFlag1;
    unsigned int  DisplayBuf[MENU_PER_BYTE];
    LCD_RECT      r;
    
    TempColor = LCD_GetColor();
    
    TempDispFlag = EncDispFlag;
    EncDispFlag = ENC_DISPFLAG_NULL;
    
    TempFlag = 0x00000001;
    while (TempDispFlag != 0) {
        TempDisplayFlag1 = TempDispFlag & TempFlag;
        TempDispFlag   &= (~TempFlag);
        TempFlag = TempFlag << 1;
        
        if (TempDisplayFlag1 != 0) {
            switch (TempDisplayFlag1) {
            case ENC_DISPFLAG_BACKGROUND:
            {
                DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, RECORD_BKGROUND_PIC);
                DisplayMenuStr(RECORD_TYPE_TXT_X, RECORD_TYPE_TXT_Y, RECORD_TYPE_TXT_XSIZE, RECORD_TYPE_TXT_YSIZE, LCD_TEXTALIGN_CENTER, (RECORD_TYPE_MIC_TXT + (unsigned long)(EncPlayInfo.Mode) * MENU_PER_BYTE));
                DisplayMenuStr(RECORD_QUALITY_TXT_X, RECORD_QUALITY_TXT_Y, RECORD_QUALITY_TXT_XSIZE, RECORD_QUALITY_TXT_YSIZE, LCD_TEXTALIGN_CENTER, (SETUP_RECORD_QUALITY_TXT + (unsigned long)(GlobalEncVar.MicQuality + 1) * MENU_PER_BYTE));
                break;
            }
            case ENC_DISPFLAG_CUR_TIME:
            {
                LCD_SetColor(COLOR_BLACK);
                GetTimeHMS(g_dwEncoderCurrTimeSecond,&i,&j,&k);
                ConTime2String(1, i, j, k, DisplayBuf);
                DisplayPicture(RECORD_TIMECURRBK_PIC_X, RECORD_TIMECURRBK_PIC_Y, RECORD_TIMECURRBK_PIC_XSIZE, RECORD_TIMECURRBK_PIC_YSIZE, RECORD_TIMECURRBK_PIC);
                r.x0 = RECORD_TIMECURRBK_PIC_X;
                r.y0 = RECORD_TIMECURRBK_PIC_Y;
                r.x1 = RECORD_TIMECURRBK_PIC_X + RECORD_TIMECURRBK_PIC_XSIZE - 1;
                r.y1 = RECORD_TIMECURRBK_PIC_Y + RECORD_TIMECURRBK_PIC_YSIZE - 1;
                LCD_DispStringInRect(&r, &r, DisplayBuf, LCD_TEXTALIGN_CENTER);
                
                GetTimeHMS(g_dwEncoderRemainTimeSecond, &i, &j, &k);
                ConTime2String(-1, i, j, k, DisplayBuf);
                DisplayPicture(RECORD_TIMETOTALBK_PIC_X, RECORD_TIMETOTALBK_PIC_Y, RECORD_TIMETOTALBK_PIC_XSIZE, RECORD_TIMETOTALBK_PIC_YSIZE, RECORD_TIMETOTALBK_PIC);
                r.x0 = RECORD_TIMETOTALBK_PIC_X;
                r.y0 = RECORD_TIMETOTALBK_PIC_Y;
                r.x1 = RECORD_TIMETOTALBK_PIC_X + RECORD_TIMETOTALBK_PIC_XSIZE - 1;
                r.y1 = RECORD_TIMETOTALBK_PIC_Y + RECORD_TIMETOTALBK_PIC_YSIZE - 1;
                LCD_DispStringInRect(&r, &r, DisplayBuf, LCD_TEXTALIGN_CENTER);
                break;
            }
            case ENC_DISPFLAG_GUAGE:
            {
                for (i = 0; i < EncPlayInfo.TimeGuageVal; i ++) {
                    DisplayPicture(RECORD_GUAGE_PIC_X + i, RECORD_GUAGE_PIC_Y, RECORD_GUAGE_PIC_XSIZE, RECORD_GUAGE_PIC_YSIZE, RECORD_GUAGE_FRONT_PIC);
                }
                for (i = EncPlayInfo.TimeGuageVal; i < TIME_GUAGE_CONST; i ++ ) {
                    DisplayPicture(RECORD_GUAGE_PIC_X + i, RECORD_GUAGE_PIC_Y, RECORD_GUAGE_PIC_XSIZE, RECORD_GUAGE_PIC_YSIZE, RECORD_GUAGE_BKGROUND_PIC);
                }
                break;
            }
            case ENC_DISPFLAG_BATT:
            {
                DisplayPicture(RECORD_BATT_PIC_X, RECORD_BATT_PIC_Y, RECORD_BATT_PIC_XSIZE, RECORD_BATT_PIC_YSIZE, 
                              (NFA_RECORD_BATT_PIC + (unsigned long)(BatteryLevel) * RECORD_BATT_PIC_SIZE));
                break;
            }
            case ENC_DISPFLAG_STATUS:
            {
                DisplayPicture(RECORD_STATUSBK_PIC_X, RECORD_STATUSBK_PIC_Y, RECORD_STATUSBK_PIC_XSIZE, RECORD_STATUSBK_PIC_YSIZE, RECORD_STATUSBK_PIC);
                DisplayMenuStr(RECORD_STATUS_TXT_X, RECORD_STATUS_TXT_Y, RECORD_STATUS_TXT_XSIZE, RECORD_STATUS_TXT_YSIZE, LCD_TEXTALIGN_CENTER, (RECORD_STATUS_PLAY_TXT + (unsigned long)(EncPlayStatus) * MENU_PER_BYTE));
                break;
            }
            case ENC_DISPFLAG_HOLD:
            {
                DisplayPicture(RECORD_HOLD_PIC_X, RECORD_HOLD_PIC_Y, RECORD_HOLD_PIC_XSIZE, RECORD_HOLD_PIC_YSIZE, 
                              (NFA_RECORD_HOLD_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * RECORD_HOLD_PIC_SIZE));
                break;
            }
            case ENC_DISPFLAG_FILENAME:
            {
                DisplayPicture(RECORD_FILENAMEBK_PIC_X, RECORD_FILENAMEBK_PIC_Y, RECORD_FILENAMEBK_PIC_XSIZE, RECORD_FILENAMEBK_PIC_YSIZE, RECORD_FILENAMEBK_PIC);
                
                LCD_SetColor(COLOR_BLACK);
                r.x0 = RECORD_FILENAMEBK_PIC_X;
                r.y0 = RECORD_FILENAMEBK_PIC_Y;
                r.x1 = RECORD_FILENAMEBK_PIC_X + RECORD_FILENAMEBK_PIC_XSIZE - 1;
                r.y1 = RECORD_FILENAMEBK_PIC_Y + RECORD_FILENAMEBK_PIC_YSIZE - 1;
                LCD_DispStringInRect(&r,&r,SysFile1Info.LongFileName, LCD_TEXTALIGN_CENTER);
                break;
            }
            default:
                break;
            }
        }
    }
    LCD_SetColor(TempColor);
    LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
}
