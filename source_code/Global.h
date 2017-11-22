//******************************************************************************
//  Global.inc          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#ifndef  _GLOBAL_H
#define  _GLOBAL_H

#include "Struct.h"
//#include "Config.h"

#ifdef _IN_MAIN
#undef  EXT
#define EXT
#else
#define EXT extern
#endif

//------------------------------------------------------------------------------
EXT unsigned int        BroActFlag;
EXT unsigned int        SystemDebugFlag;
//------------------------------------------------------------------------------
EXT unsigned int        TestCount;
EXT unsigned int        TestCount1;
//------------------------------------------------------------------------------
//���籣�������
EXT unsigned int OutputVolume;      //�������
EXT unsigned int LineInputVolume;   //LineIn��������
EXT unsigned int MicInputVolume;    //Mic   ��������

EXT unsigned int LightOffTime;      //����ر�ʱ��
EXT unsigned int PowerOffTime;      //�ػ�ʱ��

EXT unsigned int LigthLevel;        //���ȵ���
EXT unsigned int ContrastLevel;     //�Աȶȵ���
EXT unsigned int DisplayMode;
EXT unsigned int ScrollSpeed;       //�����ٶ�
EXT unsigned int PicBrowserTime;

EXT unsigned int Language;
EXT unsigned int MemorySelect;
EXT unsigned int PrewMemorySelect;
EXT unsigned int Id3Switch;    
EXT unsigned int PlaySpeed;    

//------------------------------------------------------------------------------
//  ����ı���ͼƬ�Զ������������ı���
//------------------------------------------------------------------------------
EXT unsigned int gTxtAutoBrowserTime;
EXT unsigned int gJpegAutoBrowserTime;
           
EXT unsigned int DisplayRecoveryTime; //��Ļ��ʾ�ָ�ʱ��
EXT unsigned int LightOffCounter;
EXT unsigned int SysIdleCounter;
EXT unsigned long PowerOffCounter;
EXT unsigned int  FileFragCleanFlag;
EXT unsigned int  EarInStatus;

EXT unsigned int Timer0Val;
EXT unsigned int PrevTimer0Val;
//------------------------------------------------------------------------------
EXT unsigned long SysFreeMemeryFlash;
EXT unsigned long SysFreeMemeryCard;
EXT unsigned long LanguageResoureOffset;    // ������Դƫ����
EXT unsigned int CurrMusicTypePlaying;
EXT unsigned int MusicDisplayInterface;
EXT int          PrevFindMusicDir; //�����ļ��ķ��� ���ڽ������ļ������ʱ��ʹ��
EXT int          SysTimeCounter0;
EXT unsigned int CurrCpuFreq;
EXT unsigned int PrevCpuFreq;
EXT unsigned int CurrApbFreq;
EXT unsigned int CodecPowerDownFlag;
EXT unsigned int LcdLightOffTimeCounter;
EXT unsigned int I2sClockCon;

EXT unsigned int PreLigthLevel;        // ��һ�ε����ȵ���ֵ
EXT unsigned int Sche1MsgArray[1];//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned int Sche2MsgArray[1];//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned int Fun1MsgArray[5];//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned int Fun2MsgArray[5];//__attribute__((section(".sys_bss, \"aw\"")));
//EXT unsigned int KeyProcMsgArray[8];//__attribute__((section(".sys_bss, \"aw\"")));
//EXT unsigned int KeyMsgArray[8];//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned int DispMsgArray[8];//__attribute__((section(".sys_bss, \"aw\"")));
//EXT unsigned int USBDetMsgArray[8];//__attribute__((section(".sys_bss, \"aw\"")));

EXT unsigned int PreviousScheduler1Msg;//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned int PreviousScheduler2Msg;//__attribute__((section(".sys_bss, \"aw\"")));
//------------------------------------------------------------------------------
//File System Variable
//unsigned int    RandomCount;//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned int    GlobalRanDomNum;
EXT unsigned int    File1RandomBuffer[16];//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned int    File2RandomBuffer[16];//__attribute__((section(".sys_bss, \"aw\"")));

EXT char            hMainFile;//__attribute__((section(".sys_bss, \"aw\"")));
EXT char            hSlaveFile;//__attribute__((section(".sys_bss, \"aw\"")));
//EXT unsigned char   MainLongFileName[MAX_FILENAME_LEN];//__attribute__((section(".sys_bss, \"aw\"")));
//EXT unsigned char   SlaveLongFileName[MAX_FILENAME_LEN];

EXT SYS_FILE_INFO   SysFile1Info;//__attribute__((section(".sys_bss, \"aw\"")));
EXT SYS_FILE_INFO   SysFile2Info;//__attribute__((section(".sys_bss, \"aw\"")));
EXT SYSTEMINFO      SystemInfo;

EXT unsigned int    DisplayFlag;
EXT unsigned int    PreDisplayFlag;
EXT SYS1_INIT_FLAG  Sys1InitFlag;//__attribute__((section(".sys_bss, \"aw\"")));
EXT SYS2_INIT_FLAG  Sys2InitFlag;//__attribute__((section(".sys_bss, \"aw\"")));
EXT SYS_BATT_FLAG   SysBatteryFlag;
EXT SYS_USB_FLAG    SysUsbFlag;
EXT SYS_PERIPH_FLAG PeriphFlag;
//------------------------------------------------------------------------------
EXT GLOBAL_MUSIC_VAR            GlobalMusicVar;//���ֲ���ģ��������Ϣ
EXT GLOBAL_ENC_VAR              GlobalEncVar;  //¼��ģ��������Ϣ

EXT MUSIC_PLAY_INFO             MusicPlayInfo;//__attribute__((section(".sys_bss, \"aw\"")));
EXT MUSIC_DISP_MESSAGE_STRUCT   MusicDisplayBlock;//__attribute__((section(".sys_bss, \"aw\"")));
EXT unsigned long   MusicDispFlag;
//------------------------------------------------------------------------------
EXT BATTERYFLAG  BatteryFlag;
EXT unsigned int BatteryDetectCounter;
EXT unsigned int BatteryDetectState;
EXT unsigned int UsbTranFlag;
EXT unsigned int BatteryLevel;
EXT unsigned int BatteryDetectCountL;
EXT unsigned int BatteryDetectCountH;

EXT unsigned int AdcSampleCount;
EXT unsigned int AdcData0;
EXT unsigned int AdcData1;
EXT unsigned int AdcData2;
//EXT unsigned int AdcData3;

EXT unsigned int BatteryAdcData;
EXT unsigned int BatteryAdcDataSum;
EXT unsigned int EarAdcData;
EXT unsigned int EarAdcDataSum;
//EXT unsigned int ChargeAdcData;
//EXT unsigned int ChargeAdcDataSum;
//------------------------------------------------------------------------------
EXT unsigned int MenuId;
EXT unsigned int PrevMenuId;
EXT unsigned int MenuDispFlag;
EXT unsigned int MainMenuState;

//------------------------------------------------------------------------------
EXT unsigned int FmTimerCount;
EXT _FM_GLOBAL   GlobalFmVar;
//--------------2006-2-8 14:56-----------------------
extern unsigned char MusicFileExtString[16];
extern unsigned char MovieFileExtString[4];
extern unsigned char JpegFileExtString[7];
extern unsigned char TextFileExtString[4];
extern unsigned int MusicEqTable[12];

//extern unsigned int MusicPlayEqStr[12][8];
//extern unsigned int MusicPlaySampeRateStr[9][10];
EXT unsigned long       ImageLogicAddress;
EXT unsigned long       MenuLogicAddress;
EXT unsigned long       FontLogicAddress;
EXT unsigned long       GBKLogicAddress;
EXT unsigned long       SysInfomationAddress;

EXT unsigned int        Pre_CR1_Value;   //��� P_ADD_CR1
void    (*pRC4)(short * Source, short DataLen);


//------------------------------------------------------------------------------

EXT volatile unsigned int DmaFillImage2LcdFlag;
EXT volatile unsigned int Dma_DispFlag;
EXT unsigned int          DmaTanData2LcdCont;
EXT volatile unsigned int Dma_ReadLcdFlag;

EXT unsigned int        MusicGuageBak;
//------------------------------------------------------------------------------
EXT volatile unsigned int MusicDmaFinishFlag;

EXT SCROLL_SRTING_STRUCT ScrollString;
//------------------------------------------------------------------------------
EXT unsigned int        uiTotalMusicFileNum; // hjl@2006-10-27 17:30 ��������е��ļ�����
                                             // �ñ���Ӧ�ڽ�������ģ��ʱ��ʼ��������ļ�ɾ��
                                             // ��������г�ʼ���������ط���Ӧ������г�ʼ��

EXT FIND_DATA           FindDataPlay; 
EXT FIND_DATA           FindDataFunction2;
//------------------------------------------------------------------------------
#endif
//******************************************************************************
