/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3DecodeGlobal.h
* Author     : tonyzyz
* Description: This Head File Define the Global Varibal For MP3 Decode and WMA Decode
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _DECODEGLOBAL_H
#define _DECODEGLOBAL_H

#include "Struct.h"
#include "Mp3DecodeStruct.h"

#ifdef _IN_MP3DECODESCHEDULE
#undef  EXT
#define EXT
#else
#define EXT     extern
#endif

//------------------------------------------------------------------------------
EXT int                 BufferForEncode[READOUT_BUF_LENGTH + APPLICATION_INPUT_BUF_LENGTH + 2 * PCM_OUT_BUF_LENGTH] __attribute__((section(".mp3_bss, \"aw\"")));
EXT short               StroboscopeArray[22] __attribute__((section(".mp3_bss, \"aw\"")));
//EXT unsigned long       MP3_Decode_Freq_Tbl[2][4] __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned char       Mp3LrcFileName[8] __attribute__((section(".mp3_bss, \"aw\"")));
//------------------------------------------------------------------------------
EXT int                 *pPCMOutputBuf __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned int        *pMP3DecodeOutputBuf __attribute__((section(".mp3_bss, \"aw\"")));

EXT int                 *MP3DecodeTempReadoutBuf __attribute__((section(".mp3_bss, \"aw\"")));
EXT int                 *ApplicationInputBufferStart __attribute__((section(".mp3_bss, \"aw\"")));
EXT int                 *FirstPCMOutAddress __attribute__((section(".mp3_bss, \"aw\"")));
//------------------------------------------------------------------------------
EXT unsigned int        DecodeCount __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned int        StroboscopeCount __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned int        BitrateCount __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned int        MP3DecodeBadFrameCount __attribute__((section(".mp3_bss, \"aw\"")));

EXT unsigned long       MP3FileOffSet __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned long       MP3FileOffSetA __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned long       MP3FileOffSetB __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned long       MP3TimeCount __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned long       MP3TimeGuageCount __attribute__((section(".mp3_bss, \"aw\"")));

EXT unsigned long       Mp3CurrTimeSec __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned long       Mp3TotalTimeSec __attribute__((section(".mp3_bss, \"aw\"")));

EXT unsigned int        PCMOutBufferLength __attribute__((section(".mp3_bss, \"aw\"")));

EXT unsigned int        MP3DecodeInputBufWordsUsed __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned int        MP3DecodeInputBufWordsRemaining __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned int        MP3DecodeErrorTimes     __attribute__((section(".mp3_bss, \"aw\"")));    
EXT unsigned int        MP3FileFormatError      __attribute__((section(".mp3_bss, \"aw\"")));
EXT unsigned int        Mp3PlayStatus      __attribute__((section(".mp3_bss, \"aw\"")));
//------------------------------------------------------------------------------
//EXT _TIME               MP3PlayCurTime __attribute__((section(".mp3_bss, \"aw\"")));
//EXT _TIME               MP3PlayTotalTime __attribute__((section(".mp3_bss, \"aw\"")));

EXT _MP3_FLAG           Mp3PlayFlag __attribute__((section(".mp3_bss, \"aw\"")));
EXT _MP3_DISPLAY_FLAG   MP3DispFlag __attribute__((section(".mp3_bss, \"aw\"")));

extern EQ_Mode          EQmode __attribute__((section(".mp3_bss, \"aw\"")));
EXT ID3_TAG             Id3Info __attribute__((section(".mp3_bss, \"aw\"")));

extern unsigned long    MP3_Decode_Freq_Tbl[2][4];

short Mp3EQ3DInit(short Freq,short index,short con);

//------------------------------------------------------------------------------
#endif
//******************************************************************************
