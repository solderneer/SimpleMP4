/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaDecodeStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _WMAWMA_H
#define _WMAWMA_H
//------------------------------------------------------------------------------
#define  Word16 int
#define  Word32 long

#define max(a,b)	(a)<(b)?(b):(a)
#define min(a,b)	(a)>(b)?(b):(a)

extern short WMAHuffmanTableBase16OB ;//zyf 2005-12-07
extern short WMAHuffmanTableBase44OB  ;//zyf 2005-12-07
extern short WMAHuffmanTableBase44QB  ;//zyf 2005-12-07

extern ZSP_getpcm  zsp_wma_pcm;
extern Word16 wmabuffer[INBUFFERSIZE];
extern Word16 wmapcm[OUTBUFFERSIZE];
extern zspstream  wmastream_data;  
extern WMA_FILE  WMA_content;
extern tWMAFileHdrStateInternal ghdr_parse;

extern Word16 WMAInit(void);
extern Word16 WMA_Decoder_Reset(void);
extern Word16 ASF_TagParser(Word16*pbuffer,void*WMA_cont);
extern Word16 WMAFileDecodeData( Word16 *pnDecodedSamples);
extern Word16 WMAGetPCM(Word16* ppbDst,Word16 fetchpcm);

//------------------------------------------------------------------------------
#endif
//******************************************************************************
