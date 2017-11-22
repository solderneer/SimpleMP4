/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : JpegDecodeMacro.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _JPEGDECODEMACRO_H
#define _JPEGDECODEMACRO_H
//------------------------------------------------------------------------------
#define JPG_PIPO_BUF_WORDSIZE           512
#define JPG_PIPO_BUF_BYTESIZE           512*2
#define JPG_BUF_WORDSIZE                512*2
    
//RGB Output Buffer Length Define
#define RGB_PIPO_BUF_WORDSIZE           160
#define RGB_PIPO_BUF_BYTESIZE           320
#define RGB_BUF_WORDSIZE                320

#define SIZE_Y                  (160*(128>>1))
#define SIZE_U                  (80*(64>>1))
#define SIZE_V                  (80*(64>>1))

//JPEG Decode Out Status
#define JPG_DECODE_OK           0 
#define JPG_FILE_INPUT_OK       1
#define JPG_DECODE_FAIL         -1 
#define JPG_DECODE_ESCAPE       -2  
#define JPG_FILE_END            -3

#define SCR_LENGTH              320
#define SCR_HEIGHT              240

#define SHIFT_SCALE             9
#define SCALE_BIT               15

//------------------------------------------------------------------------------
#define	ONEONE					1
#define	TWOTWO					2
#define	FOURFOUR				4
#define	EIGHTEIGHT				8

#define JPGD_MAXCOMPONENTS		3
#define JPGD_MAXHUFFTABLES		4
#define JPGD_MAXQUANTTABLES		4
#define JPGD_MAXCOMPSINSCAN		4
#define JPGD_MAXBLOCKSPERMCU	6

#define JPGD_YH1V1              1
#define JPGD_YH2V1              2
#define JPGD_YH2V2              3

#define JPGD_MAXBLOCKSPERROW	(SCR_LENGTH*8)
#define JPGD_MAX_HEIGHT			(SCR_HEIGHT*16)
#define JPGD_MAX_WIDTH			(SCR_LENGTH*16)
#define JPEG_BUFFER				2046

//------------------------------------------------------------------------------
#define JPGD_OKAY               0
#define JPGD_DONE               1
#define JPGD_FAILED             -1

//------------------------------------------------------------------------------
#define RST0                    0xD0

//------------------------------------------------------------------------------
#define  M_SOF0                 0xC0
#define  M_SOF1                 0xC1
#define  M_SOF2                 0xC2
#define  M_SOF3                 0xC3
                                
#define  M_SOF5                 0xC5
#define  M_SOF6                 0xC6
#define  M_SOF7                 0xC7
                                
#define  M_JPG                  0xC8
#define  M_SOF9                 0xC9
#define  M_SOF10                0xCA
#define  M_SOF11                0xCB
                                
#define  M_SOF13                0xCD
#define  M_SOF14                0xCE
#define  M_SOF15                0xCF
                                
#define  M_DHT                  0xC4
                                
#define  M_DAC                  0xCC
                                
#define  M_RST0                 0xD0
#define  M_RST1                 0xD1
#define  M_RST2                 0xD2
#define  M_RST3                 0xD3
#define  M_RST4                 0xD4
#define  M_RST5                 0xD5
#define  M_RST6                 0xD6
#define  M_RST7                 0xD7
                                
#define  M_SOI                  0xD8
#define  M_EOI                  0xD9
#define  M_SOS                  0xDA
#define  M_DQT                  0xDB
#define  M_DNL                  0xDC
#define  M_DRI                  0xDD
#define  M_DHP                  0xDE
#define  M_EXP                  0xDF
                                
#define  M_APP0                 0xE0
#define  M_APP15                0xEF
                                
#define  M_JPG0                 0xF0
#define  M_JPG13                0xFD
#define  M_COM                  0xFE
                                
#define  M_TEM                  0x01
                                
#define  M_ERROR                0x100

#define JPEG_TMR_CHANNEL    4
//------------------------------------------------------------------------------
#endif
//******************************************************************************
