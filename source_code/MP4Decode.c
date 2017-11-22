/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : AviMp3Decode.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#define _IN_MP4DECODE

//#include "Lsi402.h"
#include "Global.h"
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "AviMacro.h"
#include "AviGlobal.h"
#include "AviFile.h"
#include "xvid_decoder.h" //xvid/


short           MP4DecFirstLoadBuffer(void);
void            PP_Init(void);
unsigned int    Read_VideoData_ToBuf(unsigned int *bufptr, unsigned int len);

//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
short MP4DecodeProc(unsigned int InitStatus)
{
	short	     MP4DecStatus;
	unsigned int *pTemp;
	
	if(InitStatus){
		//(1)	1st receive data from NandFlash
		MP4DecStatus = MP4DecFirstLoadBuffer();
		if(MP4DecStatus == -1)
		    return MP4_DECODE_END;
		//(2)	MPEG-4 decoder main structer initialization
        Mp4_Memory_Init();
		Mp4_Dec_Init((unsigned short *)MP4InputBuffer);
		//(3)	post processing initialization
		PP_Init();
		while(DmaFillImage2LcdFlag);
		DmaInit();
	}
       
    //while(Dma_RGB_E2LCD != 0);
    
	//LCD_SetDispAddr(0,0);
	MP4DecStatus = MP4_DECODE_ROUTINE();
	
	return 	MP4DecStatus;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
short MP4DecFirstLoadBuffer(void)
{
	unsigned short Len;
	
	AviPlayFlag.bc.bMP4DecodeBufferFull = FALSE;
#ifdef  _FLASH_DMA

#else
    Len = Read_VideoData_ToBuf(MP4InputBuffer, MP4_PIPO_BUF_WORDSIZE);
    AviPlayFlag.bc.bMP4DecodeBufferFull = TRUE;
#endif

    if(Len < MP4_PIPO_BUF_WORDSIZE)
        return -1;
    else
        return 0;    
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/

__attribute__((section(".avi_v_text, \"ax\"")))
void FillBuffer(BITSTREAM *bs)
{
	unsigned int	*bufferptr;
	unsigned short		len;

//	unsigned long   TimeOut;
//	TimeOut = 0;
//    do {
//        if (++TimeOut == 32768)
//            break;
//    } while (AviPlayFlag.bc.bMP4DecodeBufferFull != TRUE);
	
    while (AviPlayFlag.bc.bMP4DecodeBufferFull != TRUE);	
							                 
	AviPlayFlag.bc.bMP4DecodeBufferFull = FALSE;      
	
	bs->exchange = (bs->exchange + 1 ) & 0x1;

	bufferptr = MP4InputBuffer + bs->exchange * MP4_PIPO_BUF_WORDSIZE;
	
	len = Read_VideoData_ToBuf(bufferptr, MP4_PIPO_BUF_WORDSIZE);
    AviPlayFlag.bc.bMP4DecodeBufferFull = TRUE;
    
	if(len < MP4_PIPO_BUF_WORDSIZE) {			//512	
		bs->file_end = MP4_DECODE_END;
	}
	
	bs->start = (unsigned long *)bufferptr;
	bs->tail =  bs->start;
}


/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
unsigned int Read_VideoData_ToBuf(unsigned int *bufptr, unsigned int length)
{
    unsigned int i,j;
    unsigned int len,ReadOutDataLen,FileReadDataLen;
    int          ReadResult = I_FRAME;
    
    AVI_MP3_DECODE_DISABLE;
    
    len = (length & 0xfffe);
    
    for (j = 0; j < 2; j ++) {
        if (ERROR_FRAME == ReadResult)
            break;
            
        ReadOutDataLen = 0;
        if(NeedFillDataNum){
            ReadOutDataLen = NeedFillDataNum;
            switch(NeedFillDataNum){
                case 1:
                    *(pAviMp4FileReadBuf    ) = 0xb7;
                    break;
                    
                case 2:
                    *(pAviMp4FileReadBuf    ) = 0x01;
                    *(pAviMp4FileReadBuf + 1) = 0xb7;
                    break;
                                        
                case 3:
                    *(pAviMp4FileReadBuf    ) = 0x00;
                    *(pAviMp4FileReadBuf + 1) = 0x01;
                    *(pAviMp4FileReadBuf + 2) = 0xb7;
                    break;
                
                case 4:
                    *(pAviMp4FileReadBuf    ) = 0x00;
                    *(pAviMp4FileReadBuf + 1) = 0x00;
                    *(pAviMp4FileReadBuf + 2) = 0x01;
                    *(pAviMp4FileReadBuf + 3) = 0xb7;
                    break;
                                          
                default:
                    break;  
            }
            NeedFillDataNum = 0;
        }
        
        do {
            if(AviVideoChunkLength == 0) {
                ReadResult = GetNextVideoChunk(&AviVideoChunkOffset,&AviVideoChunkLength);
                FileSeek(AviVideoChunkOffset,SEEK_CUR,hMainFile);
            }
            
            if(ERROR_FRAME == ReadResult) {
                AviPlayFlag.bc.bMp4DecodeEnable = 0;
                PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_END,0,0,0/*,0,0,0*/);
            }
            if(!AviVideoChunkLength){
                if((ReadOutDataLen) > (MP4_PIPO_BUF_WORDSIZE - 5)){
                    switch(ReadOutDataLen){
                        case (MP4_PIPO_BUF_BYTESIZE - 4):
                            *(pAviMp4FileReadBuf + ReadOutDataLen    ) = 0x00;
                            *(pAviMp4FileReadBuf + ReadOutDataLen + 1) = 0x00;
                            *(pAviMp4FileReadBuf + ReadOutDataLen + 2) = 0x01;
                            *(pAviMp4FileReadBuf + ReadOutDataLen + 3) = 0xb7; 
                            NeedFillDataNum = 0;
                            ReadOutDataLen += 4;
                            break;
                            
                        case (MP4_PIPO_BUF_BYTESIZE - 3):   
                            *(pAviMp4FileReadBuf + ReadOutDataLen    ) = 0x00;
                            *(pAviMp4FileReadBuf + ReadOutDataLen + 1) = 0x00;
                            *(pAviMp4FileReadBuf + ReadOutDataLen + 2) = 0x01;
                            NeedFillDataNum = 1;
                            ReadOutDataLen += 3;
                            break;
                            
                        case (MP4_PIPO_BUF_BYTESIZE - 2):
                            *(pAviMp4FileReadBuf + ReadOutDataLen    ) = 0x00;
                            *(pAviMp4FileReadBuf + ReadOutDataLen + 1) = 0x00;
                            NeedFillDataNum = 2;
                            ReadOutDataLen += 2;
                            break;
                            
                        case (MP4_PIPO_BUF_BYTESIZE - 1):
                            *(pAviMp4FileReadBuf + ReadOutDataLen    ) = 0x00;
                            NeedFillDataNum = 3;
                            ReadOutDataLen += 1;
                            break;

                    }   
                }   
                else{
                    *(pAviMp4FileReadBuf + ReadOutDataLen    ) = 0x00;
                    *(pAviMp4FileReadBuf + ReadOutDataLen + 1) = 0x00;
                    *(pAviMp4FileReadBuf + ReadOutDataLen + 2) = 0x01;
                    *(pAviMp4FileReadBuf + ReadOutDataLen + 3) = 0xb7;  
                    ReadOutDataLen += 4; 
                }
            }            
            /////////////////////// modify end   //////////////////////////
            if (ERROR_FRAME == ReadResult)
                break;
                
            if (AviVideoChunkLength > (len - ReadOutDataLen)) {
                FileReadDataLen = FileRead((unsigned char*)pAviMp4FileReadBuf + ReadOutDataLen, len - ReadOutDataLen, hMainFile);
                if (FileReadDataLen < (len - ReadOutDataLen)) {
                    break;
                }
            } else {
                FileReadDataLen = FileRead((unsigned char*)pAviMp4FileReadBuf + ReadOutDataLen, AviVideoChunkLength, hMainFile);
                if (FileReadDataLen < AviVideoChunkLength) {
                    break;
                }
            }
            AviVideoChunkLength -= FileReadDataLen;
            ReadOutDataLen += FileReadDataLen;
                
        } while (len != ReadOutDataLen);                
            
        // 20051116:Huangjiliu modified    
        //Byte2Word((bufptr + (((length & 0xfffe)*j) >> 1)), pAviMp4FileReadBuf, (j * ((length & 0xfffe)) + ReadOutDataLen) >> 1);
        Byte2Word((bufptr + (((length & 0xfffe)*j) >> 1)), pAviMp4FileReadBuf, ReadOutDataLen >> 1);
        
        len = 2*length -len;
    }
    AVI_MP3_DECODE_ENABLE;
    AviPlayFlag.bc.bMP4DecodeBufferFull = 1;
    
    return(((j - 1) * ((length & 0xfffe)) + ReadOutDataLen) >> 1);
}

//**********************YUV--RGB Convert******************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void PP_Init(void)
{
		 													
}
/*------------------------------------------------------------------------------
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
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void Send_RGBData_ToLcd(void)
{

}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".avi_v_text, \"ax\"")))
void AviMp4SendLcdDmaEnd(void)
{
    AviPlayFlag.bc.bRGBBufferEmpty = 1;
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
//INT16 Post_Processing(IMAGE *pimg, INT16 BusWidth)
__attribute__((section(".avi_v_text, \"ax\"")))
void Post_Processing(void)
{
    /*
	unsigned short *pStreamY, *pStreamUV;
	IN_MEM  *pin_mem = 0;
    
    pin_mem = &IntMem;
    
	pStreamY    = pin_mem->mb_y_row;
	pStreamUV   = pin_mem->mb_uv_row;
	
	LCD_SetDispAddr(0, 0);
	yuv2rgb565(pStreamY, pStreamUV); 
	*/
}
//******************************************************************************
