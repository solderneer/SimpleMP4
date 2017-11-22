//******************************************************************************
#define     _IN_LCD_LTPS
//------------------------------------------------------------------------------
#include "Memap.h"
#include "Macro.h"
#include "Lcd.h"
//------------------------------------------------------------------------------
void LCD_Command(unsigned int cmd)
{
    write_XDATA(LCD_COMMAND,cmd);
}

void LCD_Data(unsigned int data)
{
    write_XDATA(LCD_DATA,data);
}

void LCD_Reg_Set(unsigned int cmd,unsigned int data)
{
    write_XDATA(LCD_COMMAND,cmd);
    write_XDATA(LCD_DATA,data);
}

unsigned int LCD_StatusRead(void)
{
    unsigned int i,j;
    i = read_XDATA(LCD_COMMAND);
#ifdef  Parallel8
    j = read_XDATA(LCD_COMMAND);
    i = ((i & 0xff00) | (j >> 8));
#endif
    return i;
}

void LCD_SetPixel(unsigned int x, unsigned int y, unsigned int color)
{
	LCD_Reg_Set(0X14, x);
    LCD_Reg_Set(0X15, y);
	write_XDATA(LCD_DATA,color);	
}

unsigned int LCD_GetPixel(unsigned int x, unsigned int y)
{
	unsigned int color;
	unsigned int i;
/*	LCD_Reg_Set(0x0003,0x1030);
	LCD_Reg_Set(0x0044,0x7f00);
    LCD_Reg_Set(0x0045,0x9f00);*/

	LCD_Reg_Set(0X14, x);
    LCD_Reg_Set(0X15, y);	
	color = read_XDATA(LCD_DATA);
	//for(i=10; i>0; i--);
	color = read_XDATA(LCD_DATA);
	//color = DataChange(color);
	return(color);
}

void LCD_XorPixel(unsigned int x, unsigned int y)
{
	unsigned int color;
	color = LCD_GetPixel(x,y);
	LCD_SetPixel(x,y,0xFFFF-color);	
}
/*
void LCD_ReadBipmapData(unsigned int x, unsigned int y, unsigned int xsize, unsigned int ysize, unsigned int *pBuf)
{
	unsigned int x_bck;
	unsigned int i;
	for(i=0; i<ysize; i++){
		x_bck = x;
		LCD_Reg_Set(0X14, x);
    	LCD_Reg_Set(0X15, y);
		while(x_bck > 0){
			*pBuf   = read_XDATA(LCD_DATA);
			*pBuf++ = read_XDATA(LCD_DATA);
		}
	}	
}
*/

void LCD_PowerOff(void)
{
	LCD_Reg_Set(0X41, 0x95); /*Set PWDN=0*/
	LCD_Reg_Set(0X20, 0x00);
	LCD_Reg_Set(0X34, 0x18);
	/*Set Main/Sub Panel Off*/
	/*Set CKV1 and CKV2 inversion*/
	LCD_Reg_Set(0X3A,0x00);	
}

void MP4_LCD_Init(void)
{
	int x,y;
	
	LCD_Reg_Set( 0X01, 0x06 ); 	/*Set CPU Interface Mode??16 bits Data Bus and
									  R:G:B=5:6:5*/
	//LCD_Reg_Set( 0x02, 0x02);								  	
	
	LCD_Reg_Set( 0X16, 0x10 ); 	/*Set Main Panel Full Color Display*/

	LCD_Reg_Set( 0x17, 0x01 );

	LCD_Reg_Set( 0X20, 0x04 ); 	/*Set Main/Sub Memory Used for Main/Sub Panel
									  Display Respectively??Line Inv. ??Resolution
									  128x160??Main Panel On??Sub Panel Off*/
	LCD_Reg_Set( 0x25, 0x10 );		// 0x20 ltor						  
									  
	LCD_Reg_Set( 0X40, 0x28 );		/*Set VCO Frequency for Single Panel Display*/
	LCD_Reg_Set( 0X4A, 0xBF ); 	/*Set Charge Pump Frequency*/
	LCD_Reg_Set( 0X34, 0x09 ); 	/*Set CKV to No Mask*/
	LCD_Reg_Set( 0X2A, 0x28 );
	LCD_Reg_Set( 0X2B, 0x01 );
	LCD_Reg_Set( 0X30, 0x00 );
	LCD_Reg_Set( 0X3A, 0x00 );
	
	//LCD_Reg_Set( 0X36, 0x00 );
	
	LCD_Reg_Set( 0X70, 0x00 ); 	/*Set Main Memory Active*/
	LCD_Reg_Set( 0X10, 0x00 ); 	/*Set Start Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X11, 0x00 ); 	/*Set Start Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X12, 0x7F ); 	/*Set End Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X13, 0x9F ); 	/*Set End Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X14, 0x00 ); 	/*Set Start Address of Memory Access in X-axis*/
	LCD_Reg_Set( 0X15, 0x00 ); 	/*Set Start Address of Memory Access in Y-axis*/

	

//	for(x=0;x<128;x++){
//		for(y=0;y<160;y++){
//			write_XDATA(LCD_DATA,0xF800);
//		}
//	}
	LCD_Reg_Set( 0X41, 0xC0 ); 		/*Set PWDN=1*/   
	
	//write_mem(pio,0x7fff);
}

void LCD_Init(void)
{
	int x,y;
	
	LCD_Reg_Set( 0X01, 0x06 ); 	/*Set CPU Interface Mode??16 bits Data Bus and
									  R:G:B=5:6:5*/
	//LCD_Reg_Set( 0x02, 0x02);								  	
	
	LCD_Reg_Set( 0X16, 0x10 ); 	/*Set Main Panel Full Color Display*/
	LCD_Reg_Set( 0X17, 0x00);
	
	LCD_Reg_Set( 0X20, 0x04 ); 	/*Set Main/Sub Memory Used for Main/Sub Panel
									  Display Respectively??Line Inv. ??Resolution
									  128x160??Main Panel On??Sub Panel Off*/
	LCD_Reg_Set( 0X25, 0x00 );								  
									  
	LCD_Reg_Set( 0X40, 0x28 );		/*Set VCO Frequency for Single Panel Display*/
	LCD_Reg_Set( 0X4A, 0xBF ); 	/*Set Charge Pump Frequency*/
	LCD_Reg_Set( 0X34, 0x09 ); 	/*Set CKV to No Mask*/
	LCD_Reg_Set( 0X2A, 0x28 );
	LCD_Reg_Set( 0X2B, 0x01 );
	LCD_Reg_Set( 0X30, 0x00 );
	LCD_Reg_Set( 0X3A, 0x00 );
	
	//LCD_Reg_Set( 0X36, 0x03 );
	
	LCD_Reg_Set( 0X70, 0x00 ); 	/*Set Main Memory Active*/
	LCD_Reg_Set( 0X10, 0x00 ); 	/*Set Start Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X11, 0x00 ); 	/*Set Start Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X12, 0x7F ); 	/*Set End Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X13, 0x9F ); 	/*Set End Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X14, 0x00 ); 	/*Set Start Address of Memory Access in X-axis*/
	LCD_Reg_Set( 0X15, 0x00 ); 	/*Set Start Address of Memory Access in Y-axis*/

	for(x=0;x<128;x++){
		for(y=0;y<160;y++){
			write_XDATA(LCD_DATA,0x0);
		}
	}
	LCD_Reg_Set( 0X41, 0xC0 ); 		/*Set PWDN=1*/   
	
	//write_mem(pio,0x7fff);
}

void Display_Sub(unsigned int RGB_Data)
{
    unsigned int  i,j;

    LCD_Reg_Set(0X14, 0x00);
    LCD_Reg_Set(0X15, 0x00);
    for(i = 160; i>0; i--){
        for(j = 128; j>0; j--)        
            write_XDATA(LCD_DATA,RGB_Data);
    }
}

void Display_Image(unsigned int *pBuffer)
{
    unsigned int i,j,temp,temp_data;

    LCD_Reg_Set(0X14, 0x00);
    LCD_Reg_Set(0X15, 0x00);
    for(i = 0x800; i>0; i--){
        temp_data = *pBuffer++;//pBuffer[i];
        temp = 0x0080;
        for(j=8; j>0; j--){
            if(temp & temp_data){
                write_XDATA(LCD_DATA,0x001f);
            }else{
                write_XDATA(LCD_DATA,0x07e0);
            }
            temp = temp >> 1;
        }
    }
}

void Display_Picture(unsigned int *pBuffer)
{
    unsigned int  i,j;

    LCD_Reg_Set(0X14, 0x00);
    LCD_Reg_Set(0X15, 0x00);
    for(i = 160; i>0; i--){
        for(j = 128; j>0; j--){
           	write_XDATA(LCD_DATA,*pBuffer++);
     	}
    }
}



void LCD_SetDispAddr(unsigned int x,unsigned int y)
{
	LCD_Reg_Set(0X14, x);
    LCD_Reg_Set(0X15, y);
}

void LCDDEV_SetWindow(int x0, int y0, int x1, int y1)
{
	LCD_Reg_Set(0x10,x0);
	LCD_Reg_Set(0x11,y0);
	LCD_Reg_Set(0x12,x1);
	LCD_Reg_Set(0x13,y1);
	LCD_Reg_Set(0x14,x0);
	LCD_Reg_Set(0x15,y0);	
}

void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
	LCD_Reg_Set(0x10,x0);
	LCD_Reg_Set(0x11,y0);
	LCD_Reg_Set(0x12,x1);
	LCD_Reg_Set(0x13,y1);
}

void LCD_ReadBitmap(int x0, int y0, int xsize, int ysize, unsigned int *pData)
{
	int x1, y1;
	x1 = x0+xsize-1; 
	y1 = y0+ysize-1; 
	LCDDEV_SetWindow(x0, y0, x1, y1);
	read_XDATA(LCD_DATA);
	for(y1=0; y1<ysize; y1++){
		for(x1=0; x1<xsize; x1++){
			*pData++ = read_XDATA(LCD_DATA);
		}
	}
		
	LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
}


unsigned int LCDDEV_GetPixel(int x, int y)
{
	unsigned int color;
	
	LCD_Reg_Set(0x14,x);
	LCD_Reg_Set(0x15,y);	
	color = read_XDATA(LCD_DATA);
	color = read_XDATA(LCD_DATA);
	return(color);	
}

void LCDDEV_SetPixel(int x, int y, unsigned int color)
{
	LCD_Reg_Set(0x14,x);
	LCD_Reg_Set(0x15,y);
	write_XDATA(LCD_DATA,color);
}

void LCDDEV_XorPixel(int x, int y)
{
	unsigned int color;
	color = LCDDEV_GetPixel(x,y);
	LCDDEV_SetPixel(x,y,0xFFFF-color);			
}

void LCDDEV_DrawHLine(int x0, int y, int x1)
{
	if (LCD_Context.DrawMode & LCD_DRAWMODE_XOR){
		for(; x0<=x1; x0++){
			LCDDEV_XorPixel(x0, y);
		}
	}else{
		LCDDEV_SetWindow(x0, y, x1, y);
		for(; x0<=x1; x0++){
			write_XDATA(LCD_DATA, LCD_COLOR);
		}
		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
	}
}

void LCDDEV_DrawVLine(int x, int y0, int y1)
{
    unsigned int color;
    
	if (LCD_Context.DrawMode & LCD_DRAWMODE_XOR){
		for(; y0<=y1; y0++){
			LCDDEV_XorPixel(x, y0);
		}
	}else{
	    if(LCD_Context.DrawMode == LCD_DRAWMODE_REV)
	        color = LCD_BKCOLOR;
	    else
	        color = LCD_COLOR;
		LCDDEV_SetWindow(x, y0, x, y1);
		for(; y0<=y1; y0++){
			write_XDATA(LCD_DATA, color);
		}	
		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
	}
}

void LCDDEV_FillRect(int x0, int y0, int x1, int y1)
{
    unsigned int color;
	int i;
	if (LCD_Context.DrawMode & LCD_DRAWMODE_XOR){
		for(; y0<=y1; y0++){
			LCDDEV_DrawHLine(x0, y0, x1);
		}
	}else{
	    if(LCD_Context.DrawMode == LCD_DRAWMODE_REV)
	        color = LCD_BKCOLOR;
	    else
	        color = LCD_COLOR;
		LCDDEV_SetWindow(x0, y0, x1, y1);
		for(; y0<=y1 ; y0++){
			i = x0;
			for(; i<=x1; i++){
				write_XDATA(LCD_DATA, color);
			}
		}
		
		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
	}
}

void LCDDEV_DrawBitLine1BPP(int x, int y, int xsize, int Diff, unsigned int *pData) 
{
  	x += Diff;

	switch(LCD_Context.DrawMode){
		case LCD_DRAWMODE_REV:
			while(xsize){
				if(*pData & (0x8000 >> Diff))
					LCDDEV_SetPixel(x, y, LCD_BKCOLOR);
				else
					LCDDEV_SetPixel(x, y, LCD_COLOR);
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
					LCDDEV_SetPixel(x, y, LCD_COLOR);
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
					LCDDEV_XorPixel(x,y);
				x++;
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
					LCDDEV_SetPixel(x, y, LCD_COLOR);
				else
					LCDDEV_SetPixel(x, y, LCD_BKCOLOR);
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
			write_XDATA(LCD_DATA, *pData++);
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
   			for(i=0; i<ysize; i++){ 
   				LCDDEV_DrawBitLine1BPP(x0, i+y0, xsize, Diff, pData);
   				pData += BytesPerLine;
   			}
   			break;
   		case 16:
       		LCDDEV_SetWindow(x0, y0, x0+xsize-1, y0+ysize-1);
       		for(; ysize > 0; ysize--){
       		    LCDDEV_DrawBitLine16BPP(x0, y0++, xsize, 1, pData);
       		    pData += BytesPerLine;
       		} 
       		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
       	break;
   	}
}

void DMA_LcdJpegInit(void)
{
	int x,y;
	
	
    for(x=0;x<128;x++){
		for(y=0;y<160;y++){
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			write_XDATA(LCD_DATA,0x0);
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
		}
	}
	
	LCD_Reg_Set( 0X01, 0x06 ); 	/*Set CPU Interface Mode??16 bits Data Bus and
									  R:G:B=5:6:5*/
	//LCD_Reg_Set( 0x02, 0x02);								  	
	
	LCD_Reg_Set( 0X16, 0x10 ); 	/*Set Main Panel Full Color Display*/
	LCD_Reg_Set( 0X17, 0x00);
	
	LCD_Reg_Set( 0X20, 0x04 ); 	/*Set Main/Sub Memory Used for Main/Sub Panel
									  Display Respectively??Line Inv. ??Resolution
									  128x160??Main Panel On??Sub Panel Off*/
	LCD_Reg_Set( 0X25, 0x00 );								  
									  
	LCD_Reg_Set( 0X40, 0x28 );		/*Set VCO Frequency for Single Panel Display*/
	LCD_Reg_Set( 0X4A, 0xBF ); 	/*Set Charge Pump Frequency*/
	LCD_Reg_Set( 0X34, 0x09 ); 	/*Set CKV to No Mask*/
	LCD_Reg_Set( 0X2A, 0x28 );
	LCD_Reg_Set( 0X2B, 0x01 );
	LCD_Reg_Set( 0X30, 0x00 );
	LCD_Reg_Set( 0X3A, 0x00 );
	
	//LCD_Reg_Set( 0X36, 0x03 );
	
	LCD_Reg_Set( 0X70, 0x00 ); 	/*Set Main Memory Active*/
	LCD_Reg_Set( 0X10, 0x00 ); 	/*Set Start Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X11, 0x00 ); 	/*Set Start Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X12, 0x7F ); 	/*Set End Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X13, 0x9F ); 	/*Set End Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X14, 0x00 ); 	/*Set Start Address of Memory Access in X-axis*/
	LCD_Reg_Set( 0X15, 0x00 ); 	/*Set Start Address of Memory Access in Y-axis*/
/*

	*/
	LCD_Reg_Set( 0X41, 0xC0 ); 		/*Set PWDN=1*/   
	
	//write_mem(pio,0x7fff);
}

void DMA_LcdJpegInitX(void)
{
	int x,y;
	
	for(x=0;x<128;x++){
		for(y=0;y<160;y++){
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			write_XDATA(LCD_DATA,0x0);
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
		}
	}
	
	LCD_Reg_Set( 0X01, 0x06 ); 	/*Set CPU Interface Mode??16 bits Data Bus and
									  R:G:B=5:6:5*/
	//LCD_Reg_Set( 0x02, 0x02);								  	
	
	LCD_Reg_Set( 0X16, 0x10 ); 	/*Set Main Panel Full Color Display*/

	LCD_Reg_Set( 0x17, 0x01 );

	LCD_Reg_Set( 0X20, 0x04 ); 	/*Set Main/Sub Memory Used for Main/Sub Panel
									  Display Respectively??Line Inv. ??Resolution
									  128x160??Main Panel On??Sub Panel Off*/
	LCD_Reg_Set( 0x25, 0x10 );		// 0x20 ltor						  
									  
	LCD_Reg_Set( 0X40, 0x28 );		/*Set VCO Frequency for Single Panel Display*/
	LCD_Reg_Set( 0X4A, 0xBF ); 	/*Set Charge Pump Frequency*/
	LCD_Reg_Set( 0X34, 0x09 ); 	/*Set CKV to No Mask*/
	LCD_Reg_Set( 0X2A, 0x28 );
	LCD_Reg_Set( 0X2B, 0x01 );
	LCD_Reg_Set( 0X30, 0x00 );
	LCD_Reg_Set( 0X3A, 0x00 );
	
	//LCD_Reg_Set( 0X36, 0x00 );
	
	LCD_Reg_Set( 0X70, 0x00 ); 	/*Set Main Memory Active*/
	LCD_Reg_Set( 0X10, 0x00 ); 	/*Set Start Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X11, 0x00 ); 	/*Set Start Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X12, 0x7F ); 	/*Set End Address of Display Window in X-axis*/
	LCD_Reg_Set( 0X13, 0x9F ); 	/*Set End Address of Display Window in Y-axis*/
	LCD_Reg_Set( 0X14, 0x00 ); 	/*Set Start Address of Memory Access in X-axis*/
	LCD_Reg_Set( 0X15, 0x00 ); 	/*Set Start Address of Memory Access in Y-axis*/

	/*

*/
	LCD_Reg_Set( 0X41, 0xC0 ); 		/*Set PWDN=1*/   
	
	//write_mem(pio,0x7fff);
}
//******************************************************************************
