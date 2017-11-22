//******************************************************************************
//  Struct.h          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#ifndef  _STRUCT_H
#define  _STRUCT_H

#include "Config.h"
#include "Lcd.h"

//------------------------------------------------------------------------------
typedef struct 
{
	unsigned int  PlayedFileNum;
	unsigned int  CurrentFileNum;
	unsigned int  TotalFiles;
	FDT           Fdt;
	unsigned char Path[3 + MAX_DIR_DEPTH * 12 + 1];
	unsigned char LongPath[3 + /*MAX_DIR_DEPTH * */MAX_FILENAME_LEN + 1];
	unsigned char LongFileName[MAX_FILENAME_LEN];
}SYS_FILE_INFO;

typedef struct{
    short   MODE;
    short   Usr_Def_Tab[5];
}EQ_Mode;

typedef struct 
{                                   //  FM
	unsigned int  PlayState;        //
	unsigned int  DispMode;         //
	unsigned int  DispState;        //
	unsigned int  FirstBitrate;     //
	unsigned int  Bitrate;          //
	unsigned long Freq;             //
	unsigned int  AB;               //
	unsigned int  Eq;               //
	unsigned int  Mode;             //
	unsigned int  Order;            //
	unsigned int  RepMode;          //
	unsigned int  Range;            //
	unsigned int  TimeGuageVal;     //
	unsigned int  MusicF;
	
}MUSIC_PLAY_INFO;

//typedef struct 
//{
//	unsigned int  Hour;
//	unsigned int  Minute;
//	unsigned int  Second;
//}_TIME;

typedef union {
    struct 
    {
        unsigned int    
            bMusicModuleInitEnable:         1,
            bMp3SysInitEnable:              1,
            bWmaSysInitEnable:              1,
            bWavSysInitEnable:              1,
                                              
            bAviModuleInitEnable:           1,
            bAviSysInitEnable:              1,
                                              
            bEncodeModuleInitEnable:        1,
            bLineInSysInitEnable:           1,
            bMicRecSysInitEnable:           1,
                
            bFmModuleInitEnable:            1,
            bFmSysInitEnable:               1,
            
            bUsbModuleInitEnable:           1,
            
            bEncodeStyleSelect:             1,
                
            bReserved:                      3;
    }bc;
    unsigned int word;
} SYS1_INIT_FLAG;

typedef union {
    struct 
    {
        unsigned int
                bBrowserModuleEnable:       1,
                
                bSetingMenuModuleEnable:    1,
                
                bJpegModuleInitEnable:      1,
                bJpegSysInitEnable:         1,
                
                bLrcModuleInitEnable:       1,
                bLrcSysInitEnable:          1,
                
                bTxtModuleInitEnable:       1,
                bTxtSysInitEnable:          1,
                    
                bGameModuleInitEnable:      1,
                bGameSysInitEnable:         1,
                    
                bReserved:                  6;
    }bc;
    unsigned int word;
} SYS2_INIT_FLAG;

typedef union {
    struct 
    {
        unsigned int    
                BatteryDetectRun:           1,
                
                BatteryChargeRun:           1,
                BatteryChargeEn:            1,
                
                bBatteryDisplay:            1,
                    
                bReserved:                  13;
    }bc;
    unsigned int word;
} SYS_BATT_FLAG;

typedef union {
    struct 
    {
        unsigned int    
                TranMode:                   1,
                UsbTran:                    1,
                    
                bReserved:                  14;
    }bc;
    unsigned int word;
} SYS_USB_FLAG;

typedef union {
    struct 
    {
        unsigned int    
                UsbPlugIn:                  1,
                    
                SdPlugIn:                   1,
                    
                EarPlugIn:                  1,
                EarEjectout:                1,
                    
                bEarStatusCheckEn:          1,
                
                bEarAdcRdy:                 1,
                
                bChargeAdcRdy:              1,
                    
                bLcdStandby:                1,
                
                bReserved:                  8;
    }bc;
    unsigned int word;
} SYS_PERIPH_FLAG;

//------------------------------------------------------------------------------
typedef union {
    struct 
    {
    unsigned long bHold:                    1,
                  bMute:                    1,
                  bVol:                     1,
                  bBatt:                    1,
                  
                  bCurrTime:                1,
                  bTotalTime:               1,
                  bMusicPlay_PlayState:     1,
                  bMusicPlay_Bitrate:       1,
                    
                  bMusicPlay_Freq:          1,
                  bMusicPlay_Eq:            1,
                  bMusicPlay_Mode:          1,
                  bMusicPlay_AB:            1,
                    
                  bMusicPlay_Order:         1,
                  bMusicPlay_TimeGuageVal:  1,
                  bStroboscope:             1,
                  bLongFileName:            1,
                  
                  bId3Info:                 1,
                  bLrc:                     1,
                  bFileNum:                 1,
                  bId3LrcDisplayChange:     1, //当由ID3显示切换为频谱显示 或 频谱显示I切换为D3显示时，置1
                    
                  bInterface:               1,
                  bMusicF:                  1,
                  bReserved:                10;
    }bit;
    unsigned long dword;
} DISPLAY_FLAG;

typedef struct{
    DISPLAY_FLAG    DisplayFlag;
    
    unsigned int    Hold;
    unsigned int    *pMute;
    unsigned int    *pVol;
    unsigned int    *pBatt;
    
//    _TIME           *pCurrTime;
//    _TIME           *pTotalTime;
    
    MUSIC_PLAY_INFO *pMusicPlay;
    
    union {
        struct {
                    short   *pStroboscope;
            unsigned char   *pLongFileName;
        }Music;
        struct {
            unsigned int    *pLrcBuffer;
            unsigned int    *pId3Info; 
        }Lrc;
    }MusicUnion;
    
} MUSIC_DISP_MESSAGE_STRUCT;

typedef struct{
        unsigned int MusicCurFileNum;
        
        unsigned int Mode;
        unsigned int Order;
        unsigned int Eq;
        unsigned int UserEq[5];
}GLOBAL_MUSIC_VAR;

typedef struct{
        unsigned int EncodeMode;
        unsigned int MicQuality;
        unsigned int LineInQuality;
        unsigned int FmRecQuality;
        unsigned int AutoDevideSection;
}GLOBAL_ENC_VAR;
    
//------------------------------------------------------------------------------
typedef union {     //电池电压检测标志位
    struct 
    {
    unsigned int  Rdy:                    1,
                  
                  bReserved:              15;
    }bit;
    unsigned int word;
} BATTERYFLAG;
//------------------------------------------------------------------------------
typedef union {
    struct 
    {
    unsigned int    bTimer0Int:                 1,  //定时器0中断标志
                    
                    bDisplayRecovery:           1,  //显示界面恢复标志位(用于背光重新打开，或弹出指示框结束时恢复显示界面)
                    
                    bHoldOn:                    1,  //Hold状态
                    bHoldDisplay:               1,  //Hold显示标志位
                        
                    bBackLightOnStart:          1,  //背光打开标志位
                    bBackLightOnStart1:         1,  //背光打开标志位
                    bBackLightOffStart:         1,  
                    bBackLightOffEn:            1,  //背光关闭允许位
                    bBackLightStatus:           2,  //背光状态标志位 0开，1关
                        
                    bSysIdleEn:                 1,
                    bSysIdleStatus:             1,
                        
                    bUsbFirstDisable:           1,
                    bPowerOffDisable:           1,
                    bMusicMenuSet:              1,
                    
                    bReversed:                  1;
    }bc;
    unsigned int word;
} SYSTEMINFO;
//------------------------------------------------------------------------------
typedef struct 
{
	unsigned int    FmArea;           //CHINA EUROPE JAPAN USA
	unsigned int    FmStereo;         //0 STEREO; 1 MONO
	
	unsigned int    FmState;          //0 手动搜台模式； 1 自动存台模式
	unsigned int    FmFreq;           //当前电台频率
	
	unsigned int    FmSaveNum;
	unsigned int    FmFreqArray[FREQMAXNUMBLE];
}_FM_GLOBAL;

typedef struct{
    unsigned int  xize;
    unsigned int  ysize;
    unsigned long PicNfa;
}BITMAP_STRUCT;

typedef struct 
{
    unsigned int  flag;
    int           xIntVal;
    int           xIntVal1;
    int           x;
    int           x1;
    LCD_RECT      r;
    
    unsigned int  color;
    unsigned char *pStr;
    
    int           BkPicX;
    int           BkPicY;
    int           BkPicXsize;
    int           BkPicYsize;
    unsigned long BkPicNfa;
    
}SCROLL_SRTING_STRUCT;

typedef struct TIME_STRUCT
{
    unsigned int TimeCounter;
    unsigned int TimeInitValue;
    void         (*TimeFunc)(void *);
    void         *FuncArg;
}TimeStruct;

typedef struct TMR_STRUCT
{
    TimeStruct   TmrTbl[TIMER_MAX_CHANNEL];
    unsigned int TmrFlag;
}TMR;

//------------------------------------------------------------------------------
#endif
//******************************************************************************
