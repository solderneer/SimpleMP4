/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : SetMenuGlobal.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _SETMENUGLOBAL_H
#define _SETMENUGLOBAL_H

#undef  EXT
#ifdef _IN_SETMENUSCHEDULE
#define EXT
#else
#define EXT     extern
#endif
//------------------------------------------------------------------------------
EXT unsigned int SetMenuAdjust __attribute__((section(".setmenu_bss, \"aw\"")));

EXT unsigned int AValue __attribute__((section(".setmenu_bss, \"aw\"")));
EXT unsigned int *pAValue __attribute__((section(".setmenu_bss, \"aw\"")));
EXT unsigned int ALimit __attribute__((section(".setmenu_bss, \"aw\"")));
EXT unsigned int SetPrevScrollY __attribute__((section(".setmenu_bss, \"aw\"")));

EXT unsigned int SetMenuAdjust __attribute__((section(".setmenu_bss, \"aw\"")));
EXT unsigned int AdjustValueTmp __attribute__((section(".setmenu_bss, \"aw\"")));
EXT unsigned int SettingValueTmp __attribute__((section(".setmenu_bss, \"aw\"")));

EXT SET_MENU *pSetMenu __attribute__((section(".setmenu_bss, \"aw\"")));
//EXT SET_MENU *pSetMenu __attribute__((section(".setmenu_bss, \"aw\"")));

EXT unsigned int UserEqCont __attribute__((section(".setmenu_bss, \"aw\"")));
EXT unsigned int UserEqBufTmp[5] __attribute__((section(".setmenu_bss, \"aw\"")));

extern unsigned int UserEqBuf[5] __attribute__((section(".setmenu_bss, \"aw\"")));

//------------------------------------------------------------------------------
#endif
//******************************************************************************

