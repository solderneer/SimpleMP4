#include "xvid_section_def.h"
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!    #define BITS_N_FRAME(A,B)   (unsigned long)A = (unsigned long)(A|((unsigned long)1<<B))
!    Author:
!        Jian Huan
!    Description:
!        Because the number of RGB Page in SDRAM is 48, so Frame_Type is a array
!    Date:
!        2006-7-19 10:02    
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!        
MP4_ASSEMBLE_SECTION
.global _BITS_N_FRAME
.walign 2
_BITS_N_FRAME:
            stu     r7,     r12,    -1
            movl    r6,     0xF
            
            movh    r6,     0x0
            
            and     r6,     r5
            shrl    r5,     4               !0 <= B <= 15,  r5=0 
                                            !16<= B <= 31,  r5=1
                                            !32<= B <= 47,  r5=2
                                            
            ldx     r13,    r4
            mov     r7,     1
            
            shll    r7,     r6
            
            or      r7,     r13
            
            stx     r7,     r4     
            
            ld      r7,     r12,    1
            add     r12,    1
            ret
            
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!    #define BITC_N_FRAME(A,B)   (unsigned long)A = (unsigned long)(A&(~((unsigned long)1<<B)))
!    Author:
!        Jian Huan
!    Description:
!        Because the number of RGB Page in SDRAM is 48, so Frame_Type is a array
!    Date:
!        2006-7-19 10:03    
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!        
MP4_ASSEMBLE_SECTION
.global _BITC_N_FRAME
.walign 2
_BITC_N_FRAME:
            stu     r7,     r12,    -1
            movl    r6,     0xF
            
            movh    r6,     0x0
            
            and     r6,     r5
            shrl    r5,     4               !0 <= B <= 15,  r5=0 
                                            !16<= B <= 31,  r5=1
                                            !32<= B <= 47,  r5=2
                                            
            ldx     r13,    r4
            mov     r7,     1
            
            shll    r7,     r6
            
            not     r7,     r7
            
            and     r7,     r13
            
            stx     r7,     r4     
            
            ld      r7,     r12,    1
            add     r12,    1
            ret

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!    #define BITT_N_FRAME(A,B)   (unsigned long)(A&((unsigned long)1<<B))    
!    Author:
!        Jian Huan
!    Description:
!        Because the number of RGB Page in SDRAM is 48, so Frame_Type is a array
!    Date:
!        2006-7-19 10:22  
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!        
MP4_ASSEMBLE_SECTION
.global _BITT_N_FRAME
.walign 2
_BITT_N_FRAME:
            movl    r6,     0xF
            
            movh    r6,     0x0
            
            and     r6,     r5
            shrl    r5,     4               !0 <= B <= 15,  r5=0 
                                            !16<= B <= 31,  r5=1
                                            !32<= B <= 47,  r5=2
                                            
            ldx     r13,    r4
            mov     r4,     1
            
            shll    r4,     r6
            
            and     r4,     r13
            
            ret            
            
            
