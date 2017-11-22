/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaLrc.c
* Author     : 
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*         
********************************************************************************
*/
#define _IN_WMALRC

#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "Global.h"
#include "WmaLrcVar.h"
#include "WmaDecodeGlobal.h"

#include "Config.h"
#include "Message.h"

void WmaLrcInit(void);
void WmaLrcPlay(void);
int WmaGetNextLrcTime(unsigned long *TimeSec);
int WmaGetLrcBuffer(unsigned long TimeSec);

//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaLrcModuleProc(void)
{
    switch (Fun2MsgArray[1])
    {
    case MSG_FUNCTION2_LRC_INIT:
        WmaLrcInit();
        break;
        
    case MSG_FUNCTION2_LRC_PLAY:
        WmaLrcPlay();
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
//    MusicDisplayBlock.MusicUnion.Lrc.pLrcBuffer = WmaLrcBuffer;
//    PostDisplayMessage(MSG_DISP_MUSIC_PLAY, (unsigned int)(&MusicDisplayBlock), 0, 0, 0, 0, 0, 0);
//}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaLrcInit(void)
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
        WmaLrcBuffer[ReturnVal] = 0;
    }
    
    //MusicDispFlag |= MUSIC_DISPFLAG_LRC;


    WmaPrevLrcPlayTimeSec = 0;
    WmaNextLrcPlayTimeSec = 0xffffffff;
    
    WmaLrcFlag.word = 0;
    WmaLrcBufferOffset = 0;
    
    WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
    if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
    
    TempTimeSec1 = cbLastTime;//WMAPlayCurTime.Hour * 3600 + WMAPlayCurTime.Minute * 60 + WMAPlayCurTime.Second;
    do {
        ReturnVal = WmaGetNextLrcTime(&TempTimeSec);
        if ((TempTimeSec > WmaPrevLrcPlayTimeSec) && (TempTimeSec < TempTimeSec1))
            WmaPrevLrcPlayTimeSec = TempTimeSec;
            
        if ((TempTimeSec > WmaPrevLrcPlayTimeSec) && (TempTimeSec < WmaNextLrcPlayTimeSec))
            WmaNextLrcPlayTimeSec = TempTimeSec;
    } while (ReturnVal != -1);
    
    if (WmaNextLrcPlayTimeSec != 0xffffffff) {
        FileSeek(0, SEEK_SET, hSlaveFile);
        WmaLrcFlag.word = 0;
        WmaLrcBufferOffset = 0;
        WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
        if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
            
        WmaGetLrcBuffer(WmaPrevLrcPlayTimeSec);
        TxtGbk2Unicode(WmaLrcBuffer,WmaLrcBuffer, 1);
        PostFun2Message(MSG_FUNCTION2_LRC,MSG_FUNCTION2_LRC_PLAY,0,0,0/*,0,0,0*/);
        MusicDispFlag |= MUSIC_DISPFLAG_LRC;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
void WmaLrcPlay(void)
{
    int ReturnVal;
    unsigned long TempTimeSec;
    
    TempTimeSec = cbLastTime;//WMAPlayCurTime.Hour * 3600 + WMAPlayCurTime.Minute * 60 + WMAPlayCurTime.Second;
    if(TempTimeSec < WmaPrevLrcPlayTimeSec) {
        WmaNextLrcPlayTimeSec = TempTimeSec;//0;//ffw
        WmaLrcBuffer[0] = 0;
    }
    if (TempTimeSec < WmaNextLrcPlayTimeSec) {
        return;
    }
    FileSeek(0, SEEK_SET, hSlaveFile);
    WmaLrcFlag.word = 0;
    WmaLrcBufferOffset = 0;
    WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
    WmaGetLrcBuffer(WmaNextLrcPlayTimeSec);
    TxtGbk2Unicode(WmaLrcBuffer,WmaLrcBuffer, 1);
    MusicDispFlag |= MUSIC_DISPFLAG_LRC;
//    if (DisplayFlag == DISP_MUSIC) {
//        PostDisplayMessage(MSG_DISP_MUSIC_PLAY, (unsigned int)(&MusicDisplayBlock), 0, 0, 0, 0, 0, 0);
//    }
    
    WmaPrevLrcPlayTimeSec = WmaNextLrcPlayTimeSec;
    WmaNextLrcPlayTimeSec = 0xffffffff;
    FileSeek(0, SEEK_SET, hSlaveFile);
    WmaLrcFlag.word = 0;
    WmaLrcBufferOffset = 0;
    WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
    if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
    
    do {
        ReturnVal = WmaGetNextLrcTime(&TempTimeSec);
        if ((TempTimeSec > WmaPrevLrcPlayTimeSec) && (TempTimeSec < WmaNextLrcPlayTimeSec))
            WmaNextLrcPlayTimeSec = TempTimeSec;
    } while (ReturnVal != -1);
    
    if (WmaNextLrcPlayTimeSec == 0xffffffff) {
        PostFun2Message(MSG_FUNCTION2_LRC,MSG_FUNCTION2_LRC_END,0,0,0/*,0,0,0*/);
    }
}

///*------------------------------------------------------------------------------
//Function:
//Input:
//Output:
//------------------------------------------------------------------------------*/
//__attribute__((section(".wma_text,\"ax\"")))
//int WmaGetNextLrcTime(unsigned long *TimeSec)
//{
//    //unsigned int TempTimeArray[6];
//    unsigned int i;
//    
//    while (1) {
//        if ((WmaLrcReadoutBuf[WmaLrcBufferOffset] == '[') && 
//            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 3] == ':') &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] <= '9')) &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] <= '9')) &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] <= '9')) &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] <= '9'))) {
//             
//            if (WmaLrcReadoutBuf[WmaLrcBufferOffset + 6] != ':') {
//                *TimeSec = ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 60
//                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0');
//                WmaLrcBufferOffset += 5;
//                break;
//            } else if (((WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] >= '0') && 
//                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] <= '9')) &&
//                       ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] >= '0') && 
//                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] <= '9'))) {
//                *TimeSec = ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 3600
//                            + ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0')) * 60
//                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] - '0');
//                WmaLrcBufferOffset += 8;
//                break;
//            }
//        } else {
//            WmaLrcBufferOffset += 1;
//        }
//            
//        if (WmaLrcBufferOffset >= WmaLrcBufferLen) {
//            if (WmaLrcFlag.bc.FileEnd)
//                return (-1);
//            else {
//                WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
//                if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
//                WmaLrcBufferOffset = 0;
//            }
//        }
//    }
//    return (0);
//}
//
///*------------------------------------------------------------------------------
//Function:
//Input:
//Output:
//------------------------------------------------------------------------------*/
//__attribute__((section(".wma_text,\"ax\"")))
//void WmaGetLrcBuffer(unsigned long TimeSec)
//{
//    unsigned int i,j;
//    
//    while (1) {
//        if ((WmaLrcReadoutBuf[WmaLrcBufferOffset] == '[') && 
//            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 3] == ':') &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] <= '9')) &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] <= '9')) &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] <= '9')) &&
//            ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] >= '0') && 
//             (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] <= '9'))) {
//             
//            if (WmaLrcReadoutBuf[WmaLrcBufferOffset + 6] != ':') {
//                if (TimeSec == ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 60
//                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0')) {
//                    WmaLrcBufferOffset += 5;
//                    break;
//                }
//                WmaLrcBufferOffset += 5;
//            } else if (((WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] >= '0') && 
//                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] <= '9')) &&
//                       ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] >= '0') && 
//                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] <= '9'))) {
//                if (TimeSec == ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 3600
//                            + ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0')) * 60
//                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] - '0')) {
//                    WmaLrcBufferOffset += 8;
//                    break;
//                }
//                WmaLrcBufferOffset += 8;
//            }
//        } else {
//            WmaLrcBufferOffset += 1;
//        }
//            
//        if (WmaLrcBufferOffset >= WmaLrcBufferLen) {
//            if (WmaLrcFlag.bc.FileEnd)
//                return;
//            else {
//                WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
//                if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
//                WmaLrcBufferOffset = 0;
//            }
//        }
//    }
//    
//    while (1) {
//        if ((WmaLrcReadoutBuf[WmaLrcBufferOffset] == ']') && 
//            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] != '[') &&
//            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] != 0x0d/*'\d'*/) &&
//            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] != '\r')) {
//            WmaLrcBufferOffset ++;
//            i = 0;
//            j = 0;
//            while ((WmaLrcReadoutBuf [WmaLrcBufferOffset/* + i*/] != '[') &&
//                    (WmaLrcReadoutBuf [WmaLrcBufferOffset/* + i*/] != 0x0d/*'\d'*/) &&
//                    (WmaLrcReadoutBuf [WmaLrcBufferOffset/* + i*/] != '\r')) {
//                    WmaLrcBuffer[i] = WmaLrcReadoutBuf [WmaLrcBufferOffset ++ /* + i*/];
//                i += 1;
//                if (WmaLrcBufferOffset >= WmaLrcBufferLen) {
//                    if (!WmaLrcFlag.bc.FileEnd) {
//                        WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
//                        if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
//                        WmaLrcBufferOffset = 0;
//				    } else {
//				        break;
//				    }
//                }
//            }
//            WmaLrcBuffer[i] = '\0';
//            break;
//        } else {
//            WmaLrcBufferOffset ++;
//        }
//    }
//}
////******************************************************************************

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
int WmaLrcBufferOffsetInc(void)
{
    if (++WmaLrcBufferOffset >= WmaLrcBufferLen) {
        if (WmaLrcFlag.bc.FileEnd)
            return (-1);
        else {
            WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
            if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
            WmaLrcBufferOffset = 0;
        }
    }
    return (0);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
int WmaLrcCheck(void)
{
    unsigned int i,j;
    
    if (WmaLrcReadoutBuf[WmaLrcBufferOffset] != '[') {
        if ( -1 == WmaLrcBufferOffsetInc()) return (-1);
//        if (++WmaLrcBufferOffset >= WmaLrcBufferLen) {
//            if (WmaLrcFlag.bc.FileEnd)
//                return (-1);
//            else {
//                WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
//                if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
//                WmaLrcBufferOffset = 0;
//            }
//        }
    } else {
        i = 0;
         do {
            if ((WmaLrcBufferOffset + i) >= (WmaLrcBufferLen/* - 1*/)) {
                
                if (WmaLrcFlag.bc.FileEnd) return (-1);
                    
                for (j = 0; j < i; j++) WmaLrcReadoutBuf[j] = WmaLrcReadoutBuf[512-i+j];
                WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf+i, 512-i,hSlaveFile);
                if (WmaLrcBufferLen < (512 - i)) WmaLrcFlag.bc.FileEnd = 1;
                else WmaLrcBufferLen = 512;
                WmaLrcBufferOffset = 0;
            }
            i++;
        } while(WmaLrcReadoutBuf[WmaLrcBufferOffset+i] != ']');
        
        if ((WmaLrcReadoutBuf[WmaLrcBufferOffset] == '[') && 
           (WmaLrcReadoutBuf[WmaLrcBufferOffset + 3] == ':') &&
           ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] >= '0') && 
            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] <= '9')) &&
           ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] >= '0') && 
            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] <= '9')) &&
           ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] >= '0') && 
            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] <= '9')) &&
           ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] >= '0') && 
            (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] <= '9'))) {
                return (0);
        } else {
            if ( -1 == WmaLrcBufferOffsetInc()) return (-1);
        }
    }
    return (1);
}     
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".wma_text,\"ax\"")))
int WmaGetNextLrcTime(unsigned long *TimeSec)
{
    int ReturnVal;
    
    while(1){
        ReturnVal = WmaLrcCheck();
        if (ReturnVal == 0) {
            if (WmaLrcReadoutBuf[WmaLrcBufferOffset + 6] != ':') {
                *TimeSec = ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 60
                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0');
                WmaLrcBufferOffset += 5;
                break;
            } else if (((WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] >= '0') && 
                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] <= '9')) &&
                       ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] >= '0') && 
                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] <= '9'))) {
                *TimeSec = ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 3600
                            + ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0')) * 60
                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] - '0');
                WmaLrcBufferOffset += 8;
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
__attribute__((section(".wma_text,\"ax\"")))
int WmaGetLrcBuffer(unsigned long TimeSec)
{
    unsigned int i,j;
    int ReturnVal;
    
    while (1){
        ReturnVal = WmaLrcCheck();
        if (ReturnVal == 0) {
            if (WmaLrcReadoutBuf[WmaLrcBufferOffset + 6] != ':') {
                if (TimeSec == ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 60
                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0')) {
                    WmaLrcBufferOffset += 5;
                    break;
                }
                WmaLrcBufferOffset += 5;
            } else if (((WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] >= '0') && 
                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] <= '9')) &&
                       ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] >= '0') && 
                        (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] <= '9'))) {
                if (TimeSec == ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 2] - '0')) * 3600
                            + ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 4] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 5] - '0')) * 60
                            + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 7] - '0') * 10 + (WmaLrcReadoutBuf[WmaLrcBufferOffset + 8] - '0')) {
                    WmaLrcBufferOffset += 8;
                    break;
                }
                WmaLrcBufferOffset += 8;
            }
        } else if (ReturnVal == -1) {
            return(-1);
        }
    }
    
    while (1) {
        if (WmaLrcReadoutBuf[WmaLrcBufferOffset] == ']') { 
            if ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] != '[') &&
                ((WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] != 0x0d) && (WmaLrcReadoutBuf[WmaLrcBufferOffset + 1] != 0x0a))) {
                //WmaLrcBufferOffset ++;
                if ( -1 == WmaLrcBufferOffsetInc()) return (-1);
                i = 0;
                while ((WmaLrcReadoutBuf [WmaLrcBufferOffset] != '[') &&
                      ((WmaLrcReadoutBuf [WmaLrcBufferOffset] != 0x0d) && (WmaLrcReadoutBuf [WmaLrcBufferOffset] != 0x0a))) {
                    
                    WmaLrcBuffer[i ++ ] = WmaLrcReadoutBuf [WmaLrcBufferOffset/* ++ */];
                    if ( -1 == WmaLrcBufferOffsetInc()) break;
//                    if (WmaLrcBufferOffset >= WmaLrcBufferLen) {
//                        if (!WmaLrcFlag.bc.FileEnd) {
//                            WmaLrcBufferLen = FileRead((unsigned char *)WmaLrcReadoutBuf, 512,hSlaveFile);
//                            if (WmaLrcBufferLen < 512) WmaLrcFlag.bc.FileEnd = 1;
//                            WmaLrcBufferOffset = 0;
//    				    } else {
//    				        break;
//    				    }
//                    }
                }
                WmaLrcBuffer[i] = '\0';
                break;
            }
        }
        if ( -1 == WmaLrcBufferOffsetInc()) return (-1);
    }
}
//******************************************************************************
