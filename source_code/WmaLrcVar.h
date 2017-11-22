/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaLrcVar.h
* Author     : 
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
********************************************************************************
*/
#ifndef     _WMALRCVAE_H
#define     _WMALRCVAR_H

#undef  EXT
#ifdef _IN_WMALRC
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
} WMA_LRC_FLAG;
//-----------------------------------------------------------------------------
EXT unsigned int WmaLrcReadoutBuf[512] __attribute__((section(".wmalrc_bss, \"aw\"")));
EXT unsigned int WmaLrcBuffer[512] __attribute__((section(".wmalrc_bss, \"aw\"")));

EXT unsigned int WmaLrcBufferLen __attribute__((section(".wmalrc_bss, \"aw\"")));
EXT unsigned int WmaLrcBufferOffset __attribute__((section(".wmalrc_bss, \"aw\"")));

EXT unsigned long WmaPrevLrcPlayTimeSec __attribute__((section(".wmalrc_bss, \"aw\"")));
EXT unsigned long WmaNextLrcPlayTimeSec __attribute__((section(".wmalrc_bss, \"aw\"")));

EXT WMA_LRC_FLAG WmaLrcFlag __attribute__((section(".wmalrc_bss, \"aw\"")));
//-----------------------------------------------------------------------------
#endif
//******************************************************************************

