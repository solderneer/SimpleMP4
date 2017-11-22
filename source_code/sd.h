#ifndef _SD_H
#define _SD_H


//1可配置参数

//1结构定义
//MMC/SD卡CSD数据结构
typedef union _SD_CSD
{
	struct _field
	{
//16	
		uint8 CsdStruct			:2;
		uint8 SpecVer           		:4;
		uint8 Reserved1          	:2;
		uint8 TAAC  				:8;
//16		
		uint8 NSAC				:8;
		uint8 TranSpeed			:8;
//16		
		uint8 CCC				:12;
		uint8 ReadBlkLen			:4;

//18		
		uint8 PartialReadEn		:1;
		uint8 WriteBlkMisalign	:1;
		uint8 ReadBlkMisalign		:1;
		uint8 DSR_IMP			:1;
		uint8 Reserved2			:2;
		uint8 CSize				:12;

//30		
		uint8 VddReadCurMin		:3;
		uint8 VddReadCurMax		:3;
		uint8 VddWriteCurMin		:3;
		uint8 VddWriteCurMax		:3;
		uint8 CSizeMult			:3;
		uint8 EraseGroupSize		:5;
		uint8 EraseGroupMult		:5;
		uint8 WPGroupSize		:5;
//16		
		uint8 WPGroupEn			:1;
		uint8 DefaultEcc			:2;
		uint8 WriteSpeedFactor	:3;
		uint8 WriteBlkLen			:4;
		uint8 PartialWriteEn		:1;
		uint8 Reserved3			:5;
//15		
		uint8 FileFormatGroup	:1;
		uint8 CopyFlag			:1;
		uint8 PermWriteProtect	:1;
		uint8 TmpWriteProtect	:1;
		uint8 FileFormat			:2;
		uint8 EccCode			:2;
		uint8 CRC				:7;
		uint8 Reserved4			:1;
	} bits;
	uint16 SdCsd[8];
} SD_CSD;


//1常量定义
#define		X8			0
#define		X16			1

#define		FLASH		0
#define		CARD		1

#define		SUCCEED	0
#define		TIME_OUT	1
#define		NO_CARD	2


//1全局变量
#undef	EXT
#ifdef	IN_SD
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint16	MediaSel			DRAM_FAT;					//媒体选择,0=NAND,1=SD CARD
EXT		uint32	SDTotSec		DRAM_FAT;					//SD卡总扇区数
EXT		uint16	SDBytsPerSec		DRAM_FAT;					//SD卡每扇区字节数
EXT		bool 	SDCardInsert		DRAM_FAT;					//SD卡每扇区字节数


//1函数原型声明
//sd.c
extern	uint8	MediaSetup(uint8 memory);
extern	bool 	SDWaitReady(void);
extern	bool 	CardCheckIn(void);
extern	uint8 	SDInit(void);
extern	void 	ReadCsd(uint16 *buf);
extern	uint8 	SdCmdInit(void);
extern	bool 	SdWriteSec(uint32 LBA, uint16 col, uint8 *buf, uint16 Length, uint8 width);
extern	bool 	SdReadSec(uint32 LBA, uint16 col, uint8 *buf, uint16 Length, uint8 width);

extern	bool 	UsbSdReadMulSecInit(uint32 LBA, uint16 count);
extern	bool 	UsbSdReadFifo(uint8 *buf);
extern	bool 	UsbSdWriteMulSecInit(uint32 LBA, uint16 count);
extern	bool 	UsbSdWriteFifo(uint8 *buf);

//1表格定义
#ifdef	IN_SD
#else
#endif
#endif


