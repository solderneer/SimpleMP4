/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3_Decode.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef     _MP3_DECODE_H
#define     _MP3_DECODE_H

void PCMOutBufferChange(void);
void MP3DecodeOutputEnable(void);
void MP3DecodeOutputDisable(unsigned int bEnd);
void MP3DecodeStart(void);
void MP3DecodePlay(void);
void Mp3DecodeEnd(void);
void MP3DecodeTimeUpdate(void);

#endif
//***************************************************************************************************

