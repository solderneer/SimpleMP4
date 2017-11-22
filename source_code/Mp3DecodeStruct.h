/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : DecodeMacro.h
* Author     : tonyzyz
* Description: This Head File Define the Global Varibal For MP3 Decode and WMA Decode
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _DECODESTRUCT_H
#define _DECODESTRUCT_H

#include "Mp3DecodeMacro.h"

//------------------------------------------------------------------------------

typedef union {
    struct 
    {
        unsigned int    bPcmBufEmpty:       1,
                        bPcmBufFull:        1,
                        bBufferIndex:       1,
                        bMP3DecodeError:    1,
                        bMP3FileFFW:        1,
                        bMP3DecodeInit:     1,
                        bMP3DecodeEnable:   1,
                        bMp3Id3Have:        1,
                        bMp3LrcHave:        1,
                        bMp3LrcDisp:        1,
                        bReserved:          6;
    }bc;
    unsigned int word;
} _MP3_FLAG;

typedef union {
    struct 
    {
        unsigned int    A_:         1,
                        AB:         1,
                        ABOFF:      1,
                        CurTime:    1,
                        TimeGuage:  1,
                        Bitrate:    1,
                        SamplingFreq:   1,
                        Stroboscope:    1,
                        bReserved:  8;
    }bc;
    unsigned int word;
} _MP3_DISPLAY_FLAG;

typedef struct{
long   id3_size;
short  id3_title_size;
short  id3_singer_size;
short  id3_album_size;
short  id3_title[ID3_TAG_SIZE];
short  id3_singer[ID3_TAG_SIZE];
short  id3_album[ID3_TAG_SIZE];
short  id3_genren;  //liupai
short  id3_genren_info[ID3_GENREN_INFO];
short  id3_ver;     //v1,v2
short  id3_check;   //null or full

}ID3_TAG;

//------------------------------------------------------------------------------
#endif
//******************************************************************************
