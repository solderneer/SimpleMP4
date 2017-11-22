/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _AVIGLOBAL_H
#define _AVIGLOBAL_H

#include "struct.h"
#include "AviMacro.h"
#include "AviStruct.h"

#ifdef _IN_AVISCHEDULE
#undef  EXT
#define EXT
#else
#define EXT extern
#endif

//------------------------------------------------------------------------------
EXT signed char     hAudioIndex __attribute__((section(".avi_v_bss, \"aw\"")));
EXT signed char     hVideoIndex __attribute__((section(".avi_v_bss, \"aw\"")));
   
EXT unsigned int    AviPlayState __attribute__((section(".avi_v_bss, \"aw\""))); 
EXT unsigned long   AviCurrFrameNum __attribute__((section(".avi_v_bss, \"aw\"")));            /*Avi 文件当前帧号*/
EXT unsigned int    AviFileFormatError __attribute__((section(".avi_v_bss, \"aw\"")));

/*以下5个变量用于Chunk seek   */
EXT unsigned long   AudioFilePos __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   AudioChunkLen __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   VideoFilePos __attribute__((section(".avi_v_bss, \"aw\"")));             
EXT unsigned long   VideoChunkLen __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   VideoIndexCount __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   AudioIndexCount __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   AviCurrFrameNum __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   AviLastChunkNum __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   AviAudioCurrFrameNum __attribute__((section(".avi_v_bss, \"aw\"")));

EXT unsigned long   AviAudioChunkOffset __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   AviAudioChunkLength __attribute__((section(".avi_v_bss, \"aw\"")));

EXT unsigned long   AviVideoChunkOffset __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned long   AviVideoChunkLength __attribute__((section(".avi_v_bss, \"aw\"")));

EXT int             Mp4DecodeState __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int    AviMp4FrameCount __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int    NeedFillDataNum __attribute__((section(".avi_v_bss, \"aw\"")));                // 解决长度为0的CHUNK的BUG

EXT AVI_STREAM_INFO AviStreamInfo __attribute__((section(".avi_v_bss, \"aw\"")));
EXT volatile AVI_PLAY_FLAG   AviPlayFlag __attribute__((section(".avi_v_bss, \"aw\"")));

//------------------------------------------------------------------------------
//Avi File Mp3 Global define
EXT unsigned long   AviMp3DecodeFreqTbl[2][4] __attribute__((section(".avi_a_bss, \"aw\"")));
EXT int             AviMp3DecodeBuffer[AVI_MP3_READOUT_BUF_LENGTH + AVI_MP3_INPUT_BUF_LENGTH + 2 * AVI_PCM_OUT_BUF_LENGTH] __attribute__((section(".avi_a_bss, \"aw\"")));

EXT int             *pAviMp3FileReadBuf __attribute__((section(".avi_a_bss, \"aw\"")));
EXT int             *pAviMp3InputBuffer __attribute__((section(".avi_a_bss, \"aw\"")));
EXT int             *pAviMp3PCMOutAddress __attribute__((section(".avi_a_bss, \"aw\"")));
EXT unsigned int    *pAviMP3OutputBuf __attribute__((section(".avi_a_bss, \"aw\"")));
EXT int             *pAviPCMOutputBuf __attribute__((section(".avi_a_bss, \"aw\"")));

EXT unsigned int    AviPCMOutBufferLength __attribute__((section(".avi_a_bss, \"aw\"")));

EXT unsigned int    AviMp3DecodeInputBufWordsUsed __attribute__((section(".avi_a_bss, \"aw\"")));
EXT unsigned int    AviMp3DecodeInputBufWordsRemaining __attribute__((section(".avi_a_bss, \"aw\"")));

EXT unsigned long   AviMp3Freq __attribute__((section(".avi_a_bss, \"aw\"")));
EXT unsigned int    AviMp3Bitrate __attribute__((section(".avi_a_bss, \"aw\"")));
EXT unsigned int    AviMP3DecodeFrameCount __attribute__((section(".avi_a_bss, \"aw\"")));

//------------------------------------------------------------------------------
EXT unsigned int    InputByteBuffer[MP4_PIPO_BUF_WORDSIZE] __attribute__((section(".avi_v_bss, \"aw\""))); //__attribute__((section(".static_buffer, \"aw\"")));			//buffer used for serial input data


EXT unsigned int    MP4InputBuffer[MP4_BUF_WORDSIZE] __attribute__((section(".avi_v_bss, \"aw\"")));       // __attribute__((section(".static_buffer, \"aw\"")));   //MP4 Decode Input PIPO Buffer

EXT int             *pAviMp4FileReadBuf __attribute__((section(".avi_v_bss, \"aw\"")));


//------------------------------------------------------------------------------
EXT unsigned int AviDmaFlag      __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int AviTimeNum      __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int AviBlockFlag    __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int AviLcdDmaFlag   __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int AviI2EDmaFlag   __attribute__((section(".avi_v_bss, \"aw\"")));
//EXT unsigned int volatile AviDmaChlFlag   __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int AviLcdLine     __attribute__((section(".avi_v_bss, \"aw\"")));

EXT unsigned int volatile Dma_16x16Block   __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_16x8Block    __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_E2IBlock     __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_E2IBlock_Y   __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_E2IBlock_UV  __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_I2EBlock     __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_Rgb2Lcd      __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_RGB_I2E      __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile Dma_RGB_E2LCD    __attribute__((section(".avi_v_bss, \"aw\"")));
//EXT unsigned int LcdTanCtr        __attribute__((section(".avi_v_bss, \"aw\"")));

EXT unsigned int volatile Dma_AviDispFuncFlag __attribute__((section(".avi_v_bss, \"aw\"")));

EXT unsigned int volatile AviRgbSdramIndex __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile   LCDSdramIndex __attribute__((section(".avi_v_bss, \"aw\"")));
EXT unsigned int volatile FRAME_TYPE_LABEL[3] __attribute__((section(".avi_v_bss, \"aw\"")));
EXT AVI_DMA_CHANNEL volatile AviDmaChlFlag  __attribute__((section(".avi_v_bss, \"aw\"")));
//------------------------------------------------------------------------------
EXT    unsigned int disabledmatolcd         __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int uiAviStatusBarDispTime  __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int uiAviStatusBarTimeCont  __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int uiAviStatusBarDispFlag  __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int uiAviLcdDmaDoneFlag     __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int uiAviStatusBarDispBits  __attribute__((section(".avi_v_bss, \"aw\"")));

EXT    unsigned int uiAviSpeedFlag          __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int VideoIndexBuff_ptr      __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int VideoIndexBuff_valid    __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int VideoIndexBuff[512]     __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int AudioIndexBuff_ptr      __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int AudioIndexBuff_valid    __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int AudioIndexBuff[512]     __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int FFDWFramecount          __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    unsigned int AviPlayEndFlag          __attribute__((section(".avi_v_bss, \"aw\"")));

EXT    unsigned long   AviCurr_IFrameNum_FrameNum __attribute__((section(".avi_v_bss, \"aw\"")));            /*Avi 文件当前帧号*/
EXT    long m_video_framenum __attribute__((section(".avi_v_bss, \"aw\""))); 
EXT    long m_audio_framenum __attribute__((section(".avi_v_bss, \"aw\"")));  

EXT    unsigned int MovieGuageBak __attribute__((section(".avi_v_bss, \"aw\""))); 

EXT    unsigned long Timeadjust __attribute__((section(".avi_v_bss, \"aw\"")));
EXT    long Play_Frame_Rate __attribute__((section(".avi_v_bss, \"aw\""))); 
//------------------------------------------------------------------------------
#endif
//******************************************************************************
