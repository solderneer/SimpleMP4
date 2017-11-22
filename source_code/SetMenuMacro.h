/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : SetMenuMacro.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _SETMENUMACRO_H
#define _SETMENUMACRO_H
//------------------------------------------------------------------------------
#define SET_MENU_TEXT __attribute__((section(".setmenu_text, \"ax\"")))

#ifdef		FM_MODULE
#define     SET_MENU_ITEM_TOTAL     24
#else
#define     SET_MENU_ITEM_TOTAL     21
#endif

#define     SET_SCREEN_PER_LINE     9

#define SET_ITEM_DISP_SPACE     27
#define SET_ITEM_ICON_X         52
#define SET_ITEM_ICON_Y         9
#define SET_ITEM_DISP_X         SET_ITEM_ICON_X
#define SET_ITEM_DISP_Y         SET_ITEM_ICON_Y
#define SET_SCROLL_BAR_COUNT    (240 - 50 -21)
//unsigned int Id3Switch;
//unsigned int PlaySpeed;
//
//unsigned int SolidSwState;
//unsigned int RadioRegion;

#define REPEATSTATE         GlobalMusicVar.Mode
#define PLAYMODE            GlobalMusicVar.Order
#define EQSTATE             GlobalMusicVar.Eq
#define ID3SWITCHSTATE      Id3Switch     
#define PLAYSPEED           PlaySpeed          

#define SOLIDSWSTATE        GlobalFmVar.FmStereo      
#define RADIOREGION         GlobalFmVar.FmArea            

//#define LINEINQUALITY       GlobalEncVar.LineInQuality
#define MICINQUALITY        GlobalEncVar.MicQuality
//#define FMINQUALITY         GlobalEncVar.FmRecQuality
#define RECORDMICVOL        MicInputVolume
//#define RECORDLINEVOL       LineInputVolume
#define AUTOSWSTATE         GlobalEncVar.AutoDevideSection

#define BKLIGHT             LightOffTime
#define LCDLUM              LigthLevel
#define LCDDISPMODE         DisplayMode

#define LANGUAGESTATE       Language
#define MEMORY_SELECT       MemorySelect

#define POWEROFFTIMER       PowerOffTime
#define PICBROWSERTIMER     PicBrowserTime

//------------------------------------------------------------------------------
//  添加文本和图片自动浏览功能所需的变量
//------------------------------------------------------------------------------
#define S_TXTAUTOBROWSERTIME    gTxtAutoBrowserTime
#define S_JPEGAUTOBROWSERTIME   gJpegAutoBrowserTime

//------------------------------------------------------------------------------
#endif
//******************************************************************************

