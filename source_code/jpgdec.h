/*******************************************************************************
 *							Smart Jpeg Decoder
 *	
 *	File:
 *		jpgdec.c
 *	
 ******************************************************************************/

#ifndef JPGDEC_H
#define JPGDEC_H

#include "JpegDecodeStruct.h"

#undef  EXT
#ifdef  _IN_JPEGDECODESCHEDULE
#define EXT
#else
#define EXT extern
#endif
//------------------------------------------------------------------------------

short read_dht_marker(void);
short read_dqt_marker(void);
short read_sof_marker(void);
short skip_variable_marker(void);
short read_dri_marker(void);
short read_sos_marker(void);
short next_marker(void);
short process_markers(void);
short locate_soi_marker(void);
short locate_sof_marker(void);
short locate_sos_marker(void);
short calcu_output(void);
void init_decode(void);

void output_one_mcu2(void);
void make_huff_table(int index,  JPEG_PHUFF_TABLES_T hs);
short check_quant_tables(void);
short check_huff_tables(void);
int init_scan(void);
short init_frame(void);
short process_restart(void);

void idct(short *data, short *Pdst_ptr);
void idct4x4(short *data, short *Pdst_ptr);
void idct2x2(short *data, short *Pdst_ptr);
void idct1x1(short *data, short *Pdst_ptr);

unsigned short get_char_4(void);
void Read_Bits_From_Peritheral(void);
void colorconvert_1(void);

short decode(void);
unsigned short get_char1(void);
void stuff_char(unsigned int c);
unsigned short get_octet(void);
unsigned short show_bits_1(int num_bits);
unsigned short show_bits_2(int numbits);
int huff_decode(JPEG_PHUFF_TABLES_T Ph);
int HUFF_EXTEND_TBL( int a, int b);
void jpeg_postprocess( void);
void write_blank( void );
unsigned short	show_char(void);
short decode_next(void);

void yuv2rgb(int *Yptr,int *Uptr,int *Vptr) ;
void v_col(int *Vptr1,int *Vptr2,int *Vptr) ;
void u_col(int *Uptr1,int *Uptr2,int *Uptr) ;
void y_col(int *Yptr1,int *Yptr2,int *Yptr);
void uv_row(int *Uptr1,int *u_src,int *v_src);
void yy_row(int *y_src,int *Yptr1) ;
void unscale_yuv2rgb(int *Yptr,int *Uptr,int *Vptr) ;
void init_postprocess(void);
void calculate_proportion(short in,short out);

//-----------------------------------------------------------------------------
#endif
//******************************************************************************

