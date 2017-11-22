/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviMp3Decode.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#define _IN_AVIMP3DECODE

#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "Memap.h"
#include "Global.h"
#include "config.h"
#include "AviMacro.h"
#include "AviGlobal.h"

#include "AviFile.h"

#include "avi_mp3_api.h"

extern void avi_MP3_Init_MP3(MP3_DECODER_STRUCT *pMp3Obj);
extern void avi_MP3_FrameDecode(int *pOutBuffer, frame_params *pMP3FrameParams, MP3_DECODER_STRUCT *pMp3Obj);

__attribute__((section(".avi_a_text, \"ax\""))) void AviClearPcmData(void);
__attribute__((section(".avi_a_text, \"ax\""))) void AviFillPcmData(void);
__attribute__((section(".avi_a_text, \"ax\""))) void AviMP3DecodeOutputEnable(void);
__attribute__((section(".avi_a_text, \"ax\""))) void AviMP3DecodeOutputDisable(unsigned int bEnd);
__attribute__((section(".avi_a_text, \"ax\""))) unsigned int Read_AudioData_ToBuf(unsigned int *bufptr, unsigned int len);

MP3_DECODER_STRUCT  AviMP3Obj  __attribute__((section(".avi_a_bss, \"aw\"")));
frame_params        AviMP3FrameParams  __attribute__((section(".avi_a_bss, \"aw\"")));
al_table            avi_alloc_table  __attribute__((section(".avi_a_bss, \"aw\"")));
long local_MP3_s_freq[2][4] __attribute__((section(".avi_a_bss,\"aw\"")))= {{22050, 24000, 16000, 0}, {44100, 48000, 32000, 0}} ;

//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))
void AviMp3DmaOutBufferChange(void)
{
    unsigned int temp;
//    read_creg(%imask,temp);
//    bitclear_creg(%imask,15);
    
    write_mem32(DMAR_SAR2,(((unsigned long)(pAviPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB + 1) << 1);
    write_mem32(DMAR_CTL2,0x00100113);
    write_mem32(DMAR_CTL2+2,AviPCMOutBufferLength<<1);
    write_mem32(DMAR_CFG2,0x00000040);
    write_mem32(DMAR_CFG2+2,0x00001004);
    write_mem32(DMAR_DmaCfgReg,0x00000001);
    write_mem32(DMAR_MaskBlock, 0x00000404);
    write_mem32(DMAR_ChEnReg,   0x00000404); 
    
    AviPlayFlag.bc.bMp3BufferIndex = ~AviPlayFlag.bc.bMp3BufferIndex;
    pAviMP3OutputBuf = pAviMp3PCMOutAddress + AviPlayFlag.bc.bMp3BufferIndex * AVI_PCM_OUT_BUF_LENGTH;
    
    AVI_MP3_DECODE_REQ;
    
//    write_creg(%imask,temp);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))
void AviMp3DecodeHead(void)
{
    unsigned int temp;

    AviMp3Freq = 44100;
    AviMp3Bitrate = 128;
    AviAudioChunkLength = 0;
    AviAudioCurrFrameNum = 0;
    
    AviPlayFlag.bc.bMp3DecodeInit = 1;
    AviPlayFlag.bc.bMp3DecodeEnable = 1;
    
    read_creg(%imask, temp);
    bitclear_creg(%imask, 15);
    if (AVI_MP3_DECODE_OK == AviMp3DecodePlay()) {
        AviPlayFlag.bc.bMp3DecodeInit = 0;
    } 
    write_creg(%imask, temp);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))
void AviMp3DecodeStart(void)
{
    unsigned int temp;

    if (AviPlayState == AVI_DECODE_PLAY) {
        AviMP3DecodeOutputEnable();
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/

int AviBufWordsRemaining_Dram __attribute__((section(".avi_v_bss, \"aw\"")));

__attribute__((section(".avi_a_text, \"ax\"")))
int AviMp3DecodePlay(void)
{
    unsigned int i;
    unsigned int AviMp3DecodeTemp,ReadDataLen;
    int     testVal;
    
    if(AviPlayFlag.bc.bMp3DecodeInit)
    {
        AviPlayFlag.bc.bMp3DecodeInit = 0;
        AviPlayFlag.bc.bMp3DecodeError = 0;
        
        AviMP3FrameParams.mpeg2_5 = 0;
        AviMP3Obj.head_pack_state = 0;
        AviMP3Obj.head_totbit = 0;
        AviMP3FrameParams.alloc=&avi_alloc_table;
        AviMP3Obj.head_pkptr = (short *)(pAviMp3InputBuffer);
        avi_MP3_Init_MP3(&AviMP3Obj);
        
        AviMp3DecodeInputBufWordsUsed = 0;
        AviMp3DecodeInputBufWordsRemaining = 0;
        AviMP3DecodeFrameCount = 0;
    }
    
    AviMp3DecodeInputBufWordsUsed = AviMP3Obj.head_pkptr - (short *)(pAviMp3InputBuffer);
    
    if(AviMp3DecodeInputBufWordsUsed > AviMp3DecodeInputBufWordsRemaining) {
        AviPlayFlag.bc.bMp3DecodeError = 1;
    }
    
    if(AviPlayFlag.bc.bMp3DecodeError == 1) AviMp3DecodeInputBufWordsUsed = 40;
        
    AviMp3DecodeInputBufWordsRemaining = AviMp3DecodeInputBufWordsRemaining - AviMp3DecodeInputBufWordsUsed;
    //----------------------------------------------------------------------
    //MP3InputBufferToStart();
    for(i=0; i<AviMp3DecodeInputBufWordsRemaining; i++)
        pAviMp3InputBuffer[i] = pAviMp3InputBuffer[i + AviMp3DecodeInputBufWordsUsed];
        
    for( ; i < AVI_MP3_INPUT_BUF_LENGTH; i++ )
        pAviMp3InputBuffer[i] = 0;
    //----------------------------------------------------------------------
    AviMp3DecodeTemp = Read_AudioData_ToBuf(pAviMp3InputBuffer+AviMp3DecodeInputBufWordsRemaining, AVI_MP3_DECODE_MAX_FRAME_LENGTH - AviMp3DecodeInputBufWordsRemaining);
    if (AviMp3DecodeTemp < (AVI_MP3_DECODE_MAX_FRAME_LENGTH - AviMp3DecodeInputBufWordsRemaining)) {
        AviPlayFlag.bc.bMp3DecodeEnable = 0;
        //AviMP3DecodeOutputDisable(0);			// hjl @20060908
        PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_END,0,0,0/*,0,0,0*/);
        return;
    }
    AviMp3DecodeInputBufWordsRemaining += AviMp3DecodeTemp;
    
    AviMP3Obj.head_pkptr   = (short *)(pAviMp3InputBuffer);
    AviMP3Obj.head_totbit -= (AviMp3DecodeInputBufWordsUsed << 4);
    
    if(AviPlayFlag.bc.bMp3DecodeError == 1) {
        AviPlayFlag.bc.bMp3DecodeError = 0;
        AviMP3Obj.head_pack_state = 0;
        AviMP3Obj.head_totbit = 0;
        avi_MP3_Init_MP3(&AviMP3Obj);
    }
    
    avi_MP3_FrameDecode(pAviMP3OutputBuf, &AviMP3FrameParams, &AviMP3Obj);
    
    m_audio_framenum++;
    
    switch(AviMP3FrameParams.MP3_ErrCode)
    {       
    case 0: 
//        if(AviMP3DecodeFrameCount < 8)
//        {
//            if(++AviMP3DecodeFrameCount == 8)
//            {
//                OutputVolumeSet(OutputVolume);
//                MusicCodecMute(0);
//            }
//        }
        AviFillPcmData();
        return AVI_MP3_DECODE_OK;
        break;
        
    case 1:
    case 2:
        AviClearPcmData();
        AviPlayFlag.bc.bMp3DecodeInit = 1;
        return AVI_MP3_DECODE_ERROR;
        break;
        
    case 3:
    case 4:
    case 5:
    case 6:
    default:
        AviClearPcmData();  
        AviPlayFlag.bc.bMp3DecodeInit = 1;
        return AVI_MP3_DECODE_ERROR;
        break;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))
void AviMp3DecodeEnd(void)
{
    AviMP3DecodeOutputDisable(1);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))
unsigned int Read_AudioData_ToBuf(unsigned int *bufptr, unsigned int length)
{
    unsigned int i,len,FileReadDataLen,ReadedLength;
    int          ReadResult = I_FRAME;
    ReadedLength=0;
    while(length!=0){
        len =pAviMp3FileReadBuf -AviMp3DecodeBuffer;
        if((len>>1)>=length){
            Byte2Word(bufptr +ReadedLength, AviMp3DecodeBuffer, length);//len >> 1);
            for(i=0;i<(len-(length<<1));i++){
                AviMp3DecodeBuffer[i] = AviMp3DecodeBuffer[i+(length<<1)];
            }
            pAviMp3FileReadBuf -=(length<<1);
            ReadedLength +=length;
            break;
        }
        if(len >1){
            Byte2Word(bufptr +ReadedLength, AviMp3DecodeBuffer, len >>1);//len >> 1);
            AviMp3DecodeBuffer[0]=AviMp3DecodeBuffer[len-1]; 
            pAviMp3FileReadBuf -=(len&0xfffe);
            ReadedLength +=(len>>1);
            length -=(len>>1);
        }
        while (AviAudioChunkLength == 0){
            ReadResult = GetNextAudioChunk(&AviAudioChunkOffset,&AviAudioChunkLength);
            if (ERROR_FRAME == ReadResult) {
                PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_END,0,0,0/*,0,0,0*/);
                break;
            }
            FileSeek(AviAudioChunkOffset,SEEK_CUR,hSlaveFile);
        }
        if(ReadResult == ERROR_FRAME)
            break;
        if (AviAudioChunkLength > (AVI_MP3_READOUT_BUF_LENGTH-1)) {
            FileReadDataLen = FileRead((unsigned char*)pAviMp3FileReadBuf,(AVI_MP3_READOUT_BUF_LENGTH-1),hSlaveFile);
            if (FileReadDataLen !=(AVI_MP3_READOUT_BUF_LENGTH-1)) {
                break;
            }
        } else {
            FileReadDataLen = FileRead((unsigned char*)pAviMp3FileReadBuf,AviAudioChunkLength,hSlaveFile);
            if (FileReadDataLen !=AviAudioChunkLength) {
                break;
            }
        }
        AviAudioChunkLength -=FileReadDataLen;
        pAviMp3FileReadBuf +=FileReadDataLen;
    }
    return (ReadedLength);
    
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))
void AviClearPcmData(void)
{
    int *pData;
    unsigned int i;
    
    AviPCMOutBufferLength = 576;
        
    if(AviMP3FrameParams.header.lay == 1)  AviPCMOutBufferLength = 384;
    else if((AviMP3FrameParams.header.version != MPEG_PHASE2_LSF) || (AviMP3FrameParams.header.lay !=3))
         {AviPCMOutBufferLength = 1152;}
    
    pAviPCMOutputBuf = pAviMP3OutputBuf;
    
    for(i=0;i<(AviPCMOutBufferLength<<1);i++)
        pAviPCMOutputBuf[i] = 0;
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))

void AviFillPcmData(void)
{
    short    pcmbufferleftchannel=0,pcmbufferrightchannel=0;
    unsigned int l = 576;
       
    AviPCMOutBufferLength = 1152;
    pAviPCMOutputBuf = pAviMP3OutputBuf;
    
    if(AviMP3FrameParams.stereo == 2) {
        PCM16_PCMBufferDataChangeStereo(pAviMP3OutputBuf,pAviMP3OutputBuf,l);
        PCM16_PCMBufferDataChangeStereo(pAviMP3OutputBuf+1152,pAviMP3OutputBuf+1152,l);
    } else {
        PCM16_PCMBufferDataChangeMono(pAviMP3OutputBuf,pAviMP3OutputBuf,l);
        PCM16_PCMBufferDataChangeMono(pAviMP3OutputBuf+1152,pAviMP3OutputBuf+1152,l);
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_a_text, \"ax\"")))
void AviMP3DecodeOutputEnable(void)
{
    MusicCodecMute(0);
    
    bitclear_creg(%imask,15);
    AviMP3DecodeFrameCount = 0;
    
    write_mem(I2DSP_DPCTRL,0x0001);
    write_mem(I2DSP_TXCOM, 0x0006);
    
    write_mem32(DMAR_ClearBlock,0x00000004);
    write_mem32(DMAR_SAR2,(((unsigned long)(pAviPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB + 1) << 1);
    write_mem32(DMAR_CTL2,0x00100113);
    write_mem32(DMAR_CTL2+2,AviPCMOutBufferLength<<1);
    write_mem32(DMAR_CFG2,0x00000040);
    write_mem32(DMAR_CFG2+2,0x00001004);
    write_mem32(DMAR_DmaCfgReg,0x00000001);
    write_mem32(DMAR_MaskBlock, 0x00000404);
    write_mem32(DMAR_ChEnReg,   0x00000404); 
    
    SystemInfo.bc.bBackLightOffEn = 1;
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
__attribute__((section(".avi_a_text, \"ax\"")))
void AviMP3DecodeOutputDisable(unsigned int bEnd)
{
    MusicCodecMute(1);
    
    write_mem32(DMAR_ChEnReg,0x00000400);   //read_mem32(DMAR_ChEnReg)&0xfffffbfb);
    delay_nops(200);
    
    bitclear_creg(%imask,15);
    
    write_mem(I2DSP_TXCOM, 0x0006);         //Tx enable,I2Dsp enable,DMA request Enable
    
    write_mem32(DMAR_ChEnReg,0x00000400);   //read_mem32(DMAR_ChEnReg)&0xfffffbfb);
    write_mem32(DMAR_ClearBlock,0x00000004);
    
    while((read_mem(I2DSP_STATUS)&0x01)) write_mem32(I2DSP_TXDB, 0x0000);  
    while(!(read_mem(I2DSP_STATUS)&0x01));
    
    delay_nops(48*6);
    
    write_mem(I2DSP_TXCOM, 0x0000);         //Tx enable,I2Dsp enable,DMA request Enable
    
    if (bEnd) {
        AVI_MP3_DECODE_REQ_CLR;
        AVI_MP3_DECODE_DISABLE;
        write_mem32(DMAR_ChEnReg,0x00000700);   //read_mem32(DMAR_ChEnReg)&0xfffffbfb);
        write_mem32(DMAR_MaskBlock,0x00000700); //read_mem32(DMAR_MaskBlock)&0xfffffbfb);
        write_mem32(DMAR_ClearBlock,0x00000007);
    }
    
    SystemInfo.bc.bBackLightOffEn = 0;
    SystemInfo.bc.bSysIdleEn = 1;
    LightOffCounter = 0;
    SysIdleCounter = 0;
    
    bitset_creg(%imask,15);
    
//    unsigned int TimeOut;
//    
//    MusicCodecMute(1);
//    OutputVolumeSet(0);
//    
//    bitclear_creg(%imask,15);
//    
////------------------------------------------------------------
////wait DMA Transmite End
//    TimeOut = 0;
//    while((read_mem(DMAR_StatusBlock) & 0x0004) == 0) {
//        delay_nops(480);
//        if (++TimeOut > 1024) break;
//    }
//    write_mem32(DMAR_ClearBlock,0x00000004);
//    write_mem32(DMAR_ChEnReg,0x00000400);
//    
////------------------------------------------------------------
////wait I2DSP Transmite End
//    TimeOut = 0;
//    while((read_mem(I2DSP_STATUS) & 0x01) == 0) {
//        delay_nops(480);
//        if (++TimeOut > 1024) break;
//    }
//    Delay1Ms(4);
//    write_mem(I2DSP_TXCOM, 0x0000);
//    write_mem(I2DSP_DPCTRL,0x0001);         //Tx FIFO Reset
//    
////------------------------------------------------------------
////wait Other Dma End
//    if (bEnd) {
//        Delay1Ms(20);                       //zyz20060901(如果其他DMA正在传输，等待传输结束)
//        AVI_MP3_DECODE_REQ_CLR;
//        AVI_MP3_DECODE_DISABLE;
//        write_mem32(DMAR_ChEnReg,0x00000700);
//        write_mem32(DMAR_MaskBlock,0x00000700);
//        write_mem32(DMAR_ClearBlock,0x00000007);
//    }
//
//    SystemInfo.bc.bBackLightOffEn = 0;
//    SystemInfo.bc.bSysIdleEn = 1;
//    LightOffCounter = 0;
//    SysIdleCounter = 0;
//    
////------------------------------------------------------------
//    bitset_creg(%imask,15);
}

//********************************************************************************

