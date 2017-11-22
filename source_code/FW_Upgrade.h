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

//1�����ò���
#define 	MAX_LUN					2			/*���֧�ֵ��߼��豸��*/

//1��������

#define	MAX_CDBLEN      				0x10		//���CBW����

/*******************************************************************
CSW����״ֵ̬
*******************************************************************/
#define	CSW_GOOD					0x00		//����ͨ��
#define	CSW_FAIL					0x01		//����ʧ��

/*******************************************************************
����׶�
*******************************************************************/
#define	K_CommandPhase			0x00
#define	K_OutDataPhase				0x01
#define 	K_InDataPhase				0x81
#define 	K_InCSWPhase				0x08

/*******************************************************************
�̼��������
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

//1����ԭ������
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

//1�ṹ����
typedef struct _FWUSBMSDC_CSW
{
        uint8	dCSWSignature[4];
        uint8	dCSWTag[4];
        uint8	dCSWDataResidue[4];
        uint8	bCSWStatus;
} FWUSBMSDC_CSW;


//1ȫ�ֱ���
#undef	EXT
#ifdef	IN_FW_Upgrade
		#define	EXT
#else
		#define	EXT		extern
#endif		

	EXT		uint8 			FWStatusPhase FIRMWARE_BSS2;			//״̬�׶�״̬��
	EXT		uint8 			FWCmdPhase FIRMWARE_BSS2;			//����׶�״̬��
	EXT		uint32			FWdCSWDataResidueVal FIRMWARE_BSS2;	//ʣ���ֽ���
	EXT		uint32			FWdCBWDaTrLen FIRMWARE_BSS2;			//�����
	EXT		uint8			*G_DataInBuff FIRMWARE_BSS2;			//IN��������ָ��

	EXT		uint16			FW_XferDataLen FIRMWARE_BSS2;
	EXT		uint16 			FW_DataLenCnt FIRMWARE_BSS2;
	EXT		uint32 			FW_Write10PBA FIRMWARE_BSS2;
	EXT		uint32 			FW_Read10PBA FIRMWARE_BSS2;
	EXT		uint16 			FW_WriteSpareCol FIRMWARE_BSS2;
	EXT		FWUSBMSDC_CSW  	FWpCSW FIRMWARE_BSS2;
	EXT		uint8 			G_FlashBuff[512] FIRMWARE_BSS2;


//1�����
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

