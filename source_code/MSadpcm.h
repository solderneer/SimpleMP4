/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : MSadpcm.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/

#ifndef _MSADPCM_H
#define _MSADPCM_H

//------------------------------------------------------------------------------

unsigned int msadpcm_encode_block(MS_ADPCM_DATA *msadpcm, unsigned int *output,int *input,int num);

//------------------------------------------------------------------------------
#endif
//******************************************************************************
