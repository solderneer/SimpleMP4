#include "xvid_section_def.h"
#include "Dma_Macro.h"
#include "Memap.h"
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!   Func:
!       DMA_YUV_I2E
!   Description:
!       Internal SRAM to External SDRAM
!  Author:
!      Fang ZF
!  Date:
!      2006-1-3 21:30
!  Modify:
!      Jian Huan
!  Date:
!      2006-2-15 9:21
!  Optimize:
!       Jian Huan
!  Date:
!       2006-7-14 9:46 
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!  Code Prototype:
!
!    __attribute__((section(".avi_v_text, \"ax\"")))
!    void DMA_YUV_I2E(DECODER *xvid_dec,short y, short size)
!    {
!        unsigned int     temp2;
!        DMA_CHANNEL_REG *DmaChl;
!        
!        unsigned long dstY=0, dstUV=0;
!    
!        while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
!        
!        dstY = (unsigned long )((unsigned long )(xvid_dec->cur->y_page)<<16) + (unsigned long)(y)*(unsigned long)(size);          //指向SDRAM里当前帧Y分量的地址　
!        
!        temp2 = 0x8c00-320*4*y;
!    //---------------------------------------------------------------------
!        DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!        DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
!    //---------------------------------------------------------------------    
!    
!        DmaChl->DmaSar = (unsigned long)(((unsigned long)(IntMem.mb_y_row)<<1)+(unsigned long)0x40000);
!        DmaChl->DmaDar = ((unsigned long)(dstY)<<1)+(unsigned long)0x4000000;
!        
!        DmaChl->DmaCtl.Lsb = 0x00040025;
!        DmaChl->DmaCtl.Msb = 0x00000F00;
!        
!        DmaChl->DmaCfg.Lsb = 0x00000020;
!        DmaChl->DmaCfg.Msb = 0x00001004;
!        
!        DmaChl->DmaDsr = 0xA0000000|temp2;
!        
!        Dma_I2EBlock   = 1;
!        
!        DmaChl->Priority   = 0;
!        
!        DmaChl->pResFlag   = &Dma_I2EBlock;
!        
!        CALL_DmaChannelSetup(0);
!    }
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
MP4_ASSEMBLE_SECTION
.global _DMA_YUV_I2E
.walign 2
_DMA_YUV_I2E:
            stdu        r0,     r12,    -2
            stdu        r2,     r12,    -2
            stdu        r8,     r12,    -2
            stdu        r10,    r12,    -2
            stdu        r14,    r12,    -2
            mov         r0,     %rpc
            stu         r0,     r12,    -1
            
            mov         r8,     r5                          !backup y
            mov         r9,     r6                          !backup size

DMA_YUVI2E_GET_DMA_STATE:
            mov         r4,     0
            lda         r13,    _GetDmaChannelState         !Here one question may occur becaues of 
                                                            !the return of _GetDmaChannelState is pointer of struct
            call        r13
            
            cmp         r4,     -1                          !DMA_CHANNEL_FULL
            bz          DMA_YUVI2E_GET_DMA_STATE
            
            mov         r13,    r4                          !DmaChl
            mov         r6,     r4                          !DmaChl
!dstY = (unsigned long )((unsigned long )(xvid_dec->cur->y_page)<<16) + (unsigned long)(y)*(unsigned long)(size);
            movh        r4,     _Xvidec
            mov         r0,     0
            
            movl        r4,     _Xvidec
            xor         r3,     r3
                        
            add         r4,     4
            movh        r2,     0x8c
            
            ldu         r1,     r4,     1                   !xvid_dec->cur
            movl        r2,     0x00
            
            ld          r1,     r1                          !xvid_dec->cur->y_page      
            mov         r4,     -1
            
            mac.a       r8,     r9                          !dstY
            mov         r5,     -1
!temp2 = 0x8c00-320*4*y;            
            movh        r9,     0x05
            
            movl        r9,     0x00                        !320*4
            movh        r10,    _IntMem
            
            macn.b      r8,     r9                          !temp2
            movl        r10,    _IntMem
            mov         r15,    0
           
!DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
            stdu        r4,     r13,    2            
            mov         r8,     0x0000
!DmaChl->DmaSar = (unsigned long)(((unsigned long)(IntMem.mb_y_row)<<1)+(unsigned long)0x40000);
!DmaChl->DmaDar = ((unsigned long)(dstY)<<1)+(unsigned long)0x4000000;
!
!DmaChl->DmaCtl.Lsb = 0x00040025;
!DmaChl->DmaCtl.Msb = 0x00000F00;
!
!DmaChl->DmaCfg.Lsb = 0x00000020;
!DmaChl->DmaCfg.Msb = 0x00001004;
!DmaChl->DmaDsr = 0xA0000000|temp2;
            ld          r14,    r10,    0
            mov         r9,     0x0004
            
            shll.e      r14,    1
            
            add.e       r14,    r8
            
            add         r13,    2
            shll        r9,     8
            
            stdu        r14,    r13,    2               !DmaChl->DmaSar
            
            shll.e      r0,     1
            
            add.e       r0,     r8

            stdu        r0,     r13,    2               !DmaChl->DmaDar
            
            movlh       r8,     0x00040025
            
            stdu        r8,     r13,    2               !DmaChl->DmaCtl.Lsb
            
            movlh       r10,    0x00000F00
            
            stdu        r10,    r13,    2               !DmaChl->DmaCtl.Msb
            
            movlh       r8,     0x00000020
            
            stdu        r8,     r13,    2               !DmaChl->DmaCfg.Lsb
            
            movlh       r10,    0x00001004
            
            stdu        r10,    r13,    2               !DmaChl->DmaCfg.Msb
            
            add         r13,    2
            movh        r10,    _Dma_I2EBlock
            
            movlh       r0,     0xA0000000
            
            or.e        r0,     r2
            
            stdu        r0,     r13,    2               !DmaChl->DmaDsr
!Dma_I2EBlock   = 1;
!
!DmaChl->Priority   = 0;
!
!DmaChl->pResFlag   = &Dma_I2EBlock;
!
!CALL_DmaChannelSetup(0);
            movl        r10,    _Dma_I2EBlock
            mov         r14,    1
            
            st          r14,    r10,    0               ![1]Dma_I2EBlock = 1
            mov         r15,    0
            
            st          r15,    r6,     3               !DmaChl->Priority   = 0;
            mov         r4,     0
            
            st          r10,    r6,     2               !DmaChl->pResFlag   = &Dma_I2EBlock;
            movh        r13,    _CALL_DmaChannelSetup
            
            movl        r13,    _CALL_DmaChannelSetup
            
            call        r13
                            
            mov         r5,     r12
            
            add         r5,     1
            
            ldu         r6,     r5,     1               
            mov         %rpc,   r6
            lddu        r14,    r5,     2
            lddu        r10,    r5,     2
            lddu        r8,     r5,     2
            lddu        r2,     r5,     2
            lddu        r0,     r5,     2
            add         r5,     -1
            mov         r12,    r5
            ret     
                        
                              
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!  Func:
!     DMA_YUV_E2I_A
!  Description:
!      index: 0-->y;  2-->uv
!      dst_offset_pos: offset position of refer-image
!  NB:
!      The Size of Match_Block is 18X17 or 18X9
!  Author:
!      fang zf
!      jian huan   (modify) 
!  Date:
!      2006-1-3 14:47
!      2006-1-9 10:49  (modify)
!  Optimize:
!      Jian Huan
!  Date:
!      2006-7-14 16:18    
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!__attribute__((section(".avi_v_text, \"ax\"")))
!void DMA_YUV_E2I_A(short index,   short   dst_offset_pos)       //32BIT--32BIT
!{
!    short   vert_in, hor_in, hor_out;
!    unsigned long   src;
!    unsigned short  dst;
!    IMAGE *refimg = Xvidec.ref;
!    DMA_CHANNEL_REG *DmaChl;
!    unsigned int  resflag;
!        
!    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
! 
!    
!   if(index == 0){ 
!        Global_Offset = (unsigned short)(src_offset_pos & 0x1);
!        src = (((unsigned long)(refimg->y_page))<<16) + (src_offset_pos - (unsigned long)Global_Offset);
!        dst = &match_y_block[0];  
!        resflag   = &Dma_E2IBlock_Y;
!        Dma_E2IBlock_Y = 1;
!        //hor_in  = 18;
!        vert_in = 17;
!    }else if(index == 2){
!        src = (((unsigned long)(refimg->uv_page))<<16) + 0x2c00 + src_offset_pos;
!        dst = &match_uv_block[0];       
!        resflag   = &Dma_E2IBlock_UV;
!        Dma_E2IBlock_UV = 1;
!        //hor_in  = 18;
!        vert_in = 9;
!    }
!//---------------------------------------------------------------------
!    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
!//---------------------------------------------------------------------    
!    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);
!    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
!    
!    DmaChl->DmaCtl.Lsb = 0x00020025;         //32-bit transfer
!    DmaChl->DmaCtl.Msb = (unsigned long)(9*(unsigned long)vert_in);
!    
!    DmaChl->DmaCfg.Lsb = 0x00000020;
!    DmaChl->DmaCfg.Msb = 0x00001004;
!    
!    //DmaChl.DmaSgr.Lsb = (unsigned long)(160-9)|((unsigned long)(9)<<20);
!    DmaChl->DmaSgr = 0x00900097;
!    
!    DmaChl->Priority   = 1;
!    
!    DmaChl->pResFlag   = resflag;
!    
!    CALL_DmaChannelSetup(0);
!    
!}
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
MP4_ASSEMBLE_SECTION
.global _DMA_YUV_E2I_A
.walign 2
_DMA_YUV_E2I_A:
            stdu        r0,     r12,    -2
            stdu        r2,     r12,    -2
            stdu        r8,     r12,    -2
            stdu        r10,    r12,    -2
            stdu        r14,    r12,    -2
            mov         r0,     %rpc
            stu         r0,     r12,    -1
            
            mov         r8,     r4                      !index

DMA_YUVE2I_A_GET_DMA_STATE:            
            mov         r4,     0
            lda         r13,    _GetDmaChannelState
            call        r13
            
            cmp         r4,     -1                      !DMA_CHANNEL_FULL
            bz          DMA_YUVE2I_A_GET_DMA_STATE

            mov         r13,    r4
            mov         r6,     r4
                        
            cmp         r8,     0
            bnz         DMA_YUVE2I_A_INDEX_2

DMA_YUVE2I_A_INDEX_0:            
            mov         r4,     1
            movh        r11,    _src_offset_pos    
            
            movl        r11,    _src_offset_pos    
            movh        r10,    _Global_Offset
            
            lddu        r14,    r11,    2               !src_offset_pos
            
            and         r4,     r14                     !Global_Offset
            movl        r10,    _Global_Offset
            
            st          r4,     r10
            movh        r11,    _Xvidec
            
            movl        r11,    _Xvidec
            xor         r5,     r5

!src = (((unsigned long)(refimg->y_page))<<16) + (src_offset_pos - (unsigned long)Global_Offset);
            add         r11,    5
            ld          r1,     r11
            
            xor         r0,     r0
            ld          r1,     r1
            
            sub.e       r14,    r4
            
            add.e       r0,     r14                     !src
!dst = &match_y_block[0];            
            movh        r14,    _match_y_block
            xor         r15,    r15                     !dst
            
            movl        r14,    _match_y_block
            movh        r5,     _Dma_E2IBlock_Y
            
            movl        r5,     _Dma_E2IBlock_Y
            mov         r4,     1
            
            st          r4,     r5,     0
            movh        r8,     0x00
            
            movl        r8,     0x99                    !9*17
            br          DMA_YUVE2I_A_CONFIG

DMA_YUVE2I_A_INDEX_2:
!src = (((unsigned long)(refimg->uv_page))<<16) + 0x2c00 + src_offset_pos;
            movh        r11,    _src_offset_pos
            movh        r5,     _Xvidec
            
            movl        r11,    _src_offset_pos
            movl        r5,     _Xvidec
            
            lddu        r14,    r11,    2               !src_offset_pos
            add         r5,     5
            
            xor         r0,     r0
            ld          r1,     r5                      !refimg
            
            movl        r8,     9*9
            ld          r1,     r1,     2
            
            movlh       r4,     0x00002c00
            
            add.e       r0,     r4
            
            add.e       r0,     r14                     !src
            
            movh        r14,    _match_uv_block
            movh        r5,     _Dma_E2IBlock_UV
            
            movl        r14,    _match_uv_block
            xor         r15,    r15                     !dst
            
            movl        r5,     _Dma_E2IBlock_UV
            mov         r4,     1
            
            st          r4,     r5,     0
            movh        r8,     0x00
            
DMA_YUVE2I_A_CONFIG:
!DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
            mov         r10,    -1
            mov         r11,    -1
            stdu        r10,    r13,    2
            
            movlh       r10,    0x04000000
!DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)0x4000000;
!DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
!
!DmaChl->DmaCtl.Lsb = 0x00020025;
!DmaChl->DmaCtl.Msb = (unsigned long)(9*(unsigned long)vert_in);
!
!DmaChl->DmaCfg.Lsb = 0x00000020;
!DmaChl->DmaCfg.Msb = 0x00001004;
!DmaChl->DmaSgr = 0x00900097;
            shll.e      r0,     1
            
            add.e       r0,     r10
            
            add         r13,    2
            shrl        r11,    8
            
            stdu        r0,     r13,    2               !DmaChl->DmaSar
            
            shll.e      r14,    1
            
            add.e       r14,    r10

            stdu        r14,    r13,    2               !DmaChl->DmaDar
            
            movlh       r10,    0x00020025
            
            stdu        r10,    r13,    2               !DmaChl->DmaCtl.Lsb
            xor         r9,     r9
            
            stdu        r8,     r13,    2               !DmaChl->DmaCtl.Msb
            mov         r15,    1
            
            movlh       r8,     0x00000020
            
            stdu        r8,     r13,    2               !DmaChl->DmaCfg.Lsb
            
            movlh       r10,    0x00001004
            
            stdu        r10,    r13,    2               !DmaChl->DmaCfg.Msb
            mov         r4,     0
            
            movlh       r0,     0x00900097
            
            stdu        r0,    r13,    2
            
!DmaChl->Priority   = 1;
!
!DmaChl->pResFlag   = resflag;
!
!CALL_DmaChannelSetup(0);
            
            st          r15,    r6,     3               !DmaChl->Priority   = 1;
            movh        r13,    _CALL_DmaChannelSetup
            
            st          r5,     r6,     2               !DmaChl->pResFlag   = resflag
            movl        r13,    _CALL_DmaChannelSetup
            
            call        r13 
            
            mov         r5,     r12
            add         r5,     1
            
            ldu         r6,     r5,     1               
            mov         %rpc,   r6
            lddu        r14,    r5,     2
            lddu        r10,    r5,     2
            lddu        r8,     r5,     2
            lddu        r2,     r5,     2
            lddu        r0,     r5,     2
            add         r5,     -1
            mov         r12,    r5
            ret                                                                    

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!   Func:
!       DMA_YUV_E2I_B
!   
!__attribute__((section(".avi_v_text, \"ax\"")))
!void DMA_YUV_E2I_B(short index,   short   dst_offset_pos)           //16BIT--16BIT
!{
!    short   vert_in, hor_in, hor_out;
!    unsigned long   src;
!    unsigned short  dst;
!    PIXEL_COMP_STRUCT   *pcs = &PCS, *pcs_uv = &PCS_UV;
!    IMAGE *refimg = Xvidec.ref;
!    unsigned int  resflag;
!    DMA_CHANNEL_REG *DmaChl;
!    
!    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
!    
!   if(index == 0){              
!        src = (((unsigned long)(refimg->y_page))<<16) + src_offset_pos;
!        dst = &match_y_block[0] + dst_offset_pos;
!        resflag   = &Dma_E2IBlock_Y;
!        Dma_E2IBlock_Y = 1;
!        vert_in = pcs->vert_in;
!        hor_in  = pcs->hor_in; 
!        hor_out = pcs->hor_out;
!    }else if(index == 2){
!        src = (((unsigned long)(refimg->uv_page))<<16) + 0x2c00 + src_offset_pos;
!        dst = &match_uv_block[0] + dst_offset_pos;
!        resflag   = &Dma_E2IBlock_UV;
!        Dma_E2IBlock_UV = 1;
!        vert_in = pcs_uv->vert_in;
!        hor_in  = pcs_uv->hor_in; 
!        hor_out = pcs_uv->hor_out;
!    }
!//---------------------------------------------------------------------
!    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
!//---------------------------------------------------------------------    
!
!    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);
!    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
!    
!    DmaChl->DmaCtl.Lsb = 0x00060013;
!    DmaChl->DmaCtl.Msb = (unsigned long)(hor_in*vert_in);
!    
!    DmaChl->DmaCfg.Lsb = 0x00000020;
!    DmaChl->DmaCfg.Msb = 0x00001004;
!    
!    DmaChl->DmaSgr = (unsigned long)(320-hor_in)|((unsigned long)(hor_in)<<20);
!    
!    DmaChl->DmaDsr = ((unsigned long)(hor_in)<<20)|((unsigned long)(hor_out));
!    
!    DmaChl->Priority   = 1;
!
!    DmaChl->pResFlag   = resflag;
!    
!    CALL_DmaChannelSetup(0);
!}
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
MP4_ASSEMBLE_SECTION
.global _DMA_YUV_E2I_B
.walign 2
_DMA_YUV_E2I_B:
                stdu        r0,     r12,    -2
                stdu        r2,     r12,    -2
                stdu        r8,     r12,    -2
                stdu        r10,    r12,    -2
                stdu        r14,    r12,    -2
                mov         r6,     %rpc
                stdu        r6,     r12,    -2                          !save r7        

                mov         r8,     r4
                mov         r9,     r5

DMA_YUV_E2I_B_DMA_STATE:                
                mov         r4,     0
                lda         r13,    _GetDmaChannelState
                call        r13
                
                cmp         r4,     -1                                  !DMA_CHANNEL_FULL
                bz          DMA_YUV_E2I_B_DMA_STATE                   
                
                mov         r13,    r4
                mov         r6,     r4
                
                cmp         r8,     0                                   !index
                bnz         DMA_YUV_E2I_B_INDEX_1
                
DMA_YUV_E2I_B_INDEX_0:
                lda         r10,    _Xvidec
                mov         r11,    5
                
                ldx         r4,     r10                                 !refimg
                
                ld          r5,     r4                                  !refimg->y_page 
                xor         r4,     r4                                  !(refimg->uv_page)<<16
                
                lda         r8,     _src_offset_pos
                
                lddu        r10,    r8,     2                           !src_offset_pos
                
                add.e       r4,     r10                                 !src
                
                lda         r14,    _match_y_block
                
                add         r14,    r9
                xor         r15,    r15                                 !dst
                
                lda         r8,     _Dma_E2IBlock_Y                     !resflag
                mov         r9,     1
                
                st          r9,     r8                                  !Dma_E2IBlock_Y = 1
                
                lda         r7,     _PCS
                lddu        r10,    r7,     2                       !hor_in,vert_in
                ld          r7,     r7,     0                       !hor_out
                add         r13,    2
                br          DMA_YUV_E2I_B_CONFIG
                
DMA_YUV_E2I_B_INDEX_1:
                lda         r10,    _Xvidec
                mov         r11,    5
                
                ldx         r4,     r10                                 !refimg
                
                ld          r5,     r4,     2                           !refimg->uv_page
                xor         r4,     r4                                  !(refimg->uv_page)<<16
                
                lda         r8,     _src_offset_pos
                movlh       r14,    0x00002c00
                
                lddu        r10,    r8,     2                           !src_offset_pos
                
                add.e       r4,     r10                                 
                
                add.e       r4,     r14                                 !src
                
                lda         r14,    _match_uv_block
                
                add         r14,    r9
                xor         r15,    r15                                 !dst
                
                lda         r8,     _Dma_E2IBlock_UV
                mov         r9,     1
                
                st          r9,     r8
                
                lda         r7,     _PCS_UV
                lddu        r10,    r7,     2                       !hor_in,vert_in
                ld          r7,     r7,     0                       !hor_out                             
                add         r13,    2
DMA_YUV_E2I_B_CONFIG:
                mov         r0,     -1
                mov         r1,     -1
!DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
                
                stdu        r0,     r13,    2  
                
                movlh       r2,     0x04000000
                
                shll.e      r4,     1
                
                add.e       r4,     r2              
                
!DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);                
                stdu        r4,     r13,    2                           
                
                shrl        r3,     8
                
                shll.e      r14,    1
!DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
                add.e       r14,    r2
            
                stdu        r14,    r13,    2                           !DmaChl->DmaDar
                
                movlh       r0,     0x00060013
                stdu        r0,     r13,    2                           !DmaChl->DmaCtl.Lsb
                
                mul.a       r10,    r11                                 !hor_in*vert_in
                
                stdu        r0,     r13,    2                           !DmaChl->DmaCtl.Msb
                
                movlh       r2,     0x00000020
                stdu        r2,     r13,    2                           !DmaChl->DmaCfg.Lsb
                
                movlh       r0,     0x00001004
                stdu        r0,     r13,    2                           !DmaChl->DmaCfg.Msb
                
                mov         r4,     320
                sub         r4,     r10                                 !320-hor_in
                xor         r5,     r5
                
                mov         r11,    r10
                xor         r10,    r10
                
                shll        r11,    4
                
                
                or.e        r4,     r10
                stdu        r4,     r13,    2                           !DmaChl->DmaSgr
                
                mov         r4,     r7
                mov         r5,     0
                
                or.e        r4,     r10
                stdu        r4,     r13,    2                           !DmaChl->DmaDsr
                
                mov         r0,     1
                st          r0,     r6,     3                           !DmaChl->Priority
                
                st          r8,     r6,     2                           !DmaChl->pResFlag
                
                mov         r4,     0
                lda         r13,    _CALL_DmaChannelSetup
                
                call        r13
                
                mov         r5,     r12
                add         r5,     1
                
                lddu        r6,     r5,     2
                mov         %rpc,   r6
                lddu        r14,    r5,     2
                lddu        r10,    r5,     2
                lddu        r8,     r5,     2
                lddu        r2,     r5,     2
                lddu        r0,     r5,     2
                add         r5,     -1
                mov         r12,    r5
                ret
                

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!   Func:
!       DMA_YUV_E2I_C
!   
!__attribute__((section(".avi_v_text, \"ax\"")))
!void DMA_YUV_E2I_C(short index, short dst_offset_pos)       //32BIT--32BIT
!{
!    short   vert_in, hor_in, hor_out;
!    unsigned long   src;
!    unsigned short  dst;
!    IMAGE *refimg = Xvidec.ref;
!    DMA_CHANNEL_REG *DmaChl;
!    
!    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
! 
!    Global_Offset = (unsigned short)(src_offset_pos & 0x1);
!    src = (((unsigned long)(refimg->y_page))<<16) + (src_offset_pos - (unsigned long)Global_Offset);
!    dst = &match_y_block[0];  
!    hor_in  = 10;
!    vert_in = 9;
!//---------------------------------------------------------------------
!    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
!//---------------------------------------------------------------------    
!    
!    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);
!    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
!    
!    DmaChl->DmaCtl.Lsb = 0x00020025;         //32-bit transfer
!    DmaChl->DmaCtl.Msb = (unsigned long)(45);
!    
!    DmaChl->DmaCfg.Lsb = 0x00000020;
!    DmaChl->DmaCfg.Msb = 0x00001004;
!    
!    DmaChl->DmaSgr = (unsigned long)(160-5)|((unsigned long)(5)<<20);
!    
!    Dma_E2IBlock_Y   = 1;
!    
!    DmaChl->Priority   = 1;
!    
!    DmaChl->pResFlag   = &Dma_E2IBlock_Y;
!    
!    CALL_DmaChannelSetup(0);
!}
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
MP4_ASSEMBLE_SECTION
.global _DMA_YUV_E2I_C
.walign 2
_DMA_YUV_E2I_C:
                stdu        r0,     r12,    -2
                stdu        r2,     r12,    -2
                stdu        r8,     r12,    -2
                stdu        r10,    r12,    -2
                stdu        r14,    r12,    -2
                mov         r6,     %rpc
                stdu        r6,     r12,    -2                          !save r7        
            
DMA_YUV_E2I_C_DMA_CASE:                
                mov         r4,     0
                lda         r13,    _GetDmaChannelState
                call        r13
                
                cmp         r4,     -1                                  !DMA_CHANNEL_FULL
                bz          DMA_YUV_E2I_C_DMA_CASE

                mov         r13,    r4
                mov         r6,     r4
                
                lda         r8,     _src_offset_pos
                
                lddu        r10,    r8,     2                           !src_offset_pos

                mov         r14,    1
                and         r14,    r10                                 !Global_Offset

                lda         r7,     _Global_Offset
                st          r14,    r7
                xor         r15,    r15
                
                lda         r8,    _Xvidec
                mov         r9,    5
                
                ldx         r4,     r8                                  !refimg
                
                ld          r5,     r4                                  !refimg->y_page 
                xor         r4,     r4                                  !(refimg->y_page)<<16
                
                add.e       r4,     r10                                 
                
                sub.e       r4,     r14                                 !src
                
                lda         r14,    _match_y_block
                xor         r15,    r15                                 !dst
                
                lda         r8,     _Dma_E2IBlock_Y                     !resflag
                mov         r9,     1
                
                st          r9,     r8                                  !Dma_E2IBlock_Y = 1
                
                mov         r0,     -1
                mov         r1,     -1
!DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
                
                stdu        r0,     r13,    2  
                
                movlh       r2,     0x04000000
                
                shll.e      r4,     1
                
                add.e       r4,     r2              
                
                add         r13,    2
!DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)(0x4000000);                
                stdu        r4,     r13,    2                           
                
                shrl        r3,     8
                
                shll.e      r14,    1
!DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
                add.e       r14,    r2
            
                stdu        r14,    r13,    2
                
                movlh       r0,     0x00020025
                stdu        r0,     r13,    2                           !DmaChl->DmaCtl.Lsb
                
                movlh       r0,     0x0000002D                          !45
                stdu        r0,     r13,    2                           !DmaChl->DmaCtl.Msb
                
                movlh       r2,     0x00000020
                stdu        r2,     r13,    2                           !DmaChl->DmaCfg.Lsb
                
                movlh       r0,     0x00001004
                stdu        r0,     r13,    2                           !DmaChl->DmaCfg.Msb
                
                movlh       r4,     0x0050009B                          !(unsigned long)(160-5)|((unsigned long)(5)<<20)
                stdu        r4,     r13,    2                           !DmaChl->DmaSgr
                
                mov         r0,     1
                st          r0,     r6,     3                           !DmaChl->Priority
                
                st          r8,     r6,     2                           !DmaChl->pResFlag
                
                mov         r4,     0
                lda         r13,    _CALL_DmaChannelSetup
                
                call        r13
                
                mov         r5,     r12
                add         r5,     1
                
                lddu        r6,     r5,     2
                mov         %rpc,   r6
                lddu        r14,    r5,     2
                lddu        r10,    r5,     2
                lddu        r8,     r5,     2
                lddu        r2,     r5,     2
                lddu        r0,     r5,     2
                add         r5,     -1
                mov         r12,    r5
                ret
                
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!  Func:
!      copy_y_from_external_to_internal
!  Dsecripton:
!      External SDRAM to Internal SRAM
!  NB:
!      32bit-32bit
!  Author:
!      Fang ZF
!      Jian Huan (Modify)
!  Date:
!      2006-1-3 21:35
!      2006-1-10 9:00
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!__attribute__((section(".avi_v_text, \"ax\"")))
!void    copy_y_from_external_to_internal(DECODER *xvid_dec, short x, short y)
!{
!    unsigned long xy_cord;
!    unsigned long src, dst;
!    DMA_CHANNEL_REG *DmaChl;
!    
!    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
!    
!    xy_cord = (unsigned long)(xvid_dec->width) * ((unsigned long)(y)<<4) + ((unsigned long)(x)<<4);
!   
!    src = (unsigned long)(((unsigned long)(xvid_dec->ref->y_page)<<16)) + (unsigned long)xy_cord;       //128K/2 = 131072/2
!    dst = (unsigned long)IntMem.mb_y_row + ((unsigned long)(x)<<4);
!//---------------------------------------------------------------------
!    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
!//---------------------------------------------------------------------
!    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)0x4000000;
!    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
!    
!    DmaChl->DmaCtl.Lsb = 0x00060025;                 //0x00060013;
!    DmaChl->DmaCtl.Msb = 16*8;                       //16*16;
!    
!    DmaChl->DmaCfg.Lsb = 0x00000020;
!    DmaChl->DmaCfg.Msb = 0x00001004;
!    
!    DmaChl->DmaSgr = 0x00800098;                 //0x01000130;
!    
!    DmaChl->DmaDsr = 0x00800098;                 //0x01000130;
!    
!    Dma_16x16Block   = 1;
!    
!    DmaChl->Priority   = 2;
!
!    DmaChl->pResFlag   = &Dma_16x16Block;
!        
!    CALL_DmaChannelSetup(0);
!  
!}
MP4_ASSEMBLE_SECTION
.global _copy_y_from_external_to_internal
.walign 2
_copy_y_from_external_to_internal:
            stdu        r0,     r12,    -2
            stdu        r2,     r12,    -2
            stdu        r8,     r12,    -2
            stdu        r10,    r12,    -2
            stdu        r14,    r12,    -2
            mov         r0,     %rpc
            mov         r8,     r4

            stu         r0,     r12,    -1
            mov         r9,     320*16              !320
            
            mul.a       r9,     r6
            shll        r5,     4
            
            add         r0,     r5                  !xy_cord
            mov         r9,     r5
            
DMA_COPY_Y_FROM_EXTERNAL_TO_INTERNAL_STATE:            
            mov         r4,     0
            lda         r13,    _GetDmaChannelState
            call        r13
            
            cmp         r4,     -1
            bz          DMA_COPY_Y_FROM_EXTERNAL_TO_INTERNAL_STATE
            
            mov         r13,    r4
            add         r8,     5
            
            mov         r6,     r4
            ld          r2,     r8                  !xvid_dec->ref
            
            ld          r3,     r2                  !xvid_dec->ref->y_page
            xor         r2,     r2
            
            add.e       r2,     r0                  !src
            
            movh        r10,    _IntMem
            mov         r14,    -1
            
            movl        r10,    _IntMem
            mov         r15,    -1
            
            ld          r10,    r10
            xor         r11,    r11
            
            add         r10,    r9                  !dst
            
            stdu        r14,    r13,    2
            add         r13,    2
            
            movlh       r8,     0x04000000
            
            shll.e      r2,     1
            
            add.e       r2,     r8
            
            stdu        r2,     r13,    2           !DmaChl->DmaSar
            
            shrl        r9,     8
            
            shll.e      r10,    1
            
            add.e       r10,    r8
            
            stdu        r10,    r13,    2           !DmaChl->DmaDar
            
            movlh       r8,     0x00060025
            
            stdu        r8,     r13,    2           !DmaChl->DmaCtl.Lsb
            
            movlh       r10,    16*8
            
            stdu        r10,    r13,    2           !DmaChl->DmaCfg.Msb
            
            movlh       r8,     0x00000020
            
            stdu        r8,     r13,    2           !DmaChl->DmaCfg.Lsb
            
            movlh       r10,    0x00001004
            
            stdu        r10,    r13,    2           !DmaChl->DmaCfg.Msb
            
            movlh       r8,     0x00800098
            
            stdu        r8,     r13,    2           !DmaChl->DmaSgr
            
            stdu        r8,     r13,    2           !DmaChl->DmaDsr
            
            mov         r5,     1
            movh        r8,     _Dma_16x16Block
            
            mov         r9,     2
            movl        r8,     _Dma_16x16Block

            st          r5,     r8,     0
            mov         r4,     0
            
            st          r9,     r6,     3           !DmaChl->Priority
            movh        r13,    _CALL_DmaChannelSetup
            
            st          r8,     r6,     2           !DmaChl->pResFlag
            movl        r13,    _CALL_DmaChannelSetup
            
            call        r13     
            
            mov         r5,     r12
            add         r5,     1
            
            ldu         r6,     r5,     1
            mov         %rpc,   r6
            lddu        r14,    r5,     2
            lddu        r10,    r5,     2
            lddu        r8,     r5,     2
            lddu        r2,     r5,     2
            lddu        r0,     r5,     2
            add         r5,     -1
            mov         r12,    r5
            ret

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!  Func:
!      copy_uv_from_external_to_internal
!  Dsecripton:
!      External SDRAM to Internal SRAM
!  NB:
!      32bit-32bit
!  Author:
!      Fang ZF
!      Jian Huan   (Modify)
!  Date:
!      2006-2006-1-3 21:36
!      2006-1-10 9:01
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!__attribute__((section(".avi_v_text, \"ax\"")))
!void    copy_uv_from_external_to_internal(DECODER *xvid_dec, short x, short y)
!{
!    unsigned long   xy_cord;
!    unsigned long   src = 0, dst = 0;
!    DMA_CHANNEL_REG *DmaChl;
!    
!    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0))); 
!    
!    xy_cord = (unsigned long)(xvid_dec->width) * (unsigned long)(y<<3) + (unsigned long)(x<<4);
!
!    src = (unsigned long)((unsigned long)(xvid_dec->ref->uv_page)<<16) + 0x2c00 + (unsigned long)xy_cord;
!    dst = (unsigned long)IntMem.mb_uv_row + (unsigned long)(x<<4);
!//---------------------------------------------------------------------
!    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
!//---------------------------------------------------------------------
!
!    DmaChl->DmaSar = ((unsigned long)(src)<<1)+(unsigned long)0x4000000;
!    DmaChl->DmaDar = ((unsigned long)(dst)<<1)+(unsigned long)0x40000;
!    
!    DmaChl->DmaCtl.Lsb = 0x00060025;                 //0x00060013;
!    DmaChl->DmaCtl.Msb = 16*4;                       //16*8;
!    
!    DmaChl->DmaCfg.Lsb = 0x00000020;
!    DmaChl->DmaCfg.Msb = 0x00001004;
!    
!    DmaChl->DmaSgr = 0x00800098;                 //0x01000130;
!    
!    DmaChl->DmaDsr = 0x00800098;                 //0x01000130;
!    
!    Dma_16x8Block   = 1;
!    
!    DmaChl->Priority   = 2;
!    
!    DmaChl->pResFlag   = &Dma_16x8Block;
!
!    CALL_DmaChannelSetup(0);
!
!}
MP4_ASSEMBLE_SECTION
.global _copy_uv_from_external_to_internal
.walign 2
_copy_uv_from_external_to_internal:
            stdu        r0,     r12,    -2
            stdu        r2,     r12,    -2
            stdu        r8,     r12,    -2
            stdu        r10,    r12,    -2
            stdu        r14,    r12,    -2
            mov         r0,     %rpc
            mov         r8,     r4

            stu         r0,     r12,    -1
            mov         r9,     320*8               !320
            
            mul.a       r9,     r6
            shll        r5,     4
            
            add         r0,     r5                  !xy_cord
            mov         r9,     r5
            
DMA_COPY_UV_FROM_EXTERNAL_TO_INTERNAL_STATE:            
            mov         r4,     0
            lda         r13,    _GetDmaChannelState
            call        r13
            
            cmp         r4,     -1
            bz          DMA_COPY_UV_FROM_EXTERNAL_TO_INTERNAL_STATE
            
            mov         r13,    r4
            add         r8,     5
            
            mov         r6,     r4
            ld          r2,     r8                  !xvid_dec->ref
            
            ld          r3,     r2,     2           !xvid_dec->ref->uv_page
            xor         r2,     r2
            
            movlh       r10,    0x00002c00
            
            add.e       r2,     r10
            
            add.e       r2,     r0                  !src
            
            movh        r10,    _IntMem
            mov         r14,    -1
            
            movl        r10,    _IntMem
            mov         r15,    -1
            
            ld          r10,    r10,    1           !IntMem.mb_uv_row
            xor         r11,    r11
            
            add         r10,    r9                  !dst
            
            stdu        r14,    r13,    2
            add         r13,    2
            
            movlh       r8,     0x04000000
            
            shll.e      r2,     1
            
            add.e       r2,     r8
            
            stdu        r2,     r13,    2           !DmaChl->DmaSar
            
            shrl        r9,     8
            
            shll.e      r10,    1
            
            add.e       r10,    r8
            
            stdu        r10,    r13,    2           !DmaChl->DmaDar
            
            movlh       r8,     0x00060025
            
            stdu        r8,     r13,    2           !DmaChl->DmaCtl.Lsb
            
            movlh       r10,    16*4
            
            stdu        r10,    r13,    2           !DmaChl->DmaCfg.Msb
            
            movlh       r8,     0x00000020
            
            stdu        r8,     r13,    2           !DmaChl->DmaCfg.Lsb
            
            movlh       r10,    0x00001004
            
            stdu        r10,    r13,    2           !DmaChl->DmaCfg.Msb
            
            movlh       r8,     0x00800098
            
            stdu        r8,     r13,    2           !DmaChl->DmaSgr
            
            stdu        r8,     r13,    2           !DmaChl->DmaDsr
            
            mov         r5,     1
            movh        r8,     _Dma_16x8Block
            
            mov         r9,     2
            movl        r8,     _Dma_16x8Block

            st          r5,     r8,     0
            mov         r4,     0
            
            st          r9,     r6,     3           !DmaChl->Priority
            movh        r13,    _CALL_DmaChannelSetup
            
            st          r8,     r6,     2           !DmaChl->pResFlag
            movl        r13,    _CALL_DmaChannelSetup
            
            call        r13     
            
            mov         r5,     r12
            add         r5,     1
            
            ldu         r6,     r5,     1
            mov         %rpc,   r6
            lddu        r14,    r5,     2
            lddu        r10,    r5,     2
            lddu        r8,     r5,     2
            lddu        r2,     r5,     2
            lddu        r0,     r5,     2
            add         r5,     -1
            mov         r12,    r5
            ret
            
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!  Func:
!      DMA_RGB_I2E
!  Description:
!      RGB Internal SRAM to External SDRAM
!  Author:
!      Jian H
!  Date:
!      2006-1-5 9:10
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!__attribute__((section(".avi_v_text, \"ax\"")))
!void DMA_RGB_I2E(unsigned short *RgbBuf, short y, short size)
!{
!    unsigned long dstRGB;
!    unsigned long RGBPage = 4;
!    DMA_CHANNEL_REG *DmaChl;
!
!    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 
!    
!    RGBPage = 262144 + ((unsigned long)AviRgbSdramIndex)*76800; 
!    dstRGB = (unsigned long )((unsigned long )(RGBPage)) + (unsigned long)(y)*(unsigned long)(size/320);          //指向SDRAM里当前帧Y分量的地址　
!
!//---------------------------------------------------------------------
!    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
!//--------------------------------------------------------------------- 
!    
!    DmaChl->DmaSar = (unsigned long)(((unsigned long)(RgbBuf)<<1)+(unsigned long)0x40000);
!    DmaChl->DmaDar = ((unsigned long)(dstRGB)<<1)+(unsigned long)0x4000000;
!    
!    DmaChl->DmaCtl.Lsb = 0x00040025;              
!    DmaChl->DmaCtl.Msb = 0x00000A00;             //320*16/2
!    
!    DmaChl->DmaCfg.Lsb = 0x00000020;
!    DmaChl->DmaCfg.Msb = 0x00001004;
!    
!    DmaChl->DmaDsr = 0x00800070;
!    
!    Dma_RGB_I2E   = 1;
!    
!    DmaChl->Priority   = 2;
!
!    DmaChl->pResFlag   = &Dma_RGB_I2E;
!        
!    CALL_DmaChannelSetup(1);
!} 
MP4_ASSEMBLE_SECTION
.global _DMA_RGB_I2E
.walign 2
_DMA_RGB_I2E:
            stdu        r0,     r12,    -2
            stdu        r2,     r12,    -2
            stdu        r8,     r12,    -2
            stdu        r10,    r12,    -2
            stdu        r14,    r12,    -2

            mov         r0,     %rpc
            mov         r8,     r4
            
            stu         r0,     r12,    -1
            mov         r0,     r5

DMA_RGB_I2E_STATE:            
            mov         r4,     1
            lda         r13,    _GetDmaChannelState
            call        r13
            
            cmp         r4,     -1              !DMA_CHANNEL_FULL           
            bz          DMA_RGB_I2E_STATE
            
            mov         r13,    r4
            mov         r6,     r4
            
            movl        r11,    _AviRgbSdramIndex
            movl        r14,    150
            
            movh        r11,    _AviRgbSdramIndex
            movh        r14,    0
            
            
            ld          r11,    r11             !AviRgbSdramIndex             
            shll        r0,     4               !y*(size/320)=y*16
            
            shll        r11,    9               !AviRgbSdramIndex*32*16
            mov         r1,     4               !4*64*1024=262144=0x4,0000
            
            mac.a       r11,    r14             !dstRGB

            mov         r14,    -1
            mov         r15,    -1

!DmaChl->pDmaFnct    = DMA_FNCT_NULL;
!DmaChl->pDmaFnctArg = DMA_FNCT_NULL;            
            stdu        r14,    r13,    2       
            
!DmaChl->DmaSar = (unsigned long)(((unsigned long)(RgbBuf)<<1)+(unsigned long)0x40000);
!DmaChl->DmaDar = ((unsigned long)(dstRGB)<<1)+(unsigned long)0x4000000;            
            mov         r9,     0
            add         r13,    2
            
            shll.e      r8,     1
            
            movlh       r10,    0x40000
            
            add.e       r8,     r10
            
            stdu        r8,     r13,    2
            shll        r11,    8
            
            shll.e      r0,     1
            
            add.e       r0,     r10
            
            stdu        r0,     r13,    2
            
!DmaChl->DmaCtl.Lsb = 0x00040025;              
!DmaChl->DmaCtl.Msb = 0x00000A00;  
            movlh       r8,     0x00040025
            stdu        r8,     r13,    2           !DmaChl->DmaCtl.Lsb
            
            movlh       r10,    0x00000A00
            stdu        r10,    r13,    2           !DmaChl->DmaCtl.Msb

!DmaChl->DmaCfg.Lsb = 0x00000020;
!DmaChl->DmaCfg.Msb = 0x00001004;
            
            movlh       r8,     0x00000020
            stdu        r8,     r13,    2           !DmaChl->DmaCfg.Lsb
            
            movlh       r10,    0x00001004
            stdu        r10,    r13,    2           !DmaChl->DmaCfg.Msb

!DmaChl->DmaDsr = 0x00800070;            
            movlh       r8,     0x00800070

            add         r13,    2
            mov         r5,     1

            stdu        r8,     r13,    2           !DmaChl->DmaDsr
            
            movh        r8,     _Dma_RGB_I2E
            mov         r9,     2

            movl        r8,     _Dma_RGB_I2E
            mov         r4,     1

            st          r5,     r8,     0
            movh        r13,    _CALL_DmaChannelSetup
            
            st          r9,     r6,     3           !DmaChl->Priority
            movl        r13,    _CALL_DmaChannelSetup
            
            st          r8,     r6,     2           !DmaChl->pResFlag
            
            call        r13     
            
            mov         r5,     r12
            add         r5,     1
            
            ldu         r6,     r5,     1
            mov         %rpc,   r6
            lddu        r14,    r5,     2
            lddu        r10,    r5,     2
            lddu        r8,     r5,     2
            lddu        r2,     r5,     2
            lddu        r0,     r5,     2
            add         r5,     -1
            mov         r12,    r5
            ret
            
            