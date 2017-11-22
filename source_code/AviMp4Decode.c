/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviMp4Decode.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#define _IN_AVIMP4DECODE

#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"
#include "config.h"
#include "Lcd.h"
#include "AviGlobal.h"
#include "AviMacro.h"
#include "MP4Decode.h"

#include "macro.h"

//#include "stdio.h"

#include "utils/mem_transfer.h"

__attribute__((section(".avi_v_text, \"ax\"")))
void AviMp4DecodeStart(void);

//********************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviMp4DecodeHead(void)
{
    int FrameID;
    
    NeedFillDataNum = 0;
    AviCurrFrameNum = 0;
    AviVideoChunkLength = 0;
    Mp4DecodeState = 0xffff;
    
    AviPlayFlag.bc.bMp4DecodeInit = 1;
    
    while (1)
    {
        Mp4DecodeState = MP4DecodeProc(AviPlayFlag.bc.bMp4DecodeInit);
        
        if (Mp4DecodeState == MP4_DECODE_OK) {
            AviPlayFlag.bc.bMp4DecodeInit = 0;
            break;
        } else {
            AviPlayFlag.bc.bMp4DecodeInit = 1;
            do {
                FrameID = GetNextVideoChunk(&AviVideoChunkOffset,&AviVideoChunkLength);
                if (FrameID == ERROR_FRAME) {
                    AviFileFormatError = 1;
//                    PreDisplayFlag = DISP_AVI;
//                    DisplayRecoveryTime = 0;
//                    LCD_Init(0/*LCD_INIT_NORMAL*/);
//                    LCD_SetColor(0);
//                    LCD_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
//                    SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
//                    #ifdef LCD_MEMORY_DEV
//                        DmaFillImage2Lcd();
//                    #endif
                    
                    PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_NONE,0,0,0/*,0,0,0*/);
                    return;
                }
                FileSeek(AviVideoChunkOffset,SEEK_CUR,hSlaveFile);
            } while (FrameID == I_FRAME);
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviMp4DecodeStart(void)
{
    AviMp4FrameCount = 0;
    if (AviPlayState == AVI_DECODE_PLAY) {
        AviPlayFlag.bc.bMp4DecodeEnable = 1;
    } else {
        AviPlayFlag.bc.bMp4DecodeEnable = 0;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviMp4DecodeEnd(void)
{
//    write_mem32(DMAR_ChEnReg,0x00000300);   //read_mem32(DMAR_ChEnReg)&0xfffffbfb);
//    write_mem32(DMAR_MaskBlock,0x00000300); //read_mem32(DMAR_MaskBlock)&0xfffffbfb);
//    write_mem32(DMAR_ClearBlock,0x00000003);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviMp4DecodePlay(void)
{
    int FrameID;
    
    if ((AviPlayFlag.bc.bMp4DecodeEnable) || (AviPlayFlag.bc.bAviFirstLongDown==1)) { 

        
        if(AviPlayFlag.bc.bAviFirstLongDown==1){
            if(uiAviSpeedFlag == 1){
                uiAviSpeedFlag   = 0;
                AviMp4FrameCount = 2;
            }else if(uiAviSpeedFlag == 2){
                AviPlayFlag.bc.bAviFirstLongDown = 0;
                uiAviSpeedFlag   = 0;
                AviMp4FrameCount = 2;
                AviMp4DecodeStart();
                AviMp3DecodeStart();
            }else{
                //AviMp4FrameCount = 0;
                return;
            }
        }
        
        Mp4DecodeState = MP4DecodeProc(AviPlayFlag.bc.bMp4DecodeInit);
        //AviCurrFrameNum ++ ;
        
        switch (Mp4DecodeState) {
        case MP4_DECODE_OK:                
            AviPlayFlag.bc.bMp4DecodeInit = 0;
            
           // DMA_RGB_E2LCD( );    <jh@2006-3-1 17:07>
            
            if (AviPlayState != AVI_DECODE_PLAY)  //当暂停状态下按快进快退时，显示一贞后应停止播放
                AviPlayFlag.bc.bMp4DecodeEnable = 0;
            //AVI_MP3_DECODE_ENABLE;
            //uiAviSendtoLCDTimes = 0;        // 每次解完一帧后初始化该计数器            
            //if((uiAviStatusTimeout % 15) == 0)
            //    AviPlayFlag.bc.bAviFirstPause = 1;
            //uiAviStatusTimeout++;                
            //Post_Processing();
            break;
            
        case MP4_DECODE_FAIL:
            
            //do {
                AVI_MP3_DECODE_DISABLE;
                AviPlayFlag.bc.bMp4DecodeInit = 1;
                AviPlayFlag.bc.bMp3DecodeInit = 1;
                //FrameID = GetNextVideoChunk(&AviVideoChunkOffset,&AviVideoChunkLength);
                FrameID = GetNextVideoIFrameChunk( &AviVideoChunkOffset, &AviVideoChunkLength);  // Modified by HuangJiliu @20060418
                if (FrameID == ERROR_FRAME) {
                    PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_END,0,0,0/*,0,0,0*/);
                    break;
                }
                else{
                    FileSeek(AviVideoChunkOffset,SEEK_CUR,hMainFile);
                    SynAudio2VideoChunk(&AviAudioChunkOffset, &AviAudioChunkLength, &AviCurr_IFrameNum_FrameNum, &AviLastChunkNum, &AviStreamInfo);
                    FileSeek(AviAudioChunkOffset,SEEK_CUR,hSlaveFile);
                    AviMp4FrameCount = 0;
                }
                AVI_MP3_DECODE_ENABLE;
                //AviCurrFrameNum ++ ;
                
            //} while (FrameID == I_FRAME);
            break;
        case MP4_DECODE_END:
        case XVID_ERR_FORMAT:
            AviPlayFlag.bc.bMp4DecodeInit = 1;
            PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_END,0,0,0/*,0,0,0*/);
            break;
        default:
            AviPlayFlag.bc.bMp4DecodeInit = 1;
            break;
        }
    }
    else if(AviPlayFlag.bc.bMp4DecodeEnable == 0){
//--------------2006-01-18增加对DMA的判断，防止出现白屏-------	
       	unsigned long   TimeOut;
    	TimeOut = 0;
//        do {
//            if (++TimeOut == 32768)
//                break;
//        } while(AviPlayFlag.bc.bRGBBufferEmpty != TRUE);		//if buffer A/B is still transferring data through DMA,then waiting
//        while(AviPlayFlag.bc.bRGBBufferEmpty != TRUE);
//--------------2006-01-18增加对DMA的判断，防止出现白屏-------
        if((TimeOut<32768)&&(AviPlayFlag.bc.bAviFirstPause == 1)){
            AviPlayFlag.bc.bAviFirstPause = 0;
            //DISP_Mp4();
        }
    }
    else{
        ;   
    }
} 


/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/


//********************************************************************************
__attribute__((section(".avi_v_text, \"ax\"")))
void AviStatusBarDispWinSet(void)
{
    if(uiAviStatusBarDispFlag == AVI_STATUS_BAR_NULL){
        if(uiAviStatusBarDispBits){
            LCDDEV_SetWindow(0, (VIDEO_UPMAIN_PIC_Y + VIDEO_UPMAIN_PIC_YSIZE), (LCD_MAX_XSIZE - 1), (LCD_MAX_YSIZE - VIDEO_DOWNMAIN_PIC_YSIZE - 1));
            uiAviLcdDmaDoneFlag    = 1;
            uiAviStatusBarDispFlag = AVI_STATUS_BAR_DISP;
            uiAviStatusBarTimeCont = 0;
            uiAviStatusBarDispTime = 0;
        }
    }else if(uiAviStatusBarDispFlag == AVI_STATUS_BAR_DISP){
        uiAviStatusBarTimeCont++;
        if(++uiAviStatusBarDispTime >= Play_Frame_Rate){
            uiAviStatusBarDispTime = 0;
            uiAviStatusBarDispBits |= (AVI_STATUS_BAR_JDT_BMASK | AVI_STATUS_BAR_TMR_BMASK);
        }
        if(uiAviStatusBarTimeCont >= Play_Frame_Rate*5){
            LCDDEV_SetWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
            uiAviStatusBarDispFlag = AVI_STATUS_BAR_NULL;
            uiAviStatusBarDispBits = 0;
            uiAviLcdDmaDoneFlag    = 0;
        }else if(uiAviStatusBarDispBits != 0){
            uiAviLcdDmaDoneFlag    = 1;
        }
    }   
}

__attribute__((section(".avi_v_text, \"ax\"")))
void AviMp4FrameCountInc(void)
{
    if(AviPlayFlag.bc.bMp4DecodeEnable){
//--------------------------------------------------------------------------------------        
        //修正AVI音视频同步
//        if(Play_Frame_Rate == 24){

        if(AviAudioCurrFrameNum < m_audio_framenum){
            if(Timeadjust*1225 < (((unsigned long)(m_audio_framenum - AviAudioCurrFrameNum)<<6)*Play_Frame_Rate) ){
                if(AviMp4FrameCount>0){
                    Timeadjust++;          //计数AviMp4FrameCount减过多少；即调整过多少2/framerate毫秒
                    AviMp4FrameCount--;
                }
            }
        }
        if(m_video_framenum - ((((long)AviAudioCurrFrameNum<<5)*Play_Frame_Rate+613)/1225) >= 1){    
            if(AviMp4FrameCount >= 2) 
                AviMp4FrameCount-=2;
            m_video_framenum -= 1;
        }
        
//---------------------------------------------------------------------------------------                
        if((++ AviMp4FrameCount >= 2) && (Dma_RGB_E2LCD == 0) && (((LCDSdramIndex +1)%(AVI_RGB_SDRAM_BUF_CONT+1)) != AviRgbSdramIndex) && (disabledmatolcd == 0)){//<jh@2006-3-1 17:08>
            
            AviMp4FrameCount -= 2;   //<jh@2006-3-1 17:08>
//============================================================================//
            AviStatusBarDispWinSet();
//============================================================================//            
            DMA_RGB_E2LCD();       //<jh@2006-3-1 17:09>
            m_video_framenum++;
          }
    }else{
        if(Dma_RGB_E2LCD == 0){
            uiAviStatusBarTimeCont = 0;
            uiAviStatusBarDispTime = 0;
            
            if(uiAviLcdDmaDoneFlag == 0){
                AviStatusBarDispWinSet();
            }
            if(uiAviLcdDmaDoneFlag == 1){
                uiAviLcdDmaDoneFlag = 2;
            }
        }
    }
}


/*
void AviFFD_L(void)
{
    int FindIFrameResult;
    FindIFrameResult = GetPreVideoIFrameChunk( &AviVideoChunkOffset, &AviAudioChunkOffset, &AviVideoChunkLength, &AviAudioChunkLength);
    if
}
*/
