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

#ifndef _MB_CODING_H_
#define _MB_CODING_H_

__attribute__((section(".avi_v_text, \"ax\"")))
short 
get_mcbpc_inter(BITSTREAM * bs);

__attribute__((section(".avi_v_text, \"ax\"")))
short 
get_cbpy(BITSTREAM * bs,  short intra);

__attribute__((section(".avi_v_text, \"ax\"")))
void
get_imb_layer(BITSTREAM * , MACROBLOCK * ,DECCOEFF * );

#endif /* _MB_CODING_H_ */
