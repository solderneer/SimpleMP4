/******************************************************************************
	File Name:
		decoder.c
	File Description:
		[1]	JPEG decoder main function 
		[2]	data input routine
		[3]	data output routine
		[4]	interrupt service routine 
		[5]	key scaning routine
	Author:
		Zheng yongzhi	<zyz@rock-chips.com>
		Jian huan		<jh@rock-chips.com>
	Date:
		2005-11-04
	History:
            Modified by HJL@20051026
*******************************************************************************/
#define _IN_JPEGDECODE

#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "JpegDecodeMacro.h"
#include "Resource.h"
#include "Global.h"
#include "Lcd.h"
#include "JpegDecodeGlobal.h"
#include "JpegDecodeSchedule.h"

#include "jpgdec.h"
#include "Dma.h"
/********************************************************************
	Func:
		DecProc
	Description:
		JPEG main decoding routine, which including
		[1]	initialization
		[2]	decoder routine
		[3]	decoder status detection
		[4]	post processing routine
		[5]	key scaning
	Author:
		Jian huan		<jh@rock-chips.com>
	Date:
		2005-07-21
********************************************************************/
__attribute__((section(".jpeg_text, \"ax\"")))

void JpegDecodeHead(void)
{
    int  wmaResult=0;
    unsigned int words_read;
    unsigned int i,j;
    unsigned int temp1,temp2;
    
    if ((hSlaveFile = FileOpen(SysFile2Info.Path, SysFile2Info.Fdt.Name, "R")) == NOT_OPEN_FILE) {
        //PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_DECODE_END,0,0,0,0,0,0);
        JpegFileFormatError = 1;
        PreDisplayFlag = DISP_JPEG;
        DisplayRecoveryTime = 0;
        LCD_Init(0);//(LCD_INIT_NORMAL);
        LCD_SetColor(0);
        LCD_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_NONE,0,0,0/*,0,0,0*/);
        return;
    }
    GetLongFileName(SysFile2Info.LongFileName);
    
    PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_DECODE_PLAY,0,0,0/*,0,0,0*/);
}
/********************************************************************
	Func:
		DecProc
	Description:
		JPEG main decoding routine, which including
		[1]	initialization
		[2]	decoder routine
		[3]	decoder status detection
		[4]	post processing routine
		[5]	key scaning
	Author:
		Jian huan		<jh@rock-chips.com>
	Date:
		2005-07-21
********************************************************************/
__attribute__((section(".jpeg_text, \"ax\"")))

void JpegDecodePlay(void)
{   
//    int JpegDecStatus;
    
    JpegFlag.bc.bJpgBufferIndex = 1;
	JpegFlag.bc.bJpgBufferFull = 1;
	
	bitclear_creg(%imask,5);
	MusicSetFreq51();
    SystemInfo.bc.bSysIdleEn = 0;
    SystemInfo.bc.bBackLightOffEn = 1;
    bitset_creg(%imask,5);
    if (JpegDecode()){
        JpegFileFormatError = 1;
        PreDisplayFlag = DISP_JPEG;
        DisplayRecoveryTime = 0;
        LCD_Init(0/*LCD_INIT_NORMAL*/);
        LCD_SetColor(0);
        LCD_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_NONE,0,0,0/*,0,0,0*/);
    } else {
        PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_DECODE_END,0,0,0/*,0,0,0*/);
    }
//    JpegDebug(321);
//	Delay1Ms(1000);
    FileClose(hSlaveFile);
    bitclear_creg(%imask,5);
    SystemInfo.bc.bSysIdleEn = 1;
    SystemInfo.bc.bBackLightOffEn = 0;
    SysIdleCounter = 0;
    LightOffCounter = 0;
    JpegTimeReset();
    MusicSetFreq51Return();
    bitset_creg(%imask,5);
}    

/********************************************************************
  Func:
  	JpgReadDataFromMedium
  Description:
  	Load Data From Medium To JPEG Decoder Input BufferA or BufferB
  	Input:    
  		JpgFlag.bc.bJpgBufferIndex
  	Output:
  		 (JpegFlag.bc.bJpgBufferFull),JpgInputBuffer[A/B]
  Author:
 	jian huan
  Date:
  	2005-06
 ********************************************************************/
 __attribute__((section(".jpeg_text, \"ax\"")))

void JpegDecodeEnd(void)
{
    FileClose(hSlaveFile);
//    SysCpuSetFreq(24);
//    SysTimer0Set(24,10000);
    PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_NONE,0,0,0/*,0,0,0*/);
}
/********************************************************************
  Func:
  	JpgReadDataFromMedium
  Description:
  	Load Data From Medium To JPEG Decoder Input BufferA or BufferB
  	Input:    
  		JpgFlag.bc.bJpgBufferIndex
  	Output:
  		 (JpegFlag.bc.bJpgBufferFull),JpgInputBuffer[A/B]
  Author:
 	jian huan
  Date:
  	2005-06
 ********************************************************************/
 __attribute__((section(".jpeg_text, \"ax\"")))

void JpgReadDataFromMedium(void)
{
	unsigned int i;
	unsigned int *pBuffer;
	unsigned int InputBufferLen ;
	unsigned long TimeOut;
	
	TimeOut = 0;
    do {
        if (++TimeOut == 32768)
            break;
    }while (JpegFlag.bc.bJpgBufferFull != TRUE);
	JpegFlag.bc.bJpgBufferFull = 0;
    
    JpegFlag.bc.bJpgBufferIndex = ~JpegFlag.bc.bJpgBufferIndex;
	pBuffer = JpegFlag.bc.bJpgBufferIndex * JPG_PIPO_BUF_WORDSIZE + JPGInputBuffer;
	
	InputBufferLen = FileRead((unsigned char*)JPGFileReadBuffer, JPG_PIPO_BUF_BYTESIZE, hSlaveFile);   
//    if(InputBufferLen < JPG_PIPO_BUF_BYTESIZE) {
//        return;
//    }
    for(i = 0; i < InputBufferLen ; i+=2)
    	*pBuffer ++ = (JPGFileReadBuffer[i] ) |( JPGFileReadBuffer[i+1]<< 8);
    
    JpegFlag.bc.bJpgBufferFull = TRUE;
}

/********************************************************************
  Func:
  	SetupDMA
  	Post_Processing
  Description:
  	The Following 2 functions are uesd in post processing routine which including 
  	Image-Scale, YUV2RGB565
  Author:
  	Chen lijun
  	Jian 	Huan	
  Date:
  	2005-10-07
 ********************************************************************/
 __attribute__((section(".jpeg_text, \"ax\"")))
void JpgSendRGBToLCD(void)
{
    DMA_CHANNEL_REG *DmaChl;
	unsigned int temp;
 	unsigned int length ;
	unsigned int *pBuffer;
	unsigned long TimeOut;
    unsigned int imask_tmp;

	    
	pBuffer = pJpegRGBOutBuffer;
	length = jpeg_process_marker->hor_ver_length ;
    while(JpeaDmaTanFlag);
    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(0)));
    
//-----------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;            //(220/22)-1;
//-----------------------------------------------------------------     
    DmaChl->DmaSar = ((unsigned long)((unsigned int)(pBuffer)) << 1) + (unsigned long)0x40000; 
    DmaChl->DmaDar = (((unsigned long)(LCD_DATA))<<1);
    
    DmaChl->DmaCtl.Lsb = 0x00000113;             //SRC_GATHER_EN:0 DINC:No change, SRC_TR_WIDTH:16-bit, DST_TR_WIDTH:16-bit, INT_EN:1
    DmaChl->DmaCtl.Msb = length;             //(unsigned long)176*22;            
    
    DmaChl->DmaCfg.Lsb = 0x00000000;
    DmaChl->DmaCfg.Msb = 0x00001004;

    JpeaDmaTanFlag     = 1;
    DmaChl->pResFlag   = &JpeaDmaTanFlag;
    
    DMA_DISABLE_INTERRUPT;
    	DmaChannelSetup(0);
    DMA_ENABLE_INTERRUPT;    
    JpegFlag.bc.bRGBBufferIndex = ~JpegFlag.bc.bRGBBufferIndex;
	pJpegRGBOutBuffer = JpegFlag.bc.bRGBBufferIndex * SCR_LENGTH + JpegRGBOutputBuffer;
}

/********************************************************************
  Func:
  	JpgSendRGBBufferEmpty
  Description:
  	Send RGB Data To LCD BufferEmpty Flag Set(DMAIsr call this Function)
  	Input:    
  		NULL
  	Output:
		NULL
  Author:
 	Zheng yongzhi
  Date:
  	2005-06
 ********************************************************************/
 __attribute__((section(".jpeg_text, \"ax\"")))

void JpgSendRGBBufferEmpty(void)
{
	JpegFlag.bc.bRGBBufferEmpty = 1;
}

/********************************************************************/
