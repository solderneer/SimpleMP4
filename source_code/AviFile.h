/*
****************************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  File name :     avifile.h
*  Author:         Jiliu Huang
*  Description:    Parse .avi file, get the startaddress of video and audio data in this
*                  file.
*  Others:         
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/01      1.0           ORG
*
****************************************************************************************
*/
#ifndef _AVIFILE_H
#define _AVIFILE_H
//------------------------------------------------------------------------------
#include "avistruct.h"

int AviFileIdentify(signed char Handle,AVI_STREAM_INFO *avi_file_information);

int GetNextAudioChunk(unsigned long *AudioChuckOffset, unsigned long *ChunkLen);
int GetNextVideoChunk(unsigned long *videoChuckOffset, unsigned long *ChunkLen);

int GetPreVideoIFrameChunk( unsigned long *VideoChunkOffset,  
                            unsigned long *ChunkLen);
                             
int GetNextVideoIFrameChunk( unsigned long *VideoChunkOffset, 
                             unsigned long *ChunkLen);
                             
int SynAudio2VideoChunk(unsigned long       *AudioChunkOffset, 
                        unsigned long       *ChunkLen,
                        unsigned long       *AviCurrFrameNum,
                        unsigned long       *AviLastChunkNum,
                        AVI_STREAM_INFO     *avi_file_information);                             
//------------------------------------------------------------------------------
#endif
//******************************************************************************
