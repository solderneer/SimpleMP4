/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviMacro.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _AVIMACRO_H
#define _AVIMACRO_H

#include <creg.h>
#include "Memap.h"

//------------------------------------------------------------------------------
#define     AVI_STATUS_BAR_NULL         0
#define     AVI_STATUS_BAR_START        1
#define     AVI_STATUS_BAR_DISP         2
#define     AVI_STATUS_BAR_FADE         3
#define     AVI_STATUS_BAR_EXIT         4
//------------------------------------------------------------------------------
#define AVI_STATUS_BAR_VOL_BMASK        0x0001
#define AVI_STATUS_BAR_STATE_BMASK      0x0002
#define AVI_STATUS_BAR_LOCK_BMASK       0x0004
#define AVI_STATUS_BAR_JDT_BMASK        0x0008
#define AVI_STATUS_BAR_TMR_BMASK        0x0010
#define AVI_STATUS_BAR_BATT_BMASK       0x0020

#define AVI_STATUS_BAR_DISP_ALL         0x003F 


//------------------------------------------------------------------------------
#define     AVI_MP3_READOUT_BUF_LENGTH          512
#define     AVI_PCM_OUT_BUF_LENGTH              2304
#define     AVI_MP3_INPUT_BUF_LENGTH            665
#define     AVI_MP3_DECODE_MAX_FRAME_LENGTH     409//665-256

//------------------------------------------------------------------------------
#define     MP4_PIPO_BUF_WORDSIZE				2*1024					//864	
#define     MP4_PIPO_BUF_BYTESIZE				4*1024
#define     MP4_BUF_WORDSIZE					4*1024
	
//RGB Output Buffer Length Define
#define     RGB_PIPO_BUF_WORDSIZE               320*1
#define     RGB_PIPO_BUF_BYTESIZE               320*2
#define	    RGB_BUF_WORDSIZE					320*2
//------------------------------------------------------------------------------
//AVI Format define
#define     AVI_AUDIO_ONLY          1
#define     AVI_VIDEO_ONLY          2
#define     AVI_AUDIO_VIDEO         3

//AVI Play State define
#define     AVI_DECODE_PLAY         0
#define     AVI_DECODE_PAUSE        1

//GetNextAudioChunk / GetNextVideoChunk Return val
#define     PN_FRAME                0
#define     I_FRAME                 1
#define     ERROR_FRAME            -1
//------------------------------------------------------------------------------
//Mp4 Decode Error Code define
#define     MP4_DECODE_OK           0
#define     MP4_DECODE_END          1
#define     MP4_DECODE_FAIL        -1

//------------------------------------------------------------------------------
//Mp4 Decode Error Code define
#define     AVI_MP3_DECODE_OK       0
#define     AVI_MP3_DECODE_ERROR   -1
//------------------------------------------------------------------------------
#define     DMA_CHANNEL0_STATE                  (1<<0)
#define     DMA_CHANNEL1_STATE                  (1<<1)
#define     DMA_CHANNEL2_STATE                  (1<<2)
//------------------------------------------------------------------------------
#define     DMA_SDRAM_TO_SRAM       (1<<0)
#define     DMA_SRAM_TO_SDRAM       (1<<1)
//------------------------------------------------------------------------------
#define     AVI_STATUS_TIMEOUT         75           // 30 frames = 2s 
//------------------------------------------------------------------------------

//#define BITS_N_FRAME(A,B)   (unsigned long)A = (unsigned long)(A|((unsigned long)1<<B))
//#define BITC_N_FRAME(A,B)   (unsigned long)A = (unsigned long)(A&(~((unsigned long)1<<B)))
//#define BITT_N_FRAME(A,B)   (unsigned long)(A&((unsigned long)1<<B))
                            
////////////////////////// Add by HJL for AVI play /////////////////////////////                            
#define         AVI_AUDIO_FRAME_TIME            13061

#define         AVI_RGB_SDRAM_BUF_CONT          42                  //48 RGB Page = 3,686,400 words

#define         NUMBER_OF_FRAME_RATE            9
#endif
//******************************************************************************
