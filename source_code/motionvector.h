/*****************************************************************************
 *
 *  XVID MPEG-4@QVGA VIDEO DECODER ON ROCKCHIP RK2606
 *	
 *	Author:
 *		Jian Huan	<jh@rock-chips.com>
 *
 *	Date:
 *		2005-11-18 15:49
 ****************************************************************************/

#ifndef _MOTIONVECTOR_H_
#define _MOTIONVECTOR_H_

#include "../xvid_decoder.h"

__attribute__((section(".avi_v_text, \"ax\"")))
void
get_0_motion_vector(MACROBLOCK *mb);

__attribute__((section(".avi_v_text, \"ax\"")))
void
get_1_motion_vector( MACROBLOCK *mb, POSITION *pos, short fcode);

__attribute__((section(".avi_v_text, \"ax\"")))
void
get_4_motion_vector(MACROBLOCK *mb, POSITION *pos,  short fcode);

#endif //_MOTIONVECTOR_H_
