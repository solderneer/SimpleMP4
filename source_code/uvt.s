//*****************************************************************************
// uvt.s    2005.08.09        copyright  rock-chip  authour zyz
//*****************************************************************************
    .section    ".vectors", "ax"
    .global     _PIM_uvt_table
    .extern     _main
    .extern     _Timer0Isr
    .extern     _DmaIsr
    .extern     _UserIsr
    .extern     _UsbTranIsr
    .walign 8
    
_PIM_uvt_table:
_uvt_reset:         //reset
    mov     r12,0xbfff
    lda     r13,_main
    br      r13       

    .walign 8
_uvt_nmi:           //nmi
    reti            

    .walign 8
_uvt_dei:           //JTAG
    //br      Deu_Isr
    reti

    .walign 8
_uvt_user1:         //user1
    reti            

    .walign 8
_uvt_user2:         //user2
    reti            

    .walign 8
_uvt_user3:         //user3
    reti

    .walign 8
_uvt_usb:           //usb
    stu     r13,r12,-1
    lda     r13,_UsbTranIsr
    br      uvt_Isr

    .walign 8
_uvt_user4:         //user4
//    stu     r13,r12,-1
//    lda     r13,_UserIsr
//    br      uvt_Isr
    reti

    .walign 8
_uvt_wdt:           //wdt
    reti            

    .walign 8
_uvt_dma:           //dma interrupt
    stu     r13,r12,-1
    lda     r13,_DmaIsr
    br      uvt_Isr

    .walign 8
_uvt_t1:            //timer1 interrupt
    reti            

    .walign 8
_uvt_t0:            //timer0 interrupt
    stu     r13,r12,-1
    lda     r13,_Timer0Isr
    br      uvt_Isr 

    .walign 8
_uvt_pwm:           //PWM
    reti            

    .walign 8
_uvt_ex3:           //external int3
    reti            

    .walign 8
_uvt_ex2:           //external int2
    reti

    .walign 8
_uvt_ex1:           //external int1
    reti

    .walign 8
_uvt_ex0:           //external int0
    stu     r13,r12,-1
    lda     r13,_UserIsr
    br      uvt_Isr
    reti
//*****************************************************************************
    .section    ".fasttext", "ax"
    .walign 2
uvt_Isr:
    //stu     r13,r12,-1
    stdu    r0,r12,-2
    stdu    r2,r12,-2
    stdu    r4,r12,-2
    stdu    r6,r12,-2
    stdu    r8,r12,-2
    stdu    r10,r12,-2
    stdu    r14,r12,-2
    
    mov     r4,%hwflag
    stu     r4,r12,-1
    mov     r4,%rpc
    stu     r4,r12,-1
    mov     r4,%amode
    stu     r4,r12,-1
    mov     r5,0
    mov     %amode,r5
    mov     r4,%fmode
    stu     r4,r12,-1
    mov     r5,0
    mov     %fmode,r5
    mov     r4,%smode
    stu     r4,r12,-1
    mov     r5,0x0080
    mov     %smode,r5
    
    mov     r4,%cb0_beg
    stu     r4,r12,-1
    mov     r4,%cb0_end
    stu     r4,r12,-1
    mov     r4,%cb1_beg
    stu     r4,r12,-1
    mov     r4,%cb1_end
    stu     r4,r12,-1
    
    mov     r4,%loop0
    stu     r4,r12,-1
    mov     r4,%loop1
    stu     r4,r12,-1
    mov     r4,%loop2
    stu     r4,r12,-1
    mov     r4,%loop3
    stu     r4,r12,-1
    
    mov     r4,r12
    mov     r5,28
    add     r4,r5
    stu     r4,r12,-1
    
    //lda    r13,_Timer0Isr
    call    r13
    
    mov     r13,r12
    add     r13,2
    
    ldu     r4,r13,1
    mov     %loop3,r4
    ldu     r4,r13,1
    mov     %loop2,r4
    ldu     r4,r13,1
    mov     %loop1,r4
    ldu     r4,r13,1
    mov     %loop0,r4
    
    ldu     r4,r13,1
    mov     %cb1_end,r4
    ldu     r4,r13,1
    mov     %cb1_beg,r4
    ldu     r4,r13,1
    mov     %cb0_end,r4
    ldu     r4,r13,1
    mov     %cb0_beg,r4
    
    ldu     r4,r13,1
    mov     %smode,r4
    ldu     r4,r13,1
    mov     %fmode,r4
    ldu     r4,r13,1
    mov     %amode,r4
    ldu     r4,r13,1
    mov     %rpc,r4
    ldu     r4,r13,1
    mov     %hwflag,r4
    
    lddu    r14,r13,2
    lddu    r10,r13,2
    lddu    r8,r13,2
    lddu    r6,r13,2
    lddu    r4,r13,2
    lddu    r2,r13,2
    lddu    r0,r13,2
    
    ld      r13,r13
    ld      r12,r12,1
    reti
//-----------------------------------------------------------------------------
//    .section    ".fasttext", "ax"
//    !.global    Dma_Isr
//    .extern     _DmaIsr
//    .walign 2
//Dma_Isr:
//    stu     r13,r12,-1
//    stdu    r0,r12,-2
//    stdu    r2,r12,-2
//    stdu    r4,r12,-2
//    stdu    r6,r12,-2
//    stdu    r8,r12,-2
//    stdu    r10,r12,-2
//    stdu    r14,r12,-2
//    
//    mov     r4,%hwflag
//    stu     r4,r12,-1
//    mov     r4,%rpc
//    stu     r4,r12,-1
//    mov     r4,%amode
//    stu     r4,r12,-1
//    mov     r5,0
//    mov     %amode,r5
//    mov     r4,%fmode
//    stu     r4,r12,-1
//    mov     r5,0
//    mov     %fmode,r5
//    mov     r4,%smode
//    stu     r4,r12,-1
//    mov     r5,0x0080
//    mov     %smode,r5
//    
//    mov     r4,%cb0_beg
//    stu     r4,r12,-1
//    mov     r4,%cb0_end
//    stu     r4,r12,-1
//    mov     r4,%cb1_beg
//    stu     r4,r12,-1
//    mov     r4,%cb1_end
//    stu     r4,r12,-1
//    
//    mov     r4,%loop0
//    stu     r4,r12,-1
//    mov     r4,%loop1
//    stu     r4,r12,-1
//    mov     r4,%loop2
//    stu     r4,r12,-1
//    mov     r4,%loop3
//    stu     r4,r12,-1
//    
//    mov     r4,r12
//    mov     r5,28
//    add     r4,r5
//    stu     r4,r12,-1
//    
//	//bitc	   %fmode, 4
//    lda    r13,_DmaIsr
//    call    r13
//    
//    mov     r13,r12
//    add     r13,2
//    
//    ldu     r4,r13,1
//    mov     %loop3,r4
//    ldu     r4,r13,1
//    mov     %loop2,r4
//    ldu     r4,r13,1
//    mov     %loop1,r4
//    ldu     r4,r13,1
//    mov     %loop0,r4
//    
//    ldu     r4,r13,1
//    mov     %cb1_end,r4
//    ldu     r4,r13,1
//    mov     %cb1_beg,r4
//    ldu     r4,r13,1
//    mov     %cb0_end,r4
//    ldu     r4,r13,1
//    mov     %cb0_beg,r4
//    
//    ldu     r4,r13,1
//    mov     %smode,r4
//    ldu     r4,r13,1
//    mov     %fmode,r4
//    ldu     r4,r13,1
//    mov     %amode,r4
//    ldu     r4,r13,1
//    mov     %rpc,r4
//    ldu     r4,r13,1
//    mov     %hwflag,r4
//    
//    lddu    r14,r13,2
//    lddu    r10,r13,2
//    lddu    r8,r13,2
//    lddu    r6,r13,2
//    lddu    r4,r13,2
//    lddu    r2,r13,2
//    lddu    r0,r13,2
//    
//    ld      r13,r13
//    ld      r12,r12,1
//    reti
////-----------------------------------------------------------------------------
////-----------------------------------------------------------------------------
//    .section    ".fasttext", "ax"
//    !.global    User_Isr
//    .extern     _UserIsr
//    .walign 2
//User_Isr:
//    stu     r13,r12,-1
//    stdu    r0,r12,-2
//    stdu    r2,r12,-2
//    stdu    r4,r12,-2
//    stdu    r6,r12,-2
//    stdu    r8,r12,-2
//    stdu    r10,r12,-2
//    stdu    r14,r12,-2
//    
//    mov     r4,%hwflag
//    stu     r4,r12,-1
//    mov     r4,%rpc
//    stu     r4,r12,-1
//    mov     r4,%amode
//    stu     r4,r12,-1
//    mov     r5,0
//    mov     %amode,r5
//    mov     r4,%fmode
//    stu     r4,r12,-1
//    mov     r5,0
//    mov     %fmode,r5
//    mov     r4,%smode
//    stu     r4,r12,-1
//    mov     r5,0x0080
//    mov     %smode,r5
//    
//    mov     r4,%cb0_beg
//    stu     r4,r12,-1
//    mov     r4,%cb0_end
//    stu     r4,r12,-1
//    mov     r4,%cb1_beg
//    stu     r4,r12,-1
//    mov     r4,%cb1_end
//    stu     r4,r12,-1
//    
//    mov     r4,%loop0
//    stu     r4,r12,-1
//    mov     r4,%loop1
//    stu     r4,r12,-1
//    mov     r4,%loop2
//    stu     r4,r12,-1
//    mov     r4,%loop3
//    stu     r4,r12,-1
//    
//    mov     r4,r12
//    mov     r5,28
//    add     r4,r5
//    stu     r4,r12,-1
//    
//    lda    r13,_UserIsr
//    call    r13
//    
//    mov     r13,r12
//    add     r13,2
//    
//    ldu     r4,r13,1
//    mov     %loop3,r4
//    ldu     r4,r13,1
//    mov     %loop2,r4
//    ldu     r4,r13,1
//    mov     %loop1,r4
//    ldu     r4,r13,1
//    mov     %loop0,r4
//    
//    ldu     r4,r13,1
//    mov     %cb1_end,r4
//    ldu     r4,r13,1
//    mov     %cb1_beg,r4
//    ldu     r4,r13,1
//    mov     %cb0_end,r4
//    ldu     r4,r13,1
//    mov     %cb0_beg,r4
//    
//    ldu     r4,r13,1
//    mov     %smode,r4
//    ldu     r4,r13,1
//    mov     %fmode,r4
//    ldu     r4,r13,1
//    mov     %amode,r4
//    ldu     r4,r13,1
//    mov     %rpc,r4
//    ldu     r4,r13,1
//    mov     %hwflag,r4
//    
//    lddu    r14,r13,2
//    lddu    r10,r13,2
//    lddu    r8,r13,2
//    lddu    r6,r13,2
//    lddu    r4,r13,2
//    lddu    r2,r13,2
//    lddu    r0,r13,2
//    
//    ld      r13,r13
//    ld      r12,r12,1
//    reti
////-----------------------------------------------------------------------------    .section    ".fasttext", "ax"
//    !.global    Usb_Isr
//    .extern     _UsbTranIsr
//    .walign 2
//    
//Usb_Isr:
//    stu     r13,r12,-1
//    stdu    r0,r12,-2
//    stdu    r2,r12,-2
//    stdu    r4,r12,-2
//    stdu    r6,r12,-2
//    stdu    r8,r12,-2
//    stdu    r10,r12,-2
//    stdu    r14,r12,-2
//    
//    mov     r4,%hwflag
//    stu     r4,r12,-1
//    mov     r4,%rpc
//    stu     r4,r12,-1
//    mov     r4,%amode
//    stu     r4,r12,-1
//    mov     r5,0
//    mov     %amode,r5
//    mov     r4,%fmode
//    stu     r4,r12,-1
//    mov     r5,0
//    mov     %fmode,r5
//    mov     r4,%smode
//    stu     r4,r12,-1
//    mov     r5,0x0080
//    mov     %smode,r5
//    
//    mov     r4,%cb0_beg
//    stu     r4,r12,-1
//    mov     r4,%cb0_end
//    stu     r4,r12,-1
//    mov     r4,%cb1_beg
//    stu     r4,r12,-1
//    mov     r4,%cb1_end
//    stu     r4,r12,-1
//    
//    mov     r4,%loop0
//    stu     r4,r12,-1
//    mov     r4,%loop1
//    stu     r4,r12,-1
//    mov     r4,%loop2
//    stu     r4,r12,-1
//    mov     r4,%loop3
//    stu     r4,r12,-1
//    
//    mov     r4,r12
//    mov     r5,28
//    add     r4,r5
//    stu     r4,r12,-1
//    
//    lda     r13 , _UsbTranIsr
//    call    r13
//    
//    mov     r13,r12
//    add     r13,2
//    
//    ldu     r4,r13,1
//    mov     %loop3,r4
//    ldu     r4,r13,1
//    mov     %loop2,r4
//    ldu     r4,r13,1
//    mov     %loop1,r4
//    ldu     r4,r13,1
//    mov     %loop0,r4
//    
//    ldu     r4,r13,1
//    mov     %cb1_end,r4
//    ldu     r4,r13,1
//    mov     %cb1_beg,r4
//    ldu     r4,r13,1
//    mov     %cb0_end,r4
//    ldu     r4,r13,1
//    mov     %cb0_beg,r4
//    
//    ldu     r4,r13,1
//    mov     %smode,r4
//    ldu     r4,r13,1
//    mov     %fmode,r4
//    ldu     r4,r13,1
//    mov     %amode,r4
//    ldu     r4,r13,1
//    mov     %rpc,r4
//    ldu     r4,r13,1
//    mov     %hwflag,r4
//    
//    lddu    r14,r13,2
//    lddu    r10,r13,2
//    lddu    r8,r13,2
//    lddu    r6,r13,2
//    lddu    r4,r13,2
//    lddu    r2,r13,2
//    lddu    r0,r13,2
//    
//    ld      r13,r13
//    ld      r12,r12,1
//    reti
//-----------------------------------------------------------------------------
//    .section    ".fasttext", "ax"
//    !.global    Deu_Isr
//    !.extern        
//    .walign 2
//
//Deu_Isr:  
//    mov     %ded, r0
//    mov     r0, %hwflag
//    mov     %dei, r0
//    bitc    %hwflag, 0
//    bits    %hwflag, 1
//loop0:  
//    bitt    %hwflag, 0
//    bz      loop0
//    
//    bitc    %hwflag, 1
//clearloop0:  
//    bitc    %hwflag, 0
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    bitt    %hwflag, 0
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    bitt    %hwflag, 0
//    bnz     clearloop0
//    
//    mov     r0, %rpc
//    mov     %dei, r0
//    mov     %ded, r1
//    bitc    %hwflag, 0
//    bits    %hwflag, 1
//loop1:  
//    bitt    %hwflag, 0
//    bz      loop1
//          
//    bitc    %hwflag, 1
//clearloop1:  
//    bitc    %hwflag, 0
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    bitt    %hwflag, 0
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    bitt    %hwflag, 0
//    bnz     clearloop1
//    movl    r0, 0x1e
//    movh    r0, 0xfc
//    mov     %ded, r2
//    mov     %dei, r3
//    call    wait
//    mov     %ded, r4
//    mov     %dei, r5
//    nop     
//    call    wait
//    mov     %ded, r6
//    mov     %dei, r7
//    nop     
//    call    wait
//    mov     %ded, r8
//    mov     %dei, r9
//    nop     
//    call    wait
//    mov     %ded, r10
//    mov     %dei, r11
//    nop     
//    call    wait
//    mov     %ded, r12
//    mov     %dei, r13
//    nop     
//    call    wait
//    mov     %ded, r14
//    mov     %dei, r15
//    nop     
//    call    wait
//    mov     r2, %fmode
//    mov     r1, %tc
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %amode
//    mov     %ded, r2
//    nop     
//    call    wait
//    mov     r2, %imask
//    mov     r1, %ireq
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %ip0
//    mov     r1, %ip1
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %loop0
//    mov     r1, %loop1
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %loop2
//    mov     r1, %loop3
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %guard
//    mov     r1, %vitr
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %smode
//    mov     r1, %tpc
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %cb0_beg
//    mov     r1, %cb1_beg
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %cb0_end
//    mov     r1, %cb1_end
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//    call    wait
//    mov     r2, %timer0
//    mov     r1, %timer1
//    mov     %ded, r2
//    mov     %dei, r1
//    nop     
//self1:  
//    bits    %smode, 4
//    bitc    %smode, 5
//    bitc    %smode, 0
//    bitc    %smode, 1
//    bits    %smode, 7
//    movl    r0, 0x11
//    movh    r0, 0x0
//    mov     r1, %imask
//    st      r1, r0, 0
//    mov     r1, 0
//    mov     %imask, r1
//    movl    r0, 0x12
//    movh    r0, 0x0
//    mov     r1, %tpc
//    st      r1, r0, 0
//    movl    r0, 0x10
//    movh    r0, 0x0
//    movl    r1, 0x2
//    movh    r1, 0xbf
//    st      r1, r0, 0
//    movl    r0, 0x20
//    movh    r0, 0xfb
//    mov     %tpc, r0
//    mov     %pc, %tpc
//    
//wait:   
//    bitc    %hwflag, 0
//    bits    %hwflag, 1
//loop3:  
//    bitt    %hwflag, 0
//    bz      loop3
//    
//    bitc    %hwflag, 1
//clearloop3:
//    bitc    %hwflag, 0
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    bitt    %hwflag, 0
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    nop     
//    bitt    %hwflag, 0
//    bnz     clearloop3
//    mov     %pc, %rpc
//*****************************************************************************
