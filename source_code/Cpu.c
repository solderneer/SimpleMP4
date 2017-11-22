#include <creg.h>
#include "Macro.h"
#include "Global.h"
#include "MsgDef.h"

/*------------------------------------------------------------------------------
Function: 
Input   :
Output  : 80MHZ < Fvco < 200MHZ
------------------------------------------------------------------------------*/
void SysCpuSetFreq(unsigned int wFreq) 
{
    unsigned int imask_temp;
    unsigned int a,b,c,lcdwaitcont;
    
    if (CurrCpuFreq == wFreq)
        return;
    
    read_creg(%imask,imask_temp);
    bitclear_creg(%imask,15);
//------------------------------------------------------------------------------
    if (((24 % CurrCpuFreq) == 0 ) && ((24 % wFreq) == 0)) {
        write_mem(CLOCK_MCLKCON,(24/wFreq) - 1);
        delay_nops(400);
    } else {
        write_mem(CLOCK_PLL_PDBP,   0x0001);
        write_mem(CLOCK_MCLKCON,    0x0000);
        delay_nops(10);
        write_mem(CLOCK_AHBCLKCON,  0x0000);    //DMA,MEMCTR Clock
        write_mem(CLOCK_APBCLKCON,  0x0000);    //SD/MMC Clock, ADC/PWM Clock
        write_mem(CLOCK_PLL_ODDIV,  0x0000);    //PLL_DIV = 1
            
        if (((wFreq % 17) == 0) && (wFreq <= 68)) {
            write_mem(CLOCK_PLL_NDIV,   0x000f);    //N = 17, 24/17 = 1.4117
            write_mem(CLOCK_PLL_MDIV,   0x008e);    //0x0046
        }
        else if (wFreq > 51) {              //102MHZ--200MHZ
            write_mem(CLOCK_PLL_NDIV,   0x000a);    //24/12
            write_mem(CLOCK_PLL_MDIV,   (wFreq-2));
        } 
        else if (wFreq > 33) {              //102MHZ--150HMZ
            write_mem(CLOCK_PLL_NDIV,   0x0006);    //24/8
            write_mem(CLOCK_PLL_MDIV,   (wFreq-2));
        } 
        else {                              //150MHZ--192MHZ
            write_mem(CLOCK_PLL_NDIV,   0x0002);    //24/4
            write_mem(CLOCK_PLL_MDIV,   (wFreq-2));
        }
//------------------------------------------------------------------------------
        write_creg(%imask,imask_temp);
        delay_nops(12000);
        bitclear_creg(%imask,15);
//------------------------------------------------------------------------------
        if (((wFreq % 17) == 0) && (wFreq <= 68)) {
            write_mem(CLOCK_MCLKCON,    (203/(wFreq - 1)) - 1);    //Mclk = 203.6 / 3 = 16.9344 * 4
            if (wFreq > 51) {
                delay_nops(10);
                write_mem(CLOCK_APBCLKCON,  0x0001);
            }
        }
        else if (wFreq > 51) {              //102MHZ--200MHZ
            write_mem(CLOCK_MCLKCON,    0x0001);
            delay_nops(10);
            write_mem(CLOCK_APBCLKCON,  0x0001);
        } 
        else if (wFreq > 33) {              //102MHZ--150HMZ
            write_mem(CLOCK_MCLKCON,    0x0002);
        } 
        else {                              //150MHZ--192MHZ
            write_mem(CLOCK_MCLKCON,    0x0005);
        }
        delay_nops(10);
        if ((24 % wFreq) == 0 ) {
            write_mem(CLOCK_MCLKCON,    (24/wFreq) - 1);
            write_mem(CLOCK_PLL_PDBP,   0x0003);    //PLL_POWER_OFF / PLL_BYPASS    
        } else {
            write_mem(CLOCK_PLL_PDBP,   0x0000);
        }
        delay_nops(400);
    }
//------------------------------------------------------------------------------
    write_mem(LCDWAIT, 0x20c3);
    write_mem(FMWAIT,  0x1061);
//------------------------------------------------------------------------------
    CurrCpuFreq = wFreq;
    CurrApbFreq = wFreq;
    if (CurrCpuFreq > 51) CurrApbFreq = wFreq >> 1;
    
    write_XDATA(PWM_TAPRE , (CurrApbFreq - 1));  //pre scale
    write_mem(ADC_ADCFRE, (CurrApbFreq -1));
    
    write_creg(%imask,imask_temp);
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
注释    ： <1> 当wFreq = 0时，定时器0关闭
           <2> 当wFreq <= 64M时, 定时时间范围为 1uS -- 65,535uS
           <3> 当wFreq <= 128M时,定时时间范围为 0.5uS -- 32,767uS
------------------------------------------------------------------------------*/
void SysTimer0Set(unsigned int wFreq, unsigned int wCount)
{
    unsigned int temp,imask_temp;
    
    read_creg(%imask,imask_temp);
    bitclear_creg(%imask,15);
    
    Timer0Val = wCount;
    read_creg(%tc,temp);
    if (wFreq == 0) {
        write_creg(%tc,(temp & 0xff00));
    } else if (wFreq <= 64) {
        write_creg(%timer0,wCount);
        write_creg(%tc,((temp & 0xff00) | (0x00c0 + (wFreq - 1))));
    } else {
        write_creg(%timer0,(wCount << 1));
        write_creg(%tc,((temp & 0xff00) | (0x00c0 + ((wFreq >> 1) - 1))));
    }
    write_creg(%imask,imask_temp);
}

/*------------------------------------------------------------------------------
Function: 
Input   :
Output  :
注释    ： <1> 当wFreq = 0时，定时器1关闭
           <2> 当wFreq <= 64M时, 定时时间范围为 1uS -- 65,535uS
           <3> 当wFreq <= 128M时,定时时间范围为 0.5uS -- 32,767uS
------------------------------------------------------------------------------*/
/*
void SysTimer1Set(unsigned int wFreq, unsigned int wCount)
{
    unsigned int temp,imask_temp;
    
    read_creg(%imask,imask_temp);
    bitclear_creg(%imask,15);
    
    read_creg(%tc,temp);
    if (wFreq == 0) {
        write_creg(%tc,(temp & 0x00ff));
    } else if (wFreq <= 64) {
        write_creg(%timer0,wCount);
        write_creg(%tc,((temp & 0x00ff) | (0xc000 + ((wFreq - 1) << 4 ))));
    } else {
        write_creg(%timer0,(wCount << 1));
        write_creg(%tc,((temp & 0x00ff) | (0xc000 + (((wFreq >> 1) - 1) << 4))));
    }
    write_creg(%imask,imask_temp);
}
*/
/*------------------------------------------------------------------------------
Function: 
Input   :
Output  : 80MHZ < Fvco < 200MHZ
------------------------------------------------------------------------------*/
void I2sClkConSet(unsigned int ConVal)
{
    //if (((ConVal == 0x0100) && (I2sClockCon != 0x0100)) || ((ConVal != 0x0100) && (I2sClockCon == 0x0100))) {
    if ((ConVal & 0x0100) != (I2sClockCon & 0x0100)) {
        write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) & 0xfffd);
        delay_nops(5);
    }
    write_mem(CLOCK_I2SMCLKCON, ConVal);
    I2sClockCon = ConVal;
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0002);
}
/*------------------------------------------------------------------------------
Function: 
Input   :
Output  : 80MHZ < Fvco < 200MHZ
------------------------------------------------------------------------------*/
void MusicSetFreq51(void) 
{
//    PrevCpuFreq = CurrCpuFreq;
//    if (CurrCpuFreq < 51) {
//        SysCpuSetFreq(51);
//        SysTimer0Set(51,25000);
//    }
}

/*------------------------------------------------------------------------------
Function: 
Input   :
Output  : 80MHZ < Fvco < 200MHZ
------------------------------------------------------------------------------*/
void MusicSetFreq51Return(void) 
{
//    SysCpuSetFreq(PrevCpuFreq);
//    SysTimer0Set(PrevCpuFreq,25000);//SysTimer0Set(PrevCpuFreq,10000);
}

#define  LCD_BACK_LIGHT_PWM_TADATA1     100

unsigned int LightLevelBuffer[6] = {90, 69, 48, 27, 6, 96};
/*------------------------------------------------------------------------------
Function: 
Input   :
Output  :
------------------------------------------------------------------------------*/
void LcdBackLevelSet(unsigned int Level)
{   
    unsigned int i;
{///////////////////////////////////////////////////////////////////////////////
//    for (i = 0; i < LightLevelBuffer[Level]; i++)
//    {
//      write_mem(GPIO_PDAT2,read_mem(GPIO_PDAT2) & 0xdfff);
//      delay_nops(CurrCpuFreq);
//      write_mem(GPIO_PDAT2,read_mem(GPIO_PDAT2) | 0x2000);
//      delay_nops(CurrCpuFreq);
//    }
}///////////////////////////////////////////////////////////////////////////////
    write_XDATA(PWM_TACMD , 0x0000);             //disable PWM_TACMD
    write_XDATA(PWM_TAPRE , (CurrApbFreq - 1));  //pre scale
    write_XDATA(PWM_TADATA0 , LightLevelBuffer[Level]);     //TA_DATA
    write_XDATA(PWM_TADATA1 , LCD_BACK_LIGHT_PWM_TADATA1);     //TA_DATA1
    delay_nops(100);
    write_XDATA(PWM_TACMD,0x0003);               //Initialize the timer and enable it
    write_mem(GPIO_PCON2B , read_mem(GPIO_PCON2B)|0x0c00);//p2.15 : PWM
}

/*------------------------------------------------------------------------------
Function: 
Input   :
Output  :
------------------------------------------------------------------------------*/
void LcdBackLightOn(void)
{
    LightOffCounter = 0;
    SystemInfo.bc.bBackLightStatus = 0;
    LcdBackLevelSet(LigthLevel);
}

/*------------------------------------------------------------------------------
Function: 
Input   :
Output  :
------------------------------------------------------------------------------*/
void LcdBackLightOff(void)
{
    if (DisplayMode == 0) {
        SystemInfo.bc.bBackLightStatus = 1;             // 正常模式
        LcdBackLevelSet(5);
    } else {
        SystemInfo.bc.bBackLightStatus = 2;             // 省电模式
        write_XDATA(PWM_TACMD , 0x0000);
        write_mem(GPIO_PCON2B , (read_mem(GPIO_PCON2B)|0x0c00) & 0xf7ff);
        write_mem(GPIO_PDAT2 , (read_mem(GPIO_PDAT2) & 0xdfff));
        
        //在视频播放时不能关闭SDRAM
        if ((Sys1InitFlag.bc.bAviModuleInitEnable == 1) && 
           ((Sys2InitFlag.word == 0xffff) || ((Sys2InitFlag.word & 0x0030) != 0x0030))) {
            PeriphFlag.bc.bLcdStandby = 1;
            DmaTanData2LcdCont = 0;
            Standby_lcd();
            SDRAM_PowerDown();
            write_mem(CLOCK_PWRCON,(read_mem(CLOCK_PWRCON) & 0xfff7));     //MemCtr Clock Power Off
        }
    }
}

/*------------------------------------------------------------------------------
Function: 
Input   :
Output  :
------------------------------------------------------------------------------*/
void LCD_SDRAM_Standby_Return(void)
{
    if (SystemInfo.bc.bBackLightStatus == 2) {
        SystemInfo.bc.bBackLightStatus = 0;
        //在视频和功能2时，没有关闭LCD和SDRAM，因此不能开LCD和SDRAM，也不能恢复显示
        SystemInfo.bc.bDisplayRecovery = 0;
#if 1
        if (PeriphFlag.bc.bLcdStandby == 1) {
#else
        if ((Sys1InitFlag.bc.bAviModuleInitEnable == 1) && 
           ((Sys2InitFlag.word == 0xffff) || ((Sys2InitFlag.word & 0x0030) != 0x0030))) {
#endif
            PeriphFlag.bc.bLcdStandby = 0;
            SystemInfo.bc.bDisplayRecovery = 1;
            write_mem(CLOCK_PWRCON,(read_mem(CLOCK_PWRCON) | 0x0008));
            delay_nops(10);
            SDRAM_Init();
            standby_return();
        }
    }
}
/*------------------------------------------------------------------------------
Function:
Input   :
Output  :
------------------------------------------------------------------------------*/
void Delay1Ms(unsigned int Counter)
{
    unsigned int i;
    
    for(i = 0; i < Counter; i++) {
        delay_nops(CurrCpuFreq * 1000);
    }
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
void GetTimeHMS(unsigned long TempSec, unsigned int *pHour, unsigned int *pMin, unsigned int *pSec)
{
    unsigned long temp = TempSec;
    
    *pSec = (unsigned int)(temp % 60);
    temp = temp / 60;
    *pMin = temp % 60;
    *pHour = temp / 60;
}
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
//#if(CPU_TYPE == _RK2608A_)
//void CodecVddaoOn(void)
//{
//    write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0xffdf));  //p1.5--->VDDA控制
//    Delay1Ms(300);
//    write_mem(GPIO_PDAT1,(read_mem(GPIO_PDAT1) & 0xfff7));  //p1.3--->VDDAO控制
//    Delay1Ms(300);
//}
//#endif
/******************************************************************************/
