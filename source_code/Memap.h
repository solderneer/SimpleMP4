#ifndef _MEMAP_H
#define	_MEMAP_H

#define       ZSP_STACK           0xbfff

#define       LCD_COMMAND         ((0x1f900     )/2)
#define       LCD_DATA            ((0x1f900+0x04)/2)
#define       LCD_DATA1           ( 0x10000        )


#define       FLASH0_DATA         ((0x1fa00     )/2)
#define       FLASH0_ADDR         ((0x1fa00+0x04)/2)
#define       FLASH0_CMD          ((0x1fa00+0x08)/2)
#define       FLASH1_DATA         ((0x1fb00     )/2)
#define       FLASH1_ADDR         ((0x1fb00+0x04)/2)
#define       FLASH1_CMD          ((0x1fb00+0x08)/2)


#define       EXT_REG_BASE         0x1f800
#define       ECC0                ((EXT_REG_BASE+0x00)/2)
#define       ECC1                ((EXT_REG_BASE+0x04)/2)
#define       ECC2                ((EXT_REG_BASE+0x08)/2)
#define       ECC3                ((EXT_REG_BASE+0x0c)/2)
#define       ECCCTL              ((EXT_REG_BASE+0x10)/2)
#define       FMCTL               ((EXT_REG_BASE+0x14)/2)
#define       CFWAIT              ((EXT_REG_BASE+0x18)/2)
#define       FMWAIT              ((EXT_REG_BASE+0x1c)/2)
#define       LCDWAIT             ((EXT_REG_BASE+0x20)/2)
#define       SYSCTL              ((EXT_REG_BASE+0x24)/2)

#define       SDRAM_BASE          (0x4000000/2)
#define       ZSPMEM_BASE         (0x20000/2  )
#define       USB_BASE            (0x1e000/2  )
#define       EXTMEM_BASE         (0x00000/2  )
#define       EXTMEM_BASE2        (0x80000/2  )
#define       I2C_BASE            (0x1e800/2  )
#define       WDT_BASE            (0x1e400/2  )
                                        
                                        
#define       DW_APB_I2C_BASE     0x1e800
#define       IC_CON              ((DW_APB_I2C_BASE + 0x00)/2)   //  R/W  [5:0]   0x04
#define       IC_TAR              ((DW_APB_I2C_BASE + 0x04)/2)   //  R/W  [11:0]  0x55
#define       IC_SAR              ((DW_APB_I2C_BASE + 0x08)/2)   //  R/W  [9:0]   0x55
#define       IC_HS_MADDR         ((DW_APB_I2C_BASE + 0x0c)/2)
#define       IC_DATA_CMD         ((DW_APB_I2C_BASE + 0x10)/2)   //  R/W  [8:0]   0x0
#define       IC_SS_HCNT          ((DW_APB_I2C_BASE + 0x14)/2)   //  R/W  [15:0]  0x60
#define       IC_SS_LCNT          ((DW_APB_I2C_BASE + 0x18)/2)   //  R/W  [15:0]  0x80
#define       IC_FS_HCNT          ((DW_APB_I2C_BASE + 0x1c)/2)   //  R/W  [15:0]  0x10
#define       IC_FS_LCNT          ((DW_APB_I2C_BASE + 0x20)/2)   //  R/W  [15:0]  0x22
#define       IC_HS_HCNT          ((DW_APB_I2C_BASE + 0x24)/2)
#define       IC_HS_LCNT          ((DW_APB_I2C_BASE + 0x28)/2)
#define       IC_INTR_STAT        ((DW_APB_I2C_BASE + 0x2c)/2)
#define       IC_INTR_MASK        ((DW_APB_I2C_BASE + 0x30)/2)
#define       IC_RAW_INTR_STAT    ((DW_APB_I2C_BASE + 0x34)/2)
#define       IC_RX_TL            ((DW_APB_I2C_BASE + 0x38)/2)
#define       IC_TX_TL            ((DW_APB_I2C_BASE + 0x3c)/2)
#define       IC_CLR_INTR         ((DW_APB_I2C_BASE + 0x40)/2)
#define       IC_CLR_RX_UNDER     ((DW_APB_I2C_BASE + 0x44)/2)
#define       IC_CLR_RX_OVER      ((DW_APB_I2C_BASE + 0x48)/2)
#define       IC_CLR_TX_OVER      ((DW_APB_I2C_BASE + 0x4c)/2)
#define       IC_CLR_RD_REQ       ((DW_APB_I2C_BASE + 0x50)/2)
#define       IC_CLR_TX_ABRT      ((DW_APB_I2C_BASE + 0x54)/2)
#define       IC_CLR_RX_DONE      ((DW_APB_I2C_BASE + 0x58)/2)
#define       IC_CLR_ACTIVITY     ((DW_APB_I2C_BASE + 0x5c)/2)
#define       IC_CLR_STOP_DET     ((DW_APB_I2C_BASE + 0x60)/2)
#define       IC_CLR_START_DET    ((DW_APB_I2C_BASE + 0x64)/2)
#define       IC_CLR_GEN_CALL     ((DW_APB_I2C_BASE + 0x68)/2)
#define       IC_ENABLE           ((DW_APB_I2C_BASE + 0x6c)/2)
#define       IC_STATUS           ((DW_APB_I2C_BASE + 0x70)/2)   //  R    [4:0]   0x6
#define       IC_TXFLR            ((DW_APB_I2C_BASE + 0x74)/2)
#define       IC_RXFLR            ((DW_APB_I2C_BASE + 0x78)/2)
#define       IC_SRESET           ((DW_APB_I2C_BASE + 0x7c)/2)
#define       IC_TX_ABRT_SOURCE   ((DW_APB_I2C_BASE + 0x80)/2)
#define       IC_VERSION_ID       ((DW_APB_I2C_BASE + 0xf8)/2)
#define       IC_DMA_CR           ((DW_APB_I2C_BASE + 0x88)/2)
#define       IC_DMA_TDLR         ((DW_APB_I2C_BASE + 0x8c)/2)
#define       IC_DMA_RDLR         ((DW_APB_I2C_BASE + 0x90)/2)
#define       I2CPING_1BIT_WR     ((IC_TAR)/2                )


#define       DMACBASE            0x1fc00                //  R/W     Width   Reset Value
#define       DMAR_SAR0           ((DMACBASE+0x000)/2)     //  R/W     32   0x0
#define       DMAR_DAR0           ((DMACBASE+0x008)/2)     //  R/W     32   0x0
#define       DMAR_CTL0           ((DMACBASE+0x018)/2)     //  R/W     64   0x00000002_00004825
#define       DMAR_CFG0           ((DMACBASE+0x040)/2)     //  R/W     64   0x00000004_00000c00
#define       DMAR_SGR0           ((DMACBASE+0x048)/2)     //  R/W     32   0x0
#define       DMAR_DSR0           ((DMACBASE+0x050)/2)     //  R/W     32   0x0
#define       DMAR_SAR1           ((DMACBASE+0x058)/2)     //  R/W     32   0x0
#define       DMAR_DAR1           ((DMACBASE+0x060)/2)     //  R/W     32   0x0
#define       DMAR_CTL1           ((DMACBASE+0x070)/2)     //  R/W     64   0x00000002_00004825
#define       DMAR_CFG1           ((DMACBASE+0x098)/2)     //  R/W     64   0x00000004_00000c20
#define       DMAR_SGR1           ((DMACBASE+0x0a0)/2)     //  R/W     32   0x0
#define       DMAR_DSR1           ((DMACBASE+0x0a8)/2)     //  R/W     32   0x0
#define       DMAR_SAR2           ((DMACBASE+0x0b0)/2)     //  R/W     32   0x0
#define       DMAR_DAR2           ((DMACBASE+0x0b8)/2)     //  R/W     32   0x0
#define       DMAR_CTL2           ((DMACBASE+0x0c8)/2)     //  R/W     64   0x00000002_00004825     
#define       DMAR_CFG2           ((DMACBASE+0x0f0)/2)     //  R/W     64   0x00000004_00000c40     
#define       DMAR_SGR2           ((DMACBASE+0x0f8)/2)     //  R/W     32   0x0
#define       DMAR_DSR2           ((DMACBASE+0x100)/2)     //  R/W     32   0x0
#define       DMAR_RawBlock       ((DMACBASE+0x2c8)/2)     //  R       3    0x0
#define       DMAR_RawSrcTran     ((DMACBASE+0x2d0)/2)     //  R       3    0x0
#define       DMAR_RawDstTran     ((DMACBASE+0x2d8)/2)     //  R       3    0x0
#define       DMAR_RawErr         ((DMACBASE+0x2e0)/2)     //  R       3    0x0
#define       DMAR_RawTfr         ((DMACBASE+0x2c0)/2)     //  R       3    0x0
#define       DMAR_StatusTfr      ((DMACBASE+0x2e8)/2)     //  R       3    0x0
#define       DMAR_StatusBlock    ((DMACBASE+0x2f0)/2)     //  R       3    0x0
#define       DMAR_StatusBlock    ((DMACBASE+0x2f0)/2)     //  R       3    0x0
#define       DMAR_StatusSrcTran  ((DMACBASE+0x2f8)/2)     //  R       3    0x0
#define       DMAR_StatusDstTran  ((DMACBASE+0x300)/2)     //  R       3    0x0
#define       DMAR_StatusErr      ((DMACBASE+0x308)/2)     //  R       3    0x0
#define       DMAR_MaskTfr        ((DMACBASE+0x310)/2)     //  R/W     3-3  0x0
#define       DMAR_MaskBlock      ((DMACBASE+0x318)/2)     //  R/W     3-3  0x0
#define       DMAR_MaskSrcTran    ((DMACBASE+0x320)/2)     //  R/W     3-3  0x0
#define       DMAR_MaskDstTran    ((DMACBASE+0x328)/2)     //  R/W     3-3  0x0
#define       DMAR_MaskErr        ((DMACBASE+0x330)/2)     //  R/W     3-3  0x0
#define       DMAR_ClearTfr       ((DMACBASE+0x338)/2)     //  W       3    0x0
#define       DMAR_ClearBlock     ((DMACBASE+0x340)/2)     //  W       3    0x0
#define       DMAR_ClearSrcTran   ((DMACBASE+0x348)/2)     //  W       3    0x0
#define       DMAR_ClearDstTran   ((DMACBASE+0x350)/2)     //  W       3    0x0
#define       DMAR_ClearErr       ((DMACBASE+0x358)/2)     //  W       3    0x0
#define       DMAR_StatusInt      ((DMACBASE+0x360)/2)     //  W       5    0x0
#define       DMAR_ReqSrcReg      ((DMACBASE+0x368)/2)     //  R/W     3-3  0x0
#define       DMAR_ReqDstReg      ((DMACBASE+0x370)/2)     //  R/W     3-3  0x0
#define       DMAR_SglReqSrcReg   ((DMACBASE+0x378)/2)     //  R/W     3-3  0x0
#define       DMAR_SglReqDstReg   ((DMACBASE+0x380)/2)     //  R/W     3-3  0x0
#define       DMAR_LstSrcReg      ((DMACBASE+0x388)/2)     //  R/W     3-3  0x0
#define       DMAR_LstDstReg      ((DMACBASE+0x390)/2)     //  R/W     3-3  0x0
#define       DMAR_DmaCfgReg      ((DMACBASE+0x398)/2)     //  R/W     1    0x0
#define       DMAR_ChEnReg        ((DMACBASE+0x3a0)/2)     //  R/W     3-3  0x0
#define       DMAR_DmaIdReg       ((DMACBASE+0x3a8)/2)     //  R       32      
#define       DMAR_DmaTestReg     ((DMACBASE+0x3b0)/2)     //  R/W     1    0x0
#define       DMAR_DMACID         ((DMACBASE+0x3f8)/2)     //  R       64   0x0x44_57_11_10


#define       DW_APB_WDT_BASE     0x1e400
#define       WDT_CR              ((DW_APB_WDT_BASE + 0x00)/2)   //  R/W  [4:0]   0x0
#define       WDT_TORR            ((DW_APB_WDT_BASE + 0x04)/2)   //  R/W  [3:0]   0x7
#define       WDT_CCVR            ((DW_APB_WDT_BASE + 0x08)/2)   //  R    [31:0]  0x7fffff
#define       WDT_CRR             ((DW_APB_WDT_BASE + 0x0C)/2)   //  R/W  [7:0]   0x0  //0x76
#define       WDT_STAT            ((DW_APB_WDT_BASE + 0x10)/2)   //  R    [0]     0x0
#define       WDT_EOI             ((DW_APB_WDT_BASE + 0x14)/2)   //  R    [0]     0x0
#define       WDTPING_1BIT_WR     ((WDT_CR)/2                )


#define       DW_MEMCTL_BASE      0x0000
#define       MEMCTL_SCONR        ((DW_MEMCTL_BASE + 0x00)/2)   //  R/W  [31:0]  0x1c1168
#define       MEMCTL_STMG0R       ((DW_MEMCTL_BASE + 0x04)/2)   //  R/W  [31:0]  0x19d9451
#define       MEMCTL_STMG1R       ((DW_MEMCTL_BASE + 0x08)/2)   //  R/W  [31:0]  0x70008
#define       MEMCTL_SCTLR        ((DW_MEMCTL_BASE + 0x0c)/2)   //  R/W  [31:0]  0x2009
#define       MEMCTL_SREFR        ((DW_MEMCTL_BASE + 0x10)/2)   //  R/W  [31:0]  0x64
#define       MEMCTL_SCSLR0_LOW   ((DW_MEMCTL_BASE + 0x14)/2)
#define       MEMCTL_SCSLR0_HIGH  ((DW_MEMCTL_BASE + 0x34)/2)
#define       MEMCTL_SMSKR0       ((DW_MEMCTL_BASE + 0x54)/2)   //  R/W  [31:0]  0x80b
#define       MEMCTL_SMTMGR_SET0  ((DW_MEMCTL_BASE + 0x94)/2)   //  R/W  [31:0]  0x10441
#define       MEMCTL_SMTMGR_SET1  ((DW_MEMCTL_BASE + 0x98)/2)   //  R/W  [31:0]  0x7c4f5b
#define       MEMCTL_SMTMGR_SET2  ((DW_MEMCTL_BASE + 0x9c)/2)   //  R/W  [31:0]  0x1c4f5b
#define       MEMCTL_FLASH_TRPDR  ((DW_MEMCTL_BASE + 0xa0)/2)   //  R/W  [31:0]  0xc8
#define       MEMCTL_SMCTLR       ((DW_MEMCTL_BASE + 0xa4)/2)   //  R/W  [31:0]  0x2480
#define       MEMCTLMEMCTL_PING_1BIT_WR  ((MEMCTL_SCONR)/2)


/*///////// Registers of APB Register File /////////*/
#define       I2DSP_TXCONF       (0x1ec00/2)    //w/r [5:0];	   6'h0; //i2dsp 0x1_ec00
#define       I2DSP_TXCOM        (0x1ec08/2)    //w/r [2:0];     3'h0; // tx_en,i1dsp_en,tx_dma_en  
#define       I2DSP_TXDB         (0x1ec10/2)    //w             16'h0;
#define       I2DSP_DPCTRL       (0x1ec20/2)    //w/r [1:0];     2'h0; // rxfiforst,txfiforst
#define       I2DSP_RXCONF       (0x1ec30/2)    //w/r [5:0];     6'h0; // [5:3]:fs,[2]:master,[1:0]:24-16bits
#define       I2DSP_RXCOM        (0x1ec34/2)    //w/r [1:0];     2'h0; // rx_en,i1dsp_en,rx_dma_en
#define       I2DSP_STATUS       (0x1ec38/2)    //r   [2:0];     3'h0; // rxfifonemp,txfifonfull  
#define       I2DSP_RXDB         (0x1ec40/2)    //r   [2:0];     3'h0;
#define       I2C_EXT            (0x1ec60/2)    //w/r [1:0];     2'h0; //I2DSP_EXT,I2C_EXT
#define       SW_CODEC_RSTN      (0x1ec64/2)    //w/r [0];       1'h0; //SW_CODEC_RSTN

#define       PWM_TACMD          (0x1ec84/2)    //w/r [1:0];	   2'h0; //pwm  0x1_ec80
#define       PWM_TADATA0        (0x1ec88/2)    //w/r [15:0];   16'h0;
#define       PWM_TADATA1        (0x1ec8c/2)    //w/r [15:0];   16'h0;
#define       PWM_TAPRE          (0x1ec90/2)    //w/r [9:0];    10'h0;

#define       ADC_ADCCON         (0x1ed00/2)    //w/r [4:0];	   5'h0; //adc  0x1_ed00   
#define       ADC_ADCDAT0        (0x1ed04/2)    //r   [9:0];    10'h3ff
#define       ADC_ADCFRE         (0x1ed08/2)    //w/r [7:0];     8'h9;
#define       ADC_ADCRDY         (0x1ed0c/2)    //r   [0];       1'h1

#define       DCDC_CON0          (0x1ed80/2)    //w/r [2:0];     3'h0; //dcdc 0x1_ed80 
#define       DCDC_CON1          (0x1ed88/2)    //w/r [2:0];     3'h0;                 

#define       CLOCK_MCLKCON      (0x1ee00/2)    //w/r [7:0];     8'h1; //clock 0x1_ee00
#define       CLOCK_I2SMCLKCON   (0x1ee04/2)    //w/r [8:0];     9'h107
#define       CLOCK_AHBCLKCON    (0x1ee08/2)    //w/r [1:0];     2'h0; 
#define       CLOCK_APBCLKCON    (0x1ee0c/2)    //w/r [1:0];     2'h0; 
#define       CLOCK_PLL_NDIV     (0x1ee10/2)    //w/r [4:0];     5'h6; 
#define       CLOCK_PLL_MDIV     (0x1ee14/2)    //w/r [8:0];     9'h1e;
#define       CLOCK_PLL_ODDIV    (0x1ee18/2)    //w/r [1:0];     2'h0; 
#define       CLOCK_PLL_PDBP     (0x1ee1c/2)    //w/r [1:0];     2'h1; 
#define       CLOCK_PLL_BP       (0x1ee1c/2)    //w/r [1:0];     2'h1; 
#define       CLOCK_PWRCON       (0x1ee38/2)    //w/r [4:0];     5'h1f; 

#define       GPIO_PCON0         (0x1ee80/2)  //w/r [15:0];	  16'b0; //gpio 0x1_ee80 
#define       GPIO_PDAT0         (0x1ee88/2)    //w/r [7:0];     8'b0;
#define       GPIO_PCON1         (0x1ee90/2)    //w/r [15:0];   16'b0;
#define       GPIO_PDAT1         (0x1ee98/2)    //w/r [7:0];     8'b0;
#define       GPIO_PCON2A        (0x1eea0/2)    //w/r [15:0];   16'b0;
#define       GPIO_PCON2B        (0x1eea4/2)    //w/r [15:0];   16'b0;
#define       GPIO_PDAT2         (0x1eea8/2)    //w/r [15:0];   16'b0;
#define       GPIO_PCON3         (0x1eeb0/2)    //w/r [15:0];   16'b0;
#define       GPIO_PDAT3         (0x1eeb8/2)    //w/r [7:0];     8'b0;
#define       GPIO_EXTINTR       (0x1eec0/2)    //w/r [7:0];     8'b0;

#define       SDCI_CTRL          (0x1ef00/2)    //w/r [4:0];     5'h0; //sdmmc 0x1_ef00
#define       SDCI_DCTRL         (0x1ef04/2)    //w/r [2:0];     3'h0;
#define       SDCI_DLEN          (0x1ef08/2)    //w/r [15:0];   16'h0;
#define       SDCI_TOKEN         (0x1ef0c/2)    //w/r [7:0];    8'h0; 
#define       SDCI_CMD           (0x1ef10/2)    //w/r {[15:8],1'b0,[6:0]};  16'h0;
#define       SDCI_ARG           (0x1ef14/2)    //w/r [31:0];         32'h0;
#define       SDCI_STAC          (0x1ef18/2)    //w   {[15:14],5'b0,[8:0]}; 16'h0;
#define       SDCI_STA           (0x1ef1c/2)    //r   [15:0];   16'h3000;
#define       SDCI_RESP0         (0x1ef20/2)    //r   [31:0];   32'h0;
#define       SDCI_RESP1         (0x1ef24/2)    //r   [7:0];    8'h0;
#define       SDCI_TX            (0x1ef28/2)    //w   [31:0];   32'h0;
#define       SDCI_RX            (0x1ef2C/2)    //r   [31:0];   32'h0;
#define       SDCI_CRC16         (0x1ef30/2)    //r   [15:0];   32'h0;

//codec i2c address
#define       P_ADD_AICR         (0x0000)  //0b00000000    //w/r  [7:0];         8'h40//00
#define       P_ADD_CR1          (0x0002)  //0b00000010    //w/r  [7]r,[6:0];    8'h08//88
#define       P_ADD_CR2          (0x0004)  //0b00000100    //w/r  [7]r,[6:0];    8'h78
#define       P_ADD_CCR          (0x0006)  //0b00000110    //w/r  [7:4],[2:0];   8'h00
#define       P_ADD_PMR1         (0x0008)  //0b00001000    //w/r  [7:0];         8'h27//26
#define       P_ADD_PMR2         (0x000a)  //0b00001010    //w/r  [7:3],[1:0];   8'h00
#define       P_ADD_CGR1         (0x000c)  //0b00001100    //w/r  [7:0];         8'h00
#define       P_ADD_CGR2         (0x000e)  //0b00001110    //w/r  [7:6],[4:0];   8'h04
#define       P_ADD_CGR3         (0x0010)  //0b00010000    //w/r  [4:0];         8'h04
#define       P_ADD_CGR4         (0x0012)  //0b00010010    //w/r  [7:6],[4:0];   8'h04
#define       P_ADD_CGR5         (0x0014)  //0b00010100    //w/r  [4:0];         8'h04
#define       P_ADD_CGR6         (0x0016)  //0b00010110    //w/r  [7:6],[4:0];   8'h04
#define       P_ADD_CGR7         (0x0018)  //0b00011000    //w/r  [4:0];         8'h04
#define       P_ADD_CGR8         (0x001a)  //0b00011010    //w/r  [7:6],[4:0];   8'h0a
#define       P_ADD_CGR9         (0x001c)  //0b00011100    //w/r  [4:0];         8'h0a
#define       P_ADD_CGR10        (0x001e)  //0b00011110    //w/r  [7:0];         8'h00
#define       P_ADD_TR1          (0x0020)  //0b00100000    //w/r  [7:0];         8'h00
#define       P_ADD_TR2          (0x0022)  //0b00100010    //w/r  [7:0];         8'hd8

#endif

