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
//---------driver program ILI9320---TFT2.0-------------------------------------------------------------------
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
	//LCD_Reg_Set(0x004E, y);    //RAM Addr x
	//LCD_Reg_Set(0x004F, x);    //RAM Addr y
//	LCD_Reg_Set(0x50, y);		// Horizontal GRAM Start Address-----HSA[7:0]
//	LCD_Reg_Set(0x52, x);		// Vertical GRAM Start Address-----VSA[8:0]

	LCD_Reg_Set(0x20, y);
	LCD_Reg_Set(0x21, x);
	LCD_Command(0x22);
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
    //LCD_Reg_Set(0x0001, 0x6B3F);    //F
    //LCD_Reg_Set(0x0011, 0x6038);    //F
//    LCD_SetDispAddr(0, 0); 

	//************* Start Initial Sequence **********//
	//LCD_Reg_Set(0x00, 0x0001);		// Start internal OSC.
	//LCD_Reg_Set(0x01, 0x0000); //0x0100);		// set SS and SM bit
	//LCD_Reg_Set(0x02, 0x0400);		// set 1 line inversion
	if(type == MP4_LCD_INIT)
		LCD_Reg_Set(0x03, 0x1030); //0x1028); //0x1038);		// set GRAM write direction and BGR=1.
	else
		LCD_Reg_Set(0x03, 0x1038); //0x1028); //0x1038);		// set GRAM write direction and BGR=1.
	LCD_Reg_Set(0x04, 0x0000);		// Resize register
	LCD_Reg_Set(0x08, 0x0202);		// set the back porch and front porch
	LCD_Reg_Set(0x09, 0x0000);		// set non-display area refresh cycle ISC[3:0]
	LCD_Reg_Set(0x0A, 0x0000);		// FMARK function
	LCD_Reg_Set(0x0C, 0x0000);		// RGB interface setting
	LCD_Reg_Set(0x0D, 0x0000);		// Frame marker Position
	LCD_Reg_Set(0x0F, 0x0000);		// RGB interface polarity

	//*************Power On sequence ****************//    WCOM(0x51, 0x00, 0xEF);
	//LCD_Reg_Set(0x10, 0x0000);                // SAP, BT[3:0], AP, DSTB, SLP, STB
	//LCD_Reg_Set(0x11, 0x0007);                // DC1[2:0], DC0[2:0], VC[2:0]
	//LCD_Reg_Set(0x12, 0x0000);                // VREG1OUT voltage
	//LCD_Reg_Set(0x13, 0x0000);                // VDV[4:0] for VCOM amplitude
	//Delay1Ms(50);
	
	LCD_Reg_Set(0x10, 0x17B0);                // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_Reg_Set(0x11, 0x0001);                // DC1[2:0], DC0[2:0], VC[2:0]
	Delay1Ms(50);
	LCD_Reg_Set(0x12, 0x013C); //0x0130);                // VREG1OUT voltage
	Delay1Ms(50);
	LCD_Reg_Set(0x13, 0x1300); //0x1800);                // VDV[4:0] for VCOM amplitude
	LCD_Reg_Set(0x29, 0x0004); //0x0005);                // VCM[4:0] for VCOMH
	Delay1Ms(50);
	LCD_Reg_Set(0x20, 0x0000);                // GRAM horizontal Address                              
	LCD_Reg_Set(0x21, 0x0000);                // GRAM Vertical Address                                
	LCD_Reg_Set(0x2B, 0x0020);		// Frame Rate and Color Control-----16M_EN, Dither, FR_SEL[1:0]                                                                                               

	LCDDEV_SetWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
}

void  PowerOnLcdinit()
{
    //unsigned int x,y;

	Delay1Ms(10);
	//************* Start Initial Sequence **********//
	LCD_Reg_Set(0x00, 0x0001);		// Start internal OSC.
	LCD_Reg_Set(0x01, 0x0000); //0x0100);		// set SS and SM bit
	LCD_Reg_Set(0x02, 0x0400);		// set 1 line inversion
	LCD_Reg_Set(0x03, 0x1038); //0x1028); //0x1038);		// set GRAM write direction and BGR=1.
	LCD_Reg_Set(0x04, 0x0000);		// Resize register
	LCD_Reg_Set(0x08, 0x0202);		// set the back porch and front porch
	LCD_Reg_Set(0x09, 0x0000);		// set non-display area refresh cycle ISC[3:0]
	LCD_Reg_Set(0x0A, 0x0000);		// FMARK function
	LCD_Reg_Set(0x0C, 0x0000);		// RGB interface setting
	LCD_Reg_Set(0x0D, 0x0000);		// Frame marker Position
	LCD_Reg_Set(0x0F, 0x0000);		// RGB interface polarity

	//*************Power On sequence ****************//    WCOM(0x51, 0x00, 0xEF);
	LCD_Reg_Set(0x10, 0x0000);                // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_Reg_Set(0x11, 0x0007);                // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_Reg_Set(0x12, 0x0000);                // VREG1OUT voltage
	LCD_Reg_Set(0x13, 0x0000);                // VDV[4:0] for VCOM amplitude
	Delay1Ms(50);

	LCD_Reg_Set(0x10, 0x17B0);                // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_Reg_Set(0x11, 0x0001);                // DC1[2:0], DC0[2:0], VC[2:0]
	Delay1Ms(50);
	LCD_Reg_Set(0x12, 0x013C); //0x0130);                // VREG1OUT voltage
	Delay1Ms(50);
	LCD_Reg_Set(0x13, 0x1300); //0x1800);                // VDV[4:0] for VCOM amplitude
	LCD_Reg_Set(0x29, 0x0004); //0x0005);                // VCM[4:0] for VCOMH
	Delay1Ms(50);
	LCD_Reg_Set(0x20, 0x0000);                // GRAM horizontal Address
	LCD_Reg_Set(0x21, 0x0000);                // GRAM Vertical Address
	LCD_Reg_Set(0x2B, 0x0020);		// Frame Rate and Color Control-----16M_EN, Dither, FR_SEL[1:0]

	// ---------- Gamma Control  ---------- //
  	LCD_Reg_Set(0x30, 0x0000);
  	LCD_Reg_Set(0x31, 0x0306);
  	LCD_Reg_Set(0x32, 0x0200);
  	LCD_Reg_Set(0x35, 0x0107);
  	LCD_Reg_Set(0x36, 0x0404);
  	LCD_Reg_Set(0x37, 0x0606);
  	LCD_Reg_Set(0x38, 0x0105);
  	LCD_Reg_Set(0x39, 0x0707);
  	LCD_Reg_Set(0x3C, 0x0600);
  	LCD_Reg_Set(0x3D, 0x0807);
	// ---------- Window Address Area  ---------- //
	LCD_Reg_Set(0x50, 0x0000);		// Horizontal GRAM Start Address-----HSA[7:0]
	LCD_Reg_Set(0x51, 0x00EF);		// Horizontal GRAM End Address-----HEA[7:0]
	LCD_Reg_Set(0x52, 0x0000);		// Vertical GRAM Start Address-----VSA[8:0]
	LCD_Reg_Set(0x53, 0x013F);		// Vertical GRAM Start Address-----VEA[8:0]

	// ---------- Gate Scan Control  ---------- //
	LCD_Reg_Set(0x60, 0x2700);		// GS, NL[5:0], SCN[5:0]
	LCD_Reg_Set(0x61, 0x0001);		// NDL,VLE, REV
	LCD_Reg_Set(0x6A, 0x0000);		// VL[8:0]

	// ---------- Partial Display Control  ---------- //
	LCD_Reg_Set(0x80, 0x0000);		// Partial Image 1 Display Position-----PTDP0[8:0]
	LCD_Reg_Set(0x81, 0x0000);		// Partial Image 1 Start Address-----PTSA0[8:0]
	LCD_Reg_Set(0x82, 0x0000);		// Partial Image 1 End Address-----PTEA0[8:0]
	LCD_Reg_Set(0x83, 0x0000);		// Partial Image 2 Display Position-----PTDP1[8:0]
	LCD_Reg_Set(0x84, 0x0000);		// Partial Image 2 Start Address-----PTSA1[8:0]
	LCD_Reg_Set(0x85, 0x0000);		// Partial Image 2 Start Address-----PTEA1[8:0]

	// ---------- Panel Interface Control  ---------- //
	LCD_Reg_Set(0x90, 0x0013);		// Panel Interface Control 1-----DIVI[1:0], RTNI[4:0]
	LCD_Reg_Set(0x92, 0x0000);		// Panel Interface Control 2-----NOWI[2:0]
	LCD_Reg_Set(0x93, 0x0003);		// Panel Interface Control 3-----MCPI[2:0]
	LCD_Reg_Set(0x95, 0x0110);		// Panel Interface Control 4-----DIVE[1:0], RTNE[5:0]
	LCD_Reg_Set(0x97, 0x0000);		// Panel Interface Control 5-----NOWE[3:0]
	LCD_Reg_Set(0x98, 0x0000);		// Panel Interface Control 6-----MCPE[2:0]

	LCD_Reg_Set(0x07, 0x0001);		// Display Control 1
	Delay1Ms(50);
	LCD_Reg_Set(0x07, 0x0021);		// Display Control 1
	LCD_Reg_Set(0x07, 0x0023);		// Display Control 1
	Delay1Ms(50);
	LCD_Reg_Set(0x07, 0x0173);		// Display Control 1-----262K color and display ON

/*{
	int i,j;
    LCD_Reg_Set(0x21,0x0000) ;
    
    LCD_Command(0x22);
	for(i=0;i<320;i++)
		for(j=0;j<240;j++)
    			LCD_Data(0xF8F8);
    while(1);
}*/
/*{
	int i,j;
LCDDEV_SetWindow(0x00, 0x00, 0x13f, 0x20);
	for(i=0;i<0x13f;i++)
		for(j=0;j<0x20;j++)
    			LCD_Data(0xF8F8);
while(1);
}*/

/*    Delay1Ms(10);
    LCD_Reg_Set(0x0000, 0x0001);
    
    Delay1Ms(10);
	LCD_Reg_Set(0x0003, 0xA8A4);
 	LCD_Reg_Set(0x000C, 0x0000);
	LCD_Reg_Set(0x000D, 0x0308);
	LCD_Reg_Set(0x000E, 0x3000);
	LCD_Reg_Set(0x0001, 0x6B3F);    //F
	LCD_Reg_Set(0x0002, 0x0600);
	
	Delay1Ms(10);
	LCD_Reg_Set(0x0010, 0x0000);
	LCD_Reg_Set(0x0011, 0x6038);    //F
	
	Delay1Ms(10);
	
	LCD_Reg_Set(0x0005, 0x0000);
	LCD_Reg_Set(0x0006, 0x0000);
	LCD_Reg_Set(0x0016, 0xEF1C);
	LCD_Reg_Set(0x0017, 0x0003);
	
	Delay1Ms(10);
	LCD_Reg_Set(0x0007, 0x0001);
 	Delay1Ms(10);
 	LCD_Reg_Set(0x0007, 0x0021);
 	LCD_Reg_Set(0x0007, 0x0023);
    Delay1Ms(10);
 	LCD_Reg_Set(0x0007, 0x0033);
	Delay1Ms(10);
	
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
	LCD_Reg_Set(0x0021, 0x0000);  
#ifdef LCD_MEMORY_DEV
    LCD_Command(0x0022);
    
    for(x=0; x<240; x++){
        for(y=0; y<320; y++){
            LCD_Data(0);
        }
    } 
#endif*/
}

void Standby_lcd(void)
{
	Display_off();
	LCD_Reg_Set(0x10,0x6021);

/*	LCD_Reg_Set(0x0007, 0x0033);
	Delay1Ms(10);
	LCD_Reg_Set(0x0007, 0x0023);
	LCD_Reg_Set(0x0007, 0x0021);
	Delay1Ms(10);

	LCD_Reg_Set(0x0007, 0x0001);
	Delay1Ms(10);
	LCD_Reg_Set(0x0010, 0x0001);
	Delay1Ms(10);*/
}


void  standby_return()
{
	LCD_Reg_Set(0x00, 0x0001);		// Start internal OSC.
	//*************Power On sequence ****************//    WCOM(0x51, 0x00, 0xEF);
	LCD_Reg_Set(0x10, 0x0000);                // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_Reg_Set(0x11, 0x0007);                // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_Reg_Set(0x12, 0x0000);                // VREG1OUT voltage
	LCD_Reg_Set(0x13, 0x0000);                // VDV[4:0] for VCOM amplitude
	Delay1Ms(50);
	
	LCD_Reg_Set(0x10, 0x17B0);                // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_Reg_Set(0x11, 0x0001);                // DC1[2:0], DC0[2:0], VC[2:0]
	Delay1Ms(50);
	LCD_Reg_Set(0x12, 0x013C); //0x0130);                // VREG1OUT voltage
	Delay1Ms(50);
	LCD_Reg_Set(0x13, 0x1300); //0x1800);                // VDV[4:0] for VCOM amplitude
	LCD_Reg_Set(0x29, 0x0004); //0x0005);                // VCM[4:0] for VCOMH
	Delay1Ms(50);
	LCD_Reg_Set(0x20, 0x0000);                // GRAM horizontal Address                              
	LCD_Reg_Set(0x21, 0x0000);                // GRAM Vertical Address                                
	LCD_Reg_Set(0x2B, 0x0020);		// Frame Rate and Color Control-----16M_EN, Dither, FR_SEL[1:0]                                                                                               

	LCD_Reg_Set(0x07, 0x0001);		// Display Control 1
	Delay1Ms(50);
	LCD_Reg_Set(0x07, 0x0021);		// Display Control 1
	LCD_Reg_Set(0x07, 0x0023);		// Display Control 1
	Delay1Ms(50);
	LCD_Reg_Set(0x07, 0x0173);		// Display Control 1-----262K color and display ON

	/*LCD_Reg_Set(0x0010, 0x0000);
	Delay1Ms(10);
	LCD_Reg_Set(0x0007, 0x0001);
	Delay1Ms(10);
	LCD_Reg_Set(0x0007, 0x0021);
	LCD_Reg_Set(0x0007, 0x0023);
	Delay1Ms(10);
	LCD_Reg_Set(0x0007, 0x0033);
	Delay1Ms(10); */
}

//wjm 2.0 ILI9320
void Display_off(void)
{
	LCD_Reg_Set(0x0B,0x0000); 
	LCD_Reg_Set(0x10,0x6020);  
	LCD_Reg_Set(0x07,0x0036);
	Delay1Ms(6);
	LCD_Reg_Set(0x07,0x0026);
	Delay1Ms(6);
	LCD_Reg_Set(0x07,0x0004);
	Delay1Ms(9);
	LCD_Reg_Set(0x07,0x0037); 
	Delay1Ms(10);
	LCD_Reg_Set(0x11,0x0000);
	LCD_Reg_Set(0x10,0x0000);
	LCD_Reg_Set(0x12,0x0000);
	LCD_Reg_Set(0x13,0x0000);
}


void LCDDEV_SetWindow(int x0, int y0, int x1, int y1)
{
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif    
    //LCD_Reg_Set(0x0044, y1<<8 | y0);
    //LCD_Reg_Set(0x0045, x0);
    //LCD_Reg_Set(0x0046, x1);
    //LCD_SetDispAddr(x0, y0);

	LCD_Reg_Set(0x50, y0);		// Horizontal GRAM Start Address-----HSA[7:0]
	LCD_Reg_Set(0x51, y1);		// Horizontal GRAM End Address-----HEA[7:0]
	LCD_Reg_Set(0x52, x0);		// Vertical GRAM Start Address-----VSA[8:0]
	LCD_Reg_Set(0x53, x1);		// Vertical GRAM Start Address-----VEA[8:0]
	LCD_SetDispAddr(x0, y0);
	//LCD_Command(0x22);
}


void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif
    //LCD_Reg_Set(0x0044, y1<<8 | y0);
    //LCD_Reg_Set(0x0045, x0);
    //LCD_Reg_Set(0x0046, x1); 

	/*LCD_Reg_Set(0x50, x0);		// Horizontal GRAM Start Address-----HSA[7:0]
	LCD_Reg_Set(0x51, x1);		// Horizontal GRAM End Address-----HEA[7:0]
	LCD_Reg_Set(0x52, y0);		// Vertical GRAM Start Address-----VSA[8:0]
	LCD_Reg_Set(0x53, y1);		// Vertical GRAM Start Address-----VEA[8:0]
	LCD_Command(0x0022);*/
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





//length
void DMA_LcdJpegInitX(void)
{
    //LCD_Reg_Set(0x0001, 0x6B3F);    //F
    //LCD_Reg_Set(0x0011, 0x6038);    //F
    LCD_Reg_Set(0x03, 0x10B8); //0x1038); //0x1028); //0x1038);		// set GRAM write direction and BGR=1.
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 
}


//weight
void DMA_LcdJpegInit(void)
{
    //LCD_Reg_Set(0x0001, 0x2B3F);    //F
    //LCD_Reg_Set(0x0011, 0x6030);    //F
    LCD_Reg_Set(0x03, 0x10A0); //0x1020); //0x1028); //0x1038);		// set GRAM write direction and BGR=1.
    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1); 	
}


void MP4_LCD_Init(void)
{
    //LCD_Reg_Set(0x0001, 0x6B3F);    //F
    //LCD_Reg_Set(0x0011, 0x6030);    //F
//    LCD_Reg_Set(0x03, 0x1030); //0x1028); //0x1038);		// set GRAM write direction and BGR=1.
//    LCDDEV_SetWindow(0, 0,  LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
    
    LCD_Init(MP4_LCD_INIT);
    LCD_SetDispAddr(0,0);
}



          
//******************************************************************************


        
