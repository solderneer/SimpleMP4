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
//DRM9 ID 如果该表要做修改，对应于BulkOnly.h中的InquirySnData[]要做同样的修改
//
unsigned int g_pmid[8] =            
{ 
    0x3233, 0x4445, 0x3845, 0x3241, 0x3934, 
    0x4544, 0x4343, 0x3046
};

/***************************************************************************
函数描述:修改DRM9 ID值
入口参数:
出口参数:
调用函数:
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
//USB Device 厂商信息
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

//USB Device Mp3 Description，must be 24 words.
unsigned char USBDeviceMp3Description[] DRAM_USB_TABLE=
{
	'R','O','C','K','C','H','I','P',	//Vendor Information		0~7

	'U','S','B',' ','M','P','3',	//Product Idernification	8~23
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
 };

//USB Device Sd Description，must be 24 words.
unsigned char USBDeviceSDDescription[] DRAM_USB_TABLE=
{

	'R','O','C','K','C','H','I','P',	//Vendor Information		0~7

	'U','S','B',' ',' ','S','D',	//Product Idernification	8~23
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
 };
/***************************************************************************
函数描述:USB Device 描述修改函数
入口参数:
出口参数:
调用函数:
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
    Modify_DRM9ID();//修改DRM9的ID值
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
** 函数名称	:SaveEncInfo
** 功能描述	:录音时保存参数, 以便于掉电恢复
** 输　入	:RecoveryInfo结构体(文件名, 文件首簇, 目录簇)
** 输　出	:无
** 全局变量	:无
** 调用模块	:
** 说     明		:RecoveryInfo结构体由录音分配首簇后存FLASH
********************************************************************************************************/
IRAM_IDLE
void SaveEncInfo(RCV_INFO* RecoveryInfo)
{
}


/*********************************************************************************************************
** 函数名称	:ClearEncInfo
** 功能描述	:清除录音时保存的掉电恢复信息
** 输　入	:无
** 输　出	:无
** 全局变量	:无
** 调用模块	:
** 说     明		:只把RecoveryInfo结构体中的Flag赋0
********************************************************************************************************/
IRAM_IDLE
void ClearEncInfo(void)
{
}

/***************************************************************************
函数描述:检测VBUS
入口参数:无
出口参数:VBUS存在返回1, 不存在返回0
调用函数:
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
函数描述:检测SD卡
入口参数:无
出口参数:卡存在返回1, 不存在返回0
调用函数:
***************************************************************************/
IRAM_FAT
bool CheckCard(void)
{
	bool detected=0;
	
//#if(CPU_PACK_PIN==144)
//	if ((read_XDATA(GPIO_PDAT2) & 0x1000) == 0)	//Sp2.12 D卡插入检测
//#elif (CPU_PACK_PIN==100 )
//	if ((read_XDATA(GPIO_PDAT1) & 0x0008) != 0)	//p1.3 SD卡插入检测
//#elif (CPU_PACK_PIN==128)
//	if ((read_XDATA(GPIO_PDAT1) & 0x0010) != 0)	//p1.4 SD卡插入检测
//#endif
    if (SDCARD_INSERT())	//p1.4 SD卡插入检测
		detected=1;
	return (detected);
}
