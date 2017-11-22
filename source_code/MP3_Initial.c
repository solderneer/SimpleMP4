//////////////////////////////////////////////////////////////////////////
//  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
//  File name :     mp3_initial.c
//  Author:         Jiliu Huang
//  Description:    
//  Others:         
//  Function list:  
//  History:        
//           <author>      <time>     <version>       <desc>
//           Jiliu Huang  05/09/17      1.0           ORG
//
/////////////////////////////////////////////////////////////////////////
#define _IN_MP3INITIAL

#include <Creg.h>
#include "Memap.h"
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Resource.h"
#include "Mp3DecodeGlobal.h"

#include "Config.h"
#include "Message.h"
#include "id3_rock.h"

//extern  short filemp3iden(short startpointer,long* mp3filesampling,short* mp3filebitrate);
extern short filemp3iden(int *startpointer,unsigned long* mp3filesampling,unsigned int* mp3filebitrate);
    //-------------------------------------------------------
    //only for test
extern  unsigned char FileOpenErrString[]   ;//__attribute__((section(".wma_data,\"aw\"")))= "FILE OPEN ERROR!";
extern  unsigned char FileSizeErrString[]   ;//__attribute__((section(".wma_data,\"aw\"")))= "FILE SIZE ERROR!";
extern  unsigned char FileFormatErrString[] ;//    __attribute__((section(".wma_data,\"aw\"")))= "FILE FORMAT ERROE!";

//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void ParseMp3Id3(unsigned int * Id3Part,unsigned int size)
{
  
    unsigned int i,k=size;
	  while(k>=0)
	  {
	   if(Id3Part[k] != 0x0020 && Id3Part[k] != 0x0000)break;
	   //else Id3Info.id3_album[k] = 0x0000;//rock@20061026 解决ID3显示为空的问题
	   k--;
	  }
		 Id3Part[k+1] = 0x0000;
  
		if((Id3Part[0]!=0xFF)&&(Id3Part[0]!=0xFE)){
		  TxtGbk2Unicode(Id3Part, Id3Part,1);
		  Id3Part[size]=0;
		}else{
			for(i=0;i<size;i++)
			{
			Id3Part[i] = Id3Part[2 + 2*i] + (Id3Part[3 + 2*i]<<8);
			}
			Id3Part[size]=0;
		}
}


/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void Mp3DisplayModeInit(void)
{
    int i;
    
    //ScrollStringDispDisable();
    
    if (Fun2MsgArray[0] == MSG_FUNCTION2_LRC) {
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
    }
    
    MusicDispFlag &= ((~MUSIC_DISPFLAG_PATH) & (~MUSIC_DISPFLAG_FILENAME) & (~MUSIC_DISPFLAG_SPECTRUM) & (~MUSIC_DISPFLAG_ID3) & (~MUSIC_DISPFLAG_LRC));
    
    if (MusicPlayInfo.DispState == MUSIC_DISPLAY) {
        MusicDispFlag |= (MUSIC_DISPFLAG_PATH | MUSIC_DISPFLAG_FILENAME | MUSIC_DISPFLAG_SPECTRUM);
        
        AVI_MP3_DECODE_DISABLE;
        for(i = 21; i >= 0; i--) *(StroboscopeArray + i) = 0;
        AVI_MP3_DECODE_ENABLE;
        
    } else {
        if (Mp3PlayFlag.bc.bMp3LrcHave == 1) {
            memcpy(SysFile2Info.Path , SysFile1Info.Path ,(3 + MAX_DIR_DEPTH*12 + 1) );
            memcpy(SysFile2Info.Fdt.Name , /*SysFile1Info.Fdt.Name*/Mp3LrcFileName ,8);
            SysFile2Info.Fdt.Name [8] = 'L';
            SysFile2Info.Fdt.Name [9] ='R';
            SysFile2Info.Fdt.Name [10] ='C';
            PostFun2Message(MSG_FUNCTION2_LRC,MSG_FUNCTION2_LRC_INIT,0,0,0/*,0,0,0*/); 
        } else {
            MusicPlayInfo.DispState = MusicPlayInfo.DispMode;
            MusicDispFlag |= MUSIC_DISPFLAG_SPECTRUM;
        }
        
        if (Mp3PlayFlag.bc.bMp3Id3Have == 1) {
            MusicDispFlag |= MUSIC_DISPFLAG_ID3;
        } else {
            MusicDispFlag |= (MUSIC_DISPFLAG_PATH | MUSIC_DISPFLAG_FILENAME);
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
__attribute__((section(".mp3_text,\"ax\"")))
void MP3DecodeHead(void)
{    
    int            i,j;
    int            FindFileResult;
    short          mp3fileyesno;
    unsigned int   FileNameLen;
    unsigned short wordreadout;
    unsigned long  Id3Size;
    unsigned long  nums_while;
    SYS_FILE_INFO  SysFileInfoTemp;
    
{//-------------------------Mp3 Variable Init----------------------------------
    MusicPlayInfo.Bitrate = 128;
    MusicPlayInfo.Freq = 44100;
    Mp3PlayStatus = MusicPlayInfo.PlayState;
    
    Mp3CurrTimeSec = 0;
    Mp3TotalTimeSec = 0;
    Mp3PlayFlag.word = 0;
    
    MusicPlayInfo.AB = MUSIC_DECODE_ABOFF;
    MusicPlayInfo.DispState = MusicPlayInfo.DispMode;
    
    if(MusicDisplayInterface == 1) {
        MusicDisplayInterface = 0;
        MusicDispFlag = (MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |
                         MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | MUSIC_DISPFLAG_EQ | MUSIC_DISPFLAG_AB |
                         MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | MUSIC_DISPFLAG_VOL | 
                         MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS | MUSIC_DISPFLAG_ORDER);
    } else {
        MusicDispFlag = (/*MUSIC_DISPFLAG_BACKGROUND | MUSIC_DISPFLAG_HOLD | MUSIC_DISPFLAG_BATT |*/
                         MUSIC_DISPFLAG_FILENUM | MUSIC_DISPFLAG_BITRATE | /*MUSIC_DISPFLAG_EQ |*/MUSIC_DISPFLAG_AB |
                         MUSIC_DISPFLAG_CUR_TIME | MUSIC_DISPFLAG_GUAGE | /*MUSIC_DISPFLAG_VOL |*/
                         MUSIC_DISPFLAG_MODE | MUSIC_DISPFLAG_STATUS/* | MUSIC_DISPFLAG_ORDER*/);
    }
}

{//------------------------Check Mp3 File Head---------------------------------
    if ((hMainFile = FileOpen(SysFile1Info.Path, SysFile1Info.Fdt.Name, "R")) == NOT_OPEN_FILE) {
        MP3FileFormatError = 1;
        PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_NONE,0,0,0/*,0,0,0*/);
        return;
    }
    wordreadout = FileRead((unsigned char *)MP3DecodeTempReadoutBuf, 512,hMainFile);
    Id3Size = 0;
    if((MP3DecodeTempReadoutBuf[0] == 0x0049) && (MP3DecodeTempReadoutBuf[1] == 0x0044))
    {
        Id3Size = (((unsigned long)(MP3DecodeTempReadoutBuf[6]) & 0x007f) << 21/*0x200000*/)
                + (((unsigned long)(MP3DecodeTempReadoutBuf[7]) & 0x007f) << 10/*0x400*/)
                + (((unsigned long)(MP3DecodeTempReadoutBuf[8]) & 0x007f) << 7/*0x80*/)
                +  ((unsigned long)(MP3DecodeTempReadoutBuf[9]) & 0x007f) + 10;
    }
    
    FileSeek(Id3Size, SEEK_SET, hMainFile);
    
    nums_while = 0;
    do {
        for(i=0; i<6; i++){
            if (512 > (wordreadout = FileRead((unsigned char *)MP3DecodeTempReadoutBuf, 512,hMainFile)))
                break;
            for(j = 0; j < 256; j += 1)
                ApplicationInputBufferStart[(i<<8)+j] = ((MP3DecodeTempReadoutBuf[j<<1] << 8)| (MP3DecodeTempReadoutBuf[(j << 1) + 1] ));
        }
        
        mp3fileyesno = filemp3iden(ApplicationInputBufferStart,&MusicPlayInfo.Freq,&MusicPlayInfo.Bitrate);
        nums_while ++ ;
        
		
        if (((Id3Size == 0) && (nums_while > 65535)) || (nums_while > 655) || (wordreadout < 512)/* || (times == 2)*/) {
            FileClose(hMainFile);
            MP3FileFormatError = 1;
            PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_NONE,0,0,0/*,0,0,0*/);
            return; 
        }
    } while(mp3fileyesno != 0xff);
    
    MusicPlayInfo.FirstBitrate = MusicPlayInfo.Bitrate;
    if(MusicPlayInfo.FirstBitrate == 0) MusicPlayInfo.FirstBitrate = 128;
    GetLongFileName(SysFile1Info.LongFileName);
    GetDirLongPath(SysFile1Info.LongPath);
}

{//----------------------------Check ID3 Have----------------------------------
    FileSeek(0, SEEK_SET, hMainFile);
    for(i = 0; i < 8; i++) {
        if (512 > (wordreadout = FileRead((unsigned char*)MP3DecodeTempReadoutBuf, 512,hMainFile))) {
            FileClose(hMainFile);
            MP3FileFormatError = 1;
            PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_NONE,0,0,0/*,0,0,0*/);
            return; 
        }
        for(j = 0; j < wordreadout/* >> 1*/; j += 1)        // 修正ID3信息显示错误
            ApplicationInputBufferStart[(i<<9)+j] = MP3DecodeTempReadoutBuf[j];
    }
    Find_ID3_TAG(&Id3Info,(short *)ApplicationInputBufferStart);
    //---------hjl 2006-12-21 16:45--------------------------------------------------
    ParseMp3Id3(Id3Info.id3_title,Id3Info.id3_title_size);
    ParseMp3Id3(Id3Info.id3_singer,Id3Info.id3_singer_size);
    ParseMp3Id3(Id3Info.id3_album,Id3Info.id3_album_size);
    //-----------------------------------------------------------

    //GetLongFileName(SysFile1Info.LongFileName);   // hjl 2006-12-21 16:23 移至FileModule.c中获取。

    FileSeek(0, SEEK_SET, hMainFile);
    MP3FileOffSet = 0;
    
    if(Id3Info.id3_check == 1) {
        Mp3PlayFlag.bc.bMp3Id3Have = 1;
//        Id3Info.id3_title[ID3_TAG_SIZE-1]=0;
//        Id3Info.id3_album[ID3_TAG_SIZE-1]=0;//rock@20061026 解决ID3显示为空的问题
//        Id3Info.id3_singer[ID3_TAG_SIZE-1]=0;
//        TxtGbk2Unicode(Id3Info.id3_title,Id3Info.id3_title, 1);
//        TxtGbk2Unicode(Id3Info.id3_album,Id3Info.id3_album, 1);
//        TxtGbk2Unicode(Id3Info.id3_singer,Id3Info.id3_singer, 1);
    }
}

{//-------------------------Check Lrc File Have--------------------------------
//    SysFileInfoTemp.Path[0] = '.';
//    SysFileInfoTemp.Path[1] = 0; 
    
    SysFileInfoTemp.TotalFiles = GetTotalFiles(SysFile1Info.Path,"LRC");
    Mp3LrcFileName[0] = 0 ;
    for(i = 0 ; i < MAX_FILENAME_LEN; i++) {
        if(SysFile1Info.LongFileName[i] == 0) break;
    }
    FileNameLen = i - 4;
    
    unsigned long FileIndex = 0;             
    FindFileResult = FindFirst(&SysFileInfoTemp.Fdt, &FindDataFunction2, SysFile1Info.Path, "LRC");
    if (FindFileResult == RETURN_OK){
        for (SysFileInfoTemp.CurrentFileNum = 1; 
             SysFileInfoTemp.CurrentFileNum <= SysFileInfoTemp.TotalFiles; 
             SysFileInfoTemp.CurrentFileNum++) 
        {
            if(FindFileResult != RETURN_OK){
                break;
            }
            GetLongFileName(SysFileInfoTemp.LongFileName);
        
            for (i = 0; i < FileNameLen; i++) {
                unsigned int char1,char2;
                char1 = SysFileInfoTemp.LongFileName[i];
                char2 = SysFile1Info.LongFileName[i];
                if(char1>96 && char1<123)char1 -= 32;
                if(char2>96 && char2<123)char2 -= 32;
                if (char1 != char2) {
                    break;
                }
            }
            if( i == FileNameLen ) {
                memcpy(Mp3LrcFileName , SysFileInfoTemp.Fdt.Name ,8);   //把歌词文件名临时存放在LrcFileName中.           
                if (Fun2MsgArray[0] == MSG_FUNCTION2_NONE) {
                    Mp3PlayFlag.bc.bMp3LrcHave = 1;
                    MusicPlayInfo.DispState = LRC_DISPLAY;
                }
                break;
            }else{
                FindFileResult = FindNext(&SysFileInfoTemp.Fdt, &FindDataFunction2, "LRC");   
            }
        }
    }
}
{//-------------------------Mp3 Variable Init----------------------------------
    
    MP3FileOffSetA = 0;
    MP3FileOffSetB = 0;
    MusicPlayInfo.TimeGuageVal = 0;
    
    Mp3DisplayModeInit();
    
    MP3TimeCount = (unsigned long)(MP3FileOffSet % (((unsigned long)(MusicPlayInfo.FirstBitrate) *125)/20));
    MP3TimeGuageCount = (unsigned long)(MP3FileOffSet % (SysFile1Info.Fdt.FileSize / TIME_GUAGE_CONST));
    Mp3TotalTimeSec = (unsigned long)(SysFile1Info.Fdt.FileSize / ((unsigned long)(MusicPlayInfo.FirstBitrate)*125));
    
    PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_START,0,0,0/*,0,0,0*/);
}
}
//******************************************************************************
