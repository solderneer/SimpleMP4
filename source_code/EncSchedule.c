/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : EncSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#define _IN_ENCSCHEDULE

#include <creg.h>
#include "Memap.h"
#include "MsgDef.h"
#include "Macro.h"
#include "Global.h"
#include "Resource.h"
#include "EncMacro.h"
#include "EncGlobal.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "Lcd.h"

#include "LcdChar.h"

#include "Dir.h"

void EncodeSysInit(void);
void EncodeInitial();
void EncodeVariableInit(void);
void EncodeLcdInit(void);
void LineInEncodeCPUInit(void);
void LineInEncodeCodecInit(void);
void MicRecEncodeCPUInit(void);
void MicRecEncodeCodecInit(void);
void EncKeyProc(void);
void EncFunKey(void);
void EncodeSelKey(void);

#if(FLASHROM_TYPE == _MLC_)
int  LINE_IN_MSADPCM_BUFFER[2048] __attribute__((section(".enc_bss, \"aw\"")));
#endif
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncModuleProc(void)
{    
    unsigned long nfa_tmp;
    
    switch(Fun1MsgArray[1])
    {
    case MSG_FUNCTION1_ENC_SELINIT:
    {
        EncodeSysInit();
        
        MusicSetFreq51();
        DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, (RECORD_SEL_PIC_LINE + (unsigned long)(1 - EncPlayInfo.Mode) * MAX_PIC_SIZE));
        PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_SEL,0,0,0/*,0,0,0*/);
        break;
    }
    case MSG_FUNCTION1_ENC_SEL:
    {
        break;
    }
    case MSG_FUNCTION1_ENC_INIT:
    {
        EncodeSysInit();
        EncodeInitial();
        break;
    }
    case MSG_FUNCTION1_ENC_HEAD:
    {
        EncodeHead();
        break;
    }
    case MSG_FUNCTION1_ENC_FRAG:
    {
        PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_NONE,0,0,0/*,0,0,0*/);
        break;
    }
    case MSG_FUNCTION1_ENC_START:
    {
        EncodeStart();
        break;
    }
    case MSG_FUNCTION1_ENC_PLAY:
#if(FLASHROM_TYPE == _MLC_)
//写数据到flash，编码在中断中处理
    if(MLCEncoderOutputBufferLeftData >= MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE)
    {
        if(FileWrite(&MLCEncoderOutputBuffer[MLCEncoderOutputBufferIndex * MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE],
            MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE,hMainFile)
             != MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE)
        {
            EncDispFlag |= ENC_DISPFLAG_STATUS;   //MusicDisplayBlock.DisplayFlag.bit.bMusicPlay_PlayState = 1;
            EncPlayStatus = MUSIC_ENCODE_END;
            PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_END, EncPlayInfo.Mode,0,0);
         }
         
         MLCEncoderOutputBufferIndex++ ;
         if(MLCEncoderOutputBufferIndex >= MLC_ENCODER_BUFFER_NUM) MLCEncoderOutputBufferIndex = 0;
         AVI_MP3_DECODE_DISABLE;
         MLCEncoderOutputBufferLeftData -= MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE;
         AVI_MP3_DECODE_ENABLE;
    }
#else
        EncodePlay();
#endif
        break;
    case MSG_FUNCTION1_ENC_END:
    {
        EncodeFlag.bc.bEncodeEnable = 0;
        EncodeEnd();
        break;
    }
    default:
        break;
    }
    
    if (DisplayFlag == DISP_ENCODE) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            EncKeyProc();
        } else {
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
            
            DisplayRecoveryTime = 0;
            PreDisplayFlag = DisplayFlag;
            DisplayFlag = DISP_HOLD;
            
            MusicSetFreq51();
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MAX_PIC_SIZE);
            EncDispFlag |= ENC_DISPFLAG_HOLD;
            MusicSetFreq51Return();
            return;
        }
            
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            EncDispFlag |= ENC_DISPFLAG_BATT;
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {           //add by zyz
            SystemInfo.bc.bDisplayRecovery = 0;
            EncDispFlag = ENC_DISPFLAG_ALL;
        }
            
        if ((EncDispFlag != 0) && (SystemInfo.bc.bBackLightStatus != 2)){  //Display Recovery
            if (Fun1MsgArray[1] >= MSG_FUNCTION1_ENC_INIT) {
                DISP_Record();
            }
        }
        
        if (SystemInfo.bc.bBackLightOnStart1) {             //Back Light On 
            SystemInfo.bc.bBackLightOnStart1 = 0;
            DmaFillImage2Lcd();
            LcdBackLightOn();
            KeyReset();
        }
    }
}
__attribute__((section(".enc_text,\"ax\"")))
void EncPlay()
{
#if(FLASHROM_TYPE == _MLC_)
    if(EncodeFlag.bc.bEncodeEnable)
    {
        EncodePlay();//录音编码
    }
#endif
}


/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncodeSysInit(void)
{
    if (Sys1InitFlag.bc.bEncodeModuleInitEnable) {
        Sys1InitFlag.word = 0xffff;
        Sys1InitFlag.bc.bEncodeModuleInitEnable = 0;
    }
    KeyReset();
    EncPlayInfo.Mode = Fun1MsgArray[2];
    EncPlayInfo.PlayState = MUSIC_ENCODE_READY;
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncodeInitial(void)
{
    EncPlayInfo.Mode = Fun1MsgArray[2];
    
    EncodeVariableInit();
    
    MemorySelect = 0;
    PrewMemorySelect = 0;
    MediaSetup(MemorySelect);
    
    if (EncPlayInfo.Mode == ENCODE_MIC_REC) {
        Sys1InitFlag.bc.bMicRecSysInitEnable = 0;
        
        MicRecEncodeCPUInit();
        MicRecEncodeCodecInit();
        
        MakeDir("\\", "VOICE");
    } else {
        Sys1InitFlag.bc.bLineInSysInitEnable = 0;
        
        LineInEncodeCPUInit();
        LineInEncodeCodecInit();
        
        if (EncPlayInfo.Mode == ENCODE_LINE_IN) {
            MakeDir("\\", "LINEIN");
        } else if (EncPlayInfo.Mode == ENCODE_FM_REC) {
            MakeDir("\\", "FM");
        }
    }
    
    PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_HEAD,0,0,0/*,0,0,0*/);
}

/*------------------------------------------------------------------------------
Function: Initial The Global Variable In The Line In Encode Module
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncodeVariableInit(void)
{
    EncodeFlag.bc.bFileCreat = 0;
    
    if (EncPlayInfo.Mode == ENCODE_MIC_REC){
        switch(GlobalEncVar.MicQuality) {
        case 0:
            EncPlayInfo.Bitrate = 64;
            EncPlayInfo.Freq = (unsigned long)16000;
            break;
            
        case 1:
        default:
            EncPlayInfo.Bitrate = 32;
            EncPlayInfo.Freq = (unsigned long)8000;
            break;
        }            
    } else {
#if(FLASHROM_TYPE == _MLC_)
        EncPlayInfo.Freq = (unsigned long)32000;
        EncPlayInfo.Bitrate = 256;
#else
        EncPlayInfo.Freq = (unsigned long)44100;
        EncPlayInfo.Bitrate = 128;
#endif
    }
    
    g_dwEncoderCurrTimeSecond = 0;
    g_dwEncoderRemainTimeSecond = 0x0036e070;       //999Hour
    EncPlayInfo.TimeGuageVal = 0;
    
    EncPlayInfo.PlayState = MUSIC_ENCODE_READY;
    EncPlayStatus = MUSIC_ENCODE_STOP;
    
#if(FLASHROM_TYPE == _MLC_)
    MicRecEncodeBuf = &LINE_IN_MSADPCM_BUFFER[512];
    MicRecHeadBuf = &LINE_IN_MSADPCM_BUFFER[0];
#else
    MicRecEncodeBuf = &EncodeInputBuffer[1024];
    MicRecHeadBuf = &EncodeInputBuffer[1024+ AD_PIPO_BUFFER_SIZE];
#endif    
    
    EncDispFlag = (ENC_DISPFLAG_ALL & (~ENC_DISPFLAG_PATH) & (~ENC_DISPFLAG_FILENAME));
    //LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
}

/*------------------------------------------------------------------------------
Function: CPU Initial In Line In Encode Module
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void LineInEncodeCPUInit(void)
{
    while(DmaFillImage2LcdFlag);
    
    bitclear_creg(%imask,15);
    
    SysCpuSetFreq(60);//48
    SysTimer0Set(60,25000);//SysTimer0Set(48,10000);
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0002);
    write_mem(CLOCK_I2SMCLKCON, 0x0100);    //I2SMclk = 12MHZ 
    
    write_mem32(DMAR_ClearBlock,0x00000007);
    
    write_creg(%ip0,0x0000);
    write_creg(%ip1,0x4800);
    write_creg(%ireq,0x0000);
    //bitset_creg(%imask,7);
    bitset_creg(%imask,15);
}

/*------------------------------------------------------------------------------
Function: CPU Initial In Mic Rec Encode Module
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void MicRecEncodeCPUInit(void)
{
    while(DmaFillImage2LcdFlag);
    
    bitclear_creg(%imask,15);
    
    /*现在使用边录音边整理FLASH碎片,CPU频率需要提高2006-8-28*/
    SysCpuSetFreq(36);
    SysTimer0Set(36,25000);//SysTimer0Set(24,10000);
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0002);
    write_mem(CLOCK_I2SMCLKCON, 0x0100);    //I2SMclk = 12MHZ 
    
    write_mem32(DMAR_ClearBlock,0x00000007);
    
    write_creg(%ip0,0x0000);
    write_creg(%ip1,0x4800);
    write_creg(%ireq,0x0000);
    //bitset_creg(%imask,7);
    bitset_creg(%imask,15);
}

/*------------------------------------------------------------------------------
Function: Codec Initial In Line In Encode Module
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void LineInEncodeCodecInit(void)
{
    CodecIicInit();
    
#ifndef  _INSIDE_HPSENSE_USED_    
    CodecCommandSet(P_ADD_TR1,  0x0002);
#else 
    CodecCommandSet(P_ADD_TR1,  0x0000);
#endif


    CodecCommandSet(P_ADD_CGR10, (MicInputVolume * 3) << 4 + (MicInputVolume * 3));
    
    if (EncPlayInfo.Mode == ENCODE_LINE_IN) {
        Pre_CR1_Value = 0x0024;
        CodecCommandSet(P_ADD_CR1,0x0024);                  //Bypass1
        CodecCommandSet(P_ADD_CR2,0x0004);                  //Line1 Input

//---------------------------------------------------------
#if(CPU_TYPE == _RK2608A_)
    CodecCommandSet(P_ADD_PMR1, 0x0007);    //All Power Down
#else
    CodecCommandSet(P_ADD_PMR1, 0x0027);    //All Power Down
#endif
//----------------------------------------------------------    
    } else {
        Pre_CR1_Value = 0x0022;
        CodecCommandSet(P_ADD_CR1,0x0022);                  //Bypass2
        CodecCommandSet(P_ADD_CR2,0x0005);                  //Line2 Input

//----------------------------------------------------------
#if(CPU_TYPE == _RK2608A_)
    CodecCommandSet(P_ADD_PMR1, 0x000b);    //All Power Down
#else
    CodecCommandSet(P_ADD_PMR1, 0x002b);    //All Power Down
#endif
//-----------------------------------------------------------------
    }
    CodecCommandSet(P_ADD_PMR2, 0x0000);    //All Power Down
   
#if(FLASHROM_TYPE == _MLC_)
    CodecCommandSet(P_ADD_CCR,0x0022);      //MusicPlayInfo.Freq = 32KHZ
#else
    CodecCommandSet(P_ADD_CCR,0x0011);      //MusicPlayInfo.Freq = 44.1KHZ
#endif
   
    Delay1Ms(100);
    OutputVolumeSet(OutputVolume);
   
#if(FLASHROM_TYPE == _MLC_)
    write_mem(I2DSP_RXCONF,0x00010);        //
#else
    write_mem(I2DSP_RXCONF,0x0008);         //Slave Mode, 16 bit, 44.1KHZ
#endif
    write_mem(I2DSP_RXCOM, 0x0003);                         //Tx enable,I2Dsp enable,DMA request Enable   
    write_mem(I2DSP_DPCTRL,0x0002);                         //Tx FIFO Reset
    CodecPowerOn();
}

/*------------------------------------------------------------------------------
Function: Codec Initial In Rec Encode Encode Module
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void MicRecEncodeCodecInit(void)
{
    CodecIicInit();
    
#ifndef  _INSIDE_HPSENSE_USED_    
    CodecCommandSet(P_ADD_TR1,  0x0002);
#else 
    CodecCommandSet(P_ADD_TR1,  0x0000);
#endif
    CodecCommandSet(P_ADD_CGR10,((MicInputVolume * 3) << 4) + (MicInputVolume * 3));    //ADC Gain
    CodecCommandSet(P_ADD_CR1,  0x0020);                    //SideTone No Sel
    Pre_CR1_Value = 0x0020;
    
    CodecCommandSet(P_ADD_CR2,  0x0006);                    //Mic Input

#if(CPU_TYPE == _RK2608A_)
    CodecCommandSet(P_ADD_PMR1, 0x000d);    //All Power Down
#else
    CodecCommandSet(P_ADD_PMR1, 0x002d);    //All Power Down
#endif
    CodecCommandSet(P_ADD_PMR2, 0x0000);    //All Power Down
    
    if (EncPlayInfo.Freq == (unsigned long)8000) {
        write_mem(I2DSP_RXCONF,0x0030);                     //8KHz
        CodecCommandSet(P_ADD_CCR,0x0066);                  //8KHz
    } else /*if (EncPlayInfo.Freq == (unsigned long)16000)*/ {
        write_mem(I2DSP_RXCONF,0x0020);                     //16KHz
        CodecCommandSet(P_ADD_CCR,0x0044);                  //16KHa
    }
    Delay1Ms(100);
    OutputVolumeSet(OutputVolume);
    
    write_mem(I2DSP_RXCOM, 0x0003);                         //Rx enable,I2Dsp enable,DMA request Enable   
    write_mem(I2DSP_DPCTRL,0x0002);                         //Rx FIFO Reset
    CodecPowerOn();
}

/*******************************************************************************

            Module Function: Mic Rec Initial

*******************************************************************************/
__attribute__((section(".enc_text,\"ax\"")))
void EncKeyProc(void)
{
    if(KeyFlag.bc.bKeyProssed == 0) return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
        
    KeyFlag.bc.bKeyProssed = 0;
    
    if (Fun1MsgArray[1] <= MSG_FUNCTION1_ENC_SEL) {
        EncodeSelKey();
    } else {
        EncFunKey();
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void ExitEncode(void)
{
    if((EncPlayInfo.PlayState == MUSIC_ENCODE_PLAY) || (EncPlayInfo.PlayState == MUSIC_ENCODE_PAUSE)){
        EncodeEnd();
    //-----------------------------------------------
    //修改文件格式不支持
        //GetRemapTbl();
        MediaSetup(MemorySelect);
        FileInit();
    //------------------------------------------------
    }
    bitclear_creg(%imask,5);
    SystemInfo.bc.bSysIdleEn = 1;
    SysIdleCounter = 0;
    bitset_creg(%imask,5);
}
__attribute__((section(".enc_text,\"ax\"")))
void EncFunKey(void)
{
    unsigned int temp1,temp2;
    
    switch (KeyVal) {
//-------------------- PLAY KEY ---------------------------
    /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_SHORT_UP:
    case KEY_VAL_PLAY_LONG_UP:*/
    case KEY_VAL_PLAY_PRESS:
    {
        if (KeyScanCnt >= 80/*200*/) {
            EncodeEnd();
            SysPowerDown();
        }
        break;
    }
//-------------------- REC KEY ----------------------------
    /*case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_LONG_UP:*/
    case KEY_VAL_REC_PRESS_START:                           //Stop
    {
        if(EncPlayInfo.PlayState != MUSIC_ENCODE_READY) {
            PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_END,EncPlayInfo.Mode,0,0/*,0,0,0*/);
        }
        break;
    }
    case KEY_VAL_REC_SHORT_UP:
    {
        EncDispFlag |= ENC_DISPFLAG_STATUS;
         
        if(EncPlayInfo.PlayState == MUSIC_ENCODE_READY) {   //Start
            EncPlayInfo.PlayState = MUSIC_ENCODE_PLAY;
            EncPlayStatus = MUSIC_ENCODE_PLAY;
            
            CodecPowerOn();
            EncodeFlag.bc.bEncodeEnable = 1;
            EncodeFlag.bc.bPcmBufFull = 0;
            
            bitclear_creg(%imask,5);
            SystemInfo.bc.bSysIdleEn = 0;
            bitset_creg(%imask,5);
            
            PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_START,EncPlayInfo.Mode,0,0/*,0,0,0*/);
            
        } else if (EncPlayInfo.PlayState == MUSIC_ENCODE_PLAY) {//Pause
            EncPlayInfo.PlayState = MUSIC_ENCODE_PAUSE;
            EncPlayStatus = MUSIC_ENCODE_PAUSE;
            
            EncodeFlag.bc.bEncodeEnable = 0;
            bitclear_creg(%imask,5);
            SystemInfo.bc.bSysIdleEn = 1;
            SysIdleCounter = 0;
            bitset_creg(%imask,5);
            
        } else if (EncPlayInfo.PlayState == MUSIC_ENCODE_PAUSE) {//Play
            EncPlayInfo.PlayState = MUSIC_ENCODE_PLAY;
            EncPlayStatus = MUSIC_ENCODE_PLAY;
            
            CodecPowerOn();
            EncodeFlag.bc.bEncodeEnable = 1;
            EncodeFlag.bc.bTheFirstPcmData = 1;
            
            bitclear_creg(%imask,5);
            SystemInfo.bc.bSysIdleEn = 0;
            bitset_creg(%imask,5);
        } 
         break;
    }
//-------------------- MENU KEY ----------------------------
    /*case KEY_VAL_MENU_DOWN:
    case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_SHORT_UP:
    case KEY_VAL_MENU_LONG_UP:*/
    case KEY_VAL_MENU_PRESS_START:
    {
//      if((EncPlayInfo.PlayState == MUSIC_ENCODE_PLAY) || (EncPlayInfo.PlayState == MUSIC_ENCODE_PAUSE)){
//        //if(EncPlayInfo.PlayState != MUSIC_ENCODE_READY) {
//            EncodeEnd();
//        }
        /*由于在Fun1ModuleChange()调用了EncodeEnd()，
          而EncodeEnd()函数里面对Fun1MsgArray[]有操作，会发送新的消息
          所以在调用Fun1ModuleChange()之后应该执行Fun1MsgArray[0] = MSG_FUNCTION1_NONE;*/
        Fun1ModuleChange();
        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        
        if(EncPlayInfo.Mode == ENCODE_FM_REC){
            DisplayFlag = DISP_FM;
            PreDisplayFlag = DisplayFlag;
            
            PostSche1Message(MSG_SCHEDULER1_FM);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_INIT,0,0,0);
            KeyReset();
        }
        else{
            MainMenuState = 0;
            DisplayFlag = DISP_MENU;
            PreDisplayFlag = DisplayFlag;
            
            SysCpuSetFreq(51);
            SysTimer0Set(51,25000);
        }
        break;
    }
//-------------------- UP KEY ----------------------------
  /*case KEY_VAL_UP_PRESS_START:
    case KEY_VAL_UP_SHORT_UP:
    case KEY_VAL_UP_LONG_UP:*/
    case KEY_VAL_UP_DOWN:
    case KEY_VAL_UP_PRESS:
    {
        OutputVolumeInc();
        break;
    }
//-------------------- DOWN KEY ----------------------------
  /*case KEY_VAL_DOWN_PRESS_START:
    case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_LONG_UP:*/
    case KEY_VAL_DOWN_DOWN:
    case KEY_VAL_DOWN_PRESS:
    {
        OutputVolumeDec();
        break;
    }
    default:
        break;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncodeSelKey(void)
{
    switch (KeyVal){
//-------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_SHORT_UP:
    {
        PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_INIT,EncPlayInfo.Mode,0,0);
        break;
    }
    case KEY_VAL_MENU_PRESS_START:
    {
        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        Fun1ModuleChange();
        
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        break;
    }
//--------------------- FFW KEY ---------------------------
//--------------------- FFD KEY ---------------------------
    case KEY_VAL_FFW_DOWN:
    case KEY_VAL_FFD_DOWN:
    {
        EncPlayInfo.Mode = 1 - EncPlayInfo.Mode;
        DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, (RECORD_SEL_PIC_LINE + (unsigned long)(1 - EncPlayInfo.Mode) * MAX_PIC_SIZE));
        break;
    }
        
    default:
        break;
    }
}
//******************************************************************************
