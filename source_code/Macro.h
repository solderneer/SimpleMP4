//******************************************************************************
//  Macro.h          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#ifndef  _MACRO_H
#define  _MACRO_H
//------------------------------------------------------------------------------
//CPU类型选择定义
#define     _RK2608_        0
#define     _RK2608A_       1

//FLASH类型选择
#define     _SLC_           0
#define     _MLC_           1
//------------------------------------------------------------------------------
//功能选择定义
#define     SYSTEM_SD_CARD_SP
//#define     _DRM9_ENABLE_               //支持DRM9

//------------------------------------------------------------------------------
#define     CPU_TYPE        _RK2608_   //CPU类型
#define     FLASHROM_TYPE   _SLC_       //Flash类型
#define                     _ROCK2_V3_  //模块类型
#define                     _ALL_       //编译范围

//------------------------------------------------------------------------------
#if(CPU_TYPE == _RK2608A_)
    #define _INSIDE_HPSENSE_USED_
#endif

#ifdef  _ROCK2_V3_
    #define CPU_PACK_PIN        128
    #define LCD_DATA_BUS_WIDTH  16
#endif

#ifdef      _ALL_
#define     MODULE_OVERLAY
#define     BROWSER_MODULE
#define     SETMENU_MODULE

#define     MUSIC_MODULE
#define     MP3_DECODE_MODULE
#define     WMA_DECODE_MODULE
#define     WAV_DECODE_MODULE
#define     AVI_DECODE_MODULE
#define     ENCODE_MODULE
#define     FM_MODULE
#define     USB_MODULE
#define     UPGRADE_MODULE

#define     JPEG_DECODE_MODULE
#define     TXT_MODULE
#define     GAME_MODULE
#define     LRC_MODULE
#endif

//------------------------------------------------------------------------------
#ifdef		FM_MODULE
#define     MODULE_MAX_NUM          9
#else
#define     MODULE_MAX_NUM          8
#endif

//Main Menu ID Define
#define     MUSIC_MODULE_ID         0
#define     AVI_DECODE_MODULE_ID    MUSIC_MODULE_ID       + 1

#ifdef		FM_MODULE
#define     FM_MODULE_ID            AVI_DECODE_MODULE_ID  + 1
#else
#define     FM_MODULE_ID            AVI_DECODE_MODULE_ID
#endif

#define     JPEG_DECODE_MODULE_ID   FM_MODULE_ID          + 1
#define     TXT_MODULE_ID           JPEG_DECODE_MODULE_ID + 1
#define     ENCODE_MODULE_ID        TXT_MODULE_ID         + 1
#define     BROWSER_MODULE_ID       ENCODE_MODULE_ID      + 1
#define     GAME_MODULE_ID          BROWSER_MODULE_ID     + 1
#define     SETMENU_MODULE_ID       GAME_MODULE_ID        + 1

/*
********************************************************************************
硬件相关定义
********************************************************************************
*/
//------------------------------------------------------------------------------
//  GPIO0 管脚定义
//------------------------------------------------------------------------------
/******************************************************************************/
#define     SDCARD_INSERT()     (!(read_mem(GPIO_PDAT1) & 0x0010))
#define     USB_INSERT()        ((read_mem(GPIO_PDAT1) & 0x0040))

//------------------------------------------------------------------------------
#ifndef     TRUE
#define     TRUE                            1
#endif
#ifndef     FALSE
#define     FALSE                           0
#endif
#ifndef     NULL
#define     NULL                            0
#endif
//******************************************************************************
#define     read_mem(address)               (*((unsigned int volatile*)(address)))
#define     write_mem(address, value)       (*((unsigned int volatile*)(address)) = (value))

#define     read_mem32(address)             (*((unsigned long volatile*)(address)))
#define     write_mem32(address, value)     (*((unsigned long volatile*)(address)) = (value))

#define     bitset_mem(addr,bitnum)         (write_mem((addr),(read_mem(addr)|(0x0001 << (bitnum)))))
#define     bitclear_mem(addr,bitnum)       (write_mem((addr),(read_mem(addr)&(~(0x0001 << (bitnum))))))
#define     bitinvert_mem(addr,bitnum)      ((read_mem(addr)&(0x0001 << (bitnum))) ? (bitclear_mem(addr,bitnum)) : (bitset_mem(addr,bitnum)))

#ifndef     read_XDATA
#define     read_XDATA(address)             read_mem(address) 
#define     write_XDATA(address, value)     write_mem(address, value)
#endif

//user4 interrupt
#define     AVI_MP3_DECODE_REQ              bitset_creg(%ireq,0)
#define     AVI_MP3_DECODE_REQ_CLR          bitclear_creg(%ireq,0)
#define     AVI_MP3_DECODE_DISABLE          bitclear_creg(%imask,0)
#define     AVI_MP3_DECODE_ENABLE           bitset_creg(%imask,0)

#define     DI_GLOBAL                       bitclear_creg(%imask,15)
#define     EI_GLOBAL                       bitset_creg(%imask,15)

//------------------------------------------------------------------------------
#if 1
    #define     SYSTEM_TEXT_SECTION     __attribute__((section(".text, \"ax\"")))
    #define     SYSTEM_DATA_SECTION     __attribute__((section(".system_data, \"aw\"")))
    #define     SYSTEM_BSS_SECTION      __attribute__((section(".system_bss, \"aw\"")))
#else
    #define     SYSTEM_TEXT_SECTION     
    #define     SYSTEM_DATA_SECTION     
    #define     SYSTEM_BSS_SECTION      
#endif


#define TIMER_DISABLE_INT   {read_creg(%imask, imask_tmp); bitclear_creg(%imask, 15);}
#define TIMER_ENABLE_INT    {write_creg(%imask, imask_tmp);}

#define SYSTEM_TIMER_CHANNEL        0
#define SCROLL_TIMER_CHANNEL        2


#define     TIMER_CODE_SECTION
#define     TIMER_DATA_SECTION
#define     TIMER_BSS_SECTION

#define     TIMER_MAX_CHANNEL   6
//******************************************************************************
//          System Const Define
//******************************************************************************
//Idle Sys Freq define
#define     SYS_IDLE_FREQ                   24

//----System Display Flag define--------------
#define     DISP_MENU                       0
#define     DISP_MUSIC                      1
#define     DISP_AVI                        2
#define     DISP_ENCODE                     3
#define     DISP_FM                         4

#define     DISP_BROWSER                    5
#define     DISP_SETTING                    6
#define     DISP_JPEG                       7
#define     DISP_TEXT                       8
#define     DISP_GAME                       9

#define     DISP_USB                        10
#define     DISP_BATTERY                    11
#define     DISP_HOLD                       12
#define     DISP_DIALOG                     13

#define     DISP_UPGRADE_DIALOG             14

////-------------Color Define-------------------
//#define     COLOR_BLACK                     0x0000
//#define     COLOR_BLUE                      0x001f
//#define     COLOR_GREEN                     0x07e0
//#define     COLOR_YELLOW                    0x07ff
//#define     COLOR_RED                       0xf800
//#define     COLOR_WHITE                     0xffff

//--------Return To Browser Mode---------------
#define     TO_CURRENT_FILE                 0
#define     TO_DEV_FIRST_FILE               1

//--------------Adc Detect---------------------
//Battery Level Detect                          Vb =  2.0v
#define     BATTERY_LEVEL_LOW                820    //3.0
#define     BATTERY_LEVEL_0                  880    //3.2
#define     BATTERY_LEVEL_1                  930    //3.4
#define     BATTERY_LEVEL_2                  990    //3.7
#define     BATTERY_LEVEL_3                 1050    //4.0
#define     BATTERY_LEVEL_4                 1080    //4.2
#define     BATTERY_LEVEL_SETP                 5

//Ear Adc Detect
#define     EAR_ADC_16                      300
#define     EAR_ADC_20                      1200

//Charge Status Detect
#define     CHARGE_ADC_30                   1700

//******************************************************************************
//          Music Play Const Define
//******************************************************************************
//Music AB State define
#define     MUSIC_DECODE_ABOFF              0
#define     MUSIC_DECODE_A_                 1
#define     MUSIC_DECODE_AB                 2

//Music Play State define
#define     MUSIC_DECODE_PLAY               0
#define     MUSIC_DECODE_PAUSE              1
#define     MUSIC_DECODE_STOP               2
#define     MUSIC_DECODE_FFD                3
#define     MUSIC_DECODE_FFW                4

//Music Play Mode Define
#define     MUSIC_DECODE_ONE                0       //Track Once
#define     MUSIC_DECODE_ONE_REP            1       //Repeat Track
#define     MUSIC_DECODE_DIR                2       //Folder Once
#define     MUSIC_DECODE_DIR_REP            3       //Repeat Folder
#define     MUSIC_DECODE_ALL                4       //Normal
#define     MUSIC_DECODE_ALL_REP            5       //Repeat All
#define     MUSIC_DECODE_BROWSE             6       //Repeat All

//Music File Find Order Define
#define     MUSIC_DECODE_ORDER              0
#define     MUSIC_DECODE_RANDOM             1

//Music File Repeat Mode Define
#define     MUSIC_REPMODE_NORMAL            0
#define     MUSIC_REPMODE_REPEAT            1

//Music EQ Define
#define     MUSIC_DECODE_EQ_NORMAL          0
#define     MUSIC_DECODE_EQ_3D              1
#define     MUSIC_DECODE_EQ_ROCK            2 
#define     MUSIC_DECODE_EQ_POP             3 
#define     MUSIC_DECODE_EQ_CLASSIC         4 
#define     MUSIC_DECODE_EQ_BASS            5 
#define     MUSIC_DECODE_EQ_JAZZ            6 
#define     MUSIC_DECODE_EQ_USER 		    7 
#define     MUSIC_DECODE_EQ_METAL           8 
#define     MUSIC_DECODE_EQ_DANCE           9 
#define     MUSIC_DECODE_EQ_PARTY           10 
#define     MUSIC_DECODE_EQ_CLUB            11

//Music EQ Mode Define
#define		NORMAL			                0
#define		ROCK			                1
#define		POP                             2
#define		CLASSIC			                3
#define		BASS			                4
#define		JAZZ			                5
#define     METAL                           6
#define     DANCE                           7
#define     PARTY                           8
#define     CLUB                            9
#define		USER                            10
#define		EQ3D                            11

//Music Display Mode Define
#define     MUSIC_DISPLAY                   0 //Display (MUSIC_DISPFLAG_PATH | MUSIC_DISPFLAG_FILENAME | MUSIC_DISPFLAG_SPECTRUM)
#define     ID3_DISPLAY                     1 //Display (MUSIC_DISPFLAG_ID3  | MUSIC_DISPFLAG_SPECTRUM)
#define     LRC_DISPLAY                     2 //Display (MUSIC_DISPFLAG_ID3  | MUSIC_DISPFLAG_LRC)

//Music Display Flag define
#define     MUSIC_DISPFLAG_NULL             ((unsigned long)(0x00000000))

#define     MUSIC_DISPFLAG_BACKGROUND       ((unsigned long)(0x00000001))

#define     MUSIC_DISPFLAG_SPECTRUM         ((unsigned long)(0x00000002))

#define     MUSIC_DISPFLAG_GUAGE            ((unsigned long)(0x00000004))

#define     MUSIC_DISPFLAG_CUR_TIME         ((unsigned long)(0x00000008))
#define     MUSIC_DISPFLAG_FREE_TIME        ((unsigned long)(0x00000008))

#define     MUSIC_DISPFLAG_LRC              ((unsigned long)(0x00000010))

#define     MUSIC_DISPFLAG_BATT             ((unsigned long)(0x00000020))

#define     MUSIC_DISPFLAG_BITRATE          ((unsigned long)(0x00000040))

#define     MUSIC_DISPFLAG_STATUS           ((unsigned long)(0x00000080))

#define     MUSIC_DISPFLAG_VOL              ((unsigned long)(0x00000100))

#define     MUSIC_DISPFLAG_HOLD             ((unsigned long)(0x00000200))

#define     MUSIC_DISPFLAG_AB               ((unsigned long)(0x00000400))

#define     MUSIC_DISPFLAG_PATH             ((unsigned long)(0x00000800))
#define     MUSIC_DISPFLAG_FILENAME         ((unsigned long)(0x00000800))

#define     MUSIC_DISPFLAG_ID3              ((unsigned long)(0x00001000))

#define     MUSIC_DISPFLAG_MODE             ((unsigned long)(0x00002000))

#define     MUSIC_DISPFLAG_ORDER            ((unsigned long)(0x00004000))

#define     MUSIC_DISPFLAG_EQ               ((unsigned long)(0x00008000))

#define     MUSIC_DISPFLAG_FILENUM          ((unsigned long)(0x00010000))

#define     MUSIC_DISPFLAG_ALL              ((unsigned long)(0xffffffff))
//******************************************************************************
//          Encode Const Define
//******************************************************************************
//only use for encode 
#define     MUSIC_ENCODE_PLAY               0
#define     MUSIC_ENCODE_PAUSE              1
#define     MUSIC_ENCODE_STOP               2

#define     MUSIC_ENCODE_READY              2
#define     MUSIC_ENCODE_END                3

//Encode 模式选择
#define     ENCODE_MIC_REC                  0
#define     ENCODE_LINE_IN                  1
#define     ENCODE_FM_REC                   2

//******************************************************************************
//          Fm Const Define
//******************************************************************************
//Fm Macro Define
#define     FREQMAXNUMBLE                   20

#define     FM_AREA_CHINA                   0
#define     FM_AREA_EUROPE                  1
#define     FM_AREA_JAPAN                   2
#define     FM_AREA_USA                     3

//******************************************************************************
//          Setting Menu Const Define
//******************************************************************************
//Max Language define
#define     LANGUAGE_MAX_COUNT              11

#define     LANGUE_CHINESE_S                 0      //简体中文
#define     LANGUE_CHINESE_T                 1      //繁体中文
#define     LANGUE_ENGLISH                   2      //英语
#define     LANGUE_KOREA                     3      //韩语
#define     LANGUE_JAPANESE                  4      //日语
#define     LANGUE_SPANISH                   5      //西班牙语
#define     LANGUE_FRANCH                    6      //法语
#define     LANGUE_GERMAN                    7      //德语
#define     LANGUE_ITALIAN                   8      //意大利与
#define     LANGUE_PORTUGUESE                9      //葡萄牙语
#define     LANGUE_RUSSIAN                  10      //俄语

#define     LANGUE_FINNISH                  11      //芬兰语
#define     LANGUE_NORWEGIAN                12      //挪威语
#define     LANGUE_SWEDISH                  13      //瑞典语
#define     LANGUE_DENISH                   14      //丹麦语

#define     DEFAULT_LANGUE                  LANGUE_CHINESE_S
//------------------------------------------------------------------------------

#endif
//******************************************************************************
