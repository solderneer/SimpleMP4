
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Global.h"
#include "Dma.h"

#ifdef AVI_DECODE_MODULE
    #include "AviGlobal.h"
#endif

#define     CHKBIT(x,bit) ((x&(1<<bit)) ? 1:0)
#define     SETBIT(x,bit) ((x)|=(1<<bit))
#define     CLRBIT(x,bit) ((x)&=(~(1<<bit)))

//#define  DMA_DISABLE_INTERRUPT      { read_creg(%imask, imask_tmp); bitclear_creg(%imask, 15); }
//#define  DMA_ENABLE_INTERRUPT       { write_creg(%imask, imask_tmp);}
//
////#define  DMA_DISABLE_INTERRUPT 
////#define  DMA_ENABLE_INTERRUPT  

unsigned int    *DmaResFlag[DMA_MAX_CHANNEL]                   DMA_BSS_SECTION;
DMA_CHANNEL_REG DmaTab1[DMA_FIFO_BUF_SIZE/2]                     DMA_BSS_SECTION;
DMA_CHANNEL_REG DmaTab2[DMA_FIFO_BUF_SIZE]                     DMA_BSS_SECTION;
DMA_FNCT        DmaChannelFnct[DMA_MAX_CHANNEL]                DMA_BSS_SECTION;
                                                               
                                                               
unsigned int    volatile    DmaChannelState[DMA_MAX_CHANNEL]   DMA_BSS_SECTION;
//unsigned int    DmaBufInIx[DMA_MAX_CHANNEL]                    DMA_BSS_SECTION;
//unsigned int    DmaBufOutIx[DMA_MAX_CHANNEL]                   DMA_BSS_SECTION; 
//unsigned int    volatile    DmaNRead[DMA_MAX_CHANNEL]          DMA_BSS_SECTION;

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : DMA_Init()
*  Author:         ZhenFu Fang
*  Description:    DMA 初始化
*  Calls:          无
*  Input:          无
*  Return:         无
*  Remark:         无
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/12/19      1.0           ORG
*
***********************************************************
*/
unsigned int  DmaSuspendFlag = 0;

int DmaResumeStatus(void)
{
    return DmaSuspendFlag;
}

void DmaResumeClear(void)
{
    DmaSuspendFlag = 0;
}

#if(FLASHROM_TYPE == _MLC_)
unsigned int DmaSuspendStatus(void)
{
    return 0;    
}
#endif

DMA_TEXT_SECTION
void DmaInit(void){
    unsigned int i;

    DmaChannelState[0] = 0;
    DmaChannelState[1] = 0;
    
    for(i=0; i<DMA_MAX_CHANNEL; i++){
        DmaResFlag[i]           = 0xFFFF;
    }
    for(i=0; i<DMA_FIFO_BUF_SIZE/2; i++){
        DmaTab1[i].pResFlag = 0xFFFF;
    }
    for(i=0; i<DMA_FIFO_BUF_SIZE; i++){
        
        DmaTab2[i].pResFlag = 0xFFFF;
    }
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : DMA_ChannelSetup()
*  Author:         ZhenFu Fang
*  Description:    DMA 通道设置
*  Calls:          无
*  Input:          i：  DMA通道
*                  pDma DMA寄存器结构体指针
*  Return:         无
*  Remark:         DMA　总是以中断方式存在
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/12/19      1.0           ORG
*
***********************************************************
*/
DMA_TEXT_SECTION
void DmaChannelSetup(unsigned int channel)
{
    unsigned int DmaRegAddrBase;
    unsigned int tmp = 0;
    unsigned int i;
//    unsigned int imask_tmp;
    
    DMA_CHANNEL_REG *pDmaReg = DMA_CHANNEL_FULL;
    if(channel >= 3)
        return;
    if( DmaChannelState[channel] !=0)
        return;
    if(channel == 0 ){
	    for(i=0; i<DMA_FIFO_BUF_SIZE/2; i++){
	    	if((DmaTab1[i].pResFlag != DMA_CHANNEL_FULL) && (DmaTab1[i].pResFlag != DMA_BUF_IREQ)){
    			if(DmaTab1[i].Priority >=tmp){
    				tmp = DmaTab1[i].Priority;
    				pDmaReg = &DmaTab1[i];
	    		}
	    	}
	    }
	}else{
	    for(i=0; i<DMA_FIFO_BUF_SIZE; i++){
	    	if((DmaTab2[i].pResFlag != DMA_CHANNEL_FULL) && (DmaTab2[i].pResFlag != DMA_BUF_IREQ)){
    			if(DmaTab2[i].Priority >= tmp){
    				tmp = DmaTab2[i].Priority;
    				pDmaReg = &DmaTab2[i];
	    		}
	    	}
	    }
    }
    if(pDmaReg ==DMA_CHANNEL_FULL){
    	return;
    }
//    DMA_DISABLE_INTERRUPT;
    	DmaRegAddrBase = 0xFE00 + channel*44;   // 44 is One Dma Channel Reg    
        *pDmaReg->pResFlag  = 2;
        DmaChannelState[channel] = 1;
        DmaResFlag[channel] = pDmaReg->pResFlag;
        pDmaReg->pResFlag = DMA_CHANNEL_FULL;
        //-----------------------------------------------------------------
        DmaChannelFnct[channel].pDmaFnct    = pDmaReg->pDmaFnct; 
        DmaChannelFnct[channel].pDmaFnctArg = pDmaReg->pDmaFnctArg; 
        //----------------------------------------------------------------- 
         
        write_mem32(DmaRegAddrBase,      pDmaReg->DmaSar);          //DMAR_SAR
        
        write_mem32(DmaRegAddrBase+0x04, pDmaReg->DmaDar);          //DMAR_DAR
        
        write_mem32(DmaRegAddrBase+0x0C, pDmaReg->DmaCtl.Lsb);      //DMAR_CTL
        write_mem32(DmaRegAddrBase+0x0E, pDmaReg->DmaCtl.Msb);      

        write_mem32(DmaRegAddrBase+0x20, pDmaReg->DmaCfg.Lsb);      //DMAR_CFG
        write_mem32(DmaRegAddrBase+0x22, pDmaReg->DmaCfg.Msb);
              
        write_mem32(DmaRegAddrBase+0x24, pDmaReg->DmaSgr);      //DMAR_SGR
        
        write_mem32(DmaRegAddrBase+0x28, pDmaReg->DmaDsr);      //DMAR_DSR

        write_mem(DMAR_DmaCfgReg, 0x0001);

        write_mem(DMAR_MaskBlock, (0x0101<<channel));

        if((channel == 0) && (MSG_FUNCTION1_AVI_DECODE == Fun1MsgArray[0])){
            write_XDATA(DMAR_CFG1,(read_XDATA(DMAR_CFG1) | 0x100));
        }
//------------------------------------------------------------------------------        
        if(0 != DmaSuspendStatus()){
            if(channel == 0){
                write_XDATA(DMAR_CFG0, (read_XDATA(DMAR_CFG0) | 0x0100));
            }else{
                write_XDATA(DMAR_CFG1, (read_XDATA(DMAR_CFG1) | 0x0100));
            }
            DmaSuspendFlag |= 0x0001<<channel;
        } 
//------------------------------------------------------------------------------        
        write_mem(DMAR_ChEnReg, (0x0101<<channel));
//    DMA_ENABLE_INTERRUPT;
}


/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : DMA_IsrProc()
*  Author:         ZhenFu Fang
*  Description:    DMA 中断处理函数
*  Calls:          无
*  Input:          无
*  Return:         无
*  Remark:         应把此函数入在DMA中断中调用
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/12/19      1.0           ORG
*
***********************************************************
*/

//#ifndef DMA_MANAGE_SYSTEM
        
DMA_TEXT_SECTION
void DMA_IsrProc(void)
{
    unsigned int     dma_temp;
    
    void (*pDmaFnctTmp)(void *);
    void *pDmaFnctArgTmp;   
    
    
    pDmaFnctTmp    = DMA_FNCT_NULL;
    pDmaFnctArgTmp = DMA_FNCT_NULL;
    
    dma_temp = read_mem(DMAR_StatusBlock);
//    if(MSG_FUNCTION1_AVI_DECODE == Fun1MsgArray[0])
//	    write_XDATA(DMAR_CFG1,(read_XDATA(DMAR_CFG1) & 0xfeff));    
    do {
        if(dma_temp & 0x0004){
            write_mem(DMAR_ClearBlock,0x0004);
            switch (Fun1MsgArray[0]) {
#ifdef MUSIC_MODULE        
        #ifdef MP3_DECODE_MODULE        
                case MSG_FUNCTION1_MUSIC_MP3:
                    PCMOutBufferChange();
                    break;
        #endif
        
        #ifdef WMA_DECODE_MODULE           
                case MSG_FUNCTION1_MUSIC_WMA:
                    WmaPCMOutBufferChange();
                    break;
        #endif
        
        #ifdef WAV_DECODE_MODULE    
                case MSG_FUNCTION1_MUSIC_WAV:
                    WavPCMOutBufferChange();
                    break;
        #endif
                            
#endif         

#ifdef AVI_DECODE_MODULE           
                case MSG_FUNCTION1_AVI_DECODE:
                    AviMp3DmaOutBufferChange();   //005-12-22 Test
                    break;
#endif

#ifdef ENCODE_MODULE            
                case MSG_FUNCTION1_ENCODE_PROC:
                    EncodePcmBufferChange();
                    break;
#endif                  
            }
        }        
        if(dma_temp & 0x0001){
            if (Fun1MsgArray[0] == MSG_FUNCTION1_USB_TRANS) {
                DmaUsbIsr();
            } else {
                if(0 != DmaSuspendStatus()){
                    if(read_XDATA(DMAR_CFG1) & 0x0100){
                        DmaSuspendFlag |= 0x0001<<1;
                    }
                }else{
                    write_XDATA(DMAR_CFG1,(read_XDATA(DMAR_CFG1) & 0xfeff));
                }
                write_mem(DMAR_ClearBlock, 0x0001);

                if(DmaResFlag[0] != 0xFFFF){
                    *DmaResFlag[0] = 0;
                    DmaResFlag[0]  = 0xFFFF;
                }
                DmaChannelState[0] = 0;
                
                pDmaFnctTmp    = DmaChannelFnct[0].pDmaFnct;
                pDmaFnctArgTmp = DmaChannelFnct[0].pDmaFnctArg;
                
                DmaChannelFnct[0].pDmaFnct    = DMA_FNCT_NULL;
                DmaChannelFnct[0].pDmaFnctArg = DMA_FNCT_NULL;
                
                if(pDmaFnctTmp != DMA_FNCT_NULL){
                    (*pDmaFnctTmp)(pDmaFnctArgTmp);
                    pDmaFnctTmp    = DMA_FNCT_NULL;
                    pDmaFnctArgTmp = DMA_FNCT_NULL;
                }
                if(DmaChannelState[0] == 0){
                	DmaChannelSetup(0);
                } 
            }   
        }   
        if(dma_temp & 0x0002){
            write_mem(DMAR_ClearBlock,0x0002);
            
            if(DmaResFlag[1] != 0xFFFF){
                *DmaResFlag[1] = 0;
                DmaResFlag[1]  = 0xFFFF;
            }
            DmaChannelState[1] = 0;
            pDmaFnctTmp    = DmaChannelFnct[1].pDmaFnct;
            pDmaFnctArgTmp = DmaChannelFnct[1].pDmaFnctArg;
                
            DmaChannelFnct[1].pDmaFnct    = DMA_FNCT_NULL;
            DmaChannelFnct[1].pDmaFnctArg = DMA_FNCT_NULL;
                
            if(pDmaFnctTmp != DMA_FNCT_NULL){
                (*pDmaFnctTmp)(pDmaFnctArgTmp);
                pDmaFnctTmp    = DMA_FNCT_NULL;
                pDmaFnctArgTmp = DMA_FNCT_NULL;
            }
            if(DmaChannelState[1] == 0){
            	DmaChannelSetup(1);
            }
        } 
    }while((dma_temp = read_mem(DMAR_StatusBlock)) & 0x0007);
}           

DMA_CHANNEL_REG * GetDmaChannelState(unsigned int channel){
    unsigned int i;
    unsigned int imask_tmp;
    DMA_DISABLE_INTERRUPT;
    if(channel ==0){
    	for(i=0; i<DMA_FIFO_BUF_SIZE/2; i++){
    		if(DmaTab1[i].pResFlag == DMA_CHANNEL_FULL){
                DmaTab1[i].pResFlag   = DMA_BUF_IREQ;
                DMA_ENABLE_INTERRUPT;
    			return &DmaTab1[i];
    		}
     	}
    }else{
    	for(i=0; i<DMA_FIFO_BUF_SIZE; i++){
    		if(DmaTab2[i].pResFlag == DMA_CHANNEL_FULL){
                DmaTab2[i].pResFlag   = DMA_BUF_IREQ;
                DMA_ENABLE_INTERRUPT;
    			return &DmaTab2[i];
    		}
     	}
    }
    DMA_ENABLE_INTERRUPT;
    return DMA_CHANNEL_FULL;
}