//******************************************************************************
#ifndef     _LCDCHAR_H
#define     _LCDCHAR_H

#define     FONT_12x12          1
#define     FONT_16x16          2

#define     SYSTEM_CHAR_FONT    FONT_16x16


#if(SYSTEM_CHAR_FONT == FONT_12x12)
    #define     AS_CHAR_XSIZE   6
    #define     AS_CHAR_YSIZE   12
    #define     CH_CHAR_XSIZE   12
    #define     CH_CHAR_YSIZE   AS_CHAR_YSIZE
#else
    #define     AS_CHAR_XSIZE   8
    #define     AS_CHAR_YSIZE   16
    #define     CH_CHAR_XSIZE   16
    #define     CH_CHAR_YSIZE   AS_CHAR_YSIZE
#endif

unsigned int Gbk2Unicode(unsigned int code);
unsigned int TxtGbk2Unicode(unsigned int *pSbuf, unsigned int *pTbuf, unsigned int Len);
void ConTime2String(int Sign, unsigned int Hour, unsigned int Minute, unsigned int Second, unsigned int *pStr);
int LCD_GetUnicodeStringSize(unsigned int *s);
void LCD_NFDispUnicodeCharAt(int x, int y, unsigned int c);
void LCD_NFDispUnicodeChar(unsigned int c);
int LCD_GetStringSize(unsigned int *s);
int LCD_GetCharXSize(unsigned int s);
int LCD_GetCharsPerLine(LCD_RECT *r, unsigned int *s);
int LCD_GetLineDistX(unsigned int *s, int LineNumChar);
void LCD_DispLineChar(unsigned int *s, int LineNumChar);
void LCD_NFDispString(unsigned int *pStr);
void LCD_NFDispStringAt(int x, int y, unsigned int *pStr);
void LCD_DispStringInRect(LCD_RECT *pDr, LCD_RECT *pSr, unsigned int *pStr, unsigned int AlignMode);

//------------------------------------------------------------------------------
#endif
//******************************************************************************
