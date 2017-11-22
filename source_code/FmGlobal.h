/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FmGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef  _FMGLOBAL_H
#define  _FMGLOBAL_H

#include "FmMacro.h"
#include "FmStruct.h"

#undef	EXT
#ifdef	_IN_FMSCHEDULE
		#define	EXT
#else
		#define	EXT		extern
#endif
//------------------------------------------------------------------------------
EXT unsigned int    FmStereoCount __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmStereoCount1 __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmStereoState __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmSensitive  __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmGuageVal __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmFreqMaxVal __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmFreqMinVal __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    UpDownFlag __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned char   IFReadDataH[5]  __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned char   IFReadDataL[5] __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned char   TEA5767ControlData[5] __attribute__((section(".fm_bss, \"aw\"")));
EXT FM_DISPLAY_STRUCT FmDisplayBlock __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned long   FmDisplayFlag  __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmBlinkingCounter __attribute__((section(".fm_bss, \"aw\"")));

EXT unsigned int    FmDialogFlag      __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmDialogVal       __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmBufCont         __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmBufMode         __attribute__((section(".fm_bss, \"aw\"")));
EXT unsigned int    FmFunctionSele    __attribute__((section(".fm_bss, \"aw\""))); 
EXT unsigned int    CurCh             __attribute__((section(".fm_bss, \"aw\""))); 
EXT unsigned int    PrevCh            __attribute__((section(".fm_bss, \"aw\""))); 
EXT unsigned int    FmGuageX          __attribute__((section(".fm_bss, \"aw\""))); 
//------------------------------------------------------------------------------
#endif
//******************************************************************************

