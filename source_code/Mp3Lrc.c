/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Mp3Lrc.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/14       1.0            ORG
********************************************************************************
*/
#define _IN_MP3LRC
#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "Global.h"
#include "Mp3DecodeGlobal.h"
#include "Mp3LrcVar.h"

#include "Config.h"
#include "Message.h"

void Mp3LrcInit(void);
void Mp3LrcPlay(void);
int GetNextLrcTime(unsigned long *TimeSec);
int GetLrcBuffer(unsigned long TimeSec);

//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3LrcModuleProc(void)
{
    switch (Fun2MsgArray[1])
    {
    case MSG_FUNCTION2_LRC_INIT:
        Mp3LrcInit();
        break;
        
    case MSG_FUNCTION2_LRC_PLAY:
        Mp3LrcPlay();
        break;
        
//    case MSG_FUNCTION2_LRC_END:
//        FileClose(hSlaveFile);
//        PostFun2Message(MSG_FUNCTION2_NONE,0,0,0,0,0,0,0);
//        break;
        
    default:
        break;
    }
}

///*------------------------------------------------------------------------------
//Function:
//Input:
//Output:
//------------------------------------------------------------------------------*/
//__attribute__((section(".mp3_text,\"ax\"")))
//void Id3DisplayInit(void)
//{
//    MusicDisplayBlock.DisplayFlag.dword = 0xffffffff;
//    MusicDisplayBlock.DisplayFlag.bit.bStroboscope = 0;
//    MusicDisplayBlock.DisplayFlag.bit.bLongFileName = 0;
//    
//    MusicDisplayBlock.MusicUnion.Lrc.pId3Info = (unsigned int *)(&Id3Info);
//    MusicDisplayBlock.MusicUnion.Lrc.pLrcBuffer = LrcBuffer;
//    PostDisplayMessage(MSG_DISP_MUSIC_PLAY, (unsigned int)(&MusicDisplayBlock), 0, 0, 0, 0, 0, 0);
//}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3LrcInit(void)
{
    int ReturnVal;
    unsigned long TempTimeSec,TempTimeSec1;
    
    Sys2InitFlag.word = 0xffff;
    Sys2InitFlag.bc.bLrcSysInitEnable = 0;
    Sys2InitFlag.bc.bLrcModuleInitEnable = 0;
    
    hSlaveFile = FileOpen(SysFile2Info.Path, SysFile2Info.Fdt.Name, "R");
    if (hSlaveFile == NOT_OPEN_FILE) {
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;//PostFun2Message(MSG_FUNCTION2_NONE,0,0,0,0/*,0,0,0*/);
        MusicPlayInfo.DispState = ID3_DISPLAY;
        return;
    }
    
    MusicPlayInfo.DispState = LRC_DISPLAY;
    for (ReturnVal = 0; ReturnVal < 512; ReturnVal++) {
        LrcBuffer[ReturnVal] = 0;
    }
    
    //MusicDispFlag |= MUSIC_DISPFLAG_LRC;


    PrevLrcPlayTimeSec = 0;
    NextLrcPlayTimeSec = 0xffffffff;
    
    LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf, 512,hSlaveFile);
    LrcFlag.word = 0;
    LrcBufferOffset = 0;
    
    if (LrcBufferLen < 512) LrcFlag.bc.FileEnd = 1;
    
    TempTimeSec1 = Mp3CurrTimeSec;//MP3PlayCurTime.Hour * 3600 + MP3PlayCurTime.Minute * 60 + MP3PlayCurTime.Second;
    do {
        ReturnVal = GetNextLrcTime(&TempTimeSec);
        if ((TempTimeSec > PrevLrcPlayTimeSec) && (TempTimeSec < TempTimeSec1))
            PrevLrcPlayTimeSec = TempTimeSec;
            
        if ((TempTimeSec > PrevLrcPlayTimeSec) && (TempTimeSec < NextLrcPlayTimeSec))
            NextLrcPlayTimeSec = TempTimeSec;
    } while (ReturnVal != -1);
    
    if (NextLrcPlayTimeSec != 0xffffffff) {
        
        FileSeek(0, SEEK_SET, hSlaveFile);
        LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf, 512,hSlaveFile);
        LrcFlag.word = 0;
        LrcBufferOffset = 0;
        
        if (LrcBufferLen < 512) LrcFlag.bc.FileEnd = 1;
            
        GetLrcBuffer(PrevLrcPlayTimeSec);
        TxtGbk2Unicode(LrcBuffer,LrcBuffer, 1);
        PostFun2Message(MSG_FUNCTION2_LRC,MSG_FUNCTION2_LRC_PLAY,0,0,0/*,0,0,0*/);
        MusicDispFlag |= MUSIC_DISPFLAG_LRC;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3LrcPlay(void)
{
    int ReturnVal;
    unsigned long TempTimeSec;
    
    TempTimeSec = Mp3CurrTimeSec;//MP3PlayCurTime.Hour * 3600 + MP3PlayCurTime.Minute * 60 + MP3PlayCurTime.Second;
    if(TempTimeSec < PrevLrcPlayTimeSec)
    {
        NextLrcPlayTimeSec = TempTimeSec;//0;//ffw
        LrcBuffer[0] = 0;
    }
    if (TempTimeSec < NextLrcPlayTimeSec) {
        return;
    }
    
    FileSeek(0, SEEK_SET, hSlaveFile);
//    LrcFlag.word = 0;
//    LrcBufferOffset = 0;
    LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf, 512,hSlaveFile);
    LrcFlag.word = 0;
    LrcBufferOffset = 0;
    
    GetLrcBuffer(NextLrcPlayTimeSec);
    TxtGbk2Unicode(LrcBuffer,LrcBuffer, 1);
    MusicDispFlag |= MUSIC_DISPFLAG_LRC;
//    if (DisplayFlag == DISP_MUSIC) {
//        PostDisplayMessage(MSG_DISP_MUSIC_PLAY, (unsigned int)(&MusicDisplayBlock), 0, 0, 0, 0, 0, 0);
//    }
    
    PrevLrcPlayTimeSec = NextLrcPlayTimeSec;
    NextLrcPlayTimeSec = 0xffffffff;
    
    FileSeek(0, SEEK_SET, hSlaveFile);
//    LrcFlag.word = 0;
//    LrcBufferOffset = 0;
    LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf, 512,hSlaveFile);
    LrcFlag.word = 0;
    LrcBufferOffset = 0;
    if (LrcBufferLen < 512) LrcFlag.bc.FileEnd = 1;
    
    do {
        ReturnVal = GetNextLrcTime(&TempTimeSec);
        if ((TempTimeSec > PrevLrcPlayTimeSec) && (TempTimeSec < NextLrcPlayTimeSec))
            NextLrcPlayTimeSec = TempTimeSec;
    } while (ReturnVal != -1);
    
//    if (NextLrcPlayTimeSec == 0xffffffff) {
//        PostFun2Message(MSG_FUNCTION2_LRC,MSG_FUNCTION2_LRC_END,0,0,0,0,0,0);
//    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
int Mp3LrcBufferOffsetInc(void)
{
    if (++LrcBufferOffset >= LrcBufferLen) {
        if (LrcFlag.bc.FileEnd)
            return (-1);
        else {
            LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf, 512,hSlaveFile);
            if (LrcBufferLen < 512) LrcFlag.bc.FileEnd = 1;
            LrcBufferOffset = 0;
        }
    }
    return (0);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
int Mp3LrcCheck(void)
{
    unsigned int i,j;
    
    if (LrcReadoutBuf[LrcBufferOffset] != '[') {
        if ( -1 == Mp3LrcBufferOffsetInc()) return (-1);
//        if (++LrcBufferOffset >= LrcBufferLen) {
//            if (LrcFlag.bc.FileEnd)
//                return (-1);
//            else {
//                LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf, 512,hSlaveFile);
//                if (LrcBufferLen < 512) LrcFlag.bc.FileEnd = 1;
//                LrcBufferOffset = 0;
//            }
//        }
    } else {
        i = 0;
        do {
            if ((LrcBufferOffset + i) >= (LrcBufferLen/* - 1*/)) {
                
                if (LrcFlag.bc.FileEnd) return (-1);
                    
                for (j = 0; j < i; j++) LrcReadoutBuf[j] = LrcReadoutBuf[512-i+j];
                LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf+i, 512-i,hSlaveFile);
                if (LrcBufferLen < (512 - i)) LrcFlag.bc.FileEnd = 1;
                else LrcBufferLen = 512;
                LrcBufferOffset = 0;
            }
            i++;
        }while (LrcReadoutBuf[LrcBufferOffset+i] != ']');
        
        if ((LrcReadoutBuf[LrcBufferOffset] == '[') && 
           (LrcReadoutBuf[LrcBufferOffset + 3] == ':') &&
           ((LrcReadoutBuf[LrcBufferOffset + 1] >= '0') && 
            (LrcReadoutBuf[LrcBufferOffset + 1] <= '9')) &&
           ((LrcReadoutBuf[LrcBufferOffset + 2] >= '0') && 
            (LrcReadoutBuf[LrcBufferOffset + 2] <= '9')) &&
           ((LrcReadoutBuf[LrcBufferOffset + 4] >= '0') && 
            (LrcReadoutBuf[LrcBufferOffset + 4] <= '9')) &&
           ((LrcReadoutBuf[LrcBufferOffset + 5] >= '0') && 
            (LrcReadoutBuf[LrcBufferOffset + 5] <= '9'))) {
                return (0);
        } else {
            //LrcBufferOffset += 1;
            if ( -1 == Mp3LrcBufferOffsetInc()) return (-1);
        }
    }
    return (1);
}     
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
int GetNextLrcTime(unsigned long *TimeSec)
{
    int ReturnVal;
    
    while(1){
        ReturnVal = Mp3LrcCheck();
        if (ReturnVal == 0) {
            if (LrcReadoutBuf[LrcBufferOffset + 6] != ':') {
                *TimeSec = ((LrcReadoutBuf[LrcBufferOffset + 1] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 2] - '0')) * 60
                            + (LrcReadoutBuf[LrcBufferOffset + 4] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 5] - '0');
                LrcBufferOffset += 5;
                break;
            } else if (((LrcReadoutBuf[LrcBufferOffset + 7] >= '0') && 
                        (LrcReadoutBuf[LrcBufferOffset + 7] <= '9')) &&
                       ((LrcReadoutBuf[LrcBufferOffset + 8] >= '0') && 
                        (LrcReadoutBuf[LrcBufferOffset + 8] <= '9'))) {
                *TimeSec = ((LrcReadoutBuf[LrcBufferOffset + 1] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 2] - '0')) * 3600
                            + ((LrcReadoutBuf[LrcBufferOffset + 4] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 5] - '0')) * 60
                            + (LrcReadoutBuf[LrcBufferOffset + 7] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 8] - '0');
                LrcBufferOffset += 8;
                break;
            }
        } else if (ReturnVal == -1) {
            return(-1);
        }
    }
    return (0);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
int GetLrcBuffer(unsigned long TimeSec)
{
    unsigned int i,j;
    int ReturnVal;
    
    while (1){
        ReturnVal = Mp3LrcCheck();
        if (ReturnVal == 0) {
            if (LrcReadoutBuf[LrcBufferOffset + 6] != ':') {
                if (TimeSec == ((LrcReadoutBuf[LrcBufferOffset + 1] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 2] - '0')) * 60
                            + (LrcReadoutBuf[LrcBufferOffset + 4] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 5] - '0')) {
                    LrcBufferOffset += 5;
                    break;
                }
                LrcBufferOffset += 5;
            } else if (((LrcReadoutBuf[LrcBufferOffset + 7] >= '0') && 
                        (LrcReadoutBuf[LrcBufferOffset + 7] <= '9')) &&
                       ((LrcReadoutBuf[LrcBufferOffset + 8] >= '0') && 
                        (LrcReadoutBuf[LrcBufferOffset + 8] <= '9'))) {
                if (TimeSec == ((LrcReadoutBuf[LrcBufferOffset + 1] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 2] - '0')) * 3600
                            + ((LrcReadoutBuf[LrcBufferOffset + 4] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 5] - '0')) * 60
                            + (LrcReadoutBuf[LrcBufferOffset + 7] - '0') * 10 + (LrcReadoutBuf[LrcBufferOffset + 8] - '0')) {
                    LrcBufferOffset += 8;
                    break;
                }
                LrcBufferOffset += 8;
            }
        } else if (ReturnVal == -1) {
            return(-1);
        }
    }
    
    while (1) {
        if (LrcReadoutBuf[LrcBufferOffset] == ']') { 
            if ((LrcReadoutBuf[LrcBufferOffset + 1] != '[') &&
                ((LrcReadoutBuf[LrcBufferOffset + 1] != 0x0d) && (LrcReadoutBuf[LrcBufferOffset + 1] != 0x0a))) {
                //LrcBufferOffset ++;
                if ( -1 == Mp3LrcBufferOffsetInc()) return (-1);
                i = 0;
                while ((LrcReadoutBuf [LrcBufferOffset] != '[') &&
                      ((LrcReadoutBuf [LrcBufferOffset] != 0x0d) && (LrcReadoutBuf [LrcBufferOffset] != 0x0a))) {
                    
                    LrcBuffer[i ++ ] = LrcReadoutBuf [LrcBufferOffset/* ++ */];
                    if ( -1 == Mp3LrcBufferOffsetInc()) break;
//                    if (LrcBufferOffset >= LrcBufferLen) {
//                        if (!LrcFlag.bc.FileEnd) {
//                            LrcBufferLen = FileRead((unsigned char *)LrcReadoutBuf, 512,hSlaveFile);
//                            if (LrcBufferLen < 512) LrcFlag.bc.FileEnd = 1;
//                            LrcBufferOffset = 0;
//    				    } else {
//    				        break;
//    				    }
//                    }
                }
                LrcBuffer[i] = '\0';
                break;
            }
        }
        //LrcBufferOffset ++;
        if ( -1 == Mp3LrcBufferOffsetInc()) return (-1);
    }
}
//******************************************************************************
