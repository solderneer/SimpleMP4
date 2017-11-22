/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	config.h
Author:		XUESHAN LIN
Created:		29th SEP 2004
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H

#define		IN_SYSTEM		//嵌入到系统软件时定义
//#define		K_FAT12		//定义是否支持FAT12
#define		ENCODE			//定义使能文件系统写
#define		USB				//定义使能USB
#define		LFORMAT		//定义使能低格
//#define		WDT_EN		//定义使能低格
//#define		DEBUG
#define		RECOVERY
#define		USB_HS_EN		//定义使能USB HS
#define		DMA_INT_EN	//定义DMA中断使能
#define		OT2			//定义使用2级优化以上
#define		DMA_BULKIN_EN	//定义DMA中断使能
#define		MK_DIR_EN		//定义建目录使能
//#define		OS_FILE		//定义OS下的文件系统
#define		SYS_PROTECT	//定义系统保留区写使能
#define		FLASH_DISK		1		//定义FLASH盘使能
#define		SD_CARD_EN	0		//定义SD卡使能
#define		CD_ROM			0		//定义光驱使能

#ifndef IN_SYSTEM
#define	CPU_PACK_PIN 144
#else
#include		"Macro.h"
#endif

#ifdef USB
#define		ENCODE			//定义使能文件系统写
#endif

#ifndef _FWCONFIG_H
typedef		unsigned char		uint8;
typedef		signed char		int8;
typedef		unsigned int		uint16;
typedef		signed int			int16;
typedef		unsigned long		uint32;
typedef		signed long		int32;
typedef		unsigned char		bool;
#endif



//#include 			<stdio.h>		//标准输入/输出
//#include 			<string.h>		//字符串
#include 			<creg.h>
#include 			"memap.h"		//ROCK2寄存器文件
#include 			"epphal.h"		//硬件层头文件


#include			"bulkonly.h"		//BULK-ONLY头文件
#include 			"USB20.h"		//USB部分总头文件
#include 			"flash.h"			//FLASH和FTL头文件
#include 			"fat.h"			//FAT头文件
#include 			"file.h"			//本地文件系统头文件
#include 			"dir.h"			//目录管理头文件
#include 			"sd.h"			//SD卡头文件
//#include 		"chkdsk.h"		//chkdsk头文件
//#include 		"ATA_IF.h"		//ATA硬件接口头文件
//#include 		"ATA.h"			//ATA函数接口头文件
//#include 		"sdram.h"		//SDRAM头文件
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

