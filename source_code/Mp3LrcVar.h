/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3LrcVar.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/14       1.0            ORG
********************************************************************************
*/
#ifndef     _MP3LRCVAE_H
#define     _MP3LRCVAR_H

#undef  EXT
#ifdef _IN_MP3LRC
#define EXT
#else
#define EXT extern
#endif
//-----------------------------------------------------------------------------
typedef union {
    struct 
    {
        unsigned int    FileEnd:         1,
                        bReserved:       15;
    }bc;
    unsigned int word;
} LRC_FLAG;
//-----------------------------------------------------------------------------
EXT unsigned int LrcReadoutBuf[512] __attribute__((section(".lrc_bss, \"aw\"")));
EXT unsigned int LrcBuffer[512] __attribute__((section(".lrc_bss, \"aw\"")));

EXT unsigned int LrcBufferLen __attribute__((section(".lrc_bss, \"aw\"")));
EXT unsigned int LrcBufferOffset __attribute__((section(".lrc_bss, \"aw\"")));

EXT unsigned long PrevLrcPlayTimeSec __attribute__((section(".lrc_bss, \"aw\"")));
EXT unsigned long NextLrcPlayTimeSec __attribute__((section(".lrc_bss, \"aw\"")));

EXT LRC_FLAG LrcFlag __attribute__((section(".lrc_bss, \"aw\"")));
//-----------------------------------------------------------------------------
#endif
//******************************************************************************

