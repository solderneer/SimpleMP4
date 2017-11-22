/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : BrowserStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _BROWSERSTRUCT_H
#define _BROWSERSTRUCT_H

#include "Config.h"
#define MAX_PATH_NAME  (2+MAX_DIR_DEPTH*(11+1)+4)
//------------------------------------------------------------------------------
typedef struct _BROWSER_STRUCT{
    struct _BROWSER_STRUCT *pPrev;
	struct _BROWSER_STRUCT *pNext;
	
	unsigned int  FileType;
    unsigned int  ItemNumber;
	unsigned char LongFileName[MAX_FILENAME_LEN];
}BROWSER_STRUCT;//TestBrowserStruct;

typedef struct _DIR_TREE_STRUCT{
    unsigned char bBrwoserFlag;
    
	BROWSER_STRUCT *pBrowser;
	
	unsigned int MemMedium;     /*  zero For Flash, one For SD Card   */
	unsigned int MemSelect;
	
	unsigned int DirTotalItem;
	unsigned int DirDeep;
	unsigned int DirNum;
	unsigned int FileNum;
	unsigned int CurId[MAX_DIR_DEPTH];
	unsigned int ScrollBar;

    FDT          CurFileFDT;
	unsigned int CurPickFileType[17];
	unsigned int PathBuf[MAX_PATH_NAME];
    unsigned int KeyCounter;
}DIR_TREE_STRUCT;//TestTreeStruct;
//------------------------------------------------------------------------------
#endif
//******************************************************************************
