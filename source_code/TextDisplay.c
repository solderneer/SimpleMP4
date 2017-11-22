
#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"
#include "Macro.h"
#include "Lcd.h"
#include "LcdChar.h"


/*
******************************************************************************** 
如果没有定义相关模块代码不进行编译
目前只能先在每个文件里加入 编译 判断
以后可能需要重新进行整理
********************************************************************************
*/

#ifdef TXT_MODULE
    #include "TxtStruct.h"
	#include "TxtGlobal.h"

#define MSG_DISP_TXT_ALL        0x0003
#define TXT_SCROLL_BAR_COUNT    (240 - 60 -21)

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void DISP_Txt(void){
    
    unsigned int i;
    unsigned int color;
    unsigned int StrSize;
    LCD_RECT     r;
    TxtStruct   *pTxt;
    TxtDispBufStruct *pTxtDisp;
    
    if (DispMsgArray[0] != MSG_DISP_TXT) return;
    DispMsgArray[0] = MSG_DISP_NONE;
    
    MusicSetFreq51();
    
    pTxt = (TxtStruct *)DispMsgArray[2];
    
    color = LCD_GetColor();
    
    switch (DispMsgArray[1])
    {
    case MSG_DISP_TXT_INIT:
    {
        DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, TEXT_BKGROUND_PIC);
        DisplayPicture(TEXT_HOLD_PIC_X, TEXT_HOLD_PIC_Y, TEXT_HOLD_PIC_XSIZE, TEXT_HOLD_PIC_YSIZE, (NFA_TEXT_HOLD_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * TEXT_HOLD_PIC_SIZE));
        DisplayPicture(TEXT_BATT_PIC_X, TEXT_BATT_PIC_Y, TEXT_BATT_PIC_XSIZE, TEXT_BATT_PIC_YSIZE, (NFA_TEXT_BATT_PIC + (unsigned long)(BatteryLevel) * TEXT_BATT_PIC_SIZE));
        
        DisplayMenuStr(TEXT_TITLE_TXT_X, TEXT_TITLE_TXT_Y, TEXT_TITLE_TXT_XSIZE, TEXT_TITLE_TXT_YSIZE, LCD_TEXTALIGN_CENTER, MAIN_MENU_TEXT_TXT);
        
        r.x0 = TEXT_FILENAME_TXT_X;
        r.y0 = TEXT_FILENAME_TXT_Y;
        r.x1 = TEXT_FILENAME_TXT_X + TEXT_FILENAME_TXT_XSIZE - 1;
        r.y1 = TEXT_FILENAME_TXT_Y + TEXT_FILENAME_TXT_YSIZE - 1;
        LCD_DispStringInRect(&r, &r, pTxt->FileName, LCD_TEXTALIGN_CENTER);
    }
    case MSG_DISP_TXT_TEXT:
    {
        r.x0 = TEXT_AREABK_PIC_X;
        r.y0 = TEXT_AREABK_PIC_Y;
        r.x1 = TEXT_AREABK_PIC_X + TEXT_AREABK_PIC_XSIZE - 1;
        r.y1 = TEXT_AREABK_PIC_Y + TEXT_AREABK_PIC_YSIZE - 1;
        
        DisplayPicture(TEXT_AREABK_PIC_X, TEXT_AREABK_PIC_Y, TEXT_AREABK_PIC_XSIZE, TEXT_AREABK_PIC_YSIZE, TEXT_AREABK_PIC);
        
        i = TXT_SCROLL_BAR_COUNT - 1;
        if(pTxt->EndFileOffset < pTxt->FileSize)
            i = (unsigned int)(((unsigned long)(pTxt->StartFileOffset)*(TXT_SCROLL_BAR_COUNT - 1))/pTxt->FileSize);
        
        DisplayPicture(TEXT_SCROLL_PIC_X, TEXT_SCROLL_PIC_Y + TxtPrevScrollY, TEXT_SCROLL_PIC_XSIZE, TEXT_SCROLL_PIC_YSIZE, TEXT_SCROLL_BACKGROUND_PIC);  
        DisplayPicture(TEXT_SCROLL_PIC_X, TEXT_SCROLL_PIC_Y + i, TEXT_SCROLL_PIC_XSIZE, TEXT_SCROLL_PIC_YSIZE, TEXT_SCROLL_FOREROUND_PIC);  
        TxtPrevScrollY = i;
        
        pTxtDisp = pTxt->pTxtBuf;
        LCD_SetColor(COLOR_BLACK);
        for(i = 0; i < TXT_DISP_LINE_MAX; i++)
        {
            LCD_NFDispStringAt(r.x0, r.y0, pTxtDisp->TxtDispBuf);
            pTxtDisp = pTxtDisp->pNext;
            r.y0 += CH_CHAR_YSIZE + 3;
        }
        break;
    }
    default:
        break;
    }   
    LCD_SetColor(color);
      
    MusicSetFreq51Return();
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TxtBookMaskSelectDisp(unsigned int type, unsigned int mode, unsigned int id)
{
    unsigned int i,j;
    unsigned int color_tmp;
    unsigned int mode_tmp;
    unsigned int MaskDispBuf[10];
    unsigned int TxtLineBuf[TXT_PER_LINE_CHARS];
    unsigned int *pTxtLineBuf;
    unsigned int CharsPerLine;
    unsigned int ReadBytes;
    long         OffsetCurMask;
    LCD_RECT     r;
    
    r.x0 = TEXT_BOOKMARK_CONTENT_TXT_X;
    r.y0 = TEXT_BOOKMARK_CONTENT_TXT_Y;
    r.x1 = r.x0 + TEXT_BOOKMARK_CONTENT_TXT_XSIZE - 1;
    r.y1 = r.y0 + CH_CHAR_YSIZE - 1;
    
    if(mode == 0){
        DisplayPicture(TEXT_BOOKMARK_PIC_X, TEXT_BOOKMARK_PIC_Y, TEXT_BOOKMARK_PIC_XSIZE, TEXT_BOOKMARK_PIC_YSIZE, TEXT_BOOKMARK_PIC);
        
        DisplayMenuStr(TEXT_BOOKMARK_TITLE_TXT_X, TEXT_BOOKMARK_TITLE_TXT_Y, TEXT_BOOKMARK_TITLE_TXT_XSIZE, 
                        TEXT_BOOKMARK_TITLE_TXT_YSIZE, LCD_TEXTALIGN_CENTER, (NFA_TEXT_TEXT + (unsigned long)(type) * MENU_PER_BYTE));
    }
    
    MaskDispBuf[0] = '(';
    MaskDispBuf[1] = 'E';
    MaskDispBuf[2] = 'm';
    MaskDispBuf[3] = 'p';
    MaskDispBuf[4] = 't';
    MaskDispBuf[5] = 'y';
    MaskDispBuf[6] = ')';
    MaskDispBuf[7] = 0;
    
    mode_tmp  = LCD_SetTextMode(LCD_DRAWMODE_TRANS);
    color_tmp = LCD_GetColor();
    
    LCD_SetColor(COLOR_BLUE);
    for (i = 0; i < TXT_BOOKMARK_COUNTER; i++) {
        if(i == id) LCD_SetTextMode(LCD_DRAWMODE_REV); 
        
        DisplayPicture(TEXT_BOOKMARK_PIC_X, (TEXT_BOOKMARK_CONTENT_TXT_Y + (i * TEXT_BOOKMARK_CONTENT_TXT_YSTEP)), 
                       TEXT_BOOKMARK_PIC_XSIZE, TEXT_BOOKMARK_CONTENT_TXT_YSTEP, 
                       (NFA_TEXT_BOOKMARK_PIC + ((unsigned long)(((TEXT_BOOKMARK_CONTENT_TXT_Y - TEXT_BOOKMARK_PIC_Y) + (i * TEXT_BOOKMARK_CONTENT_TXT_YSTEP)) * TEXT_BOOKMARK_PIC_XSIZE * 2))));
                        
        if(Text.NFBookMark[i] == -1){
            LCD_NFDispStringAt(TEXT_BOOKMARK_CONTENT_TXT_X, ((TEXT_BOOKMARK_CONTENT_TXT_Y + (i * TEXT_BOOKMARK_CONTENT_TXT_YSTEP))/* + (TEXT_BOOKMARK_CONTENT_TXT_YSTEP >> 1) - 8*/), MaskDispBuf);
        } else {
            OffsetCurMask = 0;
            
            while((Text.NFBookMark[i] + OffsetCurMask) < Text.FileSize){
                FileSeek(Text.NFBookMark[i]+OffsetCurMask, SEEK_SET, hSlaveFile);
                ReadBytes = FileRead((unsigned char *)TxtLineBuf, TXT_PER_LINE_CHARS-2, hSlaveFile);
                TxtLineBuf[ReadBytes] = 0;
                ReadBytes = TxtGbk2Unicode(TxtLineBuf, TxtLineBuf, ReadBytes);
                pTxtLineBuf = &TxtLineBuf[0];
                j = 0;
                while(ReadBytes > 0){
                    if(*pTxtLineBuf == '\r' || *pTxtLineBuf == '\n' || *pTxtLineBuf == 0x3000 || *pTxtLineBuf == ' '){
                        if(*pTxtLineBuf == 0x3000){
                            OffsetCurMask += 2;
                            j += 2;
                        }else{
                            OffsetCurMask++;
                            j++;
                        }
                    }else{
                        break;
                    }
                    pTxtLineBuf++;
                    ReadBytes--;
                }
                if(j == 0){
                    break;
                }
            }
            CharsPerLine = LCD_GetCharsPerLine(&r, TxtLineBuf);
            TxtLineBuf[CharsPerLine] = 0;
            LCD_NFDispStringAt(TEXT_BOOKMARK_CONTENT_TXT_X, ((TEXT_BOOKMARK_CONTENT_TXT_Y + (i * TEXT_BOOKMARK_CONTENT_TXT_YSTEP)) + (TEXT_BOOKMARK_CONTENT_TXT_YSTEP >> 1) - 8), TxtLineBuf);
        }
        if(i == id){
            LCD_SetTextMode(LCD_DRAWMODE_TRANS); 
        }
    }
    LCD_SetTextMode(mode_tmp);
    LCD_SetColor(color_tmp);
}
#endif
