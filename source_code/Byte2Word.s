//******************************************************************************
//Byte2Word.S          2005.08.09        copyright  rock-chip  
//******************************************************************************

//-----------------------------------------------------------------------------
//Function: Byte Covert to Word
//Input:    
//Output:   
//C_function: void Byte2Word(unsigned int *pbuf, unsigned int *pSbuf, unsigned int len);
//-----------------------------------------------------------------------------
    .section    ".text", "ax"
	.global	_Byte2Word
	.walign 2
	
_Byte2Word:
	stdu	r2,		r12,	-2
	stu		r15,	r12,	-1
	mov		r2,		%loop0
	stu		r2,		r12,	-1
	mov     r2,     %rpc
	stu     r2,     r12,    -1

	cmp		r6,		0
	ble		Byte2Word_End
	add		r6,		-1
	mov		r15,	r5
	mov		%loop0,	r6
	movh	r6,		0x01
	movl	r6,		0x00
Byte2Word_Loop0:
	ldu		r3,		r15,	1
	ldu		r2,		r15,	1
	mac.b	r3,		r6
	stu		r2,		r4,	1
	agn0	Byte2Word_Loop0
	
Byte2Word_End:	
	mov		r4,		r12
	add		r4,		1
	!
	ldu     r2,     r4, 1
	mov     %rpc,   r2
	ldu		r2,		r4,	1
	mov		%loop0,	r2
	ldu		r15,	r4,	1
	lddu	r2,		r4,	2
	add		r4,		-1
	mov		r12,	r4
	ret
//-----------------------------------------------------------------------------
//Function: Byte Covert to Word
//Input:    
//Output:   
//C_function: void WriteIRAM(unsigned int Addr, unsigned int wData);
//-----------------------------------------------------------------------------
    	.section 	".text", "ax"
        .global 	_WriteIRAM
        .walign 2
        
_WriteIRAM:
        bits    %smode,4
        st      r5,r4,0
        bitc    %smode,4
        ret
//******************************************************************************

