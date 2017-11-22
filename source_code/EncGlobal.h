/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : EncGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _ENCGLOBAL_H
#define _ENCGLOBAL_H

#include "Struct.h"
#include "EncStruct.h"

#undef  EXT
#ifdef _IN_ENCSCHEDULE
#define EXT
#else
#define EXT     extern
#endif

//------------------------------------------------------------------------------
//Mic Golbal Var
EXT MS_ADPCM_DATA       ms_data __attribute__((section(".enc_bss, \"aw\"")));
EXT long                MicRecTotalData __attribute__((section(".enc_bss, \"aw\"")));
//------------------------------------------------------------------------------
//Mic Golbal Var
EXT unsigned long       mp3enc_total_data __attribute__((section(".enc_bss, \"aw\"")));
EXT unsigned long       EncDispFlag __attribute__((section(".enc_bss, \"aw\"")));
EXT unsigned int        EncPlayStatus __attribute__((section(".enc_bss, \"aw\"")));

//EXT ROCK_THREAD_DATA    Mp3enc_obj __attribute__((section(".EncoderBss, \"aw\"")));
//EXT ROCK_T              Rock_obj __attribute__((section(".enc_data, \"aw\"")));
EXT unsigned int        EncodeMode __attribute__((section(".enc_bss, \"aw\"")));
EXT ENC_FLAG            EncodeFlag __attribute__((section(".enc_bss, \"aw\"")));
//EXT _TIME               EncodeCurTime __attribute__((section(".enc_bss, \"aw\"")));
//EXT _TIME               EncodeTotalTime __attribute__((section(".enc_bss, \"aw\"")));

EXT MUSIC_PLAY_INFO     EncPlayInfo __attribute__((section(".enc_bss, \"aw\"")));;

EXT long	        g_dwEncoderCurrTimeSecond __attribute__((section(".enc_bss, \"aw\"")));
EXT long	        g_dwEncoderTotleTimeSecond __attribute__((section(".enc_bss, \"aw\"")));
EXT long            g_dwEncoderRemainTimeSecond __attribute__((section(".enc_bss, \"aw\"")));
EXT long            g_dwSampleCount __attribute__((section(".enc_bss, \"aw\"")));
//--------------------------------------------------------------------------
EXT unsigned int    PCMSoundDataCount __attribute__((section(".enc_bss, \"aw\"")));
EXT unsigned int    PCMNoSoundTimes __attribute__((section(".enc_bss, \"aw\"")));
EXT unsigned int    PCMHaveSoundTimes __attribute__((section(".enc_bss, \"aw\"")));
//------------------------------------------------------------------------------
EXT int             EncodeInputBuffer[AD_BUFFER_SIZE] __attribute__((section(".enc_bss, \"aw\"")));
EXT unsigned int	* MicRecEncodeBuf/*[256]*/ __attribute__((section(".enc_bss, \"aw\"")));
EXT int             * MicRecHeadBuf/*[512]*/ __attribute__((section(".enc_bss, \"aw\"")));

EXT int             EncodeOutputBuffer[512] __attribute__((section(".enc_bss, \"aw\"")));
EXT int             EncodeOutputBufferRemainData  __attribute__((section(".enc_bss, \"aw\"")));
EXT int         *pEncodeInputBuffer __attribute__((section(".enc_bss, \"aw\"")));
EXT int         *pPCMInputBuffer __attribute__((section(".enc_bss, \"aw\"")));


#if(FLASHROM_TYPE == _MLC_)
#define  MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE     2048
#define  MLC_ENCODER_BUFFER_NUM                  10

//录音输入buffer
EXT int  LINE_IN_MSADPCM_BUFFER[2048] __attribute__((section(".enc_bss, \"aw\"")));
//录音输出缓冲buffer
EXT int  MLCEncoderOutputBuffer[MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE *MLC_ENCODER_BUFFER_NUM] __attribute__((section(".enc_bss, \"aw\"")));
EXT int  MLCEncoderOutputBufferIndex      __attribute__((section(".enc_bss, \"aw\"")));
EXT int  pMLCEncoderOutputBufferOffset    __attribute__((section(".enc_bss, \"aw\"")));
//buffer中剩余没写到flashrom中的数据
EXT int  MLCEncoderOutputBufferLeftData    __attribute__((section(".enc_bss, \"aw\"")));
#else
#define  MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE     512

EXT ROCK_THREAD_DATA    Mp3enc_obj __attribute__((section(".EncoderBss, \"aw\"")));
EXT ROCK_T              Rock_obj __attribute__((section(".enc_data, \"aw\"")));
EXT int     EncodeOutputBuffer[MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE] __attribute__((section(".enc_bss, \"aw\"")));
#endif

#endif
//******************************************************************************

