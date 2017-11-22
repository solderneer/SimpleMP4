//******************************************************************************
#define     _IN_LCD_LTPS
//------------------------------------------------------------------------------
#include "Memap.h"
#include "Macro.h"
#include "Lcd.h"

#include "Byte2Word.h"

#define     DATA_BUS_WIDTH  8

#define     LCD_INIT_NORMAL     0
#define     LCD_MP4_INIT        1
#define     LCD_JPEG_INIT       2
#define     LCD_JPEG_X_INIT     3       

//------------------------------------------------------------------------------
void LCD_Command(unsigned int cmd)
{
#if (DATA_BUS_WIDTH==8)
    write_XDATA(LCD_COMMAND,(cmd>>8));
#endif
    write_XDATA(LCD_COMMAND,cmd);
}

void LCD_Data(unsigned int data)
{
#if (DATA_BUS_WIDTH==8)
    write_XDATA(LCD_DATA,(data>>8));
#endif    
    write_XDATA(LCD_DATA,data);
}

void LCD_Reg_Set(unsigned int cmd,unsigned int data)
{
#if (DATA_BUS_WIDTH==8)
    write_XDATA(LCD_COMMAND,(cmd>>8));
#endif
    write_XDATA(LCD_COMMAND,cmd);
    
#if (DATA_BUS_WIDTH==8)
    write_XDATA(LCD_DATA,(data>>8));
#endif    
    write_XDATA(LCD_DATA,data);
}

unsigned int LCD_StatusRead(void)
{
    unsigned int i,j;
    i = read_XDATA(LCD_COMMAND);
    return i;
}


void LCD_PowerOff(void)
{

}



#if 1
void LCDDEV_Init(unsigned int InitType)
{
    unsigned int x,y;
    
    LCD_Reg_Set(0x0007, 0x0233);

    LCD_Reg_Set(0x0000, 0x0001);

    LCD_Reg_Set(0x0010, 0x0012);

    LCD_Reg_Set(0x0011, 0x0014);
    LCD_Reg_Set(0x0012, 0x0509);
    LCD_Reg_Set(0x0013, 0x2B00);

    LCD_Reg_Set(0x001E, 0x0131);
    

    LCD_Reg_Set(0x0001, 0x2A9F);

    LCD_Reg_Set(0x0002, 0x0380);
    
    if(InitType == LCD_INIT_NORMAL)
        LCD_Reg_Set(0x0003, 0x6030);
    else if((InitType == LCD_JPEG_X_INIT)||(InitType == LCD_MP4_INIT))
        LCD_Reg_Set(0x0003, 0x6038);
    
    LCD_Reg_Set(0x0004, 0x0000);
    LCD_Reg_Set(0x0005, 0x0000);

    LCD_Reg_Set(0x0016, 0x7F00);
    LCD_Reg_Set(0x0017, 0x0101);

    LCD_Reg_Set(0x0007, 0x0233);
    
    LCD_Reg_Set(0x0040, 0x0001);
    LCD_Reg_Set(0x0041, 0x0000);
    LCD_Reg_Set(0x0042, 0x9F00);
    LCD_Reg_Set(0x0043, 0x9F00);
    LCD_Reg_Set(0x0044, 0x7F00);
    LCD_Reg_Set(0x0045, 0x9F00);

    LCD_Reg_Set(0x002C, 0x3000);

    LCD_Reg_Set(0x0030, 0x0000);
    LCD_Reg_Set(0x0031, 0x0000);
    LCD_Reg_Set(0x0032, 0x0000);
    LCD_Reg_Set(0x0033, 0x0000);
    LCD_Reg_Set(0x0034, 0x0000);
    LCD_Reg_Set(0x0035, 0x0000);
    LCD_Reg_Set(0x0036, 0x0000);
    LCD_Reg_Set(0x0037, 0x0000);
    LCD_Reg_Set(0x003A, 0x0000);
    LCD_Reg_Set(0x003B, 0x0000);
    
    LCD_Reg_Set(0x0021, 0x0000);
    LCD_Command(0x0022);
    for(y=0; y<LCD_MAX_YSIZE; y++){
		for(x=0; x<LCD_MAX_XSIZE; x++){
		    #if (DATA_BUS_WIDTH==8)
                write_XDATA(LCD_DATA,0x0000);
            #endif 
			    write_XDATA(LCD_DATA,0x0000);
		}
	}
}
#endif

#if 0
void LCDDEV_Init(unsigned int InitType)
{
    unsigned int x,y;
    
    LCD_Reg_Set(0x0007, 0x0233);

    LCD_Reg_Set(0x0000, 0x0001);

    LCD_Reg_Set(0x0010, 0x0012);

    LCD_Reg_Set(0x0011, 0x0014);
    LCD_Reg_Set(0x0012, 0x0509);
    LCD_Reg_Set(0x0013, 0x2B00);

    LCD_Reg_Set(0x001E, 0x0131);
    
    LCD_Reg_Set(0x0001, 0x2AA0);
    //LCD_Reg_Set(0x0001, 0x03AF);

    LCD_Reg_Set(0x0002, 0x0380);
    //LCD_Reg_Set(0x0002, 0x0000);

    //LCD_Reg_Set(0x0003, 0x6030);
    LCD_Reg_Set(0x0003, 0x6830);

    LCD_Reg_Set(0x0004, 0x0000);
    LCD_Reg_Set(0x0005, 0x0000);

    LCD_Reg_Set(0x0016, 0x7F00);
    LCD_Reg_Set(0x0017, 0x0101);

    LCD_Reg_Set(0x0007, 0x0233);
    
    LCD_Reg_Set(0x0040, 0x0001);
    LCD_Reg_Set(0x0041, 0x0000);
    LCD_Reg_Set(0x0042, 0x9F00);
    LCD_Reg_Set(0x0043, 0x9F00);
    LCD_Reg_Set(0x0044, 0x7F00);
    LCD_Reg_Set(0x0045, 0x9F00);

    LCD_Reg_Set(0x002C, 0x3000);

    LCD_Reg_Set(0x0030, 0x0000);
    LCD_Reg_Set(0x0031, 0x0000);
    LCD_Reg_Set(0x0032, 0x0000);
    LCD_Reg_Set(0x0033, 0x0000);
    LCD_Reg_Set(0x0034, 0x0000);
    LCD_Reg_Set(0x0035, 0x0000);
    LCD_Reg_Set(0x0036, 0x0000);
    LCD_Reg_Set(0x0037, 0x0000);
    LCD_Reg_Set(0x003A, 0x0000);
    LCD_Reg_Set(0x003B, 0x0000);
    
    LCD_Reg_Set(0x0021, 0x0000);
    LCD_Command(0x0022);
    for(x=0;x<128;x++){
		for(y=0;y<160;y++){
		    #if (DATA_BUS_WIDTH==8)
                write_XDATA(LCD_DATA,0x00F8);
            #endif 
			    write_XDATA(LCD_DATA,0xF800);
		}
	}
}
#endif

void LCD_Init(void)
{
    LCDDEV_Init(LCD_INIT_NORMAL);
}

void LCD_SetDispAddr(int x, int y)
{
    unsigned int gddram;
    gddram = ((y&0xFF)<<8) | (x&0x7F);
	LCD_Reg_Set(0x21, gddram);
	LCD_Command(0x22);
}

void Display_Sub(unsigned int RGB_Data)
{
    unsigned int  i,j;

    LCD_SetDispAddr(0,0);
    for(i = LCD_MAX_YSIZE; i>0; i--){
        for(j = LCD_MAX_XSIZE; j>0; j--)  
        #if (DATA_BUS_WIDTH==8)      
            write_XDATA(LCD_DATA,RGB_Data>>8);
        #endif
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

    LCD_SetDispAddr(0,0);
    for(i = LCD_MAX_YSIZE; i>0; i--){
        for(j = LCD_MAX_XSIZE; j>0; j--){
            #if (DATA_BUS_WIDTH==8)
           	    write_XDATA(LCD_DATA,*pBuffer>>8);
            #endif
                write_XDATA(LCD_DATA,*pBuffer++);
     	}
    }
}

void LCDDEV_SetWindow(int x0, int y0, int x1, int y1)
{
    unsigned int waddr;
    waddr = ((x1&0x7F)<<8) | (x0&0x7F);
	LCD_Reg_Set(0x44, waddr);
	
	waddr = ((y1&0xFF)<<8) | (y0&0xFF);
	LCD_Reg_Set(0x45, waddr);
	
	waddr = ((y0&0xFF)<<8) | (x0&0x7F);
	LCD_Reg_Set(0x21, waddr);
	
	LCD_Command(0x22);		
}

void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
    unsigned int waddr;
    waddr = ((x1&0x7F)<<8) | (x0&0x7F);
	LCD_Reg_Set(0x44, waddr);
	
	waddr = ((y1&0xFF)<<8) | (y0&0xFF);
	LCD_Reg_Set(0x45, waddr);
}

void LCD_ReadBitmap(int x0, int y0, int xsize, int ysize, unsigned int *pData)
{
	int x1, y1;
	x1 = x0+xsize-1; 
	y1 = y0+ysize-1; 
	LCDDEV_SetWindow(x0, y0, x1, y1);
    #if (DATA_BUS_WIDTH==8)
        read_XDATA(LCD_DATA);
    #endif 
	read_XDATA(LCD_DATA);
	for(y1=0; y1<ysize; y1++){
		for(x1=0; x1<xsize; x1++){
		    #if (DATA_BUS_WIDTH==8)
                *pData =  (read_XDATA(LCD_DATA)&0xFF)<<8;
                *pData |= read_XDATA(LCD_DATA)&0xFF;
            #else
                *pData++ = read_XDATA(LCD_DATA);
            #endif 
			
		}
	}
		
	LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
}


unsigned int LCD_GetPixel(int x, int y)
{
	unsigned int color;
	
	LCD_SetDispAddr(x,y);
	#if (DATA_BUS_WIDTH==8)
        color =  read_XDATA(LCD_DATA);
	    color =  read_XDATA(LCD_DATA);
	    color =  (read_XDATA(LCD_DATA)&0xFF)<<8;
	    color |= (read_XDATA(LCD_DATA)&0xFF);
    #else	
	    color = read_XDATA(LCD_DATA);
	    color = read_XDATA(LCD_DATA);
	#endif
	return(color);	
}

void LCD_SetPixel(int x, int y, unsigned int color)
{
	LCD_SetDispAddr(x,y);
#if (DATA_BUS_WIDTH==8)
    write_XDATA(LCD_DATA,(color>>8));
#endif    
    write_XDATA(LCD_DATA,color);	
	//write_XDATA(LCD_DATA,color);
}

void LCD_XorPixel(int x, int y)
{
	unsigned int color;
	color = LCD_GetPixel(x,y);
	LCD_SetPixel(x,y,0xFFFF-color);			
}

unsigned int LCDDEV_GetPixel(int x, int y)
{
	unsigned int color;
	
	LCD_SetDispAddr(x, y);
	#if (DATA_BUS_WIDTH==8)
        color =  read_XDATA(LCD_DATA);
	    color =  read_XDATA(LCD_DATA);
	    color =  (read_XDATA(LCD_DATA)&0xFF)<<8;
	    color |= (read_XDATA(LCD_DATA)&0xFF);
    #else	
	    color = read_XDATA(LCD_DATA);
	    color = read_XDATA(LCD_DATA);
	#endif
	return(color);	
}
void LCDDEV_SetPixel(int x, int y, unsigned int color)
{
	LCD_SetDispAddr(x, y);
#if (DATA_BUS_WIDTH==8)
    write_XDATA(LCD_DATA,(color>>8));
#endif    
    write_XDATA(LCD_DATA,color);	
	//write_XDATA(LCD_DATA,color);
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
		    #if (DATA_BUS_WIDTH==8)
		        write_XDATA(LCD_DATA, (LCD_COLOR>>8));
		    #endif
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
		    #if (DATA_BUS_WIDTH==8)
		        write_XDATA(LCD_DATA, (color>>8));
		    #endif
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
			    #if (DATA_BUS_WIDTH==8)
		            write_XDATA(LCD_DATA, (color>>8));
		        #endif
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
		    #if (DATA_BUS_WIDTH==8)
		        write_XDATA(LCD_DATA, (*pData>>8));
		    #endif
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
       		LCDDEV_DrawBitLine16BPP(x0, y0, xsize, ysize, pData);
       	break;
   	}
}

void MP4_LCD_Init(void)
{
    unsigned int x, y;
    LCD_SetDispAddr(0, 0);
    for(y=0; y<LCD_MAX_YSIZE; y++){
        for(x=0; x<LCD_MAX_XSIZE; x++){
            #if (DATA_BUS_WIDTH==8)
                write_XDATA(LCD_DATA, 0x0000);
            #endif
                write_XDATA(LCD_DATA, 0x0000);
        }
    }
    LCDDEV_Init(LCD_MP4_INIT);
}

void DMA_LcdJpegInit(void)
{
    unsigned int x, y;
    LCD_SetDispAddr(0, 0);
    for(y=0; y<LCD_MAX_YSIZE; y++){
        for(x=0; x<LCD_MAX_XSIZE; x++){
            #if (DATA_BUS_WIDTH==8)
                write_XDATA(LCD_DATA, 0x0000);
            #endif
                write_XDATA(LCD_DATA, 0x0000);
        }
    }
    LCDDEV_Init(LCD_INIT_NORMAL);
}

void DMA_LcdJpegInitX(void)
{
    unsigned int x, y;
    LCD_SetDispAddr(0, 0);
    for(y=0; y<LCD_MAX_YSIZE; y++){
        for(x=0; x<LCD_MAX_XSIZE; x++){
            #if (DATA_BUS_WIDTH==8)
                write_XDATA(LCD_DATA, 0x0000);
            #endif
                write_XDATA(LCD_DATA, 0x0000);
        }
    }
    LCDDEV_Init(LCD_JPEG_X_INIT);
}

//******************************************************************************
