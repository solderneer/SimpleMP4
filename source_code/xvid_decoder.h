/*****************************************************************************
 *
 *  XVID MPEG-4@QVGA VIDEO DECODER ON ROCKCHIP RK2606
 *	
 *	Author:
 *		Jian Huan	<jh@rock-chips.com>
 *
 *	Date:
 *		22005-11-24 16:35
 ****************************************************************************/

#ifndef _DECODER_H_
#define _DECODER_H_

//#include <stdio.h>
#include "xvid_macro.h"


#ifdef  _IN_XVID_DECODER
#undef  EXT
#define EXT
#else
#define EXT extern
#endif


/****************************************************************************
	                            Memory Struct
****************************************************************************/

typedef struct
{
    short           y_page;   
	unsigned short	y_addr;
	short			uv_page;
	unsigned short	uv_addr;
}
IMAGE;
#if 0
EXT IMAGE Frame1, Frame2 __attribute__((section(".avi_v_bss, \"aw\"")));
EXT IMAGE RGBFrame1, RGBFrame2 __attribute__((section(".avi_v_bss, \"aw\"")));
#else
//该变量的空间应该分配到AVI模块中，上面的定义是有问题的
EXT IMAGE Frame1 __attribute__((section(".avi_v_bss, \"aw\"")));
EXT IMAGE Frame2 __attribute__((section(".avi_v_bss, \"aw\"")));
EXT IMAGE RGBFrame1 __attribute__((section(".avi_v_bss, \"aw\"")));
EXT IMAGE RGBFrame2 __attribute__((section(".avi_v_bss, \"aw\"")));
#endif

typedef struct
{
	IMAGE   *frm1;
	IMAGE   *frm2;					
}
EX_MEM;
EXT EX_MEM  ExtMem __attribute__((section(".avi_v_bss, \"aw\"")));
EXT EX_MEM  RGBMem __attribute__((section(".avi_v_bss, \"aw\"")));


typedef struct
{
	unsigned short    *mb_y_row;
	unsigned short    *mb_uv_row;
	short			   turn;
}
IN_MEM;
EXT IN_MEM	IntMem __attribute__((section(".avi_v_bss, \"aw\"")));


/*****************************************************************************
 *			STRUCT
 ****************************************************************************/
typedef struct
{
	short			pos;
	unsigned long   bufa;
	unsigned long	bufb;
	unsigned long	*start;
	unsigned long	*tail;
	short			exchange;
	short			file_end;
}
BITSTREAM;
EXT BITSTREAM  Bss __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
	short   x;
	short	y;
}
VECTOR;
EXT VECTOR  pmv                 __attribute__((section(".avi_v_bss, \"aw\"")));
EXT VECTOR  uv_dx_dy            __attribute__((section(".avi_v_bss, \"aw\"")));
EXT VECTOR  matching_cord[5]    __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
	VECTOR		mvs[4];
	short		pred_values[6][MBPRED_SIZE];
	short		acpred_directions[6];
	short		mode;
	short		quant;					/* absolute quant */
}
MACROBLOCK;
EXT MACROBLOCK  MacBlk[22]   __attribute__((section(".avi_v_data, \"aw\"")));

typedef struct
{
	short		cbp;
	short		quant;
	short		rounding;
	short		fcode_forward;
	short		intra_dc_threshold;
	short		acpred_flag;
}
DECCOEFF;
EXT DECCOEFF    Xvidecoef   __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
	short     *left;
	short     *top;
	short     *diag;
	short     *current;
}
PREDICTOR;
EXT PREDICTOR   Xvidictor   __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
	short		width;
	short		height;
	short		mb_width;
	short		mb_height;
	IMAGE		*cur;
	IMAGE		*ref;
}
DECODER;
EXT DECODER Xvidec      __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
	short		x;
	short		y;
	short		mbindex;
}
POSITION;
EXT POSITION    Xvidpos __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
	short		row;
	short		column;
}Stat_Idct_Coeff;
EXT Stat_Idct_Coeff  SIC    __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
	short		hpixel_h;
	short		hpixel_v;
}Stat_Half_Pixel;
EXT Stat_Half_Pixel		SHP __attribute__((section(".avi_v_bss, \"aw\"")));
EXT Stat_Half_Pixel		SHP_UV __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct
{
    short   hor_in;
    short   vert_in;
    short   hor_out;
    short   vert_out;
    short   width;
    short   height;
    
}PIXEL_COMP_STRUCT;
EXT PIXEL_COMP_STRUCT   PCS __attribute__((section(".avi_v_bss, \"aw\"")));
EXT PIXEL_COMP_STRUCT   PCS_UV __attribute__((section(".avi_v_bss, \"aw\"")));

typedef struct{
    short   left_quant;
    short   top_quant;
}
LEFT_TOP_QUANT_STRUCT;
EXT LEFT_TOP_QUANT_STRUCT LTQ   __attribute__((section(".avi_v_bss, \"aw\"")));

/*****************************************************************************
								TEMP VARIABLE
 ****************************************************************************/
EXT short   time_inc_bits       __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long    src_offset_pos      __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned short Global_Offset __attribute__((section(".avi_v_bss, \"aw\"")));
/*****************************************************************************
							 DECODER INITIALIZATION
 ****************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
void Mp4_Memory_Init(void);

__attribute__((section(".avi_v_text, \"ax\"")))
void mp4_internal_mem_init( );

__attribute__((section(".avi_v_text, \"ax\"")))
void mp4_external_mem_init( );


__attribute__((section(".avi_v_text, \"ax\"")))
void Mp4_Dec_Init(unsigned short *bufptr);
__attribute__((section(".avi_v_text, \"ax\"")))
void mp4_dec_struct_init();
__attribute__((section(".avi_v_text, \"ax\"")))
void BufferPtrInit(BITSTREAM * bs,unsigned short *bufptr);
/*****************************************************************************
							 DECODER PROTOTYPE
 ****************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
short 
MP4_DECODE_ROUTINE();

__attribute__((section(".avi_v_text, \"ax\"")))
short	
decoder_iframe(BITSTREAM * ,DECODER * , DECCOEFF  * );

__attribute__((section(".avi_v_text, \"ax\"")))
short 
decoder_1mv_mbinter(DECODER * , MACROBLOCK * , DECCOEFF * );    

__attribute__((section(".avi_v_text, \"ax\"")))                                                                           
short 
decoder_4mv_mbinter(DECODER * , MACROBLOCK * , DECCOEFF * );

__attribute__((section(".avi_v_text, \"ax\""))) 
short
mbindex_div(short , short );
/*****************************************************************************
					READ MP4 BITSTREAM FROM PERIPHERAL
*****************************************************************************/

__attribute__((section(".avi_v_text, \"ax\"")))
short 
ReadBitsFromPeripheral(unsigned short *bufferptr, short ReadSize);

/****************************************************************************
                    Color Conversion
*****************************************************************************/

EXT unsigned short *pRGBOutBuffer __attribute__((section(".avi_v_bss, \"aw\"")));
 
 
void BITS_N_FRAME(unsigned int *A,unsigned int volatile B); 
void BITC_N_FRAME(unsigned int *A,unsigned int volatile B); 
int BITT_N_FRAME(unsigned int *A,unsigned int volatile B);


#endif
