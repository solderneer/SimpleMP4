/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : JpegDecodeStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _JPEGDECODESTRUCT_H
#define _JPEGDECODESTRUCT_H

#include "JpegDecodeMacro.h"

//------------------------------------------------------------------------------
typedef union
{                             
    struct
    {
    unsigned int
        bJpgBufferFull:     1,
		bJpgBufferEmpty:    1,
	    bJpgBufferIndex:    1,
                                                    
		bRGBBufferFull:     1,
		bRGBBufferEmpty:    1,
		bRGBBufferIndex:    1,
		                                            
		bJpgFileEnd:        1,
                    
		bReserved:          9;
    }bc;
    unsigned int  byte;
} JPEG_FLAG;

typedef struct
{
	unsigned long words_buffer;
	        short tem_flag;
	short		in_buf_left;
	unsigned int	*tail;
	short			ptrj;
	unsigned int	*start;
}JPEG_BITSTREAM;

typedef struct
{
	short	bits_left;
	unsigned long	bit_buf;
}JPEG_BITS_USE;

typedef struct
{
	unsigned short	look_up[256];
	unsigned short	code_size[256];
	unsigned short	tree[512];
} JPEG_HUFF_TABLES_T, *JPEG_PHUFF_TABLES_T;

typedef struct
{
    short  hor_ver_length;
    short  length ;
}JPEG_POST_MARKER;

typedef struct
{ 
    short	image_x_size;
    short	image_y_size;
    short	max_mcu_x_size;
    short	max_mcu_y_size;
    short	dst_x_size;
    short	dst_y_size;
    short	max_mcu_x_size2;
    short	max_mcu_y_size2;
    short	dst_x_size_expand;
    short	dst_y_size_expand;
    short	block_size;
    short	idct_size;
    short   image_dx_size;
    short	image_dy_size;
    short   deverse; 
    short   dst_x_size_expand_postprocess ;
}IMAGE_SIZE;

typedef struct
{
	int restarts_left;
	int restart_interval;
	int next_restart_num;
}RESTART_DC;

typedef struct
{
	short	mcus_per_row;
	short	blocks_per_mcu;
	short	mcu_org[JPGD_MAXBLOCKSPERMCU];
}DEOCDE_NEXT_HQ;

typedef struct
{
     short	out_lines ;
     short  had_lines ;
}SCALE_P; 


//------------------------------------------------------------------------------
#endif
//******************************************************************************
