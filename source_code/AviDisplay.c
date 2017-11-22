#define     _IN_AVIDISPLAY_C
#include "Macro.h"
#include "Global.h"
#include "Resource.h"
#include "Lcd.h"
#include "lcdchar.h"
#include "AviGlobal.h"
#include "AviMacro.h"

#include "Dma.h"

extern void LCDDEV_Init(unsigned int InitType);

//------------------------------------------------------------------------------
void AviDmaDram2Sdram(unsigned int size, unsigned int *pDramAddr, unsigned long sdram_addr);
void AviDmaSdram2Sdram(unsigned int size, unsigned long sdram_sar, unsigned long sdram_dar);
void DmaCopyPic2Sdram(unsigned int xsize, unsigned int ysize, unsigned long pic_nfa, unsigned long sdram_addr);
void AviCopyPic2Sdram(void); 
void AviDisplayPictuer(int offset_x, int offset_y, unsigned int pic_num, AVIBITMAP *pBitMap);
//------------------------------------------------------------------------------

#define TIME_GUAGE_CONST               (240 - 8)


AVIBITMAP 
AviPicterInfo[] __attribute__((section(".avi_v_data, \"aw\""))) = {

/*
*以下为图片的信息段
*图片坐标 x,y
*图片大小 xsize, ysize
*图片数据在文件中的偏移
*图片个数
*/
{
    VIDEO_UPMAIN_PIC_X, 
    VIDEO_UPMAIN_PIC_Y, 
    VIDEO_UPMAIN_PIC_XSIZE, 
    VIDEO_UPMAIN_PIC_YSIZE,
    NFA_VIDEO_UPMAIN_PIC - NFA_VIDEO_UPMAIN_PIC, 
    VIDEO_UPMAIN_PIC_CTL
},
{    
    VIDEO_VOLBK_PIC_X,
    VIDEO_VOLBK_PIC_Y,
    VIDEO_VOLBK_PIC_XSIZE,
    VIDEO_VOLBK_PIC_YSIZE,
    VIDEO_VOLBK_PIC - NFA_VIDEO_UPMAIN_PIC,
    VIDEO_VOLBK_PIC_CTL
},
{
    VIDEO_HOLD_PIC_X,
    VIDEO_HOLD_PIC_Y,
    VIDEO_HOLD_PIC_XSIZE,
    VIDEO_HOLD_PIC_YSIZE,
    NFA_VIDEO_HOLD_PIC - NFA_VIDEO_UPMAIN_PIC,
    VIDEO_HOLD_PIC_CTL
},
{
    VIDEO_BATT_PIC_X,                
    VIDEO_BATT_PIC_Y,               
    VIDEO_BATT_PIC_XSIZE,            
    VIDEO_BATT_PIC_YSIZE,            
    NFA_VIDEO_BATT_PIC - NFA_VIDEO_UPMAIN_PIC,             
    VIDEO_BATT_PIC_CTL              
},
{
    VIDEO_DOWNMAIN_PIC_X    ,
    VIDEO_DOWNMAIN_PIC_Y    ,
    VIDEO_DOWNMAIN_PIC_XSIZE,
    VIDEO_DOWNMAIN_PIC_YSIZE,
    NFA_VIDEO_DOWNMAIN_PIC - NFA_VIDEO_UPMAIN_PIC,
    VIDEO_DOWNMAIN_PIC_CTL  
},
{
    VIDEO_PLAY_STATUS_PIC_X    ,    
    VIDEO_PLAY_STATUS_PIC_Y    ,    
    VIDEO_PLAY_STATUS_PIC_XSIZE,    
    VIDEO_PLAY_STATUS_PIC_YSIZE,    
    NFA_VIDEO_PLAY_STATUS_PIC - NFA_VIDEO_UPMAIN_PIC,    
    VIDEO_PLAY_STATUS_PIC_CTL  
},
{
    VIDEO_GUAGE_PIC_X    ,    
    VIDEO_GUAGE_PIC_Y    ,
    VIDEO_GUAGE_PIC_XSIZE,
    VIDEO_GUAGE_PIC_YSIZE,
    NFA_VIDEO_GUAGE_PIC - NFA_VIDEO_UPMAIN_PIC,
    VIDEO_GUAGE_PIC_CTL  
},
{
    VIDEO_GUAGEBLOCK_PIC_X    ,  
    VIDEO_GUAGEBLOCK_PIC_Y    ,
    VIDEO_GUAGEBLOCK_PIC_XSIZE,
    VIDEO_GUAGEBLOCK_PIC_YSIZE,
    NFA_VIDEO_GUAGEBLOCK_PIC - NFA_VIDEO_UPMAIN_PIC,
    VIDEO_GUAGEBLOCK_PIC_CTL  
},
{
    VIDEO_TIMECURRBK_PIC_X    ,    
    VIDEO_TIMECURRBK_PIC_Y    ,
    VIDEO_TIMECURRBK_PIC_XSIZE,
    VIDEO_TIMECURRBK_PIC_YSIZE,
    NFA_VIDEO_TIMECURRBK_PIC - NFA_VIDEO_UPMAIN_PIC,
    VIDEO_TIMECURRBK_PIC_CTL  
},     
{
    VIDEO_TIMETOTALBK_PIC_X    ,    
    VIDEO_TIMETOTALBK_PIC_Y    ,
    VIDEO_TIMETOTALBK_PIC_XSIZE,
    VIDEO_TIMETOTALBK_PIC_YSIZE,
    NFA_VIDEO_TIMETOTALBK_PIC - NFA_VIDEO_UPMAIN_PIC,
    VIDEO_TIMETOTALBK_PIC_CTL  
}
};

__attribute__((section(".avi_v_text, \"ax\"")))
void DISP_Mp4(unsigned int DispFlagBits){   
#if 1
    unsigned int  MusicDispBuf[32];
    unsigned long tmpAviPlayedSecond;
    unsigned long tmpAviTotalSecond;
    unsigned int  i, j, k;
    unsigned int  JdtCont;
    unsigned int  TempColor;
    AVIBITMAP     *pBitMap;
    
    LCD_Init(0);
    
    TempColor = LCD_GetColor();
    
    if(DispFlagBits == AVI_STATUS_BAR_DISP_ALL){ 
//        //DI;
//        DisplayPicture(VIDEO_UPMAIN_PIC_X, VIDEO_UPMAIN_PIC_Y, VIDEO_UPMAIN_PIC_XSIZE, VIDEO_UPMAIN_PIC_YSIZE, VIDEO_UPMAIN_PIC);
//        DisplayPicture(VIDEO_DOWNMAIN_PIC_X, VIDEO_DOWNMAIN_PIC_Y, VIDEO_DOWNMAIN_PIC_XSIZE, VIDEO_DOWNMAIN_PIC_YSIZE, VIDEO_DOWNMAIN_PIC);
//        //EI;
        pBitMap = &AviPicterInfo[0];
        AviDisplayPictuer(0,0,0, pBitMap);
        pBitMap = &AviPicterInfo[4];
        AviDisplayPictuer(0,0,0, pBitMap);         
    }
    
    if(DispFlagBits & AVI_STATUS_BAR_VOL_BMASK){
//        DisplayPicture(VIDEO_VOLBK_PIC_X, VIDEO_VOLBK_PIC_Y, VIDEO_VOLBK_PIC_XSIZE, VIDEO_VOLBK_PIC_YSIZE, VIDEO_VOLBK_PIC);
        pBitMap = &AviPicterInfo[1];
        AviDisplayPictuer(0,0,0, pBitMap);
        
        MusicDispBuf[0] = ((OutputVolume)-0)/10 + '0';
        MusicDispBuf[1] = ((OutputVolume)-0)%10 + '0';
        MusicDispBuf[2] = 0;
        
        LCD_NFDispStringAt(VIDEO_VOLBK_PIC_X + 2,VIDEO_VOLBK_PIC_Y,MusicDispBuf);
    }
    
    if(DispFlagBits & AVI_STATUS_BAR_LOCK_BMASK){
//        DisplayPicture(VIDEO_HOLD_PIC_X, VIDEO_HOLD_PIC_Y, VIDEO_HOLD_PIC_XSIZE, VIDEO_HOLD_PIC_YSIZE, (NFA_VIDEO_HOLD_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * VIDEO_HOLD_PIC_SIZE));
        pBitMap = &AviPicterInfo[2];
        AviDisplayPictuer(0,0,SystemInfo.bc.bHoldOn, pBitMap);
    }
    if(DispFlagBits & AVI_STATUS_BAR_BATT_BMASK){
//        DisplayPicture(VIDEO_BATT_PIC_X, VIDEO_BATT_PIC_Y, VIDEO_BATT_PIC_XSIZE, VIDEO_BATT_PIC_YSIZE, (NFA_VIDEO_BATT_PIC + (unsigned long)(BatteryLevel) * VIDEO_BATT_PIC_SIZE ));
        pBitMap = &AviPicterInfo[3];
        AviDisplayPictuer(0,0,BatteryLevel, pBitMap);
    }
    
    
    if(DispFlagBits & AVI_STATUS_BAR_STATE_BMASK){
//        DisplayPicture(VIDEO_PLAY_STATUS_PIC_X, VIDEO_PLAY_STATUS_PIC_Y, VIDEO_PLAY_STATUS_PIC_XSIZE, VIDEO_PLAY_STATUS_PIC_YSIZE, (NFA_VIDEO_PLAY_STATUS_PIC + (unsigned long)(AviPlayState) * VIDEO_PLAY_STATUS_PIC_SIZE));
        pBitMap = &AviPicterInfo[5];
        AviDisplayPictuer(0,0,AviPlayState, pBitMap);
    }
    
    if(DispFlagBits & (AVI_STATUS_BAR_JDT_BMASK | AVI_STATUS_BAR_TMR_BMASK)){
        tmpAviPlayedSecond = (AviStreamInfo.MicroSecondPerFrame * AviCurrFrameNum) / 100000;
        tmpAviTotalSecond  = (AviStreamInfo.MicroSecondPerFrame * AviStreamInfo.TotalFrameCount) / 100000;
        JdtCont = (tmpAviPlayedSecond * TIME_GUAGE_CONST) / tmpAviTotalSecond;
        pBitMap = &AviPicterInfo[6];
        
        for (i = 0; i < JdtCont; i ++) {
//            DisplayPicture(VIDEO_GUAGE_PIC_X + i, VIDEO_GUAGE_PIC_Y, VIDEO_GUAGE_PIC_XSIZE, VIDEO_GUAGE_PIC_YSIZE, VIDEO_GUAGE_FRONT_PIC);
            AviDisplayPictuer(i,0,1, pBitMap);
        }
        for (i = JdtCont + 8; i < TIME_GUAGE_CONST; i ++ ) {
//            DisplayPicture(VIDEO_GUAGE_PIC_X + i, VIDEO_GUAGE_PIC_Y, VIDEO_GUAGE_PIC_XSIZE, VIDEO_GUAGE_PIC_YSIZE, VIDEO_GUAGE_BKGROUND_PIC);
            AviDisplayPictuer(i,0,0, pBitMap);
        }
//        DisplayPicture(VIDEO_GUAGEBLOCK_PIC_X + JdtCont, VIDEO_GUAGEBLOCK_PIC_Y, VIDEO_GUAGEBLOCK_PIC_XSIZE, VIDEO_GUAGEBLOCK_PIC_YSIZE, VIDEO_GUAGE_BLOCK_PIC);
        pBitMap++;
        AviDisplayPictuer(JdtCont,0,0, pBitMap);
        
        LCD_SetColor(COLOR_BLACK);
        GetTimeHMS(tmpAviPlayedSecond,&i,&j,&k);
        ConTime2String(1, i, j, k, MusicDispBuf);
//        DisplayPicture(VIDEO_TIMECURRBK_PIC_X, VIDEO_TIMECURRBK_PIC_Y, VIDEO_TIMECURRBK_PIC_XSIZE, VIDEO_TIMECURRBK_PIC_YSIZE, VIDEO_TIMECURRBK_PIC);
        pBitMap++;
        AviDisplayPictuer(0,0,0, pBitMap);
        LCD_NFDispStringAt(VIDEO_TIMECURRBK_PIC_X,VIDEO_TIMECURRBK_PIC_Y,MusicDispBuf);
        
        tmpAviTotalSecond = tmpAviTotalSecond - tmpAviPlayedSecond;
        GetTimeHMS(tmpAviTotalSecond, &i, &j, &k);
        ConTime2String(-1, i, j, k, MusicDispBuf);
//        DisplayPicture(VIDEO_TIMETOTALBK_PIC_X, VIDEO_TIMETOTALBK_PIC_Y, VIDEO_TIMETOTALBK_PIC_XSIZE, VIDEO_TIMETOTALBK_PIC_YSIZE, VIDEO_TIMETOTALBK_PIC);
        pBitMap++;
        AviDisplayPictuer(0,0,0, pBitMap);
        LCD_NFDispStringAt(VIDEO_TIMETOTALBK_PIC_X,VIDEO_TIMETOTALBK_PIC_Y,MusicDispBuf);
     }
    LCD_SetColor(TempColor);
#ifdef LCD_MEMORY_DEV
    if(DispFlagBits != 0){
        DMA_DisplayPicture(VIDEO_UPMAIN_PIC_X, VIDEO_UPMAIN_PIC_Y, VIDEO_UPMAIN_PIC_XSIZE, VIDEO_UPMAIN_PIC_YSIZE);
        DMA_DisplayPicture(VIDEO_DOWNMAIN_PIC_X, VIDEO_DOWNMAIN_PIC_Y, VIDEO_DOWNMAIN_PIC_XSIZE, VIDEO_DOWNMAIN_PIC_YSIZE);
    }
#endif
    MP4_LCD_Init();
#endif
}

__attribute__((section(".avi_v_text, \"ax\"")))
void AviDmaDram2Sdram(unsigned int size, unsigned int *pDramAddr, unsigned long sdram_addr)
{
    DMA_CHANNEL_REG *DmaChl;
    volatile unsigned int dma_done;
    
    unsigned int imask_tmp;
    unsigned long pData;
    
    pData = (unsigned long)((unsigned int)pDramAddr);
    
    while((DMA_CHANNEL_REG *)DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 

//---------------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//--------------------------------------------------------------------- 
    
    DmaChl->DmaSar = (unsigned long)((((unsigned long)(pData))<<1)+(unsigned long)0x40000);
    DmaChl->DmaDar = ((unsigned long)(sdram_addr)<<1)+(unsigned long)0x4000000;
    

    DmaChl->DmaCtl.Lsb = 0x00000013;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    DmaChl->DmaCtl.Msb = (unsigned long)(size);             //224*16/2
    
    DmaChl->DmaCfg.Lsb = 0x00000020;             //channel priority set
    DmaChl->DmaCfg.Msb = 0x00001004;

    DmaChl->DmaSgr = 0x00000000;
    DmaChl->DmaDsr = 0x00000000;           //(176-16)/2

    DmaChl->Priority = 0;
    dma_done     = 1;
    DmaChl->pResFlag = &dma_done;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
    while(dma_done);
}

__attribute__((section(".avi_v_text, \"ax\"")))
void AviDmaSdram2Sdram(unsigned int size, unsigned long sdram_sar, unsigned long sdram_dar)
{
    DMA_CHANNEL_REG *DmaChl;
    volatile unsigned int dma_done;
    
    unsigned int imask_tmp;

    while((DMA_CHANNEL_REG *)DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 

//---------------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//--------------------------------------------------------------------- 
    
    DmaChl->DmaSar = ((unsigned long)(sdram_sar)<<1)+(unsigned long)0x4000000;
    DmaChl->DmaDar = ((unsigned long)(sdram_dar)<<1)+(unsigned long)0x4000000;
    

    DmaChl->DmaCtl.Lsb = 0x00000013;             //DST_SCATTER_EN:1 SRC_TR_WIDTH:32 DST_TR_WIDTH:32
    DmaChl->DmaCtl.Msb = (unsigned long)(size);             //224*16/2
    
    DmaChl->DmaCfg.Lsb = 0x00000020;             //channel priority set
    DmaChl->DmaCfg.Msb = 0x00001004;

    DmaChl->DmaSgr = 0x00000000;
    DmaChl->DmaDsr = 0x00000000;           //(176-16)/2

    DmaChl->Priority = 0;
    dma_done     = 1;
    DmaChl->pResFlag = &dma_done;
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(1);
    DMA_ENABLE_INTERRUPT;
    while(dma_done);
}

__attribute__((section(".avi_v_text, \"ax\"")))
void DmaCopyPic2Sdram(unsigned int xsize, unsigned int ysize, unsigned long pic_nfa, unsigned long sdram_addr)
{
    unsigned int FlashImageBuf[LCD_MAX_XSIZE<<1];
    
    pic_nfa += ImageLogicAddress;
    for(; ysize>0; ysize--)
    {
        FlashReadLogic(pic_nfa, FlashImageBuf, xsize<<1);
		Byte2Word(FlashImageBuf, FlashImageBuf, xsize);
		AviDmaDram2Sdram(xsize, FlashImageBuf, sdram_addr);
		pic_nfa += (xsize<<1);
		sdram_addr += xsize;
    }
}

__attribute__((section(".avi_v_text, \"ax\"")))
void AviCopyPic2Sdram(void)
{
    unsigned int    i;
    unsigned int    j;
    unsigned long   pic_nfa;
    unsigned long   sdram_addr;
    AVIBITMAP       *pBitMap;
    
    pBitMap = &AviPicterInfo[0];
    for(i=0; i<11; i++){
        sdram_addr = pBitMap->offset+(unsigned long)0x3e0000;
        pic_nfa    = pBitMap->offset+(unsigned long)NFA_VIDEO_UPMAIN_PIC;
        
        for(j=0; j<pBitMap->pic_count; j++){
            DmaCopyPic2Sdram(pBitMap->xsize, 
                             pBitMap->ysize, 
                             pic_nfa+(unsigned long)(pBitMap->xsize)*pBitMap->ysize*j*2, 
                             sdram_addr);
            sdram_addr += (unsigned long)(pBitMap->xsize)*(pBitMap->ysize);
        }
        pBitMap++; 
    }    
}

__attribute__((section(".avi_v_text, \"ax\"")))
void AviDisplayPictuer(int offset_x, int offset_y, unsigned int pic_num, AVIBITMAP *pBitMap)
{
    unsigned int i;
    unsigned long sdram_sdr=(unsigned long)0x3e0000;
    unsigned long sdram_dar=(unsigned long)0x3c0000;
    unsigned long pic_sdram_addr;
    
    sdram_sdr      += (unsigned long)(pBitMap->offset + (unsigned long)(pBitMap->xsize) * pBitMap->ysize * pic_num); 
    pic_sdram_addr = (unsigned long)(LCD_MAX_XSIZE)*(pBitMap->y + offset_y)+(pBitMap->x+offset_x);
    sdram_dar      += pic_sdram_addr;
    
    for(i=0; i<pBitMap->ysize; i++){
        AviDmaSdram2Sdram(pBitMap->xsize, sdram_sdr, sdram_dar);
        sdram_dar += LCD_MAX_XSIZE;
        sdram_sdr += pBitMap->xsize;
    }
}

