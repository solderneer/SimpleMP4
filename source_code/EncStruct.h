/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : EncStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _ENCSTRUCT_H
#define _ENCSTRUCT_H

#include "EncMacro.h"

//------------------------------------------------------------------------------
typedef union {
    struct 
    {
    unsigned int
        bPcmBufEmpty:       1,
        bPcmBufFull:        1,
        bBufferIndex:       1,
        
        bEncodeInit:        1,
        bEncodeEnable:      1,
        bNewFileBegin:      1,
        bFileCreat:         1,
        bAutoDevideSection: 1,
        bFileFragClean :    1,
        
        bTheFirstPcmData:   1,    
        bDiskFull:          1,
        bReserved:          5;
    }bc;
    unsigned int word;
}ENC_FLAG;


//------------------------------------------------------------------------------
//In MsAdpcm.h
typedef struct //ms_adpcm_data
{
	int  wformattag;
	int	 channelCount;
	long nsamplespersec;
	long navgbytespersec;
	int  blockalign;
	int  wbitspersample;
	int  cbsize;

	int  samplesPerBlock;
	int  numCoefficients;
} MS_ADPCM_DATA;

typedef struct //ms_adpcm_state
{
	unsigned int	predictor;
	unsigned int	delta;
	int	sample1, sample2;
	int	coefficient1,coefficient2;	
} MS_ADPCM_STATE;

//------------------------------------------------------------------------------
//In Thread.h
typedef struct //d
{
   int l[1+SBMAX_l];
   int s[1+SBMAX_s];
} SCALEFAC_STRUCT; 

typedef struct //c 
{
	int part2_3_length;
	int big_values;	
 	int global_gain;
	int scalefac_compress;
	int window_switching_flag;		
	int table_select[3];	
	int region0_count;
	int region1_count;
	int preflag;
	int scalefac_scale;
	int count1table_select;
	
	int block_type;
	int subblock_gain[3];
    int count1;
	int part2_length;
	int sfb_lmax;
	int sfb_smin;
	int count1bits;
	/* added for LSF */
	int *sfb_partition_table;
	int slen[4];
} GR_INFO;

typedef struct //b
{
	int resvDrain_pre;
	int resvDrain_post;
	int scfsi[2][4];
	GR_INFO	tt[2][2];
} III_SIDE_INFO_T;

typedef struct //a
{
	int l[SBMAX_l];
	int s[SBMAX_s][3];
} III_SCALEFAC_T;

typedef struct //header_buf_t
{
    int write_timing;
    int ptr;
    int buf[20];
} HEADER_BUF_T;

typedef struct  bit_stream_struc 
{    
    int totbit;
    int buf_byte_idx;
    int buf_bit_idx;
    int buf[MAXMP3BUFFER];
    int buf_size;
} BIT_STREAM_STRUC;

typedef struct //rock_thread_data_s
{
    III_SCALEFAC_T	scalefac[2][2];	
    III_SIDE_INFO_T	l3_side;

    long xr[2][2][576];
	int	 xr_sign[2][2][576];
    long xrpow[2][2][576];
    long xrpow_sum[2][2];
    long xrpow_max[2][2];
	int	 l3_enc[2][2][576];
	int	 tid;
	int	 padding;
	int	 mode_ext;
	int	 bitrate_index;
	int	 ResvSize;
} ROCK_THREAD_DATA;

typedef struct //ro_t 
{
    SCALEFAC_STRUCT scalefac_band;
    
    int bv_scf[576];
	int nChannel;
    int channels_out;
	long inpFreqHz;
    long out_samplerate;
    int samplerate_index;
    int bitrate_index;
    int whole_SpF;
    int is_mpeg1;
	int mode_gr;	
	int rateKbps;
	int emphasis;
	int quality;
	int mode;
	int framesize;
	int version;
    int copyright;
    int original;
    int brate;
    long frac_SpF;
    int sideinfo_len; 
    int bitsPerFrame;   
    long slot_lag; 
    int OldValue[2];
    int CurrentStep;
    int sfb21_extra;
    int ResvSize;
    int ResvMax;
    int main_data_begin;

    int	blocktype_old[2];
    int h_ptr;
    int w_ptr;
   
    HEADER_BUF_T header[4];
    BIT_STREAM_STRUC  bs;
} ROCK_T;
//------------------------------------------------------------------------------
#endif
//******************************************************************************
