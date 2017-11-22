/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : TxtGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _TXTGLOBAL_H
#define _TXTGLOBAL_H

#include "TxtMacro.h"
#include "TxtStruct.h"

#undef  EXT
#ifdef _IN_TXTSCHEDULE
#define EXT
#else
#define EXT     extern
#endif
//------------------------------------------------------------------------------
EXT TxtDispBufStruct  TxtBuf[TXT_DISP_LINE_MAX] __attribute__((section(".txt_bss, \"aw\"")));
EXT TxtStruct         Text __attribute__((section(".txt_bss, \"aw\"")));
//------------------------------------------------------------------------------
EXT long          TxtOffsetBuf[TXT_OFFSET_BUF_MAX]      __attribute__((section(".txt_bss, \"aw\"")));
EXT unsigned int  TxtOffsetCont                         __attribute__((section(".txt_bss, \"aw\"")));
EXT long          TxtBypePerOffset                      __attribute__((section(".txt_bss, \"aw\"")));
EXT unsigned int  TxtFileFormatError                    __attribute__((section(".txt_bss, \"aw\"")));
EXT unsigned int  TxtPrevScrollY                        __attribute__((section(".txt_bss, \"aw\"")));

EXT unsigned int  TxtAutoBrowserTimeFlag                __attribute__((section(".txt_bss, \"aw\"")));
EXT unsigned int  TextAutoPlayFlag                      __attribute__((section(".txt_bss, \"aw\"")));
#endif
//******************************************************************************
