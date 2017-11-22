//******************************************************************************
#define     _IN_LCD_LTPS
//------------------------------------------------------------------------------
#include "Memap.h"
#include "Macro.h"
#include "Lcd.h"

#include "Byte2Word.h"

//#define CSTN_DELAY_TMER

#define     DATA_BUS_WIDTH  8

#define     LCD_INIT_NORMAL     0
#define     LCD_MP4_INIT        1
#define     LCD_JPEG_INIT       2
#define     LCD_JPEG_X_INIT     3      


#define OSCILLATION_MODE_SET            0x02
#define DRIVER_OUTPUT_MODE_SET          0x10
#define DC_DC_SET                       0x20
#define CURRENT_BIAS_SET                0x22
#define PCK_GENER_MODE_SET              0x24 // = DCDC Clock division set
#define DCDC_AMP_ON_OFF_SET             0x26
#define TEMP_COMPENSATION_SET           0x28
#define CONTRAST_CONTROL1               0x2A
#define CONTRAST_CONTROL2               0x2B
#define STANDBY_MODE_OFF                0x2C
#define STANDBY_MODE_ON                 0x2D
#define DDRAM_BURST_MODE_OFF            0x2E
#define DDRAM_BURST_MODE_ON             0x2F
#define ADDRESSING_MODE_SET             0x30
#define ROW_VECTOR_MODE_SET             0x32
#define N_LINE_INVERSION_SET            0x34 // = N-Block Inversion
#define FRAME_FREQUENCY_CONTROL_SET     0x36
#define ENTRY_MODE_SET                  0x40
#define X_ADDRESS_AREA_SET              0x42
#define Y_ADDRESS_AREA_SET              0x43
#define RAM_SKIP_AREA_SET               0x45
#define DISPLAY_OFF                     0x50
#define DISPLAY_ON                      0x51
#define SPEC_DISPLAY_PATTERN_SET        0x53
#define PARTIAL_DISPLAY_MODE_SET        0x55
#define PARTIAL_DISPLAY_START_LINE_SET  0x56
#define PARTIAL_DISPLAY_END_LINE_SET    0x57
#define AREA_SCROLL_MODE_SET            0x59
#define SCROLL_START_LINE_SET           0x5A
#define SET_DISPLAY_DATA_LENGTH         0xFC 

void LcdDlyTmr(unsigned int cnt)
{
    unsigned int i;
    unsigned int j;
    while(cnt-->0)
    {
        i = 0x6000;
        while(i-->0){
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }
    }
}


//------------------------------------------------------------------------------
void LCD_Command(unsigned int cmd)
{
//#if (DATA_BUS_WIDTH==8)
//    write_XDATA(LCD_COMMAND,(cmd>>8));
//#endif
    write_XDATA(LCD_COMMAND,cmd);
}

void LCD_Data(unsigned int data)
{
#if (DATA_BUS_WIDTH==8)
    write_XDATA(LCD_DATA,(data>>8));
#endif    
    write_XDATA(LCD_DATA,data);
}

void LCDDEV_Reg_Set(unsigned int cmd,unsigned int data)
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
    
    LCD_Command(STANDBY_MODE_OFF);      // Standby mode off  
    
    // Select internal or external OSC clock and OSC On/Off control
    LCD_Command(OSCILLATION_MODE_SET);  
    LCD_Command(0x01);                  // Use internal OSC and OSC On  
    
    // Select the first booster's boosting step for V1 generation
    LCD_Command(DC_DC_SET);  
    LCD_Command(0x05);                  // X1.5 step for normal mode(with partial mode 0)
                                        // and partial mode 1
                                        
    // Control the 1'st booster, V1 AMP, 2'nd booster and 3'rd booster
    LCD_Command(DCDC_AMP_ON_OFF_SET);  
    LCD_Command(0x01);                  // 1'st booster On
    
    LcdDlyTmr(25);
    
    LCD_Command(DCDC_AMP_ON_OFF_SET);  
    LCD_Command(0x09);                  // 1'st booster and V1 AMP On
    
    LcdDlyTmr(25);
    
    LCD_Command(DCDC_AMP_ON_OFF_SET);  
    LCD_Command(0x0B);                  // 1'st booster, V1 AMP and 2'nd booster On
    
    LcdDlyTmr(25);
    
    LCD_Command(DCDC_AMP_ON_OFF_SET);  
    LCD_Command(0x0F);                  // 1'st booster, V1 AMP, 2'nd booster and
                                        // 3'rd booster On
    
    LcdDlyTmr(25);
    
    LCD_Command(TEMP_COMPENSATION_SET);
    LCD_Command(0x00);                  // 0.00 % / ℃

    LCD_Command(DDRAM_BURST_MODE_OFF);  // Burst mode off for data RAM write
    
    // RAM address skip area setting
    LCD_Command(RAM_SKIP_AREA_SET);
    LCD_Command(0x00);                  // Set to No skip
    
    // Specified display pattern select
    LCD_Command(SPEC_DISPLAY_PATTERN_SET);
    LCD_Command(0x00);                  // Normal display.
    
    // Select number of display line, Segment direction and Red/Blue output swap
    if(InitType != LCD_INIT_NORMAL){
        LCD_Command(DRIVER_OUTPUT_MODE_SET);  
        LCD_Command(0x24);                  // 1/162 duty and Red/Blue output swap
    }else{
        LCD_Command(DRIVER_OUTPUT_MODE_SET);  
        LCD_Command(0x20);                  // 1/162 duty and Red/Blue output swap
    }
    
    // DCDC Clock division set
    LCD_Command(PCK_GENER_MODE_SET);  
    LCD_Command(0x11);                  // fosc/8 division for normal(with partial mode 0)
                                        // and partial mode 1
    
    // Addressing mode set
    LCD_Command(ADDRESSING_MODE_SET);
    LCD_Command(0x1C);                  // 65K color mode, Dummy sub group off,
                                        // sub group frame inversion on,
                                        // sub group inversion off,
                                        // sub group phase change every 2 pixel unit.
    
    // Row vector mode set
    LCD_Command(ROW_VECTOR_MODE_SET);
    LCD_Command(0x0E);                  // Row vector increment period : Eery sub-frame
    //LCD_Command(0x00);                                    // Row vector sequence : R1→R2→R3→R4→R1...
    
    // Entry mode set
    if(InitType != LCD_INIT_NORMAL){
        LCD_Command(ENTRY_MODE_SET);
        LCD_Command(0x02);                  // Non-Reverse, Y-direction prefer, Read modify off
    }else{
        LCD_Command(ENTRY_MODE_SET);
        LCD_Command(0x00);                  // Non-Reverse, Y-direction prefer, Read modify off
    }
    
    // Y-address area set(for segment direction)
    LCD_Command(Y_ADDRESS_AREA_SET);
    LCD_Command(0x02);
    LCD_Command(0x81);
    
    // X-address area set(for common direction)
    LCD_Command(X_ADDRESS_AREA_SET);
    LCD_Command(0x01);
    LCD_Command(0xA0);
    
    // N-line inversion set
    LCD_Command(N_LINE_INVERSION_SET);  
    LCD_Command(0x0E);                  // FIM=off, FIP=off, N-BLK=13
    
    // Frame frequency control
    LCD_Command(FRAME_FREQUENCY_CONTROL_SET);
    LCD_Command(00);
    
    // Contrast control for partial display mode 1
    LCD_Command(CONTRAST_CONTROL2);
    LCD_Command(0x20);
    
    // Contrast control for normal display and partial display mode 0
    LCD_Command(CONTRAST_CONTROL1);
    LCD_Command(0xBF);
    
    // Driving current and bias set
    LCD_Command(CURRENT_BIAS_SET);
    LCD_Command(0x11);                  // Normal current driving mode and 1/5 bias for
                                        // all display mode
    
    // Partial display mode set
    LCD_Command(PARTIAL_DISPLAY_MODE_SET);
    LCD_Command(0x00);
    
    // Display start line set for all partial display mode
    LCD_Command(PARTIAL_DISPLAY_START_LINE_SET);
    LCD_Command(0x01);
    
    // Display end line set for all partial display mode
    LCD_Command(PARTIAL_DISPLAY_END_LINE_SET);
    LCD_Command(0xA0);
    
    // Area scroll set
    //LCD_Command(AREA_SCROLL_MODE_SET);
    //LCD_Command(0x00); // Area scroll mode off(Entire display)
    //LCD_Command(0x00); // Scroll area start line
    //LCD_Command(0x9F); // Scroll area end line
    //LCD_Command(0x00); // Lower fixed number
    // Scroll start line set when use area scroll set
    //LCD_Command(SCROLL_START_LINE_SET);
    //LCD_Command(0x00);
    // Write initial display data
    //outpw(HS_LCD_CTRL_REG, lcd_init_image[128*i+j]);
    /*
    for(y=0; y<LCD_MAX_YSIZE; y++){
        for(x=0; x<LCD_MAX_XSIZE; x++){
            #if (DATA_BUS_WIDTH==8)
                write_XDATA(LCD_DATA,0x00F8);
            #endif 
			    write_XDATA(LCD_DATA,0xF800);    
        }
    }
    LcdDlyTmr(100);*/
    //Display On
    LCD_Command(DISPLAY_ON);

}
#endif

void LCD_Init(void)
{
    unsigned int x, y;
    LCDDEV_Init(LCD_INIT_NORMAL);
    //LcdDlyTmr(400);
    for(x=0;x<128;x++){
	    for(y=0;y<160;y++){
		    #if (DATA_BUS_WIDTH==8)
                write_XDATA(LCD_DATA,0x0000);
            #endif 
			    write_XDATA(LCD_DATA,0x0000);
		}
	}
	#ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
}

void LCD_SetDispAddr(int x, int y)
{
    LCD_Command(0x43);
    LCD_Command(x+2);             //与其它LCD不一致
    LCD_Command(LCD_MAX_XSIZE+1);
    
    LCD_Command(0x42);
    LCD_Command(y+1);
    LCD_Command(LCD_MAX_YSIZE);
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
    #ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
}

void Display_Image(unsigned int *pBuffer)
{
    unsigned int i,j,temp,temp_data;

    LCDDEV_Reg_Set(0X14, 0x00);
    LCDDEV_Reg_Set(0X15, 0x00);
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
    #ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
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
    #ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
}

void LCDDEV_SetWindow(int x0, int y0, int x1, int y1)
{
    //LCD_Command(0x55);		
    //LCD_Command(0x01);
    
    LCD_Command(0x56);
    LCD_Command(y0+1);
    
    LCD_Command(0x57);
    LCD_Command(y1+1);
     
    LCD_Command(0x43);
    LCD_Command(x0+2);
    LCD_Command(x1+2);
    
    LCD_Command(0x43);
    LCD_Command(x0+2);             //与其它LCD不一致
    LCD_Command(x1+2);
    
    LCD_Command(0x42);
    LCD_Command(y0+1);
    LCD_Command(y1+1);
}

void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1)
{
    //LCD_Command(0x55);		
    //LCD_Command(0x00);
    
    LCD_Command(0x56);
    LCD_Command(y0+1);
    
    LCD_Command(0x57);
    LCD_Command(y1+1);
     
    LCD_Command(0x43);
    LCD_Command(x0+2);
    LCD_Command(x1+2);
    
    LCD_SetDispAddr(0,0);
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
	#ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
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
	//#ifdef CSTN_DELAY_TMER
    //    LcdDlyTmr(100);
    //#endif
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
	//#ifdef CSTN_DELAY_TMER
    //    LcdDlyTmr(100);
    //#endif
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
       		LCDDEV_SetWindow(x0, y0, x0+xsize-1, y0+ysize-1);
       		for(; ysize > 0; ysize--){
       		    LCDDEV_DrawBitLine16BPP(x0, y0++, xsize, 1, pData);
       		    pData += BytesPerLine;
       		} 
       		LCDDEV_RevertWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
       	break;
   	}
    //#ifdef CSTN_DELAY_TMER
    //    LcdDlyTmr(100);
    //#endif
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
    #ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
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
    #ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
    LCDDEV_Init(LCD_INIT_NORMAL);
    LCD_SetDispAddr(0, 0);
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
    #ifdef CSTN_DELAY_TMER
        LcdDlyTmr(100);
    #endif
    LCDDEV_Init(LCD_JPEG_X_INIT);
    LCD_SetDispAddr(0, 0);
}

//******************************************************************************
