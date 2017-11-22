//******************************************************************************
//FunUSB.c          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#define _IN_FUNUSB

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "Lcd.h"
#include "Resource.h"
#include "MsgDef.h"
#include "Global.h"

#include "Config.h"
#include "Message.h"

void UsbInCPUInit(void);
void FunUSBTransInit(void);

unsigned int UsbTransStatus;
extern int UsbHsEnable;
#ifdef _DRM9_ENABLE_
//DRM9 ID ����ñ�Ҫ���޸ģ���Ӧ��BulkOnly.h�е�InquirySnData[]Ҫ��ͬ�����޸�
//
unsigned int g_pmid[8] =            
{ 
    0x3233, 0x4445, 0x3845, 0x3241, 0x3934, 
    0x4544, 0x4343, 0x3046
};

/***************************************************************************
��������:�޸�DRM9 IDֵ
��ڲ���:
���ڲ���:
���ú���:
***************************************************************************/
__attribute__((section(".usb_text,\"ax\"")))
void Modify_DRM9ID(){
    unsigned int i,j;
    
    InquirySnData[3] = 16;
    j = 0;
    for(i = 4;i<20; i++)
    {
        InquirySnData[i] = (g_pmid[j]>>8)&0xFF;
        i++;
        InquirySnData[i] = g_pmid[j]&0xFF;
        j++;
    }
}
#endif

#if 1
//------------------------------------------------------------
//USB Device ������Ϣ
//must be 8 words.//unicode
unsigned char USBStringManufacture[] DRAM_USB_TABLE=
{
	'R', 0x00,
	'O', 0x00,
	'C', 0x00,
	'K', 0x00,
	'C', 0x00,
	'H', 0x00,
	'I', 0x00,
	'P', 0x00
};
//must be 8 words.//unicode
unsigned char USBStringProduct[] DRAM_USB_TABLE=
{
	'R', 0x00,
	'O', 0x00,
	'C', 0x00,
	'K', 0x00,
	' ', 0x00,
	'M', 0x00,
	'P', 0x00,
	'3', 0x00
};

//USB Device Mp3 Description��must be 24 words.
unsigned char USBDeviceMp3Description[] DRAM_USB_TABLE=
{
	'R','O','C','K','C','H','I','P',	//Vendor Information		0~7

	'U','S','B',' ','M','P','3',	//Product Idernification	8~23
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
 };

//USB Device Sd Description��must be 24 words.
unsigned char USBDeviceSDDescription[] DRAM_USB_TABLE=
{

	'R','O','C','K','C','H','I','P',	//Vendor Information		0~7

	'U','S','B',' ',' ','S','D',	//Product Idernification	8~23
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
 };
/***************************************************************************
��������:USB Device �����޸ĺ���
��ڲ���:
���ڲ���:
���ú���:
***************************************************************************/
__attribute__((section(".usb_text,\"ax\"")))
void ModifyUSBDeviceDescription(){
    unsigned int i,j;
    j = 0;
    for(i = 2;i<18; i++)
    {
        StringManufacture[i] = USBStringManufacture[j];
        StringProduct[i] = USBStringProduct[j];
        j++;
    }
    j = 0;    
    for(i = 8;i<32; i++)
    {
        SCSI_INQUIRY[i] = USBDeviceMp3Description[j];
        SCSI_INQUIRY_SD[i] = USBDeviceSDDescription[j];
        j++;
    }
}
#endif
//***************************************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".usb_text,\"ax\"")))
void FunUSBInit(void)
{
    if (Sys1InitFlag.bc.bUsbModuleInitEnable) {
        Sys1InitFlag.bc.bUsbModuleInitEnable = 0;
    }
#ifdef _DRM9_ENABLE_
    Modify_DRM9ID();//�޸�DRM9��IDֵ
#endif
    UsbInCPUInit();
    
    MemorySelect = 0;
    PrewMemorySelect = MemorySelect;
    MediaSetup(0);
    
    UsbTransStatus = 1;
    
    USBInit();
    PostFun1Message(MSG_FUNCTION1_USB_TRANS,0,0,0,0/*,0,0,0*/);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".usb_text,\"ax\"")))
void UsbInCPUInit(void)
{
    unsigned int temp;
    
    bitclear_creg(%imask,15);
    
    SysCpuSetFreq(72);
    SysTimer0Set(72,25000);//SysTimer0Set(72,10000);
#if (FLASHROM_TYPE == _MLC_)
    write_mem(FMWAIT,  0x2083);
#endif
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0015);
    write_mem32(DMAR_ClearBlock,0x00000007);
    
    write_creg(%ip0,0x0000);
    write_creg(%ip1,0x0400);
    write_creg(%ireq,0x0000);
    
    //bitset_creg(%imask,5);
    //bitset_creg(%imask,10);
    bitset_creg(%imask,7);
    bitset_creg(%imask,15);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".usb_text,\"ax\"")))
void FunUSBTransInit(void)
{
    //VariableInit();
	CheckFileSystem();
    USBInit();
    PostFun1Message(MSG_FUNCTION1_USB_TRANS,0,0,0,0/*,0,0,0*/);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".usb_text,\"ax\"")))
void FunUSBTrans(void)
{
//    if ((UsbConnected == 0) && (UsbTranFlag == 1)){
//        UsbTranFlag = 2;
//    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".usb_text,\"ax\"")))
void UsbBusyHook(void)
{
    if (UsbTransStatus == 0) {
        UsbTransStatus = 1;
//        DisplayPicture(0,0,LCD_MAX_XSIZE,LCD_MAX_YSIZE,(unsigned long)(IMAGE_IN_FLASH_BASE_ADDR) + 4*PicPerPage);
//        LCD_NFDispStringAt(120, 158, "Busy....");
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".usb_text,\"ax\"")))
void UsbReadyHook(void)
{
    if (UsbTransStatus != 0) {
        UsbTransStatus= 0;
//        DisplayPicture(0,0,LCD_MAX_XSIZE,LCD_MAX_YSIZE,(unsigned long)(IMAGE_IN_FLASH_BASE_ADDR) + 4*PicPerPage);
//        LCD_NFDispStringAt(50, 158, "Ready...");
    }
}
//******************************************************************************
/*********************************************************************************************************
** ��������	:SaveEncInfo
** ��������	:¼��ʱ�������, �Ա��ڵ���ָ�
** �䡡��	:RecoveryInfo�ṹ��(�ļ���, �ļ��״�, Ŀ¼��)
** �䡡��	:��
** ȫ�ֱ���	:��
** ����ģ��	:
** ˵     ��		:RecoveryInfo�ṹ����¼�������״غ��FLASH
********************************************************************************************************/
IRAM_IDLE
void SaveEncInfo(RCV_INFO* RecoveryInfo)
{
}


/*********************************************************************************************************
** ��������	:ClearEncInfo
** ��������	:���¼��ʱ����ĵ���ָ���Ϣ
** �䡡��	:��
** �䡡��	:��
** ȫ�ֱ���	:��
** ����ģ��	:
** ˵     ��		:ֻ��RecoveryInfo�ṹ���е�Flag��0
********************************************************************************************************/
IRAM_IDLE
void ClearEncInfo(void)
{
}

/***************************************************************************
��������:���VBUS
��ڲ���:��
���ڲ���:VBUS���ڷ���1, �����ڷ���0
���ú���:
***************************************************************************/
IRAM_FAT
bool CheckVbus(void)
{
	bool Vbus=0;
	
//#if(CPU_PACK_PIN==144)
//	if ((read_XDATA(GPIO_PDAT2) & 0x4000) == 0)
//#elif (CPU_PACK_PIN==100 )
//	if ((read_XDATA(GPIO_PDAT2) & 0x0200) != 0)
//#elif (CPU_PACK_PIN==128)
//	if ((read_XDATA(GPIO_PDAT1) & 0x0040) != 0)
//#endif
    if (USB_INSERT())
		Vbus=1;
	return (Vbus);
}


/***************************************************************************
��������:���SD��
��ڲ���:��
���ڲ���:�����ڷ���1, �����ڷ���0
���ú���:
***************************************************************************/
IRAM_FAT
bool CheckCard(void)
{
	bool detected=0;
	
//#if(CPU_PACK_PIN==144)
//	if ((read_XDATA(GPIO_PDAT2) & 0x1000) == 0)	//Sp2.12 D��������
//#elif (CPU_PACK_PIN==100 )
//	if ((read_XDATA(GPIO_PDAT1) & 0x0008) != 0)	//p1.3 SD��������
//#elif (CPU_PACK_PIN==128)
//	if ((read_XDATA(GPIO_PDAT1) & 0x0010) != 0)	//p1.4 SD��������
//#endif
    if (SDCARD_INSERT())	//p1.4 SD��������
		detected=1;
	return (detected);
}
