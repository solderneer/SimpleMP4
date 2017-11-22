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
    LCD_Reg_Set(0x004E, y);    //RAM Addr x
	LCD_Reg_Set(0x004F, x);    //RAM Addr y
	LCD_Command(0x0022);
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
    LCD_Reg_Set(0x0001, 0x6B3F);    //F
	LCD_Reg_Set(0x0011, 0x6038);    //F
//    LCD_SetDispAddr(0, 0); 
    LCDDEV_SetWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
}

void  PowerOnLcdinit()
{
    unsigned int x,y;

//---------------------------------------------
//Power On Lcd Reset
//    LCD_Reg_Set(0x0007, 0x0000);
    
    LCD_Reg_Set(0x0000, 0x0000);
    LCD_Reg_Set(0x0003, 0xaee0);
	LCD_Reg_Set(0x000C, 0x0000);
 	LCD_Reg_Set(0x000D, 0x0000);
//	LCD_Reg_Set(0x000E, 0x0000);
	LCD_Reg_Set(0x001E, 0xa000);
    Delay1Ms(50);
    
    LCD_Reg_Set(0x0007,0x0021);
    LCD_Reg_Set(0x0000,0x0001);
    LCD_Reg_Set(0x0007,0x0023);
    LCD_Reg_Set(0x0010,0x0000);
    Delay1Ms(30);
    LCD_Reg_Set(0x0007,0x0033);
    LCD_Reg_Set(0x0011,0x6038);    //
    LCD_Reg_Set(0x0002,0x0600);
//---------------------------------------------

    LCD_Reg_Set(0x0003, 0xA6AA);
	LCD_Reg_Set(0x000C, 0x0004);
 	LCD_Reg_Set(0x000D, 0x000C);
	LCD_Reg_Set(0x000E, 0x3100);    //3e
//	LCD_Reg_Set(0x001E, 0x00b6);    //b6
	
	Delay1Ms(5);
	LCD_Reg_Set(0x0001, 0x6B3F);
	
	Delay1Ms(5);
	LCD_Reg_Set(0x0005, 0x0000);
	LCD_Reg_Set(0x0006, 0x0000);
	LCD_Reg_Set(0x0016, 0xEF1C);
	LCD_Reg_Set(0x0017, 0x0003);
	
	LCD_Reg_Set(0x000B, 0x5138);
	LCD_Reg_Set(0x000F, 0x0000);
	LCD_Reg_Set(0x0041, 0x0000);
	LCD_Reg_Set(0x0042, 0x0000);
	LCD_Reg_Set(0x0048, 0x0000);
	LCD_Reg_Set(0x0049, 0x013F);
	LCD_Reg_Set(0x004A, 0x0000);
	LCD_Reg_Set(0x004B, 0x0000);
	LCD_Reg_Set(0x0044, 0xEF00);    //Windows x
	LCD_Reg_Set(0x0045, 0x0000);    //Windows y Start
	LCD_Reg_Set(0x0046, 0x013F);    //Windows y End
	LCD_Reg_Set(0x004E, 0x0000);    //RAM Addr x
	LCD_Reg_Set(0x004F, 0x0000);    //RAM Addr y
	
	LCD_Reg_Set(0x0023, 0x0000);
	LCD_Reg_Set(0x0024, 0x0000);

#ifdef LCD_MEMORY_DEV
    LCD_Command(0x0022);
    
    for(x=0; x<240; x++){
        for(y=0; y<320; y++){
            LCD_Data(0);
        }
    } 
#endif
}

void Standby_lcd(void)
{
 	LCD_Reg_Set(0x0007, 0x0033);
	Delay1Ms(10);
	LCD_Reg_Set(0x0007, 0x0023);
	Delay1Ms(5);
 	LCD_Reg_Set(0x0007, 0x0021);
    Delay1Ms(10);
	LCD_Reg_Set(0x0007, 0x0001);
 	Delay1Ms(10);
 	
    LCD_Reg_Set(0x0010, 0x0001);
    Delay1Ms(10);
}


void  standby_return()
{
    LCD_Reg_Set(0x0010, 0x0000);
    Delay1Ms(20);
	
	LCD_Reg_Set(0x0007, 0x0001);
 	Delay1Ms(10);
 	LCD_Reg_Set(0x0007, 0x0021);
 	Delay1Ms(5);
 	LCD_Reg_Set(0x0007, 0x0023);
    Delay1Ms(10);
 	LCD_Reg_Set(0x0007, 0x0033);
	Delay1Ms(10); 
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
    LCD_Reg_Set(0x0007,0x0000);
    LCD_Reg_Set(0x0000,0x0000);
    LCD_Reg_Set(0x0010,0x0001);
}


void LCDDEV_SetWindow(int x0, int y0, int x1, int y1)
{
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif    
    LCD_Reg_Set(0x0044, y1<<8 | y0);
    LCD_Reg_Set(0x0045, x0);
    LCD_Reg_Set(0x0046, x1);
    LCD_SetDispAddr(x0, y0);    
}


void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif
    LCD_Reg_Set(0x0044, y1<<8 | y0);
    LCD_Reg_Set(0x0045, x0);
    LCD_Reg_Set(0x0046, x1); 
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





//length
void DMA_LcdJpegInitX(void)
{
    LCD_Reg_Set(0x0001, 0x6B3F);    //F
	LCD_Reg_Set(0x0011, 0x6038);    //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 
}


//weight
void DMA_LcdJpegInit(void)
{
    LCD_Reg_Set(0x0001, 0x2B3F);    //F
	LCD_Reg_Set(0x0011, 0x6030);    //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 	
}


void MP4_LCD_Init(void)
{
    LCD_Reg_Set(0x0001, 0x6B3F);    //F
	LCD_Reg_Set(0x0011, 0x6030);    //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);   
}

void DMA_LcdBMPInitX(void)
{
    LCD_Reg_Set(0x0001, 0x6B3F);    //F
	LCD_Reg_Set(0x0011, 0x6028);    //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 
    LCD_SetDispAddr(0,LCD_MAX_YSIZE-1);
}

void DMA_LcdBMPInitInvX(void)
{
    LCD_Reg_Set(0x0001, 0x6B3F);    //F
	LCD_Reg_Set(0x0011, 0x6038);    //F
   LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 
   LCD_SetDispAddr(0,0);
}

//weight
void DMA_LcdBMPInit(void)
{
    LCD_Reg_Set(0x0001, 0x2B3F);    //F
	LCD_Reg_Set(0x0011, 0x6010);    //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
	LCD_SetDispAddr( LCD_MAX_XSIZE-1,0);
}

//weight
void DMA_LcdBMPInitInv(void)
{
    LCD_Reg_Set(0x0001, 0x2B3F);    //F
	LCD_Reg_Set(0x0011, 0x6030);    //F
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 	
	LCD_SetDispAddr(0,0);
}
          
//******************************************************************************


        
