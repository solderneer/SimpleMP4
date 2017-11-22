/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaDecodeSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*            zyf           05-12-08       1.1           
*            hjl           05-12-10       1.2    ????????NAND FLASH??????????????
********************************************************************************
*/
#define _IN_WMADECODER

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "WmaDecodeMacro.h"
#include "WmaDecodeStruct.h"
#include "resource.h"
#include "Global.h"
#include "WmaDecodeGlobal.h"
#include "RockReadModule.h"

extern unsigned int WMATableRunDram[];
void WmaDecodeOutputEnable(void);
void WmaDecodeOutputDisable(unsigned int bEnd);
void WmaTableDataLoader(unsigned int WMATableNumber);  

ZSP_getpcm      zsp_wma_pcm __attribute__((section(".wma_bss, \"aw\"")));
zspstream       wmastream_data __attribute__((section(".wma_bss, \"aw\"")));

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaPCMOutBufferChange(void)
{    
    WmaPlayFlag.bc.bPcmBufEmpty = 1;
    
    write_mem32(DMAR_SAR2,(((unsigned long)(pWMAPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB + 1) << 1);
    write_mem32(DMAR_CTL2,0x00100123);
    write_mem32(DMAR_CTL2+2,WMAPCMOutBufferLength);
    write_mem32(DMAR_CFG2,0x00000040);
    write_mem32(DMAR_CFG2+2,0x00001004);
    write_mem32(DMAR_DmaCfgReg,0x00000001);
    write_mem32(DMAR_MaskBlock,0x00000404);
    write_mem32(DMAR_ChEnReg,0x00000404);     
    
    AVI_MP3_DECODE_REQ;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaFillHeadBuffer(long fileoffset)
{
    unsigned int i , bytes_read;
    FileSeek(fileoffset,SEEK_SET,hMainFile);
    for(i=0; i < 24; i++){//read the header of the wma file for get file 
        bytes_read = FileRead((unsigned char *)(pWMAInputBuffer + i*512), 512,hMainFile);
        if(bytes_read  < 512){//
            break;
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void fillbuffer(zspstream *stream_data)
{
    short i;
    int   fileinputtemp[512];
    short * bufWroffset = stream_data -> Pbuf_begin + stream_data -> Pbuf_WRoffset;
    short words_read = FileRead((unsigned char*)fileinputtemp,512,hMainFile);
    
    words_read >>= 1;
    fillbuffersub(bufWroffset,fileinputtemp,words_read);
    
    stream_data -> Pbuf_WRoffset += words_read;
    stream_data -> Pbuf_WRoffset &= INBUFFERMASK;   
    stream_data -> validdata += words_read;
    stream_data -> packetoffset += words_read << 1;            
    WMAFileOffSet += words_read << 1;

    /* DRM decryption if necessary */
#ifdef PD_DRM  
    if(g_WmaHas_DRM)// g_pint.bHasDRM || g_pint.bHasJanusDRM )
    {
        unsigned short cbBuffer, hr;	
        unsigned short *pBuffer = (unsigned short*)bufWroffset;	
	
        if (wmastream_data.UndecryptPayloadByte > 0)
        {
            cbBuffer = wmastream_data.UndecryptPayloadByte >= 512 ? 
			   	                                  512 : wmastream_data.UndecryptPayloadByte;
			   
            hr = CDrmPD_Decrypt__ (/*&g_pint.pDRM_state,*/ pBuffer, cbBuffer);
            wmastream_data.UndecryptPayloadByte -=  cbBuffer;	 				  
        }
    }
#endif    
}

/*------------------------------------------------------------------------------
Function:
Description: Get last 15 byte data of a packet, which is used in DRM decrypt 
Input:    
Output:
------------------------------------------------------------------------------*/
#ifdef PD_DRM
__attribute__((section(".wma_text,\"ax\"")))
short GetLast15Byte(unsigned short *pLast15, unsigned long Last15ByteFileOffset)
{
    short readbyte;
    int   fileinputtemp[512];
    
    // seek to the end of the current packet to get the last 15 byte
    FileSeek(Last15ByteFileOffset, SEEK_SET, hMainFile);	   	
    
    readbyte = FileRead((unsigned char*)fileinputtemp, 15, hMainFile);
    if (readbyte != 15)
    {
        FileSeek(WMAFileOffSet, SEEK_SET, hMainFile);
        return WMA_E_BROKEN_FRAME;
    }  
         	
    fillbuffersub(pLast15, fileinputtemp, 8);
    
    // seek back to the WMAFileOffSet for next time getting wma data
    FileSeek(WMAFileOffSet, SEEK_SET, hMainFile);
    
    return 0;
}
#endif


//******************************************************************************
__attribute__((section(".wma_text,\"ax\"")))
void FillWmaDataTable(unsigned long *NFAddress, unsigned int DMAddress, unsigned int DataLen)
{
    unsigned int i, ReadOutDataLength;
    unsigned char ReadOutBuffer[512];
    
    unsigned long SourceAddress = *NFAddress;
    unsigned int  *pMemAddr;
    unsigned long DataLength = (unsigned long)DataLen << 1;
    
    pMemAddr = (unsigned int *)DMAddress;
    while (DataLength)
    {
        if (DataLength >= 512) {
            ReadOutDataLength = 512;
        } else {
            ReadOutDataLength = DataLength;
        }
        
        FlashReadLogic(SourceAddress, ReadOutBuffer, ReadOutDataLength);
        SourceAddress += ReadOutDataLength;
        DataLength -= ReadOutDataLength;
        
        for (i = 0; i < ReadOutDataLength; i += 2) {
            *pMemAddr ++ = (ReadOutBuffer[i] << 8) + ReadOutBuffer[i + 1];
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaDecodeOutputEnable(void)
{
    bitclear_creg(%imask,15);
    cCountGetPCM = 0;
    
    write_mem(I2DSP_DPCTRL,0x0001);         //Tx FIFO Reset
    write_mem(I2DSP_TXCOM, 0x0006);         //Tx enable,I2Dsp enable,DMA request Enable   
    
    write_mem32(DMAR_ClearBlock,0x00000004);
    write_mem32(DMAR_SAR2,(((unsigned long)(pWMAPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB + 1) << 1);
    write_mem32(DMAR_CTL2,0x00100123);
    write_mem32(DMAR_CTL2+2,WMAPCMOutBufferLength);
    write_mem32(DMAR_CFG2,0x00000040);
    write_mem32(DMAR_CFG2+2,0x00001004);
    write_mem32(DMAR_DmaCfgReg,0x00000001);
    write_mem32(DMAR_MaskBlock,0x00000404);
    write_mem32(DMAR_ChEnReg,0x00000404);
    
    SystemInfo.bc.bSysIdleEn = 0;
#if(FLASHROM_TYPE == _MLC_)
    MlcDataRefresh();       // hjl 2006-12-7
#endif
    
    bitset_creg(%imask,15);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaDecodeOutputDisable(unsigned int bEnd)
{
    unsigned int TimeOut;
    
#ifdef LCD_MEMORY_DEV
    while(DmaFillImage2LcdFlag);
#endif

    MusicCodecMute(1);
    
    bitclear_creg(%imask,15);

//------------------------------------------------------------
//wait DMA Transmite End
    TimeOut = 0;
    while((read_mem(DMAR_StatusBlock) & 0x0004) == 0) {
        delay_nops(480);
        if (++TimeOut > 1024) break;
    }
    write_mem32(DMAR_ClearBlock,0x00000004);
    write_mem32(DMAR_ChEnReg,0x00000400);
    
//------------------------------------------------------------
//wait I2DSP Transmite End
    TimeOut = 0;
    while((read_mem(I2DSP_STATUS) & 0x01) == 0) {
        delay_nops(480);
        if (++TimeOut > 1024) break;
    }
    Delay1Ms(4);
    write_mem(I2DSP_TXCOM, 0x0000);
    write_mem(I2DSP_DPCTRL,0x0001);         //Tx FIFO Reset
    
//------------------------------------------------------------
    if (bEnd) {
        AVI_MP3_DECODE_REQ_CLR;
        AVI_MP3_DECODE_DISABLE;
        write_mem32(DMAR_MaskBlock,0x00000400); 
    }
    
    SystemInfo.bc.bSysIdleEn = 1;
    SysIdleCounter = 0;
    
//------------------------------------------------------------
    bitset_creg(%imask,15);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaDisplayModeInit(void)
{
    int i;
    
    //ScrollStringDispDisable();
    
    if (Fun2MsgArray[0] == MSG_FUNCTION2_LRC) {
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
    }
    
    MusicDispFlag &= ((~MUSIC_DISPFLAG_PATH) & (~MUSIC_DISPFLAG_FILENAME) & (~MUSIC_DISPFLAG_SPECTRUM) & (~MUSIC_DISPFLAG_ID3) & (~MUSIC_DISPFLAG_LRC));
    
    if (MusicPlayInfo.DispState == MUSIC_DISPLAY) {
        MusicDispFlag |= (MUSIC_DISPFLAG_PATH | MUSIC_DISPFLAG_FILENAME | MUSIC_DISPFLAG_SPECTRUM);
        
        AVI_MP3_DECODE_DISABLE;
        for(i = 21; i >= 0; i--) *(WMAStroboscopeArray + i) = 0;
        AVI_MP3_DECODE_ENABLE;
        
    } else {
        if (WmaPlayFlag.bc.bWMALrcHave == 1) {
            memcpy(SysFile2Info.Path , SysFile1Info.Path ,(3 + MAX_DIR_DEPTH*12 + 1) );
            memcpy(SysFile2Info.Fdt.Name , /*SysFile1Info.Fdt.Name*/WmaLrcFileName ,8);
            SysFile2Info.Fdt.Name [8] = 'L';
            SysFile2Info.Fdt.Name [9] ='R';
            SysFile2Info.Fdt.Name [10] ='C';
            PostFun2Message(MSG_FUNCTION2_LRC,MSG_FUNCTION2_LRC_INIT,0,0,0/*,0,0,0*/); 
        } else {
            MusicPlayInfo.DispState = MusicPlayInfo.DispMode;
            MusicDispFlag |= MUSIC_DISPFLAG_SPECTRUM;
        }
        
        if (WmaPlayFlag.bc.bWMAId3Have == 1) {
            MusicDispFlag |= MUSIC_DISPFLAG_ID3;
        } else {
            MusicDispFlag |= (MUSIC_DISPFLAG_PATH | MUSIC_DISPFLAG_FILENAME);
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
extern  unsigned char FileOpenErrString[]   ;//__attribute__((section(".wma_data,\"aw\"")))= "FILE OPEN ERROR!";
extern  unsigned char FileSizeErrString[]   ;//__attribute__((section(".wma_data,\"aw\"")))= "FILE SIZE ERROR!";
extern  unsigned char FileFormatErrString[] ;//    __attribute__((section(".wma_data,\"aw\"")))= "FILE FORMAT ERROE!";
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeHead(void)
{
    int  wmaResult=0;
    unsigned int words_read;
    unsigned int i,j;
    unsigned int temp1,temp2;
    
    int          FindFileResult;
    unsigned int FileNameLen;
    SYS_FILE_INFO SysFileInfoTemp;
    
{//-------------------------Wma Variable Init----------------------------------
    TotalPlayTime = 0;
#define WMATABLERUN WMATableRunDram
    WMAHuffmanTableBase16OB = WMATABLERUN;
    WMAHuffmanTableBase44OB = WMATABLERUN;
    WMAHuffmanTableBase44QB = WMATABLERUN; 
        
    MusicPlayInfo.Bitrate = 128;
    MusicPlayInfo.Freq = 44100;
    WmaPlayStatus = MusicPlayInfo.PlayState;
    
    cbLastTime = 0;
    cbTotalTime = 0;
    WmaPlayFlag.word = 0;
    
    MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
    MusicPlayInfo.DispState = MusicPlayInfo.DispMode;
    
    if(MusicDisplayInterface == 1) {
        MusicDisplayInterface = 0;
        MusicDispFlag = (MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |
                         MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | MUSIC_DISPFLAG_EQ | MUSIC_DISPFLAG_AB |
                         MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | MUSIC_DISPFLAG_VOL | 
                         MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS | MUSIC_DISPFLAG_ORDER);
    } else {
        MusicDispFlag = (/*MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |*/
                         MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | /*MUSIC_DISPFLAG_EQ |*/MUSIC_DISPFLAG_AB |
                         MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | /*MUSIC_DISPFLAG_VOL |*/
                         MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS/* | MUSIC_DISPFLAG_ORDER*/);
    }
}

{//----------------------------Check ID3 Have----------------------------------
    if ((hMainFile = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R")) == NOT_OPEN_FILE) {
        WmaFileFormatError = 1;
        PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_NONE,0,0,0/*,0,0,0*/);
        return;
    }
    
    for(i=0; i < 24; i++){                                                              //read the header of the wma file for get file 
        words_read = FileRead((unsigned char *)(pWMAInputBuffer + i*512), 512,hMainFile);
        if(words_read  < 512){                                  //
            break;
        }
    }

#if 0
    WmaId3Info.id3_check = 0;
#else
    ZSP_memset(&WmaId3Info,0,sizeof(WMA_ID3_TAG));//rock@2006-10-26解决WMA ID3显示错误的问题
#endif    
    if((0 != ASF_TagParser(pWMAInputBuffer,&WmaId3Info)) || (words_read  < 512)){
        FileClose(hMainFile);
        WmaFileFormatError = 1;
        PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_NONE,0,0,0/*,0,0,0*/);
        return; 
    }
    
    if(WmaId3Info.id3_check == 1) {
        WmaPlayFlag.bc.bWMAId3Have = 1;
    }
}

{//-----------------------Check WMA File Head---------------------------
    if(WMAInit() != 0){
        FileClose(hMainFile);
        WmaFileFormatError = 1;
        PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_NONE,0,0,0/*,0,0,0*/);
        return; 
    }
    
    GetLongFileName(SysFile1Info.LongFileName);
    GetDirLongPath(SysFile1Info.LongPath);

#if(1)
        if(ghdr_parse.m_iEntropyMode == FOURTYFOURS_QB)//0x12ae 
        {
            WmaTableDataLoader(WMA44QB_BSS_MODULE_NUM);
        }
        else if(ghdr_parse.m_iEntropyMode == FOURTYFOURS_OB)//0x25cd
        {
            WmaTableDataLoader(WMA440B_CODE_MODULE_NUM);
        }
        else  if(ghdr_parse.m_iEntropyMode == SIXTEENS_OB)//0x0dce
        {
            WmaTableDataLoader(WMA160B_DATA_MODULE_NUM);
        }	
#endif
}

{//-------------------------Check Lrc File Have--------------------------------
    
    SysFileInfoTemp.Path[0] = '.';
    SysFileInfoTemp.Path[1] = 0; 
    SysFileInfoTemp.TotalFiles = GetTotalFiles(SysFile1Info.Path,"LRC");
    WmaLrcFileName[0] = 0 ;
    for(i = 0 ; i < MAX_FILENAME_LEN; i++) {
        if(SysFile1Info.LongFileName[i] == 0) break;
    }
    FileNameLen = i - 4;
    
    for (SysFileInfoTemp.CurrentFileNum = 1; SysFileInfoTemp.CurrentFileNum <= SysFileInfoTemp.TotalFiles; SysFileInfoTemp.CurrentFileNum++) {
        FindFileResult = FindFile(&SysFileInfoTemp.Fdt,SysFileInfoTemp.CurrentFileNum,SysFile1Info.Path/*SysFileInfoTemp.Path*/,"LRC");
        
        if(FindFileResult != RETURN_OK) break;
        GetLongFileName(SysFileInfoTemp.LongFileName);
        
        for (i = 0; i < FileNameLen; i++) {
            if (SysFileInfoTemp.LongFileName[i] != SysFile1Info.LongFileName[i]) {
                break;
            }
        }
        if( i == FileNameLen ) {
            memcpy(WmaLrcFileName , SysFileInfoTemp.Fdt.Name ,8);   //把歌词文件名临时存放在LrcFileName中.           
            if (Fun2MsgArray[0] == MSG_FUNCTION2_NONE) {
                WmaPlayFlag.bc.bWMALrcHave = 1;
                MusicPlayInfo.DispState = LRC_DISPLAY;
            }
            break;;
        }
    }
}

{//-------------------------Wma Variable Init----------------------------------
    WMAPCMOutBufferLength= ghdr_parse.m_cFrameSampleHalf;
    zsp_wma_pcm.buff_have_pcm_num = 0;
    MusicPlayInfo.FirstBitrate = (ghdr_parse.nAvgBytesPerSec+62)/125;//add 0.5kbps
    
    cbTotalTime = TotalPlayTime;
    if(TotalPlayTime == 0)  cbTotalTime = (SysFile1Info.Fdt.FileSize-ghdr_parse.cbHeader)/ghdr_parse.nAvgBytesPerSec;
    gWMAnAvgBytesPerSec = (SysFile1Info.Fdt.FileSize-ghdr_parse.cbHeader)/(cbTotalTime+1);
        
    MusicPlayInfo.TimeGuageVal = 0;
    MusicPlayInfo.Freq=ghdr_parse.nSamplesPerSec;
    MusicPlayInfo.Bitrate= MusicPlayInfo.FirstBitrate;
//------------------------------------------------------------------
//initialize wmastream_data
    wmastream_data.use_bits=0;
    wmastream_data.pbuf_end = pWMAInputBuffer + INBUFFERSIZE;//&WMAInputBuffer[INBUFFERSIZE];
    wmastream_data.Pbuf_begin = pWMAInputBuffer;
    wmastream_data.Pbuf_WRoffset=0;   
    wmastream_data.offset_words=0;
    wmastream_data.validdata=0;
#ifdef PD_DRM
        wmastream_data.UndecryptPayloadByte = 0; //drm
#endif
//------------------------------------------------------------------
    WMAFileOffSet = ghdr_parse.cbHeader;
    FileSeek(WMAFileOffSet,SEEK_SET,hMainFile);
    
    WMAFileOffSetA = 0;
    WMAFileOffSetB = 0;
    
    WmaDisplayModeInit();
    
    PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_START,0,0,0/*,0,0,0*/);
}
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeStart(void)
{
    int i;
    
    for(i = OUTBUFFERSIZE -1; i >= 0; i --)
        *(FirstWMAPCMOutAddress + i) = 0;
    
    WmaPlayFlag.bc.bBufferIndex = 0;
    WmaPlayFlag.bc.bPcmBufEmpty = 1;
    
    WmaSamplingFreqSet(MusicPlayInfo.Freq);
    WmaCpuFreqSet(MusicPlayInfo.Freq);
    WmaDecodeEqSet();
    
    WmaPlayFlag.bc.bWMADecodeEnable = 0;
    if(MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
        WmaDecodeOutputEnable();
        WmaPlayFlag.bc.bWMADecodeEnable = 1;
    } else {
        if ((Sys2InitFlag.bc.bGameModuleInitEnable != 0) || (CurrCpuFreq != 51)) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bSysIdleEn = 1;
            SysIdleCounter = 0;
            bitset_creg(%imask,5);
        }
    }
    PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_PLAY,0,0,0/*,0,0,0*/);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeEnd(void)
{
    unsigned int i,j;
    
    WmaPlayFlag.bc.bWMADecodeEnable = 0;
	
	if(WmaPlayFlag.bc.bPcmBufEmpty)
    {
        WmaPlayFlag.bc.bPcmBufEmpty = 0;
        
        WmaDecodeOutputDisable(1);
        
        if (MusicPlayInfo.Mode == MUSIC_DECODE_ONE) {
        //返回到资源管理器，选择当前歌曲
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            WmaToBrowser(TO_CURRENT_FILE);
            return;
        }
        
        if ((MusicPlayInfo.RepMode == MUSIC_REPMODE_NORMAL) && (SysFile1Info.PlayedFileNum >= SysFile1Info.TotalFiles)) {
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            
            if (MusicPlayInfo.Range == MSG_FIND_FILE_ALL) {
            //返回到资源管理器，选择第一首歌曲
                WmaToBrowser(TO_DEV_FIRST_FILE);
            } else {
            //返回到资源管理器，选择当前歌曲
                WmaToBrowser(TO_CURRENT_FILE);
            }
            return;
        }
        
        FileClose(hMainFile);
        if (Sys2InitFlag.bc.bLrcModuleInitEnable == 0) {
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
            Fun2ModuleChange();
        }
        //ScrollStringDispDisable();
        PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_MUSIC_FILE,MSG_FIND_FILE_NEXT,MusicPlayInfo.Range,MusicPlayInfo.Order/*,0,0,0*/);
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
int WMADecodePlay(void)
{
    int wmaResult = 0;
    unsigned int i;
    unsigned long temp;
    
    if((MusicPlayInfo.AB == MUSIC_DECODE_AB) && ((WMAFileOffSet-wmastream_data.validdata) >= WMAFileOffSetB))
    {   /*AB End Position Check!*/
        WMAFileOffSet = WMAFileOffSetA;
        WmaPlayFlag.bc.bWMADecodeInit=1;
        
        if (cCountGetPCM >= 6) {
            MusicCodecMute(1);
        }
        cCountGetPCM = 0;
    }
    
    if( WmaPlayFlag.bc.bWMADecodeInit == 1)
    {
        WmaPlayFlag.bc.bWMADecodeInit=0;
        
        wmaResult=WMA_Decoder_Reset();
        wmastream_data.use_bits=0;
        wmastream_data.Pbuf_WRoffset=0; 
        wmastream_data.offset_words=0;
        wmastream_data.validdata=0;
#ifdef PD_DRM
        wmastream_data.UndecryptPayloadByte = 0; //drm
#endif
        temp=(WMAFileOffSet-ghdr_parse.cbHeader)/ghdr_parse.cbPacketSize;
        WMAFileOffSet=(unsigned long )ghdr_parse.cbPacketSize*temp+ghdr_parse.cbHeader;
        ghdr_parse.currPacketOffset=WMAFileOffSet;
        ghdr_parse.nextPacketOffset=WMAFileOffSet;

        FileSeek(WMAFileOffSet, SEEK_SET,hMainFile);

        zsp_wma_pcm.con_left_pcm_num=0;
        zsp_wma_pcm.buff_have_pcm_num=0;
    }    

    if ((WmaPlayFlag.bc.bWMADecodeEnable)/*&&(WmaPlayFlag.bc.bPcmBufEmpty == 1)*/) {
        WmaPlayFlag.bc.bBufferIndex = ~WmaPlayFlag.bc.bBufferIndex;
        pWMAPCMOutputBuf/*pWMADecodeOutputBuf*/ = FirstWMAPCMOutAddress + WmaPlayFlag.bc.bBufferIndex * WMA_PCM_BUFFER_SIZE;
        WmaPlayFlag.bc.bPcmBufEmpty = 0;
        
        int k=(INBUFFERSIZE-wmastream_data.validdata-50) >> 8;
        while(k > 0){
            fillbuffer(&wmastream_data);
            k--;
        }
        if(wmastream_data.validdata <= 0) {
            PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_END,0,0,0/*,0,0,0*/);
            return;
        }
        
        zsp_wma_pcm.buff_have_pcm_num = 0;
        while(1){
            if(zsp_wma_pcm.con_left_pcm_num > 0) {
                wmaResult = WMAGetPCM(pWMAPCMOutputBuf + 2*zsp_wma_pcm.buff_have_pcm_num,                                         
                   min( /*ghdr_parse.m_cFrameSampleHalf*/WMAPCMOutBufferLength
                   - zsp_wma_pcm.buff_have_pcm_num,zsp_wma_pcm.con_left_pcm_num));
            }
            if(zsp_wma_pcm.buff_have_pcm_num >= WMAPCMOutBufferLength){
                cCountGetPCM ++;
                if(MusicPlayInfo.Eq == MUSIC_DECODE_EQ_3D) 
                {
                    WMADsurround(pWMAPCMOutputBuf, WMAPCMOutBufferLength ) ;//
                }
                else if (MusicPlayInfo.Eq != MUSIC_DECODE_EQ_NORMAL)
                {
                    Equ_One_Frame(pWMAPCMOutputBuf,WMAPCMOutBufferLength);    
                } 
                
                if(cCountGetPCM == 6) {
                    MusicCodecMute(0);
                }
                if((DisplayFlag == DISP_MUSIC) && (SystemInfo.bc.bBackLightStatus != 2) && (MusicPlayInfo.DispState != LRC_DISPLAY) && (cCountGetPCM >= 10)){
                    cCountGetPCM = 8;
                    wma_specline10(pWMAPCMOutputBuf,WMAStroboscopeArray);
                    MusicDispFlag |= MUSIC_DISPFLAG_SPECTRUM;
                }
                break;
            } else {
                wmaResult = WMAFileDecodeData(&zsp_wma_pcm.con_left_pcm_num);
                
                if (wmaResult != cWMA_NoErr) {
                    for(i = 0;i < (WMAPCMOutBufferLength << 1); i++) {
					    pWMAPCMOutputBuf[i]=0;
					}   
                    switch (wmaResult) {
                        
                    case WMA_E_BROKEN_FRAME:
                        WMAFileOffSet = ghdr_parse.nextPacketOffset;//decoder next frame
                        WmaPlayFlag.bc.bWMADecodeInit=1;
                        break;
                        
                    case WMA_S_LOSTPACKET:
                        WMAFileOffSet= ghdr_parse.currPacketOffset;//decoder curr frame
			            WmaPlayFlag.bc.bWMADecodeInit=1;
                        break;
                        
                    case cWMA_NoMoreFrames:
                    case cWMA_Failed:
                    default:
                        PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_END,0,0,0/*,0,0,0*/);
                        break;
                    }
                    break;
                }
            }
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WMADecodeTimeUpdate(void)
{
    unsigned int i,j;
    
    i = (WMAFileOffSet - (wmastream_data.validdata << 1) - ghdr_parse.cbHeader) / gWMAnAvgBytesPerSec;
    if((cbLastTime != i) && (i <= cbTotalTime)) {
        cbLastTime = i;
        MusicDispFlag |= (MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_FREE_TIME);
    }
    
    j = (cbLastTime * (TIME_GUAGE_CONST - 1))/(cbTotalTime);
    if(j!=MusicPlayInfo.TimeGuageVal){
        MusicPlayInfo.TimeGuageVal = j;
        MusicDispFlag |= MUSIC_DISPFLAG_GUAGE;
    }
    
    if((MusicPlayInfo.Mode == MUSIC_DECODE_BROWSE) && (cbLastTime >= 10))
            PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_END,0,0,0/*,0,0,0*/);
}

/*
*******************************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : WmaTableDataLoader()
*  Author:         Jiliu Huang
*  Description:    调度WMA解码表格
*  Input:          CodeModeNum:具体表格的模块号，该模块号在resource.h中定义
*  Output:         
*  Return:         模块高度结果  0:正常调度;   -1:模块不存在;  1:没有找到“id”Block
*  Others: 
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/01      1.0           ORG
*           Jiliu Huang  06/02/09      1.1           修改了地址信息的获取方式
*******************************************************************************************
*/
#if 0
__attribute__((section(".wma_text,\"ax\"")))
void WmaDataLoader(unsigned long NFIndexAddress)
{
    unsigned int    DestAddr;
    unsigned int    DataLen;
    unsigned long   SourceAddr;
    unsigned char   Module_NF_Info[10];
    unsigned int    i;
    
    SourceAddr = NFIndexAddress;
    FlashReadLogic(SourceAddr, Module_NF_Info, 10/*WMA_TABLE_NF_LEN*/);
    /*DestAddr    = (((unsigned int)Module_NF_Info[0]) << 8) | (((unsigned int)Module_NF_Info[1]));*/
    DestAddr    = WMATableRunDram;
    DataLen     = (((unsigned int)Module_NF_Info[2]) << 8) | (((unsigned int)Module_NF_Info[3]));
    SourceAddr  = (((unsigned long)Module_NF_Info[4])<<24)
                 |(((unsigned long)Module_NF_Info[5])<<16)
                 |(((unsigned long)Module_NF_Info[6])<<8)
                 |(((unsigned long)Module_NF_Info[7])); 
    
    FillWmaDataTable(&SourceAddr,DestAddr,DataLen);                       
}
#else
__attribute__((section(".wma_text,\"ax\"")))
void WmaTableDataLoader(unsigned int WMATableNumber)
{
    unsigned int    DestAddr;
    unsigned int    DataLen;
    unsigned long   SourceAddr;
    unsigned char   Module_NF_Info[8];
    unsigned int    i;       
    long            LogicStartAddressID;
    unsigned long   *pAddress;
    
    pAddress = &SourceAddr;
    
    
    LogicStartAddressID = FindIDBlock();
    if(LogicStartAddressID == -1){
        return 1;           
    }        
    
    SourceAddr = (unsigned long)LogicStartAddressID + 512 + 4 + 8 * WMATableNumber;
    FlashReadLogic(SourceAddr, Module_NF_Info, 8);
  
    DestAddr    = WMATableRunDram;
    DataLen     = (((unsigned int)Module_NF_Info[7]) << 8) | (((unsigned int)Module_NF_Info[6]));
    SourceAddr  = (((unsigned long)Module_NF_Info[3])<<24)
                 |(((unsigned long)Module_NF_Info[2])<<16)
                 |(((unsigned long)Module_NF_Info[1])<<8)
                 |(((unsigned long)Module_NF_Info[0])); 
    SourceAddr <<= 9;       // Sector * 512 byte/sector
    
    FillWmaDataTable(&SourceAddr, DestAddr, DataLen);                        
}
#endif
