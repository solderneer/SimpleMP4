/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	epphal.h
Author:		XUESHAN LIN
Created:		29th SEP 2004
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/
#ifndef _EPPHAL_H
#define	_EPPHAL_H

	#define 		DISABLE_USB_INT			asm volatile ("bitc %%imask, %0" : : "n" (10));
	#define 		ENABLE_USB_INT 			asm volatile ("bits %%imask, %0" : : "n" (10));
	#define 		DISABLE_GLOBE_INT			asm volatile ("bitc %%imask, %0" : : "n" (15));
	#define 		ENABLE_GLOBE_INT 			asm volatile ("bits %%imask, %0" : : "n" (15));
	#define 		SWAP(x)  		 			((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))

	#define 		DMA_INT_ENABLE	 		0x0001
	#define 		DMA_DST_WIDTH_8  		 	0x0000
	#define 		DMA_DST_WIDTH_16 	 	0x0002
	#define 		DMA_DST_WIDTH_32 	 	0x0004
	
	#define 		DMA_SRC_WIDTH_8  		 	0x0000
	#define 		DMA_SRC_WIDTH_16 	 	0x0010
	#define 		DMA_SRC_WIDTH_32 	 	0x0020
	
	#define 		DMA_DINC_INC  			 	0x0000
	#define 		DMA_DINC_DEC		 	 	0x0080
	#define 		DMA_DINC_NOCHG 		 	0x0100
	
	#define 		DMA_SINC_INC  			 	0x0000
	#define 		DMA_SINC_DEC		 	 	0x0200
	#define 		DMA_SINC_NOCHG 		 	0x0400

//***********************************************************************************
	#define 		ALIGN_ATTR( align ) 			__attribute__((aligned (align)))	//2代码空间和数据空间定位,用于overlay
#ifndef IN_SYSTEM
	#define		DRAM_USB					//__attribute__((section(".usb_bss, \"aw\"")))
	#define		DRAM_FAT					//__attribute__((section(".fat_bss, \"aw\"")))
	#define		DRAM_ENCODE				//__attribute__((section(".idle_bss, \"aw\"")))
	#define		DRAM_IDLE					//__attribute__((section(".idle_bss, \"aw\"")))

	#define		DRAM_USB_TABLE			//__attribute__((section(".usb_data, \"aw\"")))
	#define		DRAM_FAT_TABLE			//__attribute__((section(".fat_data, \"aw\"")))

	#define		IRAM_USB					//__attribute__((section(".usb_text, \"ax\"")))
	#define		IRAM_FAT					//__attribute__((section(".fat_text, \"ax\"")))
	#define		IRAM_DELETE				//__attribute__((section(".fat_text, \"ax\"")))
	#define		IRAM_IDLE					//__attribute__((section(".fat_text, \"ax\"")))
	#define		IRAM_ENCODE				//__attribute__((section(".encode_text, \"ax\"")))
	#define     	IRAM_ENCODE_INIT       		 //__attribute__((section(".encode_text, \"ax\""))) //__attribute__((section(".encode_init_text, \"ax\"")))
	#define     	IRAM_FLASH_INIT         		//__attribute__((section(".flash_init_text, \"ax\"")))    // add by hjl 20060422

	#define		IRAM_USB_ASM				//.section ".usb_text", "ax"
	#define		IRAM_FAT_ASM				//.section ".fat_text", "ax"
	#define		IRAM_ENCODE_ASM			//.section ".encode_text", "ax"
#else	
	#define		DRAM_USB					__attribute__((section(".usb_bss, \"aw\"")))
	#define		DRAM_FAT					//__attribute__((section(".fat_bss, \"aw\"")))
	#define		DRAM_ENCODE				__attribute__((section(".enc_bss, \"aw\"")))
	#define		DRAM_IDLE					//__attribute__((section(".idle_bss, \"aw\"")))

	#define		DRAM_USB_TABLE			__attribute__((section(".usb_data, \"aw\"")))
	#define		DRAM_FAT_TABLE			//__attribute__((section(".fat_data, \"aw\"")))
	#define		IRAM_SD				__attribute__((section(".usb_sd, \"ax\"")))

	#define		IRAM_USB					__attribute__((section(".usb_text, \"ax\"")))
	#define		IRAM_FAT					__attribute__((section(".fat_text, \"ax\"")))
	#define		IRAM_DELETE				__attribute__((section(".fat_text, \"ax\"")))
	#define		IRAM_IDLE					__attribute__((section(".fat_text, \"ax\"")))
	#define		IRAM_ENCODE				__attribute__((section(".encode_text, \"ax\"")))
	#define     	IRAM_ENCODE_INIT       		 __attribute__((section(".encode_text, \"ax\""))) //__attribute__((section(".encode_init_text, \"ax\"")))
	#define     	IRAM_FLASH_INIT         		__attribute__((section(".flash_init_text, \"ax\"")))    // add by hjl 20060422

	#define		IRAM_USB_ASM				.section ".usb_text", "ax"
	#define		IRAM_FAT_ASM				.section ".fat_text", "ax"
	#define		IRAM_ENCODE_ASM			.section ".encode_text", "ax"
#endif
//***********************************************************************************

	#define 		KEYBOARD 					GPIO_PDAT0	//键盘片选地址

	#define 		FLASH_PROTECT_ON()		write_XDATA(FMCTL, read_XDATA(FMCTL) & (~0x04))
	#define 		FLASH_PROTECT_OFF()		write_XDATA(FMCTL, read_XDATA(FMCTL) | 0x04)

#ifndef read_XDATA
	#define     	read_XDATA(address)         	(*((unsigned char volatile*)address))
#endif
#ifndef write_XDATA
	#define     	write_XDATA(address, value) 	(*((unsigned char volatile*)address) = value)
#endif
#ifndef read_XDATA32
	#define     	read_XDATA32(address) 		(*((unsigned long volatile*)address))
#endif
#ifndef write_XDATA32
	#define     	write_XDATA32(address, value) 	(*((unsigned long volatile*)(address)) = value)
#endif



#ifndef IN_ASM
//1全局变量
#undef	EXT
#ifdef	IN_EPPHAL
		#define	EXT
#else
		#define	EXT		extern
#endif		

#ifdef DMA_INT_EN
EXT		bool 	volatile Dma0BusyFlag	DRAM_FAT;		//DMA忙标志
EXT		bool 	volatile Dma1BusyFlag	DRAM_FAT;		//DMA忙标志
#endif




	//1 底层汇编接口
	//interface.s
	extern 	void 	AsmReadFlashX32(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	AsmReadFlashX16(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	AsmReadFlash(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	AsmWriteFlashX32(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	AsmWriteFlashX16(unsigned int *buf, unsigned int len, unsigned int Addr);
	extern 	void 	AsmWriteFlash(unsigned int *buf, unsigned int len, unsigned int Addr);

	extern 	void 	AsmReadUSB(unsigned int *buf, unsigned int len, unsigned int EndPFifo);
	extern 	void 	AsmWriteUSB(unsigned int *buf, unsigned int len, unsigned int EndPFifo);
	extern 	void 	AsmReadUSBX16(unsigned int *buf, unsigned int len, unsigned int EndPFifo);
	extern 	void 	AsmWriteUSBX16(unsigned int *buf, unsigned int len, unsigned int EndPFifo);
	extern 	void 	AsmDelay10cyc(unsigned int count);

	extern 	void 	AsmSdramInit(void);
	extern 	void 	AsmSdramRefresh(void);
	extern 	void 	AsmReadSdram(unsigned int *buf, unsigned int RowAddr);
	extern 	void 	AsmWriteSdram(unsigned int *buf, unsigned int RowAddr);

	extern 	void 	AsmSdReadx8(unsigned int Col, unsigned int Length, unsigned int *buf);
	extern 	void 	AsmSdReadx16(unsigned int Col, unsigned int Length, unsigned int *buf);
	extern 	void 	AsmSdWritex8(unsigned int *buf);
	extern 	void 	AsmSdWritex16(unsigned int *buf);
	extern 	void 	AsmSdReadDiscard(unsigned int Col);

	
	extern 	void 	AsmByte2Word(unsigned int *src, unsigned int *dest, unsigned int len);
	extern 	void 	AsmWord2Byte(unsigned int *src, unsigned int *dest, unsigned int len);
	extern 	void 	DmaInit(void);
	extern 	void 	DmaXfer(uint32 SrcAddr, uint32 DstAddr, uint16 Len, uint16 Mode);
	extern 	void 	DmaXfer1(uint32 SrcAddr, uint32 DstAddr, uint16 Len, uint16 Mode);
	extern 	bool 	CheckVbus(void);
	extern 	bool 	CheckCard(void);


#endif
#endif

