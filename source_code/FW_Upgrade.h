/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	FW_Upgrade.h
Author:		XUESHAN LIN
Created:		22th Nov. 2005
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/

#ifndef _FW_UPGRADE_H
#define _FW_UPGRADE_H

//1可配置参数
#define 	MAX_LUN					2			/*最大支持的逻辑设备数*/

//1常量定义

#define	MAX_CDBLEN      				0x10		//最长的CBW长度

/*******************************************************************
CSW返回状态值
*******************************************************************/
#define	CSW_GOOD					0x00		//命令通过
#define	CSW_FAIL					0x01		//命令失败

/*******************************************************************
传输阶段
*******************************************************************/
#define	K_CommandPhase			0x00
#define	K_OutDataPhase				0x01
#define 	K_InDataPhase				0x81
#define 	K_InCSWPhase				0x08

/*******************************************************************
固件升级命令集
*******************************************************************/
#define 	K_FW_TEST_UNIT_READY		0x00
#define 	K_FW_READ_FLASH_ID		0x01
#define 	K_FW_SET_DEVICE_ID		0x02
#define 	K_FW_TEST_BAD_BLOCK		0x03
#define 	K_FW_READ_10				0x04
#define 	K_FW_WRITE_10				0x05
#define 	K_FW_ERASE_10				0x06
#define 	K_FW_WRITE_SPARE			0x07
#define 	K_FW_READ_SPARE			0x08
#define 	K_FW_RESET				0xff

    #define     FIRMWARE_DATA2  __attribute__((section(".firmware_data, \"aw\"")))
    #define     FIRMWARE_BSS2  __attribute__((section(".firmware_bss, \"aw\"")))

//1函数原型声明
extern 	void 	FWCmd(void);
extern	void    FWInPacket(void);
extern	void    FWOutPacket(void);

extern 	void 	FW_TestUnitReady(void);
extern 	void 	FW_ReadFlashID(void);
extern 	void 	FW_SetDeviceID(void);
extern 	void 	FW_TestBadBlock(void);
extern 	void 	FW_Read10(void);
extern 	void 	FW_Write10(void);
extern 	void 	FW_Erase10(void);
extern 	void 	FW_WriteSpare(void);
extern 	void 	FW_ReadSpare(void);
extern 	void 	FW_Reset(void);
extern 	bool 	FWCBWValidVerify(void);
extern 	void 	FWCSWHandler(uint8 HostDevCase, uint16 DeviceTrDataLen);



/*******************************************************************/

//1结构定义
typedef struct _FWUSBMSDC_CSW
{
        uint8	dCSWSignature[4];
        uint8	dCSWTag[4];
        uint8	dCSWDataResidue[4];
        uint8	bCSWStatus;
} FWUSBMSDC_CSW;


//1全局变量
#undef	EXT
#ifdef	IN_FW_Upgrade
		#define	EXT
#else
		#define	EXT		extern
#endif		

	EXT		uint8 			FWStatusPhase FIRMWARE_BSS2;			//状态阶段状态字
	EXT		uint8 			FWCmdPhase FIRMWARE_BSS2;			//命令阶段状态字
	EXT		uint32			FWdCSWDataResidueVal FIRMWARE_BSS2;	//剩余字节数
	EXT		uint32			FWdCBWDaTrLen FIRMWARE_BSS2;			//命令长度
	EXT		uint8			*G_DataInBuff FIRMWARE_BSS2;			//IN包的数据指针

	EXT		uint16			FW_XferDataLen FIRMWARE_BSS2;
	EXT		uint16 			FW_DataLenCnt FIRMWARE_BSS2;
	EXT		uint32 			FW_Write10PBA FIRMWARE_BSS2;
	EXT		uint32 			FW_Read10PBA FIRMWARE_BSS2;
	EXT		uint16 			FW_WriteSpareCol FIRMWARE_BSS2;
	EXT		FWUSBMSDC_CSW  	FWpCSW FIRMWARE_BSS2;
	EXT		uint8 			G_FlashBuff[512] FIRMWARE_BSS2;


//1表格定义
#ifdef IN_FW_Upgrade
FIRMWARE_DATA2
uint8 USB_CMD[]=
{
	K_FW_TEST_UNIT_READY,
	K_FW_READ_FLASH_ID,
	K_FW_SET_DEVICE_ID,
	K_FW_TEST_BAD_BLOCK,
	K_FW_READ_10,
	K_FW_WRITE_10,
	K_FW_ERASE_10,
	K_FW_WRITE_SPARE,
	K_FW_RESET
};
FIRMWARE_DATA2
uint8 *USB_CMD_Desr[]=
{
	"TEST_UNIT_READY",
	"READ_FLASH_ID",
	"SET_DEVICE_ID",
	"TEST_BAD_BLOCK",
	"READ_10",
	"WRITE_10",
	"ERASE_10",
	"WRITE_SPARE",
	"RESET",
};
#else
#endif
#endif

