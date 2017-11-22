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
宏常数定义
*******************************************************************/
#define		OK							0
#define		ERROR						1


/*******************************************************************
厂商ID编码
*******************************************************************/
#define		SAMSUNG					0x00		//三星SAMSUNG
#define		TOSHIBA					0x01		//东芝TOSHIBA
#define		HYNIX						0x02		//海力士HYNIX
#define		INFINEON					0x03		//英飞凌INFINEON
#define		MICRON						0x04		//美光MICRON
#define		RENESAS					0x05		//瑞萨RENESAS
#define		ST							0x06		//意法半导体ST

/*******************************************************************
器件ID编码
*******************************************************************/
#define		Small64M					0x00		//三星SAMSUNG
#define		Small128M					0x01		//东芝TOSHIBA
#define		Large128M					0x02		//海力士HYNIX
#define		Large256M					0x03		//英飞凌INFINEON
#define		Large512M					0x04		//美光MICRON
#define		Large1G						0x05		//瑞萨RENESAS
#define		Large2G						0x06		//意法半导体ST


/*******************************************************************
FLASH通过操作命令集(三星)
*******************************************************************/
#define		RESET_CMD					0xff
#define		READ_ID_CMD				0x90
#define		READ_STATUS_CMD			0x70
#define		PAGE_PROG_CMD				0x8010
#define		BLOCK_ERASE_CMD			0x60d0

/*******************************************************************
LARGE PAGE FLASH操作命令集(三星)
*******************************************************************/
#define		READ_CMD					0x0030
#define		READ_COPY_CMD				0x0035
#define		CACHE_PROG_CMD			0x8015
#define		COPY_PROG_CMD				0x8510
#define		RAND_DATAIN_CMD			0x85
#define		RAND_DATAOUT_CMD			0x05e0

/*******************************************************************
SMALL PAGE FLASH操作命令集(三星)
*******************************************************************/
#define		READ0_CMD					0x00
#define		READ1_CMD					0x01
#define		READ_SPARE_CMD			0x50
#define		SMALL_COPY_PROG_CMD		0x8A10

/*******************************************************************
FLASH读写接口
*******************************************************************/

//1结构定义




//1全局变量
#undef	EXT
#ifdef	IN_FLASH
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint8 	CurFlashChip;				//当前选中的FLASH
EXT		uint8 	SecPerBlock[MAX_LUN];		//FLASH每BLOCK扇区数

EXT		bool 	LargePage[MAX_LUN];			//FLASH是大PAGE
EXT		bool 	Data16bit[MAX_LUN];			//FLASH是16位
EXT		bool 	FiveAddr[MAX_LUN];			//FLASH地址有5个
EXT		bool 	LargePageCmdFlag;

//1函数原型声明
//flash.c
extern	void 	FWdelay1us(uint32 n);
extern	void 	FWReadFlashID(uint8 *buf);
extern	bool 	FWFlashBlockErase(uint32 RowAddr);
extern	uint8	FWFlashReadStatus(void);
extern	void 	FWFlashReadPage(uint32 row, uint16 col, uint8 *pData, uint16 length);
extern	bool 	FWFlashPageProg(uint32 row, uint16 col, uint8 *pData, uint16 length);
extern	bool 	FWCheckBadBlock(uint32 pageNum);
extern	void 	FWFlashProgError(uint32 PageAddr);

//1表格定义
#endif

