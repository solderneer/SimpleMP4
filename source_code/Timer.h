/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Main.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/12/12       1.0            ORG
*
********************************************************************************
*/
#ifndef  _TIMER_H
#define  _TIMER_H
//------------------------------------------------------------------------------
void SysTimerModule(void *parg);
extern void TimerModule(void);
extern void TimerCfgFunc(unsigned int n, void (*func)(void *), void *arg);
extern void TimerInit(void);
extern void TimerReset(unsigned int n);
extern void TimerSetTimer(unsigned int n, unsigned int TmrValue);
extern void TimerStart(unsigned int n);
extern void TimerStop(unsigned int n);
extern int TimerGetState(unsigned int n);
//------------------------------------------------------------------------------
#endif
//******************************************************************************
