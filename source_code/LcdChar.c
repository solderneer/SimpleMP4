//******************************************************************************
#define     _IN_LCDCHAR
//------------------------------------------------------------------------------
#include <creg.h>
#include "Macro.h"
#include "Global.h"
#include "Lcd.h"
#include "LcdChar.h"
#include "Byte2Word.h"

#define		UNICODE_IN_NF_BASE_ADDR	        (unsigned long)0x3c0000     /*0x40000*/
#define		GBK2UNICODE_NF_BASE_ADDR		(unsigned long)0x5c0000

//------------------------------------------------------------------------------
/*void Byte2Word(unsigned int *pbuf, unsigned int *pSbuf, unsigned int len){
    while(len > 0){
        *pbuf = ((*pSbuf++)&0xFF)<<8;
        *pbuf++ |= (*pSbuf++ & 0xFF);
        len--;
    }
}
*/

void ConTime2String(int Sign, unsigned int Hour, unsigned int Minute, unsigned int Second, unsigned int *pStr)
{
    if (Sign < 0) {
        *pStr++ = '-';
    }
    
    //if (Hour > 0) {
        *pStr++ = Hour/100+'0';
        if (Hour/100 == 0) *pStr--;
        Hour = Hour % 100;
        *pStr++ = Hour/10+'0';
        *pStr++ = Hour%10+'0';
        *pStr++ = ':';
    //}
    *pStr++ = Minute/10+'0';
    *pStr++ = Minute%10+'0';
    *pStr++ = ':';
    *pStr++ = Second/10+'0';
    *pStr++ = Second%10+'0';
    *pStr++ = 0;  
}

unsigned int Gbk2Unicode(unsigned int code)
{
    unsigned long CharInNFAddr;
    unsigned int  DispCharBuf[2];
	unsigned long ContUnicodeNfa;
	unsigned int Offset;

    //CharInNFAddr = GBK2UNICODE_NF_BASE_ADDR + (unsigned long)(((unsigned long)(code))<<1);

    //CharInNFAddr = GBKLogicAddress + (unsigned long)(((unsigned long)(code))<<1);
    
    switch (Language) {
        case LANGUE_CHINESE_S://GBK
            Offset = 0;
            break;
        case LANGUE_CHINESE_T://BIG5
            Offset = 1;
            break;
        case LANGUE_JAPANESE://日语
            Offset = 3;
            break;
        case LANGUE_KOREA://韩语
            Offset = 2;
            break;
        case LANGUE_RUSSIAN://俄语
            Offset = 5;
            break;
        default://英语 
            Offset = 4;
            break;
    }
    
    ContUnicodeNfa = GBKLogicAddress + (unsigned long)(Offset)*128*1024;
    
    CharInNFAddr = ContUnicodeNfa + (unsigned long)(((unsigned long)(code))<<1);

    FlashReadLogic(CharInNFAddr, DispCharBuf, 2);

    #ifndef FLASH_READ_X16
        Byte2Word(DispCharBuf,DispCharBuf,2);
    #endif
    if(code!=0 && DispCharBuf[0] == 0)DispCharBuf[0]=0x3000;
    return(DispCharBuf[0]);
}

unsigned int TxtGbk2Unicode(unsigned int *pSbuf, unsigned int *pTbuf, unsigned int Len)
{
    unsigned int GbkCode;
    unsigned int Ctr;
    
    unsigned int  Offset = 1;
    if(Language == LANGUE_CHINESE_S || Language == LANGUE_CHINESE_T || Language == LANGUE_KOREA || Language == LANGUE_JAPANESE){//0:gbk,1:big5,4:韩文，3:日文 需要置为2
        Offset = 2;
    }

    Ctr = 0;
    while(*pSbuf)
    {
        if(*pSbuf & 0x0080)
        {
            // zyf 2006-12-21 修正了双字节码表在转换时会出现个别无法转换的问题
            //if(*(pSbuf) == 0)break;
            if(Offset == 2){ 
                GbkCode     = (((*pSbuf)<<8) | (*(pSbuf+1) & 0x00FF)) ;
                if(*(pSbuf+1) == 0)break;
            }else{
                GbkCode     = (*pSbuf);    
            }
            *pTbuf++  = Gbk2Unicode(GbkCode);
            pSbuf       += Offset;
            
        }
        else
        {
            *pTbuf++ = *pSbuf++;
        }
        Ctr++;
    }
    *pTbuf = 0;
    return(Ctr);
}

void LCD_NFDispUnicodeCharAt(int x, int y, unsigned int c){
    LCD_Context.DispPosX = x;
    LCD_Context.DispPosY = y;
    LCD_NFDispUnicodeChar(c);
}

void LCD_NFDispUnicodeChar(unsigned int c)
{
#if (SYSTEM_CHAR_FONT == FONT_12x12)
    unsigned int            DispCharBuf[26];
#else
    unsigned int            DispCharBuf[34];
#endif
    unsigned long           CharInNFAddr;
    int                     x,y,xsize,ysize;
    unsigned int            D_mode, Old_mode;
    
    if(c == 0)
        return;
    
    //CharInNFAddr = UNICODE_IN_NF_BASE_ADDR + (unsigned long)((unsigned long)(c)<<5);
#if (SYSTEM_CHAR_FONT == FONT_12x12)
    CharInNFAddr = FontLogicAddress + (unsigned long)((unsigned long)(c)<<5);
#else
    CharInNFAddr = FontLogicAddress + (unsigned long)((unsigned long)(c)*33);
#endif
    /*  从FLASH中读取字模数据，如有需要须重新拼接数据   */
    
#if (SYSTEM_CHAR_FONT == FONT_12x12)
    FlashReadLogic(CharInNFAddr, DispCharBuf, 26);
#else
    FlashReadLogic(CharInNFAddr, DispCharBuf, 34);
#endif

    #ifndef FLASH_READ_X16
#if (SYSTEM_CHAR_FONT == FONT_12x12)
        Byte2Word(DispCharBuf,DispCharBuf,13);
#else
        Byte2Word(DispCharBuf,DispCharBuf,17);
#endif
    #endif
    D_mode   = LCD_Context.TextMode;
    Old_mode = LCD_SetDrawMode(D_mode);
    x        = LCD_Context.DispPosX;
    y        = LCD_Context.DispPosY;
#if (SYSTEM_CHAR_FONT == FONT_12x12)    
    if((*(DispCharBuf+12)>>8) == 0x10){
#else
    if((*(DispCharBuf+16)>>8) == 0x10){
#endif
        xsize = AS_CHAR_XSIZE;
        ysize = AS_CHAR_YSIZE;
#if (SYSTEM_CHAR_FONT == FONT_12x12) 
    }else if((*(DispCharBuf+12)>>8) == 0x20){
#else
    }else if((*(DispCharBuf+16)>>8) == 0x20){
#endif
        xsize = CH_CHAR_XSIZE;
        ysize = CH_CHAR_YSIZE;
    }else{
        LCD_SetDrawMode(Old_mode);
        return;
    }

    LCD_DrawBitmap(x, y, xsize, ysize, 1, 1, DispCharBuf);
    if((LCD_Context.XDist > 0) || (LCD_Context.YDist > 0))
    {
        if(D_mode != LCD_DRAWMODE_TRANS)
        {
            LCD_SetDrawMode(D_mode ^ LCD_DRAWMODE_REV); 
            LCD_FillRect(x+xsize, y, x+xsize+LCD_Context.XDist, y+ysize+LCD_Context.YDist);
            LCD_FillRect(x, y+ysize, x+xsize, y+ysize+LCD_Context.YDist);                        
        }
    }
    LCD_SetDrawMode(Old_mode);
    if(LCD_Context.TextMode != LCD_DRAWMODE_CIR90)
        LCD_Context.DispPosX += (xsize + LCD_Context.XDist);
    else
        LCD_Context.DispPosY += (xsize + LCD_Context.YDist);
}

int LCD_GetCharXSize(unsigned int s)
{

    unsigned int  DispCharBuf[2];
    unsigned long CharInNFAddr;
    //CharInNFAddr = UNICODE_IN_NF_BASE_ADDR + (unsigned long)((unsigned long)(s)<<5)+24;
#if (SYSTEM_CHAR_FONT == FONT_12x12)
    CharInNFAddr = FontLogicAddress + (unsigned long)((unsigned long)(s)<<5)+24;
#else
    CharInNFAddr = FontLogicAddress + (unsigned long)((unsigned long)(s)*33)+32;
#endif
    FlashReadLogic(CharInNFAddr, DispCharBuf, 2);
    //#ifndef FLASH_READ_X16
    //    Byte2Word(DispCharBuf,DispCharBuf,2);
    //#endif
    //DispCharBuf[0] >>= 8;
    
    if(0x20 == DispCharBuf[0]){
        return(CH_CHAR_XSIZE);
    }else
        return(AS_CHAR_XSIZE);  


/*
    if(s > 0x7F)
        return(12);
    else 
        return(6);
*/
}

int LCD_GetCharsPerLine(LCD_RECT *r, unsigned int *s)
{
    int NumChar =0;
    int StrDistX=0;

    while(*s){
        StrDistX += (LCD_Context.XDist + LCD_GetCharXSize(*s));
        if((*s == '\n') || (*s == '\r') || (*s == 0) || (StrDistX > (r->x1 - r->x0)))
            break;
        NumChar++;
        s++; 
    }
    return(NumChar);
}

int LCD_GetLineDistX(unsigned int *s, int LineNumChar)
{
    int DistX=0;
    for(; LineNumChar>0; LineNumChar--)
    {
        DistX += (LCD_Context.XDist + LCD_GetCharXSize(*s++));  
    }      
    return(DistX);
}

void LCD_DispLineChar(unsigned int *s, int LineNumChar)
{
    for(; LineNumChar>0; LineNumChar--)
    {
        LCD_NFDispUnicodeChar(*s++);
    }
}

int LCD_GetStringSize(unsigned int *s) {
    unsigned int  CharSize = 0;
    while(*s){
        CharSize += (LCD_Context.XDist + LCD_GetCharXSize(*s++));
    }
    return(CharSize);
}

void LCD_NFDispString(unsigned int *pStr)
{
    unsigned long   CharInNFAddr;
    unsigned int    DispCharBuf;
    unsigned int    CharXSize=0;
    unsigned int    StringSize=0;
    int             xAdjust = 0;
    int             Old_DispPosX;
    LCD_RECT        r;

    Old_DispPosX = LCD_Context.DispPosX;
    
    StringSize = LCD_GetStringSize(pStr);
    
    switch(LCD_Context.TextAlign & LCD_TEXTALIGN_HORIZONTAL)
    {
        case LCD_TEXTALIGN_RIGHT:
            xAdjust = StringSize;
            break;
        case LCD_TEXTALIGN_CENTER:
            xAdjust = StringSize/2;
            break;
        default:
            xAdjust = 0;    
    }
    LCD_Context.DispPosX -= xAdjust;
    while(*pStr){
        //LCD_Context.DispPosX -= xAdjust;
        if(*pStr == '\n')
        {
            switch(LCD_Context.TextAlign & LCD_TEXTALIGN_HORIZONTAL)
            {
                case LCD_TEXTALIGN_RIGHT:
                case LCD_TEXTALIGN_CENTER:
                    LCD_Context.DispPosX = Old_DispPosX;
                    break;
                default :
                    LCD_Context.DispPosX = LCD_Context.LBorder;
                    break;
            }
            LCD_Context.DispPosY += (LCD_Context.YDist + CH_CHAR_YSIZE);   
        }
        else
        {   
            LCD_NFDispUnicodeChar(*pStr);
        }
        pStr++;
    }
}

void LCD_NFDispStringAt(int x, int y, unsigned int *pStr)
{
    LCD_Context.DispPosX = x;
    LCD_Context.DispPosY = y;
    LCD_NFDispString(pStr);
}


void LCD_DispStringInRect(LCD_RECT *pDr, LCD_RECT *pSr, unsigned int *pStr, unsigned int AlignMode)
{
    LCD_RECT old_r;
    int      NumChar;
    int      xDist;
    int      old_DispPosX;
    int      xAdjust;
    
    old_r = LCD_Context.ClipRect;
    
    LCD_Context.ClipRect = *pDr;
    LCD_Context.DispPosY = pSr->y0;

    switch(AlignMode & LCD_TEXTALIGN_HORIZONTAL)
    {
        case LCD_TEXTALIGN_RIGHT:
            LCD_Context.DispPosX = pSr->x1;
            break;
        case LCD_TEXTALIGN_CENTER:
            LCD_Context.DispPosX = pSr->x0 + (pSr->x1 - pSr->x0)/2;
            break;
        default:
            LCD_Context.DispPosX = pSr->x0;
            break;  
    }
    
    old_DispPosX = LCD_Context.DispPosX;

    while(*pStr)
    {
        NumChar = LCD_GetCharsPerLine(pSr, pStr);
        xDist   = LCD_GetLineDistX(pStr, NumChar);
        
        switch(AlignMode & LCD_TEXTALIGN_HORIZONTAL)
        {
            case LCD_TEXTALIGN_RIGHT:
                xAdjust = xDist;
                break;
            case LCD_TEXTALIGN_CENTER:
                xAdjust = xDist/2;
                break;
            default:
                xAdjust = 0;
                break;  
        }
        LCD_Context.DispPosX -= xAdjust;
        LCD_DispLineChar(pStr, NumChar);
        pStr += NumChar;
        
        if(*pStr == '\n' )
        {
            pStr++;
        }
        LCD_Context.DispPosX = old_DispPosX;
        LCD_Context.DispPosY += (LCD_Context.YDist + CH_CHAR_YSIZE);
        if(LCD_Context.DispPosY > LCD_Context.ClipRect.y1)
            break;
    }
    
    LCD_Context.ClipRect = old_r;
} 


//******************************************************************************

//SCROLL_STRING_STRUCT Scroll;
//
///*
//**********************************************************
//*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
//*  Function name : LCD_ScrollStringAt()
//*  Author:         ZhenFu Fang
//*  Description:    字符串滚动显示
//*  Calls:          
//*  Input:          无
//*  Return:         无
//*  Remark:         picx:    字符显示底图的 x 坐标
//*                  picxsize:字符显示底图的 x 大小        
//*                       
//*  History:        
//*           <author>      <time>     <version>       <desc>
//*           FZF          06/08/21      1.0           ORG
//*
//***********************************************************
//*/
void LCD_ScrollStringAt(LCD_RECT *r, unsigned int *pstr, unsigned int xcont,
                        int picx, int picxsize, unsigned long picnfa)
{
    ScrollString.xIntVal  = xcont;
    ScrollString.xIntVal1 = -(r->x1 - r->x0); 
    ScrollString.x       = 0;
    ScrollString.x1      = -((r->x1 - r->x0)+5);
    ScrollString.r.x0    = r->x0;
    ScrollString.r.y0    = r->y0;
    ScrollString.r.x1    = r->x1;
    ScrollString.r.y1    = r->y1;
    
    ScrollString.pStr    = pstr;
    
    ScrollString.color   = LCD_GetColor();
    
    ScrollString.BkPicX     = picx;
    ScrollString.BkPicXsize = picxsize;
    ScrollString.BkPicNfa   = picnfa;
    
    LCD_SetDispRect(r->x0, r->y0, r->x1, r->y1);
    DisplayPicture(picx, r->y0, picxsize, CH_CHAR_YSIZE, picnfa);
    LCD_NFDispStringAt(r->x0, r->y0, pstr);
    LCD_SetDispRect(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);        
}                        

void ScrollStringInRect(void)
{
    unsigned int color_tmp;
    unsigned int imask_tmp;
    unsigned int str_x;
    
    read_creg(%imask, imask_tmp);
    bitclear_creg(%imask, 5);
    if(ScrollString.flag != 0){
        if(TimerGetState(SCROLL_TIMER_CHANNEL) == 0){
            ScrollString.flag = 0;
            write_creg(%imask, imask_tmp);
            return;
        }
        
        if((ScrollString.x >= (-(ScrollString.r.x1-ScrollString.r.x0)))&&
           (ScrollString.x <= (ScrollString.r.x1-ScrollString.r.x0+ScrollString.xIntVal))){
            ScrollString.x  += ScrollString.flag;
        }
        if((ScrollString.x1 >= (-(ScrollString.r.x1-ScrollString.r.x0)))&&
           (ScrollString.x1 <= (ScrollString.r.x1-ScrollString.r.x0+ScrollString.xIntVal))){
            ScrollString.x1 += ScrollString.flag;
        }
        ScrollString.flag = 0;
        write_creg(%imask, imask_tmp);
        
        LCD_SetDispRect(ScrollString.r.x0, ScrollString.r.y0, ScrollString.r.x1, ScrollString.r.y1);
        DisplayPicture(ScrollString.BkPicX, ScrollString.r.y0, ScrollString.BkPicXsize, CH_CHAR_YSIZE, ScrollString.BkPicNfa);
        color_tmp = LCD_GetColor();
        LCD_SetColor(ScrollString.color);
        LCD_NFDispStringAt(ScrollString.r.x0-ScrollString.x, ScrollString.r.y0, ScrollString.pStr);
        LCD_NFDispStringAt(ScrollString.r.x0-ScrollString.x1,ScrollString.r.y0, ScrollString.pStr);
        LCD_SetColor(color_tmp);
        LCD_SetDispRect(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
        if(ScrollString.x == (ScrollString.xIntVal+(ScrollString.r.x1-ScrollString.r.x0)/2)){
//            ScrollString.x  = -1;
            ScrollString.x1 = -(ScrollString.r.x1-ScrollString.r.x0);
        }
        if(ScrollString.x1 == (ScrollString.xIntVal+(ScrollString.r.x1-ScrollString.r.x0)/2)){
            ScrollString.x  = -(ScrollString.r.x1-ScrollString.r.x0);
//            ScrollString.x1 = -1;
        } 
    }
    write_creg(%imask, imask_tmp);
}
//
//void ScrollStringDispDisable(void)
//{
//    unsigned int imask_tmp;
//    read_creg(%imask, imask_tmp);
//    bitclear_creg(%imask, 15);
//    Scroll.flag  = 0;
//    Scroll.timer = 0;
//    Scroll.DiapInterface = 0xFF00;
//    write_creg(%imask, imask_tmp);
//}
