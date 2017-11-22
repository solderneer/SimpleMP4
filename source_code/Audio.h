/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Audio.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/12/12       1.0            ORG
*
********************************************************************************
*/
#ifndef  _AUDIO_H
#define  _AUDIO_H
//------------------------------------------------------------------------------
void CodecCommandSet(unsigned int uCmd, unsigned int uData);
void CodecPowerOnInitial(void);
void CodecPowerOff(void);
void OutputVolumeInc(void);
void OutputVolumeDec(void);
#ifdef  _INSIDE_HPSENSE_USED_
bool GetHpState(void);
#endif
//------------------------------------------------------------------------------
#endif
//******************************************************************************
