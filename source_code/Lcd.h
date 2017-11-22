//******************************************************************************
#ifndef     _LCD_H
#define     _LCD_H
//------------------------------------------------------------------------------
#define		LCD_MAX_YSIZE		240
#define		LCD_MAX_XSIZE		320
#define     DATA_BUS_WIDTH      16

#define     LCD_ASPECT_0        0
#define     LCD_ASPECT_90       90
#define     LCD_ASPECT_180      180
#define     LCD_ASPECT_270      270

#define     LCD_SWITCH_XY   

#define     LCD_MEMORY_DEV    

//------------------------------------------------------------------------------
void LCD_Command(unsigned int cmd);
void LCD_Data(unsigned int data);
void LCD_Reg_Set(unsigned int cmd, unsigned int data);
void LCD_Init(unsigned int type) ;
void LCD_SetDispAddr(unsigned int x,unsigned int y);
void LCDDEV_SetWindow(int x0, int y0, int x1, int y1);
void LCDDEV_RevertWindow(int x0, int y0, int x1, int y1);
unsigned int LCDDEV_GetPixel(int x, int y);
void LCDDEV_SetPixel(int x, int y, unsigned int color);
void LCDDEV_XorPixel(int x, int y);
void Display_off(void);
//------------------------------------------------------------------------------

void LCDDEV_DrawBitLine16BPP(int x0, int y0, int xsize, int ysize, unsigned int *pData); 
void LCDDEV_DrawHLine(int x0, int y, int x1);
void LCDDEV_DrawVLine(int x, int y0, int y1);
void LCDDEV_FillRect(int x0, int y0, int x1, int y1);
void LCDDEV_DrawBitmap(int x0,    		int y0,
                       int xsize, 		int ysize,
                       int BitsPerPixel,	int BytesPerLine,
                       unsigned int *pData, 		int Diff);



void LCD_FillRectImage(int x, int y, int xsize, int ysize, unsigned long Flash_Addr);
void LCD_DrawBitmap(int x0, int y0, int xsize, int ysize,
                    int BitsPerPixel, int BytesPerLine, unsigned int *pPixel);
void LCD_DrawHLine(int x0, int y, int x1);
void LCD_DrawPixel(int x, int y);
void LCD_DrawVLine(int x, int y0, int y1);
void LCD_FillRect(int x0, int y0, int x1, int y1);
unsigned int LCD_GetBkColor(void);
unsigned int LCD_GetColor(void);
void LCD_SetBkColor(unsigned int color);
void LCD_SetColor(unsigned int color);
unsigned int LCD_SetDrawMode(unsigned int mode);
unsigned int LCD_SetTextMode(unsigned int mode);
void LcdInit(void);
void Display_Sub(unsigned int RGB_Data, short i);
void DisplayPicture(int x, int y, int xsize, int ysize, unsigned long Flash_Addr);
//------------------------------------------------------------------------------
#ifdef LCD_MEMORY_DEV
    void LCDDEV_DrawMem(int x0,    		int y0,
                    int xsize, 		int ysize,
                    int BitsPerPixel,	int BytesPerLine,
                    unsigned int *pData); 
    void LCDDEV_ReadMem(int x0, int y0, int xsize, int ysize, unsigned int *pData);
    void LCDDEV_DrawChar2Mem(int x0, 
                            int y0, 
                            int xsize, 
                            int ysize,
                            int Diff, 
                            int BytesPerLine,
                            unsigned int *pData); 
    void LCDDEV_DrawBitLine1BPP(int x, int y, int xsize, int Diff, unsigned int *pLcdData, unsigned int *pData);    
    void DmaFillImage2Lcd(void);

#else
    void LCDDEV_DrawBitLine1BPP(int x, int y, int xsize, int Diff, unsigned int *pData); 
#endif
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
typedef	void LCD_DISPCHAR(unsigned int c);

typedef struct LCD_COLORStruct{
	unsigned int BColor;
	unsigned int FColor;
}LCD_COLOR_STRUCT;

typedef struct LcdPonitStruct{
	unsigned int x;
	unsigned int y;
}LCD_POINT;

typedef	struct LcdRectStruct{
	int x0; 
	int y0; 
	int x1; 
	int y1;
}LCD_RECT;

typedef struct LcdFontStruct{
	LCD_DISPCHAR 	*pDispChar;
	unsigned int			*pFontData;
	unsigned int			FirstChar;
	unsigned int			LastChar;
	unsigned char			XSize;
	unsigned char			YSize;
	unsigned char			BytesPerLine;	
}LCD_FONT_STRUCT;

typedef struct LCD_ContextStruct{
    int          LcdMaxXsize, LcdMaxYsize;
    int          Mode;
	unsigned int				Color,BkColor;
	LCD_RECT			ClipRect;
	unsigned int				DrawMode;
	//LCD_FONT_STRUCT		*pFont;
	int				DispPosX;
	int				DispPosY;
	int				XDist, YDist;
	int				LBorder;
	int				RBorder;
	unsigned int 				TextMode;
	unsigned int				TextAlign; 
}LCD_CONTEXT_STRUCT;
//------------------------------------------------------------------------------
LCD_CONTEXT_STRUCT	LCD_Context;

#define		LCD_COLOR		LCD_Context.Color
#define		LCD_BKCOLOR		LCD_Context.BkColor

#define LCD_DRAWMODE_NORMAL (0)
#define LCD_DRAWMODE_XOR    (1<<0)
#define LCD_DRAWMODE_TRANS  (1<<1)
#define LCD_DRAWMODE_REV    (1<<2)
#define LCD_DRAWMODE_CIR90  (1<<3)

#define	LCD_TEXTALIGN_HORIZONTAL	0x0003
#define	LCD_TEXTALIGN_LEFT			0x0000
#define	LCD_TEXTALIGN_RIGHT			0x0001
#define	LCD_TEXTALIGN_CENTER		0x0002
#define	LCD_TEXTALIGN_ANL			0x8000

#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_YELLOW    (COLOR_RED | COLOR_GREEN)
#define COLOR_PURPLE    (COLOR_RED | COLOR_BLUE)
#define COLOR_CYAN      (COLOR_GREEN | COLOR_BLUE);

extern LCD_FONT_STRUCT FontMono8x16;
//------------------------------------------------------------------------------
#endif
//******************************************************************************
