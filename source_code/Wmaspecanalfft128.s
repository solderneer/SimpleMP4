/***********************************************************************
 ×  fft128.s
 ×  Copyright (C), 2005-2006, ROCK-CHIPS Semi. Co., Ltd.
 *  Function Name:fft128()
 *  Author:       huangxudong
 *  Date:         20051005
 *  Version:      1.1
 ×  Description:  该函数描述了128点的FFT变换
 *  History:
    <Author>      <Date>          <Version>
    huangxudong   2005-10-11       1.2
 ***********************************************************************/

#include "wmafft128.h"

.section ".wma_text","ax"
//.segment "text"

.global _wma_fft128

.walign 4

_wma_fft128:

! Begin Prolog
 	                    
 	                    stu       r13,r12,-1
 	                    stdu      r0, r12, -2          
 	                    stdu      r2, r12, -2                   
 	                    stdu      r6, r12, -2                   
 	                    stdu      r8, r12, -2                   
 	                    stdu      r10, r12, -2                  
 	                    stdu      r14, r12, -2                  
 	                    mov       r0, %rpc                      
 	                    stu       r0, r12, -1
 	                    mov       r0,%cb0_beg
 	                    stu       r0,r12,-1
 	                    mov       r0,%cb1_beg
 	                    stu       r0,r12,-1
 	                    mov       r0,%cb0_end
 	                    stu       r0,r12,-1 
 	                    mov       r0,%loop0
 	                    stu       r0,r12,-1 
 	                    mov       r0,%loop1
 	                    stu       r0,r12,-1 
 	                    mov       r0,%loop2
 	                    stu       r0,r12,-1
 	                    mov       r0,%fmode
 	                    stu       r0,r12,-1
 	                    mov       r0,%imask
 	                    stu       r0,r12,-1
 	                    mov       r0,%amode
 	                    stu       r0,r12,-1 
 	                     	                  
! End Prolog

	bits	%fmode, 2					! Enable Q15
	bits	%fmode, 0					! Enable Rounding
	bits	%fmode, 4					! Enable Saturation

	mov		r8, -257
	add		r12, r8
	mov		r8, r12
	add		r8, 1
	bitt	r8, 0
	bz		L_cont1
	add		r8, 1

L_cont1:
    stu		r4, r12, -1					! data_1
	stu		r8, r12, -1					! data_addr  

! STAGE1

	mov		r0, 63
	mov		%loop0, r0
    mov		r14, 128
	mov		r13, r4          !!r13=&tones(0)
	add		r14, r4          
	mov		r9, 128
	add		r9, r8           
L_STAGE1:
    lddu	r4, r13, 2  	 	
	lddu	r6, r14, 2       
	padd.a	r4, r6				
	stu		r1, r8, 1		     	
	stu		r0, r8, 1			   
	psub.b	r4, r6				
	stu		r3, r9, 1        
	stu		r2, r9, 1			   
    agn0	L_STAGE1
!-STAGES 2-STAGES 6---------begin------------------------------------------------------------
    mov		r11, 2
	stu		r11, r12, -1
	mov		r11, 128
	mov		r0, 4
	mov		%loop0, r0           !!%loop0=4

L_STAGELoop:
	lda		r15, TWF_ADDR          
    ld		r14, r12, 2          
    shrl	r11, 1               
    ld		r13, r12, 1          
	add		r13, -2
	mov		%loop1, r13            
	add		r13, 2                 
	shll	r13, 1                 
	st		r13, r12, 1
    mov		r13, r14					   
	mov		r6, r14                
	add		r14, r11               
	shrl	r11, 1                 
	mov		r7, r14                
	add		r11, -2                !!r11=30
    lddu	r4, r13, 2					 ! ai,ar
	lddu	r8, r14, 2					   ! bi,br
	padd.a	r4, r8						   
	mov		%loop2, r11            
	psub.b	r4, r8						    

L_GROUP1:
    lddu	r4, r13, 2					! ai,ar
	lddu	r8, r14, 2					! bi,br
	stdu	r0, r6, 2
	padd.a	r4, r8						
	stdu	r2, r7, 2
	psub.b	r4, r8						
    agn2	L_GROUP1
	stdu	r0, r6, 2
    mov		r11, r14
	stdu	r2, r7, 2
    sub		r11, r13					! r11 = r14 - r13

L_GROUPLoop:
    lddu	r8, r15, 2					
    mov		r13, r14					! r13 = r14
	mov		r6, r14
    lddu	r4, r13, 2					! r4,r5 = ai,ar
	add		r14, r11
	shrl	r11, 1
	mov		r7, r14
	add		r11, -2
	mov		%loop2, r11
    lddu	r10, r14, 2					!r2,r3 = bi,br
	cmuli.a	r8, r10						  
    cmulr.b	r8, r10					  
	mov		r2, r1						    !r1 = ti; r3 = tr
	psub.a	r4, r2						  
    add		r2, r4
	add		r3, r5
L_BFLoop:
    lddu	r4, r13, 2					
    lddu	r10, r14, 2					
	stdu	r0, r7, 2
	cmuli.a	r8, r10						

    nop
	stdu	r2, r6, 2
    cmulr.b	r8, r10						
	mov		r2, r1						! r1 = ti; r3 = tr
	
	psub.a	r4, r2						
    add		r2, r4
	add		r3, r5
    agn2	L_BFLoop
    
    mov		r11, r14
	stdu	r2, r6, 2
    sub		r11, r13					! r11 = r14 - r13
	stdu	r0, r7, 2
    agn1	L_GROUPLoop
    agn0	L_STAGELoop
!-STAGES 2-STAGES 6---------end------------------------------------------------------------
    
!-STAGE7-----------------------------------------------------------------------------------
	mov		%loop1, 62
    ld		r13, r12, 2
    bitc	%fmode, 4					
    mov		r15, 0x1ff					! 9 bits
	mov		r6, -256					! buffer length 
    add		r6, r12						! r12 - 256
    and		r15, r6						
	sub		r6, r15						
    mov		r4, r6
	add		r4, -1						! next free stack location
	mov		r5, r12						! actual (old) stack  
	mov		r12, r4						! r12 points to first free location
	stu		r5, r12, -1					! store old stack
	stu		r6, r12, -1					
    bits	%fmode, 4					
    lda		r15, TWF_ADDR
    lddu	r4, r13, 2					! r4,r5 = ai,ar
    mov		r10, 38						

	mov		r2, %imask
	stu		r2, r12, -1
	bitc	%imask, 15					! disable interupts

    mov		%amode, r10					
    lddu	r2, r13, 2					
	psub.a	r4, r2
    padd.b	r4, r2

L_GROUPLoop7:
    lddu	r4, r13, 2					
	stdu	r2, r6, 2
    lddu	r8, r15, 2					
    lddu	r2, r13, 2					
	stdu	r0, r6, 2
	cmulr.a	r8, r2						
    cmuli.b	r8, r2					
	mov		r0, r3						
	padd.b	r4, r0						
	psub.a	r4, r0						
    agn1	L_GROUPLoop7
	stdu	r2, r6, 2
	stdu	r0, r6, 2
    mov		r10, 0
    mov		%amode, r10					

	ld		r2, r12, 1
	mov		%imask, r2

!-DATA Convertion - not part of FFT (swap RE <-> IM of output data)------------------------

    mov		r13, 127
	mov		%loop0, r13
	ld		r13, r12, 2       
    ld		r12, r12, 3					! restore stack
	ld		r14, r12, 3

L_RetoIm3:
    lddu	r8, r13, 2
	stu		r9, r14, 1
	stu		r8, r14, 1
	agn0	L_RetoIm3

	bitc	%fmode, 2					! Disable Q15
	bitc	%fmode, 0					! Disable Rounding
	bitc	%fmode, 4					! Disable Saturation

! End Epilog    
 	                    mov       r13,257
 	                    add       r13, r12                       
                      add       r13,4    
                      ldu       r0, r13, 1 
 	                    mov       %amode,r0
 	                    ldu       r0, r13, 1 
 	                    mov       %imask,r0
 	                    ldu       r0, r13, 1 
 	                    mov       %fmode,r0                                         
 	                    ldu       r0, r13, 1 
 	                    mov       %loop2,r0
 	                    ldu       r0,r13,1
 	                    mov       %loop1,r0
 	                    ldu       r0,r13,1
 	                    mov       %loop0,r0
 	                    ldu       r0,r13,1 	                    
 	                    mov       %cb0_end,r0
 	                    ldu       r0,r13,1
 	                    mov       %cb1_beg,r0
 	                    ldu       r0,r13,1
 	                    mov       %cb0_beg,r0
 	                    ldu       r0,r13,1                    
 	                    mov       %rpc, r0                      
 	                    lddu      r14, r13, 2                   
 	                    lddu      r10, r13, 2                    
 	                    lddu      r8, r13, 2                     
 	                    lddu      r6, r13, 2                     
 	                    lddu      r2, r13, 2                     
 	                    lddu      r0, r13, 2 
 	                    mov       r12,r13
 	                    ld        r13,r13                    
 	                    mov       %pc,%rpc 
! End Epilog
