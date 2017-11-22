/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FmSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/11       1.0            ORG
********************************************************************************
*/


#include <Creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "Lcd.h"

#ifdef FM_MODULE

    #define _IN_FMSCHEDULE
    #include "FmMacro.h"
    #include "FmStruct.h"
    #include "FmGlobal.h"
    #include "tuner_drv.h"




void FmCPUInit(void);
void FmModuleInitial(void);
void FmPlayStart(void);
void FmPlay(void);
void FmVariableInit(void);
void FmCodecInit(void);

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmTimerDec(void)
{
    if (FmTimerCount != 0) {
        FmTimerCount -- ;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmModuleProc(void)
{
    unsigned int i, j;
    
    switch(Fun1MsgArray[1])
    {
    case MSG_FUNCTION1_FM_INIT:             //Fm Cpu / Variable / Codec Init
        FmModuleInitial();
        break;
        
    case MSG_FUNCTION1_FM_START:
        FmPlayStart();                      //恢复上次电台设置
        break;
        
    case MSG_FUNCTION1_FM_PLAY:             //收音机调台处理
        FmPlay();
        break;
        
    case MSG_FUNCTION1_FM_MENU:
        break;

    case MSG_FUNCTION1_FM_DEL:
    case MSG_FUNCTION1_FM_SAVE:
        //DelFmFunc();
        break;

    default:
        break;
    }
    
    //从设置菜单返回到FM时需要做的处理
    //1、改变频段，检查当前电台及保存的电台是否超越边界
    //2、修改立体声状态，重新启动PLL时会做立体声的检测，所以这里不需要处理，只发送消息出去即可
    if (SystemInfo.bc.bMusicMenuSet == 1) {     
        SystemInfo.bc.bMusicMenuSet = 0;
        
        //Tuner_StereoControl();
        if (GlobalFmVar.FmArea == FM_AREA_JAPAN) {
            FmFreqMaxVal = 9000;
            FmFreqMinVal = 7600;
        } else {
            FmFreqMaxVal = 10800;
            FmFreqMinVal = 8700;
        }
        j = 0;
        for (i = 0; i < FREQMAXNUMBLE; i++) {
            if ((GlobalFmVar.FmFreqArray[i] < FmFreqMinVal) || (GlobalFmVar.FmFreqArray[i] > FmFreqMaxVal)) {
                GlobalFmVar.FmFreqArray[i] = 0;
                j ++ ;
            }
        }
        if ((GlobalFmVar.FmFreq < FmFreqMinVal) || (GlobalFmVar.FmFreq > FmFreqMaxVal)) {
            GlobalFmVar.FmFreq = FmFreqMinVal;
        }
        FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
        FmGuageX = FmGuageVal;
        
        PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0,0,0,0);
    }
    
    if (DisplayFlag == DISP_FM) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            FmKeyProc();
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
            
            MusicSetFreq51();         //add by zyz
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MAX_PIC_SIZE);
            MusicSetFreq51Return();
            return;
        }
            
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            FmDisplayFlag |= FM_DISPFLAG_BATT;
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {           //add by zyz
            SystemInfo.bc.bDisplayRecovery = 0;
            if (Fun1MsgArray[1] != MSG_FUNCTION1_FM_PLAY) Fun1MsgArray[1] = MSG_FUNCTION1_FM_PLAY;
            
            FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                             FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                             FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
        }
            
        if ((FmDisplayFlag != FM_DISPFLAG_NULL) && (SystemInfo.bc.bBackLightStatus != 2)) {
            if (Fun1MsgArray[1] == MSG_FUNCTION1_FM_MENU) {
                DisplayFmMenu();
            } else if ((Fun1MsgArray[1] == MSG_FUNCTION1_FM_SAVE) || (Fun1MsgArray[1] == MSG_FUNCTION1_FM_DEL)) {
                DisplayFmList();
//            } else if (Fun1MsgArray[1] == MSG_FUNCTION1_FM_DIALOG) {
//                DisplayFmDialog();
            } else {
                DISP_Fm();
            }
        }
        
        if (SystemInfo.bc.bBackLightOnStart1) {
            SystemInfo.bc.bBackLightOnStart1 = 0;
            DmaFillImage2Lcd();
            LcdBackLightOn();
            KeyReset();
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmModuleInitial(void)
{
    if (Sys1InitFlag.bc.bFmSysInitEnable) {
        FmCPUInit();
        LCD_Init(0);
        AmplifierOff();
        FmCodecInit();
        FmVariableInit();
        PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0,0,0,0);
    } else {
        FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                         FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                         FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
        FmStereoCount = 0;
        FmStereoCount1 = 0;
        PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_READ_ST,0,0,0,0,0);
    }
    KeyReset();
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmCPUInit(void)
{
    unsigned int temp;
    
    bitclear_creg(%imask,15);
    
//    SysCpuSetFreq(SYS_IDLE_FREQ);
//    SysTimer0Set(SYS_IDLE_FREQ,25000);
    SysCpuSetFreq(51);
    SysTimer0Set(51,25000);
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0002);
    
    write_creg(%ip0,0x0000);
    write_creg(%ip1,0x0000);
    write_creg(%ireq,0x0000);
    bitset_creg(%imask,15);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmVariableInit(void)
{
    unsigned int i,j;
    
    Sys1InitFlag.word = 0xffff;
    Sys1InitFlag.bc.bFmSysInitEnable = 0;
    Sys1InitFlag.bc.bFmModuleInitEnable = 0;
    
    SystemInfo.bc.bPowerOffDisable = 1;
    
    FmSensitive = 0;
    FmStereoState = 0;
    FmStereoCount = 0;
    FmStereoCount1 = 0;
                
    if (GlobalFmVar.FmArea == FM_AREA_JAPAN) {
        FmFreqMaxVal = 9000;
        FmFreqMinVal = 7600;
    } else {
        FmFreqMaxVal = 10800;
        FmFreqMinVal = 8700;
    }
    if ((GlobalFmVar.FmFreq < FmFreqMinVal) || (GlobalFmVar.FmFreq > FmFreqMaxVal))
        GlobalFmVar.FmFreq = FmFreqMinVal;
        
    j = 0;
    for (i = 0; i < FREQMAXNUMBLE; i++) {
        if ((GlobalFmVar.FmFreqArray[i] < FmFreqMinVal) || (GlobalFmVar.FmFreqArray[i] > FmFreqMaxVal)) {
            GlobalFmVar.FmFreqArray[i] = 0;
            j ++ ;
        }
    }
    if (j >= FREQMAXNUMBLE -1 ) GlobalFmVar.FmSaveNum = 0;
    
    FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
    FmGuageX = FmGuageVal;
    FmDisplayFlag = (FM_DISPFLAG_NULL | FM_DISPFLAG_BACKGROUND | FM_DISPFLAG_BATT | 
                         FM_DISPFLAG_CH | FM_DISPFLAG_FREQ | FM_DISPFLAG_GUAGE | FM_DISPFLAG_REGION |
                         FM_DISPFLAG_VOL | FM_DISPFLAG_SENS | FM_DISPFLAG_STEREO);
    
    if (GlobalFmVar.FmState != FM_STATE_PRESET) {
        GlobalFmVar.FmState = FM_STATE_MANUAL;
    }
    FmFunctionSele = GlobalFmVar.FmState;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmCodecInit(void)
{
    CodecIicInit();
#ifndef  _INSIDE_HPSENSE_USED_    
    CodecCommandSet(P_ADD_TR1,  0x0002);
#else 
    CodecCommandSet(P_ADD_TR1,  0x0000);
#endif
    CodecCommandSet(P_ADD_CR1,  0x0002);    //FM Bypass2
    Pre_CR1_Value = 0x0002;                 //2006-5-11 20:31
    CodecCommandSet(P_ADD_CR2,  0x0003);    //Mixer Output
#if(CPU_TYPE == _RK2608A_)
    CodecCommandSet(P_ADD_PMR1, 0x001b);    //All Power Down
#else
    CodecCommandSet(P_ADD_PMR1, 0x003b);    //All Power Down
#endif
    CodecCommandSet(P_ADD_PMR2, 0x0008);    //All Power Down
    
//    OutputVolumeSet(0);
//    Delay1Ms(100);    
    CodecPowerOn();
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmPlayStart(void)
{
    bitclear_creg(%imask,5);
    FmTimerCount = 5;
    bitset_creg(%imask,5);
    Tuner_PLLCount(GlobalFmVar.FmFreq/*9870*/,1);          //Send Freq To TEA5767
    PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_MUTE_OFF,0,0,0,0,0);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".fm_text,\"ax\"")))
void FmPlay(void)
{
    int temp;
    
    switch (Fun1MsgArray[2]) 
    {
        case MSG_FUNCTION1_FM_SEND_FREQHSI:
        {
            if (FmTimerCount > 0) break;
            
            bitclear_creg(%imask,5);
            FmTimerCount = 2;
            bitset_creg(%imask,5);
            Tuner_FreqChange(UpDownFlag,TUNERFREQSTEP);
            Tuner_PLLCount(GlobalFmVar.FmFreq,1);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_SEND_FREQLSI,0,0,0,0,0);

            if (((UpDownFlag == TUN_UP) && (GlobalFmVar.FmFreq == FmFreqMinVal)) ||
                ((UpDownFlag == TUN_DOWN) && (GlobalFmVar.FmFreq == FmFreqMaxVal))) {
                
                bitclear_creg(%imask,5);
                FmTimerCount = 4;
                bitset_creg(%imask,5);
                if(GlobalFmVar.FmState == FM_STATE_AUTO) {
                    GlobalFmVar.FmState = FM_STATE_PRESET;
                    
                    bitclear_creg(%imask,5);
                    FmTimerCount = 20;
                    bitset_creg(%imask,5);
                    Tuner_MuteControl(1);
                    AmplifierOff();
                    GlobalFmVar.FmSaveNum = 0;
                    GlobalFmVar.FmFreq = GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum];
                    if((GlobalFmVar.FmFreq < FmFreqMinVal) || (GlobalFmVar.FmFreq > FmFreqMaxVal)){
                        GlobalFmVar.FmFreq = FmFreqMinVal;   
                    }
                    
                    FmDisplayFlag |= (FM_DISPFLAG_GUAGE | FM_DISPFLAG_FREQ | FM_DISPFLAG_CH);
                    PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0,0,0,0);
                }
            }
            
            FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
            FmDisplayFlag |= (FM_DISPFLAG_GUAGE | FM_DISPFLAG_FREQ);
            break;
        }    
        case MSG_FUNCTION1_FM_SEND_FREQLSI:
        {
            if (FmTimerCount > 0) break;
            
            bitclear_creg(%imask,5);
            FmTimerCount = 2;
            bitset_creg(%imask,5);
            Tuner_ReadData(IFReadDataH, 5);
            Tuner_PLLCount(GlobalFmVar.FmFreq,0);
            
            if (((UpDownFlag == TUN_UP) && (GlobalFmVar.FmFreq == FmFreqMinVal)) ||
                ((UpDownFlag == TUN_DOWN) && (GlobalFmVar.FmFreq == FmFreqMaxVal)))
                bitclear_creg(%imask,5);
                FmTimerCount = 4;
                bitset_creg(%imask,5);
                
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_READ_IF,0,0,0,0,0);
            break;
        }   
        case MSG_FUNCTION1_FM_READ_IF:
        {    
            if (FmTimerCount > 0) break;
                
            Tuner_ReadData(IFReadDataL, 5);
            
            FmSensitive = 0;
            if ((IFReadDataH[3] >= 0x20) && (IFReadDataL[3] >= 0x20)) {
                FmSensitive = 1;
            } else if ((IFReadDataH[3] >= 0x40) && (IFReadDataL[3] >= 0x40)) {
                FmSensitive = 2;
            } else if ((IFReadDataH[3] >= 0x80) && (IFReadDataL[3] >= 0x80)) {
                FmSensitive = 3;
            }
            FmDisplayFlag |= FM_DISPFLAG_SENS;
            if ((IFReadDataH[3] >= 0x40) && (IFReadDataL[3] >= 0x40)) {         // 调节停电灵敏度
                temp = ((IFReadDataH[3] >> 4) & 0x000f)-((IFReadDataL[3] >> 4) & 0x000f);
                if ((-2 <= temp) && (temp <= 2)) {
                    if (((0x31 < (IFReadDataL[2] & 0x007f)) && ((IFReadDataL[2] & 0x007f) < 0x3e)) &&
                        ((0x31 < (IFReadDataH[2] & 0x007f)) && ((IFReadDataH[2] & 0x007f) < 0x3e))) {
                        if (GlobalFmVar.FmState == FM_STATE_AUTO) {             //自动扫描搜索到电台并保存电台
                            FmDisplayFlag |= FM_DISPFLAG_CH;
                            GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum] = GlobalFmVar.FmFreq;
                            
                            if ((GlobalFmVar.FmFreq == FmFreqMaxVal) || (++GlobalFmVar.FmSaveNum >= FREQMAXNUMBLE)) {
                                GlobalFmVar.FmState = FM_STATE_PRESET;
                                
                                bitclear_creg(%imask,5);
                                FmTimerCount = 20;
                                bitset_creg(%imask,5);
                                Tuner_MuteControl(1);
                                AmplifierOff();
                                GlobalFmVar.FmSaveNum = 0;
                                GlobalFmVar.FmFreq = GlobalFmVar.FmFreqArray[GlobalFmVar.FmSaveNum];
                                if((GlobalFmVar.FmFreq < FmFreqMinVal) || (GlobalFmVar.FmFreq > FmFreqMaxVal)){
                                    GlobalFmVar.FmFreq = FmFreqMinVal;
                                }
                                FmGuageVal = (unsigned int)((unsigned long)(FM_GUAGE_CONST) * (GlobalFmVar.FmFreq - FmFreqMinVal) / (FmFreqMaxVal - FmFreqMinVal));
                                FmDisplayFlag |= (FM_DISPFLAG_GUAGE | FM_DISPFLAG_FREQ | FM_DISPFLAG_CH);
                                PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_START,0,0,0,0,0,0);
                                break;
                            }
                            /*else*/ {
                            bitclear_creg(%imask,5);
                            FmTimerCount = 5;
                            bitset_creg(%imask,5);
                            UpDownFlag = TUN_UP;
                            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_SEND_FREQHSI,0,0,0,0,0);
                            break;
                            }
                        }
                        /*else*/ {
                        bitclear_creg(%imask,5);
                        FmTimerCount = 10;
                        bitset_creg(%imask,5);
                        PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_MUTE_OFF,0,0,0,0,0);
                        break;
                        }
                    }
                }
            }
            bitclear_creg(%imask,5);
            FmTimerCount = 4;
            bitset_creg(%imask,5);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_SEND_FREQHSI,0,0,0,0,0);
            
            break;
        }
        /*
        case MSG_FUNCTION1_FM_MUTE_ON:
            break;
        */
        case MSG_FUNCTION1_FM_MUTE_OFF:
        {
            if (FmTimerCount > 0) break;
            
            FmStereoCount = 0;
            FmStereoCount1 = 0;
            Tuner_MuteControl(FALSE);
            AmplifierOn();
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_PLAY,MSG_FUNCTION1_FM_READ_ST,0,0,0,0,0);
            break;
        }
        case MSG_FUNCTION1_FM_READ_ST:
        {
            if (FmTimerCount > 0) break;
            
            if (FmStereoCount1 < 32) {
                FmStereoCount1 ++ ;
                bitclear_creg(%imask,5);
                FmTimerCount = 2;
                bitset_creg(%imask,5);
                GetStereoStatus();
                if (FmStereoState == 1) {
                    FmStereoCount++;
                } else {
                    FmStereoCount = 0;
                }
            } else if (FmStereoCount1 == 32) {
                FmStereoCount1 ++ ;
                if(FmStereoCount > 20) {
                    FmStereoState = 1;
                } else {
                    FmStereoState = 0;
                }
                FmDisplayFlag |= (FM_DISPFLAG_STEREO | FM_DISPFLAG_SENS);
            }
            break;
        }
        default:
            break;
    }
}

#endif /* 如果没有定义 FM 模块代码不进行编译*/
//******************************************************************************
