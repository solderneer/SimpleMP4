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
    LCD_Command(0x1500);
    LCD_Data(y<<8);
    LCD_Data(x&0x0100);
    LCD_Data((x&0x00FF)<<8);
    
    LCD_Command(0x5C00);
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
    LCD_Command(0xbc00);  //Set date
    LCD_Data(0x0A00);       //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 
}

void  PowerOnLcdinit()
{
    unsigned int x,y;
    Delay1Ms(4);
 
   LCD_Command(0x9600);  //VDD On
   Delay1Ms(100);
  
    LCD_Command(0x9900);
 //   Delay1Ms(40);
        
    LCD_Command(0x9200);
//    Delay1Ms(40);
   
 
        LCD_Command(0xca00);   //Set display 
        LCD_Data(0x2300); 
        LCD_Data(0x0000); 
        LCD_Data(0x0100); 
        LCD_Data(0x3f00); 
        LCD_Data(0x0200); 
        LCD_Data(0x0000); 
        LCD_Data(0x0100); 
   //     Delay1Ms(20);
  
 
        LCD_Command(0xa100);   //Set display timing
        LCD_Data(0x0100);
        LCD_Data(0x1a00);
        LCD_Data(0x0100);
        LCD_Data(0x1900);
        LCD_Data(0x2300);
        LCD_Data(0x2f00);
        LCD_Data(0x0000);
       // Delay1Ms(40);
        
        LCD_Command(0xa900);  //Partial display out
   //     Delay1Ms(40);
        
   //     LCD_Command(0x8c00);  //Set AC Operation Drive
   //     LCD_Data(0x0200);
   //     Delay1Ms(20);
        
        LCD_Command(0xab00);  //Set display start Line
        LCD_Data(0x0000);
        LCD_Data(0x0000);
    //    Delay1Ms(20);
        
        LCD_Command(0x3100);  //Set display interface
        LCD_Data(0x0000);
        LCD_Data(0x0000);
    //    Delay1Ms(20);
       
        LCD_Command(0x8b00);  //Set dispaly color mode
        LCD_Data(0x0000);
    //    Delay1Ms(20);
        
        LCD_Command(0x9200);  //Start OSC
        
        LCD_Command(0x6f00);  //Gate line scan set
        LCD_Data(0x0000);
   //     Delay1Ms(20);
        
        LCD_Command(0x8c00);  //Set AC Operation Drive
        LCD_Data(0x0000);
  //     Delay1Ms(20);
        
        LCD_Command(0xbc00);  //Set date
        LCD_Data(0x0A00);
  //      Delay1Ms(20);
        
        LCD_Command(0x2000);   //Set Electronic control
        LCD_Data(0x1c00);
  
        LCD_Data(0x1a00);
  
        LCD_Data(0x0f00); //
 //       
        LCD_Data(0x1f00);
  //      
        LCD_Data(0x0500);
  //      
        LCD_Data(0x1000);
  //      
        LCD_Data(0x0b00);
 //       
        
        LCD_Command(0x2200);  //Set Negative Gamma correction
        LCD_Data(0x7700); 
        LCD_Data(0xc100);
        LCD_Data(0x4500);
        LCD_Data(0x2a00);
        LCD_Data(0x2300);
        LCD_Data(0x0400);
  //      
 
        LCD_Command(0x2500);  //Set Negative Gamma correction
        LCD_Data(0x7700);
        LCD_Data(0xc100);
        LCD_Data(0x4500);
        LCD_Data(0x2a00);
        LCD_Data(0x2300);
        LCD_Data(0x0400);
   //     Delay1Ms(40);
 
        LCD_Command(0x2100);    //Power controller set
        LCD_Data(0x0100);
        LCD_Data(0x2400);
        LCD_Data(0x0F00);
        LCD_Data(0xfe00);
        LCD_Data(0x3300);
        LCD_Data(0x3100);
        LCD_Data(0xFF00);
        LCD_Data(0x0000);
        LCD_Data(0x0000);
        LCD_Data(0x2200);
        LCD_Data(0x3300);
        LCD_Data(0x1100);
        LCD_Data(0x4400);
        LCD_Data(0x0000);
  //      Delay1Ms(40);
 
        LCD_Command(0x2300);    //Set Partial Power control
        LCD_Data(0x3300);
        LCD_Data(0xff00);
        LCD_Data(0x0000);
        LCD_Data(0x0000);
        LCD_Data(0x4400);
        LCD_Data(0x0000);
       
        LCD_Command(0x9400);
  //      Delay1Ms(40);
     
        LCD_Command(0x1500);    //Set Start address
        LCD_Data(0x0000);
        LCD_Data(0x0000);
        LCD_Data(0x0000);
        
        LCD_Command(0x7500);    //Set end address
        LCD_Data(0xef00);
        LCD_Data(0x0100);
        LCD_Data(0x3f00);
        
        LCD_Command(0xaf00);    //Display on
        
#ifdef LCD_MEMORY_DEV
    LCD_Command(0x5C00);
    
    for(x=0; x<240; x++){
        for(y=0; y<320; y++){
            LCD_Data(0);
        }
    } 
#endif
}

void Standby_lcd(void)
{
//	Display_off();
//	LCD_Reg_Set(0x0010,0x6021);
	//close_backlight
}


void  standby_return()
{
//	LCD_Reg_Set(0x0000,0x0001);
//    Delay1Ms(15);
//    LCD_Reg_Set(0x0010,0x0000);
//	
//	LCD_Reg_Set(0x0013,0x1517);
//	LCD_Reg_Set(0x0012,0x0008);
//	LCD_Reg_Set(0x0010,0x4040);
//	Delay1Ms(20);
//	LCD_Reg_Set(0x0010,0x0054);
//	LCD_Reg_Set(0x0012,0x0015);
//	Delay1Ms(20);
//	LCD_Reg_Set(0x0010,0x4460);
//	Delay1Ms(20);
//	//LCD_Reg_Set(0x0013,0x3613);
//	//LCD_Reg_Set(0x0013,0x360d);
////	LCD_Reg_Set(0x0013,0x3006);
//    LCD_Reg_Set(0x0013,0x2C0B);
//	
//	Delay1Ms(30);        
//	LCD_Reg_Set(0x0007,0x0005);
//	Delay1Ms(20);
//	LCD_Reg_Set(0x0007,0x0025);
//	LCD_Reg_Set(0x0007,0x0027);
//	Delay1Ms(20);
//	LCD_Reg_Set(0x0007,0x0037); 
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
    LCD_Command(0x1500);
    LCD_Data(y0<<8);
    LCD_Data(x0&0x0100);
    LCD_Data((x0&0x00FF)<<8);
    
    LCD_Command(0x7500);
    LCD_Data(y1<<8);
    LCD_Data(x1&0x0100);
    LCD_Data((x1&0x00FF)<<8);    
    
    LCD_Command(0x5C00);
//    LCD_SetDispAddr(x0, y0);  
}


void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif
    LCDDEV_SetWindow(x0, y0, x1, y1);
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
    LCD_Command(0xbc00);  //Set date
    LCD_Data(0x0A00);       //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 		
}


//weight
void DMA_LcdJpegInit(void)
{
    LCD_Command(0xbc00);  //Set date
    LCD_Data(0x0000);       //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);	
}


void MP4_LCD_Init(void)
{
    LCD_Command(0xbc00);  //Set date
    LCD_Data(0x0200);       //F
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
    if((DmaTanData2LcdCont) == 0 || (Fun1MsgArray[0] == MSG_FUNCTION1_USB_TRANS)) return;
        
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


        
