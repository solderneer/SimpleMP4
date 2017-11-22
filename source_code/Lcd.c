//******************************************************************************
#define _IN_LCD
//------------------------------------------------------------------------------
#include <creg.h>
#include "Memap.h"
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Lcd.h"
#include "resource.h"
#include "LcdChar.h"

#include "Dma.h"

//#ifdef LCD_LTPS
//	#include "LCD_LTPS.h"
//#endif

//------------------------------------------------------------------------------
extern void FlashReadLogic(unsigned long x,unsigned int *Buffer,unsigned int Length);

#define	RETURN_X_OUT() \
	if(x < LCD_Context.ClipRect.x0)return;	\
	if(x > LCD_Context.ClipRect.x1)return;

#define	RETURN_Y_OUT() \
	if(y < LCD_Context.ClipRect.y0)return;	\
	if(y > LCD_Context.ClipRect.y1)return;
	
#define CLIP_X() \
	if(x0 < LCD_Context.ClipRect.x0){x0 = LCD_Context.ClipRect.x0;}	\
	if(x1 > LCD_Context.ClipRect.x1){x1 = LCD_Context.ClipRect.x1;}
	
#define CLIP_Y() \
	if(y0 < LCD_Context.ClipRect.y0){y0 = LCD_Context.ClipRect.y0;}	\
	if(y1 > LCD_Context.ClipRect.y1){y1 = LCD_Context.ClipRect.y1;}
	
//------------------------------------------------------------------------------
void LCD_SetDiaplayMode(int mode)
{
    LCD_Context.Mode = mode;
    LCD_Context.ClipRect.x0 = 0;
    LCD_Context.ClipRect.y0 = 0;
    if(mode != 0){
        LCD_Context.LcdMaxXsize = LCD_MAX_YSIZE;
	    LCD_Context.LcdMaxYsize = LCD_MAX_XSIZE;
    }else{
        LCD_Context.LcdMaxXsize = LCD_MAX_XSIZE;
	    LCD_Context.LcdMaxYsize = LCD_MAX_YSIZE;
    }
    LCD_Context.ClipRect.x1 = LCD_Context.LcdMaxXsize-1;
    LCD_Context.ClipRect.y1 = LCD_Context.LcdMaxYsize-1;
}


unsigned int LCD_GetBkColor(void)
{
	return(LCD_BKCOLOR);	
}
unsigned int LCD_GetColor(void)
{
	return(LCD_COLOR);	
}

void LCD_SetBkColor(unsigned int color)
{
	LCD_BKCOLOR = color;
}

void LCD_SetColor(unsigned int color)
{
	LCD_COLOR = color;	
}

unsigned int LCD_SetDrawMode(unsigned int mode)
{
	unsigned int old_mode;
	unsigned int temp;
	old_mode = LCD_Context.DrawMode;
	//if((LCD_Context.DrawMode^mode)&LCD_DRAWMODE_REV){
	//	temp        = LCD_BKCOLOR;
	//	LCD_BKCOLOR = LCD_COLOR;
	//	LCD_COLOR   = temp;
	//}
	LCD_Context.DrawMode = mode;
	return(old_mode);
}

void LCD_DrawPixel(int x, int y)
{
	RETURN_X_OUT();
	RETURN_Y_OUT();
	if(LCD_Context.DrawMode & LCD_DRAWMODE_XOR){
		LCDDEV_XorPixel(x,y);
	}else{
		LCDDEV_SetPixel(x,y,LCD_COLOR);
	}	
}

void LCD_DrawHLine(int x0, int y, int x1)
{
	RETURN_Y_OUT();
	CLIP_X();
	if(x1 < x0)
		LCDDEV_DrawHLine(x1, y, x0);
	else
		LCDDEV_DrawHLine(x0, y, x1);
}

void LCD_DrawVLine(int x, int y0, int y1)
{
	RETURN_X_OUT();
	CLIP_Y();
	if(y1 < y0)
		LCDDEV_DrawVLine(x, y1, y0);
	else
		LCDDEV_DrawVLine(x, y0, y1);	
}

void LCD_FillRect(int x0, int y0, int x1, int y1)
{
	int temp;
	
	CLIP_X();
	CLIP_Y();
	if(x1 < x0){
		temp = x0;
		x0   = x1;
		x1   = temp;
	}	
	if(y1 < y0){
		temp = y0;
		y0   = y1;
		y1   = temp;
	}
	LCDDEV_FillRect(x0, y0, x1, y1);
}


void LCD_DrawBitmap(int x0, int y0, int xsize, int ysize,
                    int BitsPerPixel, int BytesPerLine, unsigned int *pPixel)
{
  	int x1, y1;
    int Diff;
    
  	y1 = y0 + ysize - 1;
  	x1 = x0 + xsize - 1;

    Diff = LCD_Context.ClipRect.y0 - y0;
    if (Diff > 0) {
      	ysize -= Diff;
      	if (ysize <= 0) {
		    return;
      	}
      	y0     = LCD_Context.ClipRect.y0;
        pPixel += (unsigned)Diff * (unsigned)BytesPerLine;
    }
    Diff = y1 - LCD_Context.ClipRect.y1;
    if (Diff > 0) {
      	ysize -= Diff;
      	if (ysize <= 0) {
		    return;
      	}
    }
    Diff = x1 - LCD_Context.ClipRect.x1;
    if (Diff > 0)
      	xsize -= Diff;
    Diff = 0;
    if (x0 < LCD_Context.ClipRect.x0) {
      Diff = LCD_Context.ClipRect.x0 - x0;
			xsize -= Diff;
			switch (BitsPerPixel) {
			case 1:
  				pPixel+= (Diff>>7); x0 += (Diff>>7)<<7; Diff &=15;
				break;
			case 16:
				//pPixel+= (Diff<<1); x0 += Diff; Diff=0;
				pPixel += Diff; x0 += Diff; Diff=0;
				break;
			}
    }
    if (xsize <=0) {
		  return;
    }
    LCDDEV_DrawBitmap   (x0,y0, xsize, ysize, BitsPerPixel, BytesPerLine, pPixel, Diff);
}


#define		LCD_DISPLAY_BUF		256

#if(LCD_DISPLAY_BUF <= 256)
#define DISP_PIC_BUF_SIZE       (LCD_MAX_XSIZE<<1)
void DisplayPicture(int x, int y, int xsize, int ysize, unsigned long Flash_Addr)
{
	unsigned int 	DispPicBuf[DISP_PIC_BUF_SIZE];
	unsigned int 	DispPicLen;
	int	ysize_Seg;
	int	ysize_Spare;
	int	Disp_Counter;
	if((xsize == 0) || (ysize == 0))
	    return;
	
//	DispPicLen = (unsigned long)(xsize)*ysize;
	/*	????????????NAND??Flash??????????	*/	
	if(((unsigned long)(xsize)*ysize) <= (DISP_PIC_BUF_SIZE>>1)){
		/*	????NAND??Flash????????????????		*/
		//FlashReadLogic(Flash_Addr, DispPicBuf, 512);
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, DISP_PIC_BUF_SIZE);
		/*	??????FLASH??????????????????????????????????	*/
		Byte2Word(DispPicBuf, DispPicBuf, (DISP_PIC_BUF_SIZE>>1));
		LCD_DrawBitmap(x, y, xsize, ysize, 16, xsize, DispPicBuf);
		return;
	}
	ysize_Seg    = (DISP_PIC_BUF_SIZE>>1) / xsize;
	if(ysize_Seg == 0)return;
	Disp_Counter = ysize / ysize_Seg;
	ysize_Spare  = ysize % ysize_Seg;

	DispPicLen   = ysize_Seg * xsize;
		
	for(; Disp_Counter > 0; Disp_Counter--){
		/*	??????????????????????NAND??FLASH????????				*/
		/*	??NAND??FLASH??????????????????????????????DispPicLen??	*/
		//FlashReadLogic(Flash_Addr, DispPicBuf, DispPicLen*2);
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, DispPicLen*2);
		/*	??????FLASH??????????????????????????????????	*/
		Byte2Word(DispPicBuf, DispPicBuf, DispPicLen);
		LCD_DrawBitmap(x, y, xsize, ysize_Seg, 16, xsize, DispPicBuf);
		y += ysize_Seg;
		Flash_Addr += DispPicLen*2;
	}
	if(ysize_Spare){
		DispPicLen = ysize_Spare*xsize;
		/*	??????????????????NAND??FLASH????????						*/
		/*	??NAND??FLASH??????????????????????????????????DispPicLen??	*/
		//FlashReadLogic(Flash_Addr, DispPicBuf, DispPicLen*2);
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, DispPicLen*2);
		/*	??????FLASH??????????????????????????????????	*/
		Byte2Word(DispPicBuf, DispPicBuf, DispPicLen);
		LCD_DrawBitmap(x, y, xsize, ysize_Spare, 16, xsize, DispPicBuf);
	}
}
#endif

#if 0
void LCD_FillRectImage(int x, int y, int xsize, int ysize, unsigned long Flash_Addr)
{
	unsigned int 	DispPicBuf[512];
	unsigned int 	DispPicLen;
	unsigned int    i,x1, y1;
	int	ysize_Spare;
	int	Disp_Counter;
	
	if((xsize == 0) || (ysize == 0))
	    return;
	
	DispPicLen = xsize*ysize;
	Disp_Counter = DispPicLen/256;
	ysize_Spare  = DispPicLen%256;
	
    x1 = x+xsize-1;
	y1 = y+ysize-1;
	LCDDEV_SetWindow(x, y, x1, y1);
	
	for(; Disp_Counter > 0; Disp_Counter--){
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, 512);
		Byte2Word(DispPicBuf, DispPicBuf, 256);
		for(i=0; i<256; i++){
            #if(DATA_BUS_WIDTH == 8)   
                write_XDATA(LCD_DATA, DispPicBuf[i]>>8);		    
			    write_XDATA(LCD_DATA, DispPicBuf[i]);
			#else
			    write_XDATA(LCD_DATA, DispPicBuf[i]);
			#endif
		}
		Flash_Addr += 512;
	}
	if(ysize_Spare){
		DispPicLen = ysize_Spare;
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, DispPicLen*2);
		Byte2Word(DispPicBuf, DispPicBuf, DispPicLen);
		for(i=0; i<DispPicLen; i++){
			#if(DATA_BUS_WIDTH == 8)   
                write_XDATA(LCD_DATA, DispPicBuf[i]>>8);		    
			    write_XDATA(LCD_DATA, DispPicBuf[i]);
			#else
			    write_XDATA(LCD_DATA, DispPicBuf[i]);
			#endif
		}
	}
	LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
}
#endif
#if	(LCD_DISPLAY_BUF > 256)
void DisplayPicture(int x, int y, int xsize, int ysize, unsigned long Flash_Addr)
{
	unsigned int 	DispPicBuf[2048];
	unsigned int 	DispPicLen;
	int	ysize_Seg;
	int	ysize_Spare;
	int	Disp_Counter;
	
	DispPicLen = xsize*ysize;
	/*	????????????NAND??Flash??????????	*/	
	if(DispPicLen <= 1024){
		/*	????NAND??Flash????????????????		*/
		//FlashReadLogic(Flash_Addr, DispPicBuf, 2048);
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, 2048);
		/*	??????FLASH??????????????????????????????????	*/
		Byte2Word(DispPicBuf, DispPicBuf, 1024);
		LCD_DrawBitmap(x, y, xsize, ysize, 16, xsize, DispPicBuf);
		return;
	}
	ysize_Seg    = 2048 / xsize;
	Disp_Counter = ysize / ysize_Seg;
	ysize_Spare  = ysize % ysize_Seg;

	DispPicLen   = ysize_Seg * xsize;
		
	for(; Disp_Counter > 0; Disp_Counter--){
		/*	??????????????????????NAND??FLASH????????				*/
		/*	??NAND??FLASH??????????????????????????????DispPicLen??	*/
		//FlashReadLogic(Flash_Addr, DispPicBuf, DispPicLen*2);
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, DispPicBuf*2);
		/*	??????FLASH??????????????????????????????????	*/
		Byte2Word(DispPicBuf, DispPicBuf, DispPicLen);
		LCD_DrawBitmap(x, y, xsize, ysize_Seg, 16, xsize, DispPicBuf);
		y += ysize_Seg;
		Flash_Addr += DispPicLen*2;
	}
	if(ysize_Spare){
		DispPicLen = ysize_Spare*xsize;
		/*	??????????????????NAND??FLASH????????						*/
		/*	??NAND??FLASH??????????????????????????????????DispPicLen??	*/
		//FlashReadLogic(Flash_Addr, DispPicBuf, DispPicLen*2);
		FlashReadLogic(Flash_Addr + ImageLogicAddress, DispPicBuf, DispPicBuf*2);
		/*	??????FLASH??????????????????????????????????	*/
		Byte2Word(DispPicBuf, DispPicBuf, DispPicLen);
		LCD_DrawBitmap(x, y, xsize, ysize_Spare, 16, xsize, DispPicBuf);
	}
}
#endif

unsigned int LCD_SetTextMode(unsigned int mode)
{
	unsigned int mode_temp;
	mode_temp = LCD_Context.TextMode;
	LCD_Context.TextMode = mode;
	return(mode_temp);	
}

void LcdInit(void)
{
	LCD_RECT *r;
	
	r = &LCD_Context.ClipRect;
	LCD_SetBkColor(0xFFFF);
	LCD_SetColor(0x0000);
	r->x0 = 0;
	r->y0 = 0;
	r->x1 = LCD_MAX_XSIZE-1;
	r->y1 = LCD_MAX_YSIZE-1;
	LCD_SetDiaplayMode(0);
	LCD_Context.DrawMode = LCD_DRAWMODE_NORMAL;
	LCD_SetTextMode(LCD_DRAWMODE_TRANS);
	//LCD_SetDrawMode(LCD_DRAWMODE_NORMAL);
	LCD_Context.DispPosX = 0;
	LCD_Context.DispPosY = 0;
	LCD_Context.LBorder  = 0;
	LCD_Context.XDist  = 0;
	LCD_Context.YDist  = 0;
	LCD_Context.TextAlign = LCD_TEXTALIGN_LEFT;
	LCD_Context.RBorder  = LCD_MAX_XSIZE-1;
	PowerOnLcdinit();
	//LCD_InitOne();
	//Display_Sub(LCD_BKCOLOR);
	LCD_FillRect(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
    Delay1Ms(30);
	LCD_SetTextMode(LCD_DRAWMODE_TRANS);
}

void LCD_SetDispRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
    LCD_Context.ClipRect.x0 = x0;
    LCD_Context.ClipRect.y0 = y0;
    if(LCD_Context.Mode != 0){
        LCD_Context.ClipRect.x1 = y1;
        LCD_Context.ClipRect.y1 = x1;
    }else{
        LCD_Context.ClipRect.x1 = x1;
        LCD_Context.ClipRect.y1 = y1;
    }
} 
//******************************************************************************

void DisplayMenuStr(unsigned int x, unsigned int y, unsigned int xsize, 
                    unsigned int ysize, unsigned int mode, unsigned long flash_addr){
    LCD_RECT       r;
    unsigned int TmpBuf[64];
    unsigned int color_tmp;
    
    flash_addr += MenuLogicAddress;
    LanguageResoureOffset = (unsigned long)(Language)*MENU_LANGUE_PER_BYTE;//2006-2-23 21:03 
    if(Language < LANGUAGE_MAX_COUNT) flash_addr +=LanguageResoureOffset;
        
    FlashReadLogic(flash_addr, TmpBuf, 64);
    Byte2Word(TmpBuf, TmpBuf, 64);
    
    r.x0 = x;
    r.y0 = y;
    r.x1 = x+xsize-1;
    r.y1 = y+ysize-1;
    
    LCD_DispStringInRect(&r,&r,TmpBuf, mode);
}

void SysDispDialogBox(unsigned int x, unsigned int y, unsigned long TitleFlashAddr,unsigned long ContentFlashAddr)
{
    LCD_RECT       r;
    unsigned int   StrBuf[8];
    unsigned int   color_tmp;
    unsigned int   mode_tmp;
    
    MusicSetFreq51();
    
    color_tmp = LCD_GetColor();
    LCD_SetColor(COLOR_BLACK);
    //LCD_SetDispRect(MESSAGE_BOX_PIC_X, MESSAGE_BOX_PIC_Y, (MESSAGE_BOX_PIC_X + MESSAGE_BOX_PIC_XSIZE - 1), (MESSAGE_BOX_PIC_Y + MESSAGE_BOX_PIC_YSIZE - 1));
    DisplayPicture(MESSAGE_BOX_PIC_X, MESSAGE_BOX_PIC_Y, MESSAGE_BOX_PIC_XSIZE, MESSAGE_BOX_PIC_YSIZE, MESSAGE_BOX_PIC);
    DisplayMenuStr(MSG_TEXT_TITLE_X - 8, MSG_TEXT_TITLE_Y, MSG_TEXT_TITLE_XSIZE, MSG_TEXT_TITLE_YSIZE, LCD_TEXTALIGN_CENTER, TitleFlashAddr);
    DisplayMenuStr(MSG_TEXT_CONTENT_X ,MSG_TEXT_CONTENT_Y, MSG_TEXT_CONTENT_XSIZE, MSG_TEXT_CONTENT_YSIZE, LCD_TEXTALIGN_CENTER, ContentFlashAddr);
    LCD_SetColor(color_tmp);
    
    MusicSetFreq51Return();
}

void DispSelectDialogBox(unsigned int x, unsigned int y, unsigned int mode, unsigned long title_addr)
{
    unsigned int color_tmp;
    
    MusicSetFreq51();
    
    mode &= 0x01;
    
    //LCD_SetDispRect(DIALOG_BOX_PIC_X, DIALOG_BOX_PIC_Y, (DIALOG_BOX_PIC_X + DIALOG_BOX_PIC_XSIZE - 1), (DIALOG_BOX_PIC_Y + DIALOG_BOX_PIC_YSIZE - 1));
    DisplayPicture(DIALOG_BOX_PIC_X, DIALOG_BOX_PIC_Y, DIALOG_BOX_PIC_XSIZE, DIALOG_BOX_PIC_YSIZE, DIALOG_BOX_PIC);
    DisplayPicture(DIALOG_BUTTON_YES_PIC_X, DIALOG_BUTTON_YES_PIC_Y, DIALOG_BUTTON_PIC_XSIZE, DIALOG_BUTTON_PIC_YSIZE, (DIALOG_BUTTON_NOSEL + (unsigned long)(1 - mode) * DIALOG_BUTTON_PIC_SIZE));
    DisplayPicture(DIALOG_BUTTON_NO_PIC_X, DIALOG_BUTTON_NO_PIC_Y, DIALOG_BUTTON_PIC_XSIZE, DIALOG_BUTTON_PIC_YSIZE, (DIALOG_BUTTON_NOSEL + (unsigned long)(mode) * DIALOG_BUTTON_PIC_SIZE));
    
    color_tmp = LCD_GetColor();    
    LCD_SetColor(COLOR_BLACK);
    DisplayMenuStr(DIALOG_TEXT_TITLE_X - 8, DIALOG_TEXT_TITLE_Y, DIALOG_TEXT_TITLE_XSIZE, DIALOG_TEXT_TITLE_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_WARNING_TXT);   //Title
    DisplayMenuStr(DIALOG_YES_TXT_X, DIALOG_YES_TXT_Y, DIALOG_YES_TXT_XSIZE, DIALOG_YES_TXT_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_YES_TXT);                       //Yes
    DisplayMenuStr(DIALOG_NO_TXT_X, DIALOG_NO_TXT_Y, DIALOG_NO_TXT_XSIZE, DIALOG_NO_TXT_YSIZE, LCD_TEXTALIGN_CENTER, DIALOG_NO_TXT);                            //No
    DisplayMenuStr(DIALOG_TEXT_CONTENT_X, DIALOG_TEXT_CONTENT_Y, DIALOG_TEXT_CONTENT_XSIZE, DIALOG_TEXT_CONTENT_YSIZE, LCD_TEXTALIGN_CENTER, title_addr);       //Content
    LCD_SetColor(color_tmp); 
    
    MusicSetFreq51();
}

void DisplayTestDecNum(unsigned int x, unsigned y, unsigned int DecVal)
{
    unsigned int temp;
    unsigned int Temp_BKColor, Temp_Color;
    unsigned int Temp_TxtDrawMode;
    unsigned int DisplayBuf[8];
    
    temp = DecVal / 10000;
    DisplayBuf[0] = temp + '0';
    temp = DecVal % 10000;
    DisplayBuf[1] = temp/1000 + '0';
    temp = temp % 1000;
    DisplayBuf[2] = temp/100 + '0';
    temp = temp % 100;
    DisplayBuf[3] = temp/10 + '0';
    DisplayBuf[4] = temp%10 + '0';
    DisplayBuf[5] = 0;
    
    Temp_Color = LCD_GetColor();
    LCD_SetColor(COLOR_WHITE);
    Temp_BKColor = LCD_GetBkColor();
    LCD_SetBkColor(COLOR_BLACK);
    Temp_TxtDrawMode = LCD_SetTextMode(LCD_DRAWMODE_NORMAL);
    LCD_NFDispStringAt(x, y, DisplayBuf);
#ifdef LCD_MEMORY_DEV 
    DmaFillImage2Lcd();
#endif
    LCD_SetTextMode(Temp_TxtDrawMode);
    LCD_SetBkColor(Temp_BKColor);
    LCD_SetColor(Temp_Color);
}

void LCDDEV_DrawHLine(int x0, int y, int x1)
{
#ifdef LCD_MEMORY_DEV
    LCDDEV_FillRect(x0, y, x1, y);
#else    
	if (LCD_Context.DrawMode & LCD_DRAWMODE_XOR){
		for(; x0<=x1; x0++){
			LCDDEV_XorPixel(x0, y);
		}
	}else{
		LCDDEV_SetWindow(x0, y, x1, y);
		for(; x0<=x1; x0++){
			LCD_Data(LCD_COLOR);
		}
		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
	}
#endif	
}

void LCDDEV_DrawVLine(int x, int y0, int y1)
{
#ifdef LCD_MEMORY_DEV
    LCDDEV_FillRect(x, y0, x, y1);
#else    
	if (LCD_Context.DrawMode & LCD_DRAWMODE_XOR){
		for(; y0<=y1; y0++){
			LCDDEV_XorPixel(x, y0);
		}
	}else{
		LCDDEV_SetWindow(x, y0, x, y1);
		for(; y0<=y1; y0++){
			LCD_Data(LCD_COLOR);
		}	
		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
	}
#endif	
}

#ifdef LCD_MEMORY_DEV
void LCDDEV_FillRect(int x0, int y0, int x1, int y1)
{
    unsigned long dst_addr;
    unsigned long sar_addr;
    unsigned int  xsize, ysize;
    unsigned int  cont;

    DMA_CHANNEL_REG *DmaChl;
    
    unsigned int imask_tmp;
    
    while(DmaFillImage2LcdFlag);

    xsize = x1-x0+1;
    ysize = y1-y0+1;

    for(y0; y0<=y1; y0++){
    
        while((DMA_CHANNEL_REG *)DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 
    
        dst_addr = (unsigned long)(y0)*LCD_Context.LcdMaxXsize + (unsigned long)x0  + (unsigned long)(0x3C0000);
        sar_addr = (unsigned long)((unsigned int)(&LCD_COLOR));
    
    //---------------------------------------------------------------------
        DmaChl->pDmaFnct    = DMA_FNCT_NULL;
        DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
    //--------------------------------------------------------------------- 
        
        DmaChl->DmaSar = (unsigned long)((((unsigned long)(sar_addr))<<1)+(unsigned long)0x40000);
        DmaChl->DmaDar = ((unsigned long)(dst_addr)<<1)+(unsigned long)0x4000000;
        
        if(xsize < LCD_Context.LcdMaxXsize){
            DmaChl->DmaCtl.Lsb = 0x00040413;
        }else{
            DmaChl->DmaCtl.Lsb = 0x00000413;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
        }
        DmaChl->DmaCtl.Msb = (unsigned long)(xsize);             //224*16/2
        
        DmaChl->DmaCfg.Lsb = 0x00000020;             //channel priority set
        DmaChl->DmaCfg.Msb = 0x00001004;
        
        if(xsize < LCD_Context.LcdMaxXsize){
            DmaChl->DmaSgr = 0x00000000;
            DmaChl->DmaDsr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_Context.LcdMaxXsize)-xsize));
        }else{
            DmaChl->DmaSgr = 0x00000000;
            DmaChl->DmaDsr = 0x00000000;           //(176-16)/2
        }
        DmaChl->Priority   = 0;
        Dma_DispFlag     = 1;
        DmaChl->pResFlag = &Dma_DispFlag;
        DMA_DISABLE_INTERRUPT;
        	DmaChannelSetup(1);
        DMA_ENABLE_INTERRUPT;
        while(Dma_DispFlag);
    }
    DmaTanData2LcdCont++;
    DmaFillImage2Lcd();
}
#else
void LCDDEV_FillRect(int x0, int y0, int x1, int y1)
{
	int i;
	if (LCD_Context.DrawMode & LCD_DRAWMODE_XOR){
		for(; y0<=y1; y0++){
			LCDDEV_DrawHLine(x0, y0, x1);
		}
	}else{
		LCDDEV_SetWindow(x0, y0, x1, y1);
		for(; y0<=y1 ; y0++){
			i = x0;
			for(; i<=x1; i++){
				LCD_Data(LCD_COLOR);
			}
		}
		
		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
	}
}
#endif


#ifdef LCD_MEMORY_DEV
void LCDDEV_DrawBitLine1BPP(int x,                  //坐标 x
                            int y,                  //坐标 y
                            int xsize,              //横向上的 大小
                            int Diff,               //偏移
                            unsigned int *pLcdData,
                            unsigned int *pData)    //点阵字库字据
{
	switch(LCD_Context.DrawMode){
		case LCD_DRAWMODE_REV:
			while(xsize){
				if(*pData & (0x8000 >> Diff)){
					*(pLcdData+x) = LCD_BKCOLOR;
				}else{
                    *(pLcdData+x) = LCD_COLOR;
			    }
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
		case LCD_DRAWMODE_TRANS:
		case LCD_DRAWMODE_CIR90:
			while(xsize){
				if(*pData & (0x8000 >> Diff)){
                    *(pLcdData+x) = LCD_COLOR;
                }
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
		case LCD_DRAWMODE_XOR:
			while(xsize){
				if(*pData & (0x8000 >> Diff)){
                    *(pLcdData+x) ^= LCD_COLOR;
				}
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
//	    case LCD_DRAWMODE_CIR90:
//	        while(xsize){
//				if(*pData & (0x8000 >> Diff)){
//				    *(pLcdData+x) = LCD_COLOR;
//				}
//				y++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//	        break;
		default:
		    while(xsize){
				if(*pData & (0x8000 >> Diff)){
                    *(pLcdData+x) = LCD_COLOR;
				}else{
                    *(pLcdData+x) = LCD_BKCOLOR;
				}
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
	}
}
#else
void LCDDEV_DrawBitLine1BPP(int x, int y, int xsize, int Diff, unsigned int *pData) 
{
  	x += Diff;

	switch(LCD_Context.DrawMode){
		case LCD_DRAWMODE_REV:
			while(xsize){
				if(*pData & (0x8000 >> Diff))
					LCD_SetPixel(x, y, LCD_BKCOLOR);
				else
					LCD_SetPixel(x, y, LCD_COLOR);
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
		case LCD_DRAWMODE_TRANS:
			while(xsize){
				if(*pData & (0x8000 >> Diff))
					LCD_SetPixel(x, y, LCD_COLOR);
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
		case LCD_DRAWMODE_XOR:
			while(xsize){
				if(*pData & (0x8000 >> Diff))
					LCD_XorPixel(x,y);
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
	    case LCD_DRAWMODE_CIR90:
	        while(xsize){
				if(*pData & (0x8000 >> Diff))
					LCD_SetPixel(x, y, LCD_COLOR);
				y++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
	        break;
		default:
		    while(xsize){
				if(*pData & (0x8000 >> Diff))
					LCD_SetPixel(x, y, LCD_COLOR);
				else
					LCD_SetPixel(x, y, LCD_BKCOLOR);
				x++;
				xsize--;
				if(++Diff == 16){
					Diff = 0;
					pData++;
				}
			}
			break;
	}	
}
#endif

void LCDDEV_DrawBitLine16BPP(int x0, int y0, int xsize, int ysize, unsigned int *pData) 
{
	int x1,y1;
	unsigned int i;
	
	x1 = x0+xsize-1;
	y1 = y0+ysize-1;
	LCDDEV_SetWindow(x0, y0, x1, y1);
	for(; ysize>0; ysize--){
		i = xsize;
		for(; i>0; i--){
		    LCD_Data(*pData++);
		}
	}
		
	LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
}


void LCDDEV_DrawBitmap(int x0,    		int y0,
                       int xsize, 		int ysize,
                       int BitsPerPixel,	int BytesPerLine,
                       unsigned int *pData, 		int Diff)
{
  	int i;
  	
   	switch (BitsPerPixel) {
   		case 1:
   		    #ifdef LCD_MEMORY_DEV
   		        LCDDEV_DrawChar2Mem(x0, y0, xsize, ysize, Diff, BytesPerLine, pData);

   		    #else
       		    if(LCD_Context.DrawMode == LCD_DRAWMODE_CIR90){
                    for(i=0; i<ysize; i++){
       	                LCDDEV_DrawBitLine1BPP(x0+i, y0, xsize, Diff, pData);
       	                pData += BytesPerLine;
       	                }
       		    }else{
       			    for(i=0; i<ysize; i++){ 
       				    LCDDEV_DrawBitLine1BPP(x0, i+y0, xsize, Diff, pData);
       				    pData += BytesPerLine;
       			    }
       			}
       	    #endif
   			break;
   		case 16:
//       		LCDDEV_SetWindow(x0, y0, x0+xsize-1, y0+ysize-1);
       		#ifdef LCD_MEMORY_DEV
       		    LCDDEV_DrawMem(x0, y0, xsize, ysize, BitsPerPixel, BytesPerLine, pData);
       		#else
           		for(; ysize > 0; ysize--){
           		    LCDDEV_DrawBitLine16BPP(x0, y0++, xsize, 1, pData);
           		    pData += BytesPerLine;
           		} 
            #endif
//       		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
       	break;
   	}
}



#ifdef LCD_MEMORY_DEV
void LCDDEV_ReadMem(int x0, int y0, int xsize, int ysize, unsigned int *pData)
{
    unsigned long scr;
    unsigned long dsr;
    
    DMA_CHANNEL_REG *DmaChl;  
    unsigned int imask_tmp;
    while((DMA_CHANNEL_REG *)DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1)));  
    
    dsr = (unsigned long)((unsigned int)(pData));
    scr = (unsigned long)(y0)*LCD_Context.LcdMaxXsize + (unsigned long)x0 + (unsigned long)(0x3C0000);
//---------------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//---------------------------------------------------------------------    
    DmaChl->DmaSar = (unsigned long)((((unsigned long)(scr))<<1)+(unsigned long)0x4000000);
    DmaChl->DmaDar = ((unsigned long)(dsr)<<1)+(unsigned long)0x40000;

    if(xsize < LCD_Context.LcdMaxXsize){
        DmaChl->DmaCtl.Lsb = 0x00020013;
    }else{    
        DmaChl->DmaCtl.Lsb = 0x00000013;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    }
    DmaChl->DmaCtl.Msb = (unsigned long)(xsize)*ysize;             //224*16/2
    
    DmaChl->DmaCfg.Lsb = 0x00000020;             //channel priority set
    DmaChl->DmaCfg.Msb = 0x00001004;
    
    if(xsize < LCD_Context.LcdMaxXsize){
        DmaChl->DmaSgr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_Context.LcdMaxXsize)-xsize));
    }else{
        DmaChl->DmaSgr = 0x00000000;
    }
    DmaChl->DmaDsr = 0x00000000;           //(176-16)/2
    DmaChl->Priority   = 0;
    Dma_ReadLcdFlag  = 1;
    DmaChl->pResFlag = &Dma_ReadLcdFlag;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
    while(Dma_ReadLcdFlag);
}


void LCDDEV_DrawChar2Mem(int x0, 
                            int y0, 
                            int xsize, 
                            int ysize,
                            int Diff, 
                            int BytesPerLine,
                            unsigned int *pData)
{
    unsigned int LcdDataBuf[CH_CHAR_XSIZE*CH_CHAR_YSIZE];
    unsigned int *pLcdData;
    unsigned int i;
    
    x0 += Diff;
    
    LCDDEV_ReadMem(x0, y0, xsize, ysize, LcdDataBuf);
    pLcdData = LcdDataBuf;
//    if(LCD_Context.DrawMode == LCD_DRAWMODE_CIR90){
//        for(i=0; i<ysize; i++){
//            LCDDEV_DrawBitLine1BPP(0, y0, xsize, Diff, pLcdData, pData);
//            pData    += BytesPerLine;
//            pLcdData += xsize;
//        }
//    }else{
        for(i=0; i<ysize; i++){ 
            LCDDEV_DrawBitLine1BPP(0, i+y0, xsize, Diff, pLcdData, pData);
            pData += BytesPerLine;
            pLcdData += xsize;
        }
//    }
    LCDDEV_DrawMem(x0, y0, xsize, ysize, 16, xsize, LcdDataBuf);
}


void LCDDEV_DrawMem(int x0,    		int y0,
                    int xsize, 		int ysize,
                    int BitsPerPixel,	int BytesPerLine,
                    unsigned int *pData)
{
    unsigned long dst_addr;
    unsigned long sar_addr;
    unsigned int  scatter_side;

    DMA_CHANNEL_REG *DmaChl;
    
    unsigned int imask_tmp;
    
    while(DmaFillImage2LcdFlag);

    while((DMA_CHANNEL_REG *)DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 

    dst_addr = (unsigned long)(y0)*LCD_Context.LcdMaxXsize + (unsigned long)x0  + (unsigned long)(0x3C0000);
    sar_addr = (unsigned long)((unsigned int)(pData));

//---------------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//--------------------------------------------------------------------- 
    
    DmaChl->DmaSar = (unsigned long)((((unsigned long)(sar_addr))<<1)+(unsigned long)0x40000);
    DmaChl->DmaDar = ((unsigned long)(dst_addr)<<1)+(unsigned long)0x4000000;
    
    scatter_side  = BytesPerLine - xsize;   // BytesPerLine Must Biger than xsize
    
    if(scatter_side != 0){     
        DmaChl->DmaCtl.Lsb = 0x00060013;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    }else if(xsize < LCD_Context.LcdMaxXsize){
        DmaChl->DmaCtl.Lsb = 0x00040013;
    }else{
        DmaChl->DmaCtl.Lsb = 0x00000013;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    }
    DmaChl->DmaCtl.Msb = (unsigned long)(xsize)*ysize;             //224*16/2
    
    DmaChl->DmaCfg.Lsb = 0x00000020;             //channel priority set
    DmaChl->DmaCfg.Msb = 0x00001004;
    
    if(scatter_side != 0){
        DmaChl->DmaSgr = (unsigned long)(((unsigned long)(xsize)<<20) | (unsigned long)(scatter_side));
        DmaChl->DmaDsr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_Context.LcdMaxXsize)-xsize));
    }else if(xsize < LCD_Context.LcdMaxXsize){
        DmaChl->DmaSgr = 0x00000000;
        DmaChl->DmaDsr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_Context.LcdMaxXsize)-xsize));
    }else{
        DmaChl->DmaSgr = 0x00000000;
        DmaChl->DmaDsr = 0x00000000;           //(176-16)/2
    }
    DmaChl->Priority   = 0;
    Dma_DispFlag     = 1;
    DmaChl->pResFlag = &Dma_DispFlag;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
    while(Dma_DispFlag);
    DmaTanData2LcdCont++;
}
#if 1

void DMA_DispPicSdram2Lcd(int x0, int y0, int xsize, int ysize)
{
    unsigned long sar_addr;

    DMA_CHANNEL_REG *DmaChl;
    
    unsigned int imask_tmp;
    
    while(DmaFillImage2LcdFlag);

    while((DMA_CHANNEL_REG *)DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 

    sar_addr = (unsigned long)(y0)*LCD_Context.LcdMaxXsize + (unsigned long)x0  + (unsigned long)(0x3C0000);

//---------------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//--------------------------------------------------------------------- 
    
    DmaChl->DmaSar = ((unsigned long)(sar_addr)<<1)+(unsigned long)0x4000000;
    DmaChl->DmaDar = (((unsigned long)(LCD_DATA))<<1);
    
    if(xsize < LCD_Context.LcdMaxXsize){
        DmaChl->DmaCtl.Lsb = 0x00020113;
    }else{
        DmaChl->DmaCtl.Lsb = 0x00000113;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    }
    DmaChl->DmaCtl.Msb = (unsigned long)(xsize)*ysize;             //224*16/2
    
    DmaChl->DmaCfg.Lsb = 0x00000000;             //channel priority set
    DmaChl->DmaCfg.Msb = 0x00001004;
    
    if(xsize < LCD_Context.LcdMaxXsize){
        DmaChl->DmaDsr = 0x00000000;
        DmaChl->DmaSgr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_Context.LcdMaxXsize)-xsize));
    }else{
        DmaChl->DmaSgr = 0x00000000;
        DmaChl->DmaDsr = 0x00000000;           //(176-16)/2
    }
    DmaChl->Priority     = 0;
    DmaFillImage2LcdFlag = 1;
    DmaChl->pResFlag     = &DmaFillImage2LcdFlag;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
    while(DmaFillImage2LcdFlag);
}
void DMA_DisplayPicture(int x, int y, int xsize, int ysize)
{
    unsigned int    x1, y1;
    unsigned long   DispPicLen;
    unsigned int    yseg;
	unsigned int 	ysize_Spare;
	unsigned int    DispCont;
	
	if(xsize==0 || ysize==0)
	    return;
	
	x1 = x+xsize-1;
	y1 = y+ysize-1;
	DispPicLen = xsize*ysize;
	
	LCDDEV_SetWindow(x, y, x1, y1);
    DispCont    = 4095/xsize;	
    yseg        = ysize/DispCont;
    ysize_Spare = ysize%DispCont;

    for(; yseg>0; yseg--){
        DMA_DispPicSdram2Lcd(x, y, xsize, DispCont);
        y += DispCont;
    }
    
    if(ysize_Spare > 0){
        DMA_DispPicSdram2Lcd(x, y, xsize, ysize_Spare);
    }
    
    LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
    DmaTanData2LcdCont = 0;
}
#endif


void DmaFillImage2Lcd(void)
{
    DMA_CHANNEL_REG *DmaChl;
    unsigned long TanDataLen;
    unsigned int imask_tmp;
    unsigned long srcRGB;
    unsigned int i;

    if((DmaTanData2LcdCont) == 0/* || (Fun1MsgArray[0] == MSG_FUNCTION1_USB_TRANS)*/) return;
        
//    if(Sys1InitFlag.bc.bMusicModuleInitEnable == 0)
//    {
//        if(DmaStartFillLcdFlag == 0){
//            return;
//        }
//        DmaStartFillLcdFlag = 0;
//    }    
    if(SystemInfo.bc.bBackLightStatus == 2){
        DmaTanData2LcdCont = 0;
        return;
    }    
        
    DmaTanData2LcdCont = 0;
    
    while(DmaFillImage2LcdFlag);
    
    LCD_SetDispAddr(0, 0);
    
    for(i=10; i>0; i--){    
    //------------------------------------------------------------------------------
        while((DMA_CHANNEL_REG *)DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1)));
    //-----------------------------------------------------------------
        srcRGB = (unsigned long)(320)*24*(i-1) + (unsigned long)(0x3C0000);
        srcRGB = ((unsigned long)(srcRGB)<<1)+(unsigned long)0x4000000;
        
//---------------------------------------------------------------------
        DmaChl->pDmaFnct    = DMA_FNCT_NULL;
        DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//---------------------------------------------------------------------     
        DmaChl->DmaSar = srcRGB; 
        DmaChl->DmaDar = (((unsigned long)(LCD_DATA))<<1);
        
        DmaChl->DmaCtl.Lsb = 0x00000123;             //SRC_GATHER_EN:0 DINC:No change, SRC_TR_WIDTH:16-bit, DST_TR_WIDTH:16-bit, INT_EN:1
    
        DmaChl->DmaCtl.Msb = (unsigned long)(320)*12;////TanDataLen;             //(unsigned long)176*22;             
    
        DmaChl->DmaCfg.Lsb = 0x00000000;
        DmaChl->DmaCfg.Msb = 0x00001004;

        DmaChl->DmaSgr = 0x00000000  ;//2|(110<<20);       <jh@2006-3-2 9:12>
        DmaChl->Priority     = 0;
        DmaFillImage2LcdFlag = 1;
        DmaChl->pResFlag    = &DmaFillImage2LcdFlag;
    }
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
    while(DmaFillImage2LcdFlag);
}
#endif
