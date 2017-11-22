/*
**************************************************************************************************
* Run on ROCKMP3(LSI402)
* Developed by WMA team
* Copyright (c) 2005 ROCK-CHIPS
* All Rights Reserved.
* 
*
* DESCRIPTION: 
*	@file				:ZSP_copybits.S
* @module			:get PCM from "pau->m_rgiCoefReconOrig" to PCM buffer
* @author1		:Huang XuDong
* @author2		:
* @author3		:
* @version		: 1.0
* @since 2005.9.25
* @log
*	N   | 	 NAME 	 	|   	 DATE 	 	 |		 REMARKS
*	1		|YIFENG ZHAO		|2005-10-13 14:48|		modify
**************************************************************************************************
*/

//#define PCM_FORMAT_LRLR		//if define PCM_FORMAT_LRLR,pcm format is:LRLRLRLRLRLR
														//else pcm format is:LLLLLLLLRRRRRRRR

 #include "WmaDecodeMacro.h"

/****************************************************
   main Function Name:  wmagetpcmsub();
   sub-Function  Name: 
   Author            : Huang XuDong
   Time              : 2005-9-20
   Version           : 1st edition 
  ***************************************************/
  
/**************************program section begin***********/  

.global  _WMAGetPCMsub
WMA_DECODER_TEXT_ASM
.walign  2
_WMAGetPCMsub:
!!!!!!!!!!!!!!!!!!!!!!!!push stack!!!!!!!!!!!!!!
 	                    stdu      r0, r12, -2          
 	                    stdu      r2, r12, -2                   
 	                    stu       r7, r12, -1                   
 	                    stdu      r8, r12, -2                   
 	                    stdu      r10, r12, -2 
 	                    stu       r13, r12, -1                 
 	                    stdu      r14, r12, -2                  
 	                    mov       r0, %rpc                      
 	                    stu       r0, r12, -1
 	                    mov				r0,%fmode
 	                    stu       r0, r12, -1
 	                    mov				r0,%loop0
 	                    stu       r0, r12, -1
!!!!!!!!r4=numpcm,r5=pcmbufferch0,r6=pcmbufferch1 r12<-ppbDst ppbDst2!!!!!!!
                     bits       %fmode,4 	   
                     mov        r13,16
                     add        r13,r12
                     ldu         r8,r13,1            !!r8=ppbDst
                     
                     mov        r10,r4
                     add        r10,-1            !!r10=numpcm-1
                     mov        %loop0,r10
                     mov        r14,r5            !!r14=pcmbufferch0
                     mov        r15,r6             !!r15=pcmbufferch1
                     mov        r9,16
                     ld					r6,r13							!!r6=ppbDst2
                     
                      
wmagetpcmsub_for:
	                  lddu       r0,r14,2            !!r1r0=pcfpcmbuf[0],r14=&pcfpcmbuf[1]
	                  shla.e     r0,r9
	                  shra.e     r0,r9
	                  stu        r0,r8,1
	                  lddu       r2,r15,2            !!r3r2=pcfpcmbuf[0],r15=&pcfpcmbuf[1]
	                  shla.e     r2,r9
	                  shra.e     r2,r9
//#ifdef PCM_FORMAT_LRLR
                    stu					r2,r8,1							//output PCM format
            //:LRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLR
//#else	                 
                    //stu        r2,r6,1							//output PCM format
                //:LLLLLLLLLLLLLLLLLLLLLRRRRRRRRRRRRRRRRRRRR			
//#endif  
	                  
	                  agn0       wmagetpcmsub_for               
	                                       
                   // bitc       %fmode,4           !!clear saturation


 !!!!!!!!!!!!!!!!!!!!!!!pop stack!!!!!!!!!!!!!!!!!!!!
 	                    mov       r6, r12                       
                      add       r6,1     
                      ldu       r0, r6, 1                     
 	                    mov       %loop0, r0 
 	                    ldu       r0, r6, 1                     
 	                    mov       %fmode, r0                                         
 	                    ldu       r0, r6, 1                     
 	                    mov       %rpc, r0                      
 	                    lddu      r14, r6, 2                
 	                    ldu       r13,r6,1    
 	                    lddu      r10, r6, 2                    
 	                    lddu      r8, r6, 2                     
 	                    ldu       r7, r6, 1                     
 	                    lddu      r2, r6, 2                     
 	                    lddu      r0, r6, 2                     
 	                    add       r6, -1                        
 	                    mov       r12, r6           
                           ret 
/*******************program  section end ****************/ 


