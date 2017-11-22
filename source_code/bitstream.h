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

#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_

#include "../xvid_decoder.h"
#include "../static_data.h"

/*****************************************************************************
 * Prototypes
 ****************************************************************************/
#define READ_MARKER()				BitstreamSkip(bs, 1)

//* header stuff */
__attribute__((section(".avi_v_text, \"ax\"")))
short BitstreamReadHeaders(BITSTREAM * bs,
							DECCOEFF  *xvid_decoeff);

__attribute__((section(".avi_v_text, \"ax\"")))
short 
log2bin(short value);

/*****************************************************************************
 * BITSTREAM functions
 ****************************************************************************/
/* reads n bits from bitstream without changing the stream pos */
__attribute__((section(".avi_v_text, \"ax\"")))
unsigned long
BitstreamShowBits(BITSTREAM * bs, short bits);

/*  read bitstream in ping-pong buffer from file */
__attribute__((section(".avi_v_text, \"ax\"")))
void 
FillBuffer(BITSTREAM *bs);

__attribute__((section(".avi_v_text, \"ax\"")))
void
BitstreamSkip(BITSTREAM * bs,  short bits);

/* move forward to the next byte boundary */
__attribute__((section(".avi_v_text, \"ax\"")))
void
BitstreamByteAlign(BITSTREAM * bs);

/* read n bits from bitstream */
__attribute__((section(".avi_v_text, \"ax\"")))
unsigned long 
BitstreamGetBits(BITSTREAM * bs, short n);

#endif /* _BITSTREAM_H_ */
