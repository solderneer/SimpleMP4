/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3DecodeMacro.h
* Author     : tonyzyz
* Description: This File Define the Macros For MP3 Decode and WMA Decode
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _MP3DECODEMACRO_H
#define _MP3DECODEMACRO_H

#define MUSIC_PLAY_STATE_FFW    1
#define MUSIC_PLAY_STATE_FFD    0

//------------------------------------------------------------------------------
#define     READOUT_BUF_LENGTH              512             //File System Read Data Output Buffer Size
#define     APPLICATION_INPUT_BUF_LENGTH    1280            //MP3 Decode Input Buffer Size
#define     PCM_OUT_BUF_LENGTH              2*2304          //MP3 Decode Output Buffer Size
#define     MP3_DECODE_MAX_FRAME_LENGTH     868             //MP3 Decode Max Frame Length

//------------------------------------------------------------------------------
#define     TIME_GUAGE_CONST                205              //Time Guage Count
//------------------------------------------------------------------------------
#define   ID3_V2_INBUFFER_SIZE   4096
#define   ID3_V1_INBUFFER_SIZE   128
#define   ID3_TAG_SIZE           30
#define   ID3_GENREN_INFO        10
//------------------------------------------------------------------------------
#endif
//******************************************************************************
