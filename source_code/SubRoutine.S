/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : SubRoutine.S
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/12/12       1.0            ORG
*
********************************************************************************
*/
/*------------------------------------------------------------------------------
Function: Delay R4 nops
Input   : r4
Output  : 
C_function: void delay_nops(unsigned int Count)
------------------------------------------------------------------------------*/
        .section    ".text", "ax"
        .global     _delay_nops
        .walign 2
        
_delay_nops:
        !--------push------------
        stu     r5,r12,-1
        mov     r5,%loop0
        stu     r5,r12,-1
        !
        mov    %loop0,r4
        nop
delay_100nop_loop:
        nop
        nop
        nop
        nop
        agn0    delay_100nop_loop
    !--------pop-------------
        ld      r5,r12,1
        mov     %loop0,r5
        ld      r5,r12,2
        add     r12,2
    !------------------------
        ret
//******************************************************************************
