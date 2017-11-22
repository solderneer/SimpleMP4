/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : BrowserGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _BROWSERGLOBAL_H
#define _BROWSERGLOBAL_H

#include "BrowserMacro.h"
#include "BrowserStruct.h"
#include "file.h"

#undef  EXT
#ifdef _IN_BROWSERSCHEDULE
#define EXT
#else
#define EXT     extern
#endif
//------------------------------------------------------------------------------
EXT BROWSER_STRUCT      BrowserItem[BROWSER_SCREEN_PER_LINE];
EXT DIR_TREE_STRUCT     DirTreeInfo;
EXT unsigned long 		CurDirClusBak;
EXT unsigned int        BrowserDispFlag;
EXT unsigned int        DeleteFileAdjust;
EXT unsigned int        DialogBoxAdjustValue;
EXT unsigned int        PrevScrollY;

EXT FIND_DATA           FindDataBrowser;


//EXT unsigned int        BroActFlag;
//------------------------------------------------------------------------------
#endif
//******************************************************************************
