#ifndef _FLASH_H
#define _FLASH_H

//1�����ò���
//#define		FLASH_X16							//�����Ƿ���֧��16λFLASH

#define 		MAX_FLASH_NUM			4			/*���֧�ֵ�FLASH��*/
#define 		MAX_ZONE_NUM				64			/*����ZONE��,ZONE=256BLOCKs*/
#define 		MAX_EXCH_BLOCKS			10			/*���ĵĻ��齻�����*/
#define 		SYS_AREA					10240		/*ϵͳ����,��KBΪ��λ*/

/*******************************************************************
ϵͳ���
*******************************************************************/
//FTL.c
#define 		FIRST_SPARE_NUM			8			//ZONE 0�����ݽ�����BLOCK��(9��BLOCK)
#define		SECOND_BLOCK_NUM			253			//256-2-1,ZONE=256BLOCKs,2ӳ���,1���ݽ�����
#define		ZONE_RESERVE				(256-SECOND_BLOCK_NUM)
													//ÿ��ZONE��ʵ�ʿ��ÿ���
#define		FST_ZONE_RESERVE			(ZONE_RESERVE+FIRST_SPARE_NUM+1+MAX_EXCH_BLOCKS+1+1)
										//��һ��ZONE��������2+9+1�ÿ��¼+1��ʱ��,1���������

/*******************************************************************
�곣������
*******************************************************************/
#define		OK							0
#define		ERROR						1
#define		OFFSET						(FLASH1_DATA-FLASH0_DATA)


/*******************************************************************
����ID����
*******************************************************************/
#define		SAMSUNG					0x00		//����SAMSUNG
#define		TOSHIBA					0x01		//��֥TOSHIBA
#define		HYNIX						0x02		//����ʿHYNIX
#define		INFINEON					0x03		//Ӣ����INFINEON
#define		MICRON						0x04		//����MICRON
#define		RENESAS					0x05		//����RENESAS
#define		ST							0x06		//�ⷨ�뵼��ST

/*******************************************************************
����ID����
*******************************************************************/
#define		Small64M					0x00		//����SAMSUNG
#define		Small128M					0x01		//��֥TOSHIBA
#define		Large128M					0x02		//����ʿHYNIX
#define		Large256M					0x03		//Ӣ����INFINEON
#define		Large512M					0x04		//����MICRON
#define		Large1G						0x05		//����RENESAS
#define		Large2G						0x06		//�ⷨ�뵼��ST


/*******************************************************************
FLASHͨ���������(����)
*******************************************************************/
#define		RESET_CMD					0xff
#define		READ_ID_CMD				0x90
#define		READ_STATUS_CMD			0x70
#define		PAGE_PROG_CMD				0x8010
#define		BLOCK_ERASE_CMD			0x60d0

/*******************************************************************
LARGE PAGE FLASH�������(����)
*******************************************************************/
#define		READ_CMD					0x0030
#define		READ_COPY_CMD				0x0035
#define		CACHE_PROG_CMD			0x8015
#define		COPY_PROG_CMD				0x8510
#define		RAND_DATAIN_CMD			0x85
#define		RAND_DATAOUT_CMD			0x05e0

/*******************************************************************
SMALL PAGE FLASH�������(����)
*******************************************************************/
#define		READ0_CMD					0x00
#define		READ1_CMD					0x01
#define		READ_SPARE_CMD			0x50
#define		SMALL_COPY_PROG_CMD		0x8A10

/*******************************************************************
FLASH��д�ӿ�
*******************************************************************/

//1�ṹ����




//1ȫ�ֱ���
#undef	EXT
#ifdef	IN_FLASH
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint8 	SecPerBlock[MAX_FLASH_NUM] 	DRAM_FAT;		//FLASHÿBLOCK������
EXT		uint32	G_TotLogicSec[MAX_FLASH_NUM]	DRAM_FAT;		//�ܵ��߼�������
EXT		uint32	G_TotPhySec[MAX_FLASH_NUM]	DRAM_FAT;		//�ܵ�����������
EXT		uint32	G_TotBlks[MAX_FLASH_NUM]	DRAM_FAT;		//�ܵ�����BLOCK��
EXT		uint16 	FlashCmdPort[MAX_FLASH_NUM] 	DRAM_FAT;
EXT		uint16 	FlashAddrPort[MAX_FLASH_NUM] 	DRAM_FAT;
EXT		uint16 	FlashDataPort[MAX_FLASH_NUM] 	DRAM_FAT;
EXT		uint16 	FlashPageBuf[1024]			DRAM_FAT;
EXT		uint32 	MlcRefleshRow				DRAM_FAT;
EXT		uint16 	syscopyaddr[64]			DRAM_FAT;
EXT		uint16 	SysAreaRawBlock			DRAM_FAT;
EXT		uint16 	SysBackupSta			DRAM_FAT;
#undef	EXT
#ifdef	IN_FTL
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint16 	RemapPosInBlk[MAX_ZONE_NUM]	DRAM_FAT;	//ӳ����п��е�λ��,���32ZONE,ÿZONE=256BLOCK
EXT		uint8	RemapPosInZone[MAX_ZONE_NUM]	DRAM_FAT;	//ӳ�����ZONE�е�λ��
EXT		uint8	ValidBlksTbl[MAX_ZONE_NUM]		DRAM_FAT;	//��¼ÿ��ZONE����Ч����,�͸�ʱ��¼
EXT		uint16 	BadBlkExchTbl[MAX_EXCH_BLOCKS] 	DRAM_FAT;	//��д���̻����滻�б�

EXT		uint8 	RemapTblBuf[512]		DRAM_FAT;				//ӳ���
EXT		uint8 	CacheRemap			DRAM_FAT;				//��ǰCACHE��ӳ���
EXT		uint8 	SparePos				DRAM_FAT;				//ZONE0ӳ������
EXT		uint32 	CopyStartPage		DRAM_FAT;				//�鿽��ʱĿ����ʼ��ַ
EXT		uint32 	CopyEndPage			DRAM_FAT;				//�鿽��ʱĿ�������ַ
EXT		uint32 	SrcBlock				DRAM_FAT;				//�鿽��ʱԴ���ַ
EXT		bool 	LargePageCmdFlag		DRAM_FAT;				//���ڴ�PAGE FLASHд�����־
EXT		uint8 	SecCount				DRAM_FAT;
EXT		uint8 	SysAreaBlock			DRAM_FAT;				//ϵͳ������
EXT		uint16 	WriteSectors			DRAM_FAT;				//NAND FLASH����д������FTLʹ��
EXT		uint16 	SCSI_DataLength		DRAM_FAT;				//USB BULK-ONLYʹ��
EXT		uint8	FlashProtectStatus		DRAM_FAT;				//NAND FLASHд����״̬
EXT		uint32	PreRemapLBA			DRAM_FAT;				//FTLʹ��
EXT		uint32	UsbFlashRowAddr		DRAM_FAT;				//���ڴ�PAGE FLASHд�����־

EXT		uint32	UsbNandErrSector		DRAM_FAT;				//NAND ��̳�������
EXT		uint16*	UsbNandErrBuff		DRAM_FAT;				//NAND ��̳������ݻ�����
EXT		uint16	UsbNandErrLen		DRAM_FAT;				//NAND ��̳���������
EXT		bool		UsbNandErrChkEn		DRAM_FAT;				//NAND ��̳�����ʹ��
EXT		uint8 	NeedSetRemap		DRAM_FAT;				//2006.08.14 debug @win2000
EXT		uint8 	EndSetRemap			DRAM_FAT;				//MLC¼��ʱӳ����Ҫ
EXT		uint32 	gTempBlock			DRAM_FAT;				//2006.08.14 debug @win2000
#ifndef IN_SYSTEM
EXT		uint32	Test1;
EXT		uint32	Test2;
EXT		uint16	Test3;
EXT		uint16	Test4;
EXT		uint16	Test5;
EXT		uint16	Test6;
EXT		FILE 	*fp;
EXT		FILE 	*fp1;
EXT		FILE 	*fp2;
#endif

//1����ԭ������
//FTL.c
extern	void 	FTLInit(void);
extern	void 	GetZone(uint32 LBA, uint8* zone, uint8* BlockOffset, uint8* SectorOffset);
extern	uint32 	GetAbsPhyAddr(uint16 BlkAddr);
extern	uint32 	GetRemap(uint32 LBA);
extern	void		SetRemap(uint32 LBA);
extern	uint32 	GetExchageBlock(uint32 LBA);
extern	uint8 	GetRemapTbl(void);
extern	void 	MakeRemapTbl(void);
extern	uint32 	BadBlkReplace(uint32 SrcBlockAddr);
extern	bool 	NandFlashReadSector(uint32 LBA, uint8 *pData);
extern	bool 	NandFlashWriteSector(uint32 LBA,  uint8 *pData);
extern	bool 	NandFlashWriteFile(uint32 LBA, uint8 *pData);
extern	bool 	UsbNandFlashReadSector(uint32 LBA);
extern	bool 	UsbNandFlashWriteSector(uint32 LBA);
extern	uint32 	FlashProgError(uint32 PageAddr, uint8 *DataBuf, uint16 nSec);
extern	uint32 	UsbFlashProgError(uint32 PageAddr, uint16 *DataBuf, uint16 nSec);



//flash.c
extern	void 	delay1us(uint32 n);
extern	uint8 	FlashInit(uint32 *IdBlkByteAddr);
extern	uint8 	FlashGetChip(uint32 row);
extern	void 	FlashCs(uint8 ChipSel);
extern	void 	FlashDeCs(uint8 ChipSel);
extern	void 	FlashProgCmd(uint32 row, uint16 col);
extern	void 	FlashReadCmd(uint32 row, uint16 col);
extern	void 	FlashWaitBusy(uint8 ChipSel);
extern	bool 	FlashBlockErase(uint32 RowAddr);
extern	uint8	FlashReadStatus(uint8 ChipSel);
extern	uint8 	FlashReadPage(uint32 row, uint16 col, uint8 *pData, uint16 length);
extern	uint8 	FlashReadPagelogic(uint32 row, uint8 *pData);
extern	bool 	FlashPageProg(uint32 row, uint16 col, uint8 *pData, uint16 length);
extern	bool 	FlashPageProgLogic(uint32 row, uint16 col, uint8 *pData, uint16 length);
extern	bool 	FlashProgLargePage(uint32 row, uint8 *pData);
extern	bool 	FlashCopyProg(uint32 SrcRow, uint32 DestRow, uint8 PageCnt);
extern	void 	MlcReadErrorHook(uint32 row);
extern	void 	MlcDataRefresh(void);

extern	bool 	UsbFlashPageProg(uint32 row);
extern	bool 	UsbProgLargePage(uint32 row);
extern	bool 	UsbFlashReadPage(uint32 row);

//1�����
#ifdef	IN_FLASH
		/*******************************
		����ID��
		********************************/
		//uint8 ManufactureIDTbl[] DRAM_FAT_TABLE=
		//{
		//	0xec,					//����SAMSUNG
		//	0x98,					//��֥TOSHIBA
		//	0xad,					//����ʿHYNIX
		//	0xc1,					//Ӣ����INFINEON
		//	0x2c,					//����MICRON
		//	0x07,					//����RENESAS
		//	0x20					//�ⷨ�뵼��ST
		//};
		
		/*******************************
		����ID��
		********************************/
		uint8 DeviceCode[] DRAM_FAT_TABLE=
		{
			0x76,					//small 8bit-64MB
			0x79,					//small 8bit-128MB
			0xF1,					//large 8bit-128M
		 	0xDA,					//large 8bit-256M
		 	0xDC,					//large 8bit-512M
		 	0xD3,					//large 8bit-1G
		 	0xD5,					//large 8bit-2G

			0x56,					//small 16bit-64MB
			0x74,					//small 16bit-128MB
			0xc1,					//large 16bit-128M
			0xca,					//large 16bit-256M
			0xcc,					//large 16bit-512M
			0xc3,					//large 16bit-1G
			0xc5					//large 16bit-2G
		};

		/*******************************
		������Ϣ��
		********************************/
		uint32 DeviceInfo[] DRAM_FAT_TABLE=		//phisical sectors
		{
			131072, 					// DI_64M,	small page
			262144, 					// DI_128M,	small page
			262144, 					// DI_128M,	large page
			524288,  					// DI_256M,	large page
			1048576,					// DI_512M,	large page
			2097152,					// DI_1G,		large page
			4194304					// DI_2G,		large page
		};
#else
		extern	uint8 	ManufactureIDTbl[];
		extern	uint8 	DeviceCode[];
		extern	uint32	DeviceInfo[];
#endif
#endif

