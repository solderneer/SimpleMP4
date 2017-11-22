/*****************************************************************************
 *
 *  XVID MPEG-4@QVGA VIDEO DECODER ON ROCKCHIP RK2606
 *	
 *	Author:
 *		Jian Huan	<jh@rock-chips.com>
 *
 *	Date:
 *		2005-06-15
 ****************************************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "../xvid_decoder.h"

__attribute__((section(".avi_v_text, \"ax\"")))
void image_swap(IMAGE * image1, IMAGE * image2);


#endif							/* _IMAGE_H_ */
