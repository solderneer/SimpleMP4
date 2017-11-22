#ifndef __BMPDECODER_H_
#define __BMPDECODER_H_

//#include "stdio.h"

#define	DWORD	long;
#define	LONG	long;
#define	WORD	short;


#define SCREENWIDTH		320
#define SCREENHEIGHT	240
#define SCANLEDOWN		32
//#define MASTBITS		(SCANLEDOWN-1)
#define MASTBITS		31


typedef struct tagRGBQUAD {
        short    rgbBlue;
        short    rgbGreen;
        short    rgbRed;
        short    rgbReserved;
} RGBQUAD;


typedef struct tagBITMAPINFOHEADER{
        long      biSize;
        long       biWidth;
        long       biHeight;
        short       biPlanes;
        short       biBitCount;
        long      biCompression;
        long      biSizeImage;
        long       biXPelsPerMeter;
        long       biYPelsPerMeter;
        long      biClrUsed;
        long      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
        short    bfType;
        long   bfSize;
        short    bfReserved1;
        short    bfReserved2;
        long   bfOffBits;
} BITMAPFILEHEADER;

typedef	struct{
	short screw;
	short doscale;
	short zoomtofit;
	long	width;
	long	height;
	short renverse;
	short deswidth;
	short desheight;
	long 	expendwidth;
	long	expendheight;
	short	sampling;
	short sampwidth;
	short sampheight;
	
	short samplong;
	short  sampshort;
	short tocut;		// 0 = shortsize,1 = longsize
}Imageinfo;



#define	MAX_LINES	3

BITMAPFILEHEADER	BmpFileHead __attribute__((section(".bmp_bss, \"aw\"")));
BITMAPINFOHEADER	BmpInfoHead __attribute__((section(".bmp_bss, \"aw\"")));
	
volatile short	BMP2LCD __attribute__((section(".bmp_bss, \"aw\"")));
Imageinfo       image   __attribute__((section(".bmp_bss, \"aw\"")));

signed char		hhMainFile __attribute__((section(".bmp_bss, \"aw\"")));

//unsigned short	infilebuf[2560*3];
//unsigned short	index;
unsigned short	RGB_index[256][3] __attribute__((section(".bmp_bss, \"aw\"")));
unsigned short	MASK_BITS[3]      __attribute__((section(".bmp_bss, \"aw\"")));
short	        Bitsns[3]       __attribute__((section(".bmp_bss, \"aw\"")));

unsigned short	RGB_Buffer[3*SCREENWIDTH*2] __attribute__((section(".bmp_bss, \"aw\"")));

unsigned short  obj_line[SCREENWIDTH*3] __attribute__((section(".bmp_bss, \"aw\"")));

unsigned short  one_line[SCREENWIDTH] __attribute__((section(".bmp_bss, \"aw\"")));

unsigned short	rgb_temp_buff[MAX_LINES+1][SCREENWIDTH*3] __attribute__((section(".bmp_bss, \"aw\"")));



#endif
