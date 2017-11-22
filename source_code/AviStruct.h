/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _AVISTRUCT_H
#define _AVISTRUCT_H
//------------------------------------------------------------------------------
typedef struct {
    unsigned int    AviFileFormat;                  /*  
                                                      1：音频流
                                                      2：视频流
                                                      3：音视频流 
                                                       ：非法流格式
                                                    */
    unsigned long   MicroSecondPerFrame;            /*每贞的播放时间，以10uS为单位*/
    unsigned long   TotalFrameCount;                /*总贞数*/
    
    unsigned long   StartDataPos;                   /*音视频数据块的起始文件偏移地址*/
    unsigned long   StartIndexPos;                  /*音视频索引块的起始文件偏移地址*/
    unsigned long   AudioSamplingRate;               /*音频的采样率*/
} AVI_STREAM_INFO;

typedef union {
    struct 
    {
        unsigned int    bMp3DecodeEnable:       1,
                        bMp3DecodeInit:         1,
                        bMp3BufferIndex:        1,
                        bMp3DecodeError:        1,
                            
                        bMp4DecodeEnable:       1,
                        bMp4DecodeInit:         1,
                        bRGBBufferIndex:        1,
                        
                        bMP4DecodeBufferFull:   1,
                        bRGBBufferEmpty:        1,
                        
                        bAviFileFFW:            1,
                        bAviFileFFD:            1,
                        bAviFirstPause:         1,      // Add on 20060220 for display play information when pause state
                        bAviFirstLongDown:      1,
                        bAviFindIFrameOK:       1,
                            
                        bReserved:              2;
    }bc;
    unsigned int word;
} AVI_PLAY_FLAG;

typedef union{
    struct{
        unsigned  int  channel0    : 1,
                       channel1    : 1,
                       channel2    : 1,
                       reserved    : 13;
    }bChl;
    unsigned int Chl;
}AVI_DMA_CHANNEL;

typedef struct {
    unsigned int    ickid[4];
    unsigned int    iFlag[4];
    unsigned int    iChuckOffset[4];
    unsigned int    ichuckLen[4];    
}AVI_INDEX_ENTRY;

typedef struct{
    unsigned int  DmaType;
    unsigned int  DataBus;      //与DMA寄存器中的位设置一至
    
    unsigned long SAddr;
    unsigned long DAddr;
    
    unsigned int  TransferData;
    
    unsigned int  STransferSize;
    unsigned long STransferInterval;
    
    unsigned int  DTransferSize;
    unsigned long DTransferInterval;
}DMA_TRAN_STRUCT;

typedef struct{
    int     fps;        //frame number per second
    long    mspf;       //micro-second per frame
}AVI_FRAME_RATE_SETTING;

typedef struct {
    int x;
    int y;
    int xsize;
    int ysize;
    unsigned long offset;
    int pic_count;
}AVIBITMAP;
//------------------------------------------------------------------------------
#endif
//******************************************************************************
