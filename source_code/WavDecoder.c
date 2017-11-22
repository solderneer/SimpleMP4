/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WavDecodeSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#define _IN_WAVDECODER

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "WavDecodeMacro.h"
#include "Global.h"
#include "WavDecodeGlobal.h"
#include "Resource.h"

void WavDecodeOutputEnable(void);
void WavDecodeOutputDisable(unsigned int bEnd);

ms_adpcm_data   g_tWavAdpcmData __attribute__((section(".wav_bss, \"aw\"")));
short           g_wWavBlockAlign __attribute__((section(".wav_bss, \"aw\"")));
unsigned long   g_dwWavTotalData __attribute__((section(".wav_bss, \"aw\"")));

extern  unsigned char FileOpenErrString[];//    __attribute__((section(".wav_data,\"aw\"")))= "FILE OPEN ERROR!";
extern  unsigned char FileSizeErrString[];//       __attribute__((section(".wav_data,\"aw\"")))= "FILE SIZE ERROR!";
extern  unsigned char FileFormatErrString[] ;//    __attribute__((section(".wav_data,\"aw\"")))= "FILE FORMAT ERROE!";

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavPCMOutBufferChange(void)
{    
    WavPlayFlag.bc.bPcmBufEmpty = 1;
    
    write_mem32(DMAR_SAR2,(((unsigned long)(pWavPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB + 1) << 1);
    write_mem32(DMAR_CTL2+2,WavPCMOutBufferLength<<1);
    write_mem32(DMAR_CTL2,0x00100113);
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
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeOutputEnable(void)
{
    bitclear_creg(%imask,15);
    dwWavCountGetPCM = 0;
    
    write_mem(I2DSP_DPCTRL,0x0001);         //Tx FIFO Reset
    write_mem(I2DSP_TXCOM, 0x0006);         //Tx enable,I2Dsp enable,DMA request Enable   
    
    write_mem32(DMAR_ClearBlock,0x00000004);
    write_mem32(DMAR_SAR2,(((unsigned long)(pWavPCMOutputBuf) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_DAR2,(unsigned long)(I2DSP_TXDB + 1) << 1);
    write_mem32(DMAR_CTL2+2,WavPCMOutBufferLength<<1);
    write_mem32(DMAR_CTL2,0x00100113);
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
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeOutputDisable(unsigned int bEnd)
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
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeHead(void)
{
    short        ch;
    int          wavResult = 0;
    unsigned int words_read;
    unsigned int i,j;
    unsigned int temp1,temp2;
    unsigned long samplerate;
{//-------------------------Wav Variable Init----------------------------------

    MusicPlayInfo.Bitrate = 128;
    MusicPlayInfo.Freq = 44100;
    WavPlayStatus = MusicPlayInfo.PlayState;
    
    wWavLastTime = 0;
    wWavTotalTime = 0;
    WavPlayFlag.word = 0;
        
    MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
    //MusicPlayInfo.DispState = MUSIC_DISPLAY;
    
    if(MusicDisplayInterface == 1) {
        MusicDisplayInterface = 0;
        MusicDispFlag = (MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |
                         MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | MUSIC_DISPFLAG_EQ | 
                         MUSIC_DISPFLAG_AB | MUSIC_DISPFLAG_SPECTRUM | MUSIC_DISPFLAG_FILENAME | 
                         MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | MUSIC_DISPFLAG_VOL | 
                         MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS | MUSIC_DISPFLAG_ORDER);
    } else {
        MusicDispFlag = (/*MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |*/
                         MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | MUSIC_DISPFLAG_EQ | 
                         MUSIC_DISPFLAG_AB | MUSIC_DISPFLAG_SPECTRUM | MUSIC_DISPFLAG_FILENAME | 
                         MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | /*MUSIC_DISPFLAG_VOL | */
                         MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS/* | MUSIC_DISPFLAG_ORDER*/);
    }
}
{//-------------------------Check WAV File Head--------------------------------
    if ((hMainFile = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R")) == NOT_OPEN_FILE) {
        WavFileFormatError = 1;
        PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_NONE,0,0,0/*,0,0,0*/);
        return;
    }
    
    words_read = FileRead(pWavInputBuffer, 512,hMainFile);
    if(words_read  < 512) {            //
       wavResult = -1 ;
    }
    
	WavFileOffSet=512;
    if(pWavInputBuffer[0]!=0x52||pWavInputBuffer[1]!=0x49||pWavInputBuffer[2]!=0x46||pWavInputBuffer[3]!=0x46) 
        wavResult = -1 ;//  return 1; //error not the ms-adpcm format RIFF
    
    if(pWavInputBuffer[12]!=0x66||pWavInputBuffer[13]!=0x6d||pWavInputBuffer[14]!=0x74||pWavInputBuffer[15]!=0x20)
        wavResult = -1 ;// return 1; //error not the ms-adpcm format fmt 
    
    if(pWavInputBuffer[504]!=0x64||pWavInputBuffer[505]!=0x61||pWavInputBuffer[506]!=0x74||pWavInputBuffer[507]!=0x61)
       wavResult = -1 ;//  return 1; //error not the ms-adpcm format data
   
    g_tWavAdpcmData.numCoefficients=7;
    g_tWavAdpcmData.wbitspersample=16;
    ch = g_tWavAdpcmData.channelCount=pWavInputBuffer[22];          //ch;
  
    if( g_tWavAdpcmData.channelCount != 1 && g_tWavAdpcmData.channelCount != 2) wavResult = -1 ;// return 1;
    samplerate=(pWavInputBuffer[25]<<8)|pWavInputBuffer[24];  //sampling rate
    
    if(samplerate==8000)
         g_wWavBlockAlign=256;
    else if(samplerate==16000)
         g_wWavBlockAlign=256;
    else if(samplerate==32000)
         g_wWavBlockAlign = 1024;
    else 
        wavResult = -1 ;// return 1;
    
    if((pWavInputBuffer[33]<<8)!=g_wWavBlockAlign) wavResult = -1 ;//return 1;
        
    if(pWavInputBuffer[34]!=4)wavResult = -1 ;// return 1;

     if(wavResult == -1) {//error
        FileClose(hMainFile);
        WavFileFormatError = 1;
        PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_NONE,0,0,0/*,0,0,0*/);
        return; 
    }
        
    GetLongFileName( SysFile1Info.LongFileName);
    GetDirLongPath(SysFile1Info.LongPath);
}
{//-------------------------Wav Variable Init----------------------------------
    g_tWavAdpcmData.samplesPerBlock =((g_wWavBlockAlign-(7*ch))*8)/(4*ch) +2;         
    WavPCMOutBufferLength =  g_tWavAdpcmData.samplesPerBlock;    

    g_dwWavTotalData=(((unsigned long)pWavInputBuffer[511]&0xff)<<24)
                    |(((unsigned long)pWavInputBuffer[510]&0xff)<<16)
                    |(((unsigned long)pWavInputBuffer[509]&0xff)<<8)
                    |(((unsigned long)pWavInputBuffer[508]&0xff));
               
    wWavTotalTime=(((SysFile1Info.Fdt.FileSize)/g_wWavBlockAlign)* g_tWavAdpcmData.samplesPerBlock) /samplerate;//计算时间按文件大小计算
	gWavnAvgBytesPerSec=(samplerate/g_tWavAdpcmData.samplesPerBlock)*g_wWavBlockAlign;
    MusicPlayInfo.FirstBitrate=(samplerate * g_tWavAdpcmData.channelCount)/250;//kbyte
    
//---------------------------------------------------------------------
    MusicPlayInfo.TimeGuageVal = 0;
    MusicPlayInfo.Freq = samplerate;
    MusicPlayInfo.Bitrate = MusicPlayInfo.FirstBitrate;
//------------------------------------------------------------------ 
    
    WavFileOffSetA = 0;
    WavFileOffSetB = 0;
    dwWavCountGetPCM = 0;
    
    PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_START,0,0,0/*,0,0,0*/);
}
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeStart(void)
{
    int i;
    
    for(i = WAVOUTBUFFERSIZE - 1; i >= 0; i --)
        *(FirstWavPCMOutAddress + i) = 0;
    
    WavPlayFlag.bc.bBufferIndex = 0;
    WavPlayFlag.bc.bPcmBufEmpty = 1;
    
    WavSamplingFreqSet(MusicPlayInfo.Freq);
    WavCpuFreqSet(MusicPlayInfo.Freq);
    
    WavPlayFlag.bc.bWavDecodeEnable = 0;
    if(MusicPlayInfo.PlayState == MUSIC_DECODE_PLAY) {
        WavDecodeOutputEnable();
        WavPlayFlag.bc.bWavDecodeEnable = 1;
    } else {
        if ((Sys2InitFlag.bc.bGameModuleInitEnable != 0) || (CurrCpuFreq != 51)) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bSysIdleEn = 1;
            SysIdleCounter = 0;
            bitset_creg(%imask,5);
        }
    }
    PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_PLAY,0,0,0/*,0,0,0*/);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeEnd(void)
{
    unsigned int i,j;
    
    WavPlayFlag.bc.bWavDecodeEnable = 0;
	
	if(WavPlayFlag.bc.bPcmBufEmpty)
    {
        WavPlayFlag.bc.bPcmBufEmpty = 0;
        
        FileClose(hMainFile);
        
        WavDecodeOutputDisable(1);
        
        if (MusicPlayInfo.Mode == MUSIC_DECODE_ONE) {
        //返回到资源管理器，选择当前歌曲
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            WavToBrowser(TO_CURRENT_FILE);
            return;
        }
        
        if ((MusicPlayInfo.RepMode == MUSIC_REPMODE_NORMAL) && (SysFile1Info.PlayedFileNum >= SysFile1Info.TotalFiles)) {
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            
            if (MusicPlayInfo.Range == MSG_FIND_FILE_ALL) {
            //返回到资源管理器，选择第一首歌曲
                WavToBrowser(TO_DEV_FIRST_FILE);
            } else {
            //返回到资源管理器，选择当前歌曲
                WavToBrowser(TO_CURRENT_FILE);
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
__attribute__((section(".wav_text,\"ax\"")))
int WavDecodePlay(void)
{
    int wmaResult = 0;
    unsigned int i;
    unsigned long temp;
    
    if((MusicPlayInfo.AB == MUSIC_DECODE_AB) && ((WavFileOffSet) >= WavFileOffSetB))
    {   /*AB End Position Check!*/
        WavFileOffSet = WavFileOffSetA; 
        WavPlayFlag.bc.bWavDecodeInit = 1;
        if (dwWavCountGetPCM >= 1) {
            MusicCodecMute(1);
        }
        dwWavCountGetPCM = 0;
    }
        
    if(WavPlayFlag.bc.bWavDecodeInit == 1) 
    {
        FileSeek(WavFileOffSet, SEEK_SET,hMainFile);
        WavPlayFlag.bc.bWavDecodeInit = 0;
        dwWavCountGetPCM = 0;
    }
    
    if ((WavPlayFlag.bc.bWavDecodeEnable)&&(WavPlayFlag.bc.bPcmBufEmpty)) {
        WavPlayFlag.bc.bBufferIndex = ~WavPlayFlag.bc.bBufferIndex;
        pWavPCMOutputBuf= FirstWavPCMOutAddress + WavPlayFlag.bc.bBufferIndex * WAVPCMBUFFERSIZE;
        WavPlayFlag.bc.bPcmBufEmpty = 0;
        
        short wReadWord=FileRead(pWavInputBuffer,g_wWavBlockAlign,hMainFile);		
        if(wReadWord!=g_wWavBlockAlign) 
        {
 		 PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_END,0,0,0/*,0,0,0*/);
		 return;
	    }
        WavFileOffSet+=g_wWavBlockAlign;
        WAVms_adpcm_decode_block(&g_tWavAdpcmData, pWavInputBuffer ,pWavPCMOutputBuf);
        
        i = WavPCMOutBufferLength-1;
        if( g_tWavAdpcmData.channelCount == 1){//单声道转换成双声道
            while(1) {
                pWavPCMOutputBuf[ (i) << 1 ] = pWavPCMOutputBuf[i];
                pWavPCMOutputBuf[( (i) << 1 ) + 1] = pWavPCMOutputBuf[i];
                i -= 1;
                if (i == 0)
                    break;
            }
        }

        dwWavCountGetPCM++; 
        if(dwWavCountGetPCM == 1) {
            MusicCodecMute(0);
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wav_text,\"ax\"")))
void WavDecodeTimeUpdate(void)
{
    unsigned int i,j;
    
    i = WavFileOffSet / gWavnAvgBytesPerSec;
    if((wWavLastTime != i) && (i <= wWavTotalTime)) {
        wWavLastTime = i;
        MusicDispFlag |= (MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_FREE_TIME);
    }
    
    j = (wWavLastTime * (TIME_GUAGE_CONST))/(wWavTotalTime);
    if(j!=MusicPlayInfo.TimeGuageVal){
        MusicPlayInfo.TimeGuageVal = j;
        MusicDispFlag |= MUSIC_DISPFLAG_GUAGE;
    }
    if((MusicPlayInfo.Mode == MUSIC_DECODE_BROWSE) && (wWavLastTime >= 10))
            PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_END,0,0,0/*,0,0,0*/);
}

//******************************************************************************
