/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : Main.c
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/12/12       1.0            ORG
*
********************************************************************************
*/
#define _IN_AUDIO

#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "Global.h"
#include "MsgDef.h"

//unsigned int CodecPowerDownFlag;
void OutputVolumeSet(unsigned int bVol);
/*------------------------------------------------------------------------------
Function: Rock2 Mp3 Timer0 Isr
Input   :
Output  :
------------------------------------------------------------------------------*/
void CodecIicInit(void)
{
    AVI_MP3_DECODE_DISABLE;
    
    write_mem(GPIO_PCON1,(read_mem(GPIO_PCON1) & 0xffc3));//00,00,00,00,00,11,11,00
    
    write_mem(IC_ENABLE,    0x0000);        //IIC Transfer Disable
    write_mem(IC_CON,       0x0065);        //Master Mode, 7Bit Address, Fast Mode
    write_mem(IC_FS_HCNT,   0x0020);//CurrCpuFreq);        //IIC Wave Hight Level Width
    write_mem(IC_FS_LCNT,   0x0044);//CurrCpuFreq << 1);        //IIC Wave Low Level Width
    write_mem(IC_TAR,       0x0055);        //Codec IIC Address
    write_mem(I2C_EXT,      0x0000);        //To Internal Codec
    write_mem(IC_ENABLE,    0x0001);        //IIC Transfer Enable
    AVI_MP3_DECODE_ENABLE;
    
    delay_nops(1600);
}
/*------------------------------------------------------------------------------
Function: Rock2 Mp3 Timer0 Isr
Input   :
Output  :
------------------------------------------------------------------------------*/
void CodecCommandSet(unsigned int uCmd, unsigned int uData)
{
    unsigned int i,temp;
    
    AVI_MP3_DECODE_DISABLE;
    write_mem(GPIO_PCON1,(read_mem(GPIO_PCON1) & 0xffc3));//00,00,00,00,00,11,11,00
    
    write_mem(IC_DATA_CMD,uCmd);
    write_mem(IC_DATA_CMD,uData);
    AVI_MP3_DECODE_ENABLE;
    
    delay_nops(8000);
    //---------------------------------
    
    AVI_MP3_DECODE_DISABLE;
    write_mem(IC_DATA_CMD,uCmd);
    write_mem(IC_DATA_CMD,0x100);
    AVI_MP3_DECODE_ENABLE;
    
    delay_nops(8000);
    for(i = 0; i < 200; i++) {
    //while(1){
        temp = read_mem(IC_STATUS);
        if (temp & 0x0008)
            break;
    }
    temp = read_mem(IC_DATA_CMD);
    //---------------------------------
}
/*------------------------------------------------------------------------------
Function: Rock2 Codec Power On initial
Input   :
Output  :
------------------------------------------------------------------------------*/
__attribute__((section(".flash_init_text, \"ax\"")))
void CodecPowerOnInitial(void)
{
    unsigned int i;
    
    write_mem(GPIO_PCON1,read_mem(GPIO_PCON1) & 0xffc3);//p1.1 p1.2 input
//------------------------------------------
    write_mem(SW_CODEC_RSTN,0x0000);
    Delay1Ms(20);
    write_mem(SW_CODEC_RSTN,0x0001);        //IIC Reset
    write_mem(IC_ENABLE,    0x0000);        //IIC Transfer Disable
    write_mem(IC_CON,       0x0065);        //Master Mode, 7Bit Address, Fast Mode
    write_mem(IC_FS_HCNT,   0x0020);        //IIC Wave Hight Level Width
    write_mem(IC_FS_LCNT,   0x0044);        //IIC Wave Low Level Width
    write_mem(IC_TAR,       0x0055);        //Codec IIC Address
    write_mem(I2C_EXT,      0x0000);        //To Internal Codec
    write_mem(IC_ENABLE,    0x0001);        //IIC Transfer Enable
//------------------------------------------
    CodecCommandSet(P_ADD_CGR4,0x0000);
    
//------------------------------------------
#ifndef  _INSIDE_HPSENSE_USED_    
    CodecCommandSet(P_ADD_TR1,  0x0002);
#else 
    CodecCommandSet(P_ADD_TR1,  0x0000);
#endif

    Pre_CR1_Value = 0x0028;
    
    CodecCommandSet(P_ADD_CCR,  0x0011);    //12MHZ,44.1KHZ/44.1KHZ
    CodecCommandSet(P_ADD_CR1,  0x0028);
    CodecCommandSet(P_ADD_CR2,  0x0060);    //24BIT

    CodecCommandSet(P_ADD_CGR1, 0x00ff);
    CodecCommandSet(P_ADD_CGR2, 0x001f);
    CodecCommandSet(P_ADD_CGR3, 0x001f);
    CodecCommandSet(P_ADD_CGR4, 0x001f);
    CodecCommandSet(P_ADD_CGR5, 0x001f);
    CodecCommandSet(P_ADD_CGR6, 0x001f);
    CodecCommandSet(P_ADD_CGR7, 0x001f);
    CodecCommandSet(P_ADD_CGR8, 0x001f);
    CodecCommandSet(P_ADD_CGR9, 0x001f);

#if(CPU_TYPE == _RK2608A_)
    CodecCommandSet(P_ADD_PMR1, 0x001f);    //All Power Down
    CodecCommandSet(P_ADD_PMR2, 0x0008);    //All Power Down
    Delay1Ms(100);

//-----------------------------------------------------------------
    write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0xffdf));  //p1.5--->VDDA控制
    Delay1Ms(100);
    write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0xfff7)); //p1.3--->VDDAO控制
    Delay1Ms(100);
//-----------------------------------------------------------------
    CodecCommandSet(P_ADD_CGR4,0x0000);

#ifndef  _INSIDE_HPSENSE_USED_    
    CodecCommandSet(P_ADD_TR1,  0x0002);
#else 
    CodecCommandSet(P_ADD_TR1,  0x0000);
#endif
    
    CodecCommandSet(P_ADD_CGR4,0x001f);
    
    CodecCommandSet(P_ADD_PMR1, 0x001f);    //All Power Down
    CodecCommandSet(P_ADD_PMR2, 0x0008);    //All Power Down
    Delay1Ms(100);
//-----------------------------------------------------------------
#ifdef  _INSIDE_HPSENSE_USED_               
    CodecCommandSet(P_ADD_TR1, 0x0002);
	Delay1Ms(100);
	CodecCommandSet(P_ADD_TR1, 0x0000);
    Delay1Ms(100);
#endif   

#else
    CodecCommandSet(P_ADD_PMR1, 0x003f);    //All Power Down
    CodecCommandSet(P_ADD_PMR2, 0x0008);    //All Power Down
    Delay1Ms(100);
#endif
}
/*------------------------------------------------------------------------------
Function: Rock2 Codec Power Off 
Input   :
Output  :
------------------------------------------------------------------------------*/
void CodecPowerOff(void)
{
    unsigned int i;
    
    CodecIicInit();
    
////------------------------------------------
//#ifndef  _INSIDE_HPSENSE_USED_    
//    CodecCommandSet(P_ADD_TR1,  0x0002);
//#else 
//    CodecCommandSet(P_ADD_TR1,  0x0000);
//#endif

    Pre_CR1_Value = 0x0028;
    CodecCommandSet(P_ADD_CCR,  0x0011);    //12MHZ,44.1KHZ/44.1KHZ
    CodecCommandSet(P_ADD_CR1,  0x0028);
    CodecCommandSet(P_ADD_CR2,  0x0060);    //24BIT
    CodecCommandSet(P_ADD_CGR1, 0x00ff);
    CodecCommandSet(P_ADD_CGR2, 0x001f);
    CodecCommandSet(P_ADD_CGR3, 0x001f);
    CodecCommandSet(P_ADD_CGR4, 0x001f);
    CodecCommandSet(P_ADD_CGR5, 0x001f);
    CodecCommandSet(P_ADD_CGR6, 0x001f);
    CodecCommandSet(P_ADD_CGR7, 0x001f);
    CodecCommandSet(P_ADD_CGR8, 0x001f);
    CodecCommandSet(P_ADD_CGR9, 0x001f);
    Delay1Ms(100);
    CodecCommandSet(P_ADD_PMR1, 0x00ff);    //All Power Down
    CodecCommandSet(P_ADD_PMR2, 0x000b);    //All Power Down
    Delay1Ms(100);
#if(CPU_TYPE == _RK2608A_)
    write_mem(GPIO_PDAT1,read_mem(GPIO_PDAT1)|0x0008);; //p1.3--->VDDAO控制
    Delay1Ms(100);
    write_mem(GPIO_PDAT1,read_mem(GPIO_PDAT1)|0x0020);  //p1.5--->VDDA控制
    Delay1Ms(100);
#endif

}
/*------------------------------------------------------------------------------
Function: 
Input   :
Output  :
------------------------------------------------------------------------------*/
void MusicCodecMute(short mode)
{
    CodecIicInit();
    
    if(mode) {
        Pre_CR1_Value |= 0x0020;
        OutputVolumeSet(0);
    } else {
        Pre_CR1_Value &= 0xffdf;
        OutputVolumeSet(OutputVolume);
    }
    Delay1Ms(80);
}
/*------------------------------------------------------------------------------
Function: Rock2 Codec Power On initial
Input   :
Output  :
------------------------------------------------------------------------------*/
void CodecPowerOn(void)
{
//    if (CodecPowerDownFlag == 1) {
//        CodecPowerDownFlag = 0;
//        CodecIicInit();
//        if(Sys1InitFlag.bc.bEncodeModuleInitEnable == 0) {
//            CodecCommandSet(P_ADD_PMR2, 0x0000);
//        } else {
//            CodecCommandSet(P_ADD_PMR2, 0x0008);
//        }
//        Delay1Ms(200);
//    }
}
/*------------------------------------------------------------------------------
Function: Rock2 Codec Power On initial
Input   :
Output  :
------------------------------------------------------------------------------*/
void CodecPowerDown(void)
{
//    if (CodecPowerDownFlag == 0) {
//        CodecPowerDownFlag = 1;
//        CodecIicInit();
////        if(Sys1InitFlag.bc.bEncodeModuleInitEnable == 0) {
////            CodecCommandSet(P_ADD_PMR2, 0x0002);
////        } else {
//            CodecCommandSet(P_ADD_PMR2, 0x000a);
////        }
//        Delay1Ms(200);
//    }
}

/*------------------------------------------------------------------------------
Function: Rock2 Output Volume Set
Input   :
Output  :
------------------------------------------------------------------------------*/
void OutputVolumeSet(unsigned int Vol)
{
//    AVI_MP3_DECODE_DISABLE;
    unsigned int bVol = Vol * 7 / 10;
    
    CodecIicInit();
    if (bVol > 8) {
        CodecCommandSet(P_ADD_CR1,  Pre_CR1_Value/*0x0008*/);
        
        CodecCommandSet(P_ADD_CGR1, 0x0000);
        CodecCommandSet(P_ADD_CGR2, 0x0000);
        CodecCommandSet(P_ADD_CGR3, 0x0000);
        CodecCommandSet(P_ADD_CGR4, 0x0000);
        CodecCommandSet(P_ADD_CGR5, 0x0000);
        CodecCommandSet(P_ADD_CGR6, 0x0000);
        CodecCommandSet(P_ADD_CGR7, 0x0000);
        CodecCommandSet(P_ADD_CGR8, (40 - bVol) & 0x001f);
        CodecCommandSet(P_ADD_CGR9, (40 - bVol) & 0x001f);
    } else if (bVol > 0) {
        CodecCommandSet(P_ADD_CR1,  Pre_CR1_Value/*0x0008*/);
        
        CodecCommandSet(P_ADD_CGR1, ((15 - ((bVol-1) << 1)) <<4 ) + (15 - ((bVol-1) << 1)));
        CodecCommandSet(P_ADD_CGR2, (30 - ((bVol-1) << 2)));
        CodecCommandSet(P_ADD_CGR3, (30 - ((bVol-1) << 2)));
        CodecCommandSet(P_ADD_CGR4, (30 - ((bVol-1) << 2)));
        CodecCommandSet(P_ADD_CGR5, (30 - ((bVol-1) << 2)));
        CodecCommandSet(P_ADD_CGR6, (30 - ((bVol-1) << 2)));
        CodecCommandSet(P_ADD_CGR7, (30 - ((bVol-1) << 2)));
        CodecCommandSet(P_ADD_CGR8, 0x001f);
        CodecCommandSet(P_ADD_CGR9, 0x001f);
    } else {
        CodecCommandSet(P_ADD_CR1,  0x0028);
        CodecCommandSet(P_ADD_CGR1, 0x00ff);
        CodecCommandSet(P_ADD_CGR2, 0x001f);
        CodecCommandSet(P_ADD_CGR3, 0x001f);
        CodecCommandSet(P_ADD_CGR4, 0x001f);
        CodecCommandSet(P_ADD_CGR5, 0x001f);
        CodecCommandSet(P_ADD_CGR6, 0x001f);
        CodecCommandSet(P_ADD_CGR7, 0x001f);
        CodecCommandSet(P_ADD_CGR8, 0x001f);
        CodecCommandSet(P_ADD_CGR9, 0x001f);
    }
//    AVI_MP3_DECODE_ENABLE;
}

/*------------------------------------------------------------------------------
Function: Rock2 Output Volume Set
Input   :
Output  :
------------------------------------------------------------------------------*/
void OutputVolumeInc(void)
{
    if (OutputVolume < 40){
        OutputVolume ++ ;
        OutputVolumeSet(OutputVolume);
    }
}

/*------------------------------------------------------------------------------
Function: Rock2 Output Volume Set
Input   :
Output  :
------------------------------------------------------------------------------*/
void OutputVolumeDec(void)
{
    if (OutputVolume != 0){
        OutputVolume -- ;
        CodecIicInit();
        OutputVolumeSet(OutputVolume);
    }
}
//*******************************************************************************
#ifdef  _INSIDE_HPSENSE_USED_
/*------------------------------------------------------------------------------
Function: Rock2 CODEC 寄存器读取
Input   :
Output  :
------------------------------------------------------------------------------*/

unsigned int CodecCommandRead(unsigned int uCmd)
{
    unsigned int i,temp;
    
    write_mem(IC_DATA_CMD,uCmd);
    write_mem(IC_DATA_CMD,0x100);
    delay_nops(8000);
    for(i = 0; i < 5000; i++) 
	{
    	;
    	temp = read_mem(IC_STATUS);
        if (temp & 0x0008)
            break;            
    }
    temp = read_mem(IC_DATA_CMD);
    return temp;
}

/*------------------------------------------------------------------------------
Function: Rock2 CODEC 获取耳机插入状态
Input   :
Output  :
------------------------------------------------------------------------------*/
bool GetHpState(void)
{
	unsigned int Temp1,Temp2;
	unsigned int Temp;
	
	CodecIicInit();
	Temp1 = CodecCommandRead (P_ADD_CR2);          //CCMC
	if(Temp1 >=0x80) 			                        //若短路保护
	{
        Temp = Pre_CR1_Value;
		MusicCodecMute(1);
    	CodecCommandSet(P_ADD_TR1, 0x0002);
    	delay_nops(10);
    	CodecCommandSet(P_ADD_TR1, 0x0000);
    	if ((Temp & 0x0020) == 0) {
        	MusicCodecMute(0);
        }
	}

	if(Sys1InitFlag.bc.bFmModuleInitEnable == 0) {
	    return TRUE;
	} else {
	    Temp2 = CodecCommandRead (P_ADD_CR1); 			//JACK
	    return (Temp2 <0x80);							//返回耳机状态位
	}
}
#endif 
