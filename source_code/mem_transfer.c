/*****************************************************************************
 *
 *  XVID MPEG-4@QVGA VIDEO DECODER ON ROCKCHIP RK2606
 *	
 *	Author:
 *		Jian Huan	<jh@rock-chips.com>
 *
 *	Date:
 *		2005-11-26 10:22
 ****************************************************************************/
#include "Macro.h"
#include "Memap.h"
#include "Global.h"
#include "Resource.h"

#include "AviMacro.h"
#include "AviGlobal.h"

#include "Dma.h"

#include "mem_transfer.h"
#include "../static_data.h"
#include "../xvid_decoder.h"
#include "../xvid_macro.h"

/*********************************************************************
 *  Func:
 *      DMA_YUV_I2E
 *  Description:
 *      Internal SRAM to External SDRAM
 *  Author:
 *      Fang ZF
 *  Date:
 *      2006-1-3 21:30
 *  Modify:
 *      Jian Huan
 *  Date:
 *      2006-2-15 9:21
 *********************************************************************/

//__attribute__((section(".avi_v_text, \"ax\"")))
//void DMA_YUV_I2E(DECODER *xvid_dec,short y, short size)
//{
//    unsigned int     temp2;
//    DMA_CHANNEL_REG *DmaChl;
//    
//    unsigned long dstY=0, dstUV=0;
//
//    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
//    
//    dstY = (unsigned long )((unsigned long )(xvid_dec->cur->y_page)<<16) + (unsigned long)(y)*(unsigned long)(size);          //????SDRAM????????Y????????????
//    
//    temp2 = 0x8c00-320*4*y;
//    //---------------------------------------------------------------------
//    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//    //---------------------------------------------------------------------    
//
//    DmaChl->DmaSar = (unsigned long)(((unsigned long)(IntMem.mb_y_row)<<1)+(unsigned long)0x40000);
//    DmaChl->DmaDar = ((unsigned long)(dstY)<<1)+(unsigned long)0x4000000;
//    
//    DmaChl->DmaCtl.Lsb = 0x00040025;
//    DmaChl->DmaCtl.Msb = 0x00000F00;
//    
//    DmaChl->DmaCfg.Lsb = 0x00000020;
//    DmaChl->DmaCfg.Msb = 0x00001004;
//    
//    DmaChl->DmaDsr = 0xA0000000|temp2;
//    
//    Dma_I2EBlock   = 1;
//    
//    DmaChl->Priority   = 0;
//    
//    DmaChl->pResFlag   = &Dma_I2EBlock;
//    
//    CALL_DmaChannelSetup(0);
//}
//
///***************************************************************************
// *  Func:
// *     DMA_YUV_E2I_A
// *  Description:
// *      index: 0-->y;  2-->uv
// *      dst_offset_pos: offset position of refer-image
// *  NB:
// *      The Size of Match_Block is 18X17 or 18X9
// *  Author:
// *      fang zf
// *      jian huan   (modify) 
// *  Date:
// *      2006-1-3 14:47
// *      2006-1-9 10:49  (modify)
// **************************************************************************/  
//__attribute__((section(".avi_v_text, \"ax\"")))
//void DMA_YUV_E2I_A(short index,   short   dst_offset_pos)       //32BIT--32BIT
//{
//    short   vert_in, hor_in, hor_out;
//    unsigned long   src;
//    unsigned short  dst;
//    IMAGE *refimg = Xvidec.ref;
//    DMA_CHANNEL_REG *DmaChl;
//    unsigned int  resflag;
//        
//    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
// 
//    
//   if(index == 0){ 
//        Global_Offset = (unsigned short)(src_offset_pos & 0x1);
//        src = (((unsigned long)(refimg->y_page))<<16) + (src_offset_pos - (unsigned long)Global_Offset);
//        dst = &match_y_block[0];  
//        resflag   = &Dma_E2IBlock_Y;
//        Dma_E2IBlock_Y = 1;
//        //hor_in  = 18;
//        vert_in = 17;
//    }else if(index == 2){
//        src = (((unsigned long)(refimg->uv_page))<<16) + 0x2c00 + src_offset_pos;
//        dst = &match_uv_block[0];       
//        resflag   = &Dma_E2IBlock_UV;
//        Dma_E2IBlock_UV = 1;
//        //hor_in  = 18;
//        vert_in = 9;
//    }
////---------------------------------------------------------------------
//    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
////---------------------------------------------------------------------    
//    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);
//    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
//    
//    DmaChl->DmaCtl.Lsb = 0x00020025;         //32-bit transfer
//    DmaChl->DmaCtl.Msb = (unsigned long)(9*(unsigned long)vert_in);
//    
//    DmaChl->DmaCfg.Lsb = 0x00000020;
//    DmaChl->DmaCfg.Msb = 0x00001004;
//    
//    //DmaChl.DmaSgr.Lsb = (unsigned long)(160-9)|((unsigned long)(9)<<20);
//    DmaChl->DmaSgr = 0x00900097;
//    
//    DmaChl->Priority   = 1;
//    
//    DmaChl->pResFlag   = resflag;
//    
//    CALL_DmaChannelSetup(0);
//    
//}
//
///***************************************************************************
// *  Func:
// *     DMA_YUV_E2I_B
// *  Description:
// *      index: 0-->y;  2-->uv
// *      dst_offset_pos: offset position of refer-image
// *
// *  Author:
// *      fang zf
// *  Date:
// *      2006-1-3 14:47
// **************************************************************************/  
//__attribute__((section(".avi_v_text, \"ax\"")))
//void DMA_YUV_E2I_B(short index,   short   dst_offset_pos)           //16BIT--16BIT
//{
//    short   vert_in, hor_in, hor_out;
//    unsigned long   src;
//    unsigned short  dst;
//    PIXEL_COMP_STRUCT   *pcs = &PCS, *pcs_uv = &PCS_UV;
//    IMAGE *refimg = Xvidec.ref;
//    unsigned int  resflag;
//    DMA_CHANNEL_REG *DmaChl;
//    
//    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
//    
//   if(index == 0){              
//        src = (((unsigned long)(refimg->y_page))<<16) + src_offset_pos;
//        dst = &match_y_block[0] + dst_offset_pos;
//        resflag   = &Dma_E2IBlock_Y;
//        Dma_E2IBlock_Y = 1;
//        vert_in = pcs->vert_in;
//        hor_in  = pcs->hor_in; 
//        hor_out = pcs->hor_out;
//    }else if(index == 2){
//        src = (((unsigned long)(refimg->uv_page))<<16) + 0x2c00 + src_offset_pos;
//        dst = &match_uv_block[0] + dst_offset_pos;
//        resflag   = &Dma_E2IBlock_UV;
//        Dma_E2IBlock_UV = 1;
//        vert_in = pcs_uv->vert_in;
//        hor_in  = pcs_uv->hor_in; 
//        hor_out = pcs_uv->hor_out;
//    }
////---------------------------------------------------------------------
//    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
////---------------------------------------------------------------------    
//
//    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);
//    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
//    
//    DmaChl->DmaCtl.Lsb = 0x00060013;
//    DmaChl->DmaCtl.Msb = (unsigned long)(hor_in*vert_in);
//    
//    DmaChl->DmaCfg.Lsb = 0x00000020;
//    DmaChl->DmaCfg.Msb = 0x00001004;
//    
//    DmaChl->DmaSgr = (unsigned long)(320-hor_in)|((unsigned long)(hor_in)<<20);
//    
//    DmaChl->DmaDsr = ((unsigned long)(hor_in)<<20)|((unsigned long)(hor_out));
//    
//    DmaChl->Priority   = 1;
//
//    DmaChl->pResFlag   = resflag;
//    
//    CALL_DmaChannelSetup(0);
//}
//
//
///***************************************************************************
// *  Func:
// *     DMA_YUV_E2I_C
// *  Description:
// *      index: 0-->y;  2-->uv
// *      dst_offset_pos: offset position of refer-image
// *  NB:
// *      The Size of Match_Block is 10x9
// *  Author:
// *      fang zf
// *      jian huan   (modify) 
// *  Date:
// *      2006-1-3 14:47
// *      2005-1-11 11:53  (modify)
// **************************************************************************/  
//__attribute__((section(".avi_v_text, \"ax\"")))
//void DMA_YUV_E2I_C(short index, short dst_offset_pos)       //32BIT--32BIT
//{
//    short   vert_in, hor_in, hor_out;
//    unsigned long   src;
//    unsigned short  dst;
//    IMAGE *refimg = Xvidec.ref;
//    DMA_CHANNEL_REG *DmaChl;
//    
//    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
// 
//    Global_Offset = (unsigned short)(src_offset_pos & 0x1);
//    src = (((unsigned long)(refimg->y_page))<<16) + (src_offset_pos - (unsigned long)Global_Offset);
//    dst = &match_y_block[0];  
//    hor_in  = 10;
//    vert_in = 9;
////---------------------------------------------------------------------
//    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
////---------------------------------------------------------------------    
//    
//    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);
//    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
//    
//    DmaChl->DmaCtl.Lsb = 0x00020025;         //32-bit transfer
//    DmaChl->DmaCtl.Msb = (unsigned long)(45);
//    
//    DmaChl->DmaCfg.Lsb = 0x00000020;
//    DmaChl->DmaCfg.Msb = 0x00001004;
//    
//    DmaChl->DmaSgr = (unsigned long)(160-5)|((unsigned long)(5)<<20);
//    
//    Dma_E2IBlock_Y   = 1;
//    
//    DmaChl->Priority   = 1;
//    
//    DmaChl->pResFlag   = &Dma_E2IBlock_Y;
//    
//    CALL_DmaChannelSetup(0);
//}
//
//
///***************************************************************************
// *  Func:
// *      copy_y_from_external_to_internal
// *  Dsecripton:
// *      External SDRAM to Internal SRAM
// *  NB:
// *      32bit-32bit
// *  Author:
// *      Fang ZF
// *      Jian Huan (Modify)
// *  Date:
// *      2006-1-3 21:35
// *      2006-1-10 9:00
// **************************************************************************/
//__attribute__((section(".avi_v_text, \"ax\"")))
//void    copy_y_from_external_to_internal(DECODER *xvid_dec, short x, short y)
//{
//    unsigned long xy_cord;
//    unsigned long src, dst;
//    DMA_CHANNEL_REG *DmaChl;
//    
//    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
//    
//    xy_cord = (unsigned long)(xvid_dec->width) * ((unsigned long)(y)<<4) + ((unsigned long)(x)<<4);
//   
//    src = (unsigned long)(((unsigned long)(xvid_dec->ref->y_page)<<16)) + (unsigned long)xy_cord;       //128K/2 = 131072/2
//    dst = (unsigned long)IntMem.mb_y_row + ((unsigned long)(x)<<4);
////---------------------------------------------------------------------
//    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
////---------------------------------------------------------------------
//    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)0x4000000;
//    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
//    
//    DmaChl->DmaCtl.Lsb = 0x00060025;                 //0x00060013;
//    DmaChl->DmaCtl.Msb = 16*8;                       //16*16;
//    
//    DmaChl->DmaCfg.Lsb = 0x00000020;
//    DmaChl->DmaCfg.Msb = 0x00001004;
//    
//    DmaChl->DmaSgr = 0x00800098;                 //0x01000130;
//    
//    DmaChl->DmaDsr = 0x00800098;                 //0x01000130;
//    
//    Dma_16x16Block   = 1;
//    
//    DmaChl->Priority   = 2;
//
//    DmaChl->pResFlag   = &Dma_16x16Block;
//        
//    CALL_DmaChannelSetup(0);
//  
//}
//    
///***************************************************************************
// *  Func:
// *      copy_uv_from_external_to_internal
// *  Dsecripton:
// *      External SDRAM to Internal SRAM
// *  NB:
// *      32bit-32bit
// *  Author:
// *      Fang ZF
// *      Jian Huan   (Modify)
// *  Date:
// *      2006-2006-1-3 21:36
// *      2006-1-10 9:01
// **************************************************************************/
//__attribute__((section(".avi_v_text, \"ax\"")))
//void    copy_uv_from_external_to_internal(DECODER *xvid_dec, short x, short y)
//{
//    unsigned long   xy_cord;
//    unsigned long   src = 0, dst = 0;
//    DMA_CHANNEL_REG *DmaChl;
//    
//    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
//    
//    xy_cord = (unsigned long)(xvid_dec->width) * (unsigned long)(y<<3) + (unsigned long)(x<<4);
//
//    src = (unsigned long)((unsigned long)(xvid_dec->ref->uv_page)<<16) + 0x2c00 + (unsigned long)xy_cord;
//    dst = (unsigned long)IntMem.mb_uv_row + (unsigned long)(x<<4);
////---------------------------------------------------------------------
//    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
////---------------------------------------------------------------------
//
//    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)0x4000000;
//    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
//    
//    DmaChl->DmaCtl.Lsb = 0x00060025;                 //0x00060013;
//    DmaChl->DmaCtl.Msb = 16*4;                       //16*8;
//    
//    DmaChl->DmaCfg.Lsb = 0x00000020;
//    DmaChl->DmaCfg.Msb = 0x00001004;
//    
//    DmaChl->DmaSgr = 0x00800098;                 //0x01000130;
//    
//    DmaChl->DmaDsr = 0x00800098;                 //0x01000130;
//    
//    Dma_16x8Block   = 1;
//    
//    DmaChl->Priority   = 2;
//    
//    DmaChl->pResFlag   = &Dma_16x8Block;
//
//    CALL_DmaChannelSetup(0);
//
//}
//
///*********************************************************************
// *  Func:
// *      DMA_RGB_I2E
// *  Description:
// *      RGB Internal SRAM to External SDRAM
// *  Author:
// *      Jian H
// *  Date:
// *      2006-1-5 9:10
// *********************************************************************/
//__attribute__((section(".avi_v_text, \"ax\"")))
//void DMA_RGB_I2E(unsigned short *RgbBuf, short y, short size)
//{
//    unsigned long dstRGB;
//    unsigned long RGBPage;
//    DMA_CHANNEL_REG *DmaChl;
//
//    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 
//    
//    RGBPage = 262144 + ((unsigned long)AviRgbSdramIndex)*(unsigned long)76800;
//    dstRGB = (unsigned long )(RGBPage) + (unsigned long)(y)*(unsigned long)(16);          //????SDRAM????????Y????????????
//
////---------------------------------------------------------------------
//    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
////--------------------------------------------------------------------- 
//    
//    DmaChl->DmaSar = (unsigned long)(((unsigned long)(RgbBuf)<<1)+(unsigned long)0x40000);
//    DmaChl->DmaDar = ((unsigned long)(dstRGB)<<1)+(unsigned long)0x4000000;
//    
//    DmaChl->DmaCtl.Lsb = 0x00040025;              
//    DmaChl->DmaCtl.Msb = 0x00000A00;             //320*16/2
//    
//    DmaChl->DmaCfg.Lsb = 0x00000020;
//    DmaChl->DmaCfg.Msb = 0x00001004;
//    
//    DmaChl->DmaDsr = 0x00800070;
//    
//    Dma_RGB_I2E   = 1;
//    
//    DmaChl->Priority   = 2;
//
//    DmaChl->pResFlag   = &Dma_RGB_I2E;
//        
//    CALL_DmaChannelSetup(1);
//}

/*********************************************************************
 *  Func:
 *      DMA_RGB_E2E
 *  Description:
 *      RGB External SAM to LCD
 *  Author:
 *      Jian Huan
 *  Date:
 *      2006-1-5 10:01
 *********************************************************************/
unsigned int  LcdTanCtr;
 __attribute__((section(".avi_v_text, \"ax\"")))
void DMA_Sdram2LcdFnct(void *parg)
{
    unsigned long srcRGB;
    unsigned long RGBPage;
    DMA_CHANNEL_REG *DmaChl;
    unsigned long TanDataLen;
    
//------------------------------------------------------------------------------
//  ??????????????????
//------------------------------------------------------------------------------
    TanDataLen = 240*16;

    if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
        TanDataLen = (MAX_PIC_YSIZE - VIDEO_UPMAIN_PIC_YSIZE - VIDEO_DOWNMAIN_PIC_YSIZE) * 16;
    }  
//------------------------------------------------------------------------------  

    //RGBPage = 4 + (LCDSdramIndex<<1); 
    RGBPage = 262144 + ((unsigned long)LCDSdramIndex)*(unsigned long)76800; 

    if(LcdTanCtr > 0){
        
    	while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1)));     	
        
        if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
            srcRGB = (unsigned long )(RGBPage) + (unsigned long)(10-(LcdTanCtr))*240*32+VIDEO_UPMAIN_PIC_YSIZE;
        }else{
            srcRGB = (unsigned long )(RGBPage) + (unsigned long)(10-(LcdTanCtr))*240*32;
        }
        
        srcRGB = ((unsigned long)(srcRGB)<<1)+(unsigned long)0x4000000;
        
//---------------------------------------------------------------------
        DmaChl->pDmaFnct    = DMA_Sdram2LcdFnct;
        DmaChl->pDmaFnctArg = LcdTanCtr-1;
        LcdTanCtr = LcdTanCtr-1;
//---------------------------------------------------------------------     
        DmaChl->DmaSar = srcRGB; 
        DmaChl->DmaDar = ((unsigned long)(LCD_DATA)<<1);
    
        if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
            DmaChl->DmaCtl.Lsb = 0x00020123;
        }else{    
            DmaChl->DmaCtl.Lsb = 0x00000123;             //SRC_GATHER_EN:0 DINC:No change, SRC_TR_WIDTH:16-bit, DST_TR_WIDTH:16-bit, INT_EN:1
        }
        
        DmaChl->DmaCtl.Msb = TanDataLen;             //320*240/2
    
        DmaChl->DmaCfg.Lsb = 0x00000000;
        DmaChl->DmaCfg.Msb = 0x00001004;
    
        if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
            DmaChl->DmaSgr = (unsigned long)(((unsigned long)(((unsigned long)(MAX_PIC_YSIZE) - VIDEO_UPMAIN_PIC_YSIZE - VIDEO_DOWNMAIN_PIC_YSIZE) >> 1)<<20) | (unsigned long)((VIDEO_UPMAIN_PIC_YSIZE + VIDEO_DOWNMAIN_PIC_YSIZE) >> 1));//0x0dc00004  ;//2|(110<<20);       <jh@2006-3-2 9:12>
        }else{
            DmaChl->DmaSgr = 0x00000000  ;//2|(110<<20);       <jh@2006-3-2 9:12>
        }
    
        Dma_RGB_E2LCD   = 1;
        
        DmaChl->Priority   = 0;
        
        DmaChl->pResFlag   = &Dma_RGB_E2LCD; 
        
        CALL_DmaChannelSetup(1);
    }else{
//---------------------------------------------------------------------
//        DmaChl->pDmaFnct    = DMA_FNCT_NULL;
//        DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
        //---------------------------------------------------------------------   
//============================================================================//
        if(uiAviLcdDmaDoneFlag == 1){
            uiAviLcdDmaDoneFlag = 2;
        }
//============================================================================//
    }
}

 __attribute__((section(".avi_v_text, \"ax\"")))
void DMA_RGB_E2LCD(void)
{
    unsigned long srcRGB;
    unsigned long RGBPage;
    unsigned long TanDataLen;
    DMA_CHANNEL_REG *DmaChl;
    int     lcd_looptimes = 0;
        

    LCDSdramIndex = (LCDSdramIndex+1)%(AVI_RGB_SDRAM_BUF_CONT+1);  //zcs &1    
    
    RGBPage = 262144 + ((unsigned long)LCDSdramIndex)*(unsigned long)76800; 

    if(BITT_N_FRAME(&FRAME_TYPE_LABEL[0], LCDSdramIndex) != 0){
        BITC_N_FRAME(&FRAME_TYPE_LABEL[0],  LCDSdramIndex);
        return;
    }          
    
//------------------------------------------------------------------------------
//  ??????????????????
//------------------------------------------------------------------------------
    TanDataLen = 240*16;

    if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
        TanDataLen = (MAX_PIC_YSIZE - VIDEO_UPMAIN_PIC_YSIZE - VIDEO_DOWNMAIN_PIC_YSIZE)*16;
    }  
//------------------------------------------------------------------------------  
        
    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 

    if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
        srcRGB = (unsigned long )((unsigned long )(RGBPage))+VIDEO_UPMAIN_PIC_YSIZE;
    }else{
        srcRGB = (unsigned long )((unsigned long )(RGBPage));
    }

//-----------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_Sdram2LcdFnct;
    DmaChl->pDmaFnctArg = 9;
    LcdTanCtr           = 9;
//-----------------------------------------------------------------     
    DmaChl->DmaSar = ((unsigned long)(srcRGB)<<1)+(unsigned long)0x4000000; 
    DmaChl->DmaDar = ((unsigned long)(LCD_DATA)<<1);
    
    if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
        DmaChl->DmaCtl.Lsb = 0x00020123;
    }else{    
        DmaChl->DmaCtl.Lsb = 0x00000123;             //SRC_GATHER_EN:0 DINC:No change, SRC_TR_WIDTH:16-bit, DST_TR_WIDTH:16-bit, INT_EN:1
    }

    if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
        DmaChl->DmaSgr = (unsigned long)(((unsigned long)(((unsigned long)(MAX_PIC_YSIZE) - VIDEO_UPMAIN_PIC_YSIZE - VIDEO_DOWNMAIN_PIC_YSIZE) >> 1)<<20) | (unsigned long)((VIDEO_UPMAIN_PIC_YSIZE + VIDEO_DOWNMAIN_PIC_YSIZE) >> 1));//0x0dc00004  ;//2|(110<<20);       <jh@2006-3-2 9:12>
    }else{
        DmaChl->DmaSgr = 0x00000000;
    }

    DmaChl->DmaCtl.Msb = TanDataLen;             //320*240/2
    
    DmaChl->DmaCfg.Lsb = 0x00000000;
    DmaChl->DmaCfg.Msb = 0x00001004;
    
    Dma_RGB_E2LCD      = 1;
    
    DmaChl->Priority   = 0;
    
    DmaChl->pResFlag   = &Dma_RGB_E2LCD;
    
    CALL_DmaChannelSetup(1);
}

__attribute__((section(".avi_v_text, \"ax\"")))
unsigned int GetLCDSdramIndex()
{
	return LCDSdramIndex;
}
void CALL_DmaChannelSetup(unsigned int channel){
	unsigned int imask_tmp;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(channel);
    DMA_ENABLE_INTERRUPT;
}


