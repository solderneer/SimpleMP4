/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#define _IN_AVISCHEDULE

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"

#include "AviMacro.h"
#include "AviGlobal.h"

#include "Config.h"
#include "AviFile.h"
#include "AviMp4Decode.h"
#include "AviMp3Decode.h"
#include "BrowserMacro.h"
#include "KeyMacro.h"
#include "Lcd.h"

//void FunAviPlay(void);
//void FunAviInit(void);
void FunAviInit(void);
void AviCPUInit(void);
void AviVariableInit(void);
void AviCodecInit(void);
void AviKeyProc(void);
void AviHeadProc(void);
void AviDecodeStart(void);
void AviDecodeEnd(void);
void AviDecodePlay(void);

AVI_FRAME_RATE_SETTING
Avi_Frame_Rate_Tab[] __attribute__((section(".avi_v_data, \"aw\""))) = {
                            {15, 66666},
                            {16, 62500},
                            {18, 55555},
                            {20, 50000},
                            {22, 45454},
                            {23, 41708},        //23.976
                            {24, 41666},
                            {25, 40000},
                            {30, 33333}
                       };
//********************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviModuleProc(void)
{    
    unsigned int imask_tmp;
    switch(Fun1MsgArray[1])
    {
        case MSG_FUNCTION1_AVI_DECODE_INIT:
            FunAviInit();
            break;
        
        case MSG_FUNCTION1_AVI_DECODE_HEAD:
            AviHeadProc();
            break;
        
        case MSG_FUNCTION1_AVI_DECODE_START:
            AviDecodeStart();
            break;
        
        case MSG_FUNCTION1_AVI_DECODE_PLAY:
            AviDecodePlay();
            break;
        
        case MSG_FUNCTION1_AVI_DECODE_END:
            AviDecodeEnd();
            break;
        
        default:
            break;
    }
    
    if (AviFileFormatError == 1) {
        AviFileFormatError = 0;
        //if (/*(DisplayFlag != DISP_AVI) || */(SystemInfo.bc.bDisplayRecovery)/* || (SystemInfo.bc.bBackLightStatus == 2)*/) {
            //文件错误，显示对话框结束，进入下一首歌曲
            AviWaitForDmaDone();
            AviDecodeEnd();
//-------------------------------------------------------------
//解决AVI刷屏死机的问题
            read_creg(%imask, imask_tmp);
            bitclear_creg(%imask, 15);
            uiAviStatusBarDispFlag = AVI_STATUS_BAR_NULL;
            uiAviStatusBarDispBits = 0;
            uiAviLcdDmaDoneFlag    = 0;
            write_creg(%imask, imask_tmp);
//-------------------------------------------------------------

            LCD_Init(0);
            LCD_SetColor(0);
            LCD_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
            SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
#ifdef LCD_MEMORY_DEV
            DmaFillImage2Lcd();
#endif
            Delay1Ms(1000);
//------------------------------------------------------------------------------            
            return;
        //}
    }
    
    if (DisplayFlag == DISP_AVI) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            AviKeyProc();
        }else {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 0;
            bitset_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart1 = 1;
            LCD_SDRAM_Standby_Return();
        }
        
        if (SystemInfo.bc.bHoldDisplay) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bHoldDisplay = 0;
            bitset_creg(%imask,5);
            AviStatusBarDispSet(AVI_STATUS_BAR_LOCK_BMASK);
            PreDisplayFlag = DisplayFlag;
            return;
        }
            
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            if(uiAviStatusBarDispFlag != AVI_STATUS_BAR_NULL){
                AviStatusBarDispSet(AVI_STATUS_BAR_BATT_BMASK);
            }
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {           //add by zyz
            SystemInfo.bc.bDisplayRecovery = 0;
            AviStatusBarDispSet(AVI_STATUS_BAR_DISP_ALL);
        }
        
        if (SystemInfo.bc.bBackLightOnStart1) {
            SystemInfo.bc.bBackLightOnStart1 = 0;
            LcdBackLightOn();
            KeyReset();
        }
        
        if(uiAviLcdDmaDoneFlag == 2){
            
            disabledmatolcd = 1;
            if(uiAviLcdDmaDoneFlag == 2){
                LCDDEV_SetWindow(0, 0, LCD_MAX_XSIZE-1, LCD_MAX_YSIZE-1);
                DISP_Mp4(uiAviStatusBarDispBits);
                LCDDEV_SetWindow(0, (VIDEO_UPMAIN_PIC_Y + VIDEO_UPMAIN_PIC_YSIZE), (LCD_MAX_XSIZE - 1), (LCD_MAX_YSIZE - VIDEO_DOWNMAIN_PIC_YSIZE - 1));
                
                read_creg(%imask, imask_tmp);
                bitclear_creg(%imask, 15);
                uiAviStatusBarDispBits = 0;
                uiAviLcdDmaDoneFlag    = 0;
                write_creg(%imask, imask_tmp);
            }
            disabledmatolcd =0;
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void FunAviInit(void)
{
    if (Sys1InitFlag.bc.bAviSysInitEnable) {
        AviCPUInit();
        AviCodecInit();
//------------------------------------------------------------------------------
        AviCopyPic2Sdram();
//------------------------------------------------------------------------------    
    }   
    KeyReset();
    AviVariableInit();
//    DmaInit();
//    SDRAM_Init();
    MP4_LCD_Init();
    AviFileFormatError = 0;
    SystemInfo.bc.bBackLightOffEn = 1;
    PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_HEAD,0,0,0/*,0,0,0*/);
}

//********************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviCPUInit(void)
{
    unsigned int temp;
    
    while(DmaFillImage2LcdFlag);
    
    bitclear_creg(%imask,15);
    
    SysCpuSetFreq(72);      //SysCpuSetFreq(72);
    SysTimer0Set(72,25000);  //SysTimer0Set(72,8333);
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0002);
    write_mem32(DMAR_ClearBlock,0x00000007);
    
    write_creg(%ip0, 0x0000);
    write_creg(%ip1, 0x4000);//write_creg(%ip1,0x4800);
    write_creg(%ireq,0x0000);
    bitset_creg(%imask,7);
    bitset_creg(%imask,15);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviVariableInit(void)
{
    Sys1InitFlag.word = 0xffff;
    
    Sys1InitFlag.bc.bAviSysInitEnable = 0;
    Sys1InitFlag.bc.bAviModuleInitEnable = 0;
    
    pAviMp3FileReadBuf = AviMp3DecodeBuffer;
    pAviMp3InputBuffer = AviMp3DecodeBuffer + AVI_MP3_READOUT_BUF_LENGTH;
    pAviMp3PCMOutAddress = pAviMp3InputBuffer + AVI_MP3_INPUT_BUF_LENGTH;
    pAviMP3OutputBuf = pAviMp3PCMOutAddress;
    pAviPCMOutputBuf = pAviMp3PCMOutAddress + AVI_PCM_OUT_BUF_LENGTH;
    
    pAviMp4FileReadBuf = InputByteBuffer;
    
    AviPlayState = AVI_DECODE_PLAY;
    
    AviBlockFlag  = 1;
    AviLcdDmaFlag = 1;
    AviI2EDmaFlag = 1;
    AviDmaChlFlag.Chl = 0;
    Dma_16x16Block = 0;
    Dma_16x8Block  = 0;
    Dma_E2IBlock   = 0;
    Dma_E2IBlock_Y = 0;
    Dma_E2IBlock_UV= 0;
    Dma_I2EBlock   = 0;
    Dma_Rgb2Lcd    = 0;
    
    Dma_RGB_I2E     = 0;
    Dma_RGB_E2LCD   = 0;
    
    AviRgbSdramIndex = 0;          //<jh@2006-3-2 11:17, because AviRgbSdramIndex will increment in advance>

    FRAME_TYPE_LABEL[0] = 1;
    FRAME_TYPE_LABEL[1] = 0;
    FRAME_TYPE_LABEL[2] = 0;
    
    LCDSdramIndex = AVI_RGB_SDRAM_BUF_CONT; //zcs       //<jh@2006-3-1 17:10>
    AviLcdLine    = 0;
    VideoIndexCount = 0;
//------------------------------------------------------------------------------
    m_audio_framenum = 0;
    m_video_framenum = 0;
    Timeadjust = 0;
    Play_Frame_Rate = 20;    
//--------------------------------------------------------------------------
    uiAviStatusBarDispFlag = 0;
    uiAviStatusBarDispTime = 0;
    uiAviLcdDmaDoneFlag    = 0;
    uiAviStatusBarTimeCont = 0;
    uiAviSpeedFlag         = 0;
    uiAviStatusBarDispBits = AVI_STATUS_BAR_DISP_ALL;
    MovieGuageBak          = 0;
//------------------------------------------------------------------------------
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviCodecInit(void)
{

    CodecIicInit();
#ifndef  _INSIDE_HPSENSE_USED_    
    CodecCommandSet(P_ADD_TR1,  0x0002);
#else 
    CodecCommandSet(P_ADD_TR1,  0x0000);
#endif    
    CodecCommandSet(P_ADD_CCR,  0x0011);    //12MHZ,44.1KHZ/44.1KHZ
    CodecCommandSet(P_ADD_CR1,  0x0028);
    
    Pre_CR1_Value = 0x0008;
    
    CodecCommandSet(P_ADD_CR2,  0x0000);    //16BIT
//    CodecCommandSet(P_ADD_PMR1, 0x003f);    //DAC Power On
//-----------------------------------------------------------------
#if(CPU_TYPE == _RK2608A_)
    CodecCommandSet(P_ADD_PMR1, 0x001f);    //All Power Down
#else
    CodecCommandSet(P_ADD_PMR1, 0x003f);    //All Power Down
#endif

    CodecCommandSet(P_ADD_PMR2, 0x0008);    //All Power Down
//-----------------------------------------------------------------
    Delay1Ms(100);
    write_mem(I2DSP_TXCONF,0x0008);         //Slave Mode, 16 bit, 44.1KHZ
    write_mem(I2DSP_TXCOM, 0x0000);         //Tx enable,I2Dsp enable,DMA request Enable   
    write_mem(I2DSP_DPCTRL,0x0001);         //Tx FIFO Reset
    write_mem(CLOCK_I2SMCLKCON, 0x0100);
    OutputVolumeSet(OutputVolume);
    CodecPowerOn();
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviHeadProc(void)
{    
    unsigned long tmpTotalSecond;
    unsigned int imask_tmp;
    unsigned int time0_interval;   
    int Temp, i, frame_rate_supported = TRUE;
    
    hMainFile = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R");
    if(hMainFile == NOT_OPEN_FILE){
        AviFileFormatError = 1;
//        PreDisplayFlag = DISP_AVI;
//        DisplayRecoveryTime = 0;
//
////-------------------------------------------------------------
////解决AVI刷屏死机的问题
//        read_creg(%imask, imask_tmp);
//        bitclear_creg(%imask, 15);
//        uiAviStatusBarDispFlag = AVI_STATUS_BAR_NULL;
//        uiAviStatusBarDispBits = 0;
//        uiAviLcdDmaDoneFlag    = 0;
//        write_creg(%imask, imask_tmp);
////-------------------------------------------------------------
//        
//        LCD_Init(0/*LCD_INIT_NORMAL*/);
//        LCD_SetColor(0);
//        LCD_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
//        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_NONE,0,0,0/*,0,0,0*/);
        return;
    }
    hSlaveFile = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R");
    hAudioIndex = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R");
    hVideoIndex = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R");
    
    Temp = AviFileIdentify(hMainFile,&AviStreamInfo);
    
    frame_rate_supported = FALSE;
    for(i = 0; i < NUMBER_OF_FRAME_RATE; i++){
        if(AviStreamInfo.MicroSecondPerFrame == ((unsigned long)Avi_Frame_Rate_Tab[i].mspf/10)){
            frame_rate_supported = TRUE;
            break;
        }
    }
    //if (/*(hMainFile == NOT_OPEN_FILE) || */(Temp != 0) || ((AviStreamInfo.MicroSecondPerFrame != 5000) && (AviStreamInfo.MicroSecondPerFrame != 4166) &&(AviStreamInfo.MicroSecondPerFrame != 4545))) {
    if ((Temp != 0) || (frame_rate_supported == FALSE)) {
        AviFileFormatError = 1;
//        PreDisplayFlag = DISP_AVI;
//        DisplayRecoveryTime = 0;
//        
////-------------------------------------------------------------
////解决AVI刷屏死机的问题
//        read_creg(%imask, imask_tmp);
//        bitclear_creg(%imask, 15);
//        uiAviStatusBarDispFlag = AVI_STATUS_BAR_NULL;
//        uiAviStatusBarDispBits = 0;
//        uiAviLcdDmaDoneFlag    = 0;
//        write_creg(%imask, imask_tmp);
////-------------------------------------------------------------
//        
//        LCD_Init(0/*LCD_INIT_NORMAL*/);
//        LCD_SetColor(0);
//        LCD_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
//        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_NONE,0,0,0/*,0,0,0*/);
        return;
    }

    for(i = 0; i < NUMBER_OF_FRAME_RATE; i++){
        if(AviStreamInfo.MicroSecondPerFrame == ((unsigned long)Avi_Frame_Rate_Tab[i].mspf/10)){
            Play_Frame_Rate = (long)Avi_Frame_Rate_Tab[i].fps;
            time0_interval =  (unsigned int)(Avi_Frame_Rate_Tab[i].mspf/2);
            if(Play_Frame_Rate == 15){
                SysCpuSetFreq(64);
                SysTimer0Set(64, time0_interval);
            }else if(Play_Frame_Rate < 20){
                if (MemorySelect == 0) {   
                    SysCpuSetFreq(70);     
                    SysTimer0Set(70,time0_interval);
                } else {                   
                    SysCpuSetFreq(78);     
                    SysTimer0Set(78,time0_interval);
                }                          
            }else{
                if (MemorySelect == 0) {   
                    SysCpuSetFreq(78);     
                    SysTimer0Set(78,time0_interval);
                } else {                   
                    SysCpuSetFreq(80);     
                    SysTimer0Set(80,time0_interval);
                }                 
            }
            break;
        }
    }
        
    FFDWFramecount = Play_Frame_Rate*10;    
//    ////// 20060220 计算影片的总时间 start /////////////////////////////////////////////////////////
    tmpTotalSecond = (AviStreamInfo.MicroSecondPerFrame * AviStreamInfo.TotalFrameCount)/100000;
//    AviPlayTotalTime.Hour   = tmpTotalSecond / 3600;
//    AviPlayTotalTime.Minute = (tmpTotalSecond % 3600) / 60;
//    AviPlayTotalTime.Second = ((tmpTotalSecond % 3600) % 60);
    //////  计算影片的总时间 end ///////////////////////////////////////////////////////////
    
    FileSeek(AviStreamInfo.StartDataPos,SEEK_SET,hMainFile);
    FileSeek(AviStreamInfo.StartDataPos,SEEK_SET,hSlaveFile);
    
    if (AviStreamInfo.AviFileFormat == AVI_AUDIO_ONLY) {
        //发送显示“黑屏”消息
    } else if (AviStreamInfo.AviFileFormat == AVI_VIDEO_ONLY) {
        AviMp4DecodeHead();
    } else{// (AviStreamInfo.AviFileFormat == AVI_AUDIO_VIDEO) {
        AviMp3DecodeHead(); //2005-12-22 Test
        AviMp4DecodeHead();
    }
    //发送显示时间，曲目等信
    PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_START,0,0,0/*,0,0,0*/);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviDecodeStart(void)
{
    if (AviStreamInfo.AviFileFormat == AVI_AUDIO_ONLY) {
        AviMp3DecodeStart();//2005-12-22 Test    
    } else if (AviStreamInfo.AviFileFormat == AVI_VIDEO_ONLY) {
        AviMp4DecodeStart();
    } else {
        AviMp3DecodeStart();//2005-12-22 Test    
        AviMp4DecodeStart();
    }
    
    PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_PLAY,0,0,0/*,0,0,0*/);
    if (AviPlayState == AVI_DECODE_PAUSE) {
        //PostDisplayMessate();
        //当AVI文件播放暂停时，显示AVI文件信息，如：
        //1、播放时间
        //2、播放状态 Play State
        //3、文件序号
        bitclear_creg(%imask,5);
        SystemInfo.bc.bBackLightOffEn = 0;
        LightOffCounter = 0;
        SystemInfo.bc.bSysIdleEn = 1;
        SysIdleCounter = 0;
        bitset_creg(%imask,5);
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviDecodeEnd(void)
{
    AviMp4DecodeEnd();
    AviMp3DecodeEnd();//2005-12-22 Test    
    
    FileClose(hVideoIndex);
    FileClose(hAudioIndex);
    FileClose(hSlaveFile);
    FileClose(hMainFile);
    
    Sche1MsgArray[0] = MSG_SCHEDULER1_NONE;
    PreviousScheduler1Msg = MSG_SCHEDULER1_NONE;
    Sys1InitFlag.bc.bAviModuleInitEnable = 1;
    
    if (SysFile1Info.CurrentFileNum < SysFile1Info.TotalFiles) {
        PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_AVI_FILE,MSG_FIND_FILE_NEXT,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
    } else {
        AviWaitForDmaDone();
        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        Fun1ModuleChange();
        uiAviStatusBarDispFlag = AVI_STATUS_BAR_NULL;
        uiAviStatusBarDispBits = 0;
        uiAviLcdDmaDoneFlag    = 0;
        BrowserDirInit();
        DisplayFlag = DISP_BROWSER;
        PreDisplayFlag = DisplayFlag;
        PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,FIND_FILE_TYPE_AVI,0,0/*,0,0,0*/);
        SysCpuSetFreq(51);
        SysTimer0Set(51,25000);
        LCD_Init(0);
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviDecodePlay(void)
{
    //if (AviPlayState == AVI_DECODE_PLAY) {
    AviMp4DecodePlay();
    //} else {
        
    //}
}

//********************************************************************************
