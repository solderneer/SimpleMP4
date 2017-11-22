/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	epphal.h
Author:		XUESHAN LIN
Created:		22th Nov. 2005
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/
#ifndef _FWEPPHAL_H
#define	_FWEPPHAL_H

	#define 		DISABLE_USB_INT			asm volatile ("bitc %%imask, %0" : : "n" (10));
	#define 		ENABLE_USB_INT 			asm volatile ("bits %%imask, %0" : : "n" (10));
	#define 		SWAP(x)  		 			((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))

	#define 		KEYBOARD 					GPIO_PDAT0	//¼üÅÌÆ¬Ñ¡µØÖ·

	#define 		FLASH_PROTECT_ON()		write_XDATA(FMCTL, read_XDATA(FMCTL) & (~0x04))
	#define 		FLASH_PROTECT_OFF()		write_XDATA(FMCTL, read_XDATA(FMCTL) | 0x04)


    #define     FIRMWARE_CODE  __attribute__((section(".firmware_text, \"ax\"")))
    #define     FIRMWARE_DATA  __attribute__((section(".firmware_data, \"aw\"")))
    #define     FIRMWARE_BSS  __attribute__((section(".firmware_bss, \"aw\"")))
//    #define     FIRMWARE_CODE_ASM       .section ".firmware_text", "ax"



#ifndef read_XDATA
	#define     	read_XDATA(address)         	(*((unsigned char volatile*)address))
#endif
#ifndef write_XDATA
	#define     	write_XDATA(address, value) 	(*((unsigned char volatile*)address) = value)
#endif
#ifndef read_XDATA32
	#define     	read_XDATA32(address, value) 	(*((unsigned long volatile*)address) = value)
#endif
#ifndef write_XDATA32
	#define     	write_XDATA32(address, value) 	(*((unsigned long volatile*)address) = value)
#endif

#ifndef IN_ASM
	//1 µ×²ã»ã±à½Ó¿Ú
	//interface.s
	extern 	void 	FWFWAsmReadFlashX8X2(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	FWFWAsmReadFlashX16(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	FWAsmReadFlash(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	FWFWAsmWriteFlashX8X2(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	FWFWAsmWriteFlashX16(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	FWAsmWriteFlash(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	FWAsmReadUSB(unsigned int *buf, unsigned int len, unsigned int EndPFifo);
	extern 	void 	FWAsmWriteUSB(unsigned int *buf, unsigned int len, unsigned int EndPFifo);
#endif
#endif

