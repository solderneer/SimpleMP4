#define IN_FWINIT
#include <creg.h>
#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "FWconfig.h"
#include "Global.h"

void FWUsbInCPUInit(void);
/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
FIRMWARE_CODE
void FunUpgradeInit(void)
{
    if (Sys1InitFlag.bc.bUsbModuleInitEnable) {
        Sys1InitFlag.bc.bUsbModuleInitEnable = 0;
    }
    
    MemorySelect = 0;
    PrewMemorySelect = MemorySelect;
    MediaSetup(0);
    
    FWUsbInCPUInit();
    FWUSBInit();
    PostFun1Message(MSG_FUNCTION1_UPGRADE_TRANS,0,0,0,0/*,0,0,0*/);
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
FIRMWARE_CODE
void UpgradeUSBTrans(void)
{
    
}

/*------------------------------------------------------------------------------
Function:
Input:
Output:
------------------------------------------------------------------------------*/
FIRMWARE_CODE
void FWUsbInCPUInit(void)
{
    unsigned int temp;
    
    bitclear_creg(%imask,15);
    
//----------  CLOCK INIT  -----------
    SysCpuSetFreq(48);
    SysTimer0Set(48,25000);
    
    write_mem(CLOCK_PWRCON,read_mem(CLOCK_PWRCON) | 0x0001);
    delay_nops(100);
    write_mem(CLOCK_I2SMCLKCON, 0x0100);    //I2SMclk = 12MHZ 
    delay_nops(100);
    
    write_creg(%ip0,0x0000);
    write_creg(%ip1,0x4800);
    write_creg(%ireq,0x0000);
    
    //bitset_creg(%imask,10);
    //bitset_creg(%imask,8);
    bitset_creg(%imask,7);
    bitset_creg(%imask,15);
}
//******************************************************************************
