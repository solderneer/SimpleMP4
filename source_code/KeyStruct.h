/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : KeyStruct.h
* Author     : tonyzyz
* Description: This Head File Define the Global Varibal For MP3 Decode and WMA Decode
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _KEYSTRUCT_H
#define _KEYSTRUCT_H

//------------------------------------------------------------------------------
typedef union {
    struct 
    {       
        unsigned int    bKeyProssed:         1,
                        bReserved:           15;
    }bc;
    unsigned int word;
} KEY_FLAG;

typedef struct MENU_STRUCT{
	unsigned int 		id;
	unsigned long 		DispInNFAddr;
	struct MENU_STRUCT	*per, *next;
	struct MENU_STRUCT	*father, *son;
}MENU;


//------------------------------------------------------------------------------
#endif
//******************************************************************************
