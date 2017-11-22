/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaDecodeGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*           ZYF             05-12-08      1.1
********************************************************************************
*/
#ifndef _WMADECODEGLOBAL_H
#define _WMADECODEGLOBAL_H

#include "Macro.h"
#include "Struct.h"
#include "WmaDecodeStruct.h"
#include "Equ_Config.h"             //zyf

#undef  EXT
#ifdef _IN_WMADECODESCHEDULE
#define EXT
#else
#define EXT     extern
#endif

#ifdef _DRM9_ENABLE_
    #define PD_DRM
#endif

#ifdef PD_DRM
    #include "drmtype.h"
    #include "drmpd.h"
#endif 

//------------------------------------------------------------------------------
EXT short           WMAStroboscopeArray[22] __attribute__((section(".wma_bss, \"aw\"")));
//EXT int             fileinputtemp[512] __attribute__((section(".wma_bss, \"aw\"")));
EXT int             WMABufferForEncode[12288]   __attribute__((section(".bufferdata, \"aw\"")));
EXT unsigned char   WmaLrcFileName[8] __attribute__((section(".wma_bss, \"aw\"")));
//------------------------------------------------------------------------------
EXT int             *pWMAInputBuffer __attribute__((section(".wma_bss, \"aw\"")));
EXT int             *pWMAPCMOutputBuf __attribute__((section(".wma_bss, \"aw\"")));
EXT int             *pWMADecodeOutputBuf __attribute__((section(".wma_bss, \"aw\"")));
EXT int             *FirstWMAPCMOutAddress __attribute__((section(".wma_bss, \"aw\"")));

EXT short           WMAPCMOutBufferLength __attribute__((section(".wma_bss, \"aw\"")));

EXT unsigned long   WMAFileOffSet __attribute__((section(".wma_bss, \"aw\"")));
EXT unsigned long   WMAFileOffSetA __attribute__((section(".wma_bss, \"aw\"")));
EXT unsigned long   WMAFileOffSetB __attribute__((section(".wma_bss, \"aw\"")));

EXT long            gWMAnAvgBytesPerSec __attribute__((section(".wma_bss, \"aw\"")));
EXT unsigned long   cbLastTime __attribute__((section(".wma_bss, \"aw\"")));
EXT unsigned long   cbTotalTime __attribute__((section(".wma_bss, \"aw\"")));
EXT long            cCountGetPCM __attribute__((section(".wma_bss, \"aw\"")));

//EXT _TIME           WMAPlayCurTime __attribute__((section(".wma_bss, \"aw\"")));
//EXT _TIME           WMAPlayTotalTime __attribute__((section(".wma_bss, \"aw\"")));
EXT _WMA_FLAG       WmaPlayFlag __attribute__((section(".wma_bss, \"aw\"")));
extern EQ_Mode         WMAEQMode ;//__attribute__((section(".wma_bss, \"aw\"")));
EXT WMA_ID3_TAG     WmaId3Info __attribute__((section(".wma_bss, \"aw\"")));
//EXT short           WMAEQMode __attribute__((section(".wma_bss, \"aw\"")));//add by zyf
EXT int WMAEq3DEnable __attribute__((section(".wma_bss, \"aw\"")));

EXT unsigned int    WmaFileFormatError __attribute__((section(".wma_bss, \"aw\"")));
EXT unsigned int    WmaPlayStatus __attribute__((section(".wma_bss, \"aw\"")));
//---------------------------------------------------------------------
//WMATableRunDram first define in file "TableDram.S"
//#if(FLASHROM_TYPE == _MLC_ ) 
//    #ifdef _DRM9_ENABLE_
        EXT unsigned int   WMATableRunDram[0x1DCE]  __attribute__((section(".WMATableRunDram, \"aw\"")));
//    #else
//        EXT unsigned int   WMATableRunDram[0x25d0]  __attribute__((section(".WMATableRunDram, \"aw\"")));
//    #endif
//#else
//        EXT unsigned int   WMATableRunDram[0x25d0]  __attribute__((section(".WMATableRunDram, \"aw\"")));
//#endif
extern int g_WmaHas_DRM;
extern int g_WmaFileRate;

extern long     TotalPlayTime;
extern tWMAFileHdrStateInternal ghdr_parse;
extern short WMAHuffmanTableBase16OB ;//zyf 2005-12-07
extern short WMAHuffmanTableBase44OB  ;//zyf 2005-12-07
extern short WMAHuffmanTableBase44QB  ;//zyf 2005-12-07
short WMAEQ3DInit(short Freq,short index,short con);
//------------------------------------------------------------------------------
#endif
//******************************************************************************
