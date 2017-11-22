//******************************************************************************
//SysInit.S          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#include "Memap.h"
#include "Creg.h"
//******************************************************************************
  
      
        
      
 
//-----------------------------------------------------------------------------
//Function: Fill MP3 Decode Output PCM Data
//Input:    
//Output:   
//C_function: void PCM16_PCMBufferDataChangeStereo(unsigned int *pDecodeBuffer,unsigned int GrainLen,unsigned int *pPCMBuffer);
//-----------------------------------------------------------------------------
    	.section    ".avi_a_text","ax"
        .global 	_PCM16_PCMBufferDataChangeStereo
        .walign 2
        
_PCM16_PCMBufferDataChangeStereo:
    !--------push------------
        stdu    r0,r12,-2
        stdu    r2,r12,-2
        stdu    r14,r12,-2
        mov     r1,%loop0
        stu     r1,r12,-1
        mov     r1,%rpc
        stu     r1,r12,-1
        !
        mov     r1,r12
        mov     r0,8
        add     r1,r0
        stu     r1,r12,-1
    !------------------------
        mov     r15,r12
        mov     r0,576
        sub     r12,r0
        
        mov     r14,1
        add     r14,r12
        
        mov     r0,r6
        add     r0,-1
        mov     %loop0,r0
AVI_first_576_pull_stack:
        ldu    r2,r4,1
        stu    r2,r14,1
        agn0    AVI_first_576_pull_stack 
    
        mov     r6,r15
        mov     r15,r5 
        
        mov     r14,1
        add     r14,r12
        mov     r1,576
        add     r5,r1
        mov     %loop0,r0
              
AVI_first_576_push_stack:
        ldu     r0,r14,1
        stu     r0,r15,1
        ldu     r2,r5,1       
        stu     r2,r15,1
        agn0    AVI_first_576_push_stack
    
    
        mov     r12,r6
    !--------pop-------------
        mov     r1,r12
        add     r1,2
        !
        ldu     r0,r1,1
        mov     %rpc,r0
        ldu     r0,r1,1
        mov     %loop0,r0
        lddu    r14,r1,2
        lddu    r2,r1,2
        ldu     r0,r1,1
        ld      r1,r1
        ld      r12,r12,1
    !------------------------
        ret
//-----------------------------------------------------------------------------
//Function: Fill MP3 Decode Output PCM Data
//Input:    
//Output:   
//C_function: void PCMBufferDataChangeMono(unsigned int *pDecodeBuffer,unsigned int GrainLen,unsigned int *pPCMBuffer);
//-----------------------------------------------------------------------------
        .section    ".avi_a_text","ax"
        .global 	_PCM16_PCMBufferDataChangeMono
        .walign 2
_PCM16_PCMBufferDataChangeMono:
    !--------push------------
        stdu    r0,r12,-2
        stdu    r14,r12,-2
        mov     r1,%loop0
        stu     r1,r12,-1
        mov     r1,%rpc
        stu     r1,r12,-1
        !
        mov     r1,r12
        mov     r0,6
        add     r1,r0
        stu     r1,r12,-1
    !------------------------
        mov     r15,r12
       
        sub     r12,r6
        add     r6,-1
        mov     %loop0,r6
        
        mov     r14,r4
        
        mov     r5,r12
        add     r5,1
        mov     r1,r5
        
_pcm_to_stack_left576:  //stack
        ldu     r0,r4,1    
        stu     r0,r5,1
        agn0    _pcm_to_stack_left576
        
        mov     r4,r1
        mov     r5,r14        
        mov     %loop0,r6
        
_PCMBufferDataChangeMonoLoop0:
        ldu      r0,r4,1 
        mov      r1,r0   
        stdu     r0,r5,2
        agn0    _PCMBufferDataChangeMonoLoop0
        

        
        mov     r12,r15
 
    !--------pop-------------
        mov     r1,r12
        add     r1,2
        !
        ldu     r0,r1,1
        mov     %rpc,r0
        ldu     r0,r1,1
        mov     %loop0,r0
        lddu    r14,r1,2
        ldu     r0,r1,1
        ld      r1,r1
        ld      r12,r12,1
    !------------------------
        ret
        
//******************************************************************************
