/********************************************************************************
*********************************************************************************
            COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
                --  ALL RIGHTS RESERVED  --

File Name:  config.h
Author:     XUESHAN LIN
Created:        22th Nov. 2005
Modified:
Revision:       1.00
********************************************************************************
********************************************************************************/
#ifndef _FWCONFIG_H
#define _FWCONFIG_H
#define     SYS_CLOCK       48
#define     USB_HS_EN       //????????USB HS

#ifndef _CONFIG_H
typedef     unsigned char   uint8;
typedef     signed char     int8;
typedef     unsigned int    uint16;
typedef     signed int      int16;
typedef     unsigned long   uint32;
typedef     signed long     int32;
typedef     unsigned char   bool;
#endif

#include        "Memap.h"
#include        "FWEpphal.h"        //????????????

#include        "FWUSB20.h"     //USB????????????
#include        "FW_Upgrade.h"  //FW_Upgrade??????
#include        "FWflash.h"         //FLASH??????
#endif

