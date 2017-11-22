//******************************************************************************
#define     _IN_LCD_LTPS
//------------------------------------------------------------------------------
#include "Memap.h"
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Lcd.h"
#include "LcdChar.h"
#include "Dma.h"
//---------driver program HX8309---TFT2.0-------------------------------------------------------------------
#undef   Parallel8
#define  UINT16     unsigned short
#define LCD_INIT_NORMAL 0
#define MP4_LCD_INIT    2


void LCD_Init(unsigned int type) ;
void LCD_SetDispAddr(unsigned int x,unsigned int y);
void LCDDEV_SetWindow(int x0, int y0, int x1, int y1);
void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1);
void Display_off(void) ;

unsigned int LcdScanType=0;

void LCD_Command(unsigned int cmd)
{
    write_mem(LCD_COMMAND,cmd);
}

void LCD_Data(unsigned int data)
{
    write_mem(LCD_DATA,data);
}

void LCD_Reg_Set(unsigned int cmd,unsigned int data)
{
    write_mem(LCD_COMMAND,cmd); 
    write_mem(LCD_DATA,data);
}       

unsigned int LCD_StatusRead(void)
{
//    unsigned int i,j;
//    i = read_XDATA(LCD_COMMAND);
//#ifdef  Parallel8
//    j = read_XDATA(LCD_COMMAND);
//    i = ((i & 0xff00) | (j >> 8));
//#endif
//    return i;
}



void LCD_SetDispAddr(unsigned int x,unsigned int y)
{
    LCD_Command(0x4200 | y);    //SW XY 320x240
    LCD_Command(0x4300 | (x>>8));
    LCD_Command(0x4400 | (x&0x00FF));
}

void LCD_SetPixel(unsigned int x, unsigned int y, unsigned int color)
{
    LCD_SetDispAddr(x,y);	
	LCD_Data(color);	        //color表示RGB值, 这个函数的功能是往(x,y)写入color值
}


unsigned int LCD_GetPixel(unsigned int x, unsigned int y)
{
//	unsigned int color;
//	unsigned int i;
//
//	LCD_SetDispAddr(x,y);		
//	color = read_XDATA(LCD_DATA);
//	//for(i=10; i>0; i--);
//	color = read_XDATA(LCD_DATA);
//	//color = DataChange(color);
//	return(color);
}


void LCD_XorPixel(unsigned int x, unsigned int y)
{
//	unsigned int color;
//	color = LCD_GetPixel(x,y);
//	LCD_SetPixel(x,y,0xFFFF-color);	
}

void LCD_Init(unsigned int type)
{
    LcdScanType = 0;
    LCD_Command(0x0000);
    LCD_Command(0x0504);
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);  
}

void  PowerOnLcdinit()
{
    unsigned int x,y;
    LcdScanType = 0;
    LCD_Command(0x0110);
    LCD_Command(0x0000);
    LCD_Command(0x0301);
	Delay1Ms(20);
	
    LCD_Command(0x0000);
    LCD_Command(0x0110);
    LCD_Command(0x0300);
    LCD_Command(0x0504);
    LCD_Command(0x2B04);
    LCD_Command(0x5901);
    LCD_Command(0x6022);
    LCD_Command(0x5900);
    LCD_Command(0x2830);
    LCD_Command(0x1A05);    //R26
    LCD_Command(0x2507);    //R37
    LCD_Command(0x1900);    //R25
    LCD_Command(0x1C73);    //R28
    LCD_Command(0x2474);    //R36
    LCD_Command(0x1E01);    //R30
    LCD_Command(0x18C1);    //R24
    Delay1Ms(20);
    
    LCD_Command(0x18E1);
    LCD_Command(0x18F1);
    Delay1Ms(50);
    
    LCD_Command(0x18F5);
    Delay1Ms(50);
    
    LCD_Command(0x1B09);
    Delay1Ms(50);
    
    LCD_Command(0x1F0E);
    LCD_Command(0x200B);
    LCD_Command(0x1E81);
    Delay1Ms(20);
    
    LCD_Command(0x9D00);
    LCD_Command(0xC000);
    LCD_Command(0xC101);    //bit 0: BGR
    LCD_Command(0x0E00);
    LCD_Command(0x0F00);
    LCD_Command(0x1000);
    LCD_Command(0x1100);
    LCD_Command(0x1200);
    LCD_Command(0x1300);
    LCD_Command(0x1400);
    LCD_Command(0x1500);
    LCD_Command(0x1600);
    LCD_Command(0x1700);
    
    LCD_Command(0x3401);
    LCD_Command(0x3500);
    LCD_Command(0x4B00);
    LCD_Command(0x4C00);
    LCD_Command(0x4E00);
    LCD_Command(0x4F00);
    LCD_Command(0x5000);
    LCD_Command(0x3C00);
    LCD_Command(0x3D00);
    LCD_Command(0x3E01);
    LCD_Command(0x3F3F);
    LCD_Command(0x4002);
    LCD_Command(0x4102);
    LCD_Command(0x4200);
    LCD_Command(0x4300);
    LCD_Command(0x4400);
    LCD_Command(0x4500);
    LCD_Command(0x4600 | (LCD_MAX_YSIZE-1));
    LCD_Command(0x4700);
    LCD_Command(0x4800);
    LCD_Command(0x4900 | ((LCD_MAX_XSIZE-1)>>8));
    LCD_Command(0x4A00 | ((LCD_MAX_XSIZE-1)&0x00FF));
    LCD_Command(0x1D08);
    LCD_Command(0x8600);
    LCD_Command(0x8730);
    LCD_Command(0x8802);
    LCD_Command(0x8905);
    LCD_Command(0x8D01);
    LCD_Command(0x8B30);
    LCD_Command(0x3301);
    LCD_Command(0x3701);
    LCD_Command(0x7600);
    LCD_Command(0x8F10);
    LCD_Command(0x9067);
    LCD_Command(0x9107);
    LCD_Command(0x9256);
    LCD_Command(0x9307);
    LCD_Command(0x9401);
    LCD_Command(0x9576);
    LCD_Command(0x9665);
    LCD_Command(0x9700);
    LCD_Command(0x9800);
    LCD_Command(0x9900);
    LCD_Command(0x9A00);
    LCD_Command(0x3B01);
    Delay1Ms(10);
#ifdef LCD_MEMORY_DEV
    LCD_Command(0x0020);
    
    for(x=0; x<240; x++){
        for(y=0; y<320; y++){
            LCD_Data(0);
        }
    } 
#endif 
    LCD_Init(0);
}

void Standby_lcd(void)
{
/*  Display Off Sequence    */    
    LCD_Command(0x0000);
    Delay1Ms(50);
    LCD_Command(0x3B00);
    
/*  Power Off Sequence    */  
    LCD_Command(0x1E01);
    LCD_Command(0x1B08);
    LCD_Command(0x18C0);
    Delay1Ms(10);
    LCD_Command(0x1800);
    LCD_Command(0x1C30);  
    
    LCD_Command(0x0111);
    LCD_Command(0x0001);
}


void  standby_return()
{
    unsigned int LcdScanType_tmp;
    
    LcdScanType_tmp = LcdScanType;
    
    PowerOnLcdinit();
    LcdScanType = LcdScanType_tmp;
    
    LCDDEV_SetWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
}

//wjm 2.0 HX8309
void Display_off(void)
{
//	LCD_Reg_Set(0x000B,0x0000); 
//	LCD_Reg_Set(0x0010,0x6020);  
//	LCD_Reg_Set(0x0007,0x0036);
//	Delay1Ms(6);
//	LCD_Reg_Set(0x0007,0x0026);
//	Delay1Ms(6);
//	LCD_Reg_Set(0x0007,0x0004);
//	Delay1Ms(9);
//	LCD_Reg_Set(0x0007,0x0037); 
//	Delay1Ms(10);
//	LCD_Reg_Set(0x0011,0x0000);
//	LCD_Reg_Set(0x0010,0x0000);
//	LCD_Reg_Set(0x0012,0x0000);
//	LCD_Reg_Set(0x0013,0x0000);
}


void LCDDEV_SetWindow(int x0, int y0, int x1, int y1)
{
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif
    if((LcdScanType == 0) || (LcdScanType == 2)){
        LCD_Command(0x0514);
    }else{
        LCD_Command(0x0510);
    }
    LCD_Command(0x4500 | y0);
    LCD_Command(0x4600 | y1);
    
    LCD_Command(0x4700 | (x0>>8));
    LCD_Command(0x4800 | (x0&0x00FF));
    LCD_Command(0x4900 | (x1>>8));
    LCD_Command(0x4A00 | (x1&0x00FF));
    LCD_SetDispAddr(x0, y0);  
    
}


void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif    
   if((LcdScanType == 0) || (LcdScanType == 2)){
        LCD_Command(0x0504);
    }else{
        LCD_Command(0x0500);
    }
    LCD_Command(0x4500 | y0);
    LCD_Command(0x4600 | y1);
    
    LCD_Command(0x4700 | (x0>>8));
    LCD_Command(0x4800 | (x0&0x00FF));
    LCD_Command(0x4900 | (x1>>8));
    LCD_Command(0x4A00 | (x1&0x00FF));  
}


//void LCD_ReadBitmap(int x0, int y0, int xsize, int ysize, unsigned int *pData)
//{
//	int x1, y1;
//	x1 = x0+xsize-1; 
//	y1 = y0+ysize-1; 
//	LCDDEV_SetWindow(x0, y0, x1, y1);
//	read_XDATA(LCD_DATA);
//	for(y1=0; y1<ysize; y1++){
//		for(x1=0; x1<xsize; x1++){
//			*pData++ = read_XDATA(LCD_DATA);
//		}
//	}
//
//	LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
//}


unsigned int LCDDEV_GetPixel(int x, int y)
{
//	unsigned int color;
//	
//	LCD_SetDispAddr(x,y);	
//	color = read_XDATA(LCD_DATA);
//	color = read_XDATA(LCD_DATA);
//	return(color);	
}


void LCDDEV_SetPixel(int x, int y, unsigned int color)
{
    LCD_SetDispAddr(x,y);	
	LCD_Data(color);	        //color表示RGB值, 这个函数的功能是往(x,y)写入color值
}


void LCDDEV_XorPixel(int x, int y)
{
//	unsigned int color;
//	color = LCDDEV_GetPixel(x,y);
//	LCDDEV_SetPixel(x,y,0xFFFF-color);			
}

#if 0
void LCDDEV_DrawHLine(int x0, int y, int x1)
{
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
}

void LCDDEV_DrawVLine(int x, int y0, int y1)
{
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
}

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

#ifdef LCD_MEMORY_DEV
void LCDDEV_DrawBitLine1BPP(int x,                  //坐标 x
                            int y,                  //坐标 y
                            int xsize,              //横向上的 大小
                            int Diff,               //偏移
                            unsigned int *pLcdData,
                            unsigned int *pData)    //点阵字库字据
{
//    unsigned int DataForLcd[CH_CHAR_XSIZE];
//    unsigned int *pLcdData;
//    unsigned int x_bak;
//  	x += Diff;
//
//    x_bak = x;
//    
////    LCDDEV_ReadMem(x, y, xsize, 1, DataForLcd);
//    x = 0;
//    pLcdData = DataForLcd;
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
	    case LCD_DRAWMODE_CIR90:
	        while(xsize){
				if(*pData & (0x8000 >> Diff)){
				    *(pLcdData+x) = LCD_COLOR;
				}
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
//	LCDDEV_DrawMem(x_bak, y, xsize, 1, 16, xsize, DataForLcd);
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
			#ifdef  Parallel8
			write_mem(LCD_DATA,*pData>>8);
    	    #endif
    	    write_mem(LCD_DATA,*pData++);
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

#endif

//length
void DMA_LcdJpegInitX(void)
{
    LcdScanType = 2;
    LCD_Command(0x0000);
    LCD_Command(0x0504);
//    LCD_Command(0x1D08);
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);  		
}


//weight
void DMA_LcdJpegInit(void)
{
    LcdScanType = 1;
    LCD_Command(0x0020);
    LCD_Command(0x0500);
//    LCD_Command(0x1D00);
    
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 	
}


void MP4_LCD_Init(void)
{
    unsigned int x, y;
//    LCD_SetDispAddr(0, 0);
//    for(x=0; x<LCD_MAX_XSIZE; x++){
//        for(y=0; y<LCD_MAX_YSIZE; y++){
//            LCD_Data(0xF800);
//        }
//    }
//    Delay1Ms(500);
    LcdScanType = 3;
    LCD_Command(0x0000);
    LCD_Command(0x0500);
//    LCD_Command(0x1D00);
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);   
}


#if 0 

#ifdef LCD_MEMORY_DEV
void LCDDEV_ReadMem(int x0, int y0, int xsize, int ysize, unsigned int *pData)
{
    unsigned long scr;
    unsigned long dsr;
    
    DMA_CHANNEL_REG *DmaChl;  
    unsigned int imask_tmp;
//------------------------------------------------------------------------------    
    read_creg(%imask, imask_tmp);
    bitset_creg(%imask, 15);
    bitset_creg(%imask, 7);
//------------------------------------------------------------------------------
    
//    while(DMA_CHANNEL_FULL == GetDmaChannelState(1)); 
    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1)));  
    write_creg(%imask, imask_tmp);
    
    dsr = (unsigned long)((unsigned int)(pData));
    scr = (unsigned long)(y0)*LCD_MAX_XSIZE + (unsigned long)x0;
//---------------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//---------------------------------------------------------------------    
    DmaChl->DmaSar = (unsigned long)((((unsigned long)(scr))<<1)+(unsigned long)0x4000000);
    DmaChl->DmaDar = ((unsigned long)(dsr)<<1)+(unsigned long)0x40000;

    if(xsize < LCD_MAX_XSIZE){
        DmaChl->DmaCtl.Lsb = 0x00020013;
    }else{    
        DmaChl->DmaCtl.Lsb = 0x00000013;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    }
    DmaChl->DmaCtl.Msb = (unsigned long)(xsize)*ysize;             //224*16/2
    
    DmaChl->DmaCfg.Lsb = 0x00000020;             //channel priority set
    DmaChl->DmaCfg.Msb = 0x00001004;
    
    if(xsize < LCD_MAX_XSIZE){
        DmaChl->DmaSgr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_MAX_XSIZE)-xsize));
    }else{
        DmaChl->DmaSgr = 0x00000000;
    }
    DmaChl->DmaDsr = 0x00000000;           //(176-16)/2
    Dma_ReadLcdFlag  = 1;
    DmaChl->pResFlag = &Dma_ReadLcdFlag;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
//------------------------------------------------------------------------------    
    read_creg(%imask, imask_tmp);
    bitset_creg(%imask, 15);
    bitset_creg(%imask, 7);
//------------------------------------------------------------------------------
   
//    DmaBufIn(&DmaChl, 1);
    while(Dma_ReadLcdFlag);
//------------------------------------------------------------------------------
    write_creg(%imask, imask_tmp);
//------------------------------------------------------------------------------
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
    if(LCD_Context.DrawMode == LCD_DRAWMODE_CIR90){
        for(i=0; i<ysize; i++){
            LCDDEV_DrawBitLine1BPP(0, y0, xsize, Diff, pLcdData, pData);
            pData    += BytesPerLine;
            pLcdData += xsize;
        }
    }else{
        for(i=0; i<ysize; i++){ 
            LCDDEV_DrawBitLine1BPP(0, i+y0, xsize, Diff, pLcdData, pData);
            pData += BytesPerLine;
            pLcdData += xsize;
        }
    }
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
    
//------------------------------------------------------------------------------    
//    read_creg(%imask, imask_tmp);
//    bitset_creg(%imask, 15);
//    bitset_creg(%imask, 7);
//------------------------------------------------------------------------------    
    
    while(DmaFillImage2LcdFlag);

    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 

//    write_creg(%imask, imask_tmp);
    
    dst_addr = (unsigned long)(y0)*LCD_MAX_XSIZE + (unsigned long)x0;
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
    }else if(xsize < LCD_MAX_XSIZE){
        DmaChl->DmaCtl.Lsb = 0x00040013;
    }else{
        DmaChl->DmaCtl.Lsb = 0x00000013;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    }
    DmaChl->DmaCtl.Msb = (unsigned long)(xsize)*ysize;             //224*16/2
    
    DmaChl->DmaCfg.Lsb = 0x00000020;             //channel priority set
    DmaChl->DmaCfg.Msb = 0x00001004;
    
    if(scatter_side != 0){
        DmaChl->DmaSgr = (unsigned long)(((unsigned long)(xsize)<<20) | (unsigned long)(scatter_side));
        DmaChl->DmaDsr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_MAX_XSIZE)-xsize));
    }else if(xsize < LCD_MAX_XSIZE){
        DmaChl->DmaSgr = 0x00000000;
        DmaChl->DmaDsr = (unsigned long)(((unsigned long)(xsize)<<20) | ((unsigned long)(LCD_MAX_XSIZE)-xsize));
    }else{
        DmaChl->DmaSgr = 0x00000000;
        DmaChl->DmaDsr = 0x00000000;           //(176-16)/2
    }
    Dma_DispFlag     = 1;
    DmaChl->pResFlag = &Dma_DispFlag;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
//------------------------------------------------------------------------------    
//    read_creg(%imask, imask_tmp);
//    bitset_creg(%imask, 15);
//    bitset_creg(%imask, 7);
////------------------------------------------------------------------------------    
    while(Dma_DispFlag);
////------------------------------------------------------------------------------
//    write_creg(%imask, imask_tmp);
//------------------------------------------------------------------------------
    DmaTanData2LcdCont++;
}

void DMA_Sdram2LcdFnctHook(void *parg)
{
    unsigned long srcRGB;
    DMA_CHANNEL_REG *DmaChl;
    unsigned int  LcdSecTanCtr;
    unsigned long TanDataLen;
    unsigned int imask_tmp;
    
    TanDataLen = 320*12;
    
    LcdSecTanCtr = (unsigned int)(parg);

    if(LcdSecTanCtr > 0){
        while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1)));
        srcRGB = (unsigned long)(20-(LcdSecTanCtr))*320*12;
        srcRGB = ((unsigned long)(srcRGB)<<1)+(unsigned long)0x4000000;
        
//---------------------------------------------------------------------
        DmaChl->pDmaFnct    = DMA_Sdram2LcdFnctHook;
        DmaChl->pDmaFnctArg = LcdSecTanCtr-1;
//---------------------------------------------------------------------     
        DmaChl->DmaSar = srcRGB; 
        DmaChl->DmaDar = (((unsigned long)(LCD_DATA))<<1);
        
        DmaChl->DmaCtl.Lsb = 0x00000123;             //SRC_GATHER_EN:0 DINC:No change, SRC_TR_WIDTH:16-bit, DST_TR_WIDTH:16-bit, INT_EN:1
    
        DmaChl->DmaCtl.Msb = (unsigned long)(320)*6;////TanDataLen;             //(unsigned long)176*22;             
    
        DmaChl->DmaCfg.Lsb = 0x00000000;
        DmaChl->DmaCfg.Msb = 0x00001004;

        DmaChl->DmaSgr = 0x00000000  ;//2|(110<<20);       <jh@2006-3-2 9:12>
        DmaFillImage2LcdFlag = 1;
        DmaChl->pResFlag   = &DmaFillImage2LcdFlag; 
        DMA_DISABLE_INTERRUPT;
    	    DmaChannelSetup(1);
        DMA_ENABLE_INTERRUPT;
//        DmaBufIn(&DmaChl, 1);       
    }else{
        DmaChl->pDmaFnct    = DMA_FNCT_NULL;
        DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
    }
}

void DmaFillImage2Lcd(void)
{
    DMA_CHANNEL_REG *DmaChl;
    unsigned long TanDataLen;
    unsigned int imask_tmp;
//------------------------------------------------------------------------------    
    read_creg(%imask, imask_tmp);
    bitset_creg(%imask, 15);
    bitset_creg(%imask, 7);
//------------------------------------------------------------------------------
    if((DmaTanData2LcdCont) == 0/* || (Fun1MsgArray[0] == MSG_FUNCTION1_USB_TRANS)*/) return;
        
    DmaTanData2LcdCont = 0;
    
    while(DmaFillImage2LcdFlag);
    write_creg(%imask, imask_tmp);
//------------------------------------------------------------------------------
    TanDataLen = 320*12;
  
//    while(DMA_CHANNEL_FULL == GetDmaChannelState(1)); 
    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1)));

//-----------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_Sdram2LcdFnctHook;
    DmaChl->pDmaFnctArg = 19;            //(220/22)-1;
//-----------------------------------------------------------------     
    DmaChl->DmaSar = (unsigned long)0x4000000; 
    DmaChl->DmaDar = (((unsigned long)(LCD_DATA))<<1);
    
    DmaChl->DmaCtl.Lsb = 0x00000123;             //SRC_GATHER_EN:0 DINC:No change, SRC_TR_WIDTH:16-bit, DST_TR_WIDTH:16-bit, INT_EN:1
    DmaChl->DmaCtl.Msb = (unsigned long)(320)*6;             //(unsigned long)176*22;            
    
    DmaChl->DmaCfg.Lsb = 0x00000000;
    DmaChl->DmaCfg.Msb = 0x00001004;
//    DmaChl.DmaSgr.Lsb = 0x00000000  ;           //2|(110<<20);       <jh@2006-3-2 9:12>
    DmaFillImage2LcdFlag = 1;
    DmaChl->pResFlag   = &DmaFillImage2LcdFlag;
    
//    DmaBufIn(&DmaChl, 1);   
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
//------------------------------------------------------------------------------
//    write_creg(%imask, imask_tmp);
//------------------------------------------------------------------------------    
}
#endif

#endif
          
//******************************************************************************


        
