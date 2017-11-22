/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : BrowserKey.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       06/01/09       1.0            ORG
*
********************************************************************************
*/
#define _IN_BROWSER_KEY

#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "BrowserMacro.h"

#include "Global.h"
#include "BrowserGlobal.h"
#include "Resource.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"

void BrowserKeyFun(void);
void DeleteFileFunc(void);

#ifdef SYSTEM_SD_CARD_SP
    void BrowserMemSelect(void);
#endif
/******************************************************************************

                            按键处理子程序模块
        
*******************************************************************************/
void BrowserUpProc(void)
{
    FDT             BroFDTBuf;
    BROWSER_STRUCT  *pBro;
    unsigned char *pFilePath;
    unsigned char FileExtend[16];
    unsigned int  StartItem;
    unsigned int  i,j;

    unsigned int  ItemNum = 0;
    unsigned int  k = 0;
    
    MusicSetFreq51();

    for(j=0; j<16; j++){
        FileExtend[j] = DirTreeInfo.CurPickFileType[j+1];
    }

    DirTreeInfo.pBrowser = &BrowserItem[0]; 
    pBro = DirTreeInfo.pBrowser;
    k = 0;
    
    FindFirst(&BroFDTBuf, &FindDataBrowser, DirTreeInfo.PathBuf, "*");

    StartItem = DirTreeInfo.CurId[DirTreeInfo.DirDeep] - DirTreeInfo.KeyCounter;
    k=0;
    i = DirTreeInfo.DirNum;
    if (DirTreeInfo.DirDeep != 0){  
        j = 2;
        FindNext(&BroFDTBuf, &FindDataBrowser, "*");
    }else{ 
        j = 1;
    }
    
    while(i)
    {
        if(BroFDTBuf.Attr & ATTR_DIRECTORY) //2006-1-24 FZF 
        {
            if(k >= StartItem)
            {
                GetLongFileName(pBro->LongFileName);
                pBro->FileType = FILE_ATTR_TYPE_DIR;//FIND_FILE_TYPE_DIR;//BroFDTBuf.Attr;
                pBro->ItemNumber = j;
                pBro = pBro->pNext;
                if(++ItemNum >= BROWSER_SCREEN_PER_LINE)
                {
                     MusicSetFreq51Return();         //add by zyz
                     return;
                }
            }
            k++;
            i--;
        }//End if(BroFDTBuf.Attr == ATTR_DIRECTORY)
        j++;
        if (RETURN_OK != (FindNext(&BroFDTBuf, &FindDataBrowser, "*"))){
            break;
        }        
    }
    j = 1;
    FindFirst(&BroFDTBuf, &FindDataBrowser, DirTreeInfo.PathBuf, FileExtend);
    while(ItemNum < BROWSER_SCREEN_PER_LINE)
    {
        if(((BroFDTBuf.Attr & ATTR_DIRECTORY)==0)&&(BroFDTBuf.Attr & (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_ARCHIVE))) 
        {
            if(k >= StartItem)
            {
                GetLongFileName(pBro->LongFileName);
                pBro->FileType = CheckFileType(&BroFDTBuf.Name[8]);//DirTreeInfo.CurPickFileType[0];//FindFileType;//BroFDTBuf.Attr;
                pBro->ItemNumber = j;
                pBro             = pBro->pNext;
                ItemNum++;
                if(ItemNum >= BROWSER_SCREEN_PER_LINE) break;
            }
            k++;
        }// End if(BroFDTBuf.Attr == (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_ARCHIVE)) 
        j++;

        if(RETURN_OK != FindNext(&BroFDTBuf, &FindDataBrowser, FileExtend)){
            break;
        }
    }
    MusicSetFreq51Return();         //add by zyz
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserKeyProc(void)
{
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
        
    if (SystemInfo.bc.bHoldOn) {
        KeyFlag.bc.bKeyProssed = 0;
        return;
    }
    
#ifdef SYSTEM_SD_CARD_SP
    if (Fun2MsgArray[1] == BROWSER_FUNC_MEM_SELECT) {
        KeyFlag.bc.bKeyProssed = 0;
        BrowserMemSelect();
        return;
    } 
#endif
    if (Fun2MsgArray[1] == BROWSER_FUNC_DEL_FILE) {
        DeleteFileFunc();
        KeyFlag.bc.bKeyProssed = 0;
        return;
    }
    
    KeyFlag.bc.bKeyProssed = 0;
    BrowserKeyFun();
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
#if 0
void BrowserKeyDownProc(unsigned int *FileExtend)
{
    unsigned int    i,j;
    unsigned int    PickFileType;
    //unsigned char   FileExtend[19];
    unsigned int    FileType_Pri;
    unsigned int    FindFileType;
    BROWSER_STRUCT  *pBro;
    FDT BroFDTBuf;
    
    if((DirTreeInfo.DirNum+DirTreeInfo.FileNum) == 0)
        return;
    if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] < (DirTreeInfo.DirNum+DirTreeInfo.FileNum-1))
    {
        DirTreeInfo.CurId[DirTreeInfo.DirDeep]++;
        if(DirTreeInfo.KeyCounter >= BROWSER_SCREEN_PER_LINE-1)
        {
            if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] < DirTreeInfo.DirNum)
            {
                FileType_Pri = ATTR_DIRECTORY;
                FileExtend[0] = '*';
                FileExtend[1] = '\0';
                FileExtend[2] = 0;
                FindFileType  = FIND_FILE_TYPE_DIR;
            }
            else
            {
                FileType_Pri = (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_ARCHIVE);
            }

            if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] == DirTreeInfo.DirNum)
                j = 1; 
            else
                j = DirTreeInfo.pBrowser->pPrev->ItemNumber+1;    
                
            while(1)
            {
                if (NOT_FIND_FILE == FindFileBrowser(&BroFDTBuf, j, DirTreeInfo.PathBuf, FileExtend)){
                    return;
                }
                
                if(BroFDTBuf.Attr&FileType_Pri)
                {
                    if(FileType_Pri == ATTR_DIRECTORY){
                        if(!(BroFDTBuf.Attr & ATTR_DIRECTORY)){
                            j++;
                            continue;
                        }
                    }
                        GetLongFileName(DirTreeInfo.pBrowser->LongFileName);
                        if(FileType_Pri&ATTR_DIRECTORY)
                            DirTreeInfo.pBrowser->FileType = FILE_ATTR_TYPE_DIR;//BroFDTBuf.Attr;
                        else
                            DirTreeInfo.pBrowser->FileType = CheckFileType(&BroFDTBuf.Name[8]);
                        DirTreeInfo.pBrowser->ItemNumber   = j;
                        DirTreeInfo.pBrowser               = DirTreeInfo.pBrowser->pNext;
                        break;
                    i++;
                }
                j++;
            }         
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT2, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
        }
        if(DirTreeInfo.KeyCounter < BROWSER_SCREEN_PER_LINE-1)
        {
            DirTreeInfo.KeyCounter++;
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_SELE, (unsigned int)(&DirTreeInfo), DirTreeInfo.KeyCounter-1, DirTreeInfo.KeyCounter, 0, 0, 0);
        }
    }else{                
        DirTreeInfo.CurId[DirTreeInfo.DirDeep] = 0;
        DirTreeInfo.KeyCounter             = 0;
        BrowserDirInit();
        PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT2, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
    }
}
#else
void BrowserKeyDownProc(unsigned int *FileExtend)
{
}
#endif
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserKeyFun(void)
{
    unsigned int    i,j;
    unsigned int    imask_tmp;
    unsigned int    PickFileType;
    unsigned char   FileExtend[16];
    unsigned int    FileType_Pri;
    unsigned int    FindFileType;
    unsigned char   *pFilePath;
    BROWSER_STRUCT  *pBro;
    FDT BroFDTBuf;
    
    FIND_DATA       FindCurFile;
    
    unsigned int    tempFileNum;
    unsigned int    FindFileResult;
    unsigned int    temp1, temp2;
    
    for(i = 0; i < 16; i++) {
        FileExtend[i] = DirTreeInfo.CurPickFileType[i+1];
    }
    switch (KeyVal) {
//--------------------- PLAY KEY --------------------------
//--------------------- REC KEY ---------------------------
    case KEY_VAL_REC_DOWN:
    {
        if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] >= DirTreeInfo.DirNum)
        {
            pBro = DirTreeInfo.pBrowser;
            for(j = 0; j < DirTreeInfo.KeyCounter; j++)
                pBro = pBro->pNext;
                
            i = FindFileBrowser(&DirTreeInfo.CurFileFDT, pBro->ItemNumber, DirTreeInfo.PathBuf, (unsigned char *)&DirTreeInfo.CurPickFileType[1]);
            
            if(i == RETURN_OK){
                DialogBoxAdjustValue = 0;    
                DispSelectDialogBox(0, 0, DialogBoxAdjustValue, DIALOG_DELETE_TXT);
                PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_DEL_FILE,0,0,0/*,0,0,0*/); 
            }
        }
        break;
    }   
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_PRESS_START:
    {
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        break;
    }       
    case KEY_VAL_MENU_SHORT_UP:
    {
        if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] >= DirTreeInfo.DirNum) {
            pBro = DirTreeInfo.pBrowser;
            for(j=0; j<DirTreeInfo.KeyCounter; j++)
                pBro = pBro->pNext;
            
            i = FindFileBrowser(&DirTreeInfo.CurFileFDT, pBro->ItemNumber, DirTreeInfo.PathBuf, (unsigned char *)&DirTreeInfo.CurPickFileType[1]);
            if(i == RETURN_OK) {    /* 发送功能处理模块消息 */
                switch(pBro->FileType){
                case FILE_ATTR_TYPE_TXT:
                {
                    DisplayFlag = DISP_TEXT;
                    if (Sys2InitFlag.bc.bTxtModuleInitEnable){
                        Fun2ModuleChange();
                    }
//------------------------------------------------------------------------------
//查看文本文件时，直接将路径和文件名赋值给SysFile2Info，不需要在查找文件 (2006-12-29 15:01 fzf)
#if 1
                    memcpy(SysFile2Info.Path , DirTreeInfo.PathBuf ,MAX_PATH_NAME );//3 + MAX_DIR_DEPTH*12 + 1
                    memcpy(&SysFile2Info.Fdt , &DirTreeInfo.CurFileFDT ,sizeof(FDT));
                    PostSche2Message(MSG_SCHEDULER2_TXT);
                    PostFun2Message(MSG_FUNCTION2_TXT,MSG_FUNCTION2_TXT_INIT,0,0,0);
#else
                    SysFile2Info.CurrentFileNum = BrowserFuncHook(SysFile2Info.Path, FILE_ATTR_TYPE_TXT);
                    SysFile2Info.TotalFiles = GetTotalFiles(SysFile2Info.Path,TextFileExtString);
                    PostFun2Message(MSG_FUNCTION2_FILE_FIND,MSG_FIND_TEXT_FILE,MSG_FIND_FILE_CURRENT,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
#endif
//------------------------------------------------------------------------------
                    break;
                }    
                case FILE_ATTR_TYPE_JPEG:
                {
                    DisplayFlag = DISP_JPEG;
                    if (Sys2InitFlag.bc.bJpegModuleInitEnable ) {
                        Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                        Fun1ModuleChange();
                        Fun2ModuleChange();
                    }
                    SysFile2Info.CurrentFileNum = BrowserFuncHook(SysFile2Info.Path, FILE_ATTR_TYPE_JPEG);
                    SysFile2Info.TotalFiles = GetTotalFiles(SysFile2Info.Path,JpegFileExtString);
//------------------------------------------------------------------------------
//加速文件查找(AVI和Text文件在FindFileModule文件中由于查找下一个文件时采用的是FildFile函数，所以这里需要添加该函数)(2006-12-29 15:35 Hjl)
                    FindFileResult = FindFirst(&SysFile2Info.Fdt, &FindDataFunction2, SysFile2Info.Path, JpegFileExtString/*"JPG"*/);
                    for(temp1=1; temp1<SysFile2Info.CurrentFileNum; temp1++){
                        FindFileResult = FindNext(&SysFile2Info.Fdt, &FindDataFunction2, JpegFileExtString/*"JPG"*/);
                    }                    
//------------------------------------------------------------------------------
                    PostFun2Message(MSG_FUNCTION2_FILE_FIND,MSG_FIND_JPEG_FILE,MSG_FIND_FILE_CURRENT,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
                    break;
                }
                case FILE_ATTR_TYPE_MOVIE:
                {
                    DisplayFlag = DISP_AVI;
                    if (Sys1InitFlag.bc.bAviModuleInitEnable) {
                        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                        Fun1ModuleChange();
                        Fun2ModuleChange();
                    }
                    
                    SysFile1Info.CurrentFileNum = BrowserFuncHook(SysFile1Info.Path, FILE_ATTR_TYPE_MOVIE);
                    SysFile1Info.TotalFiles = GetTotalFiles(SysFile1Info.Path,MovieFileExtString);
                        
                    PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_AVI_FILE,MSG_FIND_FILE_CURRENT,MSG_FIND_FILE_DIR,MSG_FIND_FILE_ORDER/*,0,0,0*/);
                    break;
                }
                case FILE_ATTR_TYPE_MUSIC:
                {
                    Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                    Fun1ModuleChange();
                    
                    Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
                    Fun2ModuleChange();
                    
                    DisplayFlag = DISP_MUSIC;
                    MusicDisplayInterface = 1;
                    MusicDispFlag = MUSIC_DISPFLAG_NULL;
                    MusicPlayInfo.PlayState = MUSIC_DECODE_PLAY;
                    
                    SysFile1Info.CurrentFileNum = BrowserFuncHook(SysFile1Info.Path, FILE_ATTR_TYPE_MUSIC);
                    tempFileNum = SysFile1Info.CurrentFileNum;      // hjl 2006-12-13 14:19
                    SysFile1Info.TotalFiles = GetTotalFiles(SysFile1Info.Path,MusicFileExtString);
                    i = MSG_FIND_FILE_DIR;
                    if ((MusicPlayInfo.Mode != MUSIC_DECODE_DIR) && (MusicPlayInfo.Mode != MUSIC_DECODE_DIR_REP)){
                        i = MSG_FIND_FILE_ALL;
                        SysFile1Info.CurrentFileNum = GetGlobeFileNum(SysFile1Info.CurrentFileNum,SysFile1Info.Path,MusicFileExtString);
                        SysFile1Info.TotalFiles = BuildDirInfo(MusicFileExtString);
                        uiTotalMusicFileNum = SysFile1Info.TotalFiles;  
                        tempFileNum = GetCurFileNum(SysFile1Info.CurrentFileNum,MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);                          
                    }
                    
                    j = MSG_FIND_FILE_ORDER;
//------------------------------------------------------------------------------
//加速文件查找(AVI和Text文件在FindFileModule文件中由于查找下一个文件时采用的是FildFile函数，所以这里需要添加该函数)(2006-12-29 15:36 Hjl)
                    FindFileResult = FindFirst(&SysFile1Info.Fdt, &FindDataPlay, SysFile1Info.Path, MusicFileExtString/*"MP1MP2MP3WMAWAV"*/);
                    for(temp1=1; temp1<tempFileNum; temp1++){
                        FindFileResult = FindNext(&SysFile1Info.Fdt, &FindDataPlay, MusicFileExtString);
                    }
//------------------------------------------------------------------------------
                    PostFun1Message(MSG_FUNCTION1_FILE_FIND,MSG_FIND_MUSIC_FILE,MSG_FIND_FILE_CURRENT,i,j/*,0,0,0*/);
                    break;
                }
                default:
                    break;
                }
                PreDisplayFlag = DisplayFlag;
            }
        }else{            
            if(((DirTreeInfo.DirDeep < (MAX_DIR_DEPTH - 1)) || ((DirTreeInfo.DirDeep == (MAX_DIR_DEPTH - 1)) && (DirTreeInfo.CurId[DirTreeInfo.DirDeep] == 0))) && 
                (DirTreeInfo.CurId[DirTreeInfo.DirDeep] < DirTreeInfo.DirNum)) {
                if((DirTreeInfo.CurId[DirTreeInfo.DirDeep] == 0) && (DirTreeInfo.DirDeep != 0)){
                    DirTreeInfo.CurId[DirTreeInfo.DirDeep] = 0;
                    DirTreeInfo.DirDeep--;
                    if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] >= BROWSER_SCREEN_PER_LINE-1)
                        DirTreeInfo.KeyCounter = BROWSER_SCREEN_PER_LINE-1;
                    else
                        DirTreeInfo.KeyCounter = DirTreeInfo.CurId[DirTreeInfo.DirDeep];
                } else {
                    pBro = DirTreeInfo.pBrowser;
                    for(j=0; j<DirTreeInfo.KeyCounter; j++)
                        pBro = pBro->pNext;
                    
                    FindFirst(&BroFDTBuf, &FindDataBrowser, DirTreeInfo.PathBuf, "*");
                    j = 1;
                    while(j != pBro->ItemNumber){
                        if(RETURN_OK != FindNext(&BroFDTBuf, &FindDataBrowser, "*")){
                            break;
                        }
                        j++;
                    } 
                    pFilePath = &DirTreeInfo.PathBuf[0];
                    
                    while(*pFilePath){
                        pFilePath++;
                    }
                       
                    for(i=0; i<11; i++,j++){
                        *pFilePath++ = BroFDTBuf.Name[i];
                    }
                    *pFilePath++ = '\\';
                    *pFilePath++ = 0;
                    DirTreeInfo.DirDeep++;
                    
                    DirTreeInfo.CurId[DirTreeInfo.DirDeep] = 0;
                    DirTreeInfo.KeyCounter             = 0;
                }
                BrowserDirInit();
                PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT1, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
            }
        }
        break;
    }
//--------------------- UP KEY ---------------------------
    case KEY_VAL_UP_DOWN:
    case KEY_VAL_UP_PRESS:
    {
        if((DirTreeInfo.DirNum+DirTreeInfo.FileNum) == 0) break;
        
        if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] > 0) {
            DirTreeInfo.CurId[DirTreeInfo.DirDeep]--;
        
            if(DirTreeInfo.KeyCounter == 0) 
            {
                BrowserUpProc();
                PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT2, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
            }
            else
            {
                DirTreeInfo.KeyCounter--;
                PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_SELE, (unsigned int)(&DirTreeInfo), DirTreeInfo.KeyCounter+1, DirTreeInfo.KeyCounter, 0, 0, 0);
            }
        }else{
            unsigned int TempFileNum = DirTreeInfo.DirNum + DirTreeInfo.FileNum ;
            DirTreeInfo.CurId[DirTreeInfo.DirDeep] = TempFileNum-1;
            
            if( TempFileNum > BROWSER_SCREEN_PER_LINE-1){
                DirTreeInfo.KeyCounter = BROWSER_SCREEN_PER_LINE -1;
            }else
            {
                DirTreeInfo.KeyCounter = TempFileNum-1;
            }
            BrowserDirInit();
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT2, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
        }
        break;
    }
//------------------- DOWN KEY ---------------------------
    case KEY_VAL_DOWN_DOWN:
    case KEY_VAL_DOWN_PRESS:
    {
            if((DirTreeInfo.DirNum+DirTreeInfo.FileNum) == 0)
                break;
            if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] < (DirTreeInfo.DirNum+DirTreeInfo.FileNum-1))
            {
                DirTreeInfo.CurId[DirTreeInfo.DirDeep]++;
                if(DirTreeInfo.KeyCounter >= BROWSER_SCREEN_PER_LINE-1)
                {
                    if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] < DirTreeInfo.DirNum)
                    {
                        FileType_Pri = ATTR_DIRECTORY;
                        FileExtend[0] = '*';
                        FileExtend[1] = '\0';
                        FileExtend[2] = 0;
                        FindFileType  = FIND_FILE_TYPE_DIR;
                    }
                    else
                    {
                        FileType_Pri = (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_ARCHIVE);
                    }

                    if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] == DirTreeInfo.DirNum){
                        j = 1; 
                        i = FindFirst(&BroFDTBuf, &FindDataBrowser, DirTreeInfo.PathBuf, FileExtend);
                    }else{
                        j = DirTreeInfo.pBrowser->pPrev->ItemNumber+1;    
                        i = FindNext(&BroFDTBuf, &FindDataBrowser, FileExtend);    
                   }
                   if(i != RETURN_OK) break;
                        
                    do
                    {
                        if(BroFDTBuf.Attr&FileType_Pri)
                        {
                            if(FileType_Pri == ATTR_DIRECTORY){
                                if(!(BroFDTBuf.Attr & ATTR_DIRECTORY)){
                                    j++;
                                    continue;
                                }
                            }
                                GetLongFileName(DirTreeInfo.pBrowser->LongFileName);
                                if(FileType_Pri&ATTR_DIRECTORY)
                                    DirTreeInfo.pBrowser->FileType = FILE_ATTR_TYPE_DIR;//BroFDTBuf.Attr;
                                else
                                    DirTreeInfo.pBrowser->FileType = CheckFileType(&BroFDTBuf.Name[8]);
                                DirTreeInfo.pBrowser->ItemNumber   = j;
                                DirTreeInfo.pBrowser               = DirTreeInfo.pBrowser->pNext;
                                break;
                            i++;
                        }
                        j++;
                    }while(RETURN_OK == FindNext(&BroFDTBuf, &FindDataBrowser, FileExtend));         
                    PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT2, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
                }
                if(DirTreeInfo.KeyCounter < BROWSER_SCREEN_PER_LINE-1)
                {
                    DirTreeInfo.KeyCounter++;
                    PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_SELE, (unsigned int)(&DirTreeInfo), DirTreeInfo.KeyCounter-1, DirTreeInfo.KeyCounter, 0, 0, 0);
                }
            }else{                
                DirTreeInfo.CurId[DirTreeInfo.DirDeep] = 0;
                DirTreeInfo.KeyCounter             = 0;
                BrowserDirInit();
                PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT2, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
            }
            break;
    }

//--------------------- FFW KEY --------------------------
    case KEY_VAL_FFW_SHORT_UP:
    {
        if(DirTreeInfo.DirDeep > 0){
            DirTreeInfo.CurId[DirTreeInfo.DirDeep] = 0;
            DirTreeInfo.DirDeep--;
            if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] >= BROWSER_SCREEN_PER_LINE-1)
                DirTreeInfo.KeyCounter = BROWSER_SCREEN_PER_LINE-1;
            else
                DirTreeInfo.KeyCounter = DirTreeInfo.CurId[DirTreeInfo.DirDeep];
            BrowserDirInit();
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT1, (unsigned int)(&DirTreeInfo),  DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
        }
#ifndef SYSTEM_SD_CARD_SP            
        else{
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
            Fun2ModuleChange();
            MainMenuState = 0;
            DisplayFlag = DISP_MENU;
            break;                
        }
#else          
        else{
            BroActFlag = 0;
            PostFun2Message(MSG_FUNCTION2_BROWSER, BROWSER_FUNC_MEM_SELECT, 0,0,0/*,0,0,0*/);
            BrowserMemSelectDisp(0, MemorySelect);
        }
#endif
    break;
    }

    default:
        break;
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void DeleteFileFunc(void)
{
    unsigned int tmp;
    
    switch(KeyVal){  
//------------------- FFD KEY ----------------------------
//------------------- FFW KEY ----------------------------
    case KEY_VAL_FFD_SHORT_UP:                
    case KEY_VAL_FFW_SHORT_UP:
    {
        DialogBoxAdjustValue++;
        DispSelectDialogBox(0, 0, DialogBoxAdjustValue, DIALOG_DELETE_TXT);
        break;
    }
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_MENU_SHORT_UP:
    {
        if(DialogBoxAdjustValue & 0x01){
            DispSelectDialogBox(0, 0, DialogBoxAdjustValue, DIALOG_DELETING_TXT);
#ifdef LCD_MEMORY_DEV 
            DmaFillImage2Lcd();
#endif
            Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
            Fun1ModuleChange();
            
            bitclear_creg(%imask,5);
            SysIdleCounter = 0;
            SystemInfo.bc.bSysIdleEn = 0;
            bitset_creg(%imask,5);
            
            FileFragCleanFlag = 1;
            SaveSysInformation();
            
            MusicSetFreq51();
            if(FileDelete(DirTreeInfo.PathBuf , DirTreeInfo.CurFileFDT.Name ) == RETURN_OK){
                if (MemorySelect == 0) SysFreeMemeryFlash = GetFreeMem(0);
                    
                if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] > DirTreeInfo.KeyCounter){
                    DirTreeInfo.CurId[DirTreeInfo.DirDeep]--;
                }else if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] == DirTreeInfo.KeyCounter){
                    if((DirTreeInfo.KeyCounter > 0)&&(DirTreeInfo.CurId[DirTreeInfo.DirDeep] == (DirTreeInfo.DirNum + DirTreeInfo.FileNum - 1))){
                        DirTreeInfo.CurId[DirTreeInfo.DirDeep]--;
                        DirTreeInfo.KeyCounter--;
                    }
                }

                MediaSetup(MemorySelect);
                FileInit();
                BrowserDirInit();
            }
            MusicSetFreq51Return();
            
            bitclear_creg(%imask,5);
            SystemInfo.bc.bSysIdleEn = 1;
            bitset_creg(%imask,5);
        }
        
        DeleteFileAdjust = 0;
        PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
        break;
    }
    case KEY_VAL_MENU_PRESS_START:
    {
        DeleteFileAdjust =0;
        PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
        break;
    }
    default:
        break;
    }
}

#ifdef SYSTEM_SD_CARD_SP
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserMemSelect(void)
{
    switch(KeyVal){
//--------------------- FFW KEY ----------------------------
//--------------------- MENU KEY ---------------------------
    case KEY_VAL_FFW_DOWN:
    case KEY_VAL_MENU_PRESS_START:
    {
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        PreDisplayFlag = DisplayFlag;
        break;
    }       
    case KEY_VAL_MENU_SHORT_UP:
    {
        BroActFlag = 1;
    //----------------------------------------------------------------------
    //    添加需要程序(原设置菜单 SD 卡选择)
    //----------------------------------------------------------------------
        if(DirTreeInfo.MemSelect !=  MemorySelect){
            if (DirTreeInfo.MemSelect == 1) {           //SD Card Select
                if (SDCARD_INSERT()) {
                    Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                    Fun1ModuleChange();
                    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0004);
                    Delay1Ms(1);
                    if(MediaSetup(DirTreeInfo.MemSelect)) {
                        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffb);
                        MediaSetup(0);
                        DirTreeInfo.MemSelect = 0;
                    }
                    FileInit();
                } else {
                   DirTreeInfo.MemSelect = 0; 
                }
            } else {                                    //Flash memery Select
                Fun1MsgArray[0] = MSG_FUNCTION1_NONE;
                Fun1ModuleChange();
                
                write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffb);
                MediaSetup(0);
                FileInit();
            }
            MemorySelect = DirTreeInfo.MemSelect;
        }
        PostFun2Message(MSG_FUNCTION2_BROWSER, BROWSER_FUNC_INIT, DirTreeInfo.CurPickFileType[0],0,0/*,0,0,0*/);
        break;
    }
//--------------------- UP KEY -----------------------------
//--------------------- DOWN KEY ---------------------------
    case KEY_VAL_DOWN_DOWN:
    case KEY_VAL_DOWN_PRESS:
    case KEY_VAL_UP_DOWN:
    case KEY_VAL_UP_PRESS:
    {
        if(DirTreeInfo.MemMedium >= 2){
            DirTreeInfo.MemSelect += 1;
            DirTreeInfo.MemSelect &= 1;
            BrowserMemSelectDisp(1, DirTreeInfo.MemSelect);
        }else{
            DirTreeInfo.MemSelect = 0;
        }
        break;
    }
    default:
        break;
    }
}
#endif
//------------------------------------------------------------------------------
