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

#ifndef _MEM_TRANSFER_H
#define _MEM_TRANSFER_H

#include "../xvid_decoder.h"

/*****************************************************************************
 * transfer API
 ****************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
void
DMA_YUV_I2E(DECODER *xvid_dec,short y, short size);

__attribute__((section(".avi_v_text, \"ax\"")))
void
DMA_YUV_E2I_A(short index,    short dst_offset_pos);

__attribute__((section(".avi_v_text, \"ax\"")))
void
DMA_YUV_E2I_B(short index,    short dst_offset_pos);

__attribute__((section(".avi_v_text, \"ax\"")))
void
DMA_YUV_E2I_C(short index,    short dst_offset_pos);

__attribute__((section(".avi_v_text, \"ax\"")))
void
copy_y_from_external_to_internal(DECODER *xvid_dec, short x, short y);

__attribute__((section(".avi_v_text, \"ax\"")))
void
copy_uv_from_external_to_internal(DECODER *xvid_dec, short x, short y);

__attribute__((section(".avi_v_text, \"ax\"")))
void AviDmaRelease(void *pArg,void * );

__attribute__((section(".avi_v_text, \"ax\"")))
void Rgb2Lcd(unsigned short *RgbBuf);

__attribute__((section(".avi_v_text, \"ax\"")))
void DMA_RGB_I2E(unsigned short *RgbBuf, short y, short size);

 __attribute__((section(".avi_v_text, \"ax\"")))
void DMA_RGB_E2LCD( );
 __attribute__((section(".avi_v_text, \"ax\"")))

unsigned int GetLCDSdramIndex();

__attribute__((section(".avi_v_text, \"ax\"")))
void CALL_DmaChannelSetup(unsigned int channel);

#endif
