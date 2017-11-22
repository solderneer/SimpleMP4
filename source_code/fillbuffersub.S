/*
**************************************************************************************************
*		main Function Name	:		 fillbuffersub(Word16 *des, Word16 *src ,Word16 n)
*		sub-Function  Name	:		
*		Input             	:		1.des, destination address
*														2.src ,source address
														3.n ,the length of double byte to join up  a  word
*		Output							  	none
*
*DESCRIPTION:
*		Author            : YIFENG ZHAO
*		Data              : 2005-9-25
*		Version           : 1.0
*LOG:
*	N   | 	 NAME 	 	|   	 DATE 	 	 |		 REMARKS
*	1		|YIFENG ZHAO	|2005-10-26 10:35|			modify
*											
**************************************************************************************************
*/
.section ".WmaDecoderText","ax"
.global _fillbuffersub	
.walign 2 
_fillbuffersub:  
!!!!!!!!!!!!!!!!!!!!!!!!push stack!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
												stu       r7, r12, -1
  	                    stdu      r8, r12, -2
  	                    stdu       r14, r12, -2
  	                    mov       r7, %rpc                      
  	                    stu       r7, r12, -1
  	                    mov       r7, %loop0
  	                    stu       r7, r12, -1
!!!!!!!!!!!!!!!!r4=des,r5=src,r6=n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  	                     
												cmp       r6, 0
  	                    bz        fillbuffersub_end
  	                    add				r6,-1
  	                    mov				r14,r4							//des
												mov				%loop0, r6
												mov				r15,r5							//src
									
fillbuffersub_loop0:
												lddu			r8,r15,2						//r8=data0,r9=data1
												shll			r8,8								//data0<<8
												or				r8,r9
												stu				r8,r14,1
												agn0			fillbuffersub_loop0
/*----------------
mov		rx,256
loop0:
lddu	r8,r15,2
mov		r0,r9
mac.a		r8,rx
stu  r0,r14,1
ang0 loop0:
------------------*/												
												
fillbuffersub_end:
!!!!!!!!!!!!!!!!!!!!!!!pop stack!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        mov       r6, r12
  	                    add       r6, 1
  	                    ldu       r7, r6, 1                     
  	                    mov       %loop0, r7  	                    
  	                    ldu       r7, r6, 1                     
  	                    mov       %rpc, r7   	                    
  	                    lddu       r14, r6, 2
 	                    	lddu      r8, r6, 2
  	                    ld        r7, r6, 0
  	                    mov       r12, r6
  	                    ret
!!!!!!!!!!!!!!!!!!!!!!!end of fillbuffersub!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 


