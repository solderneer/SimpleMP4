/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : EncMacro.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _ENCMACRO_H
#define _ENCMACRO_H

//------------------------------------------------------------------------------
#define ENCODER_TEXT __attribute__((section(".EncoderText, \"ax\"")))
#define ENCODER_BSS __attribute__((section(".EncoderBss, \"aw\"")))
#define ENCODER_DATA __attribute__((section(".EncoderData, \"aw\"")))

#define ENCODER_TEXT_ASM .section ".EncoderText","ax"
#define ENCODER_BSS_ASM .section ".EncoderBss","aw"
#define ENCODER_DATA_ASM .section ".EncoderData","aw"

//------------------------------------------------------------------------------
#define     TIME_GUAGE_CONST                210              //Time Guage Count

//------------------------------------------------------------------------------
#define AD_BUFFER_SIZE          2304*2// 2304*2
#define AD_PIPO_BUFFER_SIZE     2304

//------------------------------------------------------------------------------
//msadpcm
#define	MSADPCM_CH_NUMPCM	        500
#define	LINE_IN_MSADPCM_CH_NUMPCM  1012

#define MAXFILEBYTE             0x7ffffe00

#define IDELTA_COUNT            3
#define	MAX_INT16               32767
#define MIN_INT16               -32768

#define EMPTY                   0
#define FULL                    1
#define FILENAMEFULL            2

//------------------------------------------------------------------------------
//mp3
#define  MP3ENC_SIZE                    2304*2
#define  MP3ENC_AD_PIPO_BUFFER_SIZE     2304
#define  MP3ENC_CH_NUMPCM               1152
#define  MP3ENC_FULL                    1
#define  MP3ENC_EMPTY                   0
#define  MP3FILENAMEFULL                2

#define  MAXMP3BUFFER                   1500

#define  SBMAX_l                        22
#define  SBMAX_s                        13
//------------------------------------------------------------------------------
//Display Flag define
#define     ENC_DISPFLAG_NULL           ((unsigned long)(0x00000000))

#define     ENC_DISPFLAG_BACKGROUND     ((unsigned long)(0x00000001))

#define     ENC_DISPFLAG_BATT           ((unsigned long)(0x00000002))
#define     ENC_DISPFLAG_HOLD           ((unsigned long)(0x00000004))

#define     ENC_DISPFLAG_STATUS         ((unsigned long)(0x00000008))
#define     ENC_DISPFLAG_MODE           ((unsigned long)(0x00000010))

#define     ENC_DISPFLAG_BITRATE        ((unsigned long)(0x00000020))
#define     ENC_DISPFLAG_SAMPLERATE     ((unsigned long)(0x00000040))

#define     ENC_DISPFLAG_VOL            ((unsigned long)(0x00000200))

#define     ENC_DISPFLAG_CUR_TIME       ((unsigned long)(0x00001000))
#define     ENC_DISPFLAG_FREE_TIME      ((unsigned long)(0x00002000))
#define     ENC_DISPFLAG_GUAGE          ((unsigned long)(0x00004000))

#define     ENC_DISPFLAG_PATH           ((unsigned long)(0x00008000))
#define     ENC_DISPFLAG_FILENUM        ((unsigned long)(0x00010000))
#define     ENC_DISPFLAG_FILENAME       ((unsigned long)(0x00020000))
#define     ENC_DISPFLAG_EXTNAME        ((unsigned long)(0x00040000))

#define     ENC_DISPFLAG_ALL            ((unsigned long)(0xffffffff))

//------------------------------------------------------------------------------
#endif
//******************************************************************************
