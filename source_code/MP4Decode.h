/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : MP4Decode.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _MP4DECODE_H
#define _MP4DECODE_H

#include "xvid_decoder.h"

//------------------------------------------------------------------------------
short MP4DecodeProc(unsigned int InitStatus);
void Post_Processing(void);
void AviMp4SendLcdDmaEnd(void);

unsigned int Read_VideoData_ToBuf(unsigned int *bufptr, unsigned int len);
void FillBuffer(BITSTREAM *bs);
void Send_RGBData_ToLcd(void);
//------------------------------------------------------------------------------
#endif
//******************************************************************************

