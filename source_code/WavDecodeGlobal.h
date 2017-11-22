/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WavDecodeGlobal.h
* Author     : zyf
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            zyf       2005-12-6      1.0            ORG
*
********************************************************************************
*/
#ifndef _WAVDECODEGLOBAL_H
#define _WAVDECODEGLOBAL_H

#include "Struct.h"
#include "WavDecodeStruct.h"

#undef  EXT
#ifdef _IN_WAVDECODESCHEDULE
#define EXT
#else
#define EXT     extern
#endif


//------------------------------------------------------------------------------
EXT int             WavBufferForDecode[12288]   __attribute__((section(".wav_bss, \"aw\"")));
//------------------------------------------------------------------------------
#define             pWavInputBuffer  WavBufferForDecode
#define             FirstWavPCMOutAddress  (WavBufferForDecode + WAVINBUFFERSIZE)
EXT int             *pWavPCMOutputBuf __attribute__((section(".wav_bss, \"aw\"")));
EXT int             *pWavDecodeOutputBuf __attribute__((section(".wav_bss, \"aw\"")));

EXT short           WavPCMOutBufferLength __attribute__((section(".wav_bss, \"aw\"")));

EXT unsigned long   WavFileOffSet   __attribute__((section(".wav_bss, \"aw\"")));
EXT unsigned long   WavFileOffSetA __attribute__((section(".wav_bss, \"aw\"")));
EXT unsigned long   WavFileOffSetB __attribute__((section(".wav_bss, \"aw\"")));

EXT long            dwWavCountGetPCM __attribute__((section(".wav_bss, \"aw\"")));
EXT long            gWavnAvgBytesPerSec __attribute__((section(".wav_bss, \"aw\"")));
EXT unsigned long   wWavLastTime __attribute__((section(".wav_bss, \"aw\"")));
EXT unsigned long   wWavTotalTime __attribute__((section(".wav_bss, \"aw\"")));

EXT unsigned int    WavPlayStatus __attribute__((section(".wav_bss, \"aw\"")));
EXT unsigned int    WavFileFormatError __attribute__((section(".wav_bss, \"aw\"")));

//EXT _TIME           WavPlayCurTime __attribute__((section(".wav_bss, \"aw\"")));
//EXT _TIME           WavPlayTotalTime __attribute__((section(".wav_bss, \"aw\"")));

EXT _WAV_FLAG       WavPlayFlag __attribute__((section(".wav_bss, \"aw\"")));

//------------------------------------------------------------------------------
#endif
//******************************************************************************
