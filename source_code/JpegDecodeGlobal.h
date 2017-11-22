/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : JpegDecodeGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _JPEGDECODEGLOBAL_H
#define _JPEGDECODEGLOBAL_H

#include "JpegDecodeMacro.h"
#include "JpegDecodeStruct.h"

#undef  EXT
#ifdef _IN_JPEGDECODESCHEDULE
#define EXT
#else
#define EXT     extern
#endif


//------------------------------------------------------------------------------
//EXT unsigned int  JpegRGBOutputBuffer[320]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned int  JpegRGBOutputBuffer[SCR_LENGTH*2]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned int  JPGFileReadBuffer[JPG_PIPO_BUF_BYTESIZE]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned int  JPGInputBuffer[JPG_PIPO_BUF_BYTESIZE]  __attribute__((section(".jpeg_bss, \"aw\"")));

//------------------------------------------------------------------------------
EXT unsigned int  *pJpegRGBOutBuffer  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT short  do_scale  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  ScaledX __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  ScaledY __attribute__((section(".jpeg_bss, \"aw\"")));

EXT short  hor_blank  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  ver_blank  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  number  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  up_para  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  down_para  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  g_ZeroOneMarker __attribute__((section(".jpeg_bss, \"aw\"")));

EXT short temp_block[64]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short temp_mcu_block[64*6] __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int block_y_mcu[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT unsigned short	huff_val[JPGD_MAXHUFFTABLES][256]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned short	huff_num [JPGD_MAXHUFFTABLES][17]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short quant[JPGD_MAXQUANTTABLES][64] __attribute__((section(".jpeg_bss, \"aw\"")));;
EXT int   scan_type  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT int   comps_in_frame  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_h_samp[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_v_samp[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_quant[JPGD_MAXCOMPONENTS] __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_ident[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT int   comp_h_blocks[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_v_blocks[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT int   comps_in_scan  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_list[JPGD_MAXCOMPSINSCAN]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_dc_tab[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   comp_ac_tab[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT int   spectral_start  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   spectral_end  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   successive_low   __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   successive_high  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   max_blocks_per_row  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short	 mcus_per_col  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT int   max_mcus_per_row __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   max_blocks_per_mcu __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int   max_mcus_per_col  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short *component[JPGD_MAXBLOCKSPERMCU]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short last_dc_val[JPGD_MAXCOMPONENTS]  __attribute__((section(".jpeg_bss, \"aw\"")));

//EXT unsigned int Psample_buf[320*4*3]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned int Psample_buf[SCR_LENGTH*2*4*3]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short *judgeptr  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  zagtable   __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short  idct_temp[64]   __attribute__((section(".jpeg_bss, \"aw\"")));

EXT unsigned int *jpeg_outline __attribute__((section(".jpeg_bss, \"aw\"")));
//EXT int jpeg_yuvhorizental[960]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int jpeg_yuvhorizental[SCR_LENGTH*6]  __attribute__((section(".jpeg_bss, \"aw\"")));
//EXT int jpeg_yuvbuf[480]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int jpeg_yuvbuf[SCR_LENGTH*3]  __attribute__((section(".jpeg_bss, \"aw\"")));
//EXT int jpeg_scalebuffer[640]   __attribute__((section(".jpeg_bss, \"aw\"")));
EXT int jpeg_scalebuffer[SCR_LENGTH*2*3]   __attribute__((section(".jpeg_bss, \"aw\"")));

EXT unsigned short huffsize[257]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned short huffcode[257]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT short jpeg_even_odd_mcu   __attribute__((section(".jpeg_bss, \"aw\"")));

EXT unsigned int *PSampleY  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned int *PSampleU  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned int *PSampleV  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT unsigned int JpegFileFormatError __attribute__((section(".jpeg_bss, \"aw\"")));

EXT JPEG_FLAG   /*volatile*/  JpegFlag  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT JPEG_BITSTREAM *jpeg_bs __attribute__((section(".jpeg_bss, \"aw\"")));
EXT JPEG_BITSTREAM jpeg_bits __attribute__((section(".jpeg_bss, \"aw\"")));
   
EXT JPEG_BITS_USE jpeg_show_skip  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT JPEG_BITS_USE *jpeg_psss __attribute__((section(".jpeg_bss, \"aw\"")));

EXT JPEG_POST_MARKER jpeg_post_marker  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT JPEG_POST_MARKER *jpeg_process_marker __attribute__((section(".jpeg_bss, \"aw\"")));

EXT IMAGE_SIZE  jpeg_image_p  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT IMAGE_SIZE *jpeg_imp  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT JPEG_HUFF_TABLES_T	huff_table[JPGD_MAXHUFFTABLES]  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT RESTART_DC  *jpeg_rst	__attribute__((section(".jpeg_bss, \"aw\"")));
EXT RESTART_DC  jpeg_reset  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT JPEG_PHUFF_TABLES_T  dc_huff_seg[JPGD_MAXBLOCKSPERMCU]  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT JPEG_PHUFF_TABLES_T  ac_huff_seg[JPGD_MAXBLOCKSPERMCU]  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT DEOCDE_NEXT_HQ	*jpeg_dcd   __attribute__((section(".jpeg_bss, \"aw\"")));
EXT DEOCDE_NEXT_HQ	jpeg_decode_next_kit   __attribute__((section(".jpeg_bss, \"aw\"")));

EXT SCALE_P jpeg_scale_line  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT SCALE_P *jpeg_scale_point  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT unsigned int JpegTanCont __attribute__((section(".jpeg_bss, \"aw\"")));

EXT volatile unsigned int JpeaDmaTanFlag  __attribute__((section(".jpeg_bss, \"aw\"")));

EXT unsigned int JpegAutoBrowserTimeFlag  __attribute__((section(".jpeg_bss, \"aw\"")));
EXT unsigned int JpegAutoPlayFlag         __attribute__((section(".jpeg_bss, \"aw\"")));
//------------------------------------------------------------------------------
#endif
//******************************************************************************
