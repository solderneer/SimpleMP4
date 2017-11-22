//******************************************************************************
//SysInit.S          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#include "Memap.h"
#include "Creg.h"
//******************************************************************************
        .segment "data"
        .extern     _PCMOutBufferLengt
        .extern     _MP3DecodeInputBufWordsRemaining
        .extern     _MP3DecodeInputBufWordsUsed
        .extern     _ApplicationInputBufferStart
//-----------------------------------------------------------------------------
//Function: Fill MP3 Decode Output PCM Data
//Input:    
//Output:   
//C_function: void PCMBufferDataChangeStereo(unsigned int *pDecodeBuffer,unsigned int GrainLen,unsigned int *pPCMBuffer);
//-----------------------------------------------------------------------------
    	.section 	".mp3_text", "ax"
    	//.section 	".text", "ax"
        .global 	_PCMBufferDataChangeStereo
        !.extern     _PCMOutBufferLength
        .walign 2
_PCMBufferDataChangeStereo:
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
        mov     r0,1152
        sub     r12,r0
        
        mov     r14,1
        add     r14,r12
        
        mov     r0,r6
        add     r0,-1
        mov     %loop0,r0
mp3_first_576_pull_stack:
        lddu    r2,r4,2
        stdu    r2,r14,2
        agn0   mp3_first_576_pull_stack 
    
        mov     r6,r15
        mov     r15,r5 
        
        mov     r14,1
        add     r14,r12
        mov     r1,1152
        add     r5,r1
        mov     %loop0,r0
       
        
mp3_first_576_push_stack:
        lddu    r0,r14,2
        stdu    r0,r15,2
        lddu    r2,r5,2      
        stdu    r2,r15,2
        agn0    mp3_first_576_push_stack
    
    
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
    	.section 	".mp3_text", "ax"
        .global 	_PCMBufferDataChangeMono
        !.extern     _PCMOutBufferLength
        .walign 2
        
_PCMBufferDataChangeMono:
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
       
        add     r6,-1
       // mov     r2,r6
        mov     %loop0,r6
        mov     r14,r4
        mov     r15,r5
        
_PCMBufferDataChangeMonoLoop0:
        lddu    r0,r4,2    
        stdu    r0,r5,2
        agn0    _PCMBufferDataChangeMonoLoop0
        
        mov     r5,r15
        mov     r4,r14
        mov     %loop0,r6
_COPY_PCMBufferDataChangeMonoLoop0:
        lddu    r0,r5,2    
        stdu    r0,r4,2
        stdu    r0,r4,2
        agn0    _COPY_PCMBufferDataChangeMonoLoop0 
 
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
        
//_PCMBufferDataChangeStereo:
//    !--------push------------
//        stdu    r0,r12,-2
//        stdu    r2,r12,-2
//        stdu    r14,r12,-2
//        mov     r1,%loop0
//        stu     r1,r12,-1
//        mov     r1,%rpc
//        stu     r1,r12,-1
//        !
//        mov     r1,r12
//        mov     r0,8
//        add     r1,r0
//        stu     r1,r12,-1
//    !------------------------
//        mov     r14,r4
//        mov     r15,1152
//        add     r15,r14
//        !
//        mov     r4,r5
//        lda     r5,_PCMOutBufferLength
//        ld      r5,r5
//        add     r5,r4
//        !
//        add     r6,-1
//        mov     r2,r6
//        mov     %loop0,r6
//        !
//        mov     r6,16
//_PCMBufferDataChangeStereoLoop0:
//        lddu    r0,r14,2
//        shrl.e  r0,r6
//        stu     r0,r4,1
//        agn0    _PCMBufferDataChangeStereoLoop0
//        !
//        mov     %loop0,r2
//        mov     r6,16
//_PCMBufferDataChangeStereoLoop1:
//        lddu    r0,r15,2
//        shrl.e  r0,r6
//        stu     r0,r5,1
//        agn0    _PCMBufferDataChangeStereoLoop1
//        !
//    !--------pop-------------
//        mov     r1,r12
//        add     r1,2
//        !
//        ldu     r0,r1,1
//        mov     %rpc,r0
//        ldu     r0,r1,1
//        mov     %loop0,r0
//        lddu    r14,r1,2
//        lddu    r2,r1,2
//        ldu     r0,r1,1
//        ld      r1,r1
//        ld      r12,r12,1
//    !------------------------
//        ret
////-----------------------------------------------------------------------------
////Function: Fill MP3 Decode Output PCM Data
////Input:    
////Output:   
////C_function: void PCMBufferDataChangeMono(unsigned int *pDecodeBuffer,unsigned int GrainLen,unsigned int *pPCMBuffer);
////-----------------------------------------------------------------------------
//    	.section 	".mp3_text", "ax"
//        .global 	_PCMBufferDataChangeMono
//        !.extern     _PCMOutBufferLength
//        .walign 2
//        
//_PCMBufferDataChangeMono:
//    !--------push------------
//        stdu    r0,r12,-2
//        stdu    r14,r12,-2
//        mov     r1,%loop0
//        stu     r1,r12,-1
//        mov     r1,%rpc
//        stu     r1,r12,-1
//        !
//        mov     r1,r12
//        mov     r0,6
//        add     r1,r0
//        stu     r1,r12,-1
//    !------------------------
//        mov     r14,r4
//        !
//        mov     r4,r5
//        mov     r15,r4
//        lda     r5,_PCMOutBufferLength
//        ld      r5,r5
//        add     r5,r4
//        !
//        add     r6,-1
//        mov     r2,r6
//        mov     %loop0,r6
//        !
//        mov     r6,16
//_PCMBufferDataChangeMonoLoop0:
//        lddu    r0,r14,2
//        shrl.e  r0,r6
//        stu     r0,r4,1
//        agn0    _PCMBufferDataChangeMonoLoop0
//        !
//        mov     %loop0,r2
//_PCMBufferDataChangeMonoLoop1:
//        ldu     r0,r15,1
//        stu     r0,r5,1
//        agn0    _PCMBufferDataChangeMonoLoop1
//        !
//    !--------pop-------------
//        mov     r1,r12
//        add     r1,2
//        !
//        ldu     r0,r1,1
//        mov     %rpc,r0
//        ldu     r0,r1,1
//        mov     %loop0,r0
//        lddu    r14,r1,2
//        ldu     r0,r1,1
//        ld      r1,r1
//        ld      r12,r12,1
//    !------------------------
//        ret
//-----------------------------------------------------------------------------
//Function: MP3 Input Buffer Remaining Data Move To Start And Clear Other Data
//Input:    
//Output:   
//C_function:   void MP3InputBufferToStart(void)
//-----------------------------------------------------------------------------
    	.section 	".mp3_text", "ax"
        .global 	_MP3InputBufferToStart
        !.extern     _MP3DecodeInputBufWordsRemaining
        !.extern     _MP3DecodeInputBufWordsUsed
        !.extern     _ApplicationInputBufferStart
        .walign 2
        
_MP3InputBufferToStart:
    !--------push------------
        stu     r14,r12,-1
        stdu    r0,r12,-2
        mov     r14,%loop0
        stu     r14,r12,-1
        mov     r14,%rpc
        stu     r14,r12,-1
        !
        mov     r14,r12
        add     r14,5
        stu     r14,r12,-1
    !------------------------
        lda     r1,_MP3DecodeInputBufWordsRemaining
        ld      r1,r1
        cmp     r1,0
        bz      MP3InputBufferToStartRet
        add     r1,-1
        mov     %loop0,r1
        !
        lda     r0,_ApplicationInputBufferStart
        ld      r0,r0
        !
        lda     r14,_MP3DecodeInputBufWordsUsed
        ld      r14,r14
        add     r14,r0
MP3InputBufferToStartLoop1:
        ldu     r1,r14,1
        stu     r1,r0,1
        agn0    MP3InputBufferToStartLoop1
        !
        lda     r14,_MP3DecodeInputBufWordsRemaining
        ld      r14,r14
        mov     r1,1279                     !APPLICATION_INPUT_BUF_LENGTH-1
        cmp     r1,r14
        blt     MP3InputBufferToStartRet
        sub     r1,r14
        mov     %loop0,r1
        mov     r1,0
MP3InputBufferToStartLoop2:
        stu     r1,r0,1
        agn0    MP3InputBufferToStartLoop2
        !
MP3InputBufferToStartRet:
    !--------pop-------------
        mov     r14,r12
        add     r14,2
        !
        ldu     r0,r14,1
        mov     %rpc,r0
        ldu     r0,r14,1
        mov     %loop0,r0
        lddu    r0,r14,2
        ld      r14,r14
        ld      r12,r12,1
    !------------------------
        ret
//******************************************************************************
