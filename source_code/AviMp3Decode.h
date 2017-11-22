/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviMp3Decode.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _AVIMP3DECODE_H
#define _AVIMP3DECODE_H
//------------------------------------------------------------------------------

__attribute__((section(".avi_v_text, \"ax\""))) void AviMp3DecodeHead(void);
__attribute__((section(".avi_v_text, \"ax\""))) void AviMp3DecodeStart(void);

__attribute__((section(".avi_v_text, \"ax\""))) void AviMp3DecodePlay(void);
__attribute__((section(".avi_v_text, \"ax\""))) void AviMp3DmaOutBufferChange(void);

//------------------------------------------------------------------------------
#endif
//******************************************************************************
