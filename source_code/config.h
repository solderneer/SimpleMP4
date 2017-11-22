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

#define		IN_SYSTEM		//Ƕ�뵽ϵͳ���ʱ����
//#define		K_FAT12		//�����Ƿ�֧��FAT12
#define		ENCODE			//����ʹ���ļ�ϵͳд
#define		USB				//����ʹ��USB
#define		LFORMAT		//����ʹ�ܵ͸�
//#define		WDT_EN		//����ʹ�ܵ͸�
//#define		DEBUG
#define		RECOVERY
#define		USB_HS_EN		//����ʹ��USB HS
#define		DMA_INT_EN	//����DMA�ж�ʹ��
#define		OT2			//����ʹ��2���Ż�����
#define		DMA_BULKIN_EN	//����DMA�ж�ʹ��
#define		MK_DIR_EN		//���彨Ŀ¼ʹ��
//#define		OS_FILE		//����OS�µ��ļ�ϵͳ
#define		SYS_PROTECT	//����ϵͳ������дʹ��
#define		FLASH_DISK		1		//����FLASH��ʹ��
#define		SD_CARD_EN	0		//����SD��ʹ��
#define		CD_ROM			0		//�������ʹ��

#ifndef IN_SYSTEM
#define	CPU_PACK_PIN 144
#else
#include		"Macro.h"
#endif

#ifdef USB
#define		ENCODE			//����ʹ���ļ�ϵͳд
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



//#include 			<stdio.h>		//��׼����/���
//#include 			<string.h>		//�ַ���
#include 			<creg.h>
#include 			"memap.h"		//ROCK2�Ĵ����ļ�
#include 			"epphal.h"		//Ӳ����ͷ�ļ�


#include			"bulkonly.h"		//BULK-ONLYͷ�ļ�
#include 			"USB20.h"		//USB������ͷ�ļ�
#include 			"flash.h"			//FLASH��FTLͷ�ļ�
#include 			"fat.h"			//FATͷ�ļ�
#include 			"file.h"			//�����ļ�ϵͳͷ�ļ�
#include 			"dir.h"			//Ŀ¼����ͷ�ļ�
#include 			"sd.h"			//SD��ͷ�ļ�
//#include 		"chkdsk.h"		//chkdskͷ�ļ�
//#include 		"ATA_IF.h"		//ATAӲ���ӿ�ͷ�ļ�
//#include 		"ATA.h"			//ATA�����ӿ�ͷ�ļ�
//#include 		"sdram.h"		//SDRAMͷ�ļ�
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

