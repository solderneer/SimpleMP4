/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FmDisplay.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef  _FMDISPLAY_H
#define  _FMDISPLAY_H
//------------------------------------------------------------------------------
void Tuner_DummyWait(unsigned long count);
unsigned int Tuner_ReadData(unsigned char *data, unsigned int size);
unsigned int Tuner_SendCommand(unsigned char *pdata, unsigned int size);
void Tuner_MuteControl(bool active);
void IIC_SysInit(void);
void Tuner_PLLCount(unsigned int TunerFrequency, unsigned char hightsideflag);
void Tuner_FreqChange(unsigned char updownflag,unsigned int step);
void Tuner_FetchFreq(void);
void Tuner_StoreFreq(void);
//------------------------------------------------------------------------------
#endif
//******************************************************************************

