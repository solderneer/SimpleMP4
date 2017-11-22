/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO DECODER ON LSI ZSP400
 *	
 *	Author:
 *		Jian Huan	<jh@rock-chips.com>
 *
 *	Date:
 *		2005-06-15
 ****************************************************************************/

#ifndef _XVID_SECTION_DEF_H_
#define  _XVID_SECTION_DEF_H_

/******************************************************************
                    Global Variable located in Bss
******************************************************************/
#define     MP4_GLOBAL_VARIABLE_SECTION  __attribute__((section(".avi_v_bss, \"aw\"")))
/******************************************************************
                    C prototype located in I-RAM section
******************************************************************/
#define     MP4_C_PROTOTYPE_SECTION  __attribute__((section(".avi_v_text, \"ax\"")))
/******************************************************************
                    Assemble located in I-RAM
 ******************************************************************/
 #define     MP4_ASSEMBLE_SECTION .section ".avi_v_text", "ax"
 /******************************************************************
                    Data Table  located in D-RAM
 ******************************************************************/
 #define     MP4_DATA_SECTION .section ".avi_v_data", "aw"              //modified by hjl "ax"--->"aw"
 /******************************************************************
                    Array  located in D-RAM
 ******************************************************************/ 
 #define     MP4_ARRAY_SECTION .section ".avi_v_data", "aw"
 /******************************************************************
                    Array  located in D-RAM
 ******************************************************************/ 
 #define     MP4_C_ARRAY_SECTION __attribute__((section(".avi_v_data, \"aw\"")))
 
 #endif
