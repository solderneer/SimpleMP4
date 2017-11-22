/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : FmMacro.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/09/17       1.0            ORG
*
********************************************************************************
*/
#ifndef  _FMMACRO_H
#define  _FMMACRO_H
//------------------------------------------------------------------------------
#define    false                    0x00
#define    ACTIVE			        !(FALSE)

#define    FM_GUAGE_CONST           184
#define    FM_SAVE_BUF_MAX          20

//------------------------------------------------------------------------------
#define    TUNER_HIGHT_IF           0x3e
#define    YUNER_LOW_IF             0x3e

#define    TUNER_ADD_W              0xc0            /* TEA5767HN I2C slave address write DEVID */
#define    TUNER_ADD_R              0xc1            /* TEA5767HN I2C slave address write DEVID */

#define    TUNERFREQINDEX           20
#define    TUNERFREQSTEP            10              /* UP/DOWN TUNER frequency STEP            */

#define    TUNERBAND_UPPERLIMIT     10800           /* 108Mhz for high band limit              */
#define    TUNERBAND_LOWERLIMIT     8750            /* 87.5Mhz for low band limit              */

#define    TUNER_REFFREQ            32768           /* tuner reference frequency, 32.768Khz    */

//------------------------------------------------------------------------------
/* bit mask value for the 1st data in write mode    */
#define    TUNER_MUTE               0x80            /* 1: mute enabled, 0: not mute                     */
#define    TUNER_SM                 0x40            /* search mode, 1:search, 0:not search mode         */
#define    TUNER_PLLHIGH            0x3f            /* PLL[13:8] value                                  */

/* bit mask value for the 2nd data in write mode    */
#define    TUNER_PLLLOW             0xff            /* PLL[7:0] value                                   */

/* bit mask value for the 3rd data in write mode    */
#define    TUNER_SUD                0x80            /* search up/down, 1: search up, 0: search down     */
#define    TUNER_SSL                0x20            /* search stop level, datasheet pp.11, Table 12     */
#define    TUNER_HLSI               0x10            /* Hight/Low side injection, 1: high, 0: low        */
#define    TUNER_MS                 0x08            /* Mono/Stereo, 1: mono, 0:stereo                   */
#define    TUNER_ML                 0x04            /* Mute Left, 1: left ch. muted, forced mono        */
#define    TUNER_MR                 0x02            /* Mute right, 1: right ch. muted, forced mono      */
#define    TUNER_SWP1               0x01            /* Software programmable port1, 1: port1 is high    */

/* bit mask value for the 4th data in write mode    */
#define    TUNER_SWP2               0x80            /* Software programmable port2, 1: port2 is high    */
#define    TUNER_STBY               0x40            /* Standby, 1: forced to standby state              */
#define    TUNER_BL                 0x20            /* Band Limit, 1: Japan FM band, 0: US/Europe FM    */
#define    TUNER_XTAL               0x10            /* Select xtal, 1: 32.768kHz, 0: 13MHz              */
#define    TUNER_SMUTE              0x08            /* Soft Mute, 1: mute on, 0: mute off               */
#define    TUNER_HCC                0x04            /* High Cut Control, 1: on, 0: off                  */
#define    TUNER_SNC                0x02            /* Stereo noise cancelling, 1: on, 0: off           */
#define    TUNER_SI                 0x01            /* Search indicator, 1: SWPORT1 is ready flag       */

/* bit mask value for the 5th data in write mode    */
#define    TUNER_PLLREF             0x80            /* 1: 6.5MHz reference, 0: not enabled              */
#define    TUNER_DTC                0x40            /* 1: de-emphasis time constants is 75us, 0: 50us   */

/* bit mask value for the 1st data in read mode     */
#define    TUNER_RF                 0x80            /* Ready Flag, 1: a station has been found          */
#define    TUNER_BLF                0x40            /* Band Limit Flag, 1: band limit has been reached  */

/* bit mask value for the 3rd data in read mode     */
#define    TUNER_STEREO             0x80            /* Stereo indication, 1: stereo reception, 0: mono  */

/* bit mask value for the 4th data in read mode     */
#define    TUNER_LEV                0xf0            /* Level ADC output                                 */
#define    TUNER_CI                 0x0e            /* Chip identification, these bit must be set to 0  */
//------------------------------------------------------------------------------
#define    RFF                      0x10
#define    RFNE                     0x08
#define    TFE                      0x04
#define    TFNF                     0x02
#define    ACTIVITY                 0x01
#define    ENABLE                   0

#define    IIC_ENABLE               bitset_mem(IC_ENABLE, ENABLE)
#define    IIC_DISABLE              bitclear_mem(IC_ENABLE, ENABLE)
//------------------------------------------------------------------------------
enum {
	TUN_UP,
	TUN_DOWN,
	SEARCH_UP,
	SEARCH_DOWN,	
	SEARCH_PRESET
};

//------------------------------------------------------------------------------
#define     FM_DEL_CHANNEL      0x0001
#define     FM_SAVE_CHANNEL     0x0002

#define     FM_DEL_SAVE_NO      0
#define     FM_DEL_SAVE_YES     1

#define     FM_STATE_MENU       255
#define     FM_STATE_MANUAL     0
#define     FM_STATE_PRESET     1
#define     FM_STATE_SAVE       2
#define     FM_STATE_DEL        3
#define     FM_STATE_AUTO       4
//------------------------------------------------------------------------------
#define     FM_DISPFLAG_NULL                ((unsigned long)(0x00000000))

#define     FM_DISPFLAG_BACKGROUND          ((unsigned long)(0x00000001))
#define     FM_DISPFLAG_FREQ                ((unsigned long)(0x00000002))
#define     FM_DISPFLAG_GUAGE               ((unsigned long)(0x00000004))
#define     FM_DISPFLAG_BATT                ((unsigned long)(0x00000008))
#define     FM_DISPFLAG_SENS                ((unsigned long)(0x00000010))
#define     FM_DISPFLAG_STEREO              ((unsigned long)(0x00000020))
#define     FM_DISPFLAG_CH                  ((unsigned long)(0x00000040))
#define     FM_DISPFLAG_VOL                 ((unsigned long)(0x00000080))
#define     FM_DISPFLAG_REGION              ((unsigned long)(0x00000100))
#define     FM_DISPFLAG_HOLD                ((unsigned long)(0x00000200))
//#define     FM_DISPFLAG_MUTE                ((unsigned long)(0x00000008))
//#define     FM_DISPFLAG_MODE                ((unsigned long)(0x00000080))

#define     FM_DISPFLAG_SUBMENU_BK          ((unsigned long)(0x00010000))
#define     FM_DISPFLAG_SUBMENU_SEL         ((unsigned long)(0x00020000))
#define     FM_DISPFLAG_LIST_BK             ((unsigned long)(0x00040000))
#define     FM_DISPFLAG_LIST_SEL            ((unsigned long)(0x00080000))
#define     FM_DISPFLAG_DIALOG_BK           ((unsigned long)(0x00100000))
#define     FM_DISPFLAG_DIALOG_SEL          ((unsigned long)(0x00200000))

#define     FM_DISPFLAG_ALL                 ((unsigned long)(0xffffffff))
//------------------------------------------------------------------------------
#endif
//******************************************************************************

