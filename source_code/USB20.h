/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	USB.H
Author:		XUESHAN LIN
Created:		29th SEP 2004
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/
#ifndef _USB20_H
#define _USB20_H

//1可配置参数
#if MAX_LUN>1
#define 		iManufactuer		1		/*厂商描述符字符串索引*/
#define 		iProduct				2		/*产品描述符字符串索引*/
#define 		iSerialNumber		3		/*设备序列号字符串索引*/
#define 		iConfiguration		0// 4		/*配置描述符字符串索引*/
#define 		iInterface			0// 5		/*接口描述符字符串索引*/
#else
#define 		iManufactuer		0		/*厂商描述符字符串索引*/
#define 		iProduct				0		/*产品描述符字符串索引*/
#define 		iSerialNumber		0		/*设备序列号字符串索引*/
#define 		iConfiguration		0// 3		/*配置描述符字符串索引*/
#define 		iInterface			0// 4		/*接口描述符字符串索引*/
#endif

//1结构定义
//设备描述符结构
typedef struct _USB_DEVICE_DESCRIPTOR 
{
    uint8		bLength;
    uint8		bDescriptorType;
#ifdef __C51__
    uint16 		bcdUSB;
#endif
#ifdef __ZSP__
    uint8 		bcdUSB;
    uint8 		reserve1;
#endif
    uint8 		bDeviceClass;
    uint8 		bDeviceSubClass;
    uint8 		bDeviceProtocol;
    uint8 		bMaxPacketSize0;
#ifdef __C51__	
    uint16 		idVendor;
    uint16 		idProduct;
    uint16 		bcdDevice;
#endif
#ifdef __ZSP__
    uint8 		idVendor;
    uint8 		reserve2;
    uint8 		idProduct;
    uint8 		reserve3;
    uint8 		bcdDevice;
    uint8 		reserve4;
#endif
    uint8 		iiManufacturer;
    uint8 		iiProduct;
    uint8 		iiSerialNumber;
    uint8 		bNumConfigurations;
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;

//端点描述符结构
typedef struct _USB_ENDPOINT_DESCRIPTOR 
{
    uint8 		bLength;
    uint8 		bDescriptorType;
    uint8 		bEndpointAddress;
    uint8 		bmAttributes;
#ifdef __C51__	
    uint16 		wMaxPacketSize;
#endif
#ifdef __ZSP__
    uint8 		wMaxPacketSize;
    uint8 		reserve1;
#endif
    uint8 		bInterval;
} USB_ENDPOINT_DESCRIPTOR, *PUSB_ENDPOINT_DESCRIPTOR;


//配置描述符结构
typedef struct _USB_CONFIGURATION_DESCRIPTOR 
{
    uint8 		bLength;
    uint8 		bDescriptorType;
#ifdef __C51__	
    uint16		wTotalLength;
#endif
#ifdef __ZSP__
    uint8		wTotalLength;
    uint8		reserve1;
#endif
    uint8 		bNumInterfaces;
    uint8 		bConfigurationValue;
    uint8 		iiConfiguration;
    uint8 		bmAttributes;
    uint8 		MaxPower;
} USB_CONFIGURATION_DESCRIPTOR, *PUSB_CONFIGURATION_DESCRIPTOR;

//高速设备的其它速度配置描述符
typedef struct _OTHER_SPEED_CONFIG_DESCRIPTOR
{
    uint8 		bLength;
    uint8 		bDescriptorType;
#ifdef __C51__	
    uint16		wTotalLength;
#endif
#ifdef __ZSP__
    uint8		wTotalLength;
    uint8		reserve1;
#endif
    uint8 		bNumInterfaces;
    uint8 		bConfigurationValue;
    uint8 		iiConfiguration;
    uint8 		bmAttributes;
    uint8 		MaxPower;
}OTHER_SPEED_CONFIG_DESCRIPTOR;


//接口描述符结构
typedef struct _USB_INTERFACE_DESCRIPTOR 
{
    uint8 		bLength;
    uint8 		bDescriptorType;
    uint8 		bInterfaceNumber;
    uint8 		bAlternateSetting;
    uint8 		bNumEndpoints;
    uint8 		bInterfaceClass;
    uint8 		bInterfaceSubClass;
    uint8 		bInterfaceProtocol;
    uint8 		iiInterface;
} USB_INTERFACE_DESCRIPTOR, *PUSB_INTERFACE_DESCRIPTOR;

//字符串描述符结构
typedef struct _USB_STRING_DESCRIPTOR 
{
    uint8 		bLength;
    uint8 		bDescriptorType;
    uint8 		bString[1];
} USB_STRING_DESCRIPTOR, *PUSB_STRING_DESCRIPTOR;


//高速设备限制描述符
typedef struct _HS_DEVICE_QUALIFIER
{
	uint8	bLength;			//length of HS Device Descriptor
	uint8	bQualifier; 			//HS Device Qualifier Type
#ifdef __C51__	
	uint16	wVersion;			// USB 2.0 version
#endif
#ifdef __ZSP__
	uint8	wVersion;			// USB 2.0 version
	uint8	reserve1;
#endif
	uint8	bDeviceClass;		//Device class
	uint8	bDeviceSubClasss;	//Device SubClass
	uint8	bProtocol;			//Device Protocol Code
	uint8	MaxPktSize;			//Maximum Packet SIze for other speed
	uint8	bOther_Config;		//Number of Other speed configurations
	uint8	Reserved;			//Reserved
}HS_DEVICE_QUALIFIER;

//电源描述符结构
typedef struct _USB_POWER_DESCRIPTOR
{
    uint8 		bLength;
    uint8 		bDescriptorType;
    uint8 		bCapabilitiesFlags;
    uint16 		EventNotification;
    uint16 		D1LatencyTime;
    uint16 		D2LatencyTime;
    uint16		D3LatencyTime;
    uint8 		PowerUnit;
    uint16 		D0PowerConsumption;
    uint16 		D1PowerConsumption;
    uint16 		D2PowerConsumption;
} USB_POWER_DESCRIPTOR, *PUSB_POWER_DESCRIPTOR;


//通用描述符结构
typedef struct _USB_COMMON_DESCRIPTOR 
{
    uint8 		bLength;
    uint8 		bDescriptorType;
} USB_COMMON_DESCRIPTOR, *PUSB_COMMON_DESCRIPTOR;


//标准HUB描述符结构
typedef struct _USB_HUB_DESCRIPTOR 
{
    uint8        	bDescriptorLength;      	// Length of this descriptor
    uint8        	bDescriptorType;        	// Hub configuration type
    uint8        	bNumberOfPorts;         	// number of ports on this hub
    uint16	    	wHubCharacteristics;    	// Hub Charateristics
    uint8       	bPowerOnToPowerGood;  // port power on till power good in 2ms
    uint8       	bHubControlCurrent;     	// max current in mA
    // room for 255 ports power control and removable bitmask
    uint8        	bRemoveAndPowerMask[64];
} USB_HUB_DESCRIPTOR, *PUSB_HUB_DESCRIPTOR;


#define		FULL_SPEED		0
#define		HIGH_SPEED		1
//事件组标志结构
typedef union _epp_flags
{
	struct _flags
	{
		uint8 HS_FS_State	:1;
		uint8 bus_reset           :1;
		uint8 suspend             	:1;
		uint8 setup_packet  	:1;
		uint8 remote_wakeup	:1;
		uint8 Alter_Interface	:1;
		uint8 control_state	:2;

		uint8 configuration	:1;
		uint8 WdtUsbEn		:1;
		uint8 ep1_rxdone		:1;
		uint8 setup_dma		:2;	 // V2.3
		uint8 dma_state		:2;
		uint8 power_down	:1;	 // Smart Board
	} bits;
	uint16 value;
} EPPFLAGS;

//设备请求结构
typedef struct _device_request
{
	uint8 	bmRequestType;
	uint8 	bRequest;
#ifdef __C51__	
	uint16	wValue;
	uint16	wIndex;
	uint16	wLength;
#endif
#ifdef __ZSP__
	uint8	wValue;
	uint8	reserved1;
	uint8	wIndex;
	uint8	reserved2;
	uint8	wLength;
	uint8	reserved3;
#endif
} DEVICE_REQUEST;

//I/O请求结构(用于DMA)
typedef struct _IO_REQUEST 
{
	uint16	uAddressL;
	uint8	bAddressH;
	uint16	uSize;
	uint8	bCommand;
} IO_REQUEST, *PIO_REQUEST;


#define MAX_CONTROLDATA_SIZE	8		//控制管道最大包长
//带数据的设备请求结构
typedef struct _control_xfer
{
	DEVICE_REQUEST DeviceRequest;
#ifdef __C51__	
	uint16 	wLength;
	uint16 	wCount;
#endif
#ifdef __ZSP__
	uint8 	wLength;
	uint8	reserved1;
	uint8 	wCount;
	uint8	reserved2;
#endif
	uint8 	*pData;
	uint8 	dataBuffer[MAX_CONTROLDATA_SIZE];
} CONTROL_XFER;



typedef struct _TWAIN_FILEINFO 
{
	uint8	bPage;		// bPage bit 7 - 5 map to uSize bit 18 - 16
	uint8	uSizeH;		// uSize bit 15 - 8
	uint8	uSizeL;		// uSize bit 7 - 0
} TWAIN_FILEINFO, *PTWAIN_FILEINFO;



//1常量定义
#define 		MSB(x)    		(((x) >> 8) & 0xFF)
#define 		LSB(x)    		((x) & 0xFF)

#ifndef 		FALSE
#define 		FALSE  	 		0
#endif

#ifndef 		TRUE
#define 		TRUE    			(!FALSE)
#endif


#define 		NUM_ENDPOINTS		2		//端点数除0外
#define 		CONFIG_DESCRIPTOR_LENGTH    sizeof(USB_CONFIGURATION_DESCRIPTOR) \
											+ sizeof(USB_INTERFACE_DESCRIPTOR) \
											+ (NUM_ENDPOINTS * sizeof(USB_ENDPOINT_DESCRIPTOR))

//Rock2
#define 		USB_ADDR					(USB_BASE+0x00/2)
#define 		USB_INT_SRC_IN				(USB_BASE+0x02/2)
#define 		USB_INT_SRC_OUT			(USB_BASE+0x04/2)
#define 		USB_INT_EN_IN				(USB_BASE+0x06/2)
#define 		USB_INT_EN_OUT			(USB_BASE+0x08/2)
#define 		USB_INT_SRC_USB			(USB_BASE+0x0a/2)
#define 		USB_FRAME					(USB_BASE+0x0c/2)
#define 		USB_INDEX					(USB_BASE+0x0e/2)

#define 		USB_MAX_PACK_IN			(USB_BASE+0x10/2)
#define 		USB_CSR0					(USB_BASE+0x12/2)
#define 		USB_CSR_IN					(USB_BASE+0x12/2)
#define 		USB_MAX_PACK_OUT			(USB_BASE+0x14/2)
#define 		USB_CSR_OUT				(USB_BASE+0x16/2)
#define 		USB_COUNT0				(USB_BASE+0x18/2)
#define 		USB_COUNT_OUT				(USB_BASE+0x18/2)

//0x1A~0x1F RESERVE
#define 		USB_FIFO_BASE				(USB_BASE+0x20/2)

//USB_INT_EN_IN
#define 		USB_INT_ENDP0				0x0001
#define 		USB_INT_ENDP1IN			0x0002

#define 		USB_INT_ENDP1OUT			0x0002

#define 		USB_INT_SUSPEND			0x0001
#define 		USB_INT_RESUME			0x0002
#define 		USB_INT_RESET				0x0004
#define 		USB_INT_SOF				0x0008

//USB_POWER
#define 		USB_SOFTCONNECT			(0x0040 << 8)
#define 		USB_HS_ENABLE				(0x0020 << 8)
#define 		USB_INT_HS_STAT			(0x0010 << 8)
#define 		USB_RESUME				(0x0004 << 8)
#define 		USB_SUSPEND_EN			(0x0001 << 8)

//csr0
#define 		CSR0_OUT_RDY				0x0001
#define 		CSR0_IN_RDY				0x0002
#define 		CSR0_SENT_STALL			0x0004
#define 		CSR0_DATA_END				0x0008
#define 		CSR0_SETUP_END			0x0010
#define 		CSR0_SEND_STALL			0x0020
#define 		CSR0_OUT_SEVD				0x0040
#define 		CSR0_SETUP_SEVD			0x0080

//INCSR
#define 		CSR_IN_ISO					0x4000
#define 		CSR_IN_MODE				0x2000
#define 		CSR_IN_FDT					0x0800
#define 		CSR_IN_CDT					0x0040
#define 		CSR_IN_SENT_STALL			0x0020
#define 		CSR_IN_SEND_STALL			0x0010
#define 		CSR_IN_FLUSH_FIFO			0x0008
#define 		CSR_IN_FIFO_NotEmpty		0x0002
#define 		CSR_IN_PKT_RDY			0x0001

//OUTCSR
#define 		CSR_OUT_ISO				0x4000
#define 		CSR_OUT_DisNyet			0x1000
#define 		CSR_OUT_CDT				0x0080
#define 		CSR_OUT_SENT_STALL		0x0040
#define 		CSR_OUT_SEND_STALL		0x0020
#define 		CSR_OUT_FLUSH_FIFO		0x0010
#define 		CSR_OUT_PKT_RDY			0x0001

//IntUSB
#define 		INT_USB_SOF				0x0008
#define 		INT_USB_RESET				0x0004
#define 		INT_USB_RESUME			0x0002
#define 		INT_USB_SUSPEND			0x0001


//TestMode
#define 		TEST_FS					(0x20 << 8)
#define 		TEST_HS					(0x10 << 8)
#define 		TEST_PACKET				(0x08 << 8)
#define 		TEST_K						(0x04 << 8)
#define 		TEST_J						(0x02 << 8)
#define 		TEST_NAK					(0x01 << 8)


//mainloop.h
#define 		MAX_ENDPOINTS      			(uint8)0x3
#define 		EP0_TX_FIFO_SIZE   			64
#define 		EP0_RX_FIFO_SIZE   			64
#define 		EP0_PACKET_SIZE    			64

#define 		FS_BULK_RX_SIZE    			64
#define 		FS_BULK_TX_SIZE    			64
#define 		HS_BULK_RX_SIZE    			512
#define 		HS_BULK_TX_SIZE    			512


#define 		USB_IDLE           				0
#define 		USB_TRANSMIT       			1
#define 		USB_RECEIVE        			2

#define 		USB_CLASS_CODE_TEST_CLASS_DEVICE		0xdc
#define 		USB_SUBCLASS_CODE_TEST_CLASS_D12		0xa0
#define 		USB_PROTOCOL_CODE_TEST_CLASS_D12		0xb0


#define 		USB_RECIPIENT            		(uint8)0x1F
#define 		USB_RECIPIENT_DEVICE     	(uint8)0x00
#define 		USB_RECIPIENT_INTERFACE  	(uint8)0x01
#define 		USB_RECIPIENT_ENDPOINT   	(uint8)0x02

#define 		USB_REQUEST_TYPE_MASK    	(uint8)0x60
#define 		USB_STANDARD_REQUEST     	(uint8)0x00
#define 		USB_CLASS_REQUEST        	(uint8)0x20
#define 		USB_VENDOR_REQUEST       	(uint8)0x40

#define 		USB_REQUEST_MASK         	(uint8)0x0F
#define 		DEVICE_ADDRESS_MASK      	0x7F

#define 		IN_TOKEN_DMA 				1
#define 		OUT_TOKEN_DMA 			0

#define 		DMA_BUFFER_SIZE			256

#define 		DMA_IDLE					0
#define 		DMA_RUNNING				1
#define 		DMA_PENDING				2

//厂商请求代码
#define 		SETUP_DMA_REQUEST 		0x0471
#define 		GET_FIRMWARE_VERSION    	0x0472
#define 		GET_SET_TWAIN_REQUEST   	0x0473
#define 		GET_BUFFER_SIZE		    	0x0474


//usb100.h
#define 		MAXIMUM_USB_STRING_LENGTH 				255

// values for the bits returned by the USB GET_STATUS command
#define 		USB_GETSTATUS_SELF_POWERED                	0x01
#define 		USB_GETSTATUS_REMOTE_WAKEUP_ENABLED	0x02


#define 		USB_DEVICE_DESCRIPTOR_TYPE                		0x01
#define 		USB_CONFIGURATION_DESCRIPTOR_TYPE         	0x02
#define 		USB_STRING_DESCRIPTOR_TYPE                		0x03
#define 		USB_INTERFACE_DESCRIPTOR_TYPE             	0x04
#define 		USB_ENDPOINT_DESCRIPTOR_TYPE              	0x05
#define 		USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE 	0x06
#define 		USB_OTHER_SPEED_CONF_DESCRIPTOR_TYPE 	0x07
#define 		USB_INTERFACE_POWER_DESCRIPTOR_TYPE 	0x08
#define 		USB_OTG_DESCRIPTOR_TYPE 					0x09
#define 		USB_DEBUG_DESCRIPTOR_TYPE 				0x0A
#define 		USB_IF_ASSOCIATION_DESCRIPTOR_TYPE 		0x0B

#define 		USB_DESCRIPTOR_MAKE_TYPE_AND_INDEX(d, i) ((uint16)((uint16)d<<8 | i))

// Values for bmAttributes field of an
// endpoint descriptor

#define 		USB_ENDPOINT_TYPE_MASK                    		0x03
#define 		USB_ENDPOINT_TYPE_CONTROL                 		0x00
#define 		USB_ENDPOINT_TYPE_ISOCHRONOUS             	0x01
#define 		USB_ENDPOINT_TYPE_BULK                    		0x02
#define 		USB_ENDPOINT_TYPE_INTERRUPT               		0x03


// definitions for bits in the bmAttributes field of a 
// configuration descriptor.
#define 		USB_CONFIG_POWERED_MASK                   		0xc0
#define 		USB_CONFIG_BUS_POWERED                    		0x80
#define 		USB_CONFIG_SELF_POWERED                   		0x40
#define 		USB_CONFIG_REMOTE_WAKEUP                  		0x20

// Endpoint direction bit, stored in address
#define 		USB_ENDPOINT_DIRECTION_MASK               	0x80

// test direction bit in the bEndpointAddress field of
// an endpoint descriptor.
#define 		USB_ENDPOINT_DIRECTION_OUT(addr)          	(!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define 		USB_ENDPOINT_DIRECTION_IN(addr)           	((addr) & USB_ENDPOINT_DIRECTION_MASK)

// USB defined request codes
#define 		USB_REQUEST_GET_STATUS                    		0x00
#define 		USB_REQUEST_CLEAR_FEATURE                 		0x01
#define 		USB_REQUEST_SET_FEATURE                   		0x03
#define 		USB_REQUEST_SET_ADDRESS                   		0x05
#define 		USB_REQUEST_GET_DESCRIPTOR                		0x06
#define 		USB_REQUEST_SET_DESCRIPTOR                		0x07
#define 		USB_REQUEST_GET_CONFIGURATION             	0x08
#define 		USB_REQUEST_SET_CONFIGURATION             	0x09
#define 		USB_REQUEST_GET_INTERFACE                 		0x0A
#define 		USB_REQUEST_SET_INTERFACE                 		0x0B
#define 		USB_REQUEST_SYNC_FRAME                    		0x0C


// defined USB device classes
#define 		USB_DEVICE_CLASS_RESERVED           			0x00
#define 		USB_DEVICE_CLASS_AUDIO              			0x01	//音频设备
#define 		USB_DEVICE_CLASS_COMMUNICATIONS     		0x02	//通讯设备
#define 		USB_DEVICE_CLASS_HUMAN_INTERFACE    		0x03	//人机接口
#define 		USB_DEVICE_CLASS_MONITOR            			0x04	//显示器
#define 		USB_DEVICE_CLASS_PHYSICAL_INTERFACE 	0x05	//物理接口
#define 		USB_DEVICE_CLASS_POWER              			0x06	//电源
#define 		USB_DEVICE_CLASS_PRINTER            			0x07	//打印机
#define 		USB_DEVICE_CLASS_STORAGE            			0x08	//海量存储
#define 		USB_DEVICE_CLASS_HUB                			0x09	//HUB
#define 		USB_DEVICE_CLASS_VENDOR_SPECIFIC    		0xFF	//

//define	USB subclass
#define		USB_SUBCLASS_CODE_RBC					0x01
#define		USB_SUBCLASS_CODE_SFF8020I				0x02
#define		USB_SUBCLASS_CODE_QIC157					0x03
#define		USB_SUBCLASS_CODE_UFI					0x04
#define		USB_SUBCLASS_CODE_SFF8070I				0x05
#define		USB_SUBCLASS_CODE_SCSI					0x06

//define	USB protocol
#define		USB_PROTOCOL_CODE_CBI0					0x00
#define		USB_PROTOCOL_CODE_CBI1					0x01
#define		USB_PROTOCOL_CODE_BULK					0x50


// USB defined Feature selectors
#define 		USB_FEATURE_ENDPOINT_STALL          		0x0000
#define 		USB_FEATURE_REMOTE_WAKEUP           		0x0001
#define 		USB_FEATURE_POWER_D0                			0x0002
#define 		USB_FEATURE_POWER_D1                			0x0003
#define 		USB_FEATURE_POWER_D2                			0x0004
#define 		USB_FEATURE_POWER_D3                			0x0005

//values for bmAttributes Field in USB_CONFIGURATION_DESCRIPTOR
#define 		BUS_POWERED                           				0x80
#define 		SELF_POWERED                          				0x40
#define 		REMOTE_WAKEUP                         				0x20

// USB power descriptor added to core specification
#define 		USB_SUPPORT_D0_COMMAND      				0x01
#define 		USB_SUPPORT_D1_COMMAND      				0x02
#define 		USB_SUPPORT_D2_COMMAND      				0x04
#define 		USB_SUPPORT_D3_COMMAND      				0x08
#define 		USB_SUPPORT_D1_WAKEUP       				0x10
#define 		USB_SUPPORT_D2_WAKEUP       				0x20



//1全局变量
#undef	EXT
#ifdef	IN_USB_ISR
		#define	EXT
#else
		#define	EXT		extern
#endif		
		
#ifdef USB
EXT		CONTROL_XFER 	ControlData		DRAM_USB;
EXT		EPPFLAGS 		bEPPflags			DRAM_USB;
EXT		uint8 			Ep2Status		DRAM_USB;
EXT		uint16 			BulkEndpSize		DRAM_USB;
EXT		uint8 			EpBuf[64]		DRAM_USB;
EXT		bool 	volatile	UsbConnected		DRAM_USB;
EXT		bool 	volatile	UsbHsEn			DRAM_USB;
EXT		uint8 	volatile	UsbResetCnt		DRAM_USB;
#endif



//1函数原型声明
//mainloop.c

//usbCtrl.c
extern	void 	USBIdle(void);
extern	void 	USBInit(void);
extern	void 	stall_ep0(void);
extern	void 	disconnect_USB(void);
extern	void 	connect_USB(void);
extern	void 	reconnect_USB(void);
extern	void 	init_unconfig(void);
extern	void 	init_config(void);
extern	void 	single_transmit(uint8 * buf, uint8 len);
extern	void 	code_transmit(uint8 * pRomData, uint16 len);
extern	void 	control_handler(void);

//chap_9.c
extern	void 	get_status(void);
extern	void 	clear_feature(void);
extern	void 	set_feature(void);
extern	void 	set_address(void);
extern	void 	get_descriptor(void);
extern	void 	get_configuration(void);
extern	void 	set_configuration(void);
extern	void 	get_interface(void);
extern	void 	set_interface(void);
extern	void 	reserved(void);
extern	void 	wait_ep0tx(void);
extern	void 	ep0in_ack(void);

//d12ci.c
extern	void 	MUSBHSFC_InitSFR(void);
extern	void 	MUSBHSFC_InitEndp(void);

extern	void 	MUSBHSFC_SetAddressEnable(uint8 bAddress);
extern	void 	MUSBHSFC_SetEndpointEnable(uint8 bEnable);
extern	uint8	MUSBHSFC_SelectEndpoint(uint8 bEndp);
extern	void 	MUSBHSFC_SetEndpointStatus(uint8 bEndp, uint8 bStalled);
extern	uint16 	MUSBHSFC_ReadEndpoint(uint8 endp, uint16 len, uint8 * buf);
extern	uint16 	MUSBHSFC_WriteEndpoint(uint8 endp, uint16 len, uint8 * buf);

//isr.c
extern	void 	usb_isr(void);
extern 	void 	bus_reset(void);
extern 	void 	setup(void);
extern 	void 	ep0_txdone(void);
extern 	void 	ep0_rxdone(void);
extern 	void 	main_txdone(void);
extern 	void 	main_rxdone(void);

//protodma.c
extern 	void 	get_firmware_version(void);
extern 	void 	get_buffer_size(void);
extern 	void 	read_write_register(void);

//epphal.c


//1表格定义
#ifdef USB
#ifdef	IN_USB_ISR
//设备描述符
USB_DEVICE_DESCRIPTOR DeviceDescr DRAM_USB_TABLE=
{
	sizeof(USB_DEVICE_DESCRIPTOR),				//描述符的大小18(1B)
	USB_DEVICE_DESCRIPTOR_TYPE,				//描述符的类型01(1B)
#ifdef __C51__	
	SWAP(0x0110),								//USB规划分布号(2B)
#endif
#ifdef __ZSP__
	LSB(0x0110),MSB(0x0110),						//USB规划分布号(2B)
#endif
	0,//	USB_CLASS_CODE_TEST_CLASS_DEVICE,					//1类型代码(由USB指定)(1B),0x00
	0, 0,											//子类型和协议(由USB分配)(2B)
	EP0_PACKET_SIZE,							//端点0最大包长(1B)
#ifdef __C51__
	SWAP(0x071b),
	SWAP(0x3203),
	SWAP(0x0100),								// 设备系列号

//	SWAP(0x0693),
//	SWAP(0x0003),
//	SWAP(0x0100),               						// 设备系列号

//	SWAP(0x0471),								//供应商ID(由USB分配)(2B)
//	SWAP(0x0666),								//产品ID(由厂商分配)(2B)
//	SWAP(0x0100),								//设备出厂编号(2B)
#endif
#ifdef __ZSP__
	LSB(0x071b),MSB(0x071b),
	LSB(0x3203),MSB(0x3203),
	LSB(0x0100),MSB(0x0100),
#endif
	iManufactuer, iProduct, iSerialNumber,				//厂商,产品,设备序列号字符串索引(3B)
	1											//可能的配置数(1B)
};

//配置描述符
USB_CONFIGURATION_DESCRIPTOR ConfigDescr DRAM_USB_TABLE=
{
	sizeof(USB_CONFIGURATION_DESCRIPTOR),		//描述符的大小9(1B)
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		//描述符的类型02(1B)
#ifdef __C51__	
	SWAP(CONFIG_DESCRIPTOR_LENGTH),			//返回整个数据的长度(配置,接口,端点和类型或供应商)(2B)
#endif
#ifdef __ZSP__
	LSB(CONFIG_DESCRIPTOR_LENGTH),MSB(CONFIG_DESCRIPTOR_LENGTH),
#endif
	1,											//配置所支持的接口数(1B)
	1,											//作为Set configuration的一个参数选择配置值(1B)
	iConfiguration,								//用于描述配置字符串的索引(1B)
	0x80,										//位图,总线供电&远程唤醒(1B)
	200											//最大消耗电流*2mA(1B)
};

//接口描述符
USB_INTERFACE_DESCRIPTOR InterfaceDescr DRAM_USB_TABLE=
{
	sizeof(USB_INTERFACE_DESCRIPTOR),			//描述符的大小9(1B)
	USB_INTERFACE_DESCRIPTOR_TYPE,				//描述符的类型04(1B)
	0,											//接口的编号(1B)
	0,											//用于为上一个字段可供替换的设置(1B)
	NUM_ENDPOINTS,								//使用的端点数(端点0除外)(1B)
	USB_DEVICE_CLASS_STORAGE,					//1类型代码(由USB分配)(1B),USB_DEVICE_CLASS_STORAGE=Mass Storage
	USB_SUBCLASS_CODE_SCSI,					//1子类型代码(由USB分配)(1B),"0x06=Reduced Block Commands(RBC)"
	USB_PROTOCOL_CODE_BULK,					//1协议代码(由USB分配)(1B),"0X50=Mass Storage Class Bulk-Only Transport"
	iInterface										//字符串描述的索引(1B)
};


USB_ENDPOINT_DESCRIPTOR MainEP_TXDescr DRAM_USB_TABLE=
{
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x81,
	USB_ENDPOINT_TYPE_BULK,
#ifdef __C51__	
	SWAP(FS_BULK_RX_SIZE),
#endif
#ifdef __ZSP__
	LSB(FS_BULK_RX_SIZE),MSB(FS_BULK_RX_SIZE),
#endif
	0		//bulk trans invailed
};

USB_ENDPOINT_DESCRIPTOR MainEP_RXDescr DRAM_USB_TABLE=
{
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x01,
	USB_ENDPOINT_TYPE_BULK,
#ifdef __C51__	
	SWAP(FS_BULK_RX_SIZE),
#endif
#ifdef __ZSP__
	LSB(FS_BULK_RX_SIZE),MSB(FS_BULK_RX_SIZE),
#endif
	0		//bulk trans invailed
};

#ifdef OT2
//配置描述符集合
uint8 FSConfigDescrs[] DRAM_USB_TABLE=
{
	sizeof(USB_CONFIGURATION_DESCRIPTOR),		//描述符的大小9(1B)
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		//描述符的类型02(1B)
	LSB(CONFIG_DESCRIPTOR_LENGTH),MSB(CONFIG_DESCRIPTOR_LENGTH),
	1,											//配置所支持的接口数(1B)
	1,											//作为Set configuration的一个参数选择配置值(1B)
	iConfiguration,								//用于描述配置字符串的索引(1B)
	0x80,										//位图,总线供电&远程唤醒(1B)
	200,											//最大消耗电流*2mA(1B)

//接口描述符
	sizeof(USB_INTERFACE_DESCRIPTOR),			//描述符的大小9(1B)
	USB_INTERFACE_DESCRIPTOR_TYPE,				//描述符的类型04(1B)
	0,											//接口的编号(1B)
	0,											//用于为上一个字段可供替换的设置(1B)
	NUM_ENDPOINTS,								//使用的端点数(端点0除外)(1B)
	USB_DEVICE_CLASS_STORAGE,					//1类型代码(由USB分配)(1B),USB_DEVICE_CLASS_STORAGE=Mass Storage
#if (CD_ROM==1)
	USB_SUBCLASS_CODE_SFF8070I,				//1子类型代码(由USB分配)(1B),"0x05=SFF8070i"
#else
	USB_SUBCLASS_CODE_SCSI,					//1子类型代码(由USB分配)(1B),"0x06=Reduced Block Commands(RBC)"
#endif	
	USB_PROTOCOL_CODE_BULK,					//1协议代码(由USB分配)(1B),"0X50=Mass Storage Class Bulk-Only Transport"
	iInterface,										//字符串描述的索引(1B)

//端点描述符
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x81,
	USB_ENDPOINT_TYPE_BULK,
	LSB(FS_BULK_RX_SIZE),MSB(FS_BULK_RX_SIZE),
	0,		//bulk trans invailed

//端点描述符
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x01,
	USB_ENDPOINT_TYPE_BULK,
	LSB(FS_BULK_RX_SIZE),MSB(FS_BULK_RX_SIZE),
	0		//bulk trans invailed
};
#endif


//高速设备描述符
USB_DEVICE_DESCRIPTOR HSDeviceDescr DRAM_USB_TABLE=
{
	sizeof(USB_DEVICE_DESCRIPTOR),				//描述符的大小18(1B)
	USB_DEVICE_DESCRIPTOR_TYPE,				//描述符的类型01(1B)
#ifdef __C51__	
	SWAP(0x0200),								//USB规划分布号(2B)
#endif
#ifdef __ZSP__
	LSB(0x0200),MSB(0x0200),						//USB规划分布号(2B)
#endif
	0,//	USB_CLASS_CODE_TEST_CLASS_DEVICE,			//1类型代码(由USB指定)(1B),0x00
	0, 0,											//子类型和协议(由USB分配)(2B)
	EP0_PACKET_SIZE,							//端点0最大包长(1B)
#ifdef __C51__
	SWAP(0x071b),
	SWAP(0x3203),
	SWAP(0x0100),								// 设备系列号

//	SWAP(0x0693),
//	SWAP(0x0003),
//	SWAP(0x0100),               						// 设备系列号

//	SWAP(0x0471),								//供应商ID(由USB分配)(2B)
//	SWAP(0x0666),								//产品ID(由厂商分配)(2B)
//	SWAP(0x0100),								//设备出厂编号(2B)
#endif
#ifdef __ZSP__
	LSB(0x071b),MSB(0x071b),
	LSB(0x3203),MSB(0x3203),
	LSB(0x0100),MSB(0x0100),
#endif
	iManufactuer, iProduct, iSerialNumber,				//厂商,产品,设备序列号字符串索引(3B)
	1											//可能的配置数(1B)
};

//配置描述符
USB_CONFIGURATION_DESCRIPTOR HSConfigDescr DRAM_USB_TABLE=
{
	sizeof(USB_CONFIGURATION_DESCRIPTOR),		//描述符的大小9(1B)
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		//描述符的类型02(1B)
#ifdef __C51__	
	SWAP(CONFIG_DESCRIPTOR_LENGTH),			//返回整个数据的长度(配置,接口,端点和类型或供应商)(2B)
#endif
#ifdef __ZSP__
	LSB(CONFIG_DESCRIPTOR_LENGTH),MSB(CONFIG_DESCRIPTOR_LENGTH),
#endif
	1,											//配置所支持的接口数(1B)
	1,											//作为Set configuration的一个参数选择配置值(1B)
	iConfiguration,								//用于描述配置字符串的索引(1B)
	0x80,										//位图,总线供电&远程唤醒(1B)
	200											//最大消耗电流*2mA(1B)
};

//接口描述符
USB_INTERFACE_DESCRIPTOR HSInterfaceDescr DRAM_USB_TABLE=
{
	sizeof(USB_INTERFACE_DESCRIPTOR),			//描述符的大小9(1B)
	USB_INTERFACE_DESCRIPTOR_TYPE,				//描述符的类型04(1B)
	0,											//接口的编号(1B)
	0,											//用于为上一个字段可供替换的设置(1B)
	NUM_ENDPOINTS,								//使用的端点数(端点0除外)(1B)
	USB_DEVICE_CLASS_STORAGE,					//1类型代码(由USB分配)(1B),USB_DEVICE_CLASS_STORAGE=Mass Storage
	USB_SUBCLASS_CODE_SCSI,					//1子类型代码(由USB分配)(1B),"0x06=Reduced Block Commands(RBC)"
	USB_PROTOCOL_CODE_BULK,					//1协议代码(由USB分配)(1B),"0X50=Mass Storage Class Bulk-Only Transport"
	iInterface										//字符串描述的索引(1B)
};


USB_ENDPOINT_DESCRIPTOR HSMainEP_TXDescr DRAM_USB_TABLE=
{
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x81,
	USB_ENDPOINT_TYPE_BULK,
#ifdef __C51__	
	SWAP(HS_BULK_RX_SIZE),
#endif
#ifdef __ZSP__
	LSB(HS_BULK_RX_SIZE),MSB(HS_BULK_RX_SIZE),
#endif
	0		//bulk trans invailed
};

USB_ENDPOINT_DESCRIPTOR HSMainEP_RXDescr DRAM_USB_TABLE=
{
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x01,
	USB_ENDPOINT_TYPE_BULK,
#ifdef __C51__	
	SWAP(HS_BULK_RX_SIZE),
#endif
#ifdef __ZSP__
	LSB(HS_BULK_RX_SIZE),MSB(HS_BULK_RX_SIZE),
#endif
	0		//bulk trans invailed
};

#ifdef OT2
//配置描述符集合
uint8 HSConfigDescrs[] DRAM_USB_TABLE=
{
	sizeof(USB_CONFIGURATION_DESCRIPTOR),		//描述符的大小9(1B)
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		//描述符的类型02(1B)
	LSB(CONFIG_DESCRIPTOR_LENGTH),MSB(CONFIG_DESCRIPTOR_LENGTH),
	1,											//配置所支持的接口数(1B)
	1,											//作为Set configuration的一个参数选择配置值(1B)
	iConfiguration,								//用于描述配置字符串的索引(1B)
	0x80,										//位图,总线供电&远程唤醒(1B)
	200,											//最大消耗电流*2mA(1B)

//接口描述符
	sizeof(USB_INTERFACE_DESCRIPTOR),			//描述符的大小9(1B)
	USB_INTERFACE_DESCRIPTOR_TYPE,				//描述符的类型04(1B)
	0,											//接口的编号(1B)
	0,											//用于为上一个字段可供替换的设置(1B)
	NUM_ENDPOINTS,								//使用的端点数(端点0除外)(1B)
	USB_DEVICE_CLASS_STORAGE,					//1类型代码(由USB分配)(1B),USB_DEVICE_CLASS_STORAGE=Mass Storage
	USB_SUBCLASS_CODE_SCSI,					//1子类型代码(由USB分配)(1B),"0x06=Reduced Block Commands(RBC)"
	USB_PROTOCOL_CODE_BULK,					//1协议代码(由USB分配)(1B),"0X50=Mass Storage Class Bulk-Only Transport"
	iInterface,										//字符串描述的索引(1B)

//端点描述符
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x81,
	USB_ENDPOINT_TYPE_BULK,
	LSB(HS_BULK_RX_SIZE),MSB(HS_BULK_RX_SIZE),
	0,		//bulk trans invailed

//端点描述符
	sizeof(USB_ENDPOINT_DESCRIPTOR),
	USB_ENDPOINT_DESCRIPTOR_TYPE,
	0x01,
	USB_ENDPOINT_TYPE_BULK,
	LSB(HS_BULK_RX_SIZE),MSB(HS_BULK_RX_SIZE),
	0		//bulk trans invailed
};
#endif


//其它速度配置描述符
OTHER_SPEED_CONFIG_DESCRIPTOR Other_Speed_Config_Descriptor DRAM_USB_TABLE=
{
	sizeof(OTHER_SPEED_CONFIG_DESCRIPTOR),	//length of other speed configuration descriptor
	0x07,									//Other speed configuration Type
#ifdef __C51__	
	SWAP(CONFIG_DESCRIPTOR_LENGTH),		//返回整个数据的长度(配置,接口,端点和类型或供应商)(2B)
#endif
#ifdef __ZSP__
	LSB(CONFIG_DESCRIPTOR_LENGTH),MSB(CONFIG_DESCRIPTOR_LENGTH),
#endif
	1,										//配置所支持的接口数(1B)
	1,										//作为Set configuration的一个参数选择配置值(1B)
	iConfiguration,							//用于描述配置字符串的索引(1B)
	0x80,									//位图,总线供电&远程唤醒(1B)
	200										//最大消耗电流*2mA(1B)
};

//高速设备限制描述符
HS_DEVICE_QUALIFIER HS_Device_Qualifier DRAM_USB_TABLE=
{
	sizeof(HS_DEVICE_QUALIFIER),		//length of HS Device Descriptor
	0x06,							//HS Device Qualifier Type
#ifdef __C51__	
	SWAP(0x0200),					// USB 2.0 version
#endif
#ifdef __ZSP__
	LSB(0x0200),MSB(0x0200),
#endif
	USB_DEVICE_CLASS_STORAGE,		//Device class
	USB_SUBCLASS_CODE_SCSI,		//Device SubClass
	USB_PROTOCOL_CODE_BULK, 		//Device Protocol Code
	FS_BULK_RX_SIZE,				//Maximum Packet SIze for other speed
	0x01,							//Number of Other speed configurations
	0x00							//Reserved
};

//字符串描述符,LangId
uint8 LangId[] DRAM_USB_TABLE=
{
	0x04,
	0x03,

	0x09, 0x04		//English--0x0409
//	0x04, 0x08		//Chinese--0x0804
};

//字符串描述符
uint8 StringManufacture[] DRAM_USB_TABLE=
{
	18,
	0x03,

	'R', 0x00,
	'o', 0x00,
	'c', 0x00,
	'k', 0x00,
	'C', 0x00,
	'h', 0x00,
	'i', 0x00,
	'p', 0x00
};

uint8 StringProduct[] DRAM_USB_TABLE=
{
	18,
	0x03,

	'R', 0x00,
	'O', 0x00,
	'C', 0x00,
	'K', 0x00,
	' ', 0x00,
	'M', 0x00,
	'P', 0x00,
	'3', 0x00
};

uint8 StringSerialNumbel[] DRAM_USB_TABLE=
{
	18,
	0x03,

	'U', 0x00,
	'S', 0x00,
	'B', 0x00,
	'V', 0x00,
	'1', 0x00,
	'.', 0x00,
	'0', 0x00,
	'0', 0x00
};

uint8 StringConfig[] DRAM_USB_TABLE=
{
	18,
	0x03,

	'C', 0x00,
	'O', 0x00,
	'N', 0x00,
	'F', 0x00,
	'I', 0x00,
	'G', 0x00,
	' ', 0x00,
	' ', 0x00
};

uint8 StringInterface[] DRAM_USB_TABLE=
{
	18,
	0x03,

	'I', 0x00,
	'N', 0x00,
	'T', 0x00,
	'E', 0x00,
	'R', 0x00,
	'F', 0x00,
	'A', 0x00,
	'C', 0x00
};


/**************************************************************************
 USB protocol function pointer arrays
**************************************************************************/
//USB标准请求
void (*StandardDeviceRequest[])(void) DRAM_USB_TABLE=
{
	get_status,
	clear_feature,
	reserved,
	set_feature,
	reserved,
	set_address,
	get_descriptor,
	reserved,
	get_configuration,
	set_configuration,
	get_interface,
	set_interface,
	reserved,
	reserved,
	reserved,
	reserved
};

//用户厂商请求
void (*VendorDeviceRequest[])(void) DRAM_USB_TABLE=
{
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	read_write_register,
	reserved,
	reserved,
	reserved
};

#if 0
char * _NAME_USB_REQUEST_DIRECTION[] DRAM_USB_TABLE=
{
	"Host_to_device",
	"Device_to_host"
};

char * _NAME_USB_REQUEST_RECIPIENT[] DRAM_USB_TABLE=
{
	"Device",
	"Interface",
	"Endpoint(0)",
	"Other"
};

char * _NAME_USB_REQUEST_TYPE[] DRAM_USB_TABLE=
{
	"Standard",
	"Class",
	"Vendor",
	"Reserved"
};

char * _NAME_USB_STANDARD_REQUEST[] DRAM_USB_TABLE=
{
	"GET_STATUS",
	"CLEAR_FEATURE",
	"RESERVED",
	"SET_FEATURE",
	"RESERVED",
	"SET_ADDRESS",
	"GET_DESCRIPTOR",
	"SET_DESCRIPTOR",
	"GET_CONFIGURATION",
	"SET_CONFIGURATION",
	"GET_INTERFACE",
	"SET_INTERFACE",
	"SYNC_FRAME"
};
#endif

#else
extern	USB_DEVICE_DESCRIPTOR 			DeviceDescr;
extern	USB_CONFIGURATION_DESCRIPTOR 	ConfigDescr;
extern	USB_INTERFACE_DESCRIPTOR 		InterfaceDescr;
extern	USB_ENDPOINT_DESCRIPTOR 		MainEP_TXDescr;
extern	USB_ENDPOINT_DESCRIPTOR 		MainEP_RXDescr;

extern	USB_DEVICE_DESCRIPTOR 			HSDeviceDescr;
extern	USB_CONFIGURATION_DESCRIPTOR 	HSConfigDescr;
extern	USB_INTERFACE_DESCRIPTOR 		HSInterfaceDescr;
extern	USB_ENDPOINT_DESCRIPTOR 		HSMainEP_TXDescr;
extern	USB_ENDPOINT_DESCRIPTOR 		HSMainEP_RXDescr;

extern	OTHER_SPEED_CONFIG_DESCRIPTOR Other_Speed_Config_Descriptor;
extern	HS_DEVICE_QUALIFIER 			HS_Device_Qualifier;
extern	void (*StandardDeviceRequest[])(void);
extern	void (*VendorDeviceRequest[])(void);
extern	uint8 LangId[4];
extern	uint8 StringManufacture[18];
extern	uint8 StringProduct[18];
extern	uint8 StringSerialNumbel[18];
extern	uint8 StringConfig[18];
extern	uint8 StringInterface[18];
#ifdef OT2
extern	uint8 HSConfigDescrs[];
extern	uint8 FSConfigDescrs[];
#endif
#endif
#endif
#endif

