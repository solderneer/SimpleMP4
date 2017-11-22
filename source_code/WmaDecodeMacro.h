/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaDecodeMacro.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*              ZYF          05-12-08
********************************************************************************
*/
#ifndef _WMADECODEMACRO_H
#define _WMADECODEMACRO_H


#define MUSIC_PLAY_STATE_FFW    1
#define MUSIC_PLAY_STATE_FFD    0
//------------------------------------------------------------------------------
//in wma.h
#define max(a,b)	((a)<(b)?(b):(a))
#define min(a,b)	((a)>(b)?(b):(a))

//------------------------------------------------------------------------------
//in WMAbuffer.h
#define INBUFFERMASK 0x0fff
#define INBUFFERSIZE 0x1000
#define OUTBUFFERSIZE 0x2000
#define NEEDFILLDATA  3500
#define WMA_PCM_BUFFER_SIZE  0x1000 //one buffer size is 4kWords

//------------------------------------------------------------------------------
//in WMAdecoder.c
#define WMA_SEEK_SECOND_NUM_FFD  2   //in FFD and  FFW mode ,one time skip seconds
#define WMA_SEEK_SECOND_NUM_FFW  2

#define WMA_OK                0x0000 
#define WMA_S_FALSE           0x0001 
#define WMA_S_BUFUNDERFLOW    0x0002 
#define WMA_S_NEWPACKET       0x0003 
#define WMA_S_NO_MORE_FRAME   0x0004 
#define WMA_S_NO_MORE_SRCDATA 0x0005 
#define WMA_S_LOSTPACKET      0x0006 

//------------------------------------------------------------------------------
//in wmaerror.h
// ERROR codes
#define WMA_E_FAIL            0x8001 
#define WMA_E_OUTOFMEMORY     0x8002 
#define WMA_E_INVALIDARG      0x8053 
#define WMA_E_NOTSUPPORTED    0x8004 
#define WMA_E_BROKEN_FRAME    0x8005 
#define WMA_E_BUFFEROVERFLOW  0x8006 
#define WMA_E_ONHOLD          0x8007  
#define WMA_E_NO_MORE_SRCDATA 0x8008 
#define WMA_E_WRONGSTATE      0x8009 
#define WMA_E_NOMOREINPUT     0x8010 // Streaming mode: absolutely no more data (returned via GetMoreData callback)

#define WMA_SUCCEEDED(Status) ((Status) >= 0)
#define WMA_FAILED(Status) ((Status)<0)
//--------------------------------------------
#define     TIME_GUAGE_CONST                205

//------------------------------------------------------------------------------
//in wmadefine.h
#define WMA_INTI_TABLE __attribute__((section(".WmaIntiTable, \"aw\"")))
#define WMA_INTI_TEXT __attribute__((section(".WmaIntiText, \"ax\"")))
#define WMA_INTI_BARKTABLE __attribute__((section(".wmabarktable, \"aw\"")))
#define WMA_DECODER_TEXT __attribute__((section(".WmaDecoderText, \"ax\"")))
#define WMA_DECODER_TABLE __attribute__((section(".WmaDecoderTable, \"aw\"")))
#define WMA_GLOBAL_TEXT __attribute__((section(".WmaGlobalText, \"ax\"")))


#define WMA_GLOBAL_TEXT_ASM .section ".WmaGlobalText","ax"
#define WMA_DECODER_TEXT_ASM .section ".WmaDecoderText","ax"
#define WMA_DECODER_TABLE_ASM .section ".WmaDecoderTable","aw"
//------------------------------------------------------------------------------
#endif
//******************************************************************************
