/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : BatteryModule.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
#ifndef  _BATTERYMODULE_H
#define  _BATTERYMODULE_H
//------------------------------------------------------------------------------
extern void AdcSampleInit(void);
extern void AdcDataSample(void);
extern void BatteryLevelDetect(void);
extern void BatteryChargeStart(void);
extern void BatteryChargeDetect(void);
//------------------------------------------------------------------------------
#endif
//******************************************************************************

