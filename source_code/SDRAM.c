#include <creg.h>
#include "Macro.h"
#include "Memap.h"

void SDRAM_Init(void){
    
    unsigned int imask_temp;
    
    read_creg(%imask,imask_temp);
    bitclear_creg(%imask,15);
    
    write_mem(SYSCTL, 0x007F);
    //write_mem(SYSCTL, 0x007F);
    //write_mem(SYSCTL, 0x03FF);
    
    write_mem(0xf807, 0x0003);      //mempcr
    bitset_creg(%smode, 0);
    
    write_mem(MEMCTL_SCONR,   0x0f68);//0x0f68);
    write_mem(MEMCTL_SCONR+1, 0x001C);
    
    write_mem(MEMCTL_STMG0R,  0x9451);      //0x9452
    write_mem(MEMCTL_STMG0R+1,0x019D);
    
    write_mem(MEMCTL_STMG1R,  0x0008);
    write_mem(MEMCTL_STMG1R+1,0x0007);
    
    write_mem(MEMCTL_SREFR,   0x03E8);
    //write_mem(MEMCTL_SREFR,   0x00E8);
    write_mem(MEMCTL_SREFR+1, 0x0000);
    
    //write_mem(MEMCTL_SCTLR,   0x2049);
    write_mem(MEMCTL_SCTLR,   0x2009);
    write_mem(MEMCTL_SCTLR+1, 0x0000);
    
    bitclear_creg(%smode, 0);
    write_mem(0xf807, 0x0000);      //mempcr
    
    write_creg(%imask,imask_temp);
}
void SDRAM_PowerDown(void)
{
    unsigned int imask_temp;
    
    read_creg(%imask,imask_temp);
    bitclear_creg(%imask,15);
    
    write_mem(SYSCTL, 0x007F);
    write_mem(0xf807, 0x0003);      //mempcr
    bitset_creg(%smode, 0);
    
    write_mem(MEMCTL_SCTLR,   0x200e);
    write_mem(MEMCTL_SCTLR+1, 0x0000);
    
    bitclear_creg(%smode, 0);
    write_mem(0xf807, 0x0000);      //mempcr
    
    write_creg(%imask,imask_temp);
}
