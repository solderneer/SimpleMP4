/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3_Decode.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#define _IN_MP3_DECODE

#include <Creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Global.h"
#include "Resource.h"
#include "MP3DecodeGlobal.h"
#include "MP3DecodeStruct.h"
#include "Mp3ApiStruct.h"

#include "Config.h"
#include "Audio.h"
#include "Message.h"
#include "Byte2Word.h"
#include "MP3DecodeAsm.h"
#include "Subspecanal.h"

extern void MP3_Init_MP3(MP3_DECODER_STRUCT *MP3DecodeStruct);
extern void MP3_FrameDecode(unsigned int *pBuffer, frame_params *FrameParams, MP3_DECODER_STRUCT *MP3DecodeStruct);

void ClearPcmData(void);
void ClearPcmData(void);
void MP3DecodeOutputEnable(void);
void MP3DecodeOutputDisable(unsigned int bEnd);

MP3_DECODER_STRUCT  MP3Obj __attribute__((section(".mp3_bss, \"aw\"")));
frame_params        MP3FrameParams __attribute__((section(".mp3_bss, \"aw\"")));
al_table            alloc_table __attribute__((section(".mp3_bss, \"aw\"")));
//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void PCMOutBufferChange(void)
{
    
    Mp3PlayFlag.bc.bPcmBufEmpty = 1;

    write_mem32(DMAR_SAR2,(((unsigned long)(pPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB) << 1);

    write_mem32(DMAR_CTL2,0x00100125);
    write_mem32(DMAR_CTL2+2,PCMOutBufferLength<<1);
    write_mem32(DMAR_CFG2,0x00000040);
    write_mem32(DMAR_CFG2+2,0x00001004);
    write_mem32(DMAR_DmaCfgReg,0x00000001);
    write_mem32(DMAR_MaskBlock,0x00000404);
    write_mem32(DMAR_ChEnReg,0x00000404); 
    
    BitrateCount ++ ;
    StroboscopeCount ++;
    
    AVI_MP3_DECODE_REQ;
}   
    
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void ClearPcmData(void)
{
    int *pData;
    int i;
    
    PCMOutBufferLength = 576;
        
    if(MP3FrameParams.header.lay == 1)  PCMOutBufferLength = 384;
    else if((MP3FrameParams.header.version != MPEG_PHASE2_LSF) || (MP3FrameParams.header.lay !=3))
         {PCMOutBufferLength = 1152;}
    
    pPCMOutputBuf = pMP3DecodeOutputBuf;
    pData = pMP3DecodeOutputBuf;
    
    for(i = (PCMOutBufferLength << 1); i > 0 ; i--)
        *pData++ = 0;//i*56+1;
        
    Mp3PlayFlag.bc.bPcmBufFull = 1;
}
/*------------------------------------------------------------------------------
Function:
        :
        :
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void FillPcmData(void)
{
    unsigned int l = 576;
    
    PCMOutBufferLength = 576;
        
    if(MP3FrameParams.header.lay == 1){ l = 384 ; PCMOutBufferLength = 384; }
    else if((MP3FrameParams.header.version != MPEG_PHASE2_LSF) || (MP3FrameParams.header.lay !=3))
         {PCMOutBufferLength = 1152;}
    
    pPCMOutputBuf = pMP3DecodeOutputBuf;
    
    if(MP3FrameParams.stereo == 2)
    {
        PCMBufferDataChangeStereo(pMP3DecodeOutputBuf,pPCMOutputBuf,l);
        
        if(MP3FrameParams.header.lay != 1)
        {
            if((MP3FrameParams.header.version != MPEG_PHASE2_LSF) || (MP3FrameParams.header.lay !=3))
                 PCMBufferDataChangeStereo(pMP3DecodeOutputBuf+2304,pPCMOutputBuf+2304,l);
        }
    }
    else
    {
        PCMBufferDataChangeMono(pMP3DecodeOutputBuf,pPCMOutputBuf+l*2,l);
        
        if(MP3FrameParams.header.lay != 1)
        {
            if((MP3FrameParams.header.version != MPEG_PHASE2_LSF) || (MP3FrameParams.header.lay !=3))
                PCMBufferDataChangeMono(pMP3DecodeOutputBuf+2304,pPCMOutputBuf+2304+l*2,l);
        }
    }
    
    Mp3PlayFlag.bc.bPcmBufFull = 1;
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void MP3DecodeOutputEnable(void)
{
    bitclear_creg(%imask,15);
    MP3DecodeBadFrameCount = 0;
    
    write_mem(I2DSP_DPCTRL,0x0001);         //Tx FIFO Reset  
    write_mem(I2DSP_TXCOM, 0x0006);         //Tx enable,I2Dsp enable,DMA request Enable   

    write_mem32(DMAR_ClearBlock,0x00000004);
    write_mem32(DMAR_SAR2,(((unsigned long)(pPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB) << 1);
    write_mem32(DMAR_CTL2,0x00100125);
    write_mem32(DMAR_CTL2+2,PCMOutBufferLength<<1);
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
Function: ????IIS??MP3??????????
Input   : ??
Output  : ??
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void MP3DecodeOutputDisable(unsigned int bEnd)
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
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void MP3DecodeStart(void)
{
    int i;
    
    PCMOutBufferLength = 1152;
    for(i = APPLICATION_INPUT_BUF_LENGTH - 1; i >= 0; i --)
        *(ApplicationInputBufferStart + i) = 0;
        
    for(i = (PCM_OUT_BUF_LENGTH << 1) - 1; i >= 0; i --)
        *(FirstPCMOutAddress + i) = 0;
    
    Mp3PlayFlag.bc.bPcmBufFull = 0;
    Mp3PlayFlag.bc.bPcmBufEmpty = 1;
    
    Mp3SamplingFreqSet(MusicPlayInfo.Freq);
    Mp3CpuFreqSet(MusicPlayInfo.Freq);
    DecodeEqSet();
    
    Mp3PlayFlag.bc.bMP3DecodeEnable = 0;
    if(MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
        MP3DecodeOutputEnable();
        Mp3PlayFlag.bc.bMP3DecodeEnable = 1;
    } else {
        if ((Sys2InitFlag.bc.bGameModuleInitEnable != 0) || (CurrCpuFreq != 51)) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bSysIdleEn = 1;
            SysIdleCounter = 0;
            bitset_creg(%imask,5);
        }
    }
    
    Mp3PlayFlag.bc.bMP3DecodeInit = 1;
    PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_PLAY,0,0,0/*,0,0,0*/);
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3DecodeEnd(void)
{
    Mp3PlayFlag.bc.bMP3DecodeEnable = 0;
    
    if(Mp3PlayFlag.bc.bPcmBufEmpty)
    {
        Mp3PlayFlag.bc.bPcmBufEmpty = 0;
        
        MP3DecodeOutputDisable(1);
        
        if (MusicPlayInfo.Mode == MUSIC_DECODE_ONE) {
        //返回到资源管理器，选择当前歌曲
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            Mp3ToBrowser(TO_CURRENT_FILE);
            return;
        }
        
        if ((MusicPlayInfo.RepMode == MUSIC_REPMODE_NORMAL) && (SysFile1Info.PlayedFileNum >= SysFile1Info.TotalFiles)) {
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            
            if (MusicPlayInfo.Range == MSG_FIND_FILE_ALL) {
            //返回到资源管理器，选择第一首歌曲
                Mp3ToBrowser(TO_DEV_FIRST_FILE);
            } else {
            //返回到资源管理器，选择当前歌曲
                Mp3ToBrowser(TO_CURRENT_FILE);
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
Function: MP3????????????
Input   : ??
Output  : ??
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void MP3DecodeErrorProc(void)
{
    ClearPcmData();
    
    MP3DecodeErrorTimes ++;
    
    if(MP3DecodeBadFrameCount < 8) {
        MP3DecodeBadFrameCount = 0;
    }
}

__attribute__((section(".mp3_text,\"ax\"")))
void MP3DecodePlay(void)
{
    unsigned long MP3DecodeRerurnVal;
    unsigned int i,j;
    
    if((MusicPlayInfo.AB == MUSIC_DECODE_AB) && (MP3FileOffSet >= MP3FileOffSetB))
    {   /*AB End Position Check!*/
        MP3FileOffSet = MP3FileOffSetA;
        FileSeek(MP3FileOffSet, SEEK_SET, hMainFile);
        Mp3PlayFlag.bc.bMP3DecodeInit = 1;
        
        if (MP3DecodeBadFrameCount >= 8) {
            MusicCodecMute(1);
        }
        MP3DecodeBadFrameCount = 0;
    }
    
    if(Mp3PlayFlag.bc.bMP3DecodeInit)
    {
        Mp3PlayFlag.bc.bMP3DecodeInit = 0;
        
        MP3Obj.head_pkptr = (short *)(ApplicationInputBufferStart);
        MP3FrameParams.alloc=&alloc_table;
        MP3Obj.head_pack_state = 0;
        MP3Obj.head_totbit = 0;
        MP3DecodeInputBufWordsUsed = 0;
        MP3DecodeInputBufWordsRemaining = 0;
        MP3Obj.main_init=1;
        Mp3PlayFlag.bc.bMP3DecodeError = 0;
        MP3_Init_MP3(&MP3Obj);
    
        MusicPlayInfo.TimeGuageVal = (unsigned int)(MP3FileOffSet* TIME_GUAGE_CONST / SysFile1Info.Fdt.FileSize);
        
        MP3TimeGuageCount = (unsigned long)(MP3FileOffSet % (SysFile1Info.Fdt.FileSize / TIME_GUAGE_CONST));
        MusicDispFlag |= MUSIC_DISPFLAG_GUAGE;
        
        MP3TimeCount = (unsigned long)(MP3FileOffSet % (((unsigned long)(MusicPlayInfo.FirstBitrate) *125)/20));
        //GetMp3CurTime();
        Mp3CurrTimeSec = (unsigned long)(MP3FileOffSet / ((unsigned long)(MusicPlayInfo.FirstBitrate) *125));
    }
    //----------------------------------------------------------------------------------------------
    //MP3 Decode Main Loop 20051020
    if(Mp3PlayFlag.bc.bMP3DecodeEnable)
    {
        Mp3PlayFlag.bc.bBufferIndex = ~Mp3PlayFlag.bc.bBufferIndex;
        pMP3DecodeOutputBuf = FirstPCMOutAddress + Mp3PlayFlag.bc.bBufferIndex * PCM_OUT_BUF_LENGTH;
        
        MP3DecodeInputBufWordsUsed = MP3Obj.head_pkptr - (short *)(ApplicationInputBufferStart);
        if(MP3DecodeInputBufWordsUsed>MP3DecodeInputBufWordsRemaining) Mp3PlayFlag.bc.bMP3DecodeError = 1; //2005-10-9 fsh
        if(Mp3PlayFlag.bc.bMP3DecodeError == 1) MP3DecodeInputBufWordsUsed = 40;
        MP3DecodeInputBufWordsRemaining = MP3DecodeInputBufWordsRemaining - MP3DecodeInputBufWordsUsed;
        //----------------------------------------------------------------------
        for(i=0; i<MP3DecodeInputBufWordsRemaining; i++)
            ApplicationInputBufferStart[i] = ApplicationInputBufferStart[i + MP3DecodeInputBufWordsUsed];
            
        for( ; i < APPLICATION_INPUT_BUF_LENGTH; i++ )
            ApplicationInputBufferStart[i] = 0;
        //----------------------------------------------------------------------
        
        while(MP3DecodeInputBufWordsRemaining < MP3_DECODE_MAX_FRAME_LENGTH)/*MP3_DECODE_MAX_FRAME_LENGTH = 868*/
        {
            MP3DecodeRerurnVal = FileRead((unsigned char*)MP3DecodeTempReadoutBuf,512,hMainFile);
            MP3FileOffSet += MP3DecodeRerurnVal;
            MP3TimeCount += MP3DecodeRerurnVal;
            MP3TimeGuageCount += MP3DecodeRerurnVal;

            Byte2Word(ApplicationInputBufferStart+MP3DecodeInputBufWordsRemaining, MP3DecodeTempReadoutBuf, MP3DecodeRerurnVal >> 1);
            
            MP3DecodeInputBufWordsRemaining += (MP3DecodeRerurnVal >> 1); 
            
            if(MP3DecodeRerurnVal < 512)
            {   /*File End!*/
                PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_END,0,0,0/*,0,0,0*/);
                MusicPlayInfo.TimeGuageVal ++;
                MusicDispFlag |= MUSIC_DISPFLAG_GUAGE;
                Mp3PlayFlag.bc.bMP3DecodeEnable = 0;
                break;
            }
        }
        
        MP3Obj.head_pkptr   = (short *)(ApplicationInputBufferStart);
        MP3Obj.head_totbit -= (MP3DecodeInputBufWordsUsed << 4);
        
        if(Mp3PlayFlag.bc.bMP3DecodeError == 1) 
        {
            Mp3PlayFlag.bc.bMP3DecodeError = 0;
            
            MP3Obj.head_pack_state = 0;
            MP3Obj.head_totbit = 0;
            MP3_Init_MP3(&MP3Obj);
        }
        if(MP3DecodeErrorTimes >= 100)
        {
            MP3DecodeErrorTimes =0;
            PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_END,0,0,0/*,0,0,0*/);
            return;            
        }   
        
        MP3_FrameDecode(pMP3DecodeOutputBuf, &MP3FrameParams, &MP3Obj);
        switch(MP3FrameParams.MP3_ErrCode)
        {       
        case 0: 
            if(MP3DecodeBadFrameCount < 8)
            {
                if(++MP3DecodeBadFrameCount == 8)
                {
                    MusicCodecMute(0);
                }
            }
            FillPcmData();
            MP3DecodeErrorTimes = 0;
            if(MusicPlayInfo.Eq == MUSIC_DECODE_EQ_3D) 
            {   
                Mp3Dsurround(pPCMOutputBuf, PCMOutBufferLength ) ;//
            }

            if ((DisplayFlag == DISP_MUSIC) && (SystemInfo.bc.bBackLightStatus != 2) && (MusicPlayInfo.DispState != LRC_DISPLAY) && (StroboscopeCount >= 2))
            {   //100ms
                StroboscopeCount = 0;
                specline10((long *)pPCMOutputBuf,(short*)StroboscopeArray);
                MusicDispFlag |= MUSIC_DISPFLAG_SPECTRUM;
            }
            
            MP3DecodeRerurnVal = MP3_Decode_Freq_Tbl[MP3FrameParams.header.version][MP3FrameParams.header.sampling_frequency]; 
            
            if(MP3FrameParams.mpeg2_5==2) MP3DecodeRerurnVal = MP3DecodeRerurnVal >> 1; 
                
            if( MusicPlayInfo.Freq != MP3DecodeRerurnVal)
            {   /*Sampling Frequence*/
                MusicPlayInfo.Freq = MP3DecodeRerurnVal;
                Mp3SamplingFreqSet(MusicPlayInfo.Freq);
                Mp3CpuFreqSet(MusicPlayInfo.Freq);
            }
            
            if((MusicPlayInfo.Bitrate != MP3FrameParams.Bitrate ))
            {   /* Bitrate */
                MusicPlayInfo.Bitrate = MP3FrameParams.Bitrate;
                if(BitrateCount >= 8)
                {   //400ms
                    BitrateCount = 0;
                    MusicDispFlag |= MUSIC_DISPFLAG_BITRATE;
                }
            }
            break;
            
        case 1:
            MP3DecodeErrorProc();
            break;
        case 2:
            MP3Obj.main_init=1;
            MP3DecodeErrorProc();
            break;
            
      /*case 3:
        case 4:
        case 5:
        case 6:*/
        default:
            Mp3PlayFlag.bc.bMP3DecodeInit = 1;
            Mp3PlayFlag.bc.bMP3DecodeError = 1;
            MP3DecodeErrorProc();
            break;
        } 
    }
}
/*------------------------------------------------------------------------------
Function: MP3????????????
Input   : ??
Output  : ??
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
#if 0
void MP3DecodeTimeUpdate(void)
{
    unsigned long TempL;
    //----------------------------------------------------------------------------------------------
    //Time Update 20051020
    TempL = SysFile1Info.Fdt.FileSize / TIME_GUAGE_CONST;
    if(MP3TimeGuageCount >= TempL)
    {   /*Time Guage*/
        MP3TimeGuageCount -= TempL;
        MusicPlayInfo.TimeGuageVal ++;
        MusicDispFlag |= MUSIC_DISPFLAG_GUAGE;
    }
    
    TempL = (unsigned long)((MusicPlayInfo.FirstBitrate * 125)/20);
    if(MP3TimeCount >= TempL)
    {   //50ms
        MP3TimeCount -= TempL;
        BitrateCount ++ ;
        StroboscopeCount ++;
        if((++DecodeCount) >= 20) {
            DecodeCount = 0;
            Mp3CurrTimeSec ++;
            MusicDispFlag |= (MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_FREE_TIME);
        }
    }
    
    if((MusicPlayInfo.Mode == MUSIC_DECODE_BROWSE) && (Mp3CurrTimeSec >= 10))
        PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_END,0,0,0/*,0,0,0*/);
}
#else
void MP3DecodeTimeUpdate(void)
{
    unsigned long TempL;
    //----------------------------------------------------------------------------------------------
    //Time Update 20051020
    TempL = (MP3FileOffSet*TIME_GUAGE_CONST)/SysFile1Info.Fdt.FileSize;
    
    if(TempL != MusicPlayInfo.TimeGuageVal){
        MusicPlayInfo.TimeGuageVal = TempL;
        MusicDispFlag |= MUSIC_DISPFLAG_GUAGE;
    }
    
    TempL = (unsigned long)(MP3FileOffSet / ((unsigned long)(MusicPlayInfo.FirstBitrate) *125));
    
    if((Mp3CurrTimeSec != TempL)/* && (TempL <= Mp3Total)*/)
    {
        Mp3CurrTimeSec = TempL;
        MusicDispFlag |= (MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_FREE_TIME);
    }

    if((MusicPlayInfo.Mode == MUSIC_DECODE_BROWSE) && (Mp3CurrTimeSec >= 10))
        PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_END,0,0,0/*,0,0,0*/);
}
#endif
//******************************************************************************


