/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : TxtSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/


#include <creg.h>
#include "Macro.h"
#include "Global.h"

#include "config.h"
#include "MsgDef.h"
#include "Lcd.h"
#include "LcdChar.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "Resource.h"
#include "BrowserMacro.h"
#include "BrowserStruct.h"
#include "BrowserGlobal.h"


#ifdef TXT_MODULE
    #define _IN_TXTSCHEDULE
    #include "TxtMacro.h"
    #include "TxtStruct.h"
    #include "TxtGlobal.h"
    #include "TxtSchedule.h"

void TextInit(void);
void TextKeyProc(void);
void TxtKeyUpProc(void);  
void TextFileOpen(void);
void TxtKeyDownProc(void);
void TextVariableInit(void);
void TxtBookMaskSelect(void);
void SaveTextInformation(void);
void LoadTextInformation(void);
void Txt_Byte2DWord(unsigned char *pChar, unsigned long *pLong, unsigned int long_len);
void Txt_DWord2Byte(unsigned long *pLong, unsigned char *pChar, unsigned int long_len);
unsigned int TextGetBytes(unsigned int *s, unsigned int chars);

//******************************************************************************

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TextTimeProc(void *parg)
{
    if(TextAutoPlayFlag != 0) {
        TimerReset(TEXT_TMR_CHANNEL);
        TimerStart(TEXT_TMR_CHANNEL);
        TxtAutoBrowserTimeFlag++;
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TextAutoPlaySetup(void)
{
    unsigned int TextAutoPlayTimeTable[5] = {1*40,2*40,3*40,4*40,5*40};
    
    if (TextAutoPlayFlag != 0) {
        TimerCfgFunc(TEXT_TMR_CHANNEL, TextTimeProc, 0);
        TimerSetTimer(TEXT_TMR_CHANNEL, TextAutoPlayTimeTable[gTxtAutoBrowserTime]);
        TimerStart(TEXT_TMR_CHANNEL);
        TxtAutoBrowserTimeFlag = 1;
    } else {
        TxtAutoBrowserTimeFlag = 0;
        TimerStop(TEXT_TMR_CHANNEL);
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TextModule(void)
{
    switch(Fun2MsgArray[1])
    {
        case MSG_FUNCTION2_TXT_INIT:
        {
            TextVariableInit();
            KeyReset();
            PostFun2Message(MSG_FUNCTION2_TXT,MSG_FUNCTION2_TXT_HEAD,0,0,0/*,0,0,0*/);
            break;
        }    
        case MSG_FUNCTION2_TXT_HEAD:
        {
#if(FLASHROM_TYPE == _MLC_)
            if(Sys1InitFlag.word == 0xffff) {
                MlcDataRefresh();           //如果单独Text工作而Function1不工作的话，需要在适当的时候添加该语句
            }
#endif
            TextInit();
            LoadTextInformation();
            TextFileOpen();
            PostFun2Message(MSG_FUNCTION2_TXT, MSG_FUNCTION2_TXT_PLAY,0,0,0/*,0,0,0*/);
            break;
        }
        
        case MSG_FUNCTION2_TXT_PLAY:
        {    
            if((TextAutoPlayFlag != 0) && (TxtAutoBrowserTimeFlag != 0)) {
                TxtAutoBrowserTimeFlag = 0;
                
                if(Text.EndFileOffset < SysFile2Info.Fdt.FileSize) {
                    TxtKeyDownProc();
                    PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_TEXT,(unsigned int) (&Text), 0, 0, 0, 0, 0);
                }
            }
            break;
        }
        
        case MSG_FUNCTION2_BOOKMASK_SELECT:
        case MSG_FUNCTION2_TXT_END:
        default:
            break;
    }
    
    if (TxtFileFormatError == 1) {
        if (/*(DisplayFlag != DISP_TEXT) ||*/ (SystemInfo.bc.bDisplayRecovery)/* || (SystemInfo.bc.bBackLightStatus == 2)*/) {
            //文件错误，返回资源管理器
            SystemInfo.bc.bDisplayRecovery = 0;
            
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;//PostFun2Message(MSG_FUNCTION2_NONE,0,0,0,0/*,0,0,0*/);
            Fun2ModuleChange();
            //LCD_SetColor(0xFFFF);
            BrowserDirInit();
            DisplayFlag = DISP_BROWSER;
            PostFun2Message(MSG_FUNCTION2_BROWSER, BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
        }
    }
    
    if (DisplayFlag == DISP_TEXT) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            if (Fun2MsgArray[1] == MSG_FUNCTION2_BOOKMASK_SELECT) {
                TxtBookMaskSelect();
            } else {
                TextKeyProc();
            }
        } else {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 0;
            bitset_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart1 = 1;
            LCD_SDRAM_Standby_Return();
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
            
        if (SysBatteryFlag.bc.bBatteryDisplay) {
            SysBatteryFlag.bc.bBatteryDisplay = 0;
            DisplayPicture(TEXT_BATT_PIC_X, TEXT_BATT_PIC_Y, TEXT_BATT_PIC_XSIZE, TEXT_BATT_PIC_YSIZE, (NFA_TEXT_BATT_PIC + (unsigned long)(BatteryLevel) * TEXT_BATT_PIC_SIZE));
        }
        
        if (SystemInfo.bc.bDisplayRecovery) {
            SystemInfo.bc.bDisplayRecovery = 0;
            PostFun2Message(MSG_FUNCTION2_TXT, MSG_FUNCTION2_TXT_PLAY,0,0,0/*,0,0,0*/);
            PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_INIT,(unsigned int) (&Text), 0, 0, 0, 0, 0);
        }
        
        DISP_Txt();
        
        if (SystemInfo.bc.bBackLightOnStart1) {
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
__attribute__((section(".txt_text,\"ax\"")))
void TextVariableInit(void)
{
    if (Sys2InitFlag.bc.bTxtSysInitEnable){
        Sys2InitFlag.word = 0xffff;
        Sys2InitFlag.bc.bTxtSysInitEnable = 0;
        Sys2InitFlag.bc.bTxtModuleInitEnable = 0;
    }
    
    TxtFileFormatError = 0;
    TxtPrevScrollY = TEXT_SCROLL_PIC_Y;
    
    TextAutoPlayFlag = 0;
    TxtAutoBrowserTimeFlag = 0;
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void Txt_Byte2DWord(unsigned char *pChar, unsigned long *pLong, unsigned int long_len)
{
    unsigned int i;
    
    for(i = 0; i < long_len; i++){
        *pChar++ = (unsigned char)((*pLong>>24)&0x00FF);
        *pChar++ = (unsigned char)((*pLong>>16)&0x00FF);
        *pChar++ = (unsigned char)((*pLong>>8 )&0x00FF);
        *pChar++ = (unsigned char)((*pLong++  )&0x00FF); 
    }    
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void Txt_DWord2Byte(unsigned long *pLong, unsigned char *pChar, unsigned int long_len)
{
    unsigned int i;
    
    for(i = 0; i < long_len; i++){
        *pLong = ((*pChar++) & 0x00FF);
        *pLong = ((*pLong << 8)|((*pChar++) & 0x00FF));
        *pLong = ((*pLong << 8)|((*pChar++) & 0x00FF));
        *pLong = ((*pLong << 8)|((*pChar++) & 0x00FF));
        pLong++; 
    }
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void SaveTextInformation(void)
{
    unsigned char TempBuf[360];
    unsigned char *pTempBuf;
    unsigned char *pTxtFileInfo;
    long          *pTxtOffsetBuf;
    unsigned int i,j;
    unsigned long SourceAddr;
    
    for(i=0; i < TXT_BOOKMARK_COUNTER; i++){
        if (Text.NFBookMark[i] != -1) break;
    }
    
    if (i == TXT_BOOKMARK_COUNTER) return;
    
//    GetResourceAddress(&ImageLogicAddress, &MenuLogicAddress,
//                       &FontLogicAddress,  &GBKLogicAddress,
//                       &SysInfomationAddress );  
                       
#if(FLASHROM_TYPE == _MLC_)
    SourceAddr = SysInfomationAddress + (unsigned long)(256) * 1024;
#else
    SourceAddr = SysInfomationAddress + (unsigned long)(128) * 1024;
#endif
    FLASH_PROTECT_OFF();
    FlashBlockErase(SourceAddr >> 9);
    FLASH_PROTECT_ON();
    
//------------------------------------------------------------------------------
    pTxtOffsetBuf = &TxtOffsetBuf[0];
    for(j=0; j<4; j++){
        pTempBuf = &TempBuf[0];
        Txt_Byte2DWord(pTempBuf, pTxtOffsetBuf, TXT_OFFSET_BUF_MAX/4);
        pTxtOffsetBuf += TXT_OFFSET_BUF_MAX/4;
//        FLASH_PROTECT_OFF();
        FlashPageProgLogic(SourceAddr >> 9, 0, TempBuf, 512);
//        FLASH_PROTECT_ON();
        SourceAddr += 2048;
    }
    
//------------------------------------------------------------------------------
    pTempBuf = &TempBuf[0]; 
    pTxtOffsetBuf = &Text.NFBookMark[0];
    
    Txt_Byte2DWord(pTempBuf, pTxtOffsetBuf, TXT_BOOKMARK_COUNTER);

//        FLASH_PROTECT_OFF();
        FlashPageProgLogic(SourceAddr >> 9, 0, TempBuf, 512);
//        FLASH_PROTECT_ON();
        SourceAddr += 2048;
    
//------------------------------------------------------------------------------
    pTempBuf = &TempBuf[0]; 
    pTxtFileInfo = &Text.FilePath[0];
    
    for(i=0; i<MAX_PATH_NAME; i++){
        *pTempBuf++ = (unsigned char)((*pTxtFileInfo>>8 )&0x00FF);
        *pTempBuf++ = (unsigned char)((*pTxtFileInfo++  )&0x00FF); 
    }
    
//        FLASH_PROTECT_OFF();
        FlashPageProgLogic(SourceAddr >> 9, 0, TempBuf, 512);
//        FLASH_PROTECT_ON();
        SourceAddr += 2048;
    
//------------------------------------------------------------------------------
    pTempBuf      = &TempBuf[0]; 
    pTxtFileInfo = &Text.FileName[0];
    
    for(i=0; i<MAX_FILENAME_LEN; i++){
        *pTempBuf++ = (unsigned char)((*pTxtFileInfo>>8 )&0x00FF);
        *pTempBuf++ = (unsigned char)((*pTxtFileInfo++  )&0x00FF); 
    }

    *pTempBuf++ = (unsigned char)((Text.FileInMemType>>8)&0x00FF);
    *pTempBuf++ = (unsigned char)((Text.FileInMemType)&0x00FF);
    
    *pTempBuf++ = (unsigned char)((Text.FileSize>>24)&0x00FF);
    *pTempBuf++ = (unsigned char)((Text.FileSize>>16)&0x00FF);
    *pTempBuf++ = (unsigned char)((Text.FileSize>>8 )&0x00FF);
    *pTempBuf++ = (unsigned char)((Text.FileSize    )&0x00FF);

//        FLASH_PROTECT_OFF();
        FlashPageProgLogic(SourceAddr >> 9, 0, TempBuf, 512);
//        FLASH_PROTECT_ON();
//------------------------------------------------------------------------------
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void LoadTextInformation(void)
{
    unsigned long SourceAddr;// = 0x3a0000;
    unsigned char TempBuf[TXT_OFFSET_BUF_MAX];
    unsigned char *pTempBuf;
    unsigned char *pTxtFielInfo;
    long          *pTxtOffsetBuf;
    unsigned int i,j;
    
    
//    GetResourceAddress(&ImageLogicAddress, &MenuLogicAddress,
//                       &FontLogicAddress,  &GBKLogicAddress,
//                       &SysInfomationAddress );  
                       
#if(FLASHROM_TYPE == _MLC_)
    SourceAddr = SysInfomationAddress + (unsigned long)(256) * 1024;
#else
    SourceAddr = SysInfomationAddress + (unsigned long)(128) * 1024;
#endif

    pTxtOffsetBuf = &TxtOffsetBuf[0];
    
    for(j=0; j<4; j++){   
        FlashReadLogic(SourceAddr, TempBuf, TXT_OFFSET_BUF_MAX);
        pTempBuf = &TempBuf[0];
        
        Txt_DWord2Byte(pTxtOffsetBuf,pTempBuf,TXT_OFFSET_BUF_MAX/4);
        SourceAddr += 2048;
    }
    
//------------------------------------------------------------------------------
    FlashReadLogic(SourceAddr, TempBuf, TXT_BOOKMARK_COUNTER*4);
    pTempBuf = &TempBuf[0]; 
    pTxtOffsetBuf = &Text.NFBookMark[0];
    
    Txt_DWord2Byte(pTxtOffsetBuf,pTempBuf,TXT_BOOKMARK_COUNTER);
    
    SourceAddr += 2048;
    
//------------------------------------------------------------------------------
    FlashReadLogic(SourceAddr, TempBuf, MAX_PATH_NAME*2);
    pTempBuf = &TempBuf[0]; 
    pTxtFielInfo = &Text.FilePath[0];
    
    for(i=0; i<MAX_PATH_NAME; i++){
        *pTxtFielInfo = ((*pTempBuf++) & 0x00FF);
        *pTxtFielInfo = ((*pTxtFielInfo << 8)|((*pTempBuf++) & 0x00FF));
        pTxtFielInfo++;
    }
    SourceAddr += 2048;
    
//------------------------------------------------------------------------------
    FlashReadLogic(SourceAddr, TempBuf, MAX_FILENAME_LEN*2+6);
    pTempBuf      = &TempBuf[0]; 
    pTxtFielInfo  = &Text.FileName[0];
    
    for(i=0; i<MAX_FILENAME_LEN; i++){
        *pTxtFielInfo = ((*pTempBuf++) & 0x00FF);
        *pTxtFielInfo = ((*pTxtFielInfo << 8)|((*pTempBuf++) & 0x00FF));
        pTxtFielInfo++;
    }

    Text.FileInMemType = (*pTempBuf++ << 8)&0xff00;
    Text.FileInMemType |= (*pTempBuf++) & 0x00FF;
    
    Text.FileSize = ((*pTempBuf++) & 0x00FF);
    Text.FileSize = ((Text.FileSize << 8)|((*pTempBuf++) & 0x00FF));
    Text.FileSize = ((Text.FileSize << 8)|((*pTempBuf++) & 0x00FF));
    Text.FileSize = ((Text.FileSize << 8)|((*pTempBuf++) & 0x00FF));
//------------------------------------------------------------------------------
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TextInit(void)
{
    int i;
    
    for(i = TXT_DISP_LINE_MAX - 2; i >= 0 ; i--)
        TxtBuf[i].pNext = &TxtBuf[i+1];
        
    for(i = TXT_DISP_LINE_MAX - 1; i >= 1; i--)
        TxtBuf[i].pPerv  = &TxtBuf[i-1];
        
    for(i = TXT_DISP_LINE_MAX - 1; i >= 0; i--){
        TxtBuf[i].BytesPerLine = 0;
        TxtBuf[i].TxtDispBuf[0] = 0;
    }
//============================================================================//
    Text.BookMarkCont = 0;
//============================================================================//
    TxtBuf[TXT_DISP_LINE_MAX-1].pNext = TxtBuf;
    TxtBuf[0].pPerv = &TxtBuf[TXT_DISP_LINE_MAX-1];
    Text.BookMarkType    = 0;
    Text.pTxtBuf         = TxtBuf;
    Text.StartFileOffset = 0;
    Text.EndFileOffset   = 0;
    Text.FileCtr         = 0;
//============================================================================//            
    for(i = TXT_OFFSET_BUF_MAX - 1; i >= 0; i--){
        TxtOffsetBuf[i] = 0;
    }
    
    TxtOffsetCont    = 0;
    TxtBypePerOffset = 0;
//============================================================================//
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
unsigned int TextGetBytes(unsigned int *s, unsigned int chars)
{
    unsigned int LinePerBypes=0; 
    int          i; 
    
    while(*s && chars>0)
    {
        i = LCD_GetCharXSize(*s++);
  
        if(i==CH_CHAR_XSIZE)
            LinePerBypes += 2;
        else if(i==AS_CHAR_XSIZE)
            LinePerBypes += 1;
        chars--;
    }
    
    return(LinePerBypes);
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TextFileOpen(void)
{
    unsigned int i,j;
    int             ReadBytes;
    int             CharsPerLine;
    int             OffsetForFile;
    unsigned int    OpenFileBuf[TXT_PER_LINE_CHARS];
    unsigned int    *pOpenFileBuf = OpenFileBuf;
    TxtDispBufStruct *pTxt = TxtBuf;
    LCD_RECT        r;

    r.x0 = TEXT_AREABK_PIC_X;
    r.y0 = TEXT_AREABK_PIC_Y;
    r.x1 = TEXT_AREABK_PIC_X + TEXT_AREABK_PIC_XSIZE - 1;
    r.y1 = TEXT_AREABK_PIC_Y + TEXT_AREABK_PIC_YSIZE - 1;
    
    if ((hSlaveFile = FileOpen(SysFile2Info.Path, SysFile2Info.Fdt.Name, "R")) == NOT_OPEN_FILE) {
        
        TxtFileFormatError = 1;
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        PreDisplayFlag = DISP_TEXT;
        DisplayRecoveryTime = 0;
        return;
    }
//------------------------------------------------------------------------------    
    TxtBypePerOffset = (SysFile2Info.Fdt.FileSize+TXT_OFFSET_BUF_MAX)/TXT_OFFSET_BUF_MAX;
    if(TxtBypePerOffset < TXT_PER_LINE_CHARS){
        TxtBypePerOffset = TXT_PER_LINE_CHARS;
    }
//------------------------------------------------------------------------------    
    GetLongFileName(SysFile2Info.LongFileName);
    
//============================================================================//
//      当打开的文件与上次保存的文件不是同一个时重新对相关的变量始化          //
//============================================================================//
    j = 0;  
    for(i=0; i<33; i++){
        if(Text.FileName[i] != SysFile2Info.LongFileName[i])
            j++;
        Text.FileName[i] = SysFile2Info.LongFileName[i];
	if((Text.FileName[i]==0)&&(SysFile2Info.LongFileName==0))
		break;
    }
    for(i=0; i<MAX_PATH_NAME; i++){
        if(Text.FilePath[i] != SysFile2Info.Path[i])
            j++;
        Text.FilePath[i] = SysFile2Info.Path[i];
        if((SysFile2Info.Path[i] == 0)&&(Text.FilePath[i] == 0))break;//解决对书签问题 2006-7-6 17:42
    }
    if(Text.FileSize != SysFile2Info.Fdt.FileSize){
        Text.FileSize = SysFile2Info.Fdt.FileSize;
        j++;
    }
    
    
    if(j != 0){
        for(i=0; i<TXT_BOOKMARK_COUNTER; i++){
            Text.NFBookMark[i] = -1;
        }
        for(i=0; i<TXT_OFFSET_BUF_MAX; i++){
            TxtOffsetBuf[i] = 0;
        }
    }
//============================================================================//
    
    for(j=0; j < TXT_DISP_LINE_MAX; j++)
        TxtBuf[j].BytesPerLine = 0;
    
    j = 0;
    while(j<TXT_DISP_LINE_MAX)
    {
        FileSeek(Text.EndFileOffset, SEEK_SET, hSlaveFile);
        ReadBytes = FileRead((unsigned char *)OpenFileBuf, TXT_PER_LINE_CHARS-2, hSlaveFile);
        OpenFileBuf[ReadBytes] = 0;
        
        TxtGbk2Unicode(OpenFileBuf, OpenFileBuf, ReadBytes);
        CharsPerLine = LCD_GetCharsPerLine(&r, OpenFileBuf);
        OffsetForFile = TextGetBytes(OpenFileBuf, CharsPerLine);
        
        if(*(pOpenFileBuf + CharsPerLine) == '\n'){
            Text.EndFileOffset++;
            pTxt->BytesPerLine++;
            pTxt = pTxt->pNext;
            j++;
        } else {
            for(i=0; i<CharsPerLine; i++)
                pTxt->TxtDispBuf[i] = OpenFileBuf[i];
            pTxt->TxtDispBuf[i] = 0;
            
            pTxt->BytesPerLine += OffsetForFile;
            Text.EndFileOffset += OffsetForFile;
           
            if(*(pOpenFileBuf+CharsPerLine) == '\r'){
                Text.EndFileOffset++;
                pTxt->BytesPerLine++;
                continue;
            }
            pTxt = pTxt->pNext;
            j++;
        }
    }
    PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_INIT,(unsigned int) (&Text), 0, 0, 0, 0, 0);
}

/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : TxtSchedule.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/

__attribute__((section(".txt_text,\"ax\"")))
void TextKeyProc(void)
{
    unsigned int key;
    unsigned int state;
    unsigned int i,j;
    unsigned int ReadBytes;
    unsigned int CharsPerLine;
    long           OffsetForFile;
    TxtDispBufStruct *pTxt;
    LCD_RECT     r;
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) return;
    
    if (TxtFileFormatError == 1) {
        SystemInfo.bc.bDisplayRecovery = 0;
        DisplayRecoveryTime = 120;//300;
        
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        BrowserDirInit();
        DisplayFlag = DISP_BROWSER;
        PostFun2Message(MSG_FUNCTION2_BROWSER, BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
    }
    
    switch (KeyVal) {
//--------------------- PLAY KEY ---------------------------
    /*case KEY_VAL_PLAY_DOWN:
    case KEY_VAL_PLAY_PRESS:
    case KEY_VAL_PLAY_PRESS_START:
    case KEY_VAL_PLAY_LONG_UP:*/
    case KEY_VAL_PLAY_SHORT_UP:         //Play Pause
    {
        TextAutoPlayFlag = 1 - TextAutoPlayFlag;
        TextAutoPlaySetup();
        break;
    }    
//--------------------- REC KEY ----------------------------
    /*case KEY_VAL_REC_DOWN:
    case KEY_VAL_REC_PRESS:
    case KEY_VAL_REC_LONG_UP:*/
    case KEY_VAL_REC_PRESS_START:       //Save BookMark
    {
        Text.BookMarkCont = 0;
        Text.BookMarkType = TXT_BOOKMARK_TYPE_SAVE;
        TxtBookMaskSelectDisp(TXT_BOOKMARK_TYPE_SAVE, 0, 0);
        PostFun2Message(MSG_FUNCTION2_TXT, MSG_FUNCTION2_BOOKMASK_SELECT,0,0,0/*,0,0,0*/);
        break;
    }
    
    case KEY_VAL_REC_SHORT_UP:          //Distill BookMark
    {
        Text.BookMarkCont = 0;
        Text.BookMarkType = TXT_BOOKMARK_TYPE_DISTILL;
        TxtBookMaskSelectDisp(TXT_BOOKMARK_TYPE_DISTILL, 0, 0);
        PostFun2Message(MSG_FUNCTION2_TXT, MSG_FUNCTION2_BOOKMASK_SELECT,0,0,0/*,0,0,0*/);
        break;
    }
//--------------------- MENU KEY ---------------------------
    /*case KEY_VAL_MENU_DOWN:
    case KEY_VAL_MENU_PRESS:
    case KEY_VAL_MENU_LONG_UP:*/
    case KEY_VAL_MENU_PRESS_START:      //Return Main Menu
    {
        TimerStop(TEXT_TMR_CHANNEL);
        
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        MainMenuState = 0;
        DisplayFlag = DISP_MENU;
        break;
    }    
    
    case KEY_VAL_MENU_SHORT_UP:         //Return Browser
    {
        TimerStop(TEXT_TMR_CHANNEL);
        
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        
        BrowserDirInit();
        
        DisplayFlag = DISP_BROWSER;
        PostFun2Message(MSG_FUNCTION2_BROWSER, BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
        break;
    }
    /*case KEY_VAL_UP_PRESS_START:
    case KEY_VAL_UP_LONG_UP:
    case KEY_VAL_UP_SHORT_UP:*/
//--------------------- UP KEY ----------------------------
    case KEY_VAL_UP_DOWN:
    case KEY_VAL_UP_PRESS:
    {
        if(Text.StartFileOffset > 0) {
            TxtKeyUpProc();        
            PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_TEXT,(unsigned int) (&Text), 0, 0, 0, 0, 0);
        }
        break;
    }
//------------------- DOWN KEY ----------------------------
    /*case KEY_VAL_DOWN_PRESS_START:
    case KEY_VAL_DOWN_SHORT_UP:
    case KEY_VAL_DOWN_LONG_UP:*/
    case KEY_VAL_DOWN_DOWN:
    case KEY_VAL_DOWN_PRESS:
    {
        if(Text.EndFileOffset < SysFile2Info.Fdt.FileSize) {
            TxtKeyDownProc();
            PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_TEXT,(unsigned int) (&Text), 0, 0, 0, 0, 0);
        }
        break;
    }
//-------------------- FFD KEY ----------------------------
    /*case KEY_VAL_FFD_PRESS_START:
    case KEY_VAL_FFD_SHORT_UP:
    case KEY_VAL_FFD_LONG_UP:
        break;*/
    case KEY_VAL_FFD_DOWN:
    case KEY_VAL_FFD_PRESS:
    {
        if(Text.EndFileOffset < SysFile2Info.Fdt.FileSize) {
            for(i=0; i<TXT_DISP_LINE_MAX; i++)
                TxtKeyDownProc();
            PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_TEXT,(unsigned int) (&Text), 0, 0, 0, 0, 0);
        }
        break;
    }
//-------------------- FFD KEY ----------------------------
    /*case KEY_VAL_FFW_PRESS_START:
    case KEY_VAL_FFW_SHORT_UP:
    case KEY_VAL_FFW_LONG_UP:*/
    case KEY_VAL_FFW_DOWN:
    case KEY_VAL_FFW_PRESS:
    {
        if(Text.StartFileOffset > 0) {
            for(i=0; i<TXT_DISP_LINE_MAX; i++)
                TxtKeyUpProc();        
            PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_TEXT,(unsigned int) (&Text), 0, 0, 0, 0, 0);
        }
        break;           
    }
    default:
        break;
    }
    
    KeyFlag.bc.bKeyProssed = 0;
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TxtKeyDownProc(void)
{
    TxtDispBufStruct *pTxt;
    LCD_RECT             r;
    unsigned int TxtLineBuf[TXT_PER_LINE_CHARS];
    unsigned int CharsPerLine;
    unsigned int OffsetForFile;
    int             ReadBytes;
    long            StartOffset;
    unsigned int BytesPerLineBck;
    unsigned int key;
    unsigned int i;
    unsigned int *pTxtLineBuf = TxtLineBuf;


    pTxt = Text.pTxtBuf;
    
    r.x0 = TEXT_AREABK_PIC_X;
    r.y0 = TEXT_AREABK_PIC_Y;
    r.x1 = TEXT_AREABK_PIC_X + TEXT_AREABK_PIC_XSIZE - 1;
    r.y1 = TEXT_AREABK_PIC_Y + TEXT_AREABK_PIC_YSIZE - 1;
    
    if(Text.EndFileOffset >= SysFile2Info.Fdt.FileSize)
        return;
    
    BytesPerLineBck = pTxt->BytesPerLine;
    pTxt->BytesPerLine = 0;
    Text.StartFileOffset += BytesPerLineBck;
    do
    {
        FileSeek(Text.EndFileOffset, SEEK_SET, hSlaveFile);
        ReadBytes = FileRead((unsigned char *)TxtLineBuf, TXT_PER_LINE_CHARS-2, hSlaveFile);
        if(ReadBytes == 0)
            break;
    
        TxtLineBuf[ReadBytes] = 0;
        TxtGbk2Unicode(TxtLineBuf, TxtLineBuf, ReadBytes);
        CharsPerLine  = LCD_GetCharsPerLine(&r, TxtLineBuf);
        OffsetForFile = TextGetBytes(TxtLineBuf, CharsPerLine);
        Text.EndFileOffset    += OffsetForFile;
        if(*(pTxtLineBuf+CharsPerLine) == '\n')
        {
            Text.EndFileOffset++;
            pTxt->BytesPerLine++;
            break;
        }
        else
        {
            for(i=0; i<CharsPerLine; i++)
                pTxt->TxtDispBuf[i] = TxtLineBuf[i];
            pTxt->TxtDispBuf[i] = 0;
            pTxt->BytesPerLine    += OffsetForFile;
            if(*(pTxtLineBuf+CharsPerLine) == '\r')
            {
                Text.EndFileOffset++;
                pTxt->BytesPerLine++;
                continue;
            }
            break;
        }
        
    }while(1);
//------------------------------------------------------------------------------
    i = (unsigned int)((unsigned long)(Text.StartFileOffset)/TxtBypePerOffset);
    TxtOffsetBuf[i] = Text.StartFileOffset;
//------------------------------------------------------------------------------  
    Text.pTxtBuf = pTxt->pNext;    
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TxtKeyUpProc(void)
{       
    unsigned int TxtLineBuf[512];
    long           StartOffset;
    int             ReadBytes;
    unsigned int CharsPerLine;
    unsigned int OffsetForFile;
    unsigned int *pTxtLineBuf = TxtLineBuf;
    TxtDispBufStruct *pTxt;
    LCD_RECT             r;
    unsigned int i;

    if(Text.StartFileOffset <= 0) return;
        
    pTxt = Text.pTxtBuf;
    r.x0 = TEXT_AREABK_PIC_X;
    r.y0 = TEXT_AREABK_PIC_Y;
    r.x1 = TEXT_AREABK_PIC_X + TEXT_AREABK_PIC_XSIZE - 1;
    r.y1 = TEXT_AREABK_PIC_Y + TEXT_AREABK_PIC_YSIZE - 1;
    
//------------------------------------------------------------------------------
    i = Text.StartFileOffset/TxtBypePerOffset;
    if(i > 0){
        StartOffset = TxtOffsetBuf[i-1];
    }else{
        StartOffset = 0;
    }
//------------------------------------------------------------------------------    
    
    pTxt = pTxt->pPerv;
    Text.EndFileOffset  -= pTxt->BytesPerLine;
    pTxt->BytesPerLine  = 0;
    
    while(StartOffset < Text.StartFileOffset)
    {
        FileSeek(StartOffset, SEEK_SET, hSlaveFile);
        ReadBytes = FileRead((unsigned char *)TxtLineBuf, 500, hSlaveFile);
        TxtLineBuf[ReadBytes] = 0;
        if (ReadBytes == 0) return; //解决文本在很多回车换行的显示问题
        TxtGbk2Unicode(TxtLineBuf, TxtLineBuf, ReadBytes);
        pTxtLineBuf = TxtLineBuf;
        pTxt->BytesPerLine  = 0;
        while(ReadBytes > 0)//while(1)
        {
            CharsPerLine  = LCD_GetCharsPerLine(&r, pTxtLineBuf);
            OffsetForFile = TextGetBytes(pTxtLineBuf, CharsPerLine);
            
            ReadBytes -= OffsetForFile;
            if(ReadBytes <= 4)
                break;
            
            StartOffset += OffsetForFile;
            if((*(pTxtLineBuf + CharsPerLine) == '\n') )
            {
                    StartOffset++;
                    pTxt->BytesPerLine++;
                    pTxtLineBuf++;
                    if(--ReadBytes == 0)//if(--ReadBytes <= 20)
                        break;
                    if(StartOffset >= Text.StartFileOffset)
                        break;
                    else
                    {
                        pTxt->BytesPerLine = 0;
                        continue;
                    }
            }
            else
            {
                for(i=0; i<CharsPerLine; i++)
                    pTxt->TxtDispBuf[i] = *pTxtLineBuf++;//TxtLineBuf[i];
                pTxt->TxtDispBuf[i] = 0;
                pTxt->BytesPerLine += OffsetForFile;
                if(*pTxtLineBuf  == '\r')
                {
                    StartOffset++;
                    pTxt->BytesPerLine++;
                    pTxtLineBuf++;
                    ReadBytes--;
                    if(StartOffset >= Text.StartFileOffset)
                        break;
                    else
                        continue;
                }
                if(StartOffset >= Text.StartFileOffset)
                    break;
                pTxt->BytesPerLine = 0;
            }
        }
    }
    Text.pTxtBuf = pTxt;
    Text.StartFileOffset -= pTxt->BytesPerLine;
}

/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".txt_text,\"ax\"")))
void TxtBookMaskSelect(void)
{
    unsigned int i;
    unsigned int Mask_tmp;
    
    if(KeyFlag.bc.bKeyProssed == 0) return;
    
    if (SystemInfo.bc.bHoldOn) {
        KeyFlag.bc.bKeyProssed = 0;
        return;
    }
    
    Mask_tmp = Text.BookMarkCont;
    
    switch(KeyVal){
//-------------------- MENU KEY ----------------------------
    case KEY_VAL_MENU_SHORT_UP:
    {
        if(Text.BookMarkType == TXT_BOOKMARK_TYPE_DISTILL){
            if(Text.NFBookMark[Text.BookMarkCont] != -1){
                Text.StartFileOffset = Text.NFBookMark[Text.BookMarkCont];
                Text.EndFileOffset = Text.StartFileOffset;
//------------------------------------------------------------------------------
//  2006-11-2 修改：当文本内容小于一屏时读取书签后显示出错!
//------------------------------------------------------------------------------                
                Text.pTxtBuf         = TxtBuf;
//------------------------------------------------------------------------------                 
                for(i=0; i<TXT_DISP_LINE_MAX; i++) 
                {
                    Text.pTxtBuf->BytesPerLine = 0;
                    TxtKeyDownProc();
                }
//------------------------------------------------------------------------------
//  2006-10-27 修改：当文本内容小于一屏时读取书签后显示出错!
//------------------------------------------------------------------------------                
                Text.pTxtBuf         = TxtBuf;
//------------------------------------------------------------------------------ 
            }
        }else{
            if(Text.NFBookMark[Text.BookMarkCont] != Text.StartFileOffset) {
                Text.NFBookMark[Text.BookMarkCont] = Text.StartFileOffset;
                SaveTextInformation();
            }
        }
        PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_INIT/*MSG_DISP_TXT_TEXT*/,(unsigned int) (&Text), 0, 0, 0, 0, 0);
        PostFun2Message(MSG_FUNCTION2_TXT, MSG_FUNCTION2_TXT_PLAY,0,0,0/*,0,0,0*/);
        break;
    }
    case KEY_VAL_MENU_PRESS_START:
    {
        FileClose(hSlaveFile);
        Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
        Fun2ModuleChange();
        MainMenuState = 0;
        DisplayFlag    = DISP_MENU;
        PreDisplayFlag = DISP_MENU;
        break;
    }
//-------------------- UP KEY ------------------------------
    case KEY_VAL_UP_SHORT_UP:
    {
        if(Text.BookMarkCont > 0){
            Text.BookMarkCont--;
        }else{
            Text.BookMarkCont = TXT_BOOKMARK_COUNTER-1;
        }
        break;
    }
//-------------------- DOWN KEY ----------------------------
    case KEY_VAL_DOWN_SHORT_UP:
    {
        if(Text.BookMarkCont < TXT_BOOKMARK_COUNTER-1){
            Text.BookMarkCont++;
        }else{
            Text.BookMarkCont = 0;
        }
        break;
    }
//-------------------- FFW KEY ----------------------------
    case KEY_VAL_FFW_SHORT_UP:
    {
        PostDisplayMessage(MSG_DISP_TXT, MSG_DISP_TXT_INIT/*MSG_DISP_TXT_TEXT*/,(unsigned int) (&Text), 0, 0, 0, 0, 0);
        PostFun2Message(MSG_FUNCTION2_TXT, MSG_FUNCTION2_TXT_PLAY,0,0,0/*,0,0,0*/);
        break;
    }
    default:
        break;
    }
    if(Text.BookMarkCont != Mask_tmp){
        TxtBookMaskSelectDisp(Text.BookMarkType, 1, Text.BookMarkCont);
    }
    
    KeyFlag.bc.bKeyProssed = 0;
}
#endif
//******************************************************************************
