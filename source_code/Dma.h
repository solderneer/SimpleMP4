#ifndef _DMA_H
#define _DMA_H

#include "DMA_Macro.h"

#define  DMA_MANAGE_SYSTEM

#ifdef  DMA_MANAGE_SYSTEM
    #define DMA_TEXT_SECTION
    #define DMA_BSS_SECTION
#else
    #define DMA_TEXT_SECTION    __attribute__((section(".avi_v_text, \"ax\"")))
    #define DMA_BSS_SECTION     __attribute__((section(".avi_v_bss, \"aw\"")))
#endif

#define  DMA_DISABLE_INTERRUPT      { read_creg(%imask, imask_tmp); bitclear_creg(%imask, 15); }
#define  DMA_ENABLE_INTERRUPT       { write_creg(%imask, imask_tmp);}

typedef struct{
    void (*pDmaFnct)(void *);
    void *pDmaFnctArg;
}DMA_FNCT;

typedef struct{
    unsigned long TanSize;
    unsigned int  BlockSize;
}DMA_CHANNEL_TAN_DATA;


typedef struct{
    unsigned long Lsb;
    unsigned long Msb;
}REG_64BITS;

typedef struct{
    void (*pDmaFnct)(void *);               //0
    void *pDmaFnctArg;                      //1
    
    unsigned int  *pResFlag;                //2
    unsigned int  Priority;                 //3
    
    unsigned long DmaSar;                   //4
    unsigned long DmaDar;                   //6
    
    REG_64BITS    DmaCtl;                   //8
    REG_64BITS    DmaCfg;                   //12
    
    unsigned long DmaSgr;                   //16
    unsigned long DmaDsr;                   //18

    //unsigned int  DmaCounter;
    //unsigned long DmaPerSize;
    //unsigned int  DmaBlockSize;
}DMA_CHANNEL_REG;


extern unsigned int    *DmaResFlag[DMA_MAX_CHANNEL]              DMA_BSS_SECTION;
extern unsigned int    volatile DmaChannelState[DMA_MAX_CHANNEL] DMA_BSS_SECTION;
extern DMA_FNCT                 DmaChannelFnct[DMA_MAX_CHANNEL]  DMA_BSS_SECTION;    


void            DmaInit(void);
//DMA_CHANNEL_REG *DmaBufOut(unsigned int channel);
void            DmaChannelSetup(unsigned int channel);
void            DMA_IsrProc(void);
DMA_CHANNEL_REG *GetDmaChannelState(unsigned int channel);

#endif

