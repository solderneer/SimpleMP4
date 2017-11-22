/*******************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  File name :    FunEncoderInitial.c
*  Author:        YiFengZhao
*  Description:   Initial the encoder,for Line and MIC 
*  Others:         
*  Function list:  
*  History:        
*           <author>          <time>       <version>         <desc>
*       YiFeng Zhao         2005-11-12         1.0           
*
******************************************************************************/
#define _IN_FUNDECODER
#define _MPGE_initializeWork

#include <creg.h>
#include "Memap.h"
#include "Macro.h"
#include "MsgDef.h"
#include "Resource.h"
#include "Global.h"
#include "Config.h"

#include "EncMacro.h"
#include "EncGlobal.h"

#include "MSadpcm.h"
#include "msadpcminit.h"

#include "Lcd.h"
#include "LcdChar.h"

/*------------------------------------------------------------------------------
Function:PCM Buffer Change
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncodePcmBufferChange (void) 
{
    if (EncodeFlag.bc.bPcmBufEmpty) {
        
        EncodeFlag.bc.bPcmBufEmpty = 0;
        EncodeFlag.bc.bPcmBufFull = 1;
        pEncodeInputBuffer = pPCMInputBuffer;
        EncodeFlag.bc.bBufferIndex = ~EncodeFlag.bc.bBufferIndex;
        pPCMInputBuffer = EncodeFlag.bc.bBufferIndex * AD_PIPO_BUFFER_SIZE + EncodeInputBuffer;    
    }
    
    write_mem32(DMAR_SAR2,(unsigned long)(I2DSP_RXDB + 1) << 1);
    write_mem32(DMAR_DAR2,(((unsigned long)(pPCMInputBuffer) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    if(EncPlayInfo.Mode == ENCODE_MIC_REC) {
        write_mem32(DMAR_CTL2+2,MSADPCM_CH_NUMPCM << 1);
    } else {
#if(FLASHROM_TYPE == _MLC_)
        write_mem32(DMAR_CTL2+2,LINE_IN_MSADPCM_CH_NUMPCM << 1);
#else
        write_mem32(DMAR_CTL2+2,MP3ENC_CH_NUMPCM << 1);
#endif
    }
    write_mem32(DMAR_CTL2,0x00200413);
    write_mem32(DMAR_CFG2,0x00000040);
    write_mem32(DMAR_CFG2+2,0x00000184);
    write_mem32(DMAR_DmaCfgReg,0x00000001);
    write_mem32(DMAR_MaskBlock,0x00000404);
    write_mem32(DMAR_ChEnReg,0x00000404); 

#if(FLASHROM_TYPE == _MLC_)
    AVI_MP3_DECODE_REQ;//启动用户中断
#endif

}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
int put_mp3( BIT_STREAM_STRUC *bs )
{
	int minimum;
	
	minimum = bs->buf_byte_idx + 1;
	
	if( minimum <=0 )
	    return 0;

	bs->buf_byte_idx = -1;
	bs->buf_bit_idx = 0;
	
	return minimum;
}

/*------------------------------------------------------------------------------
Function:PCM Buffer Change
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
short EncodeHead(void)
{
    unsigned int i;
    unsigned long TempL;
    
    SysCpuSetFreq(72);
    SysTimer0Set(72,25000);
        
    MediaSetup(MemorySelect);
    FileInit();
    
    TempL = GetFreeMem(MemorySelect);

    FileFragClean();                // hjl@2006-9-13 10:35
    g_dwEncoderRemainTimeSecond = (TempL << 3) / ((unsigned long)(EncPlayInfo.Bitrate));
	
    g_dwEncoderCurrTimeSecond = 0;
    g_dwEncoderTotleTimeSecond = g_dwEncoderRemainTimeSecond;
    EncPlayInfo.TimeGuageVal = 0;

    //EncodeFlag.bc.bDiskFull = 0;
    if(g_dwEncoderRemainTimeSecond < 5) {	
    	DisplayRecoveryTime = 0;
        
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, RECORD_MSG_DISK_NOT_ENOUGH_TXT);
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        Fun1ModuleChange();
        return 0;
	}
	
#if(FLASHROM_TYPE == _MLC_)
    MLCEncoderOutputBufferIndex = 0;
    pMLCEncoderOutputBufferOffset = 0;
    MLCEncoderOutputBufferLeftData = 0;
#endif
	
	if (EncPlayInfo.Mode == ENCODE_MIC_REC) {
	    SysFile1Info.Path[0] = 'U';
        SysFile1Info.Path[1] = ':';
        SysFile1Info.Path[2] = '\\';
        
        SysFile1Info.Path[3] = 'V';
        SysFile1Info.Path[4] = 'O';
        SysFile1Info.Path[5] = 'I';
        SysFile1Info.Path[6] = 'C'; 
        SysFile1Info.Path[7] = 'E';
        SysFile1Info.Path[8] = '\0';
        
	    SysFile1Info.Fdt.Name[0] = 'R';
        SysFile1Info.Fdt.Name[1] = 'E';
        SysFile1Info.Fdt.Name[2] = 'C';
        SysFile1Info.Fdt.Name[7] = ' ';
        
	    SysFile1Info.Fdt.Name[8] = 'W';
        SysFile1Info.Fdt.Name[9] = 'A';
        SysFile1Info.Fdt.Name[10]= 'V';
    } else if (EncPlayInfo.Mode == ENCODE_FM_REC) {
        SysFile1Info.Path[0] = 'U';
        SysFile1Info.Path[1] = ':';
        SysFile1Info.Path[2] = '\\';
        
        SysFile1Info.Path[3] = 'F';
        SysFile1Info.Path[4] = 'M';
        SysFile1Info.Path[5] = '\0';
        
        SysFile1Info.Fdt.Name[0] = 'F';
        SysFile1Info.Fdt.Name[1] = 'M';
        SysFile1Info.Fdt.Name[2] = '0';
        SysFile1Info.Fdt.Name[7] = ' ';
        
#if(FLASHROM_TYPE == _MLC_)
	    SysFile1Info.Fdt.Name[8] = 'W';
        SysFile1Info.Fdt.Name[9] = 'A';
        SysFile1Info.Fdt.Name[10]= 'V';  
#else
	    SysFile1Info.Fdt.Name[8] = 'M';
        SysFile1Info.Fdt.Name[9] = 'P';
        SysFile1Info.Fdt.Name[10]= '3';  
#endif
    }
    else /*if(mode == ENCODE_LINE_IN)*/ {
        SysFile1Info.Path[0] = 'U';
        SysFile1Info.Path[1] = ':';
        SysFile1Info.Path[2] = '\\';
        
        SysFile1Info.Path[3] = 'L';
        SysFile1Info.Path[4] = 'I';
        SysFile1Info.Path[5] = 'N';
        SysFile1Info.Path[6] = 'E'; 
        SysFile1Info.Path[7] = 'I';
        SysFile1Info.Path[8] = 'N';
        SysFile1Info.Path[9] = '\0';
        
	    SysFile1Info.Fdt.Name[0] = 'L';
        SysFile1Info.Fdt.Name[1] = 'I';
        SysFile1Info.Fdt.Name[2] = 'N';
        SysFile1Info.Fdt.Name[7] = ' ';
        
#if(FLASHROM_TYPE == _MLC_)
	    SysFile1Info.Fdt.Name[8] = 'W';
        SysFile1Info.Fdt.Name[9] = 'A';
        SysFile1Info.Fdt.Name[10]= 'V';  
#else
	    SysFile1Info.Fdt.Name[8] = 'M';
        SysFile1Info.Fdt.Name[9] = 'P';
        SysFile1Info.Fdt.Name[10]= '3';  
#endif

#if(CPU_TYPE == _RK2608_) 
//        write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) | 0x0088)&0xff7F);
#endif
        PeriphFlag.bc.EarPlugIn = 1;
        PeriphFlag.bc.EarEjectout = 1;
    }
    
    for (TempL = 1; TempL <= 9999; TempL++) {
        SysFile1Info.Fdt.Name[6] = '0' + (TempL % 10);
        SysFile1Info.Fdt.Name[5] = '0' + ((TempL / 10) % 10);
        SysFile1Info.Fdt.Name[4] = '0' + ((TempL / 100) % 10);
        SysFile1Info.Fdt.Name[3] = '0' + TempL / 1000;
        
        if (hMainFile = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R") == NOT_OPEN_FILE) {
            break;
        }
        FileClose(hMainFile);
    }
    
   	for (i = 0; i < 11; i ++) {
  	    SysFile1Info.LongFileName[i] = SysFile1Info.Fdt.Name[i];
   	}
   	SysFile1Info.LongFileName[7] = '.';
   	SysFile1Info.LongFileName[11] = '\0';

    if (TempL > 9999) {
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, RECORD_MSG_CREAT_ERR_TXT);
        DisplayRecoveryTime = 0;
        PreDisplayFlag = DisplayFlag;
        DisplayFlag = DISP_DIALOG;
        
        EncPlayInfo.PlayState = MUSIC_ENCODE_READY;
	    EncPlayStatus = MUSIC_ENCODE_STOP;
	    
        PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_FRAG,0,0,0/*,0,0,0*/);
	} else {
	    EncodeFlag.bc.bEncodeEnable = 0;
	    
	    bitclear_creg(%imask,5);
	    SystemInfo.bc.bSysIdleEn = 1;
	    SysIdleCounter = 0;
	    bitset_creg(%imask,5);
	    
	    EncPlayInfo.PlayState = MUSIC_ENCODE_READY;
	    EncPlayStatus = MUSIC_ENCODE_STOP;
	    
	    PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_FRAG,0,0,0/*,0,0,0*/);
	}
	
//    if (MemorySelect == 1) {
//        if (EncPlayInfo.Mode == ENCODE_MIC_REC) {
//            SysCpuSetFreq(36);
//            SysTimer0Set(36,25000);
//        } else {
//            SysCpuSetFreq(60);
//            SysTimer0Set(60,25000);
//        }
//    } else {
//        if (EncPlayInfo.Mode == ENCODE_MIC_REC) {
//            SysCpuSetFreq(24);
//            SysTimer0Set(24,25000);
//        } else {
//            SysCpuSetFreq(48);
//            SysTimer0Set(48,25000);
//        }
//    }
    if ((MemorySelect == 1) && (EncPlayInfo.Mode != ENCODE_MIC_REC)) {
        SysCpuSetFreq(60);
        SysTimer0Set(60,25000);
    } else {
        SysCpuSetFreq(51);
        SysTimer0Set(51,25000);
    }
#if (FLASHROM_TYPE == _MLC_)
    write_mem(FMWAIT,  0x2083);
#endif

    EncDispFlag = ENC_DISPFLAG_ALL;
    LCD_SetDispRect(0, 0, MAX_PIC_XSIZE - 1, MAX_PIC_YSIZE - 1);
}

/*------------------------------------------------------------------------------
Function:PCM Buffer Change
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncodeStart()
{
    g_dwSampleCount=0;

    PCMSoundDataCount = 0;
    PCMHaveSoundTimes = 0;
    PCMNoSoundTimes = 0;

    MicRecTotalData = 0;
    
    EncodeFlag.bc.bNewFileBegin = 0;
    EncodeFlag.bc.bPcmBufEmpty = 1;
	EncodeFlag.bc.bPcmBufFull = 0;
    EncodeFlag.bc.bBufferIndex = 0;
    
    pPCMInputBuffer = EncodeInputBuffer;
    pEncodeInputBuffer = EncodeInputBuffer + AD_PIPO_BUFFER_SIZE; 
	
    if ((hMainFile = FileCreate(SysFile1Info.Path, SysFile1Info.Fdt.Name)) == NOT_OPEN_FILE) {
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, RECORD_MSG_CREAT_ERR_TXT);
        DisplayRecoveryTime = 0;
        PreDisplayFlag = DisplayFlag;
        DisplayFlag = DISP_DIALOG;
        PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_NONE,0,0,0/*,0,0,0*/);
        return;
    }

    if (EncPlayInfo.Mode == ENCODE_MIC_REC) {
        MSAdpamIntial(1, (short *)MicRecHeadBuf, &EncPlayInfo.Freq);
        FileWrite((unsigned char*)MicRecHeadBuf,512,hMainFile);
        write_mem32(DMAR_CTL2 + 2,MSADPCM_CH_NUMPCM << 1);
    } else {
#if(FLASHROM_TYPE == _MLC_)
        MSAdpamIntial(2, (short *)MicRecHeadBuf, &EncPlayInfo.Freq);
        FileWrite((unsigned char*)MicRecHeadBuf,512,hMainFile);
        write_mem32(DMAR_CTL2+2,LINE_IN_MSADPCM_CH_NUMPCM << 1);
#else
        MPGE_initializeWork(EncPlayInfo.Freq,EncPlayInfo.Bitrate,&Rock_obj);
#endif
        write_mem32(DMAR_CTL2 + 2,MP3ENC_CH_NUMPCM << 1);
    }
    write_mem32(DMAR_SAR2,(unsigned long)(I2DSP_RXDB + 1) << 1);
    write_mem32(DMAR_DAR2,(((unsigned long)(pPCMInputBuffer) << 1) & 0x0001ffff) + (unsigned long)0x40000);
    write_mem32(DMAR_CTL2,0x00200413);
    write_mem32(DMAR_CFG2,0x00000040);
    write_mem32(DMAR_CFG2+2,0x00000184);
    write_mem32(DMAR_DmaCfgReg,0x00000001);
    write_mem32(DMAR_MaskBlock,0x00000404);
    write_mem32(DMAR_ChEnReg,0x00000404); 
    
    unsigned int temp = 0;
    do {
        if (++temp == 32768)
            break;
    } while(EncodeFlag.bc.bPcmBufFull == 0);
    
    EncodeFlag.bc.bPcmBufFull = 0;
    EncodeFlag.bc.bPcmBufEmpty = 1;
    EncodeFlag.bc.bTheFirstPcmData = 1;
    
    PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_PLAY,0,0,0/*,0,0,0*/);
    EncodeFlag.bc.bEncodeEnable = 1;
}
/*------------------------------------------------------------------------------
Function:PCM Buffer Change
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncodeEnd(void)
{
    unsigned long SecIndex;;
    
    DisplayPicture(RECORD_STATUSBK_PIC_X, RECORD_STATUSBK_PIC_Y, RECORD_STATUSBK_PIC_XSIZE, RECORD_STATUSBK_PIC_YSIZE, RECORD_STATUSBK_PIC);
    DisplayMenuStr(RECORD_STATUS_TXT_X, RECORD_STATUS_TXT_Y, RECORD_STATUS_TXT_XSIZE, RECORD_STATUS_TXT_YSIZE, LCD_TEXTALIGN_CENTER, RECORD_STATUS_SAVE_TXT);
#ifdef LCD_MEMORY_DEV
    DmaFillImage2Lcd();
#endif

#if(FLASHROM_TYPE == _MLC_)
    if(hMainFile != NOT_OPEN_FILE)          // 
#else
    if(EncPlayInfo.Mode == ENCODE_MIC_REC)
#endif    
	{
        MicRecHeadBuf[508]=MicRecTotalData&0xff;
        MicRecHeadBuf[509]=(MicRecTotalData>>8)&0xff;
        MicRecHeadBuf[510]=(MicRecTotalData>>16)&0xff;
        MicRecHeadBuf[511]=(MicRecTotalData>>24)&0xff;
        MicRecTotalData += 512;
                
        MicRecHeadBuf[4]= MicRecTotalData&0xff;
        MicRecHeadBuf[5]=(MicRecTotalData>>8)&0xff;
        MicRecHeadBuf[6]=(MicRecTotalData>>16)&0xff;
        MicRecHeadBuf[7]=(MicRecTotalData>>24)&0xff;    
        
#if(FLASHROM_TYPE == _MLC_)
        MLCEncoderOutputBufferLeftData = 0;
    #if 0 //不回写数据大小到文件头，播放时用文件大小计算时间
 	    SecIndex=((FileInfo[hMainFile].FstClus - 2) << LogSecPerClus)+BootSector.FirstDataSector;
 	    NandFlashWriteSector(SecIndex, MicRecHeadBuf);
    #endif
#else 	    
        FileSeek(0,SEEK_SET,hMainFile);
        FileWrite(&MicRecHeadBuf[0],512,hMainFile);
#endif
	}
    FileClose(hMainFile);
    
    SysFreeMemeryFlash = (unsigned long)(EncPlayInfo.Bitrate * g_dwEncoderRemainTimeSecond) >>3;//2006-11-03
    
    if (EncPlayInfo.PlayState == MUSIC_ENCODE_END) {
		//MUSIC_ENCODE_END(磁盘空间已满): 1. 写入磁盘失败 2. 录音剩余时间 < 5秒
		SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, RECORD_MSG_DISK_FULL_TXT);
        
        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        Fun1ModuleChange();
        //-----------------------------------------------
        //修改文件格式不支持
        MediaSetup(MemorySelect);
        FileInit();
        //------------------------------------------------
        if(EncPlayInfo.Mode == ENCODE_FM_REC){
            PreDisplayFlag = DISP_FM;
            
            PostSche1Message(MSG_SCHEDULER1_FM);
            PostFun1Message(MSG_FUNCTION1_FM,MSG_FUNCTION1_FM_INIT,0,0,0);
            KeyReset();
        }
        else{
            MainMenuState = 0;
            PreDisplayFlag = DISP_MENU;
            
            SysCpuSetFreq(51);
            SysTimer0Set(51,25000);
        }
        DisplayRecoveryTime = 0;
        DisplayFlag = DISP_DIALOG;
        
        bitclear_creg(%imask,5);
    	SystemInfo.bc.bSysIdleEn = 1;
    	SysIdleCounter = 0;
    	bitset_creg(%imask,5);
	} else {
	    PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_HEAD,EncPlayInfo.Mode,0,0/*,0,0,0*/);
	}
}
/*------------------------------------------------------------------------------
Function:PCM Buffer Change
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
void EncoderInputDataFormat(short * pInputData )
{
    short i;
    short tempbuffer[576];
    
    for (i = 0; i < 576; i++)                   //0--1152 R ----> tempBuffer
        tempbuffer[i] = pInputData[(i<<1) + 1];
    for (i = 0; i < 1152; i++)                  //0--2304 L ----> 0--1152
        pInputData[i] = pInputData[i<<1];
    
    for (i = 1; i <= 576; i++)                   //1152--2304R ----> (2304-576)--2304
        pInputData[2304 - i] = pInputData[2304 - (i<<1) + 1];
    
    for (i = 0; i < 576; i++)                    //tempBuffer ----> 1152--(1152+576)
        pInputData[1152 + i] = tempbuffer[i];
}
/*------------------------------------------------------------------------------
Function:PCM Buffer Change
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".enc_text,\"ax\"")))
short EncodePlay()
{
    short i, j;
    short ret = 0;
    long  dwTemp;
    short wLowLevel,TestPCMLen;
	short enc_byte;
    short mode = EncPlayInfo.Mode;
    unsigned char* pOutputbuf;

    if ((EncodeFlag.bc.bPcmBufFull == 1) && (EncodeFlag.bc.bEncodeEnable))
    {
        EncodeFlag.bc.bPcmBufFull = 0;
        
        if (EncodeFlag.bc.bTheFirstPcmData == 1) {
            EncodeFlag.bc.bTheFirstPcmData = 0;
            EncodeFlag.bc.bPcmBufEmpty = 1;
            return 0;
        }
        
        if(EncPlayInfo.Mode == ENCODE_MIC_REC) {
        	g_dwSampleCount += MSADPCM_CH_NUMPCM;
        	for(i = 0; i< MSADPCM_CH_NUMPCM; i++) {
        	    pEncodeInputBuffer[i] = pEncodeInputBuffer[i<<1];
        	}
            msadpcm_encode_block(&ms_data, MicRecEncodeBuf,pEncodeInputBuffer,MSADPCM_CH_NUMPCM);
            MicRecTotalData += ms_data.samplesPerBlock;//256;//
            enc_byte = ms_data.samplesPerBlock;
            pOutputbuf = MicRecEncodeBuf;
        } else {
#if(FLASHROM_TYPE == _MLC_)
        	g_dwSampleCount += LINE_IN_MSADPCM_CH_NUMPCM;
            msadpcm_encode_block(&ms_data, MicRecEncodeBuf,pEncodeInputBuffer,LINE_IN_MSADPCM_CH_NUMPCM * ms_data.channelCount);
            MicRecTotalData += ms_data.samplesPerBlock;
            enc_byte = ms_data.samplesPerBlock;
            pOutputbuf = MicRecEncodeBuf;
#else
        	g_dwSampleCount += MP3ENC_CH_NUMPCM;
        	EncoderInputDataFormat( pEncodeInputBuffer );
        	ret = rock_encode_mp3_frame(pEncodeInputBuffer,&Mp3enc_obj,&Rock_obj); //
            enc_byte = put_mp3(&Rock_obj.bs);
            mp3enc_total_data += enc_byte;
            pOutputbuf = (unsigned char*)Rock_obj.bs.buf;
#endif
        }
        EncodeFlag.bc.bPcmBufEmpty = 1;

#if(FLASHROM_TYPE == _MLC_)
        j = 0;
        
        //buffer full ,return
        if(MLCEncoderOutputBufferLeftData >= (MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE *MLC_ENCODER_BUFFER_NUM) )return 0;
            
        MLCEncoderOutputBufferLeftData +=  enc_byte;
        if((pMLCEncoderOutputBufferOffset + enc_byte) >= (MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE *MLC_ENCODER_BUFFER_NUM) )
        {
            j = MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE * MLC_ENCODER_BUFFER_NUM - pMLCEncoderOutputBufferOffset;
            enc_byte -= j;
            for(i=0;i<j;i++)
            {
                #if 0
                    MLCEncoderOutputBuffer[pMLCEncoderOutputBufferOffset] = *(pOutputbuf++);
                #else
                    MLCEncoderOutputBuffer[pMLCEncoderOutputBufferOffset] = pOutputbuf[ i ];
                #endif
                pMLCEncoderOutputBufferOffset++;
            }
            pMLCEncoderOutputBufferOffset = 0;
        }

        for(i=0;i<enc_byte;i++)
        {
            #if 0
                MLCEncoderOutputBuffer[pMLCEncoderOutputBufferOffset] = *(pOutputbuf++);
            #else
                MLCEncoderOutputBuffer[pMLCEncoderOutputBufferOffset] = pOutputbuf[ i + j ];
            #endif
            pMLCEncoderOutputBufferOffset++;
        }
        
#else        
        j = 0;
        i=EncodeOutputBufferRemainData;
        enc_byte +=  EncodeOutputBufferRemainData;
        while(enc_byte>=512)
        {
            for(;i<512;i++)
            {
                EncodeOutputBuffer[i] = pOutputbuf[j];
                j++;
            }
            i = 0;
            EncodeOutputBufferRemainData = 0;
            enc_byte -=MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE;
                ret = FileWrite(EncodeOutputBuffer,MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE,hMainFile);  
            if(ret != MLC_ENCODER_WRITE2FLASH_BUFFER_SIZE) {
                EncDispFlag |= ENC_DISPFLAG_STATUS;
                EncPlayInfo.PlayState = MUSIC_ENCODE_END;
                EncPlayStatus = MUSIC_ENCODE_STOP;
                PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_END,mode,0,0/*,0,0,0*/);
            }
        }
        i = EncodeOutputBufferRemainData;
        EncodeOutputBufferRemainData = enc_byte;
        //enc_byte -= i;
        for(;i<enc_byte;i++)
        {
            EncodeOutputBuffer[i] = pOutputbuf[j];
            j++;
        }
#endif

    }
        
    if(g_dwSampleCount > EncPlayInfo.Freq)
    {
	    g_dwSampleCount -= EncPlayInfo.Freq;
	    g_dwEncoderCurrTimeSecond ++;
	    g_dwEncoderRemainTimeSecond --;

	    if(g_dwEncoderRemainTimeSecond < 1) {
	        //EncodeFlag.bc.bDiskFull = 1;
            EncDispFlag |= ENC_DISPFLAG_STATUS;
            EncPlayInfo.PlayState = MUSIC_ENCODE_END;
            EncPlayStatus = MUSIC_ENCODE_STOP;
            PostFun1Message(MSG_FUNCTION1_ENCODE_PROC,MSG_FUNCTION1_ENC_END,mode,0,0/*,0,0,0*/);
        }
        
        j = (g_dwEncoderCurrTimeSecond*( TIME_GUAGE_CONST - 1 ))/(g_dwEncoderTotleTimeSecond);
        if( j != EncPlayInfo.TimeGuageVal){
            EncPlayInfo.TimeGuageVal = j;
            EncDispFlag |= ENC_DISPFLAG_GUAGE;
        }
        EncDispFlag |= ENC_DISPFLAG_CUR_TIME;
    } 
    return 0;
}
//******************************************************************************
