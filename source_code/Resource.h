//******************************************************************************
//  Resource.h          2005-6-21 14:43        copyright  rock-chip  authour zyz
//******************************************************************************
#ifndef  _RESOURCE_H
#define  _RESOURCE_H
//------------------------------------------------------------------------------
//Flash Memery Address Distribute
#define     APPLICATION_CODE_MODULE_NUM         0
#define     APPLICATION_DATA_MODULE_NUM         1
#define     APPLICATION_BSS_MODULE_NUM          2
#define     MP3_CODE_MODULE_NUM                 3
#define     MP3_DATA_MODULE_NUM                 4
#define     MP3_BSS_MODULE_NUM                  5
#define     AVI_CODE_MODULE_NUM                 6
#define     AVI_DATA_MODULE_NUM                 7
#define     AVI_BSS_MODULE_NUM                  8
#define     JPEG_CODE_MODULE_NUM                9 
#define     JPEG_DATA_MODULE_NUM                10
#define     JPEG_BSS_MODULE_NUM                 11
#define     WMA_CODE_MODULE_NUM                 12
#define     WMA_DATA_MODULE_NUM                 13
#define     WMA_BSS_MODULE_NUM                  14
#define     WMA440B_CODE_MODULE_NUM             15
#define     WMA160B_DATA_MODULE_NUM             16
#define     WMA44QB_BSS_MODULE_NUM              17
#define     ENC_CODE_MODULE_NUM                 18
#define     ENC_DATA_MODULE_NUM                 19
#define     ENC_BSS_MODULE_NUM                  20
#define     FM_CODE_MODULE_NUM                  21
#define     FM_DATA_MODULE_NUM                  22
#define     FM_BSS_MODULE_NUM                   23
#define     TXT_CODE_MODULE_NUM                 24
#define     TXT_DATA_MODULE_NUM                 25
#define     TXT_BSS_MODULE_NUM                  26
#define     USB_CODE_MODULE_NUM                 27
#define     USB_DATA_MODULE_NUM                 28
#define     USB_BSS_MODULE_NUM                  29
#define     WAV_CODE_MODULE_NUM                 30
#define     WAV_DATA_MODULE_NUM                 31
#define     WAV_BSS_MODULE_NUM                  32
#define     SETMENU_CODE_MODULE_NUM             33
#define     FIRMUPDATE_CODE_MODULE_NUM          33
#define     FIRMUPDATE_DATA_MODULE_NUM          34
#define     FIRMUPDATE_BSS_MODULE_NUM           35
#define     GAME_CODE_MODULE_NUM                36
#define     GAME_DATA_MODULE_NUM                37
#define     GAME_BSS_MODULE_NUM                 38
#define     LRC_CODE_MODULE_NUM                 39
#define     LRC_DATA_MODULE_NUM                 40
#define     LRC_BSS_MODULE_NUM                  41

/*
****************************************************************************************************
*          Copyright (C),2006-2006, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* ModuleName :  Image.res
* Author     :  tonyzyz
* Description:  Individual Define The Image Address of Every Function Module :
*               1.Power On/Off 
*               2.Main Menu 
*               3.Music    Module
*               4.Video    Module
*               5.Radio    Module
*               6.Photo    Module
*               7.Text     Module
*               8.Record   Module
*               9.Explorer Module
*              10.Game     Module
*              11.Setup    Module
*              12.USB      Module
*              13.Other    Module
* History    :  <author>        <time>      <version>       <desc>
*                tonyzyz      2006-07-25       1.0           ORG
*
****************************************************************************************************
*/

#define     MAX_PIC_XSIZE                   320
#define     MAX_PIC_YSIZE                   240
#define     MAX_PIC_SIZE                    ((unsigned long)((unsigned long)(MAX_PIC_XSIZE) * MAX_PIC_YSIZE * 2))
#define     IMAGE_IN_FLASH_BASE_ADDR        (unsigned long)0
//**************************************************************************************************
/*
------------------------------------------------------------------------------------------
* ModuleName :  Power On/Off 
* Author     :  tonyzyz
* Description:  Power On / Off Image Address Define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/
//------Power On Display Picture-------------
#define     NFA_POWER_ON_PIC                IMAGE_IN_FLASH_BASE_ADDR
#define     POWER_ON_PIC_CTL                12
//------Power Off Display Picture------------
#define     NFA_POWER_OFF_PIC               ((unsigned long)(NFA_POWER_ON_PIC + ((unsigned long)(MAX_PIC_SIZE) * POWER_ON_PIC_CTL)))
#define     POWER_OFF_PIC_CTL               12   
/*
------------------------------------------------------------------------------------------
* ModuleName :  Main Menu
* Author     :  tonyzyz
* Description:  Main Menu Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/
//--------Main Menu Picture Address----------
#define     NFA_MAIN_MENU_PIC               ((unsigned long)(NFA_POWER_OFF_PIC + ((unsigned long)(MAX_PIC_SIZE) * POWER_OFF_PIC_CTL)))
#define     MAIN_MENU_PIC_CTL               9

#define     MAIN_MENU_MUSIC                 NFA_MAIN_MENU_PIC
#define     MAIN_MENU_AVI                   ((unsigned long)(MAIN_MENU_MUSIC    + ((unsigned long)(MAX_PIC_SIZE))))
#define     MAIN_MENU_RADIO                 ((unsigned long)(MAIN_MENU_AVI      + ((unsigned long)(MAX_PIC_SIZE))))
#define     MAIN_MENU_PHOTO                 ((unsigned long)(MAIN_MENU_RADIO    + ((unsigned long)(MAX_PIC_SIZE))))
#define     MAIN_MENU_TEXT                  ((unsigned long)(MAIN_MENU_PHOTO    + ((unsigned long)(MAX_PIC_SIZE))))
#define     MAIN_MENU_REC                   ((unsigned long)(MAIN_MENU_TEXT     + ((unsigned long)(MAX_PIC_SIZE))))
#define     MAIN_MENU_EXP                   ((unsigned long)(MAIN_MENU_REC      + ((unsigned long)(MAX_PIC_SIZE))))
#define     MAIN_MENU_GAME                  ((unsigned long)(MAIN_MENU_EXP      + ((unsigned long)(MAX_PIC_SIZE))))
#define     MAIN_MENU_SETUP                 ((unsigned long)(MAIN_MENU_GAME     + ((unsigned long)(MAX_PIC_SIZE))))

//----------------BATT-------------------
#define     NFA_MMENU_BATT_PIC              ((unsigned long)(NFA_MAIN_MENU_PIC  + ((unsigned long)(MAX_PIC_SIZE) * MAIN_MENU_PIC_CTL)))
#define     MMENU_BATT_PIC_X                265
#define     MMENU_BATT_PIC_Y                4
#define     MMENU_BATT_PIC_XSIZE            40
#define     MMENU_BATT_PIC_YSIZE            20
#define     MMENU_BATT_PIC_SIZE             ((unsigned long)((unsigned long)(MMENU_BATT_PIC_XSIZE) * MMENU_BATT_PIC_YSIZE * 2))
#define     MMENU_BATT_PIC_CTL              4
                                            
#define     MMENU_BATT_PIC_1                NFA_MMENU_BATT_PIC
#define     MMENU_BATT_PIC_2                ((unsigned long)(MMENU_BATT_PIC_1   + ((unsigned long)(MMENU_BATT_PIC_SIZE))))
#define     MMENU_BATT_PIC_3                ((unsigned long)(MMENU_BATT_PIC_2   + ((unsigned long)(MMENU_BATT_PIC_SIZE))))
#define     MMENU_BATT_PIC_4                ((unsigned long)(MMENU_BATT_PIC_3   + ((unsigned long)(MMENU_BATT_PIC_SIZE))))

//----------------Hold-------------------
#define     NFA_MMENU_HOLD_PIC              ((unsigned long)(NFA_MMENU_BATT_PIC  + ((unsigned long)(MMENU_BATT_PIC_SIZE) * MMENU_BATT_PIC_CTL)))
#define     MMENU_HOLD_PIC_X                242
#define     MMENU_HOLD_PIC_Y                4
#define     MMENU_HOLD_PIC_XSIZE            20
#define     MMENU_HOLD_PIC_YSIZE            20
#define     MMENU_HOLD_PIC_SIZE             ((unsigned long)((unsigned long)(MMENU_HOLD_PIC_XSIZE) * MMENU_HOLD_PIC_YSIZE * 2))
#define     MMENU_HOLD_PIC_CTL              2
                                            
#define     MMENU_HOLD_OFF_PIC              NFA_MMENU_HOLD_PIC
#define     MMENU_HOLD_ON_PIC               ((unsigned long)(MMENU_HOLD_OFF_PIC + ((unsigned long)(MMENU_HOLD_PIC_SIZE))))

/*
------------------------------------------------------------------------------------------
* ModuleName :  Music Module
* Author     :  tonyzyz
* Description:  Music Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/
//--------Music BackGround Address-----------
#define     NFA_MUSIC_PIC                   ((unsigned long)(NFA_MMENU_HOLD_PIC + ((unsigned long)(MMENU_HOLD_PIC_SIZE) * MMENU_HOLD_PIC_CTL)))
#define     MUSIC_BKGROUND_PIC              NFA_MUSIC_PIC

//------------------Hold---------------------
#define     NFA_MUSIC_HOLD_PIC              ((unsigned long)(MUSIC_BKGROUND_PIC + ((unsigned long)(MAX_PIC_SIZE))))
#define     MUSIC_HOLD_PIC_X                242
#define     MUSIC_HOLD_PIC_Y                4
#define     MUSIC_HOLD_PIC_XSIZE            20
#define     MUSIC_HOLD_PIC_YSIZE            20
#define     MUSIC_HOLD_PIC_SIZE             ((unsigned long)((unsigned long)(MUSIC_HOLD_PIC_XSIZE) * MUSIC_HOLD_PIC_YSIZE * 2))
#define     MUSIC_HOLD_PIC_CTL              2

#define     MUSIC_HOLD_OFF_PIC              NFA_MUSIC_HOLD_PIC
#define     MUSIC_HOLD_ON_PIC               ((unsigned long)(MUSIC_HOLD_OFF_PIC + ((unsigned long)(MUSIC_HOLD_PIC_SIZE))))

//-----------------Battery-------------------
#define     NFA_MUSIC_BATT_PIC              ((unsigned long)(NFA_MUSIC_HOLD_PIC + ((unsigned long)(MUSIC_HOLD_PIC_SIZE) * MUSIC_HOLD_PIC_CTL)))
#define     MUSIC_BATT_PIC_X                265
#define     MUSIC_BATT_PIC_Y                4
#define     MUSIC_BATT_PIC_XSIZE            40
#define     MUSIC_BATT_PIC_YSIZE            20
#define     MUSIC_BATT_PIC_SIZE             ((unsigned long)((unsigned long)(MUSIC_BATT_PIC_XSIZE) * MUSIC_BATT_PIC_YSIZE * 2))
#define     MUSIC_BATT_PIC_CTL              4

#define     MUSIC_BATT_PIC_1                NFA_MUSIC_BATT_PIC
#define     MUSIC_BATT_PIC_2                ((unsigned long)(MUSIC_BATT_PIC_1   + ((unsigned long)(MUSIC_BATT_PIC_SIZE))))
#define     MUSIC_BATT_PIC_3                ((unsigned long)(MUSIC_BATT_PIC_2   + ((unsigned long)(MUSIC_BATT_PIC_SIZE))))
#define     MUSIC_BATT_PIC_4                ((unsigned long)(MUSIC_BATT_PIC_3   + ((unsigned long)(MUSIC_BATT_PIC_SIZE))))

//-----------------Spectrum------------------
#define     NFA_MUSIC_SPECTRUM_PIC          ((unsigned long)(NFA_MUSIC_BATT_PIC + ((unsigned long)(MUSIC_BATT_PIC_SIZE) * MUSIC_BATT_PIC_CTL)))
#define     MUSIC_SPECTRUM_PIC_X            32
#define     MUSIC_SPECTRUM_PIC_Y            27
#define     MUSIC_SPECTRUM_PIC_XSIZE        10
#define     MUSIC_SPECTRUM_PIC_YSIZE        72
#define     MUSIC_SPECTRUM_PIC_SIZE         (unsigned long)((unsigned long)(MUSIC_SPECTRUM_PIC_XSIZE) * MUSIC_SPECTRUM_PIC_YSIZE * 2)
#define     MUSIC_SPECTRUM_PIC_CTL          24

#define     MUSIC_SPECTRUM_PIC_01           NFA_MUSIC_SPECTRUM_PIC
#define     MUSIC_SPECTRUM_PIC_02           ((unsigned long)(MUSIC_SPECTRUM_PIC_01 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_03           ((unsigned long)(MUSIC_SPECTRUM_PIC_02 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_04           ((unsigned long)(MUSIC_SPECTRUM_PIC_03 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_05           ((unsigned long)(MUSIC_SPECTRUM_PIC_04 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_06           ((unsigned long)(MUSIC_SPECTRUM_PIC_05 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_07           ((unsigned long)(MUSIC_SPECTRUM_PIC_06 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_08           ((unsigned long)(MUSIC_SPECTRUM_PIC_07 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_09           ((unsigned long)(MUSIC_SPECTRUM_PIC_08 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_10           ((unsigned long)(MUSIC_SPECTRUM_PIC_09 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_11           ((unsigned long)(MUSIC_SPECTRUM_PIC_10 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_12           ((unsigned long)(MUSIC_SPECTRUM_PIC_11 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_13           ((unsigned long)(MUSIC_SPECTRUM_PIC_12 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_14           ((unsigned long)(MUSIC_SPECTRUM_PIC_13 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_15           ((unsigned long)(MUSIC_SPECTRUM_PIC_14 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_16           ((unsigned long)(MUSIC_SPECTRUM_PIC_15 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_17           ((unsigned long)(MUSIC_SPECTRUM_PIC_16 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_18           ((unsigned long)(MUSIC_SPECTRUM_PIC_17 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_19           ((unsigned long)(MUSIC_SPECTRUM_PIC_18 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_20           ((unsigned long)(MUSIC_SPECTRUM_PIC_19 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_21           ((unsigned long)(MUSIC_SPECTRUM_PIC_20 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_22           ((unsigned long)(MUSIC_SPECTRUM_PIC_21 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_23           ((unsigned long)(MUSIC_SPECTRUM_PIC_22 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))
#define     MUSIC_SPECTRUM_PIC_24           ((unsigned long)(MUSIC_SPECTRUM_PIC_23 + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE))))

//----------------Lrc BackGround-------------
#define     NFA_MUSIC_LRCBK_PIC             ((unsigned long)(NFA_MUSIC_SPECTRUM_PIC + ((unsigned long)(MUSIC_SPECTRUM_PIC_SIZE) * MUSIC_SPECTRUM_PIC_CTL)))
#define     MUSIC_LRCBK_PIC_X               32
#define     MUSIC_LRCBK_PIC_Y               27
#define     MUSIC_LRCBK_PIC_XSIZE           170
#define     MUSIC_LRCBK_PIC_YSIZE           72
#define     MUSIC_LRCBK_PIC_SIZE            ((unsigned long)((unsigned long)(MUSIC_LRCBK_PIC_XSIZE) * MUSIC_LRCBK_PIC_YSIZE * 2))
#define     MUSIC_LRCBK_PIC_CTL             1

#define     MUSIC_LRCBK_PIC                 NFA_MUSIC_LRCBK_PIC

//--------------File Num BackGround----------
#define     NFA_MUSIC_FILENUMBK_PIC         ((unsigned long)(NFA_MUSIC_LRCBK_PIC + ((unsigned long)(MUSIC_LRCBK_PIC_SIZE) * MUSIC_LRCBK_PIC_CTL)))
#define     MUSIC_FILENUMBK_PIC_X           230
#define     MUSIC_FILENUMBK_PIC_Y           30
#define     MUSIC_FILENUMBK_PIC_XSIZE       80
#define     MUSIC_FILENUMBK_PIC_YSIZE       20
#define     MUSIC_FILENUMBK_PIC_SIZE        ((unsigned long)((unsigned long)(MUSIC_FILENUMBK_PIC_XSIZE) * MUSIC_FILENUMBK_PIC_YSIZE * 2))
#define     MUSIC_FILENUMBK_PIC_CTL         1
                                            
#define     MUSIC_FILENUMBK_PIC             NFA_MUSIC_FILENUMBK_PIC

//--------------Bitrate BackGround-----------
#define     NFA_MUSIC_BITERATEBK_PIC        ((unsigned long)(NFA_MUSIC_FILENUMBK_PIC + ((unsigned long)(MUSIC_FILENUMBK_PIC_SIZE) * MUSIC_FILENUMBK_PIC_CTL)))
#define     MUSIC_BITERATEBK_PIC_X          230
#define     MUSIC_BITERATEBK_PIC_Y          54
#define     MUSIC_BITERATEBK_PIC_XSIZE      80
#define     MUSIC_BITERATEBK_PIC_YSIZE      20
#define     MUSIC_BITERATEBK_PIC_SIZE       ((unsigned long)((unsigned long)(MUSIC_BITERATEBK_PIC_XSIZE) * MUSIC_BITERATEBK_PIC_YSIZE * 2))
#define     MUSIC_BITERATEBK_PIC_CTL        1

#define     MUSIC_BITERATEBK_PIC            NFA_MUSIC_BITERATEBK_PIC

//--------------Eq BackGround----------------
#define     NFA_MUSIC_EQBK_PIC              ((unsigned long)(NFA_MUSIC_BITERATEBK_PIC + ((unsigned long)(MUSIC_BITERATEBK_PIC_SIZE) * MUSIC_BITERATEBK_PIC_CTL)))
#define     MUSIC_EQBK_PIC_X                230
#define     MUSIC_EQBK_PIC_Y                77
#define     MUSIC_EQBK_PIC_XSIZE            80
#define     MUSIC_EQBK_PIC_YSIZE            20
#define     MUSIC_EQBK_PIC_SIZE             ((unsigned long)((unsigned long)(MUSIC_EQBK_PIC_XSIZE) * MUSIC_EQBK_PIC_YSIZE * 2))
#define     MUSIC_EQBK_PIC_CTL              1
                                            
#define     MUSIC_EQBK_PIC                  NFA_MUSIC_EQBK_PIC

//-------------File Name BackGround----------
#define     NFA_MUSIC_FILENAMEBK_PIC        ((unsigned long)(NFA_MUSIC_EQBK_PIC + ((unsigned long)(MUSIC_EQBK_PIC_SIZE) * MUSIC_EQBK_PIC_CTL)))
#define     MUSIC_FILENAMEBK_PIC_X          45
#define     MUSIC_FILENAMEBK_PIC_Y          110
#define     MUSIC_FILENAMEBK_PIC_XSIZE      220
#define     MUSIC_FILENAMEBK_PIC_YSIZE      50
#define     MUSIC_FILENAMEBK_PIC_SIZE       ((unsigned long)((unsigned long)(MUSIC_FILENAMEBK_PIC_XSIZE) * MUSIC_FILENAMEBK_PIC_YSIZE * 2))
#define     MUSIC_FILENAMEBK_PIC_CTL        1

#define     MUSIC_FILENAMEBK_PIC            NFA_MUSIC_FILENAMEBK_PIC

//-------------ID3 Title BackGround----------
#define     NFA_MUSIC_ID3TITLEBK_PIC        ((unsigned long)(NFA_MUSIC_FILENAMEBK_PIC + ((unsigned long)(MUSIC_FILENAMEBK_PIC_SIZE) * MUSIC_FILENAMEBK_PIC_CTL)))
#define     MUSIC_ID3TITLEBK_PIC_X          0
#define     MUSIC_ID3TITLEBK_PIC_Y          0
#define     MUSIC_ID3TITLEBK_PIC_XSIZE      0
#define     MUSIC_ID3TITLEBK_PIC_YSIZE      0
#define     MUSIC_ID3TITLEBK_PIC_SIZE       ((unsigned long)((unsigned long)(MUSIC_ID3TITLEBK_PIC_XSIZE) * MUSIC_ID3TITLEBK_PIC_YSIZE * 2))
#define     MUSIC_ID3TITLEBK_PIC_CTL        1
                                            
#define     MUSIC_ID3TITLEBK_PIC            NFA_MUSIC_ID3TITLEBK_PIC

//-------------ID3 Album BackGround----------
#define     NFA_MUSIC_ID3ALBUMBK_PIC        ((unsigned long)(NFA_MUSIC_ID3TITLEBK_PIC + ((unsigned long)(MUSIC_ID3TITLEBK_PIC_SIZE) * MUSIC_ID3TITLEBK_PIC_CTL)))
#define     MUSIC_ID3ALBUMBK_PIC_X          45
#define     MUSIC_ID3ALBUMBK_PIC_Y          110
#define     MUSIC_ID3ALBUMBK_PIC_XSIZE      220
#define     MUSIC_ID3ALBUMBK_PIC_YSIZE      50
#define     MUSIC_ID3ALBUMBK_PIC_SIZE       ((unsigned long)((unsigned long)(MUSIC_ID3ALBUMBK_PIC_XSIZE) * MUSIC_ID3ALBUMBK_PIC_YSIZE * 2))
#define     MUSIC_ID3ALBUMBK_PIC_CTL        1
                                            
#define     MUSIC_ID3ALBUMBK_PIC            NFA_MUSIC_ID3ALBUMBK_PIC

#define     MUSIC_ID3ALBUM_TXT_X            55
#define     MUSIC_ID3ALBUM_TXT_Y            113
//------------ID3 Artist BackGround----------
#define     NFA_MUSIC_ID3ARTBK_PIC          ((unsigned long)(NFA_MUSIC_ID3ALBUMBK_PIC + ((unsigned long)(MUSIC_ID3ALBUMBK_PIC_SIZE) * MUSIC_ID3ALBUMBK_PIC_CTL)))
#define     MUSIC_ID3ARTBK_PIC_X            0
#define     MUSIC_ID3ARTBK_PIC_Y            0
#define     MUSIC_ID3ARTBK_PIC_XSIZE        0
#define     MUSIC_ID3ARTBK_PIC_YSIZE        0
#define     MUSIC_ID3ARTBK_PIC_SIZE         ((unsigned long)((unsigned long)(MUSIC_ID3ARTBK_PIC_XSIZE) * MUSIC_ID3ARTBK_PIC_YSIZE * 2))
#define     MUSIC_ID3ARTBK_PIC_CTL          1
                                              
#define     MUSIC_ID3ARTBK_PIC              NFA_MUSIC_ID3ARTBK_PIC

#define     MUSIC_ID3ART_TXT_X              55
#define     MUSIC_ID3ART_TXT_Y              140
//-----------------A-B-----------------------
#define     NFA_MUSIC_AB_PIC                ((unsigned long)(NFA_MUSIC_ID3ARTBK_PIC+ ((unsigned long)(MUSIC_ID3ARTBK_PIC_SIZE) * MUSIC_ID3ARTBK_PIC_CTL)))
#define     MUSIC_AB_PIC_X                  98
#define     MUSIC_AB_PIC_Y                  164
#define     MUSIC_AB_PIC_XSIZE              32    
#define     MUSIC_AB_PIC_YSIZE              16
#define     MUSIC_AB_PIC_SIZE               ((unsigned long)((unsigned long)(MUSIC_AB_PIC_XSIZE) * MUSIC_AB_PIC_YSIZE * 2))
#define     MUSIC_AB_PIC_CTL                3

#define     MUSIC_AB_OFF_PIC                NFA_MUSIC_AB_PIC
#define     MUSIC_AB_A_PIC                  ((unsigned long)(MUSIC_AB_OFF_PIC    + ((unsigned long)(MUSIC_AB_PIC_SIZE))))
#define     MUSIC_AB_AB_PIC                 ((unsigned long)(MUSIC_AB_A_PIC     + ((unsigned long)(MUSIC_AB_PIC_SIZE))))

//-----------Current Time BackGround---------
#define     NFA_MUSIC_TIMECURRBK_PIC        ((unsigned long)(NFA_MUSIC_AB_PIC + ((unsigned long)(MUSIC_AB_PIC_SIZE) * MUSIC_AB_PIC_CTL)))
#define     MUSIC_TIMECURRBK_PIC_X          9
#define     MUSIC_TIMECURRBK_PIC_Y          164
#define     MUSIC_TIMECURRBK_PIC_XSIZE      70
#define     MUSIC_TIMECURRBK_PIC_YSIZE      16
#define     MUSIC_TIMECURRBK_PIC_SIZE       ((unsigned long)((unsigned long)(MUSIC_TIMECURRBK_PIC_XSIZE) * MUSIC_TIMECURRBK_PIC_YSIZE * 2))
#define     MUSIC_TIMECURRBK_PIC_CTL        1
                                            
#define     MUSIC_TIMECURRBK_PIC            NFA_MUSIC_TIMECURRBK_PIC

//------------Total Time BackGround----------
#define     NFA_MUSIC_TIMETOTALBK_PIC       ((unsigned long)(NFA_MUSIC_TIMECURRBK_PIC   + ((unsigned long)(MUSIC_TIMECURRBK_PIC_SIZE) * MUSIC_TIMECURRBK_PIC_CTL)))
#define     MUSIC_TIMETOTALBK_PIC_X         147
#define     MUSIC_TIMETOTALBK_PIC_Y         164
#define     MUSIC_TIMETOTALBK_PIC_XSIZE     72
#define     MUSIC_TIMETOTALBK_PIC_YSIZE     16
#define     MUSIC_TIMETOTALBK_PIC_SIZE      ((unsigned long)((unsigned long)(MUSIC_TIMETOTALBK_PIC_XSIZE) * MUSIC_TIMETOTALBK_PIC_YSIZE * 2))
#define     MUSIC_TIMETOTALBK_PIC_CTL       1
                                            
#define     MUSIC_TIMETOTALBK_PIC           NFA_MUSIC_TIMETOTALBK_PIC

//------------------Guage--------------------
#define     NFA_MUSIC_GUAGE_PIC             ((unsigned long)(NFA_MUSIC_TIMETOTALBK_PIC  + ((unsigned long)(MUSIC_TIMETOTALBK_PIC_SIZE) * MUSIC_TIMETOTALBK_PIC_CTL)))
#define     MUSIC_GUAGE_PIC_X               18 
#define     MUSIC_GUAGE_PIC_Y               192
#define     MUSIC_GUAGE_PIC_XSIZE           1       
#define     MUSIC_GUAGE_PIC_YSIZE           5
#define     MUSIC_GUAGE_PIC_SIZE            ((unsigned long)((unsigned long)(MUSIC_GUAGE_PIC_XSIZE) * MUSIC_GUAGE_PIC_YSIZE * 2))
#define     MUSIC_GUAGE_PIC_CTL             2

#define     MUSIC_GUAGE_BKGROUND_PIC        NFA_MUSIC_GUAGE_PIC
#define     MUSIC_GUAGE_FRONT_PIC           ((unsigned long)(MUSIC_GUAGE_BKGROUND_PIC   + ((unsigned long)(MUSIC_GUAGE_PIC_SIZE))))

//----------------Play Status----------------
#define     NFA_MUSIC_PLAY_STATUS_PIC       ((unsigned long)(NFA_MUSIC_GUAGE_PIC+ ((unsigned long)(MUSIC_GUAGE_PIC_SIZE) * MUSIC_GUAGE_PIC_CTL)))
#define     MUSIC_PLAY_STATUS_PIC_X         112
#define     MUSIC_PLAY_STATUS_PIC_Y         211
#define     MUSIC_PLAY_STATUS_PIC_XSIZE     20
#define     MUSIC_PLAY_STATUS_PIC_YSIZE     20
#define     MUSIC_PLAY_STATUS_PIC_SIZE      ((unsigned long)((unsigned long)(MUSIC_PLAY_STATUS_PIC_XSIZE) * MUSIC_PLAY_STATUS_PIC_YSIZE * 2))
#define     MUSIC_PLAY_STATUS_PIC_CTL       3
                                            
#define     MUSIC_STATUS_PLAY_PIC           NFA_MUSIC_PLAY_STATUS_PIC
#define     MUSIC_STATUS_PAUSE_PIC          ((unsigned long)(MUSIC_STATUS_PLAY_PIC   + ((unsigned long)(MUSIC_PLAY_STATUS_PIC_SIZE))))
#define     MUSIC_STATUS_STOP_PIC           ((unsigned long)(MUSIC_STATUS_PAUSE_PIC  + ((unsigned long)(MUSIC_PLAY_STATUS_PIC_SIZE))))
#define     MUSIC_STATUS_FFD_PIC            ((unsigned long)(MUSIC_STATUS_STOP_PIC   + ((unsigned long)(MUSIC_PLAY_STATUS_PIC_SIZE))))
#define     MUSIC_STATUS_FFW_PIC            ((unsigned long)(MUSIC_STATUS_FFD_PIC    + ((unsigned long)(MUSIC_PLAY_STATUS_PIC_SIZE))))

//----------------Repeat Mode---------------
#define     NFA_MUSIC_REPMODE_PIC           ((unsigned long)(NFA_MUSIC_PLAY_STATUS_PIC + ((unsigned long)(MUSIC_PLAY_STATUS_PIC_SIZE) * MUSIC_PLAY_STATUS_PIC_CTL)))
#define     MUSIC_REPMODE_PIC_X             58
#define     MUSIC_REPMODE_PIC_Y             211
#define     MUSIC_REPMODE_PIC_XSIZE         40
#define     MUSIC_REPMODE_PIC_YSIZE         20
#define     MUSIC_REPMODE_PIC_SIZE          ((unsigned long)((unsigned long)(MUSIC_REPMODE_PIC_XSIZE) * MUSIC_REPMODE_PIC_YSIZE * 2))
#define     MUSIC_REPMODE_PIC_CTL           7
                                            
#define     MUSIC_REPMODE_ONE_PIC           NFA_MUSIC_REPMODE_PIC
#define     MUSIC_REPMODE_ONE_REP_PIC       ((unsigned long)(MUSIC_REPMODE_ONE_PIC          + ((unsigned long)(MUSIC_REPMODE_PIC_SIZE))))
#define     MUSIC_REPMODE_FOLDER_PIC        ((unsigned long)(MUSIC_REPMODE_ONE_REP_PIC      + ((unsigned long)(MUSIC_REPMODE_PIC_SIZE))))
#define     MUSIC_REPMODE_FOLDER_REP_PIC    ((unsigned long)(MUSIC_REPMODE_FOLDER_PIC       + ((unsigned long)(MUSIC_REPMODE_PIC_SIZE))))
#define     MUSIC_REPMODE_ALL_PIC           ((unsigned long)(MUSIC_REPMODE_FOLDER_REP_PIC   + ((unsigned long)(MUSIC_REPMODE_PIC_SIZE))))
#define     MUSIC_REPMODE_ALL_REP_PIC       ((unsigned long)(MUSIC_REPMODE_ALL_PIC          + ((unsigned long)(MUSIC_REPMODE_PIC_SIZE))))
#define     MUSIC_REPMODE_PREVIEW_PIC       ((unsigned long)(MUSIC_REPMODE_ALL_REP_PIC      + ((unsigned long)(MUSIC_REPMODE_PIC_SIZE))))

//----------------Play Order-----------------
#define     NFA_MUSIC_ORDER_PIC             ((unsigned long)(NFA_MUSIC_REPMODE_PIC + ((unsigned long)(MUSIC_REPMODE_PIC_SIZE) * MUSIC_REPMODE_PIC_CTL)))
#define     MUSIC_ORDER_PIC_X               147
#define     MUSIC_ORDER_PIC_Y               211
#define     MUSIC_ORDER_PIC_XSIZE           40
#define     MUSIC_ORDER_PIC_YSIZE           20
#define     MUSIC_ORDER_PIC_SIZE            ((unsigned long)((unsigned long)(MUSIC_ORDER_PIC_XSIZE) * MUSIC_ORDER_PIC_YSIZE * 2))
#define     MUSIC_ORDER_PIC_CTL             2
                                            
#define     MUSIC_ORDER_ORDER_PIC           NFA_MUSIC_ORDER_PIC
#define     MUSIC_ORDER_RANDOM_PIC          ((unsigned long)(MUSIC_ORDER_ORDER_PIC  + ((unsigned long)(MUSIC_ORDER_PIC_SIZE))))

//--------------Vol BackGround---------------
#define     NFA_MUSIC_VOLBK_PIC             ((unsigned long)(NFA_MUSIC_ORDER_PIC + ((unsigned long)(MUSIC_ORDER_PIC_SIZE) * MUSIC_ORDER_PIC_CTL)))
#define     MUSIC_VOLBK_PIC_X               216
#define     MUSIC_VOLBK_PIC_Y               211
#define     MUSIC_VOLBK_PIC_XSIZE           20
#define     MUSIC_VOLBK_PIC_YSIZE           20
#define     MUSIC_VOLBK_PIC_SIZE            ((unsigned long)((unsigned long)(MUSIC_VOLBK_PIC_XSIZE) * MUSIC_VOLBK_PIC_YSIZE * 2))
#define     MUSIC_VOLBK_PIC_CTL             1
                                            
#define     MUSIC_VOLBK_PIC                 NFA_MUSIC_VOLBK_PIC

//--------------Vol Level--------------------
#define     NFA_MUSIC_VOLLEVEL_PIC          ((unsigned long)(NFA_MUSIC_VOLBK_PIC + ((unsigned long)(MUSIC_VOLBK_PIC_SIZE) * MUSIC_VOLBK_PIC_CTL)))
#define     MUSIC_VOLLEVEL_PIC_X            242
#define     MUSIC_VOLLEVEL_PIC_Y            168
#define     MUSIC_VOLLEVEL_PIC_XSIZE        70
#define     MUSIC_VOLLEVEL_PIC_YSIZE        70
#define     MUSIC_VOLLEVEL_PIC_SIZE         ((unsigned long)((unsigned long)(MUSIC_VOLLEVEL_PIC_XSIZE) * MUSIC_VOLLEVEL_PIC_YSIZE * 2))
#define     MUSIC_VOLLEVEL_PIC_CTL          41
                                            
#define     MUSIC_VOLLEVEL_PIC_00           NFA_MUSIC_VOLLEVEL_PIC
#define     MUSIC_VOLLEVEL_PIC_01           ((unsigned long)(MUSIC_VOLLEVEL_PIC_00  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_02           ((unsigned long)(MUSIC_VOLLEVEL_PIC_01  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_03           ((unsigned long)(MUSIC_VOLLEVEL_PIC_02  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_04           ((unsigned long)(MUSIC_VOLLEVEL_PIC_03  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_05           ((unsigned long)(MUSIC_VOLLEVEL_PIC_04  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_06           ((unsigned long)(MUSIC_VOLLEVEL_PIC_05  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_07           ((unsigned long)(MUSIC_VOLLEVEL_PIC_06  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_08           ((unsigned long)(MUSIC_VOLLEVEL_PIC_07  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_09           ((unsigned long)(MUSIC_VOLLEVEL_PIC_08  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_10           ((unsigned long)(MUSIC_VOLLEVEL_PIC_09  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_11           ((unsigned long)(MUSIC_VOLLEVEL_PIC_10  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_12           ((unsigned long)(MUSIC_VOLLEVEL_PIC_11  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_13           ((unsigned long)(MUSIC_VOLLEVEL_PIC_12  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_14           ((unsigned long)(MUSIC_VOLLEVEL_PIC_13  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_15           ((unsigned long)(MUSIC_VOLLEVEL_PIC_14  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_16           ((unsigned long)(MUSIC_VOLLEVEL_PIC_15  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_17           ((unsigned long)(MUSIC_VOLLEVEL_PIC_16  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_18           ((unsigned long)(MUSIC_VOLLEVEL_PIC_17  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_19           ((unsigned long)(MUSIC_VOLLEVEL_PIC_18  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_20           ((unsigned long)(MUSIC_VOLLEVEL_PIC_19  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_21           ((unsigned long)(MUSIC_VOLLEVEL_PIC_20  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_22           ((unsigned long)(MUSIC_VOLLEVEL_PIC_21  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_23           ((unsigned long)(MUSIC_VOLLEVEL_PIC_22  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_24           ((unsigned long)(MUSIC_VOLLEVEL_PIC_23  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_25           ((unsigned long)(MUSIC_VOLLEVEL_PIC_24  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_26           ((unsigned long)(MUSIC_VOLLEVEL_PIC_25  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_27           ((unsigned long)(MUSIC_VOLLEVEL_PIC_26  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_28           ((unsigned long)(MUSIC_VOLLEVEL_PIC_27  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_29           ((unsigned long)(MUSIC_VOLLEVEL_PIC_28  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_30           ((unsigned long)(MUSIC_VOLLEVEL_PIC_29  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_31           ((unsigned long)(MUSIC_VOLLEVEL_PIC_30  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_32           ((unsigned long)(MUSIC_VOLLEVEL_PIC_31  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_33           ((unsigned long)(MUSIC_VOLLEVEL_PIC_32  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_34           ((unsigned long)(MUSIC_VOLLEVEL_PIC_33  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_35           ((unsigned long)(MUSIC_VOLLEVEL_PIC_34  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_36           ((unsigned long)(MUSIC_VOLLEVEL_PIC_35  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_37           ((unsigned long)(MUSIC_VOLLEVEL_PIC_36  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_38           ((unsigned long)(MUSIC_VOLLEVEL_PIC_37  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_39           ((unsigned long)(MUSIC_VOLLEVEL_PIC_38  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))
#define     MUSIC_VOLLEVEL_PIC_40           ((unsigned long)(MUSIC_VOLLEVEL_PIC_39  + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE))))

/*
------------------------------------------------------------------------------------------
* ModuleName :  Vedio Module
* Author     :  tonyzyz
* Description:  Vedio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/
//--------Music BackGround Address-----------
#define     NFA_VIDEO_PIC                   ((unsigned long)(NFA_MUSIC_VOLLEVEL_PIC + ((unsigned long)(MUSIC_VOLLEVEL_PIC_SIZE) * MUSIC_VOLLEVEL_PIC_CTL)))
#define     VEDIO_BKGROUND_PIC              NFA_VIDEO_PIC

//-------------Up Main Window----------------
#define     NFA_VIDEO_UPMAIN_PIC            ((unsigned long)(NFA_VIDEO_PIC + ((unsigned long)(MAX_PIC_SIZE))))
#define     VIDEO_UPMAIN_PIC_X              0
#define     VIDEO_UPMAIN_PIC_Y              0
#define     VIDEO_UPMAIN_PIC_XSIZE          320
#define     VIDEO_UPMAIN_PIC_YSIZE          30
#define     VIDEO_UPMAIN_PIC_SIZE           ((unsigned long)((unsigned long)(VIDEO_UPMAIN_PIC_XSIZE) * VIDEO_UPMAIN_PIC_YSIZE * 2))
#define     VIDEO_UPMAIN_PIC_CTL            1

#define     VIDEO_UPMAIN_PIC                NFA_VIDEO_UPMAIN_PIC

//--------------Vol BackGround---------------
#define     NFA_VIDEO_VOLBK_PIC             ((unsigned long)(NFA_VIDEO_UPMAIN_PIC + ((unsigned long)(VIDEO_UPMAIN_PIC_SIZE) * VIDEO_UPMAIN_PIC_CTL)))
#define     VIDEO_VOLBK_PIC_X               31
#define     VIDEO_VOLBK_PIC_Y               7
#define     VIDEO_VOLBK_PIC_XSIZE           20
#define     VIDEO_VOLBK_PIC_YSIZE           16
#define     VIDEO_VOLBK_PIC_SIZE            ((unsigned long)((unsigned long)(VIDEO_VOLBK_PIC_XSIZE) * VIDEO_VOLBK_PIC_YSIZE * 2))
#define     VIDEO_VOLBK_PIC_CTL             1

#define     VIDEO_VOLBK_PIC                 NFA_VIDEO_VOLBK_PIC

//------------------Hold---------------------
#define     NFA_VIDEO_HOLD_PIC              ((unsigned long)(NFA_VIDEO_VOLBK_PIC + ((unsigned long)(VIDEO_VOLBK_PIC_SIZE) * VIDEO_VOLBK_PIC_CTL)))
#define     VIDEO_HOLD_PIC_X                240
#define     VIDEO_HOLD_PIC_Y                4
#define     VIDEO_HOLD_PIC_XSIZE            20
#define     VIDEO_HOLD_PIC_YSIZE            20
#define     VIDEO_HOLD_PIC_SIZE             ((unsigned long)((unsigned long)(VIDEO_HOLD_PIC_XSIZE) * VIDEO_HOLD_PIC_YSIZE * 2))
#define     VIDEO_HOLD_PIC_CTL              2

#define     VIDEO_HOLD_OFF_PIC              NFA_VIDEO_HOLD_PIC
#define     VIDEO_HOLD_ON_PIC               ((unsigned long)(NFA_VIDEO_HOLD_PIC + ((unsigned long)(VIDEO_HOLD_PIC_SIZE))))
//-----------------Battery-------------------
#define     NFA_VIDEO_BATT_PIC              ((unsigned long)(NFA_VIDEO_HOLD_PIC + ((unsigned long)(VIDEO_HOLD_PIC_SIZE) * VIDEO_HOLD_PIC_CTL)))
#define     VIDEO_BATT_PIC_X                266
#define     VIDEO_BATT_PIC_Y                4
#define     VIDEO_BATT_PIC_XSIZE            40
#define     VIDEO_BATT_PIC_YSIZE            20
#define     VIDEO_BATT_PIC_SIZE             ((unsigned long)((unsigned long)(VIDEO_BATT_PIC_XSIZE) * VIDEO_BATT_PIC_YSIZE * 2))
#define     VIDEO_BATT_PIC_CTL              4

#define     VIDEO_BATT_PIC_1                NFA_VIDEO_BATT_PIC
#define     VIDEO_BATT_PIC_2                ((unsigned long)(VIDEO_BATT_PIC_1   + ((unsigned long)(VIDEO_BATT_PIC_SIZE))))
#define     VIDEO_BATT_PIC_3                ((unsigned long)(VIDEO_BATT_PIC_2   + ((unsigned long)(VIDEO_BATT_PIC_SIZE))))
#define     VIDEO_BATT_PIC_4                ((unsigned long)(VIDEO_BATT_PIC_3   + ((unsigned long)(VIDEO_BATT_PIC_SIZE))))

//-------------Down Main Window----------------
#define     NFA_VIDEO_DOWNMAIN_PIC          ((unsigned long)(NFA_VIDEO_BATT_PIC + ((unsigned long)(VIDEO_BATT_PIC_SIZE) * VIDEO_BATT_PIC_CTL)))
#define     VIDEO_DOWNMAIN_PIC_X            0
#define     VIDEO_DOWNMAIN_PIC_Y            200
#define     VIDEO_DOWNMAIN_PIC_XSIZE        320
#define     VIDEO_DOWNMAIN_PIC_YSIZE        40
#define     VIDEO_DOWNMAIN_PIC_SIZE         ((unsigned long)((unsigned long)(VIDEO_DOWNMAIN_PIC_XSIZE) * VIDEO_DOWNMAIN_PIC_YSIZE * 2))
#define     VIDEO_DOWNMAIN_PIC_CTL          1
                                            
#define     VIDEO_DOWNMAIN_PIC              NFA_VIDEO_DOWNMAIN_PIC

//----------------Play Status----------------
#define     NFA_VIDEO_PLAY_STATUS_PIC       ((unsigned long)(NFA_VIDEO_DOWNMAIN_PIC+ ((unsigned long)(VIDEO_DOWNMAIN_PIC_SIZE) * VIDEO_DOWNMAIN_PIC_CTL)))
#define     VIDEO_PLAY_STATUS_PIC_X         5
#define     VIDEO_PLAY_STATUS_PIC_Y         203
#define     VIDEO_PLAY_STATUS_PIC_XSIZE     35
#define     VIDEO_PLAY_STATUS_PIC_YSIZE     35
#define     VIDEO_PLAY_STATUS_PIC_SIZE      ((unsigned long)((unsigned long)(VIDEO_PLAY_STATUS_PIC_XSIZE) * VIDEO_PLAY_STATUS_PIC_YSIZE * 2))
#define     VIDEO_PLAY_STATUS_PIC_CTL       3
                                            
#define     VIDEO_STATUS_PLAY_PIC           NFA_VIDEO_PLAY_STATUS_PIC
#define     VIDEO_STATUS_PAUSE_PIC          ((unsigned long)(VIDEO_STATUS_PLAY_PIC   + ((unsigned long)(VIDEO_PLAY_STATUS_PIC_SIZE))))
#define     VIDEO_STATUS_STOP_PIC           ((unsigned long)(VIDEO_STATUS_PAUSE_PIC  + ((unsigned long)(VIDEO_PLAY_STATUS_PIC_SIZE))))
#define     VIDEO_STATUS_FFD_PIC            ((unsigned long)(VIDEO_STATUS_STOP_PIC   + ((unsigned long)(VIDEO_PLAY_STATUS_PIC_SIZE))))
#define     VIDEO_STATUS_FFW_PIC            ((unsigned long)(VIDEO_STATUS_FFD_PIC    + ((unsigned long)(VIDEO_PLAY_STATUS_PIC_SIZE))))

//------------------Guage--------------------
#define     NFA_VIDEO_GUAGE_PIC             ((unsigned long)(NFA_VIDEO_PLAY_STATUS_PIC  + ((unsigned long)(VIDEO_PLAY_STATUS_PIC_SIZE) * VIDEO_PLAY_STATUS_PIC_CTL)))
#define     VIDEO_GUAGE_PIC_X               56
#define     VIDEO_GUAGE_PIC_Y               211
#define     VIDEO_GUAGE_PIC_XSIZE           1       
#define     VIDEO_GUAGE_PIC_YSIZE           8
#define     VIDEO_GUAGE_PIC_SIZE            ((unsigned long)((unsigned long)(VIDEO_GUAGE_PIC_XSIZE) * VIDEO_GUAGE_PIC_YSIZE * 2))
#define     VIDEO_GUAGE_PIC_CTL             2
#define     VIDEO_GUAGE_BKGROUND_PIC        NFA_VIDEO_GUAGE_PIC
#define     VIDEO_GUAGE_FRONT_PIC           ((unsigned long)(VIDEO_GUAGE_BKGROUND_PIC   + ((unsigned long)(VIDEO_GUAGE_PIC_SIZE))))

#define     NFA_VIDEO_GUAGEBLOCK_PIC        ((unsigned long)(NFA_VIDEO_GUAGE_PIC  + ((unsigned long)(VIDEO_GUAGE_PIC_SIZE) * VIDEO_GUAGE_PIC_CTL)))
#define     VIDEO_GUAGEBLOCK_PIC_X          56
#define     VIDEO_GUAGEBLOCK_PIC_Y          211
#define     VIDEO_GUAGEBLOCK_PIC_XSIZE      8       
#define     VIDEO_GUAGEBLOCK_PIC_YSIZE      8
#define     VIDEO_GUAGEBLOCK_PIC_SIZE       ((unsigned long)((unsigned long)(VIDEO_GUAGEBLOCK_PIC_XSIZE) * VIDEO_GUAGEBLOCK_PIC_YSIZE * 2))
#define     VIDEO_GUAGEBLOCK_PIC_CTL        1
#define     VIDEO_GUAGE_BLOCK_PIC           NFA_VIDEO_GUAGEBLOCK_PIC

//-----------Current Time BackGround---------
#define     NFA_VIDEO_TIMECURRBK_PIC        ((unsigned long)(NFA_VIDEO_GUAGEBLOCK_PIC + ((unsigned long)(VIDEO_GUAGEBLOCK_PIC_SIZE) * VIDEO_GUAGEBLOCK_PIC_CTL)))
#define     VIDEO_TIMECURRBK_PIC_X          46
#define     VIDEO_TIMECURRBK_PIC_Y          222
#define     VIDEO_TIMECURRBK_PIC_XSIZE      72
#define     VIDEO_TIMECURRBK_PIC_YSIZE      16
#define     VIDEO_TIMECURRBK_PIC_SIZE       ((unsigned long)((unsigned long)(VIDEO_TIMECURRBK_PIC_XSIZE) * VIDEO_TIMECURRBK_PIC_YSIZE * 2))
#define     VIDEO_TIMECURRBK_PIC_CTL        1
                                            
#define     VIDEO_TIMECURRBK_PIC            NFA_VIDEO_TIMECURRBK_PIC

//------------Total Time BackGround----------
#define     NFA_VIDEO_TIMETOTALBK_PIC       ((unsigned long)(NFA_VIDEO_TIMECURRBK_PIC   + ((unsigned long)(VIDEO_TIMECURRBK_PIC_SIZE) * VIDEO_TIMECURRBK_PIC_CTL)))
#define     VIDEO_TIMETOTALBK_PIC_X         237
#define     VIDEO_TIMETOTALBK_PIC_Y         222
#define     VIDEO_TIMETOTALBK_PIC_XSIZE     72
#define     VIDEO_TIMETOTALBK_PIC_YSIZE     16
#define     VIDEO_TIMETOTALBK_PIC_SIZE      ((unsigned long)((unsigned long)(VIDEO_TIMETOTALBK_PIC_XSIZE) * VIDEO_TIMETOTALBK_PIC_YSIZE * 2))
#define     VIDEO_TIMETOTALBK_PIC_CTL       1
                                            
#define     VIDEO_TIMETOTALBK_PIC           NFA_VIDEO_TIMETOTALBK_PIC

/*
------------------------------------------------------------------------------------------
* ModuleName :  Radio Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/
//--------Radio BackGround Address-----------
#define     NFA_RADIO_PIC                   ((unsigned long)(NFA_VIDEO_TIMETOTALBK_PIC + ((unsigned long)(VIDEO_TIMETOTALBK_PIC_SIZE) * VIDEO_TIMETOTALBK_PIC_CTL)))
#define     RADIO_BKGROUND_PIC              NFA_RADIO_PIC

//--------------Sensitivity------------------
#define     NFA_RADIO_SEN_PIC               ((unsigned long)(NFA_RADIO_PIC + ((unsigned long)(MAX_PIC_SIZE))))
#define     RADIO_SEN_PIC_X                 0
#define     RADIO_SEN_PIC_Y                 0
#define     RADIO_SEN_PIC_XSIZE             0
#define     RADIO_SEN_PIC_YSIZE             0
#define     RADIO_SEN_PIC_SIZE              ((unsigned long)((unsigned long)(RADIO_SEN_PIC_XSIZE) * RADIO_SEN_PIC_YSIZE * 2))
#define     RADIO_SEN_PIC_CTL               4
                                            
#define     RADIO_SENSITIVITY_PIC_1         NFA_RADIO_SEN_PIC
#define     RADIO_SENSITIVITY_PIC_2         ((unsigned long)(RADIO_SENSITIVITY_PIC_1     + ((unsigned long)(RADIO_SEN_PIC_SIZE))))
#define     RADIO_SENSITIVITY_PIC_3         ((unsigned long)(RADIO_SENSITIVITY_PIC_2     + ((unsigned long)(RADIO_SEN_PIC_SIZE))))
#define     RADIO_SENSITIVITY_PIC_4         ((unsigned long)(RADIO_SENSITIVITY_PIC_3     + ((unsigned long)(RADIO_SEN_PIC_SIZE))))

//---------------Stereo/Mono-----------------
#define     NFA_RADIO_SOUND_PIC             ((unsigned long)(NFA_RADIO_SEN_PIC + ((unsigned long)(RADIO_SEN_PIC_SIZE) * RADIO_SEN_PIC_CTL)))
#define     RADIO_SOUND_PIC_X               0
#define     RADIO_SOUND_PIC_Y               0
#define     RADIO_SOUND_PIC_XSIZE           0
#define     RADIO_SOUND_PIC_YSIZE           0
#define     RADIO_SOUND_PIC_SIZE            ((unsigned long)((unsigned long)(RADIO_SOUND_PIC_XSIZE) * RADIO_SOUND_PIC_YSIZE * 2))
#define     RADIO_SOUND_PIC_CTL             2
                                            
#define     RADIO_SOUND_MONO_PIC            NFA_RADIO_SOUND_PIC
#define     RADIO_SOUND_STEREO_PIC          ((unsigned long)(RADIO_SOUND_MONO_PIC   + ((unsigned long)(RADIO_SOUND_PIC_SIZE))))

//------------------Hold---------------------
#define     NFA_RADIO_HOLD_PIC              ((unsigned long)(NFA_RADIO_SOUND_PIC    + ((unsigned long)(RADIO_SOUND_PIC_SIZE) * RADIO_SOUND_PIC_CTL)))
#define     RADIO_HOLD_PIC_X                242
#define     RADIO_HOLD_PIC_Y                4
#define     RADIO_HOLD_PIC_XSIZE            20
#define     RADIO_HOLD_PIC_YSIZE            20
#define     RADIO_HOLD_PIC_SIZE             ((unsigned long)((unsigned long)(RADIO_HOLD_PIC_XSIZE) * RADIO_HOLD_PIC_YSIZE * 2))
#define     RADIO_HOLD_PIC_CTL              2

#define     RADIO_HOLD_OFF_PIC              NFA_RADIO_HOLD_PIC
#define     RADIO_HOLD_ON_PIC               ((unsigned long)(VIDEO_HOLD_OFF_PIC + ((unsigned long)(RADIO_HOLD_PIC_SIZE))))

//-----------------Battery-------------------
#define     NFA_RADIO_BATT_PIC              ((unsigned long)(NFA_RADIO_HOLD_PIC + ((unsigned long)(RADIO_HOLD_PIC_SIZE) * RADIO_HOLD_PIC_CTL)))
#define     RADIO_BATT_PIC_X                265
#define     RADIO_BATT_PIC_Y                4
#define     RADIO_BATT_PIC_XSIZE            40
#define     RADIO_BATT_PIC_YSIZE            20
#define     RADIO_BATT_PIC_SIZE             ((unsigned long)((unsigned long)(RADIO_BATT_PIC_XSIZE) * RADIO_BATT_PIC_YSIZE * 2))
#define     RADIO_BATT_PIC_CTL              4

#define     RADIO_BATT_PIC_1                NFA_RADIO_BATT_PIC
#define     RADIO_BATT_PIC_2                ((unsigned long)(RADIO_BATT_PIC_1   + ((unsigned long)(RADIO_BATT_PIC_SIZE))))
#define     RADIO_BATT_PIC_3                ((unsigned long)(RADIO_BATT_PIC_2   + ((unsigned long)(RADIO_BATT_PIC_SIZE))))
#define     RADIO_BATT_PIC_4                ((unsigned long)(RADIO_BATT_PIC_3   + ((unsigned long)(RADIO_BATT_PIC_SIZE))))

//------------------Freq---------------------
#define     NFA_RADIO_FREQNUM_PIC           ((unsigned long)(NFA_RADIO_BATT_PIC + ((unsigned long)(RADIO_BATT_PIC_SIZE) * RADIO_BATT_PIC_CTL)))
#define     RADIO_FREQNUM_PIC_X             87
#define     RADIO_FREQNUM_PIC_Y             76
#define     RADIO_FREQNUM_PIC_XSIZE         24
#define     RADIO_FREQNUM_PIC_YSIZE         40
#define     RADIO_FREQNUM_PIC_SIZE          ((unsigned long)((unsigned long)(RADIO_FREQNUM_PIC_XSIZE) * RADIO_FREQNUM_PIC_YSIZE * 2))
#define     RADIO_FREQNUM_PIC_CTL           11

#define     RADIO_FREQNUM_0_PIC             NFA_RADIO_FREQNUM_PIC
#define     RADIO_FREQNUM_1_PIC             ((unsigned long)(RADIO_FREQNUM_0_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_2_PIC             ((unsigned long)(RADIO_FREQNUM_1_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_3_PIC             ((unsigned long)(RADIO_FREQNUM_2_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_4_PIC             ((unsigned long)(RADIO_FREQNUM_3_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_5_PIC             ((unsigned long)(RADIO_FREQNUM_4_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_6_PIC             ((unsigned long)(RADIO_FREQNUM_5_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_7_PIC             ((unsigned long)(RADIO_FREQNUM_6_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_8_PIC             ((unsigned long)(RADIO_FREQNUM_7_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_9_PIC             ((unsigned long)(RADIO_FREQNUM_8_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))
#define     RADIO_FREQNUM_BK_PIC            ((unsigned long)(RADIO_FREQNUM_9_PIC    + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE))))

#define     NFA_RADIO_FREQDOT_PIC           ((unsigned long)(NFA_RADIO_FREQNUM_PIC + ((unsigned long)(RADIO_FREQNUM_PIC_SIZE) * RADIO_FREQNUM_PIC_CTL)))
#define     RADIO_FREQDOT_PIC_X             154
#define     RADIO_FREQDOT_PIC_Y             76
#define     RADIO_FREQDOT_PIC_XSIZE         12
#define     RADIO_FREQDOT_PIC_YSIZE         40
#define     RADIO_FREQDOT_PIC_SIZE          ((unsigned long)((unsigned long)(RADIO_FREQDOT_PIC_XSIZE) * RADIO_FREQDOT_PIC_YSIZE * 2))
#define     RADIO_FREQDOT_PIC_CTL           1

#define     RADIO_FREQDOT_PIC               NFA_RADIO_FREQDOT_PIC

//------------------Guage--------------------
#define     NFA_RADIO_GUAGEBK_PIC           ((unsigned long)(NFA_RADIO_FREQDOT_PIC + ((unsigned long)(RADIO_FREQDOT_PIC_SIZE) * RADIO_FREQDOT_PIC_CTL)))
#define     RADIO_GUAGEBK_PIC_X             18
#define     RADIO_GUAGEBK_PIC_Y             188
#define     RADIO_GUAGEBK_PIC_XSIZE         21   
#define     RADIO_GUAGEBK_PIC_YSIZE         13
#define     RADIO_GUAGEBK_PIC_SIZE          ((unsigned long)((unsigned long)(RADIO_GUAGEBK_PIC_XSIZE) * RADIO_GUAGEBK_PIC_YSIZE * 2))
#define     RADIO_GUAGEBK_PIC_CTL           1
#define     RADIO_GUAGE_BKGROUND_PIC        NFA_RADIO_GUAGEBK_PIC
                                            
#define     NFA_RADIO_GUAGEFRONT_PIC        ((unsigned long)(NFA_RADIO_GUAGEBK_PIC + ((unsigned long)(RADIO_GUAGEBK_PIC_SIZE) * RADIO_GUAGEBK_PIC_CTL)))
#define     RADIO_GUAGEFRONT_PIC_X          18
#define     RADIO_GUAGEFRONT_PIC_Y          188
#define     RADIO_GUAGEFRONT_PIC_XSIZE      21
#define     RADIO_GUAGEFRONT_PIC_YSIZE      13
#define     RADIO_GUAGEFRONT_PIC_SIZE       ((unsigned long)((unsigned long)(RADIO_GUAGEFRONT_PIC_XSIZE) * RADIO_GUAGEFRONT_PIC_YSIZE * 2))
#define     RADIO_GUAGEFRONT_PIC_CTL        1
#define     RADIO_GUAGE_FRONT_PIC           NFA_RADIO_GUAGEFRONT_PIC

//-----------------Region--------------------
#define     NFA_RADIO_REGIONBK_PIC          ((unsigned long)(NFA_RADIO_GUAGEFRONT_PIC + ((unsigned long)(RADIO_GUAGEFRONT_PIC_SIZE) * RADIO_GUAGEFRONT_PIC_CTL)))
#define     RADIO_REGIONBK_PIC_X            24
#define     RADIO_REGIONBK_PIC_Y            213
#define     RADIO_REGIONBK_PIC_XSIZE        64
#define     RADIO_REGIONBK_PIC_YSIZE        20
#define     RADIO_REGIONBK_PIC_SIZE         ((unsigned long)((unsigned long)(RADIO_REGIONBK_PIC_XSIZE) * RADIO_REGIONBK_PIC_YSIZE * 2))
#define     RADIO_REGIONBK_PIC_CTL          1

#define     RADIO_REGIONBK_PIC              NFA_RADIO_REGIONBK_PIC

#define     NFA_RADIO_REGIONBK_START_PIC    ((unsigned long)(NFA_RADIO_REGIONBK_PIC + ((unsigned long)(RADIO_REGIONBK_PIC_SIZE) * RADIO_REGIONBK_PIC_CTL)))
#define     RADIO_REGIONBK_START_PIC_X      10
#define     RADIO_REGIONBK_START_PIC_Y      162
#define     RADIO_REGIONBK_START_PIC_XSIZE  64
#define     RADIO_REGIONBK_START_PIC_YSIZE  16
#define     RADIO_REGIONBK_START_PIC_SIZE   ((unsigned long)((unsigned long)(RADIO_REGIONBK_START_PIC_XSIZE) * RADIO_REGIONBK_START_PIC_YSIZE * 2))
#define     RADIO_REGIONBK_START_PIC_CTL    1
#define     RADIO_REGIONBK_START_PIC        NFA_RADIO_REGIONBK_START_PIC

#define     NFA_RADIO_REGIONBK_END_PIC      ((unsigned long)(NFA_RADIO_REGIONBK_START_PIC + ((unsigned long)(RADIO_REGIONBK_START_PIC_SIZE) * RADIO_REGIONBK_START_PIC_CTL)))
#define     RADIO_REGIONBK_END_PIC_X        154
#define     RADIO_REGIONBK_END_PIC_Y        162
#define     RADIO_REGIONBK_END_PIC_XSIZE    64
#define     RADIO_REGIONBK_END_PIC_YSIZE    16
#define     RADIO_REGIONBK_END_PIC_SIZE     ((unsigned long)((unsigned long)(RADIO_REGIONBK_END_PIC_XSIZE) * RADIO_REGIONBK_END_PIC_YSIZE * 2))
#define     RADIO_REGIONBK_END_PIC_CTL      1
#define     RADIO_REGIONBK_END_PIC          NFA_RADIO_REGIONBK_END_PIC
//-----------------Preset--------------------
#define     NFA_RADIO_PRESETBK_PIC          ((unsigned long)(NFA_RADIO_REGIONBK_END_PIC + ((unsigned long)(RADIO_REGIONBK_END_PIC_SIZE) * RADIO_REGIONBK_END_PIC_CTL)))
#define     RADIO_PRESETBK_PIC_X            108
#define     RADIO_PRESETBK_PIC_Y            213
#define     RADIO_PRESETBK_PIC_XSIZE        96
#define     RADIO_PRESETBK_PIC_YSIZE        16
#define     RADIO_PRESETBK_PIC_SIZE         ((unsigned long)((unsigned long)(RADIO_PRESETBK_PIC_XSIZE) * RADIO_PRESETBK_PIC_YSIZE * 2))
#define     RADIO_PRESETBK_PIC_CTL          1

#define     RADIO_PRESETBK_PIC              NFA_RADIO_PRESETBK_PIC

//--------------Vol BackGround---------------
#define     NFA_RADIO_VOLBK_PIC             ((unsigned long)(NFA_RADIO_PRESETBK_PIC + ((unsigned long)(RADIO_PRESETBK_PIC_SIZE) * RADIO_PRESETBK_PIC_CTL)))
#define     RADIO_VOLBK_PIC_X               216
#define     RADIO_VOLBK_PIC_Y               211
#define     RADIO_VOLBK_PIC_XSIZE           20
#define     RADIO_VOLBK_PIC_YSIZE           20
#define     RADIO_VOLBK_PIC_SIZE            ((unsigned long)((unsigned long)(RADIO_VOLBK_PIC_XSIZE) * RADIO_VOLBK_PIC_YSIZE * 2))
#define     RADIO_VOLBK_PIC_CTL             1

#define     RADIO_VOLBK_PIC                 NFA_RADIO_VOLBK_PIC

//--------------Vol Level--------------------
#define     NFA_RADIO_VOLLEVEL_PIC          ((unsigned long)(NFA_RADIO_VOLBK_PIC + ((unsigned long)(RADIO_VOLBK_PIC_SIZE) * RADIO_VOLBK_PIC_CTL)))
#define     RADIO_VOLLEVEL_PIC_X            242
#define     RADIO_VOLLEVEL_PIC_Y            168
#define     RADIO_VOLLEVEL_PIC_XSIZE        70
#define     RADIO_VOLLEVEL_PIC_YSIZE        70
#define     RADIO_VOLLEVEL_PIC_SIZE         ((unsigned long)((unsigned long)(RADIO_VOLLEVEL_PIC_XSIZE) * RADIO_VOLLEVEL_PIC_YSIZE * 2))
#define     RADIO_VOLLEVEL_PIC_CTL          41
                                            
#define     RADIO_VOLLEVEL_PIC_00           NFA_RADIO_VOLLEVEL_PIC
#define     RADIO_VOLLEVEL_PIC_01           ((unsigned long)(RADIO_VOLLEVEL_PIC_00  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_02           ((unsigned long)(RADIO_VOLLEVEL_PIC_01  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_03           ((unsigned long)(RADIO_VOLLEVEL_PIC_02  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_04           ((unsigned long)(RADIO_VOLLEVEL_PIC_03  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_05           ((unsigned long)(RADIO_VOLLEVEL_PIC_04  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_06           ((unsigned long)(RADIO_VOLLEVEL_PIC_05  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_07           ((unsigned long)(RADIO_VOLLEVEL_PIC_06  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_08           ((unsigned long)(RADIO_VOLLEVEL_PIC_07  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_09           ((unsigned long)(RADIO_VOLLEVEL_PIC_08  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_10           ((unsigned long)(RADIO_VOLLEVEL_PIC_09  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_11           ((unsigned long)(RADIO_VOLLEVEL_PIC_10  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_12           ((unsigned long)(RADIO_VOLLEVEL_PIC_11  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_13           ((unsigned long)(RADIO_VOLLEVEL_PIC_12  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_14           ((unsigned long)(RADIO_VOLLEVEL_PIC_13  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_15           ((unsigned long)(RADIO_VOLLEVEL_PIC_14  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_16           ((unsigned long)(RADIO_VOLLEVEL_PIC_15  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_17           ((unsigned long)(RADIO_VOLLEVEL_PIC_16  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_18           ((unsigned long)(RADIO_VOLLEVEL_PIC_17  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_19           ((unsigned long)(RADIO_VOLLEVEL_PIC_18  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_20           ((unsigned long)(RADIO_VOLLEVEL_PIC_19  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_21           ((unsigned long)(RADIO_VOLLEVEL_PIC_20  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_22           ((unsigned long)(RADIO_VOLLEVEL_PIC_21  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_23           ((unsigned long)(RADIO_VOLLEVEL_PIC_22  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_24           ((unsigned long)(RADIO_VOLLEVEL_PIC_23  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_25           ((unsigned long)(RADIO_VOLLEVEL_PIC_24  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_26           ((unsigned long)(RADIO_VOLLEVEL_PIC_25  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_27           ((unsigned long)(RADIO_VOLLEVEL_PIC_26  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_28           ((unsigned long)(RADIO_VOLLEVEL_PIC_27  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_29           ((unsigned long)(RADIO_VOLLEVEL_PIC_28  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_30           ((unsigned long)(RADIO_VOLLEVEL_PIC_29  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_31           ((unsigned long)(RADIO_VOLLEVEL_PIC_30  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_32           ((unsigned long)(RADIO_VOLLEVEL_PIC_31  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_33           ((unsigned long)(RADIO_VOLLEVEL_PIC_32  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_34           ((unsigned long)(RADIO_VOLLEVEL_PIC_33  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_35           ((unsigned long)(RADIO_VOLLEVEL_PIC_34  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_36           ((unsigned long)(RADIO_VOLLEVEL_PIC_35  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_37           ((unsigned long)(RADIO_VOLLEVEL_PIC_36  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_38           ((unsigned long)(RADIO_VOLLEVEL_PIC_37  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_39           ((unsigned long)(RADIO_VOLLEVEL_PIC_38  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))
#define     RADIO_VOLLEVEL_PIC_40           ((unsigned long)(RADIO_VOLLEVEL_PIC_39  + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE))))

//-------------Subenu BackGround-------------
#define     NFA_RADIO_SUBMENUBK_PIC         ((unsigned long)(NFA_RADIO_VOLLEVEL_PIC + ((unsigned long)(RADIO_VOLLEVEL_PIC_SIZE) * RADIO_VOLLEVEL_PIC_CTL)))
#define     RADIO_SUBMENUBK_PIC_X           0
#define     RADIO_SUBMENUBK_PIC_Y           40
#define     RADIO_SUBMENUBK_PIC_XSIZE       73
#define     RADIO_SUBMENUBK_PIC_YSIZE       122
#define     RADIO_SUBMENUBK_PIC_SIZE        ((unsigned long)((unsigned long)(RADIO_SUBMENUBK_PIC_XSIZE) * RADIO_SUBMENUBK_PIC_YSIZE * 2))
#define     RADIO_SUBMENUBK_PIC_CTL         1
                                            
#define     RADIO_SUBMENUBK_PIC             NFA_RADIO_SUBMENUBK_PIC

//-------------Submenu Arrow-----------------
#define     NFA_RADIO_SUBMENU_ARROW_PIC     ((unsigned long)(NFA_RADIO_SUBMENUBK_PIC + ((unsigned long)(RADIO_SUBMENUBK_PIC_SIZE) * RADIO_SUBMENUBK_PIC_CTL)))
#define     RADIO_SUBMENU_ARROW_PIC_X       58
#define     RADIO_SUBMENU_ARROW_PIC_Y       94
#define     RADIO_SUBMENU_ARROW_PIC_XSIZE   12
#define     RADIO_SUBMENU_ARROW_PIC_YSIZE   12
#define     RADIO_SUBMENU_ARROW_PIC_SIZE    ((unsigned long)((unsigned long)(RADIO_SUBMENU_ARROW_PIC_XSIZE) * RADIO_SUBMENU_ARROW_PIC_YSIZE * 2))
#define     RADIO_SUBMENU_ARROW_PIC_CTL     2
                                            
#define     RADIO_SUBMENU_ARROW_ARROWBK     NFA_RADIO_SUBMENU_ARROW_PIC
#define     RADIO_SUBMENU_ARROW_ARROW       ((unsigned long)(RADIO_SUBMENU_ARROW_ARROWBK  + ((unsigned long)(RADIO_SUBMENU_ARROW_PIC_SIZE))))
//----------------Preset List----------------
#define     NFA_RADIO_LISTBK_PIC            ((unsigned long)(NFA_RADIO_SUBMENU_ARROW_PIC + ((unsigned long)(RADIO_SUBMENU_ARROW_PIC_SIZE) * RADIO_SUBMENU_ARROW_PIC_CTL)))
#define     RADIO_LISTBK_PIC_X              73
#define     RADIO_LISTBK_PIC_Y              50
#define     RADIO_LISTBK_PIC_XSIZE          116
#define     RADIO_LISTBK_PIC_YSIZE          105
#define     RADIO_LISTBK_PIC_SIZE           ((unsigned long)((unsigned long)(RADIO_LISTBK_PIC_XSIZE) * RADIO_LISTBK_PIC_YSIZE * 2))
#define     RADIO_LISTBK_PIC_CTL            1
                                            
#define     RADIO_LISTBK_PIC                NFA_RADIO_LISTBK_PIC

//-----------Preset List UpIcon--------------
#define     NFA_RADIO_LIST_UPICON_PIC       ((unsigned long)(NFA_RADIO_LISTBK_PIC + ((unsigned long)(RADIO_LISTBK_PIC_SIZE) * RADIO_LISTBK_PIC_CTL)))
#define     RADIO_LIST_UPICON_PIC_X         124
#define     RADIO_LIST_UPICON_PIC_Y         52
#define     RADIO_LIST_UPICON_PIC_XSIZE     8
#define     RADIO_LIST_UPICON_PIC_YSIZE     8
#define     RADIO_LIST_UPICON_PIC_SIZE      ((unsigned long)((unsigned long)(RADIO_LIST_UPICON_PIC_XSIZE) * RADIO_LIST_UPICON_PIC_YSIZE * 2))
#define     RADIO_LIST_UPICON_PIC_CTL       2
                                            
#define     RADIO_LIST_UPICON_ARROWBK       NFA_RADIO_LIST_UPICON_PIC
#define     RADIO_LIST_UPICON_ARROW         ((unsigned long)(RADIO_LIST_UPICON_ARROWBK  + ((unsigned long)(RADIO_LIST_UPICON_PIC_SIZE))))

//-----------Preset List DownIcon------------
#define     NFA_RADIO_LIST_DOWNICON_PIC     ((unsigned long)(NFA_RADIO_LIST_UPICON_PIC + ((unsigned long)(RADIO_LIST_UPICON_PIC_SIZE) * RADIO_LIST_UPICON_PIC_CTL)))
#define     RADIO_LIST_DOWNICON_PIC_X       124
#define     RADIO_LIST_DOWNICON_PIC_Y       141
#define     RADIO_LIST_DOWNICON_PIC_XSIZE   8
#define     RADIO_LIST_DOWNICON_PIC_YSIZE   8
#define     RADIO_LIST_DOWNICON_PIC_SIZE    ((unsigned long)((unsigned long)(RADIO_LIST_DOWNICON_PIC_XSIZE) * RADIO_LIST_DOWNICON_PIC_YSIZE * 2))
#define     RADIO_LIST_DOWNICON_PIC_CTL     2
                                            
#define     RADIO_LIST_DOWNICON_ARROWBK     NFA_RADIO_LIST_DOWNICON_PIC
#define     RADIO_LIST_DOWNICON_ARROW       ((unsigned long)(RADIO_LIST_DOWNICON_ARROWBK  + ((unsigned long)(RADIO_LIST_DOWNICON_PIC_SIZE))))

/*
------------------------------------------------------------------------------------------
* ModuleName :  Photo Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-26       1.0           ORG
------------------------------------------------------------------------------------------
*/
#define     NAF_PHOTO_PIC                   ((unsigned long)(NFA_RADIO_LIST_DOWNICON_PIC + ((unsigned long)(RADIO_LIST_DOWNICON_PIC_SIZE) * RADIO_LIST_DOWNICON_PIC_CTL)))

/*
------------------------------------------------------------------------------------------
* ModuleName :  Text Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-26       1.0           ORG
------------------------------------------------------------------------------------------
*/
//--------Record BackGround Address-----------
#define     NFA_TEXT_PIC                    ((unsigned long)(NFA_RADIO_LIST_DOWNICON_PIC + ((unsigned long)(RADIO_LIST_DOWNICON_PIC_SIZE) * RADIO_LIST_DOWNICON_PIC_CTL)))
#define     TEXT_BKGROUND_PIC               NFA_TEXT_PIC

//------------------Hold---------------------
#define     NFA_TEXT_HOLD_PIC               ((unsigned long)(NFA_TEXT_PIC + ((unsigned long)(MAX_PIC_SIZE))))
#define     TEXT_HOLD_PIC_X                 10
#define     TEXT_HOLD_PIC_Y                 0
#define     TEXT_HOLD_PIC_XSIZE             40
#define     TEXT_HOLD_PIC_YSIZE             37
#define     TEXT_HOLD_PIC_SIZE              ((unsigned long)((unsigned long)(TEXT_HOLD_PIC_XSIZE) * TEXT_HOLD_PIC_YSIZE * 2))
#define     TEXT_HOLD_PIC_CTL               2
                                            
#define     TEXT_HOLD_OFF_PIC               NFA_TEXT_HOLD_PIC
#define     TEXT_HOLD_ON_PIC                ((unsigned long)(TEXT_HOLD_OFF_PIC + ((unsigned long)(TEXT_HOLD_PIC_SIZE))))

//-----------------Battery-------------------
#define     NFA_TEXT_BATT_PIC               ((unsigned long)(NFA_TEXT_HOLD_PIC  + ((unsigned long)(TEXT_HOLD_PIC_SIZE) * TEXT_HOLD_PIC_CTL)))
#define     TEXT_BATT_PIC_X                 262
#define     TEXT_BATT_PIC_Y                 12
#define     TEXT_BATT_PIC_XSIZE             40
#define     TEXT_BATT_PIC_YSIZE             20
#define     TEXT_BATT_PIC_SIZE              ((unsigned long)((unsigned long)(TEXT_BATT_PIC_XSIZE) * TEXT_BATT_PIC_YSIZE * 2))
#define     TEXT_BATT_PIC_CTL               4
                                            
#define     TEXT_BATT_PIC_1                 NFA_TEXT_BATT_PIC
#define     TEXT_BATT_PIC_2                 ((unsigned long)(TEXT_BATT_PIC_1    + ((unsigned long)(TEXT_BATT_PIC_SIZE))))
#define     TEXT_BATT_PIC_3                 ((unsigned long)(TEXT_BATT_PIC_2    + ((unsigned long)(TEXT_BATT_PIC_SIZE))))
#define     TEXT_BATT_PIC_4                 ((unsigned long)(TEXT_BATT_PIC_3    + ((unsigned long)(TEXT_BATT_PIC_SIZE))))

//---------------Text Scroll-----------------
#define     NFA_TEXT_SCROLL_PIC             ((unsigned long)(NFA_TEXT_BATT_PIC  + ((unsigned long)(TEXT_BATT_PIC_SIZE) * TEXT_BATT_PIC_CTL)))
#define     TEXT_SCROLL_PIC_X               312
#define     TEXT_SCROLL_PIC_Y               60
#define     TEXT_SCROLL_PIC_XSIZE           5
#define     TEXT_SCROLL_PIC_YSIZE           21
#define     TEXT_SCROLL_PIC_SIZE            ((unsigned long)((unsigned long)(TEXT_SCROLL_PIC_XSIZE) * TEXT_SCROLL_PIC_YSIZE * 2))
#define     TEXT_SCROLL_PIC_CTL             2

#define     TEXT_SCROLL_BACKGROUND_PIC      NFA_TEXT_SCROLL_PIC
#define     TEXT_SCROLL_FOREROUND_PIC       ((unsigned long)(TEXT_SCROLL_BACKGROUND_PIC + ((unsigned long)(TEXT_SCROLL_PIC_SIZE))))

//-------------Text Area BackGround----------
#define     NFA_TEXT_AREABK_PIC             ((unsigned long)(NFA_TEXT_SCROLL_PIC + ((unsigned long)(TEXT_SCROLL_PIC_SIZE) * TEXT_SCROLL_PIC_CTL)))
#define     TEXT_AREABK_PIC_X               10
#define     TEXT_AREABK_PIC_Y               66
#define     TEXT_AREABK_PIC_XSIZE           296
#define     TEXT_AREABK_PIC_YSIZE           172
#define     TEXT_AREABK_PIC_SIZE            ((unsigned long)((unsigned long)(TEXT_AREABK_PIC_XSIZE) * TEXT_AREABK_PIC_YSIZE * 2))
#define     TEXT_AREABK_PIC_CTL             1
                                                
#define     TEXT_AREABK_PIC                 NFA_TEXT_AREABK_PIC

//-----------Text BookMark BackGround--------
#define     NFA_TEXT_BOOKMARK_PIC           ((unsigned long)(NFA_TEXT_AREABK_PIC + ((unsigned long)(TEXT_AREABK_PIC_SIZE) * TEXT_AREABK_PIC_CTL)))
#define     TEXT_BOOKMARK_PIC_X             0
#define     TEXT_BOOKMARK_PIC_Y             131
#define     TEXT_BOOKMARK_PIC_XSIZE         176
#define     TEXT_BOOKMARK_PIC_YSIZE         109
#define     TEXT_BOOKMARK_PIC_SIZE          ((unsigned long)((unsigned long)(TEXT_BOOKMARK_PIC_XSIZE) * TEXT_BOOKMARK_PIC_YSIZE * 2))
#define     TEXT_BOOKMARK_PIC_CTL           1
                                                
#define     TEXT_BOOKMARK_PIC               NFA_TEXT_BOOKMARK_PIC

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Record Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-26       1.0           ORG
------------------------------------------------------------------------------------------
*/              
//--------Record BackGround Address-----------
#define     NFA_RECORD_PIC                  ((unsigned long)(NFA_TEXT_BOOKMARK_PIC  + ((unsigned long)(TEXT_BOOKMARK_PIC_SIZE) * TEXT_BOOKMARK_PIC_CTL)))
#define     RECORD_SEL_PIC_LINE             NFA_RECORD_PIC
#define     RECORD_SEL_PIC_MIC              ((unsigned long)(RECORD_SEL_PIC_LINE    + ((unsigned long)(MAX_PIC_SIZE))))
#define     RECORD_BKGROUND_PIC             ((unsigned long)(RECORD_SEL_PIC_MIC     + ((unsigned long)(MAX_PIC_SIZE))))

//------------------Hold---------------------
#define     NFA_RECORD_HOLD_PIC             ((unsigned long)(RECORD_BKGROUND_PIC    + ((unsigned long)(MAX_PIC_SIZE))))
#define     RECORD_HOLD_PIC_X               0
#define     RECORD_HOLD_PIC_Y               0
#define     RECORD_HOLD_PIC_XSIZE           50
#define     RECORD_HOLD_PIC_YSIZE           50
#define     RECORD_HOLD_PIC_SIZE            ((unsigned long)((unsigned long)(RECORD_HOLD_PIC_XSIZE) * RECORD_HOLD_PIC_YSIZE * 2))
#define     RECORD_HOLD_PIC_CTL             2
                                            
#define     RECORD_HOLD_OFF_PIC             NFA_RECORD_HOLD_PIC
#define     RECORD_HOLD_ON_PIC              ((unsigned long)(RECORD_HOLD_OFF_PIC    + ((unsigned long)(RECORD_HOLD_PIC_SIZE))))

//-----------------Battery-------------------
#define     NFA_RECORD_BATT_PIC             ((unsigned long)(NFA_RECORD_HOLD_PIC  + ((unsigned long)(RECORD_HOLD_PIC_SIZE) * RECORD_HOLD_PIC_CTL)))
#define     RECORD_BATT_PIC_X               278
#define     RECORD_BATT_PIC_Y               13
#define     RECORD_BATT_PIC_XSIZE           40
#define     RECORD_BATT_PIC_YSIZE           20
#define     RECORD_BATT_PIC_SIZE            ((unsigned long)((unsigned long)(RECORD_BATT_PIC_XSIZE) * RECORD_BATT_PIC_YSIZE * 2))
#define     RECORD_BATT_PIC_CTL             4
                                            
#define     RECORD_BATT_PIC_1               NFA_RECORD_BATT_PIC
#define     RECORD_BATT_PIC_2               ((unsigned long)(RECORD_BATT_PIC_1  + ((unsigned long)(RECORD_BATT_PIC_SIZE))))
#define     RECORD_BATT_PIC_3               ((unsigned long)(RECORD_BATT_PIC_2  + ((unsigned long)(RECORD_BATT_PIC_SIZE))))
#define     RECORD_BATT_PIC_4               ((unsigned long)(RECORD_BATT_PIC_3  + ((unsigned long)(RECORD_BATT_PIC_SIZE))))

//--------------Record Status---------------
#define     NFA_RECORD_STATUSBK_PIC         ((unsigned long)(NFA_RECORD_BATT_PIC  + ((unsigned long)(RECORD_BATT_PIC_SIZE) * RECORD_BATT_PIC_CTL)))
#define     RECORD_STATUSBK_PIC_X           80
#define     RECORD_STATUSBK_PIC_Y           81
#define     RECORD_STATUSBK_PIC_XSIZE       160
#define     RECORD_STATUSBK_PIC_YSIZE       16
#define     RECORD_STATUSBK_PIC_SIZE        ((unsigned long)((unsigned long)(RECORD_STATUSBK_PIC_XSIZE) * RECORD_STATUSBK_PIC_YSIZE * 2))
#define     RECORD_STATUSBK_PIC_CTL         1
                                            
#define     RECORD_STATUSBK_PIC             NFA_RECORD_STATUSBK_PIC

//-----------Record Current Time-------------
#define     NFA_RECORD_TIMECURRBK_PIC       ((unsigned long)(NFA_RECORD_STATUSBK_PIC  + ((unsigned long)(RECORD_STATUSBK_PIC_SIZE) * RECORD_STATUSBK_PIC_CTL)))
#define     RECORD_TIMECURRBK_PIC_X         80
#define     RECORD_TIMECURRBK_PIC_Y         109
#define     RECORD_TIMECURRBK_PIC_XSIZE     160
#define     RECORD_TIMECURRBK_PIC_YSIZE     16
#define     RECORD_TIMECURRBK_PIC_SIZE      ((unsigned long)((unsigned long)(RECORD_TIMECURRBK_PIC_XSIZE) * RECORD_TIMECURRBK_PIC_YSIZE * 2))
#define     RECORD_TIMECURRBK_PIC_CTL       1
                                            
#define     RECORD_TIMECURRBK_PIC           NFA_RECORD_TIMECURRBK_PIC

//-----------Record Total Time---------------
#define     NFA_RECORD_TIMETOTALBK_PIC      ((unsigned long)(NFA_RECORD_TIMECURRBK_PIC  + ((unsigned long)(RECORD_TIMECURRBK_PIC_SIZE) * RECORD_TIMECURRBK_PIC_CTL)))
#define     RECORD_TIMETOTALBK_PIC_X        80
#define     RECORD_TIMETOTALBK_PIC_Y        189
#define     RECORD_TIMETOTALBK_PIC_XSIZE    160
#define     RECORD_TIMETOTALBK_PIC_YSIZE    16
#define     RECORD_TIMETOTALBK_PIC_SIZE     ((unsigned long)((unsigned long)(RECORD_TIMETOTALBK_PIC_XSIZE) * RECORD_TIMETOTALBK_PIC_YSIZE * 2))
#define     RECORD_TIMETOTALBK_PIC_CTL      1
                                            
#define     RECORD_TIMETOTALBK_PIC          NFA_RECORD_TIMETOTALBK_PIC

//-----------Record File Name----------------
#define     NFA_RECORD_FILENAMEBK_PIC       ((unsigned long)(NFA_RECORD_TIMETOTALBK_PIC  + ((unsigned long)(RECORD_TIMETOTALBK_PIC_SIZE) * RECORD_TIMETOTALBK_PIC_CTL)))
#define     RECORD_FILENAMEBK_PIC_X         80
#define     RECORD_FILENAMEBK_PIC_Y         154
#define     RECORD_FILENAMEBK_PIC_XSIZE     160
#define     RECORD_FILENAMEBK_PIC_YSIZE     16
#define     RECORD_FILENAMEBK_PIC_SIZE      ((unsigned long)((unsigned long)(RECORD_FILENAMEBK_PIC_XSIZE) * RECORD_FILENAMEBK_PIC_YSIZE * 2))
#define     RECORD_FILENAMEBK_PIC_CTL       1
                                            
#define     RECORD_FILENAMEBK_PIC           NFA_RECORD_FILENAMEBK_PIC

//------------------Guage--------------------
#define     NFA_RECORD_GUAGE_PIC            ((unsigned long)(NFA_RECORD_FILENAMEBK_PIC  + ((unsigned long)(RECORD_FILENAMEBK_PIC_SIZE) * RECORD_FILENAMEBK_PIC_CTL)))
#define     RECORD_GUAGE_PIC_X              56
#define     RECORD_GUAGE_PIC_Y              217
#define     RECORD_GUAGE_PIC_XSIZE          1       
#define     RECORD_GUAGE_PIC_YSIZE          5
#define     RECORD_GUAGE_PIC_SIZE           ((unsigned long)((unsigned long)(RECORD_GUAGE_PIC_XSIZE) * RECORD_GUAGE_PIC_YSIZE * 2))
#define     RECORD_GUAGE_PIC_CTL            2

#define     RECORD_GUAGE_BKGROUND_PIC       NFA_RECORD_GUAGE_PIC
#define     RECORD_GUAGE_FRONT_PIC          ((unsigned long)(RECORD_GUAGE_BKGROUND_PIC   + ((unsigned long)(RECORD_GUAGE_PIC_SIZE))))

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Explorer Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-26       1.0           ORG
------------------------------------------------------------------------------------------
*/              
//--------Record BackGround Address-----------
#define     NFA_EXPLORER_PIC                ((unsigned long)(NFA_RECORD_GUAGE_PIC + ((unsigned long)(RECORD_GUAGE_PIC_SIZE) * RECORD_GUAGE_PIC_CTL)))
#define     EXPLORER_BKGROUND_PIC           NFA_EXPLORER_PIC

//------------------Hold---------------------
#define     NFA_EXPLORER_HOLD_PIC           ((unsigned long)(EXPLORER_BKGROUND_PIC    + ((unsigned long)(MAX_PIC_SIZE))))
#define     EXPLORER_HOLD_PIC_X             10
#define     EXPLORER_HOLD_PIC_Y             0
#define     EXPLORER_HOLD_PIC_XSIZE         40
#define     EXPLORER_HOLD_PIC_YSIZE         37
#define     EXPLORER_HOLD_PIC_SIZE          ((unsigned long)((unsigned long)(EXPLORER_HOLD_PIC_XSIZE) * EXPLORER_HOLD_PIC_YSIZE * 2))
#define     EXPLORER_HOLD_PIC_CTL           2
                                            
#define     EXPLORER_HOLD_OFF_PIC           NFA_EXPLORER_HOLD_PIC
#define     EXPLORER_HOLD_ON_PIC            ((unsigned long)(EXPLORER_HOLD_OFF_PIC    + ((unsigned long)(EXPLORER_HOLD_PIC_SIZE))))

//-----------------Battery-------------------
#define     NFA_EXPLORER_BATT_PIC           ((unsigned long)(NFA_EXPLORER_HOLD_PIC  + ((unsigned long)(EXPLORER_HOLD_PIC_SIZE) * EXPLORER_HOLD_PIC_CTL)))
#define     EXPLORER_BATT_PIC_X             262
#define     EXPLORER_BATT_PIC_Y             12
#define     EXPLORER_BATT_PIC_XSIZE         40
#define     EXPLORER_BATT_PIC_YSIZE         20
#define     EXPLORER_BATT_PIC_SIZE          ((unsigned long)((unsigned long)(EXPLORER_BATT_PIC_XSIZE) * EXPLORER_BATT_PIC_YSIZE * 2))
#define     EXPLORER_BATT_PIC_CTL           4
                                            
#define     EXPLORER_BATT_PIC_1             NFA_EXPLORER_BATT_PIC
#define     EXPLORER_BATT_PIC_2             ((unsigned long)(EXPLORER_BATT_PIC_1  + ((unsigned long)(EXPLORER_BATT_PIC_SIZE))))
#define     EXPLORER_BATT_PIC_3             ((unsigned long)(EXPLORER_BATT_PIC_2  + ((unsigned long)(EXPLORER_BATT_PIC_SIZE))))
#define     EXPLORER_BATT_PIC_4             ((unsigned long)(EXPLORER_BATT_PIC_3  + ((unsigned long)(EXPLORER_BATT_PIC_SIZE))))

//-------------Explorer Scroll---------------
#define     NFA_EXPLORER_SCROLL_PIC         ((unsigned long)(NFA_EXPLORER_BATT_PIC  + ((unsigned long)(EXPLORER_BATT_PIC_SIZE) * EXPLORER_BATT_PIC_CTL)))
#define     EXPLORER_SCROLL_PIC_X           312
#define     EXPLORER_SCROLL_PIC_Y           50
#define     EXPLORER_SCROLL_PIC_XSIZE       5
#define     EXPLORER_SCROLL_PIC_YSIZE       21
#define     EXPLORER_SCROLL_PIC_SIZE        ((unsigned long)((unsigned long)(EXPLORER_SCROLL_PIC_XSIZE) * EXPLORER_SCROLL_PIC_YSIZE * 2))
#define     EXPLORER_SCROLL_PIC_CTL         2
                                            
#define     EXPLORER_SCROLL_BACKGROUND_PIC  NFA_EXPLORER_SCROLL_PIC
#define     EXPLORER_SCROLL_FOREROUND_PIC   ((unsigned long)(EXPLORER_SCROLL_BACKGROUND_PIC + ((unsigned long)(EXPLORER_SCROLL_PIC_SIZE))))

//-----------Explorer Area BackGround--------
#define     NFA_EXPLORER_AREABK_PIC         ((unsigned long)(NFA_EXPLORER_SCROLL_PIC + ((unsigned long)(EXPLORER_SCROLL_PIC_SIZE) * EXPLORER_SCROLL_PIC_CTL)))
#define     EXPLORER_AREABK_PIC_X           10
#define     EXPLORER_AREABK_PIC_Y           50
#define     EXPLORER_AREABK_PIC_XSIZE       294
#define     EXPLORER_AREABK_PIC_YSIZE       180
#define     EXPLORER_AREABK_PIC_SIZE        ((unsigned long)((unsigned long)(EXPLORER_AREABK_PIC_XSIZE) * EXPLORER_AREABK_PIC_YSIZE * 2))
#define     EXPLORER_AREABK_PIC_CTL         1
                                            
#define     EXPLORER_AREABK_PIC             NFA_EXPLORER_AREABK_PIC

//---------------File Type Icon--------------
#define     NFA_EXPLORER_ICON_PIC           ((unsigned long)(NFA_EXPLORER_AREABK_PIC  + ((unsigned long)(EXPLORER_AREABK_PIC_SIZE) * EXPLORER_AREABK_PIC_CTL)))
#define     EXPLORER_ICON_PIC_X             10
#define     EXPLORER_ICON_PIC_Y             50
#define     EXPLORER_ICON_PIC_XSIZE         20
#define     EXPLORER_ICON_PIC_YSIZE         20
#define     EXPLORER_ICON_PIC_SIZE          ((unsigned long)((unsigned long)(EXPLORER_ICON_PIC_XSIZE) * EXPLORER_ICON_PIC_YSIZE * 2))
#define     EXPLORER_ICON_PIC_CTL           6
                                            
#define     EXPLORER_ICON_FOLDER_PIC        NFA_EXPLORER_ICON_PIC
#define     EXPLORER_ICON_MUSIC_PIC         ((unsigned long)(EXPLORER_ICON_FOLDER_PIC   + ((unsigned long)(EXPLORER_ICON_PIC_SIZE))))
#define     EXPLORER_ICON_VIDEO_PIC         ((unsigned long)(EXPLORER_ICON_MUSIC_PIC    + ((unsigned long)(EXPLORER_ICON_PIC_SIZE))))
#define     EXPLORER_ICON_TXT_PIC           ((unsigned long)(EXPLORER_ICON_VIDEO_PIC    + ((unsigned long)(EXPLORER_ICON_PIC_SIZE))))
#define     EXPLORER_ICON_PHOTO_PIC         ((unsigned long)(EXPLORER_ICON_TXT_PIC      + ((unsigned long)(EXPLORER_ICON_PIC_SIZE))))
#define     EXPLORER_ICON_OTHER_PIC         ((unsigned long)(EXPLORER_ICON_PHOTO_PIC    + ((unsigned long)(EXPLORER_ICON_PIC_SIZE))))

//------------Explorer Select Bar-----------
#define     NFA_EXPLORER_BAR_PIC            ((unsigned long)(NFA_EXPLORER_ICON_PIC  + ((unsigned long)(EXPLORER_ICON_PIC_SIZE) * EXPLORER_ICON_PIC_CTL)))
#define     EXPLORER_BAR_PIC_X              34
#define     EXPLORER_BAR_PIC_Y              50
#define     EXPLORER_BAR_PIC_XSIZE          270
#define     EXPLORER_BAR_PIC_YSIZE          20
#define     EXPLORER_BAR_PIC_SIZE           ((unsigned long)((unsigned long)(EXPLORER_BAR_PIC_XSIZE) * EXPLORER_BAR_PIC_YSIZE * 2))
#define     EXPLORER_BAR_PIC_CTL            1

#define     EXPLORER_BAR_PIC                NFA_EXPLORER_BAR_PIC

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Game Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-26       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_GAME_PIC                    ((unsigned long)(NFA_EXPLORER_BAR_PIC + ((unsigned long)(EXPLORER_BAR_PIC_SIZE) * EXPLORER_BAR_PIC_CTL)))               

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Setup Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-26       1.0           ORG
------------------------------------------------------------------------------------------
*/              
//--------Record BackGround Address-----------
#define     NFA_SETUP_PIC                   ((unsigned long)(NFA_EXPLORER_BAR_PIC   + ((unsigned long)(EXPLORER_BAR_PIC_SIZE) * EXPLORER_BAR_PIC_CTL)))
#define     SETUP_BKGROUND_PIC              NFA_SETUP_PIC

//------------------Hold---------------------
#define     NFA_SETUP_HOLD_PIC              ((unsigned long)(SETUP_BKGROUND_PIC     + ((unsigned long)(MAX_PIC_SIZE))))
#define     SETUP_HOLD_PIC_X                10
#define     SETUP_HOLD_PIC_Y                0
#define     SETUP_HOLD_PIC_XSIZE            40
#define     SETUP_HOLD_PIC_YSIZE            37
#define     SETUP_HOLD_PIC_SIZE             ((unsigned long)((unsigned long)(SETUP_HOLD_PIC_XSIZE) * SETUP_HOLD_PIC_YSIZE * 2))
#define     SETUP_HOLD_PIC_CTL              2
                                            
#define     SETUP_HOLD_OFF_PIC              NFA_SETUP_HOLD_PIC
#define     SETUP_HOLD_ON_PIC               ((unsigned long)(SETUP_HOLD_OFF_PIC    + ((unsigned long)(SETUP_HOLD_PIC_SIZE))))

//-----------------Battery-------------------
#define     NFA_SETUP_BATT_PIC              ((unsigned long)(NFA_SETUP_HOLD_PIC  + ((unsigned long)(SETUP_HOLD_PIC_SIZE) * SETUP_HOLD_PIC_CTL)))
#define     SETUP_BATT_PIC_X                262
#define     SETUP_BATT_PIC_Y                12
#define     SETUP_BATT_PIC_XSIZE            40
#define     SETUP_BATT_PIC_YSIZE            20
#define     SETUP_BATT_PIC_SIZE             ((unsigned long)((unsigned long)(SETUP_BATT_PIC_XSIZE) * SETUP_BATT_PIC_YSIZE * 2))
#define     SETUP_BATT_PIC_CTL              4
                                               
#define     SETUP_BATT_PIC_1                NFA_SETUP_BATT_PIC
#define     SETUP_BATT_PIC_2                ((unsigned long)(SETUP_BATT_PIC_1  + ((unsigned long)(SETUP_BATT_PIC_SIZE))))
#define     SETUP_BATT_PIC_3                ((unsigned long)(SETUP_BATT_PIC_2  + ((unsigned long)(SETUP_BATT_PIC_SIZE))))
#define     SETUP_BATT_PIC_4                ((unsigned long)(SETUP_BATT_PIC_3  + ((unsigned long)(SETUP_BATT_PIC_SIZE))))

//--------------Setup Scroll-----------------
#define     NFA_SETUP_SCROLL_PIC            ((unsigned long)(NFA_SETUP_BATT_PIC  + ((unsigned long)(SETUP_BATT_PIC_SIZE) * SETUP_BATT_PIC_CTL)))
#define     SETUP_SCROLL_PIC_X              312
#define     SETUP_SCROLL_PIC_Y              50
#define     SETUP_SCROLL_PIC_XSIZE          5
#define     SETUP_SCROLL_PIC_YSIZE          21
#define     SETUP_SCROLL_PIC_SIZE           ((unsigned long)((unsigned long)(SETUP_SCROLL_PIC_XSIZE) * SETUP_SCROLL_PIC_YSIZE * 2))
#define     SETUP_SCROLL_PIC_CTL            2
                                               
#define     SETUP_SCROLL_BACKGROUND_PIC     NFA_SETUP_SCROLL_PIC
#define     SETUP_SCROLL_FOREROUND_PIC      ((unsigned long)(SETUP_SCROLL_BACKGROUND_PIC + ((unsigned long)(SETUP_SCROLL_PIC_SIZE))))

//------------Setup Area BackGround----------
#define     NFA_SETUP_AREABK_PIC            ((unsigned long)(NFA_SETUP_SCROLL_PIC + ((unsigned long)(SETUP_SCROLL_PIC_SIZE) * SETUP_SCROLL_PIC_CTL)))
#define     SETUP_AREABK_PIC_X              10
#define     SETUP_AREABK_PIC_Y              50
#define     SETUP_AREABK_PIC_XSIZE          294
#define     SETUP_AREABK_PIC_YSIZE          180
#define     SETUP_AREABK_PIC_SIZE           ((unsigned long)((unsigned long)(SETUP_AREABK_PIC_XSIZE) * SETUP_AREABK_PIC_YSIZE * 2))
#define     SETUP_AREABK_PIC_CTL            1
                                               
#define     SETUP_AREABK_PIC                NFA_SETUP_AREABK_PIC

//-------------Setup Select Icon-------------
#define     NFA_SETUP_ICON_PIC              ((unsigned long)(NFA_SETUP_AREABK_PIC  + ((unsigned long)(SETUP_AREABK_PIC_SIZE) * SETUP_AREABK_PIC_CTL)))
#define     SETUP_ICON_PIC_X                10
#define     SETUP_ICON_PIC_Y                50
#define     SETUP_ICON_PIC_XSIZE            20
#define     SETUP_ICON_PIC_YSIZE            20
#define     SETUP_ICON_PIC_SIZE             ((unsigned long)((unsigned long)(SETUP_ICON_PIC_XSIZE) * SETUP_ICON_PIC_YSIZE * 2))
#define     SETUP_ICON_PIC_CTL              2
                                               
#define     SETUP_ICON_SEL_OFF              NFA_SETUP_ICON_PIC
#define     SETUP_ICON_SEL_ON               ((unsigned long)(SETUP_ICON_SEL_OFF   + ((unsigned long)(SETUP_ICON_PIC_SIZE))))

//--------------Setup Select Bar------------
#define     NFA_SETUP_BAR_PIC               ((unsigned long)(NFA_SETUP_ICON_PIC  + ((unsigned long)(SETUP_ICON_PIC_SIZE) * SETUP_ICON_PIC_CTL)))
#define     SETUP_BAR_PIC_X                 34
#define     SETUP_BAR_PIC_Y                 50
#define     SETUP_BAR_PIC_XSIZE             270
#define     SETUP_BAR_PIC_YSIZE             20
#define     SETUP_BAR_PIC_SIZE              ((unsigned long)((unsigned long)(SETUP_BAR_PIC_XSIZE) * SETUP_BAR_PIC_YSIZE * 2))
#define     SETUP_BAR_PIC_CTL               1
                                            
#define     SETUP_BAR_PIC                   NFA_SETUP_BAR_PIC

//------------User Eq BackGround------------
#define     NFA_SETUP_USEREQBK_PIC          ((unsigned long)(NFA_SETUP_BAR_PIC  + ((unsigned long)(SETUP_BAR_PIC_SIZE) * SETUP_BAR_PIC_CTL)))
#define     SETUP_USEREQBK_PIC              NFA_SETUP_USEREQBK_PIC

//------------User Eq No Select-------------
#define     NFA_SETUP_USEREQ_NOSEL_PIC      ((unsigned long)(SETUP_USEREQBK_PIC     + ((unsigned long)(MAX_PIC_SIZE))))
#define     SETUP_USEREQ_NOSEL_PIC_X        50
#define     SETUP_USEREQ_NOSEL_PIC_Y        45
#define     SETUP_USEREQ_NOSEL_PIC_XSIZE    20
#define     SETUP_USEREQ_NOSEL_PIC_YSIZE    145
#define     SETUP_USEREQ_NOSEL_PIC_SIZE     ((unsigned long)((unsigned long)(SETUP_USEREQ_NOSEL_PIC_XSIZE) * SETUP_USEREQ_NOSEL_PIC_YSIZE * 2))
#define     SETUP_USEREQ_NOSEL_PIC_CTL      7
                                            
#define     SETUP_USEREQ_NOSEL_PIC1         NFA_SETUP_USEREQ_NOSEL_PIC
#define     SETUP_USEREQ_NOSEL_PIC2         ((unsigned long)(SETUP_USEREQ_NOSEL_PIC1  + ((unsigned long)(SETUP_USEREQ_NOSEL_PIC_SIZE))))
#define     SETUP_USEREQ_NOSEL_PIC3         ((unsigned long)(SETUP_USEREQ_NOSEL_PIC2  + ((unsigned long)(SETUP_USEREQ_NOSEL_PIC_SIZE))))
#define     SETUP_USEREQ_NOSEL_PIC4         ((unsigned long)(SETUP_USEREQ_NOSEL_PIC3  + ((unsigned long)(SETUP_USEREQ_NOSEL_PIC_SIZE))))
#define     SETUP_USEREQ_NOSEL_PIC5         ((unsigned long)(SETUP_USEREQ_NOSEL_PIC4  + ((unsigned long)(SETUP_USEREQ_NOSEL_PIC_SIZE))))
#define     SETUP_USEREQ_NOSEL_PIC6         ((unsigned long)(SETUP_USEREQ_NOSEL_PIC5  + ((unsigned long)(SETUP_USEREQ_NOSEL_PIC_SIZE))))
#define     SETUP_USEREQ_NOSEL_PIC7         ((unsigned long)(SETUP_USEREQ_NOSEL_PIC6  + ((unsigned long)(SETUP_USEREQ_NOSEL_PIC_SIZE))))

//--------------User Eq Select---------------
#define     NFA_SETUP_USEREQ_SEL_PIC        ((unsigned long)(NFA_SETUP_USEREQ_NOSEL_PIC  + ((unsigned long)(SETUP_USEREQ_NOSEL_PIC_SIZE) * SETUP_USEREQ_NOSEL_PIC_CTL)))
#define     SETUP_USEREQ_SEL_PIC_X        50
#define     SETUP_USEREQ_SEL_PIC_Y        45
#define     SETUP_USEREQ_SEL_PIC_XSIZE    20
#define     SETUP_USEREQ_SEL_PIC_YSIZE    145
#define     SETUP_USEREQ_SEL_PIC_SIZE     ((unsigned long)((unsigned long)(SETUP_USEREQ_SEL_PIC_XSIZE) * SETUP_USEREQ_SEL_PIC_YSIZE * 2))
#define     SETUP_USEREQ_SEL_PIC_CTL      7
                                            
#define     SETUP_USEREQ_SEL_PIC1         NFA_SETUP_USEREQ_SEL_PIC
#define     SETUP_USEREQ_SEL_PIC2         ((unsigned long)(SETUP_USEREQ_SEL_PIC1  + ((unsigned long)(SETUP_USEREQ_SEL_PIC_SIZE))))
#define     SETUP_USEREQ_SEL_PIC3         ((unsigned long)(SETUP_USEREQ_SEL_PIC2  + ((unsigned long)(SETUP_USEREQ_SEL_PIC_SIZE))))
#define     SETUP_USEREQ_SEL_PIC4         ((unsigned long)(SETUP_USEREQ_SEL_PIC3  + ((unsigned long)(SETUP_USEREQ_SEL_PIC_SIZE))))
#define     SETUP_USEREQ_SEL_PIC5         ((unsigned long)(SETUP_USEREQ_SEL_PIC4  + ((unsigned long)(SETUP_USEREQ_SEL_PIC_SIZE))))
#define     SETUP_USEREQ_SEL_PIC6         ((unsigned long)(SETUP_USEREQ_SEL_PIC5  + ((unsigned long)(SETUP_USEREQ_SEL_PIC_SIZE))))
#define     SETUP_USEREQ_SEL_PIC7         ((unsigned long)(SETUP_USEREQ_SEL_PIC6  + ((unsigned long)(SETUP_USEREQ_SEL_PIC_SIZE))))

//--------------User Eq Button-------------
#define     NFA_SETUP_USEREQ_BTN_PIC        ((unsigned long)(NFA_SETUP_USEREQ_SEL_PIC  + ((unsigned long)(SETUP_USEREQ_SEL_PIC_SIZE) * SETUP_USEREQ_SEL_PIC_CTL)))
#define     SETUP_USEREQ_BTN_PIC_XSIZE      73
#define     SETUP_USEREQ_BTN_PIC_YSIZE      29
#define     SETUP_USEREQ_BTN_PIC_SIZE       ((unsigned long)((unsigned long)(SETUP_USEREQ_BTN_PIC_XSIZE) * SETUP_USEREQ_BTN_PIC_YSIZE * 2))
#define     SETUP_USEREQ_BTN_PIC_CTL        2
                                            
#define     SETUP_USEREQ_BTN_YES_PIC_X      50
#define     SETUP_USEREQ_BTN_YES_PIC_Y      208
#define     SETUP_USEREQ_BTN_NO_PIC_X       197
#define     SETUP_USEREQ_BTN_NO_PIC_Y       208

#define     SETUP_USEREQ_BTN_NOSEL_PIC      NFA_SETUP_USEREQ_BTN_PIC
#define     SETUP_USEREQ_BTN_SEL_PIC        ((unsigned long)(SETUP_USEREQ_BTN_NOSEL_PIC  + ((unsigned long)(SETUP_USEREQ_BTN_PIC_SIZE))))

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Other Module
* Author     :  tonyzyz
* Description:  Radio Module Image Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-26       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_OTHER_PIC                   ((unsigned long)(NFA_SETUP_USEREQ_BTN_PIC + ((unsigned long)(SETUP_USEREQ_BTN_PIC_SIZE) * SETUP_USEREQ_BTN_PIC_CTL)))
                
//----------------Charge--------------------
#define     CHARGE_LEVEL_PIC                NFA_OTHER_PIC
#define     CHARGE_LEVEL_PIC_CTL            4

//----------------Hold--------------------
#define     HOLD_OFF_PIC                    ((unsigned long)(CHARGE_LEVEL_PIC   + ((unsigned long)(MAX_PIC_SIZE) * CHARGE_LEVEL_PIC_CTL)))
#define     HOLD_ON_PIC                     ((unsigned long)(HOLD_OFF_PIC       + ((unsigned long)(MAX_PIC_SIZE))))

//--------------USB Connect------------------
#define     USB_CONNECT_PIC                 ((unsigned long)(HOLD_ON_PIC        + ((unsigned long)(MAX_PIC_SIZE))))

//--------------Message---------------------
#define     NFA_MESSAGE_BOX_PIC             ((unsigned long)(USB_CONNECT_PIC    + ((unsigned long)(MAX_PIC_SIZE))))
#define     MESSAGE_BOX_PIC_X               63
#define     MESSAGE_BOX_PIC_Y               52
#define     MESSAGE_BOX_PIC_XSIZE           194
#define     MESSAGE_BOX_PIC_YSIZE           135
#define     MESSAGE_BOX_PIC_SIZE            ((unsigned long)((unsigned long)(MESSAGE_BOX_PIC_XSIZE) * MESSAGE_BOX_PIC_YSIZE * 2))
#define     MESSAGE_BOX_PIC_CTL             1
#define     MESSAGE_BOX_PIC                 NFA_MESSAGE_BOX_PIC
                
//--------------DIALOG----------------------
#define     NFA_DIALOG_BOX_PIC              ((unsigned long)(NFA_MESSAGE_BOX_PIC + ((unsigned long)(MESSAGE_BOX_PIC_SIZE) * MESSAGE_BOX_PIC_CTL)))
#define     DIALOG_BOX_PIC_X                63
#define     DIALOG_BOX_PIC_Y                52
#define     DIALOG_BOX_PIC_XSIZE            194
#define     DIALOG_BOX_PIC_YSIZE            135
#define     DIALOG_BOX_PIC_SIZE             ((unsigned long)((unsigned long)(DIALOG_BOX_PIC_XSIZE) * DIALOG_BOX_PIC_YSIZE * 2))
#define     DIALOG_BOX_PIC_CTL              1
#define     DIALOG_BOX_PIC                  NFA_DIALOG_BOX_PIC
                
#define     NFA_DIALOG_BUTTON_PIC           ((unsigned long)(NFA_DIALOG_BOX_PIC + ((unsigned long)(DIALOG_BOX_PIC_SIZE) * DIALOG_BOX_PIC_CTL)))
#define     DIALOG_BUTTON_PIC_XSIZE         60
#define     DIALOG_BUTTON_PIC_YSIZE         27
#define     DIALOG_BUTTON_PIC_SIZE          ((unsigned long)((unsigned long)(DIALOG_BUTTON_PIC_XSIZE) * DIALOG_BUTTON_PIC_YSIZE * 2))
#define     DIALOG_BUTTON_PIC_CTL           2
                
#define     DIALOG_BUTTON_NOSEL             NFA_DIALOG_BUTTON_PIC
#define     DIALOG_BUTTON_SEL               ((unsigned long)(DIALOG_BUTTON_NOSEL    + ((unsigned long)(DIALOG_BUTTON_PIC_SIZE))))

#define     DIALOG_BUTTON_YES_PIC_X         87
#define     DIALOG_BUTTON_YES_PIC_Y         145

#define     DIALOG_BUTTON_NO_PIC_X          175
#define     DIALOG_BUTTON_NO_PIC_Y          145

/*              
****************************************************************************************************
*          Copyright (C),2006-2006, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* ModuleName :  Menu.res
* Author     :  tonyzyz
* Description:  Individual Define The "Display Text" & "Menu Text" Address of Every Function Module :
*               2.Main Menu 
*               3.Music   Module
*               4.Video   Module
*               5.Radio   Module
*               6.Photo   Module
*               7.Text    Module
*               8.Record  Module
*               9.Explrer Module
*              10.Game    Module
*              11.Setup   Module
*              12.USB     Module
*              13.Other   Module
* History    :        
*               <author>        <time>     <version>       <desc>
*                tonyzyz       06/01/09       1.0            ORG
*               
****************************************************************************************************
*/              
#define     MENU_NULL                       0
#define     MENU_ITEM_TOTAL                 9
#define     MENU_PER_BYTE                   40
#define     MENU_LANGUE_PER_LINE            320

#define     MENU_LANGUE_PER_BYTE            ((unsigned long)(MENU_PER_BYTE) * MENU_LANGUE_PER_LINE)
#define     TEXT_IN_FLASH_BASE_ADDR         (unsigned long)0
/*              
------------------------------------------------------------------------------------------
* ModuleName :  Main Menu
* Author     :  tonyzyz
* Description:  Main Menu Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_MAIN_MENU_TEXT              TEXT_IN_FLASH_BASE_ADDR
#define     MAIN_MENU_TXT_CTL               16

#define     MAIN_MENU_TXT_X                 90
#define     MAIN_MENU_TXT_Y                 183
#define     MAIN_MENU_TXT_XSIZE             150
#define     MAIN_MENU_TXT_YSIZE             16
#define     MAIN_MENU_MUSIC_TXT             NFA_MAIN_MENU_TEXT
#define     MAIN_MENU_VEDIO_TXT             ((unsigned long)(MAIN_MENU_MUSIC_TXT    + (unsigned long)(MENU_PER_BYTE)))
#define     MAIN_MENU_RADIO_TXT             ((unsigned long)(MAIN_MENU_VEDIO_TXT    + (unsigned long)(MENU_PER_BYTE)))
#define     MAIN_MENU_PHOTO_TXT             ((unsigned long)(MAIN_MENU_RADIO_TXT    + (unsigned long)(MENU_PER_BYTE)))
#define     MAIN_MENU_TEXT_TXT              ((unsigned long)(MAIN_MENU_PHOTO_TXT    + (unsigned long)(MENU_PER_BYTE)))
#define     MAIN_MENU_RECORD_TXT            ((unsigned long)(MAIN_MENU_TEXT_TXT     + (unsigned long)(MENU_PER_BYTE)))
#define     MAIN_MENU_EXPLORER_TXT          ((unsigned long)(MAIN_MENU_RECORD_TXT   + (unsigned long)(MENU_PER_BYTE)))
#define     MAIN_MENU_GAME_TXT              ((unsigned long)(MAIN_MENU_EXPLORER_TXT + (unsigned long)(MENU_PER_BYTE)))
#define     MAIN_MENU_SETTING_TXT           ((unsigned long)(MAIN_MENU_GAME_TXT     + (unsigned long)(MENU_PER_BYTE)))

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Music Module
* Author     :  tonyzyz
* Description:  Music Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_MUSIC_TEXT                  ((unsigned long)(NFA_MAIN_MENU_TEXT     + ((unsigned long)(MENU_PER_BYTE) * MAIN_MENU_TXT_CTL)))
#define     MUSIC_TXT_CTL                   16

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Vedio Module
* Author     :  tonyzyz
* Description:  Vedio Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_VEDIO_TEXT                  ((unsigned long)(NFA_MUSIC_TEXT  + ((unsigned long)(MENU_PER_BYTE) * MUSIC_TXT_CTL)))
#define     VEDIO_TXT_CTL                   16

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Radio Module
* Author     :  tonyzyz
* Description:  Radio Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_RADIO_TEXT                  ((unsigned long)(NFA_VEDIO_TEXT     + ((unsigned long)(MENU_PER_BYTE) * VEDIO_TXT_CTL)))
#define     RADIO_TXT_CTL                   16

//---------------Sub Menu-------------------
#define     RADIO_SUB_MENU_TXT_X            18
#define     RADIO_SUB_MENU_TXT_Y            46
#define     RADIO_SUB_MENU_TXT_XSIZE        48
#define     RADIO_SUB_MENU_TXT_YSIZE        16
#define     RADIO_SUB_MENU_TXT_YSTEP        23

#define     RADIO_SUB_LIST_TXT_X            77
#define     RADIO_SUB_LIST_TXT_Y            63
#define     RADIO_SUB_LIST_TXT_XSIZE        104
#define     RADIO_SUB_LIST_TXT_YSIZE        16
#define     RADIO_SUB_LIST_TXT_YSTEP        20

#define     RADIO_SUB_MENU_MANUAL_TXT       NFA_RADIO_TEXT
#define     RADIO_SUB_MENU_PRESET_TXT       ((unsigned long)(RADIO_SUB_MENU_MANUAL_TXT  + (unsigned long)(MENU_PER_BYTE )))
#define     RADIO_SUB_MENU_SAVE_TXT         ((unsigned long)(RADIO_SUB_MENU_PRESET_TXT  + (unsigned long)(MENU_PER_BYTE )))
#define     RADIO_SUB_MENU_DEL_TXT          ((unsigned long)(RADIO_SUB_MENU_SAVE_TXT    + (unsigned long)(MENU_PER_BYTE )))
#define     RADIO_SUB_MENU_AUTO_TXT         ((unsigned long)(RADIO_SUB_MENU_DEL_TXT     + (unsigned long)(MENU_PER_BYTE )))

//---------------Dialog Menu----------------
#define     RADIO_DIALOG_SAVE_TXT           ((unsigned long)(RADIO_SUB_MENU_AUTO_TXT    + (unsigned long)(MENU_PER_BYTE )))
#define     RADIO_DIALOG_DEL_TXT            ((unsigned long)(RADIO_DIALOG_SAVE_TXT      + (unsigned long)(MENU_PER_BYTE )))

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Photo Module
* Author     :  tonyzyz
* Description:  Photo Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_PHOTO_TEXT                  ((unsigned long)(NFA_RADIO_TEXT     + ((unsigned long)(MENU_PER_BYTE) * RADIO_TXT_CTL)))
#define     PHOTO_TXT_CTL                   16

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Text Module
* Author     :  tonyzyz
* Description:  Text Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_TEXT_TEXT                   ((unsigned long)(NFA_PHOTO_TEXT     + ((unsigned long)(MENU_PER_BYTE) * PHOTO_TXT_CTL)))
#define     TEXT_TXT_CTL                    16

#define     TEXT_TITLE_TXT_X                82
#define     TEXT_TITLE_TXT_Y                14
#define     TEXT_TITLE_TXT_XSIZE            128
#define     TEXT_TITLE_TXT_YSIZE            16

#define     TEXT_FILENAME_TXT_X             20
#define     TEXT_FILENAME_TXT_Y             45
#define     TEXT_FILENAME_TXT_XSIZE         280
#define     TEXT_FILENAME_TXT_YSIZE         16

//-------------BookMark----------------------
#define     TEXT_BOOKMARK_TITLE_TXT_X       20
#define     TEXT_BOOKMARK_TITLE_TXT_Y       145
#define     TEXT_BOOKMARK_TITLE_TXT_XSIZE   136
#define     TEXT_BOOKMARK_TITLE_TXT_YSIZE   16

#define     TEXT_BOOKMARK_CONTENT_TXT_X     50
#define     TEXT_BOOKMARK_CONTENT_TXT_Y     167
#define     TEXT_BOOKMARK_CONTENT_TXT_XSIZE 112
#define     TEXT_BOOKMARK_CONTENT_TXT_YSIZE 16

#define     TEXT_BOOKMARK_CONTENT_TXT_YSTEP 22

#define     TEXT_BOOKMARK_SAVE_TXT          NFA_TEXT_TEXT
#define     TEXT_BOOKMARK_LOAD_TXT          ((unsigned long)(TEXT_BOOKMARK_SAVE_TXT + (unsigned long)(MENU_PER_BYTE )))

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Record Module
* Author     :  tonyzyz
* Description:  Record Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_RECORD_TEXT                 ((unsigned long)(NFA_TEXT_TEXT     + ((unsigned long)(MENU_PER_BYTE) * TEXT_TXT_CTL)))
#define     RECORD_TXT_CTL                  16

//-------------Record Type-------------------
#define     RECORD_TYPE_TXT_X               80
#define     RECORD_TYPE_TXT_Y               55
#define     RECORD_TYPE_TXT_XSIZE           160
#define     RECORD_TYPE_TXT_YSIZE           16
#define     RECORD_TYPE_MIC_TXT             NFA_RECORD_TEXT
#define     RECORD_TYPE_LINE_TXT            ((unsigned long)(RECORD_TYPE_MIC_TXT    + (unsigned long)(MENU_PER_BYTE )))
#define     RECORD_TYPE_FM_TXT              ((unsigned long)(RECORD_TYPE_LINE_TXT   + (unsigned long)(MENU_PER_BYTE )))

//------------Record Status-----------------
#define     RECORD_STATUS_TXT_X             80
#define     RECORD_STATUS_TXT_Y             81
#define     RECORD_STATUS_TXT_XSIZE         160
#define     RECORD_STATUS_TXT_YSIZE         16
#define     RECORD_STATUS_PLAY_TXT          ((unsigned long)(RECORD_TYPE_FM_TXT         + (unsigned long)(MENU_PER_BYTE )))
#define     RECORD_STATUS_PAUSE_TXT         ((unsigned long)(RECORD_STATUS_PLAY_TXT     + (unsigned long)(MENU_PER_BYTE )))
#define     RECORD_STATUS_READY_TXT         ((unsigned long)(RECORD_STATUS_PAUSE_TXT    + (unsigned long)(MENU_PER_BYTE )))
#define     RECORD_STATUS_SAVE_TXT          ((unsigned long)(RECORD_STATUS_READY_TXT    + (unsigned long)(MENU_PER_BYTE )))

//------------Record Dialog-----------------
#define     RECORD_MSG_CREAT_ERR_TXT        ((unsigned long)(RECORD_STATUS_SAVE_TXT         + (unsigned long)(MENU_PER_BYTE )))
#define     RECORD_MSG_DISK_FULL_TXT        ((unsigned long)(RECORD_MSG_CREAT_ERR_TXT    + (unsigned long)(MENU_PER_BYTE )))
#define     RECORD_MSG_DISK_NOT_ENOUGH_TXT  RECORD_MSG_DISK_FULL_TXT

#define     RECORD_QUALITY_TXT_X            16
#define     RECORD_QUALITY_TXT_Y            189
#define     RECORD_QUALITY_TXT_XSIZE        65
#define     RECORD_QUALITY_TXT_YSIZE        16
/*              
------------------------------------------------------------------------------------------
* ModuleName :  Explorer Module
* Author     :  tonyzyz
* Description:  Exporler Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_EXPLORER_TEXT               ((unsigned long)(NFA_RECORD_TEXT     + ((unsigned long)(MENU_PER_BYTE) * RECORD_TXT_CTL)))
#define     EXPLORER_TXT_CTL                16

//---------Explorer Memery Select------------
#define     EXPLORER_MEMSEL_TXT_X           0
#define     EXPLORER_MEMSEL_TXT_Y           0
#define     EXPLORER_MEMSEL_TXT_XSIZE       0
#define     EXPLORER_MEMSEL_TXT_YSIZE       0
#define     EXPLORER_MEMSEL_FLASH_TXT       NFA_EXPLORER_TEXT
#define     EXPLORER_MEMSEL_CARD_TXT        ((unsigned long)(EXPLORER_MEMSEL_FLASH_TXT  + (unsigned long)(MENU_PER_BYTE )))

#define     EXPLORER_TITLE_TXT_X            82
#define     EXPLORER_TITLE_TXT_Y            14
#define     EXPLORER_TITLE_TXT_XSIZE        128
#define     EXPLORER_TITLE_TXT_YSIZE        16
/*              
------------------------------------------------------------------------------------------
* ModuleName :  Game Module
* Author     :  tonyzyz
* Description:  Game Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define    NFA_GAME_TEXT                    ((unsigned long)(NFA_EXPLORER_TEXT     + ((unsigned long)(MENU_PER_BYTE) * EXPLORER_TXT_CTL)))
#define    GAME_TXT_CTL                     16

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Setup Module
* Author     :  tonyzyz
* Description:  Setup Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     NFA_SETUP_TEXT                  ((unsigned long)(NFA_GAME_TEXT     + ((unsigned long)(MENU_PER_BYTE) * GAME_TXT_CTL)))

#define     SETUP_MENU_TXT_X                82
#define     SETUP_MENU_TXT_Y                14
#define     SETUP_MENU_TXT_XSIZE            128
#define     SETUP_MENU_TXT_YSIZE            16
//-----------Setup Main Menu-----------------
#define     SETUP_MENU_TXT                  NFA_SETUP_TEXT
#define     SETUP_MENU_TXT_CTL              10

//-----------Setup Music Menu----------------
#define     SETUP_MUSIC_TXT                 ((unsigned long)(SETUP_MENU_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_MENU_TXT_CTL)))
#define     SETUP_MUSIC_TXT_CTL             5

//-----------Setup Radio Menu----------------
#define     SETUP_RADIO_TXT                 ((unsigned long)(SETUP_MUSIC_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_MUSIC_TXT_CTL)))
#define     SETUP_RADIO_TXT_CTL             3

//-----------Setup Record Menu---------------
#define     SETUP_RECORD_TXT                ((unsigned long)(SETUP_RADIO_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_RADIO_TXT_CTL)))
#define     SETUP_RECORD_TXT_CTL            3

//-----------Setup Display Menu--------------
#define     SETUP_DISPLAY_TXT               ((unsigned long)(SETUP_RECORD_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_RECORD_TXT_CTL)))
#define     SETUP_DISPLAY_TXT_CTL           4

//-------------Auto Play Menu----------------
#define     SETUP_BROWSER_TIME_TXT          ((unsigned long)(SETUP_DISPLAY_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_DISPLAY_TXT_CTL)))
#define     SETUP_BROWSER_TXT_CTL           3

//-----------Setup System Menu---------------
#define     SETUP_SYSTEM_TXT                ((unsigned long)(SETUP_BROWSER_TIME_TXT + ((unsigned long)(MENU_PER_BYTE) * SETUP_BROWSER_TXT_CTL)))
#define     SETUP_SYSTEM_TXT_CTL            4

//-----------Setup Sleep Menu----------------
#define     SETUP_SLEEP_TXT                 ((unsigned long)(SETUP_SYSTEM_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_SYSTEM_TXT_CTL)))
#define     SETUP_SLEEP_TXT_CTL             7

//-----------Setup Format Menu---------------
#define     SETUP_FORMAT_TXT                ((unsigned long)(SETUP_SLEEP_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_SLEEP_TXT_CTL)))
#define     SETUP_FORMAT_TXT_CTL            1

//-----------Setup Resume Menu---------------
#define     SETUP_RESUME_TXT                ((unsigned long)(SETUP_FORMAT_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_FORMAT_TXT_CTL)))
#define     SETUP_RESUME_TXT_CTL            1

//-------Setup Music Repeat Mode-------------
#define     SETUP_MUSIC_MODE_TXT            ((unsigned long)(SETUP_RESUME_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_RESUME_TXT_CTL)))
#define     SETUP_MUSIC_MODE_TXT_CTL        8

//-------Setup Music Play Order--------------
#define     SETUP_MUSIC_ORDER_TXT           ((unsigned long)(SETUP_MUSIC_MODE_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_MUSIC_MODE_TXT_CTL)))
#define     SETUP_MUSIC_ORDER_TXT_CTL       3

//-----------Setup Music Eq------------------
#define     SETUP_MUSIC_EQ_TXT              ((unsigned long)(SETUP_MUSIC_ORDER_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_MUSIC_ORDER_TXT_CTL)))
#define     SETUP_MUSIC_EQ_TXT_CTL          9

//---------Setup Music UserEq----------------
#define     SETUP_MUSIC_USEREQ_TXT          ((unsigned long)(SETUP_MUSIC_EQ_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_MUSIC_EQ_TXT_CTL)))
#define     SETUP_MUSIC_USEREQ_TXT_CTL      1

//---------Setup Radio Stereo----------------
#define     SETUP_RADIO_ST_TXT              ((unsigned long)(SETUP_MUSIC_USEREQ_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_MUSIC_USEREQ_TXT_CTL)))
#define     SETUP_RADIO_ST_TXT_CTL          3

//---------Setup Radio Stereo----------------
#define     SETUP_RADIO_REGION_TXT          ((unsigned long)(SETUP_RADIO_ST_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_RADIO_ST_TXT_CTL)))
#define     SETUP_RADIO_REGION_TXT_CTL      5

//---------Setup Record Quality--------------
#define     SETUP_RECORD_QUALITY_TXT        ((unsigned long)(SETUP_RADIO_REGION_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_RADIO_REGION_TXT_CTL)))
#define     SETUP_RECORD_QUALITY_TXT_CTL    3

//---------Setup Record Volume---------------                
#define     SETUP_RECORD_VOL_TXT            ((unsigned long)(SETUP_RECORD_QUALITY_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_RECORD_QUALITY_TXT_CTL)))
#define     SETUP_RECORD_VOL_TXT_CTL        6

//-------Setup Display BkLight Off Time------
#define     SETUP_DISPLAY_TIME_TXT          ((unsigned long)(SETUP_RECORD_VOL_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_RECORD_VOL_TXT_CTL)))
#define     SETUP_DISPLAY_TIME_TXT_CTL      7

//----------Setup Display Lum Level----------
#define     SETUP_DISPLAY_LUM_TXT           ((unsigned long)(SETUP_DISPLAY_TIME_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_DISPLAY_TIME_TXT_CTL)))
#define     SETUP_DISPLAY_LUM_TXT_CTL       6

//--------Setup Display BkLight Mode---------
#define     SETUP_DISPLAY_MODE_TXT          ((unsigned long)(SETUP_DISPLAY_LUM_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_DISPLAY_LUM_TXT_CTL)))
#define     SETUP_DISPLAY_MODE_TXT_CTL      3

//-----------Text Auto Play Time-------------
#define     SETUP_TEXT_TIME_TXT             ((unsigned long)(SETUP_DISPLAY_MODE_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_DISPLAY_MODE_TXT_CTL)))
#define     SETUP_TEXT_TIME_TXT_CTL         6

//-----------Jpeg Auto Play Time-------------
#define     SETUP_JPEG_TIME_TXT             ((unsigned long)(SETUP_TEXT_TIME_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_TEXT_TIME_TXT_CTL)))
#define     SETUP_JPEG_TIME_TXT_CTL         6

//--------Setup System Language Sel---------
#define     SETUP_SYSTEM_LANGUAGE_TXT       ((unsigned long)(SETUP_JPEG_TIME_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_JPEG_TIME_TXT_CTL)))
#define     SETUP_SYSTEM_LANGUAGE_TXT_CTL   16

//------Setup System Infomation Display------
#define     SETUP_SYSTEM_INFO_TXT           ((unsigned long)(SETUP_SYSTEM_LANGUAGE_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_SYSTEM_LANGUAGE_TXT_CTL)))
#define     SETUP_SYSTEM_INFO_TXT_CTL       1

//-----------Setup System Upgrade------------
#define     SETUP_SYSTEM_UPGRADE_TXT        ((unsigned long)(SETUP_SYSTEM_INFO_TXT     + ((unsigned long)(MENU_PER_BYTE) * SETUP_SYSTEM_INFO_TXT_CTL)))
#define     SETUP_SYSTEM_UPGRADE_TXT_CTL    1

/*              
------------------------------------------------------------------------------------------
* ModuleName :  Dialog Module
* Author     :  tonyzyz
* Description:  Dialog Module Text Address define
* History    :  <author>        <time>     <version>       <desc>
*                tonyzyz     2006-07-25       1.0           ORG
------------------------------------------------------------------------------------------
*/              
#define     MSG_TEXT_TITLE_X                126
#define     MSG_TEXT_TITLE_Y                72
#define     MSG_TEXT_TITLE_XSIZE            96
#define     MSG_TEXT_TITLE_YSIZE            16

#define     MSG_TEXT_CONTENT_X              102
#define     MSG_TEXT_CONTENT_Y              102
#define     MSG_TEXT_CONTENT_XSIZE          127
#define     MSG_TEXT_CONTENT_YSIZE          32

#define     NFA_DIALOG_TEXT                 ((unsigned long)(SETUP_SYSTEM_UPGRADE_TXT  + ((unsigned long)(MENU_PER_BYTE) * SETUP_SYSTEM_UPGRADE_TXT_CTL)))
#define     DIALOG_TEXT_X                   0
#define     DIALOG_TEXT_Y                   0
#define     DIALOG_TEXT_XSIZE               0
#define     DIALOG_TEXT_YSIZE               0

//-------------Dialog Warning---------------
#define     DIALOG_TEXT_TITLE_X             126
#define     DIALOG_TEXT_TITLE_Y             72
#define     DIALOG_TEXT_TITLE_XSIZE         96
#define     DIALOG_TEXT_TITLE_YSIZE         16

#define     DIALOG_TEXT_CONTENT_X           102
#define     DIALOG_TEXT_CONTENT_Y           102
#define     DIALOG_TEXT_CONTENT_XSIZE       127
#define     DIALOG_TEXT_CONTENT_YSIZE       32

#define     DIALOG_WARNING_TXT              NFA_DIALOG_TEXT

//-------------Dialog Yes-------------------
#define     DIALOG_YES_TXT_X                87
#define     DIALOG_YES_TXT_Y                150
#define     DIALOG_YES_TXT_XSIZE            60
#define     DIALOG_YES_TXT_YSIZE            16
#define     DIALOG_YES_TXT                  ((unsigned long)(DIALOG_WARNING_TXT + (unsigned long)(MENU_PER_BYTE)))

//-------------Dialog No--------------------
#define     DIALOG_NO_TXT_X                 175
#define     DIALOG_NO_TXT_Y                 150
#define     DIALOG_NO_TXT_XSIZE             60
#define     DIALOG_NO_TXT_YSIZE             16
#define     DIALOG_NO_TXT                   ((unsigned long)(DIALOG_YES_TXT     + (unsigned long)(MENU_PER_BYTE)))

//-----------Dialog Waiting-----------------
#define     DIALOG_WAIT_TXT                 ((unsigned long)(DIALOG_NO_TXT        + (unsigned long)(MENU_PER_BYTE)))
#define     DIALOG_WAIT_TXT_CTL             4

//-----------Dialog LowPower----------------
#define     DIALOG_LOWPOWER_TXT             ((unsigned long)(DIALOG_WAIT_TXT        + ((unsigned long)(MENU_PER_BYTE) * DIALOG_WAIT_TXT_CTL)))
#define     DIALOG_LOWPOWER_TXT_CTL         4

//-----------Dialog Upgrade-----------------
#define     DIALOG_UPGRADE_TXT              ((unsigned long)(DIALOG_LOWPOWER_TXT    + ((unsigned long)(MENU_PER_BYTE) * DIALOG_LOWPOWER_TXT_CTL)))
#define     DIALOG_UPGRADE_TXT_CTL          4

//-----------Dialog Plug In-----------------
#define     DIALOG_PLUGIN_TXT               ((unsigned long)(DIALOG_UPGRADE_TXT     + ((unsigned long)(MENU_PER_BYTE) * DIALOG_UPGRADE_TXT_CTL)))
#define     DIALOG_PLUGIN_TXT_CTL           4

//---------Dialog Resume Default------------
#define     DIALOG_DEFAULT_TXT              ((unsigned long)(DIALOG_PLUGIN_TXT      + ((unsigned long)(MENU_PER_BYTE) * DIALOG_PLUGIN_TXT_CTL)))
#define     DIALOG_DEFAULT_TXT_CTL          4
                
//-------------Dialog Format----------------
#define     DIALOG_FORMAT_TXT               ((unsigned long)(DIALOG_DEFAULT_TXT     + ((unsigned long)(MENU_PER_BYTE) * DIALOG_DEFAULT_TXT_CTL)))
#define     DIALOG_FORMAT_TXT_CTL           4
                
#define     DIALOG_FORMATING_TXT            ((unsigned long)(DIALOG_FORMAT_TXT      + ((unsigned long)(MENU_PER_BYTE) * DIALOG_FORMAT_TXT_CTL)))
#define     DIALOG_FORMATING_TXT_CTL        4
                
//-----------Dialog File Error---------------
#define     DIALOG_FILEERR_TXT              ((unsigned long)(DIALOG_FORMATING_TXT   + ((unsigned long)(MENU_PER_BYTE) * DIALOG_FORMATING_TXT_CTL)))
#define     DIALOG_FILEERR_TXT_CTL          4
                
//-------------Dialog No File ---------------
#define     DIALOG_NOFILE_TXT               ((unsigned long)(DIALOG_FILEERR_TXT     + ((unsigned long)(MENU_PER_BYTE) * DIALOG_FILEERR_TXT_CTL)))
#define     DIALOG_NOFILE_TXT_CTL           4
                
//-----------Dialog Delete File -------------
#define     DIALOG_DELETE_TXT               ((unsigned long)(DIALOG_NOFILE_TXT      + ((unsigned long)(MENU_PER_BYTE) * DIALOG_NOFILE_TXT_CTL)))
#define     DIALOG_DELETE_TXT_CTL           4
                
#define     DIALOG_DELETING_TXT             ((unsigned long)(DIALOG_DELETE_TXT      + ((unsigned long)(MENU_PER_BYTE) * DIALOG_DELETE_TXT_CTL)))
#define     DIALOG_DELETING_TXT_CTL         4

//---------------------------------------------------------------------------------------
#endif
//******************************************************************************
