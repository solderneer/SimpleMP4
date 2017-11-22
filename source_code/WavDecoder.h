/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WavDecodeSchedule.h
* Author     : zyf
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            zyf       2005-12-6       1.0            ORG
*
********************************************************************************
*/
#ifndef _WAVDECODER_H
#define _WAVDECODER_H
//------------------------------------------------------------------------------
void WavCMOutBufferChange(void);
void WavDecodeHead(void);
void WavDecodeStart(void);
void WavDecodeEnd(void);
void WavDecodePlay(void);
void WavDecodeTimeUpdate(void);
void WavDecodeOutputEnable(void);
void WavDecodeOutputDisable(unsigned int bEnd);
//------------------------------------------------------------------------------
#endif
//******************************************************************************

