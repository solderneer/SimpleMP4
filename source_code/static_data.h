/*****************************************************************************
 *
 *  XVID MPEG-4@QVGA VIDEO DECODER ON ROCKCHIP RK2606
 *	
 *	Author:
 *		Jian Huan	<jh@rock-chips.com>
 *
 *	Date:
 *		2005-06-15
 ****************************************************************************/

#ifndef _STATIC_DATA_H_
#define _STATIC_DATA_H_

#include "xvid_macro.h"
#include "xvid_section_def.h"

#ifdef  _IN_XVID_DECODER
#undef  EXT
#define EXT
#else
#define EXT extern
#endif


/************************************************************************
 *	Static Allocate 4 Y block buffers, 1 U block buffer and 1 V block buffer 
 *	diff_block[4][64], diff_uv_block[2][64]
************************************************************************/
extern short   diff_block[4][64] MP4_C_ARRAY_SECTION;
extern short   diff_uv_block[2][64] MP4_C_ARRAY_SECTION;

/************************************************************************
 *	Static Allocate 1 Y matching block buffer, 1 U/V matching block buffer 
 *	match_y_block[18*17], match_uv_block[18*9]
************************************************************************/
extern unsigned short	match_y_block[18*17] MP4_C_ARRAY_SECTION;       //[ZZ]
extern unsigned short	match_uv_block[18*9]  MP4_C_ARRAY_SECTION;

/**********************************************************************/
EXT short   predictors[8] MP4_C_ARRAY_SECTION;
EXT unsigned short *PYUV_Cur_Array[6] MP4_C_ARRAY_SECTION;
/************************************************************************
	Internal Temp Memory 
	which is used to store frame_width*16*1.5 pixels
 ************************************************************************/
extern unsigned short  mb_row_mem[INT_MEM_SIZE] MP4_C_ARRAY_SECTION;

/************************************************************************
    RGB buffer
 ************************************************************************/
extern unsigned short RGBOutBuffer[ImgX*16] MP4_C_ARRAY_SECTION;  

#endif

