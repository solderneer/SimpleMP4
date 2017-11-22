//******************************************************************************
#define     _IN_LCD_LTPS
//------------------------------------------------------------------------------
#include "Memap.h"
#include "Macro.h"
#include "Lcd.h"
//---------driver program HX8309---TFT2.0-------------------------------------------------------------------
#undef   Parallel8
#define  UINT16     unsigned short
#define LCD_INIT_NORMAL 0
#define MP4_LCD_INIT    2

void LCD_Init(unsigned int type) ;
void LCD_SetDispAddr(unsigned int x,unsigned int y);
void Display_off(void) ;


//void Delay1Ms(unsigned int tmr)
//{
//    unsigned int i;
//    unsigned int j;
//    while(tmr){
//        for(j=0; j<0x6000; j++){
//            asm("nop");
//            asm("nop");
//            asm("nop");
//            asm("nop");
//        }
//        tmr--;
//    }
//}


void LCD_Command(unsigned int cmd)
{
#ifdef  Parallel8    
    write_mem(LCD_COMMAND,cmd >> 8);
#endif    
    write_mem(LCD_COMMAND,cmd);
}

void LCD_Data(unsigned int data)
{
#ifdef  Parallel8    
    write_mem(LCD_DATA,data >> 8);
#endif    
    write_mem(LCD_DATA,data);
}

void LCD_Reg_Set(unsigned int cmd,unsigned int data)
{
#ifdef  Parallel8        
    write_mem(LCD_COMMAND,cmd >> 8);   
#endif    
    write_mem(LCD_COMMAND,cmd); 
    
#ifdef  Parallel8        
    write_mem(LCD_DATA,data >> 8);
#endif    
    write_mem(LCD_DATA,data);
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


// wjm 2.0 HX8309
void LCD_SetPixel(unsigned int x, unsigned int y, unsigned int color)
{
	  unsigned short i;
	  i = (x<<8) + y ;
    LCD_Reg_Set(0x0021,i) ;
    
    LCD_Command(0x0022) ;	
	  LCD_Data(color) ;	      //color表示RGB值, 这个函数的功能是往(x,y)写入color值
}


unsigned int LCD_GetPixel(unsigned int x, unsigned int y)
{
	unsigned int color;
	unsigned int i;

	LCD_Reg_Set(0x16, x);
  LCD_Reg_Set(0x17, y);	
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


//void LCD_ReadBipmapData(unsigned int x, unsigned int y, unsigned int xsize, unsigned int ysize, unsigned int *pBuf)
//{
//	unsigned int x_bck;
//	unsigned int i;
//	for(i=0; i<ysize; i++){
//		x_bck = x;
//		LCD_Reg_Set(0x16, x);
//    LCD_Reg_Set(0x17, y);
//		while(x_bck > 0){
//			*pBuf   = read_XDATA(LCD_DATA);
//			*pBuf++ = read_XDATA(LCD_DATA);
//		}
//	}	
//}


//wjm 2.0 
void MP4_LCD_Init(void)
{
    short i,j ;
   
    LCD_Init(MP4_LCD_INIT) ;
    
    //LCD_Reg_Set(0x0003,0x1030); //6020
    
    LCD_SetDispAddr(0, 0);    
}


//wjm 2.0 HX8309
void LCD_Init(unsigned int type)
{
	LCD_Reg_Set(0x0001,0x001B);
	
	LCD_Reg_Set(0x0002,0x0400); 
	if(type == MP4_LCD_INIT){
        LCD_Reg_Set(0x0003,0x1030); //6020
    }else{
        LCD_Reg_Set(0x0003,0x1038); //6020
  }
	LCD_Reg_Set(0x0004,0x0000);
	LCD_Reg_Set(0x0005,0x0000);
	//LCD_Reg_Set(0x0007,0x0004);
	LCD_Reg_Set(0x0008,0x0202);
	LCD_Reg_Set(0x0009,0x0000);
	LCD_Reg_Set(0x000B,0x0000);
	LCD_Reg_Set(0x000C,0x0003);
	
	LCD_Reg_Set(0x0040,0x0000);
	LCD_Reg_Set(0x0041,0x00ef); //00ef
	LCD_Reg_Set(0x0042,0xDB00);
	LCD_Reg_Set(0x0043,0xDB00);
	
	LCD_Reg_Set(0x0021,0x0000);
	LCD_Reg_Set(0x0023,0x0000);
	LCD_Reg_Set(0x0024,0x0000);
	LCD_Reg_Set(0x0044,0xAF00);
	LCD_Reg_Set(0x0045,0xDB00);
	
	LCD_Reg_Set(0x0021, 0x0000);
	LCD_Command(0x0022);	
}


void  PowerOnLcdinit()
{
	Delay1Ms(50);
		//other register setting
	LCD_Reg_Set(0x0001,0x001B);
	
	LCD_Reg_Set(0x0002,0x0400); 
	
	LCD_Reg_Set(0x0003,0x1038); //6020
	LCD_Reg_Set(0x0004,0x0000);
	LCD_Reg_Set(0x0005,0x0000);
	LCD_Reg_Set(0x0007,0x0004);
	LCD_Reg_Set(0x0008,0x0202);
	LCD_Reg_Set(0x0009,0x0000);
	LCD_Reg_Set(0x000B,0x0000);
	LCD_Reg_Set(0x000C,0x0003);
	
	LCD_Reg_Set(0x0040,0x0000);
	LCD_Reg_Set(0x0041,0x00ef); //00ef
	LCD_Reg_Set(0x0042,0xDB00);
	LCD_Reg_Set(0x0043,0xDB00);
	
	LCD_Reg_Set(0x0021,0x0000);
	LCD_Reg_Set(0x0023,0x0000);
	LCD_Reg_Set(0x0024,0x0000);
	LCD_Reg_Set(0x0044,0xAF00);
	LCD_Reg_Set(0x0045,0xDB00);
	//other register setting
	Delay1Ms(150);
	LCD_Reg_Set(0x0000,0x0001);

	Delay1Ms(50);
	LCD_Reg_Set(0x0010,0x0000);
	LCD_Reg_Set(0x0011,0x0000);
	LCD_Reg_Set(0x0012,0x0000);	
	LCD_Reg_Set(0x0013,0x0000);	
	
	LCD_Reg_Set(0x0013,0x1517);
	LCD_Reg_Set(0x0012,0x0008);
	LCD_Reg_Set(0x0010,0x4040);
	Delay1Ms(30);
	LCD_Reg_Set(0x0010,0x0054);
	LCD_Reg_Set(0x0012,0x0015);
	Delay1Ms(30);
	LCD_Reg_Set(0x0010,0x4460);
	Delay1Ms(30);
	//LCD_Reg_Set(0x0013,0x3613);
//	LCD_Reg_Set(0x0013,0x3006);
    LCD_Reg_Set(0x0013,0x2C0B);
	
	Delay1Ms(150);        
	//gamma CORRECTION
	LCD_Reg_Set(0x0030,0x0000);
	LCD_Reg_Set(0x0031,0x0207);
	LCD_Reg_Set(0x0032,0x0002);
	LCD_Reg_Set(0x0033,0x0104);
	LCD_Reg_Set(0x0034,0x0305);
	LCD_Reg_Set(0x0035,0x0002);
	LCD_Reg_Set(0x0036,0x0706);
	LCD_Reg_Set(0x0037,0x0503);
	LCD_Reg_Set(0x0038,0x0000);
	LCD_Reg_Set(0x0039,0x0000);
	//gamma CORRECTION
	
	LCD_Reg_Set(0x0007,0x0005);
	Delay1Ms(50);
	LCD_Reg_Set(0x0007,0x0025);
	LCD_Reg_Set(0x0007,0x0027);
	Delay1Ms(50);
	LCD_Reg_Set(0x0007,0x0037); 
	Delay1Ms(15);	
}

//wjm 2.0 HX8309
void Standby_lcd(void)
{
	Display_off();
	LCD_Reg_Set(0x0010,0x6021);
	//close_backlight
}


void  standby_return()
{
	LCD_Reg_Set(0x0000,0x0001);
    Delay1Ms(15);
    LCD_Reg_Set(0x0010,0x0000);
	
	LCD_Reg_Set(0x0013,0x1517);
	LCD_Reg_Set(0x0012,0x0008);
	LCD_Reg_Set(0x0010,0x4040);
	Delay1Ms(20);
	LCD_Reg_Set(0x0010,0x0054);
	LCD_Reg_Set(0x0012,0x0015);
	Delay1Ms(20);
	LCD_Reg_Set(0x0010,0x4460);
	Delay1Ms(20);
	//LCD_Reg_Set(0x0013,0x3613);
	//LCD_Reg_Set(0x0013,0x360d);
//	LCD_Reg_Set(0x0013,0x3006);
    LCD_Reg_Set(0x0013,0x2C0B);
	
	Delay1Ms(30);        
	LCD_Reg_Set(0x0007,0x0005);
	Delay1Ms(20);
	LCD_Reg_Set(0x0007,0x0025);
	LCD_Reg_Set(0x0007,0x0027);
	Delay1Ms(20);
	LCD_Reg_Set(0x0007,0x0037); 
}

//wjm 2.0 HX8309
void Display_off(void)
{
	LCD_Reg_Set(0x000B,0x0000); 
	LCD_Reg_Set(0x0010,0x6020);  
	LCD_Reg_Set(0x0007,0x0036);
	Delay1Ms(6);
	LCD_Reg_Set(0x0007,0x0026);
	Delay1Ms(6);
	LCD_Reg_Set(0x0007,0x0004);
	Delay1Ms(9);
	LCD_Reg_Set(0x0007,0x0037); 
	Delay1Ms(10);
	LCD_Reg_Set(0x0011,0x0000);
	LCD_Reg_Set(0x0010,0x0000);
	LCD_Reg_Set(0x0012,0x0000);
	LCD_Reg_Set(0x0013,0x0000);
}


//wjm 2.0 HX8309
//void Sleep_lcd(void)
//{
//  Display_off();
//	LCD_Reg_Set(0x0010,0x6022);
//	//close_backlight 	
//}


////wjm  2.0 HX8309
//void Display_Sub(UINT16 RGB_Data)
//{
//    UINT16  i,j;
//    UINT16  temp ;
//    
//    LCD_Command(0x0021);       //ram address set
//    LCD_Data(0x0000);           //    
//    
//    LCD_Command(0x0022);       //ram address write 
//    for(i = 176; i>0; i--)
//    {
//        for(j = 220; j>0; j--)      
//           {
//               LCD_Data(RGB_Data);
//           }
//    }
//    
//}


////wjm 2.0 HX8309
//void Display_Image(unsigned int *pBuffer)
//{
//    unsigned int i,j,temp,temp_data;
//
//    LCD_Command(0x0021);       //ram address set
//    LCD_Data(0x0000);           //    
//    
//    LCD_Command(0x0022);       //ram address write 
//    
//    for(i = 0x800; i>0; i--){
//        temp_data = *pBuffer++;   //pBuffer[i];
//        temp = 0x0080;
//        for(j=8; j>0; j--){
//            if(temp & temp_data){
//                LCD_Data(0x001f);
//            }else{
//                LCD_Data(0x07e0);
//            }
//            temp = temp >> 1;
//        }
//    }
//}


////wjm 2.0 HX8309
//void Display_Picture(unsigned int *pBuffer)
//{
//    unsigned int  i,j;
//
//    LCD_Command(0x0021);       //ram address set
//    LCD_Data(0x0000);           //    
//    
//    LCD_Command(0x0022);       //ram address write 
//    for(i = 176; i>0; i--){
//        for(j = 220; j>0; j--){
//           	LCD_Data(*pBuffer++);
//     	}
//    }
//}


//2.0 wjm  HX8309
void LCD_SetDispAddr(unsigned int x,unsigned int y)
{
	  unsigned short i;
	  i = (x<<8) + y ;
    LCD_Reg_Set(0x0021,i) ;
    
    LCD_Command(0x0022) ;	
}


//2.0 wjm HX8309
void LCDDEV_SetWindow(int x0, int y0, int x1, int y1)
{
	  unsigned short row,column ;
	  row =  (x1<<8) + x0 ;
	  column = (y1<<8) + y0 ;
    
	  LCD_Reg_Set(0x0045,row);
	  LCD_Reg_Set(0x0044,column);
	  LCD_SetDispAddr(x0, y0) ;
	  
	  LCD_Command(0x0022);   
}


//2.0 wjm HX8309
void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
   LCDDEV_SetWindow(x0,y0,x1,y1) ;  
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
	unsigned int color;
	
	LCD_Reg_Set(0x0021, (y<<8 | x));	
	color = read_XDATA(LCD_DATA);
	color = read_XDATA(LCD_DATA);
	return(color);	
}


void LCDDEV_SetPixel(int x, int y, unsigned int color)
{
	  unsigned short i;
	  i = (x<<8) + y ;
    LCD_Reg_Set(0x0021,i) ;
    
    LCD_Command(0x0022) ;	
	  LCD_Data(color) ;	        //color表示RGB值, 这个函数的功能是往(x,y)写入color值

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
//#ifdef LCD_SWITCH_XY
//void LCDDEV_DrawBitLine1BPP(int x, int y, int xsize, int Diff, unsigned int *pData) 
//{
//  	x += Diff;
//
//	switch(LCD_Context.DrawMode){
//		case LCD_DRAWMODE_REV:
//			while(xsize){
//				if(*pData & (0x8000 >> Diff))
//					LCD_SetPixel(y, x, LCD_BKCOLOR);
//				else
//					LCD_SetPixel(y, x, LCD_COLOR);
//				x++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//			break;
//		case LCD_DRAWMODE_TRANS:
//			while(xsize){
//				if(*pData & (0x8000 >> Diff))
//					LCD_SetPixel(y, x, LCD_COLOR);
//				x++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//			break;
//		case LCD_DRAWMODE_XOR:
//			while(xsize){
//				if(*pData & (0x8000 >> Diff))
//					LCD_XorPixel(y,x);
//				x++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//			break;
//		default:
//			while(xsize){
//				if(*pData & (0x8000 >> Diff))
//					LCD_SetPixel(y, x, LCD_COLOR);
//				else
//					LCD_SetPixel(y, x, LCD_BKCOLOR);
//				x++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//			break;
//	}	
//}
//#else
//void LCDDEV_DrawBitLine1BPP(int x, int y, int xsize, int Diff, unsigned int *pData) 
//{
//  	x += Diff;
//
//	switch(LCD_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)){
//		case 0:
//			while(xsize){
//				if(*pData & (0x8000 >> Diff))
//					LCD_SetPixel(x, y, LCD_COLOR);
//				else
//					LCD_SetPixel(x, y, LCD_BKCOLOR);
//				x++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//			break;
//		case LCD_DRAWMODE_TRANS:
//			while(xsize){
//				if(*pData & (0x8000 >> Diff))
//					LCD_SetPixel(x, y, LCD_COLOR);
//				x++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//			break;
//		case LCD_DRAWMODE_XOR:
//			while(xsize){
//				if(*pData & (0x8000 >> Diff))
//					LCD_XorPixel(x,y);
//				x++;
//				xsize--;
//				if(++Diff == 16){
//					Diff = 0;
//					pData++;
//				}
//			}
//			break;
//		default:
//			break;
//	}	
//}
//#endif

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


//length
void DMA_LcdJpegInitX(void)
{
	LCD_Reg_Set(0x0001,0x001B);
	
	LCD_Reg_Set(0x0002,0x0400); 
    LCD_Reg_Set(0x0003,0x1038); //6020

	LCD_Reg_Set(0x0004,0x0000);
	LCD_Reg_Set(0x0005,0x0000);
	//LCD_Reg_Set(0x0007,0x0004);
	LCD_Reg_Set(0x0008,0x0202);
	LCD_Reg_Set(0x0009,0x0000);
	LCD_Reg_Set(0x000B,0x0000);
	LCD_Reg_Set(0x000C,0x0003);
	
//	LCD_Reg_Set(0x0040,0x0000);
//	LCD_Reg_Set(0x0041,0x00ef); //00ef
//	LCD_Reg_Set(0x0042,0xDB00);
//	LCD_Reg_Set(0x0043,0xDB00);
//	
//	LCD_Reg_Set(0x0021,0x0000);
//	LCD_Reg_Set(0x0023,0x0000);
//	LCD_Reg_Set(0x0024,0x0000);
//	LCD_Reg_Set(0x0044,0xAF00);
//	LCD_Reg_Set(0x0045,0xDB00);
	
	LCD_Reg_Set(0x0021, 0x0000);
	LCD_Command(0x0022);		
}


//weight
void DMA_LcdJpegInit(void)
{
	LCD_Reg_Set(0x0001,0x011B);
	
	LCD_Reg_Set(0x0002,0x0400); 
    LCD_Reg_Set(0x0003,0x1030); //6020

	LCD_Reg_Set(0x0004,0x0000);
	LCD_Reg_Set(0x0005,0x0000);
	//LCD_Reg_Set(0x0007,0x0004);
	LCD_Reg_Set(0x0008,0x0202);
	LCD_Reg_Set(0x0009,0x0000);
	LCD_Reg_Set(0x000B,0x0000);
	LCD_Reg_Set(0x000C,0x0003);
	
//	LCD_Reg_Set(0x0040,0x0000);
//	LCD_Reg_Set(0x0041,0x00ef); //00ef
//	LCD_Reg_Set(0x0042,0xDB00);
//	LCD_Reg_Set(0x0043,0xDB00);
//	
//	LCD_Reg_Set(0x0021,0x0000);
//	LCD_Reg_Set(0x0023,0x0000);
//	LCD_Reg_Set(0x0024,0x0000);
//	LCD_Reg_Set(0x0044,0xAF00);
//	LCD_Reg_Set(0x0045,0xDB00);
	
	LCD_Reg_Set(0x0021, 0x0000);
	LCD_Command(0x0022);	
}


          
//******************************************************************************


        
