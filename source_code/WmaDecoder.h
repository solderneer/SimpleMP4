/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaDecodeSchedule.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _WMADECODER_H
#define _WMADECODER_H
//------------------------------------------------------------------------------
void WmaPCMOutBufferChange(void);
void WMADecodeHead(void);
void WMADecodeStart(void);
void WMADecodeEnd(void);
void WMADecodePlay(void);
void WMADecodeTimeUpdate(void);
void WmaDecodeOutputEnable(void);
void WmaDecodeOutputDisable(unsigned int bEnd);
//------------------------------------------------------------------------------
#endif
//******************************************************************************

