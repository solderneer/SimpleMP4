/*
********************************************************************************
*          Copyright (C), 2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Timer.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "Global.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"

#include "BatteryModule.h"

void TimerModule(void);
void TimerCfgFunc(unsigned int n, void (*func)(void *), void *arg);
void TimerInit(void);
void TimerReset(unsigned int n);
void TimerSetTimer(unsigned int n, unsigned int TmrValue);
void TimerStart(unsigned int n);
void TimerStop(unsigned int n);
int TimerGetState(unsigned int n);
void ScrollStrTmrFunc(void *parg);

/*------------------------------------------------------------------------------
Function: 
Input   :
Output  :
------------------------------------------------------------------------------*/
void RanDomCountInc(void)
{
    GlobalRanDomNum ++ ;
}

/*------------------------------------------------------------------------------
Function: ��ʱ�������
Input   :
Output  :
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void SysTimerModule(void *parg)
{
    //unsigned int BackLightTable[6] = {5*100,10*100,15*100,20*100,30*100,0};
    unsigned int BackLightTable[6] = {5*40,10*40,15*40,20*40,30*40,0};
    
//------------------------------------------------------------------------------    
    SystemInfo.bc.bTimer0Int = 1;
//------------------------------------------------------------------------------    
    TimerReset(SYSTEM_TIMER_CHANNEL);
    TimerStart(SYSTEM_TIMER_CHANNEL);
//------------------------------------------------------------------------------    
//------------------------------------------------------------------------------
    RanDomCountInc();                       //�����������
//------------------------------------------------------------------------------
    IncBatteryDetectCounter();              //���״̬��ʾ���������
//------------------------------------------------------------------------------
    AdcDataSample();                        //��ص�������
//------------------------------------------------------------------------------
    if (DisplayRecoveryTime < 80/*200*/) {  //��ʾͼƬ�ָ��ȴ�ʱ��
        DisplayRecoveryTime ++ ;
    }
//------------------------------------------------------------------------------
                                            //�رձ���ʱ�����
    if ((LightOffTime < 5) && (SystemInfo.bc.bBackLightStatus == 0) && (SystemInfo.bc.bBackLightOffEn == 0)) {
        LightOffCounter++;
        if (LightOffCounter > BackLightTable[LightOffTime]) SystemInfo.bc.bBackLightOffStart = 1;
    }
//------------------------------------------------------------------------------
                                            //ϵͳ����IDLE�ĵȴ�ʱ��
    if ((SystemInfo.bc.bSysIdleEn) && (SysIdleCounter < 400/*1000*/) ) {
        SysIdleCounter ++ ;
    }
//------------------------------------------------------------------------------
                                            //��ʱ�Զ��ػ�ʱ��
    if (((0 < PowerOffTime) && (PowerOffTime < 6)) || (SystemInfo.bc.bSysIdleStatus)) {
        PowerOffCounter ++;
    }
//------------------------------------------------------------------------------
}

/*------------------------------------------------------------------------------
Function: 
Input   : 
Output  : 
------------------------------------------------------------------------------*/
SYSTEM_TEXT_SECTION
void TimerProc(void)
{
    unsigned int ImaskTemp, TempDispMode;
    unsigned int PowerOffTimeTable[6] = {0,10,15,30,60,120};
    
    if (SystemInfo.bc.bTimer0Int) {
        read_creg(%imask,ImaskTemp);
        bitclear_creg(%imask,5);
        SystemInfo.bc.bTimer0Int = 0;
//------------------------------------------------------------------------------
        if ((SystemInfo.bc.bSysIdleStatus) && (SystemInfo.bc.bPowerOffDisable == 0)){
            if (PowerOffCounter >= 10 * 60 * 40/*100*/) {
                SysPowerDown();
            }
        }
        if ((PowerOffTime > 0) && (PowerOffTime < 6)) {
            if (PowerOffCounter >= ((unsigned long)(PowerOffTimeTable[PowerOffTime])* 60 * 40/*100*/)) {
                SysPowerDown();
            }
        }
//------------------------------------------------------------------------------
        if ((KeyVal == KEY_VAL_PLAY_PRESS) && (KeyScanCnt > 80/*200*/)) {
            if ((Sys1InitFlag.bc.bEncodeModuleInitEnable) && (SystemInfo.bc.bHoldOn == 0)) {
                SysPowerDown();
            }
        }
//------------------------------------------------------------------------------
        if ((KeyVal == KEY_VAL_HOLD_PRESS) && (KeyScanCnt > 40/*100*/)) {
            KeyReset();
            SystemInfo.bc.bHoldDisplay = 1;         //��ʾHoldͼƬ���־
            if (SystemInfo.bc.bHoldOn) {
                SystemInfo.bc.bHoldOn = 0;
                DisplayFlag = PreDisplayFlag;
                SystemInfo.bc.bDisplayRecovery = 1;     //�ָ���Ļ��ʾ
                DisplayRecoveryTime = 120/*300*/;
            } else {
                SystemInfo.bc.bHoldOn = 1;
                DisplayRecoveryTime = 0;
            }
        }
//------------------------------------------------------------------------------
        if (DisplayRecoveryTime == 80/*200*/) {
            DisplayRecoveryTime = 120;//300;
            SystemInfo.bc.bDisplayRecovery = 1;
            DisplayFlag = PreDisplayFlag;
        }
//------------------------------------------------------------------------------
        if (SysIdleCounter == 400/*1000*/) {
            SysIdleCounter = 440;//1100;
            PowerOffCounter = 0;
            
            PrevCpuFreq = CurrCpuFreq;
            if (PrevCpuFreq < 24) PrevCpuFreq = 24;
            SysCpuSetFreq(SYS_IDLE_FREQ);
            
            SystemInfo.bc.bSysIdleStatus = 1;
            PrevTimer0Val = Timer0Val;
            SysTimer0Set(SYS_IDLE_FREQ,25000);
            
            if ((USB_INSERT()) && (SystemInfo.bc.bBackLightStatus == 0) && (LightOffTime == 5)) {
//                TempDispMode = DisplayMode;
//                DisplayMode = 0;
                LcdBackLightOff();
//                DisplayMode = TempDispMode;
            }
        }
//------------------------------------------------------------------------------
        write_creg(%imask,ImaskTemp);
    }
}
//******************************************************************************

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : TimerModule()
*  Author:         ZhenFu Fang
*  Description:    ��ʱ������ģ�顡
*  Calls:          
*  Input:          ��
*  Return:         ��
*  Remark:         1��Ӧ�Ѵ˺����������ں�����
*               �� 2���������ڴ�ģ���з���ִ��ʱ��ϳ�������              
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/11/09      1.0           ORG
*
***********************************************************
*/



/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : TimerInit()
*  Author:         ZhenFu Fang
*  Description:    �����ʱ����ʼ��
*  Calls:          
*  Input:          ��
*  Return:         ��
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/11/09      1.0           ORG
*
***********************************************************
*/



TIMER_BSS_SECTION
TMR TmrCtr;



TIMER_CODE_SECTION
void TimerInit(void)
{
    unsigned int i;
    for(i=0; i<TIMER_MAX_CHANNEL; i++)
    {
        TmrCtr.TmrTbl[i].TimeCounter   = 0;
        TmrCtr.TmrTbl[i].TimeInitValue = 0;
        TmrCtr.TmrTbl[i].TimeFunc      = 0;
        TmrCtr.TmrTbl[i].FuncArg       = 0;
    }    
    TmrCtr.TmrFlag = 0;
    TimerCfgFunc(SYSTEM_TIMER_CHANNEL, SysTimerModule, 0);
    TimerSetTimer(SYSTEM_TIMER_CHANNEL, 1);
    TimerStart(SYSTEM_TIMER_CHANNEL);
    
    TimerCfgFunc(SCROLL_TIMER_CHANNEL, ScrollStrTmrFunc, &ScrollString.flag);
    TimerSetTimer(SCROLL_TIMER_CHANNEL, 2);
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : TimerStart()
*  Author:         ZhenFu Fang
*  Description:    ��ʱ������
*  Calls:          
*  Input:          n ��ʱ��ͨ��
*  Return:         ��
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/11/09      1.0           ORG
*
***********************************************************
*/
TIMER_CODE_SECTION
void TimerStart(unsigned int n)
{
    unsigned int imask_tmp;
    if(n < TIMER_MAX_CHANNEL)
    {
        TIMER_DISABLE_INT;
        TmrCtr.TmrFlag |= (0x0001<<(n));
        TIMER_ENABLE_INT;
    }
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : TimerStop()
*  Author:         ZhenFu Fang
*  Description:    ��ʱ��ֹͣ
*  Calls:          
*  Input:          n ��ʱ��ͨ��
*  Return:         ��
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/11/09      1.0           ORG
*
***********************************************************
*/
TIMER_CODE_SECTION
void TimerStop(unsigned int n)
{
    unsigned int imask_tmp;
    
   if(n < TIMER_MAX_CHANNEL)
    {
        TIMER_DISABLE_INT;
        TmrCtr.TmrFlag &= (~(0x0001<<(n)));
        TIMER_ENABLE_INT;
    } 
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : TimerReset()
*  Author:         ZhenFu Fang
*  Description:    ��ʱ������
*  Calls:          
*  Input:          n ��ʱ��ͨ��
*  Return:         ��
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/11/09      1.0           ORG
*
***********************************************************
*/
TIMER_CODE_SECTION
void TimerReset(unsigned int n)
{
    unsigned int imask_tmp;
    
    if(n < TIMER_MAX_CHANNEL){
        TIMER_DISABLE_INT;
        TmrCtr.TmrTbl[n].TimeCounter = TmrCtr.TmrTbl[n].TimeInitValue;
        TIMER_ENABLE_INT;
    }
}

TIMER_CODE_SECTION
int TimerGetState(unsigned int n)
{
    unsigned int imask_tmp;
    
    TIMER_DISABLE_INT;
    if(TmrCtr.TmrFlag &(0x0001<<(n))){
        TIMER_ENABLE_INT;
        return 1;
    }
    TIMER_ENABLE_INT;
    return 0;
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : TimerCfgFunc()
*  Author:         ZhenFu Fang
*  Description:    ��ʱ�������á�
*  Calls:          
*  Input:          n    ��ʱ��ͨ��
*                  func ��ʱ����ָ��
*                  arg  ��ʱ�������
*  Return:         ��
*  Remark:         ��ʱ����ĺ���Ӧ��������ʽʵ��     
*                  void TimerFunction(void *arg)     
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/11/09      1.0           ORG
*
***********************************************************
*/
TIMER_CODE_SECTION
void TimerCfgFunc(unsigned int n, void (*func)(void *), void *arg)
{
    unsigned int imask_tmp;
    TimeStruct *pTmr;
    
    if(n < TIMER_MAX_CHANNEL)
    {
        TIMER_DISABLE_INT;
        pTmr = &TmrCtr.TmrTbl[n];
        pTmr->TimeFunc      = func;
        pTmr->FuncArg       = arg;
        TIMER_ENABLE_INT; 
    }
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : TimerSetTimer()
*  Author:         ZhenFu Fang
*  Description:    ��ʱʱ�����á�
*  Calls:          
*  Input:          n        ��ʱ��ͨ��
*                  TmrValue ��ʱ��ֵ
*  Return:         ��
*  Remark:              
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          05/11/09      1.0           ORG
*
***********************************************************
*/
TIMER_CODE_SECTION
void TimerSetTimer(unsigned int n, unsigned int TmrValue)
{
    unsigned int imask_tmp;
    
    TimeStruct *pTmr;
    
    if(n < TIMER_MAX_CHANNEL)
    {
        TIMER_DISABLE_INT;
        pTmr = &TmrCtr.TmrTbl[n];
        pTmr->TimeCounter   = TmrValue;
        pTmr->TimeInitValue = TmrValue;
        TIMER_ENABLE_INT;
    }
}

TIMER_CODE_SECTION
void TimerModule(void)
{
    TimeStruct *pTmr;
    void  (*pfunc)(void *);
    void  *parg;
    unsigned int i;

    pfunc = (void (*)(void *))0;                      
    parg  = (void *)0;
   
    pTmr = &TmrCtr.TmrTbl[0];
    
    for(i=0; i<TIMER_MAX_CHANNEL; i++)
    {
        if(TmrCtr.TmrFlag & (0x0001<<(i)))
        {
            if(pTmr->TimeCounter > 0)
            {
                pTmr->TimeCounter--;
                if(pTmr->TimeCounter == 0)
                {
                   TmrCtr.TmrFlag &= (~(0x0001<<(i))); 
                   pfunc           = pTmr->TimeFunc;       
                   parg            = pTmr->FuncArg; 
                }
            }
            if (pfunc != (void (*)(void *))0) 
            {       
                (*pfunc)(parg);                                     
                pfunc = (void (*)(void *))0;
            }
        }
        pTmr++;
    }
}

SYSTEM_TEXT_SECTION
void ScrollStrTmrFunc(void *parg)
{
    unsigned int *pval;
    
    TimerReset(SCROLL_TIMER_CHANNEL);
    TimerStart(SCROLL_TIMER_CHANNEL);
    
    pval = parg;
    if(*pval == 0)
        (*pval)++;
}

