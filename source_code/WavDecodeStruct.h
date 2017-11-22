/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WavDecodeStruct.h
* Author     : zyf
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*             zyf       2005-12-6       1.0            ORG
*
********************************************************************************
*/
#ifndef _WAVDECODESTRUCT_H
#define _WAVDECODESTRUCT_H

#define WAVINBUFFERSIZE 0x1000
#define WAVOUTBUFFERSIZE 0x2000
#define WAVPCMBUFFERSIZE  0x1000 //one buffer size is 4kWords
//------------------------------------------------------------------------------
typedef union {
    struct 
    {
        unsigned int    bPcmBufEmpty:       1,
                        bPcmBufFull:        1,
                        bBufferIndex:       1,
                        bWavFileFFW:        1,
                        bWavDecodeInit:     1,
                        bWavDecodeEnable:   1,
                        bWavId3Have:        1,
                        bReserved:          9;
    }bc;
    unsigned int word;
} _WAV_FLAG;

//------------------------------------------------------------------------------
#endif
//******************************************************************************
