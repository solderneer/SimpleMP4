/*********************************************************************
 * fft128.h
 **********************************************************************
 *  
 *   Copyright (C), 2005-2006, ROCK-CHIPS Semi. Co., Ltd.
 ×  Author:Huangxudong        Version :1.0     Date:2005-10-7
 ×  Description:  128点 FFT 的实虚部系数Wi和Wr数组初始化            
 *
 ***********************************************************************
 * Change History
 *
 * Version	Author			Last Modified		Remarks
 * 
 ***********************************************************************/

#ifndef _FFT128_H
#define _FFT128_H

.section .wma_data,"aw"
//.segment "data"
.walign 4

TWF_ADDR:
	.word	-32767           !!Q15
	.word	0
	.word	-23170
	.word	23170
	.word	-23170
	.word	-23170
	.word	-12539
	.word	30273
	.word	-30273
	.word	-12539
	.word	-30273
	.word	12539
	.word	-12539
	.word	-30273
	.word	-6393
	.word	32137
	.word	-32137
	.word	-6393
	.word	-27245
	.word	18204
	.word	-18204
	.word	-27245
	.word	-18204
	.word	27245
	.word	-27245
	.word	-18204
	.word	-32137
	.word	6393
	.word	-6393
	.word	-32137
	.word	-3212
	.word	32609
	.word	-32609
	.word	-3212
	.word	-25329
	.word	20787
	.word	-20787
	.word	-25329
	.word	-15446
	.word	28898
	.word	-28898
	.word	-15446
	.word	-31356
	.word	9512
	.word	-9512
	.word	-31356
	.word	-9512
	.word	31356
	.word	-31356
	.word	-9512
	.word	-28898
	.word	15446
	.word	-15446
	.word	-28898
	.word	-20787
	.word	25329
	.word	-25329
	.word	-20787
	.word	-32609
	.word	3212
	.word	-3212
	.word	-32609
	.word	-1608
	.word	32728
	.word	-32728
	.word	-1608
	.word	-24279
	.word	22005
	.word	-22005
	.word	-24279
	.word	-14010
	.word	29621
	.word	-29621
	.word	-14010
	.word	-30852
	.word	11039
	.word	-11039
	.word	-30852
	.word	-7962
	.word	31785
	.word	-31785
	.word	-7962
	.word	-28105
	.word	16846
	.word	-16846
	.word	-28105
	.word	-19519
	.word	26319
	.word	-26319
	.word	-19519
	.word	-32412
	.word	4808
	.word	-4808
	.word	-32412
	.word	-4808
	.word	32412
	.word	-32412
	.word	-4808
	.word	-26319
	.word	19519
	.word	-19519
	.word	-26319
	.word	-16846
	.word	28105
	.word	-28105
	.word	-16846
	.word	-31785
	.word	7962
	.word	-7962
	.word	-31785
	.word	-11039
	.word	30852
	.word	-30852
	.word	-11039
	.word	-29621
	.word	14010
	.word	-14010
	.word	-29621
	.word	-22005
	.word	24279
	.word	-24279
	.word	-22005
	.word	-32728
	.word	1608
	.word	-1608
	.word	-32728

#endif // _FFT128_H
