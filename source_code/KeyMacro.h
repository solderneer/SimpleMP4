/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : KeyMacro.h
* Author     : tonyzyz
* Description: This Head File Define the Global Varibal For MP3 Decode and WMA Decode
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef _KEYMACRO_H
#define _KEYMACRO_H

#include "macro.h"
#include "Memap.h"

//------------------------------------------------------------------------------
#define KEY_PORT                    GPIO_PDAT0
//
//Key Time Define
#define KEY_TIME_DOWN               2//4
#define KEY_TIME_PRESS_START        20//50
#define KEY_TIME_PRESS_STEP         4//10

//Key Status Define
#define KEY_STATUS_NONE             0xffff
#define KEY_STATUS_DOWN             1
#define KEY_STATUS_PRESS_START      2
#define KEY_STATUS_PRESS            3
#define KEY_STATUS_SHORT_UP         4
#define KEY_STATUS_LONG_UP          5
//Key Val Define
#define KEY_VAL_NONE                0x0000
#define KEY_VAL_PLAY                0x0040
#define KEY_VAL_PLAY_DOWN           ((KEY_VAL_PLAY)|(KEY_STATUS_DOWN << 13))
#define KEY_VAL_PLAY_PRESS_START    ((KEY_VAL_PLAY)|(KEY_STATUS_PRESS_START << 13))
#define KEY_VAL_PLAY_PRESS          ((KEY_VAL_PLAY)|(KEY_STATUS_PRESS << 13))
#define KEY_VAL_PLAY_SHORT_UP       ((KEY_VAL_PLAY)|(KEY_STATUS_SHORT_UP << 13))
#define KEY_VAL_PLAY_LONG_UP        ((KEY_VAL_PLAY)|(KEY_STATUS_LONG_UP << 13))
                                    
#define KEY_VAL_UP                  0x0008
#define KEY_VAL_UP_DOWN             ((KEY_VAL_UP)|(KEY_STATUS_DOWN << 13))           
#define KEY_VAL_UP_PRESS_START      ((KEY_VAL_UP)|(KEY_STATUS_PRESS_START << 13))    
#define KEY_VAL_UP_PRESS            ((KEY_VAL_UP)|(KEY_STATUS_PRESS << 13))          
#define KEY_VAL_UP_SHORT_UP         ((KEY_VAL_UP)|(KEY_STATUS_SHORT_UP << 13))       
#define KEY_VAL_UP_LONG_UP          ((KEY_VAL_UP)|(KEY_STATUS_LONG_UP << 13))        
                                     
#define KEY_VAL_FFD                 0x0004
#define KEY_VAL_FFD_DOWN            ((KEY_VAL_FFD)|(KEY_STATUS_DOWN << 13))           
#define KEY_VAL_FFD_PRESS_START     ((KEY_VAL_FFD)|(KEY_STATUS_PRESS_START << 13))    
#define KEY_VAL_FFD_PRESS           ((KEY_VAL_FFD)|(KEY_STATUS_PRESS << 13))          
#define KEY_VAL_FFD_SHORT_UP        ((KEY_VAL_FFD)|(KEY_STATUS_SHORT_UP << 13))       
#define KEY_VAL_FFD_LONG_UP         ((KEY_VAL_FFD)|(KEY_STATUS_LONG_UP << 13))        

#define KEY_VAL_MENU                0x0001
#define KEY_VAL_MENU_DOWN           ((KEY_VAL_MENU)|(KEY_STATUS_DOWN << 13))           
#define KEY_VAL_MENU_PRESS_START    ((KEY_VAL_MENU)|(KEY_STATUS_PRESS_START << 13))    
#define KEY_VAL_MENU_PRESS          ((KEY_VAL_MENU)|(KEY_STATUS_PRESS << 13))          
#define KEY_VAL_MENU_SHORT_UP       ((KEY_VAL_MENU)|(KEY_STATUS_SHORT_UP << 13))       
#define KEY_VAL_MENU_LONG_UP        ((KEY_VAL_MENU)|(KEY_STATUS_LONG_UP << 13))        

#define KEY_VAL_DOWN                0x0010
#define KEY_VAL_DOWN_DOWN           ((KEY_VAL_DOWN)|(KEY_STATUS_DOWN << 13))           
#define KEY_VAL_DOWN_PRESS_START    ((KEY_VAL_DOWN)|(KEY_STATUS_PRESS_START << 13))    
#define KEY_VAL_DOWN_PRESS          ((KEY_VAL_DOWN)|(KEY_STATUS_PRESS << 13))          
#define KEY_VAL_DOWN_SHORT_UP       ((KEY_VAL_DOWN)|(KEY_STATUS_SHORT_UP << 13))       
#define KEY_VAL_DOWN_LONG_UP        ((KEY_VAL_DOWN)|(KEY_STATUS_LONG_UP << 13))        

#define KEY_VAL_FFW                 0x0002
#define KEY_VAL_FFW_DOWN            ((KEY_VAL_FFW)|(KEY_STATUS_DOWN << 13))           
#define KEY_VAL_FFW_PRESS_START     ((KEY_VAL_FFW)|(KEY_STATUS_PRESS_START << 13))    
#define KEY_VAL_FFW_PRESS           ((KEY_VAL_FFW)|(KEY_STATUS_PRESS << 13))          
#define KEY_VAL_FFW_SHORT_UP        ((KEY_VAL_FFW)|(KEY_STATUS_SHORT_UP << 13))       
#define KEY_VAL_FFW_LONG_UP         ((KEY_VAL_FFW)|(KEY_STATUS_LONG_UP << 13))        

#define KEY_VAL_REC                 0x0020
#define KEY_VAL_REC_DOWN            ((KEY_VAL_REC)|(KEY_STATUS_DOWN << 13))           
#define KEY_VAL_REC_PRESS_START     ((KEY_VAL_REC)|(KEY_STATUS_PRESS_START << 13))    
#define KEY_VAL_REC_PRESS           ((KEY_VAL_REC)|(KEY_STATUS_PRESS << 13))          
#define KEY_VAL_REC_SHORT_UP        ((KEY_VAL_REC)|(KEY_STATUS_SHORT_UP << 13))       
#define KEY_VAL_REC_LONG_UP         ((KEY_VAL_REC)|(KEY_STATUS_LONG_UP << 13))        

#define KEY_VAL_HOLD                0x0021
#define KEY_VAL_HOLD_DOWN           ((KEY_VAL_HOLD)|(KEY_STATUS_DOWN << 13))           
#define KEY_VAL_HOLD_PRESS_START    ((KEY_VAL_HOLD)|(KEY_STATUS_PRESS_START << 13))    
#define KEY_VAL_HOLD_PRESS          ((KEY_VAL_HOLD)|(KEY_STATUS_PRESS << 13))          
#define KEY_VAL_HOLD_SHORT_UP       ((KEY_VAL_HOLD)|(KEY_STATUS_SHORT_UP << 13))       
#define KEY_VAL_HOLD_LONG_UP        ((KEY_VAL_HOLD)|(KEY_STATUS_LONG_UP << 13))        
//------------------------------------------------------------------------------
//#define KEY_BUF_SIZE                1
//                                  
//#define KEY_RPT_DLY                 10
//#define KEY_RPT_START_DLY           50
                                    
//#define KEY_STATE_FREE              1
#define KEY_STATE_DOWN              2
#define KEY_STATE_PRESSED           3
//#define KEY_STATE_RPT_START_DLY     4
//#define KEY_STATE_RPT_DLY           5
#define KEY_STATE_UP                6
//#define KEY_STATE_LCD_LIGHT_ON      7   //zyf 2006-01-13
                                    
                                    
#define KEY_NULL                    KEY_VAL_NONE
#define KEY_PLAY                    0x0001
#define KEY_EQ                      0x0002
#define KEY_MODE                    0x0004
#define KEY_MENU                    0x0008
#define KEY_AB                      0x0010
#define KEY_FFW                     0x0020
#define KEY_DOWN                    0x0040
#define KEY_REC                     0x0080
#define KEY_FFD                     0x0100
#define KEY_UP                      0x0200
                                    
#define KEY_LONG_MASK               0x8000
#define KEY_PLAY_L                  KEY_PLAY | KEY_LONG_MASK
#define KEY_EQ_L                    KEY_EQ   | KEY_LONG_MASK
#define KEY_MODE_L                  KEY_MODE | KEY_LONG_MASK
#define KEY_MENU_L                  KEY_MENU | KEY_LONG_MASK
#define KEY_AB_L                    KEY_AB   | KEY_LONG_MASK
#define KEY_FFW_L                   KEY_FFW  | KEY_LONG_MASK
#define KEY_DOWN_L                  KEY_DOWN | KEY_LONG_MASK
#define KEY_REC_L                   KEY_REC  | KEY_LONG_MASK
#define KEY_FFD_L                   KEY_FFD  | KEY_LONG_MASK
#define KEY_UP_L                    KEY_UP   | KEY_LONG_MASK
//------------------------------------------------------------------------------
#endif                              
//******************************************************************************
