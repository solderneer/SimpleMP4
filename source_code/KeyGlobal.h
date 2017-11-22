/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : KeyMacro.h
* Author     : tonyzyz
* Description: This Head File Define the Global Varibal For MP3 Decode and WMA Decode
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _KEYGLOBAL_H
#define _KEYGLOBAL_H

#include "KeyStruct.h"

#undef  EXT
#ifdef  _IN_KEY
#define EXT
#else 
#define EXT extern  
#endif
//------------------------------------------------------------------------------
EXT KEY_FLAG KeyFlag;

EXT unsigned int KeyVal;
EXT unsigned int PrevKeyVal;
EXT unsigned int KeyStatus;
EXT unsigned int KeyScanCnt;

//EXT MENU	*pMenu;
//EXT unsigned int SelectItem;
//EXT unsigned int SelectAdjust;
//EXT unsigned char F_MenuAdjust;
//#define KeyDownTmr     KeyVal

//------------------------------------------------------------------------------
#endif
//******************************************************************************
