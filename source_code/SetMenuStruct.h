/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : SetMenuStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _SETMENUSTRUCT_H
#define _SETMENUSTRUCT_H

typedef void SET_MENU_FUNC(unsigned int key, unsigned long flash_addr, unsigned int *pValue);
//------------------------------------------------------------------------------
typedef struct SET_MENU_STRUCT{
	unsigned int 		id;
	unsigned long       DispInNFAddr;
	unsigned int        *AdjustValue;
	SET_MENU_FUNC       *pFunc;
	struct SET_MENU_STRUCT	*per, *next;
	struct SET_MENU_STRUCT	*father, *son;
}SET_MENU;

//------------------------------------------------------------------------------
#endif
//******************************************************************************

