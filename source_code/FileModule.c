//******************************************************************************
//main.c          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#define  IN_FILEMODULE

#include <Creg.h>
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"
#include "Resource.h"

#include "Config.h"
#include "Message.h"

unsigned int GetRandomNum(unsigned int *pBuffer, unsigned int RandomCount);
unsigned int GetRandomFileNum(unsigned int CurrentFileNum,unsigned int TotalFileNum,unsigned int *pBuffer);
void CreatRandomFileList(unsigned int CurrentFileNum,unsigned int TotalFileNum,unsigned int *pBuffer);
void ClearRandomBufferBit(unsigned int CurrentFileNum,unsigned int *pBuffer);
void GetDirPath(unsigned char *pPath);

//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
Explain:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
unsigned int GetRandomNum(unsigned int *pBuffer, unsigned int TotalFileNum)
{
    unsigned int i,j,BitY,BitX;
    unsigned int RandomCount;
    
    if (TotalFileNum > 256) {
        RandomCount = GlobalRanDomNum % 256;
    } else {
        RandomCount = GlobalRanDomNum % TotalFileNum;
    }
    
    BitX = RandomCount & 0x000f;
    BitY = ((RandomCount & 0x00ff) >> 4);
    
    j = BitX;
    for(i=BitY;i<16;i++)
    {
        if(pBuffer[i]!=0)
        {
            for(;j<16;j++)
            {
                if(pBuffer[i] & (0x0001 << j))
                    return((i<<4)|j);
            }
        }
        j = 0;
    }
    for(i=0;i<=BitY;i++)
    {
        if(pBuffer[i]!=0)
        {
            for(j=0;j<16;j++)
            {
                if(pBuffer[i] & (0x0001 << j))
                    return((i<<4)|j);
            }
        }
    }
    return(0xffff);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
Explain:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
unsigned int GetRandomFileNum(unsigned int CurrentFileNum,unsigned int TotalFileNum,unsigned int *pBuffer)
{
    unsigned int RandomNum;
    
    RandomNum = GetRandomNum(pBuffer,TotalFileNum);
    if(RandomNum != 0xffff)
    {
        CurrentFileNum = ((CurrentFileNum - 1) & 0xff00) | (RandomNum & 0x00ff);
    }else
    {
        CurrentFileNum += 256;
        if(CurrentFileNum > TotalFileNum)
            CurrentFileNum = 1;
        CreatRandomFileList(CurrentFileNum,TotalFileNum,pBuffer);
        RandomNum = GetRandomNum(pBuffer,TotalFileNum);
        CurrentFileNum = ((CurrentFileNum - 1) & 0xff00) | (RandomNum & 0x00ff);
    }
    return(CurrentFileNum + 1);
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
Explain:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void CreatRandomFileList(unsigned int CurrentFileNum,unsigned int TotalFileNum,unsigned int *pBuffer)
{
    unsigned int BitX,BitY,i,j;
    
    for(i=0;i<16;i++)
        pBuffer[i] = 0x0000;
    
    j = TotalFileNum - (CurrentFileNum - 1);
    if(j >= 256)
    {
        for(i=0;i<16;i++)
            pBuffer[i] = 0xffff;
    }else
    {
        BitX = j & 0x000f;
        BitY = j >> 4;
        for(i=0;i<BitY;i++)
           pBuffer[i] = 0xffff;
        i = 0;
        for(j=0;j<BitX;j++) 
            i = ((i<<1)|0x0001);
        pBuffer[BitY] = i;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
Explain:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void ClearRandomBufferBit(unsigned int CurrentFileNum,unsigned int *pBuffer)
{
    unsigned int BitX,BitY;
    
    CurrentFileNum = (CurrentFileNum - 1) & 0x00ff;
    BitX = CurrentFileNum & 0x000f;
    BitY = CurrentFileNum >> 4;
    
    pBuffer[BitY] &= (~(0x0001 << BitX));
}
//******************************************************************************
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void FindMusicFile(unsigned int FileType, unsigned int FindDirection,unsigned int FindRange,unsigned int FindOrder)
{
    unsigned int FindFileResult = 0xffff;
    unsigned int i;
    unsigned int tempFileNum;
    unsigned int uiNeedFindNext = 1;
    
    if(FindOrder == MSG_FIND_FILE_RANDOM)
    {   /*Random Find File*/
        SysFile1Info.CurrentFileNum = GetRandomFileNum(SysFile1Info.CurrentFileNum,SysFile1Info.TotalFiles,File1RandomBuffer);
        SysFile1Info.PlayedFileNum -- ;
    } else {
        if(FindDirection == MSG_FIND_FILE_NEXT) {
            if( ++SysFile1Info.CurrentFileNum > SysFile1Info.TotalFiles){
                SysFile1Info.CurrentFileNum = 1;
//------------------------------------------------------------------------------
//(2006-12-29 15:59 Hjl)
                tempFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString);
                if(FindRange == MSG_FIND_FILE_DIR){
                    FindFileResult = FindFirst(&SysFile1Info.Fdt, &FindDataPlay, SysFile1Info.Path, MusicFileExtString);
                    uiNeedFindNext = 0;
                }
                else{
                    GetDirPath(SysFile1Info.Path);
                    FindFileResult = FindFirst(&SysFile1Info.Fdt, &FindDataPlay, SysFile1Info.Path, MusicFileExtString);                        
                    uiNeedFindNext = FindFileResult;    //解决：当磁盘中只有一个文件夹，且文件夹中只有一个音乐文件时进行上下曲切换会死机的问题；
                                                        //      如果有多个文件时进入切换，当前曲目号不会改变。
                }
//------------------------------------------------------------------------------
            }                
        } else if(FindDirection == MSG_FIND_FILE_PREV) {
            if (--SysFile1Info.CurrentFileNum == 0) {
                SysFile1Info.CurrentFileNum = SysFile1Info.TotalFiles;                    
            }
        }
        SysFile1Info.PlayedFileNum = SysFile1Info.CurrentFileNum;
        CreatRandomFileList(SysFile1Info.PlayedFileNum,SysFile1Info.TotalFiles,File1RandomBuffer);
    }


// ------------ hjl 2006-12-14 10:25 -------------------------------------------
// ----解决目录循环时文件查找不对的问题, 上一曲时会提示“文件格式不支持”-------
    if(FindOrder == MSG_FIND_FILE_RANDOM || FindDirection == MSG_FIND_FILE_PREV)
    {
        tempFileNum = SysFile1Info.CurrentFileNum;
        if(FindRange != MSG_FIND_FILE_DIR){
            tempFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
            GetDirPath(SysFile1Info.Path);
        }
        
        FindFileResult = FindFirst(&SysFile1Info.Fdt, &FindDataPlay, SysFile1Info.Path, MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
        for (i=1; i<tempFileNum; i++){
            FindFileResult = FindNext(&SysFile1Info.Fdt, &FindDataPlay, MusicFileExtString/*"MP1MP2MP3WMAWAV"*/); 
        }
        uiNeedFindNext = 0;   
    }
//------------------------------------------------------------------------------
    switch (FileType)
    {
    case MSG_FIND_MUSIC_FILE:
//------------------------------------------------------------------------------
//修改文件查找方式，加快文件查找速度(2006-12-29 16:10 Hjl)
        if ((FindDirection == MSG_FIND_FILE_CURRENT)){
            FindFileResult = 0;
        }
        else{
            if ((uiNeedFindNext) && (FindDirection == MSG_FIND_FILE_NEXT)){                         
                FindFileResult = FindNext(&SysFile1Info.Fdt, &FindDataPlay, MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);                                     
                if (FindFileResult == NOT_FIND_FILE){
                    tempFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
                    GetDirPath(SysFile1Info.Path);
                    FindFileResult = FindFirst(&SysFile1Info.Fdt, &FindDataPlay, SysFile1Info.Path, MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
                }
            }
        }
        if(FindRange != MSG_FIND_FILE_DIR){
            GetDirPath(SysFile1Info.Path);
        }
        GetLongFileName(SysFile1Info.LongFileName);
//------------------------------------------------------------------------------
        if ((SysFile1Info.Fdt.Name[8] == 0x4d/*'M'*/) && 
            (SysFile1Info.Fdt.Name[9] == 0x50/*'P'*/) &&
            ((SysFile1Info.Fdt.Name[10] == 0x31/*'1'*/) || 
             (SysFile1Info.Fdt.Name[10] == 0x32/*'2'*/) ||
             (SysFile1Info.Fdt.Name[10] == 0x33/*'3'*/)))
        {
            PostSche1Message(MSG_SCHEDULER1_MP3_DECODE/*,0,0,0,0,0,0,0*/);
            PostFun1Message(MSG_FUNCTION1_MUSIC_MP3,MSG_FUNCTION1_MP3_INIT,0,0,0/*,0,0,0*/);
        
        } else if ((SysFile1Info.Fdt.Name[8] == 0x57/*'W'*/) &&
                   (SysFile1Info.Fdt.Name[9] == 0x4d/*'M'*/) &&
                   (SysFile1Info.Fdt.Name[10] == 0x41/*'A'*/))
        {
            PostSche1Message(MSG_SCHEDULER1_WMA_DECODE/*,0,0,0,0,0,0,0*/);
            PostFun1Message(MSG_FUNCTION1_MUSIC_WMA,MSG_FUNCTION1_WMA_INIT,0,0,0/*,0,0,0*/);
        } else if ((SysFile1Info.Fdt.Name[8] == 0x57/*'W'*/) &&
                   (SysFile1Info.Fdt.Name[9] == 0x41/*'A'*/) &&
                   (SysFile1Info.Fdt.Name[10] == 0x56/*'V'*/))
      
        {
            PostSche1Message(MSG_SCHEDULER1_WAV_DECODE/*,0,0,0,0,0,0,0*/);
            PostFun1Message(MSG_FUNCTION1_MUSIC_WAV,MSG_FUNCTION1_WAV_INIT,0,0,0/*,0,0,0*/);   
      	}
        break;
        
    case MSG_FIND_AVI_FILE:
        if(FindRange == MSG_FIND_FILE_DIR){
            FindFileResult = FindFile(&SysFile1Info.Fdt,SysFile1Info.CurrentFileNum,SysFile1Info.Path,MovieFileExtString/*"AVI"*/);
        } else {
            FindFileResult = FindFile(&SysFile1Info.Fdt,SysFile1Info.CurrentFileNum,"\\",MovieFileExtString/*"AVI"*/);
            GetDirPath(SysFile1Info.Path);
        }
        PostSche1Message(MSG_SCHEDULER1_AVI/*,0,0,0,0,0,0,0*/);
        PostFun1Message(MSG_FUNCTION1_AVI_DECODE,MSG_FUNCTION1_AVI_DECODE_INIT,0,0,0/*,0,0,0*/);
        break;
    
    default:
        break;
    }
    //GetDirPath(SysFile1Info.Path);
    ClearRandomBufferBit(SysFile1Info.CurrentFileNum,File1RandomBuffer);
    
    if(FindFileResult != RETURN_OK)
    {   /*Display Not Find File!*/
        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
        PostSche1Message(MSG_SCHEDULER1_NONE/*,0,0,0,0,0,0,0*/);
        PreDisplayFlag = DISP_MENU;
        DisplayFlag = DISP_DIALOG;
        DisplayRecoveryTime = 0;
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        return;
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
//__attribute__((section(".sys_text,\"ax\"")))
void FindDisplayFile(unsigned int FileType, unsigned int FindDirection,unsigned int FindRange,unsigned int FindOrder)
{
 	unsigned int i;   
	unsigned int FindFileResult;
    unsigned int tempFileNum;
    unsigned int uiNeedFindNext;
    
    uiNeedFindNext = 1;
    
    if(FindDirection == MSG_FIND_FILE_NEXT) {
        if( ++SysFile2Info.CurrentFileNum > SysFile2Info.TotalFiles){
            SysFile2Info.CurrentFileNum = 1;
//------------------------------------------------------------------------------
//(2006-12-29 15:59 Hjl)
            tempFileNum = GetCurFileNum(SysFile2Info.CurrentFileNum,JpegFileExtString/*"JPG"*/);
            if(FindRange == MSG_FIND_FILE_DIR){
                FindFileResult = FindFirst(&SysFile2Info.Fdt, &FindDataFunction2, SysFile2Info.Path, JpegFileExtString/*"JPG"*/);
            }
            else{
                FindFileResult = FindFirst(&SysFile2Info.Fdt, &FindDataFunction2, "\\", JpegFileExtString/*"JPG"*/);
            }
            uiNeedFindNext = 0;
//------------------------------------------------------------------------------
        }
    } else if(FindDirection == MSG_FIND_FILE_PREV) {
        if( --SysFile2Info.CurrentFileNum == 0) {
            SysFile2Info.CurrentFileNum = SysFile2Info.TotalFiles;                
        }
//------------------------------------------------------------------------------
//(2006-12-29 15:59 Hjl)
        FindFileResult = FindFirst(&SysFile2Info.Fdt, &FindDataFunction2, SysFile2Info.Path, JpegFileExtString/*"JPG"*/);
        for (i=1; i<SysFile2Info.CurrentFileNum; i++){
            FindFileResult = FindNext(&SysFile2Info.Fdt, &FindDataFunction2, JpegFileExtString/*"JPG"*/); 
        }
        uiNeedFindNext = 0;
//------------------------------------------------------------------------------
    }
    
    switch (FileType)
    {
    case MSG_FIND_JPEG_FILE:
//------------------------------------------------------------------------------
//(2006-12-29 15:59 Hjl)
        if (FindDirection == MSG_FIND_FILE_CURRENT){
                FindFileResult = 0;
        }
        else {
            if ((uiNeedFindNext) && (FindDirection == MSG_FIND_FILE_NEXT)){                    
                FindFileResult = FindNext(&SysFile2Info.Fdt, &FindDataFunction2, JpegFileExtString/*"JPG"*/);                     
                if (FindFileResult == NOT_FIND_FILE){
                    tempFileNum = GetCurFileNum(SysFile2Info.CurrentFileNum, JpegFileExtString/*"JPG"*/);
                    GetDirPath(SysFile2Info.Path);
                    FindFileResult = FindFirst(&SysFile2Info.Fdt, &FindDataFunction2, SysFile2Info.Path, JpegFileExtString/*"JPG"*/);
                }
            }            
            if(FindRange != MSG_FIND_FILE_DIR){
                GetDirPath(SysFile2Info.Path);             
            }
        }
//------------------------------------------------------------------------------
        PostSche2Message(MSG_SCHEDULER2_JPEG/*,0,0,0,0,0,0,0*/);
        PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_DECODE_INIT,0,0,0/*,0,0,0*/);
        break;

//------------------------------------------------------------------------------
//由于文本没有按键选择上一个或下一个，文本查找直接在资源管理器中处理(2006-12-29 15:59 Hjl)
#if 0
    case MSG_FIND_TEXT_FILE:
        if(FindRange == MSG_FIND_FILE_DIR){
            FindFileResult = FindFile(&SysFile2Info.Fdt,SysFile2Info.CurrentFileNum,SysFile2Info.Path,TextFileExtString/*"TXT"*/);
        } else {
            FindFileResult = FindFile(&SysFile2Info.Fdt,SysFile2Info.CurrentFileNum,"\\",TextFileExtString/*"TXT"*/);
            GetDirPath(SysFile1Info.Path);
        }
        PostSche2Message(MSG_SCHEDULER2_TXT/*,0,0,0,0,0,0,0*/);
        PostFun2Message(MSG_FUNCTION2_TXT,MSG_FUNCTION2_TXT_INIT,0,0,0/*,0,0,0*/);
        break;
#endif
//------------------------------------------------------------------------------
    default:
        break;
    }
    
    if(FindFileResult != RETURN_OK)
    {   /*Display Not Find File!*/
        PostSche2Message(MSG_SCHEDULER2_NONE/*,0,0,0,0,0,0,0*/);
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;//PostFun2Message(MSG_FUNCTION2_NONE,0,0,0,0/*,0,0,0*/);
        if (Sys1InitFlag.word == 0xffff) {
            PreDisplayFlag = DISP_MENU;
            DisplayFlag = DISP_DIALOG;
            DisplayRecoveryTime = 0;
            SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        }
        return;
    }
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void GetDirPath(unsigned char *pPath)
{
    unsigned int i,j;
    
    *pPath++ = 0x55;    //'U';
    *pPath++ = 0x3a;    //':';
    *pPath++ = 0x5c;    //'\\';
    for (i = 1; i <= CurDirDeep; i++){
        for (j = 0; j < 11 ; j++ ){
            *pPath++ = SubDirInfo[i].DirName[j];
        }
        *pPath++ = 0x5c;//'\\';
    }
    if (CurDirDeep != 0) pPath--;
    *pPath= 0;
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
#if 0
//获取全路径
void GetDirLongPath(unsigned char *pPath)
{
    unsigned int i, j, k;
    
    *pPath++ = 'U';
    *pPath++ = ':';
    *pPath++ = '\\';
    for (i = 1; i <= CurDirDeep; i++){
        if (SubDirInfo[i].LongDirName[0] == 0) {
            k = 0;
            for (j = 0; j < 11 ; j++ ){
                *pPath = SubDirInfo[i].DirName[j];
                if (*pPath == ' ') {
                    k++;
                } else {
                    k = 0;
                }
                pPath ++ ;
            }
            pPath -= k;
            *pPath++ = '\\';
        } else {
            for (j = 0; j < MAX_FILENAME_LEN ; j++) {
                *pPath = SubDirInfo[i].LongDirName[j];
                if (SubDirInfo[i].LongDirName[j] == '\0') {
                    *pPath++ = '\\';
                    break;
                }
                pPath ++;
            }
        }
    }
    if (CurDirDeep != 0) pPath--;
    *pPath= '\0';
}
#else
//只获取当前路径
void GetDirLongPath(unsigned char *pPath)
{
    unsigned int i, j, k;
    unsigned char *TempPath;
    
    if (CurDirDeep == 0) {
        if (MemorySelect == 0) {    //Flash
            *pPath++ = 0x46;    //'F';
            *pPath++ = 0x6c;    //'l';
            *pPath++ = 0x61;    //'a';
            *pPath++ = 0x73;    //'s';
            *pPath++ = 0x68;    //'h';
        } else {                    //SD Card
            *pPath++ = 0x43;    //'C';     
            *pPath++ = 0x61;    //'a';
            *pPath++ = 0x72;    //'r';
            *pPath++ = 0x64;    //'d';
        }
        *pPath++ = 0x3a;        //':';
        *pPath++ = 0x5c;        //'\\';
        *pPath++ = 0;
        return;
    }
    
    *pPath++ = 0x2e;    //'.'
    *pPath++ = 0x2e;    //'.'
    *pPath++ = 0x5c;    //'\\'
    TempPath = pPath;
    for (i = 1; i <= CurDirDeep; i++){
        pPath = TempPath;
        if (SubDirInfo[i].LongDirName[0] == 0) {
            k = 0;
            for (j = 0; j < 11 ; j++ ){
                *pPath = SubDirInfo[i].DirName[j];
                if (*pPath == 0x20/*' '*/) {
                    k++;
                } else {
                    k = 0;
                }
                pPath ++ ;
            }
            pPath -= k;
            *pPath++ = 0x5c;//'\\';
        } else {
            for (j = 0; j < MAX_FILENAME_LEN ; j++) {
                *pPath = SubDirInfo[i].LongDirName[j];
                if (SubDirInfo[i].LongDirName[j] == 0) {
                    *pPath++ = 0x5c;//'\\';
                    break;
                }
                pPath ++;
            }
        }
    }
    if (CurDirDeep != 0) pPath--;
    *pPath = 0;
}
#endif
//******************************************************************************
