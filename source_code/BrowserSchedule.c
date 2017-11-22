/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : BrowserSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#define _IN_BROWSERSCHEDULE
#include "Resource.h"
#include <creg.h>
#include "Macro.h"
#include "MsgDef.h"
#include "BrowserMacro.h"

#include "Global.h"
#include "BrowserGlobal.h"
#include "Resource.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "Lcd.h"
#include "LcdChar.h"

void DirValueInit(void);
void BrowserDirInit(void);
void BrowserKeyProc(void);
void DeleteFileFunc(void);
void BroswerVariableInit(void);
void BrowserFileTypeInit(unsigned int FindFileType);

unsigned int CheckFileType(unsigned char *pExternt);
unsigned int BrowserFuncHook(unsigned char *FilePath, unsigned int type);

#ifdef SYSTEM_SD_CARD_SP
    void BrowserSDCardDetect(void);
#endif
//******************************************************************************

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserModule(void)
{   
    
    switch(Fun2MsgArray[1]) 
    {
    case BROWSER_FUNC_INIT:
    {        
        BroswerVariableInit();
        KeyReset();
        
        DirValueInit();
        BrowserFileTypeInit(Fun2MsgArray[2]);
        
#ifdef SYSTEM_SD_CARD_SP            
        if(BroActFlag != 0){
            BrowserDirInit();
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum,  DirTreeInfo.KeyCounter, 0, 0, 0);
            PostFun2Message(MSG_FUNCTION2_BROWSER, MSG_FUNCTION2_NONE, 0,0,0/*,0,0,0*/);
        }else{
            DirValueInit();
            DirTreeInfo.MemSelect = MemorySelect;
            BrowserMemSelectDisp(0, DirTreeInfo.MemSelect);
            PostFun2Message(MSG_FUNCTION2_BROWSER, BROWSER_FUNC_MEM_SELECT, 0,0,0/*,0,0,0*/);
        }
#else
       BrowserDirInit();
       PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum,  DirTreeInfo.KeyCounter, 0, 0, 0);
       PostFun2Message(MSG_FUNCTION2_BROWSER, MSG_FUNCTION2_NONE, 0,0,0/*,0,0,0*/); 
#endif
        break;
    }
    case BROWSER_FUNC_RETURN:
    {
        BroswerVariableInit();
        KeyReset();
        
#ifdef SYSTEM_SD_CARD_SP
        BroActFlag = 1;
        DirTreeInfo.MemSelect = MemorySelect;
#endif
        PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum,  DirTreeInfo.KeyCounter, 0, 0, 0);
        PostFun2Message(MSG_FUNCTION2_BROWSER,MSG_FUNCTION2_NONE,0,0,0/*,0,0,0*/);
        break;
    }
#ifdef SYSTEM_SD_CARD_SP
    case BROWSER_FUNC_MEM_SELECT:
    {
        BrowserSDCardDetect();
        break;
    }
#endif
    case BROWSER_FUNC_DEL_FILE:
    default:
        break;
    }
    
    if (DisplayFlag == DISP_BROWSER) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            BrowserKeyProc();
        } else {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 0;
            bitset_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart1 = 1;
            LCD_SDRAM_Standby_Return();
        }
        
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            DisplayPicture(EXPLORER_BATT_PIC_X, EXPLORER_BATT_PIC_Y, EXPLORER_BATT_PIC_XSIZE, EXPLORER_BATT_PIC_YSIZE, (NFA_EXPLORER_BATT_PIC + (unsigned long)(BatteryLevel) * EXPLORER_BATT_PIC_SIZE ));
        }
        
        if (SystemInfo.bc.bHoldDisplay) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bHoldDisplay = 0;
            bitset_creg(%imask,5);
            
            DisplayRecoveryTime = 0;
            PreDisplayFlag = DisplayFlag;
            DisplayFlag = DISP_HOLD;
            
            MusicSetFreq51();
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MAX_PIC_SIZE);
            MusicSetFreq51Return();
            return;
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {
            SystemInfo.bc.bDisplayRecovery = 0;
            //恢复显示图片(重新解码)
//            PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum,  DirTreeInfo.KeyCounter, 0, 0, 0);
        }
        
        DISP_Browser();
        
        if ((SystemInfo.bc.bBackLightOnStart1) && (SystemInfo.bc.bBackLightStatus != 2)) {
            SystemInfo.bc.bBackLightOnStart1 = 0;
            DmaFillImage2Lcd();
            LcdBackLightOn();
            KeyReset();
        }
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BroswerVariableInit(void)
{
    if (Sys2InitFlag.bc.bBrowserModuleEnable) {
        Sys2InitFlag.bc.bBrowserModuleEnable = 0;
    }
    PrevScrollY = EXPLORER_SCROLL_PIC_Y;
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void DirValueInit(void)
{
    unsigned int i;
    unsigned int j;
        
    for(i=0; i<BROWSER_SCREEN_PER_LINE; i++)
    {
        for (j=0; j<MAX_FILENAME_LEN; j++)
            BrowserItem[i].LongFileName[j] = 0;
        BrowserItem[i].ItemNumber = 0xFFFF;
    }

    for(i=0; i<BROWSER_SCREEN_PER_LINE-1; i++)
        BrowserItem[i].pNext = &BrowserItem[i+1];
    for(i=1; i<BROWSER_SCREEN_PER_LINE; i++)
        BrowserItem[i].pPrev = &BrowserItem[i-1];
        BrowserItem[0].pPrev = &BrowserItem[BROWSER_SCREEN_PER_LINE-1];
        BrowserItem[BROWSER_SCREEN_PER_LINE-1].pNext = &BrowserItem[0];
    
    DirTreeInfo.PathBuf[0] = '\\';

    DirTreeInfo.bBrwoserFlag = 0;
    DirTreeInfo.pBrowser     = &BrowserItem[0];
    DirTreeInfo.CurId[0]     = 0;
    DirTreeInfo.CurId[1]     = 0;
    DirTreeInfo.CurId[2]     = 0;
    DirTreeInfo.CurId[3]     = 0;
    DirTreeInfo.DirDeep      = 0;
    DirTreeInfo.DirNum       = 0;
    DirTreeInfo.FileNum      = 0;
    DirTreeInfo.KeyCounter   = 0;
//------------------------------------------------------------------------------    
    //DirTreeInfo.MemMedium    = 0;
    
//------------------------------------------------------------------------------    
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserFileTypeInit(unsigned int FindFileType)
{
    unsigned int i;
    switch(FindFileType)
    {
        case FIND_FILE_TYPE_AVI:
            DirTreeInfo.CurPickFileType[0] = FIND_FILE_TYPE_AVI;
            memcpy(&DirTreeInfo.CurPickFileType[1], MovieFileExtString, 4);
            break;
        case FIND_FILE_TYPE_TXT:
            DirTreeInfo.CurPickFileType[0] = FIND_FILE_TYPE_TXT;
            memcpy(&DirTreeInfo.CurPickFileType[1], TextFileExtString, 4);
            break;
        case FIND_FILE_TYPE_JPEG:
            DirTreeInfo.CurPickFileType[0] = FIND_FILE_TYPE_JPEG;
            memcpy(&DirTreeInfo.CurPickFileType[1], JpegFileExtString, 7);
            break;
        case FIND_FILE_TYPE_MUSIC:
            DirTreeInfo.CurPickFileType[0] = FIND_FILE_TYPE_MUSIC;
            memcpy(&DirTreeInfo.CurPickFileType[1], MusicFileExtString, 16);
            break;            
        default:
            DirTreeInfo.CurPickFileType[0] = FIND_FILE_TYPE_ALL;
            DirTreeInfo.CurPickFileType[1] = '*';
            DirTreeInfo.CurPickFileType[2] = '\0';
            break;
    }
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserDirInit(void)
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
    
    pFilePath = &DirTreeInfo.PathBuf[0];
    while(*pFilePath){
        if(*pFilePath == '\\'){
            if(k >= DirTreeInfo.DirDeep){
                *(pFilePath+1) = '\0';
                    break;
                }
            k++;
            }
        pFilePath++;
    }

    for(j=0; j<16; j++)
    {
        FileExtend[j] = DirTreeInfo.CurPickFileType[j+1];
    }

    i                   = GetTotalFiles(DirTreeInfo.PathBuf, "*");
    DirTreeInfo.FileNum = GetTotalFiles(DirTreeInfo.PathBuf,FileExtend);
    DirTreeInfo.DirNum  = GetTotalSubDir(DirTreeInfo.PathBuf);
    
    if((DirTreeInfo.DirDeep != 0) && (DirTreeInfo.DirNum > 0)){ 
        DirTreeInfo.DirNum -= 1;
    }
    DirTreeInfo.DirTotalItem = i+DirTreeInfo.DirNum;
    BrowserUpProc();
    MusicSetFreq51Return();         //add by zyz
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void GotoNextFolder()
{
#if 0
     if(((DirTreeInfo.DirDeep < (BROWSER_MAX_DIR_DEPTH-1)) || ((DirTreeInfo.DirDeep == (BROWSER_MAX_DIR_DEPTH-1)) && (DirTreeInfo.CurId == 0))) && 
         (DirTreeInfo.CurId[DirTreeInfo.DirDeep] < DirTreeInfo.DirNum)) {
         if((DirTreeInfo.CurId[DirTreeInfo.DirDeep] == 0) && (DirTreeInfo.DirDeep != 0)){
             DirTreeInfo.CurId[DirTreeInfo.DirDeep] = 0;
             DirTreeInfo.DirDeep--;
             if(DirTreeInfo.CurId[DirTreeInfo.DirDeep] >= BROWSER_SCREEN_PER_LINE-1)
                 DirTreeInfo.KeyCounter = BROWSER_SCREEN_PER_LINE-1;
             else
                 DirTreeInfo.KeyCounter = DirTreeInfo.CurId[DirTreeInfo.DirDeep];
         } else {
             DirTreeInfo.DirDeep++;
             DirTreeInfo.CurId[DirTreeInfo.DirDeep] = 0;
             DirTreeInfo.KeyCounter             = 0;
         }
         BrowserDirInit();
         PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT2, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum, DirTreeInfo.KeyCounter, 0, 0, 0);
     }
#endif
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
unsigned int CheckFileType(unsigned char *pExternt)
{
    unsigned int i;
    unsigned char NameStr[3];
    unsigned int  CurFileType = FILE_ATTR_TYPE_OTHER;
    for(i=0; i<3; i++)
        NameStr[i] = *pExternt++;

    switch(NameStr[0])
    {

        case 'M':
            if(NameStr[1] == 'P')
            {
                if((NameStr[2] == '1') || (NameStr[2] == '2') || (NameStr[2] == '3')) 
                {
                    CurFileType = FILE_ATTR_TYPE_MUSIC;
                }
            }
            break;
        case 'W':
            if(((NameStr[1] == 'M') && (NameStr[2] == 'A')) || ((NameStr[1] == 'A') && (NameStr[2] == 'V')))
            {
                CurFileType = FILE_ATTR_TYPE_MUSIC;
            }
            break;
        case 'A':
            if((NameStr[1] == 'V') && (NameStr[2] == 'I'))
            {
                CurFileType = FILE_ATTR_TYPE_MOVIE;
            }
            break;
        case 'T':
            if((NameStr[1] == 'X') && (NameStr[2] == 'T'))
            {
                CurFileType = FILE_ATTR_TYPE_TXT;
            }
            break;
        case 'J':
            if((NameStr[1] == 'P') && (NameStr[2] == 'G'))
            {
                CurFileType = FILE_ATTR_TYPE_JPEG;
            }
            break;
        case 'B':
            if((NameStr[1] == 'M') && (NameStr[2] == 'P'))
            {
                CurFileType = FILE_ATTR_TYPE_JPEG;
            }
            break;
        default:
            CurFileType = FILE_ATTR_TYPE_OTHER;
            break;
    }
    return(CurFileType);
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
unsigned int BrowserFuncHook(unsigned char *FilePath, unsigned int type)
{
    FDT             BroFDTBuf;
    unsigned char FileExtend[16];
    unsigned char *pFilePath;
    unsigned int  StartItem;
    unsigned int  i,j;
    FIND_DATA     find_data_tmp;

    unsigned int  ItemNum = 0;
    unsigned int  FileNum;
    unsigned int  k = 0;
    signed char ReturnVal;
    
    *FilePath++ = 'U';
    *FilePath++ = ':';
    *FilePath++ = '\\';
                            
    for (i = 1; i < MAX_PATH_NAME; i++){
        *FilePath++ = DirTreeInfo.PathBuf[i];
    }
    *FilePath = '\0';

    switch(type)
    {
        case FILE_ATTR_TYPE_MOVIE:
            memcpy(&FileExtend[0], MovieFileExtString, 4);
            break;
        case FILE_ATTR_TYPE_TXT:
            memcpy(&FileExtend[0], TextFileExtString, 4);
            break;
        case FILE_ATTR_TYPE_JPEG:
            memcpy(&FileExtend[0], JpegFileExtString, 7);
            break;
        case FILE_ATTR_TYPE_MUSIC:
            memcpy(&FileExtend[0], MusicFileExtString, 16);
            break;            
        default:
            return(0);
            break;
    }
    j = 1;
    FindFirst(&BroFDTBuf, &find_data_tmp, DirTreeInfo.PathBuf, FileExtend);

    do{
        for(i=0; i<11; i++){
            if(BroFDTBuf.Name[i] != DirTreeInfo.CurFileFDT.Name[i])
                break;
        }           
        if(i == 11) break;  
        j++;
    }while(RETURN_OK == FindNext(&BroFDTBuf, &find_data_tmp, FileExtend));

    return(j);
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
unsigned int BrowserReturnHook(unsigned char *Path, unsigned int Num, unsigned int type)
{
#if 1
{    
    FDT             BroFDTBuf;
    BROWSER_STRUCT  *pBro;
    unsigned char *pFilePath;
    unsigned int  StartItem;
    unsigned int  i,j;

    unsigned int  ItemNum = 0;
    unsigned int  k = 0;
    
    MusicSetFreq51();

    while (*Path != '\\') Path++;
    Path++;
    
    DirValueInit();
    
    DirTreeInfo.PathBuf[1] = 0;
    pFilePath = &DirTreeInfo.PathBuf[0];
	for (k=0; ((k < (MAX_DIR_DEPTH - 1)) && (*Path)); k++)
	{
        StartItem = 0;

		FindFirst(&BroFDTBuf, &FindDataBrowser, DirTreeInfo.PathBuf, "*");

		if (k != 0) {
			FindNext(&BroFDTBuf, &FindDataBrowser, "*");
		}

		do {
			if (BroFDTBuf.Attr & ATTR_DIRECTORY) {
				for (i=0; i<11; i++) {
					if (*(Path+i) != BroFDTBuf.Name[i]) break;
				}
				if (i == 11) {
					DirTreeInfo.CurId[k] = StartItem;
					DirTreeInfo.DirDeep++;
					*pFilePath++ = '\\';
					for (i=0; i<11; i++) *pFilePath++ = *Path++;
					Path++;
					*pFilePath = '\\';
					*(pFilePath+1) = '\0';
					break;
				}
				StartItem++;
			}
		} while (RETURN_OK == (FindNext(&BroFDTBuf, &FindDataBrowser, "*")));
    }
    
    BrowserFileTypeInit(type);

    i                   = GetTotalFiles(DirTreeInfo.PathBuf, "*");
    DirTreeInfo.FileNum = GetTotalFiles(DirTreeInfo.PathBuf,&DirTreeInfo.CurPickFileType[1]);
    DirTreeInfo.DirNum  = GetTotalSubDir(DirTreeInfo.PathBuf);
    
    if ((DirTreeInfo.DirDeep != 0) && (DirTreeInfo.DirNum > 0)) { 
        DirTreeInfo.DirNum -= 1;
    }
    DirTreeInfo.DirTotalItem = i+DirTreeInfo.DirNum;
    

    DirTreeInfo.CurId[DirTreeInfo.DirDeep] = (Num) + DirTreeInfo.DirNum;
    //if((Num + DirTreeInfo.DirNum) < (BROWSER_SCREEN_PER_LINE-1)){
    if((Num + DirTreeInfo.DirNum) < (BROWSER_SCREEN_PER_LINE)){
        DirTreeInfo.KeyCounter = (Num) + DirTreeInfo.DirNum;
    }else{
        DirTreeInfo.KeyCounter = BROWSER_SCREEN_PER_LINE-1;
    }
    BrowserUpProc();

    MusicSetFreq51Return();         //add by zyz
}
#endif
}
//******************************************************************************
#ifdef SYSTEM_SD_CARD_SP
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void BrowserSDCardDetect(void)
{
    unsigned int MemSelectTmp;
    
    if(SDCARD_INSERT()){
        MemSelectTmp = 2;
    }else{
        MemSelectTmp = 1;
    }
    
    if(MemSelectTmp != DirTreeInfo.MemMedium){
        DirTreeInfo.MemMedium = MemSelectTmp;
        
        BrowserMemSelectDisp(0, DirTreeInfo.MemSelect);
        if(DirTreeInfo.MemMedium == 1){ 
            MemorySelect = 0;
            DirTreeInfo.MemSelect = 0;
        }
        if (SystemInfo.bc.bBackLightStatus != 0) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 1;
            bitset_creg(%imask,5);
//            LcdBackLightOn();
        }
    }
}
#endif
//------------------------------------------------------------------------------


