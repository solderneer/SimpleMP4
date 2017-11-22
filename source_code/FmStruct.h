/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FmStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef  _FMSTRUCT_H
#define  _FMSTRUCT_H
//------------------------------------------------------------------------------
typedef union {
    struct 
    {
        unsigned int    
                        bReserved:        16;
    }bc;
    unsigned int word;
} _FM_FLAG;

typedef union {
    struct 
    {
    unsigned int  bInterface:               1,
                  bHold:                    1,
                  bMute:                    1,
                  bVol:                     1,
                  bBatt:                    1,
                  
                  bMode:                  1,
                  bCh:                      1,
                  bFmFreq:                  1,
                  bFmGuage:                 1,
                  bFmRegion:                1,
                  bFmStereo:                1,
                    
                  bFmMenuSelect:            1,
                  bReserved:                4;
    }bit;
    unsigned int  word;
}FM_DISPLAY_FLAG;

typedef struct{
    FM_DISPLAY_FLAG  DisplayFlag;
    unsigned int    *pHold;
    unsigned int    *pMute;
    unsigned int    *pVol;
    unsigned int    *pBatt;
    unsigned int    *pMode;
    unsigned int    *pCh;
    unsigned int    *pFreq;
    unsigned int    *pGuage;
    unsigned int    *pFreqMin;
    unsigned int    *pFreqMax;
    unsigned int    *pRegion;
    unsigned int    *pStereo;
}FM_DISPLAY_STRUCT;
//------------------------------------------------------------------------------
#endif
//******************************************************************************

