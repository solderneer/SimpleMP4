/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : TxtStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _TXTSTRUCT_H
#define _TXTSTRUCT_H

#include "Config.h"
#include "TxtMacro.h"

#define MAX_PATH_NAME  (2+MAX_DIR_DEPTH*(11+1)+4)

//------------------------------------------------------------------------------

typedef struct TXT_DISP_BUF_STRUCT{
    unsigned int    BytesPerLine;
    unsigned int    TxtDispBuf[TXT_PER_LINE_CHARS];
    struct          TXT_DISP_BUF_STRUCT *pPerv;
    struct          TXT_DISP_BUF_STRUCT *pNext;
}TxtDispBufStruct;

typedef struct TXT_STRUCT{
    TxtDispBufStruct    *pTxtBuf;
    long                NFBookMark[TXT_BOOKMARK_COUNTER];
    unsigned int        BookMarkBuf[TXT_BOOKMARK_COUNTER][TXT_BOOKMARK_BUF_SIZE];
    unsigned char       BookMarkCont;
    unsigned char       BookMarkType;
    long                StartFileOffset;
    long                EndFileOffset;
    unsigned char       FileInMemType;
    unsigned char       FilePath[MAX_PATH_NAME];
    unsigned char       FileName[MAX_FILENAME_LEN]; 
    long                FileSize;
    unsigned char       FileHandle;
    unsigned int        FileCtr;
}TxtStruct;



//------------------------------------------------------------------------------
#endif
//******************************************************************************
