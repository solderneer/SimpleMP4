/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	FWflash.h
Author:		XUESHAN LIN
Created:		22th Nov. 2005
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/
#ifndef _FLASH_H
#define _FLASH_H


/*******************************************************************
�곣������
*******************************************************************/
#define		OK							0
#define		ERROR						1


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
EXT		uint8 	CurFlashChip;				//��ǰѡ�е�FLASH
EXT		uint8 	SecPerBlock[MAX_LUN];		//FLASHÿBLOCK������

EXT		bool 	LargePage[MAX_LUN];			//FLASH�Ǵ�PAGE
EXT		bool 	Data16bit[MAX_LUN];			//FLASH��16λ
EXT		bool 	FiveAddr[MAX_LUN];			//FLASH��ַ��5��
EXT		bool 	LargePageCmdFlag;

//1����ԭ������
//flash.c
extern	void 	FWdelay1us(uint32 n);
extern	void 	FWReadFlashID(uint8 *buf);
extern	bool 	FWFlashBlockErase(uint32 RowAddr);
extern	uint8	FWFlashReadStatus(void);
extern	void 	FWFlashReadPage(uint32 row, uint16 col, uint8 *pData, uint16 length);
extern	bool 	FWFlashPageProg(uint32 row, uint16 col, uint8 *pData, uint16 length);
extern	bool 	FWCheckBadBlock(uint32 pageNum);
extern	void 	FWFlashProgError(uint32 PageAddr);

//1�����
#endif

