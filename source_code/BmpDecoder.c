

#include "Macro.h"
#include "Memap.h"
#include "MsgDef.h"
#include "Resource.h"
#include "dma.h"
#include "global.h"
#include "BmpDecoder.h"
#include "JpegDecodeGlobal.h"

/****************************************************************************************************
*函数名	:DecoderBmpHead
*功能描述	:解析bmp头
*参数参数	:无
*输出参数	:无
*版本说明	:作者		时间		备注
*		 kitchen	           11/29/2006		原版
******************************************************************************************************/
__attribute__((section(".bmp_text, \"ax\"")))
void BMP_ROM2LCD_ONE_LINE(unsigned short *src, unsigned short length)
{

    unsigned long volatile srcRGB,send2lcd_block;
    DMA_CHANNEL_REG *DmaChl;
  	   
    while(DMA_CHANNEL_FULL == ( DmaChl = GetDmaChannelState(1))); 

    srcRGB = src;
//---------------------------------------------------------------------
    DmaChl->pDmaFnct    = DMA_FNCT_NULL;
    DmaChl->pDmaFnctArg = DMA_FNCT_NULL;
//---------------------------------------------------------------------     
    DmaChl->DmaSar = ((unsigned long)(srcRGB)<<1)+(unsigned long)0x40000; 
    DmaChl->DmaDar = ((unsigned long)(LCD_DATA)<<1);
    
	DmaChl->DmaCtl.Lsb = 0x00000113;           
    DmaChl->DmaCtl.Msb = (unsigned long) length ;
    
    DmaChl->DmaCfg.Lsb = 0x00000000;
    DmaChl->DmaCfg.Msb = 0x00001004; 
    
    DmaChl->pResFlag   = &BMP2LCD;

	DmaChannelSetup(1);      
}

__attribute__((section(".bmp_text, \"ax\"")))
void Byte2Word_SH(unsigned short *pbuf, unsigned short *pSbuf, unsigned short len)
{
	unsigned short	length,j;
	for(length = len/2,j = 0; j < length;j++){
		pbuf[j] = pSbuf[j*2]|pSbuf[j*2 + 1]<<8;
	}

	return;
}

__attribute__((section(".bmp_text, \"ax\"")))
short	DecoderBmpHead()
{
	unsigned short *infilebuf;
	short		i,j;
	unsigned short	redmask,greenmask,bluemask;
	
	infilebuf = RGB_Buffer;
	FileRead(infilebuf,0x1b*2,hhMainFile);
	Byte2Word_SH(infilebuf, infilebuf, 0x1b*2);
	BmpFileHead.bfType = infilebuf[0];
	
	if(infilebuf[0] != 0x4d42)
		return -1;

	BmpFileHead.bfSize = ((unsigned long) infilebuf[2])<<16|infilebuf[1];
	BmpFileHead.bfOffBits = ((unsigned long)infilebuf[6])<<16|infilebuf[5];


	BmpInfoHead.biSize = infilebuf[7]|((unsigned long)infilebuf[8])<<16;
	BmpInfoHead.biWidth = infilebuf[9]|((unsigned long)infilebuf[10])<<16;
	BmpInfoHead.biHeight = infilebuf[11]|((unsigned long)infilebuf[12])<<16;

	BmpInfoHead.biPlanes = infilebuf[13];
	BmpInfoHead.biBitCount = infilebuf[14];
	BmpInfoHead.biCompression = infilebuf[15]|((unsigned long)infilebuf[16])<<16;
	if(BmpInfoHead.biCompression == 1 && BmpInfoHead.biCompression == 2)
		return -1;

	BmpInfoHead.biSizeImage = infilebuf[17]|((unsigned long)infilebuf[18])<<16;
	
	BmpInfoHead.biXPelsPerMeter  = infilebuf[19]|(unsigned long)infilebuf[20]<<16;
	BmpInfoHead.biYPelsPerMeter =  infilebuf[21]|(unsigned long)infilebuf[22]<<16;
	BmpInfoHead.biClrUsed		=  infilebuf[23]|(unsigned long)infilebuf[24]<<16;
	BmpInfoHead.biClrImportant  =  infilebuf[25]|(unsigned long)infilebuf[26]<<16;
	if(BmpInfoHead.biCompression == 3L){
		FileRead(infilebuf,12,hhMainFile);
		Byte2Word_SH(infilebuf, infilebuf, 12);
		MASK_BITS[0] = infilebuf[0];
		MASK_BITS[1] = infilebuf[2];
		MASK_BITS[2] = infilebuf[4];
	}
	else{
		MASK_BITS[0] = 0x7C00;
		MASK_BITS[1] = 0x3E0;
		MASK_BITS[2] = 0x1F;
		
	}
	if(BmpInfoHead.biBitCount == 16){
			
		redmask = MASK_BITS[0];
		greenmask = MASK_BITS[1];
		bluemask = MASK_BITS[2];
		Bitsns[0] = 0;
		Bitsns[1] = 0;
		Bitsns[2] = 0;
					
		for ( j=0;j<16;j++){
			if ((redmask>>j)&0x01) Bitsns[0]++;
			if ((greenmask>>j)&0x01) Bitsns[1]++;
			if ((bluemask>>j)&0x01) Bitsns[2]++;
		}
		Bitsns[1]+=Bitsns[0]; Bitsns[2]+=Bitsns[1];	Bitsns[0]=8-Bitsns[0]; Bitsns[1]-=8; Bitsns[2]-=8;
		
	}
	if(BmpInfoHead.biClrImportant == 0 && BmpInfoHead.biBitCount <= 8){
		BmpInfoHead.biClrImportant = 1<<BmpInfoHead.biBitCount;
	}
	for(i = 0; i< BmpInfoHead.biClrImportant;i++){
		FileRead(infilebuf,4,hhMainFile);
		RGB_index[i][0] = infilebuf[0];
		RGB_index[i][1] = infilebuf[1];
		RGB_index[i][2] = infilebuf[2];
	}
	FileSeek(BmpFileHead.bfOffBits, SEEK_SET, hhMainFile);
	return 0;
}

/****************************************************************************************************
*函数名	:BmpPrepareSampling
*功能描述	:计算抽值所需要的参数
*参数参数	:无
*输出参数	:返回解码状态，0: 0k    -1: erro
*版本说明	:作者		时间		备注
*		 kitchen	           11/29/2006		原版
******************************************************************************************************/
__attribute__((section(".bmp_text, \"ax\"")))
short BmpPrepareSampling(void)
{
	long width,height;
	long longside,shortside;
	
	image.renverse = 0;
	if(BmpInfoHead.biHeight<0){
		image.renverse =1;
	}

	width = BmpInfoHead.biWidth > 0 ? BmpInfoHead.biWidth : -BmpInfoHead.biWidth;
	height = BmpInfoHead.biHeight > 0 ? BmpInfoHead.biHeight : -BmpInfoHead.biHeight;
	
	image.screw = 0;
	image.doscale = 0;
	
	longside = width;
	shortside = height;
	
	if(width < height){
		shortside = width;
		longside = height;
		image.screw = 1;
		if(image.renverse)
			DMA_LcdBMPInitInv();
		else
			DMA_LcdBMPInit();
	}
	else{
		if(image.renverse)
			DMA_LcdBMPInitInvX();
		else	
			DMA_LcdBMPInitX();
	}
	image.height = height;
	image.width = width;
	
	image.expendheight = ((image.height*BmpInfoHead.biBitCount + 31)/8)&0xfffffffc;//????4 ????
	image.expendwidth  = ((image.width*BmpInfoHead.biBitCount + 31 )/8)&0xfffffffc;
	
	if(longside <= SCREENWIDTH && shortside <= SCREENHEIGHT){
		image.deswidth = (short)width;
		image.desheight =(short) height;
		image.sampling = 1;
		image.sampwidth = width/image.sampling;
		image.sampheight = height/image.sampling;
		return 0;
	}
	
	image.doscale = 1;
	
	if(longside * SCREENHEIGHT > shortside * SCREENWIDTH){
		image.sampling =(short) ((longside)/SCREENWIDTH);
		image.tocut = 1;
	}else{
		image.sampling =(short) ((shortside)/SCREENHEIGHT);
		image.tocut = 0;
	}

	image.samplong = longside/image.sampling;
	image.sampshort = shortside/image.sampling;
	
	image.sampwidth = width/image.sampling;
	image.sampheight = height/image.sampling;
		
	return 0;
}
/****************************************************************************************************
*函数名	:BmpPrepareSampling
*功能描述	:计算差值所需要的参数
*参数参数	:无
*输出参数	:无
*版本说明	:作者		时间		备注
*		 kitchen	           11/29/2006		原版
******************************************************************************************************/
__attribute__((section(".bmp_text, \"ax\"")))
void	BmpPrepareSacle(void)
{
	if(image.doscale == 1){
		if(image.tocut){
			image.zoomtofit =(short)((SCANLEDOWN*image.sampheight+SCREENHEIGHT - 1)/SCREENWIDTH);
			image.desheight = (short)(image.sampheight*SCANLEDOWN/image.zoomtofit);
			image.deswidth  = (short)(image.sampwidth*SCANLEDOWN/image.zoomtofit);

		}
		else{
			image.zoomtofit =(short)(( SCANLEDOWN*image.sampshort+SCREENWIDTH - 1)/SCREENHEIGHT);	
			image.desheight = (short)(image.sampheight*SCANLEDOWN/image.zoomtofit);
			image.deswidth  = (short)(image.sampwidth*SCANLEDOWN/image.zoomtofit);
		}
		
		
	}
	else{
		image.zoomtofit = SCANLEDOWN;
		image.desheight = (short)image.sampheight;
		image.deswidth  = (short)image.sampwidth;
		
	}
	
	if(image.screw){
		if(image.desheight > SCREENWIDTH)
			image.desheight = SCREENWIDTH;
		if(image.deswidth > SCREENHEIGHT)	
			image.deswidth = SCREENHEIGHT;
	}else{
		if(image.desheight > SCREENHEIGHT)
			image.desheight = SCREENHEIGHT;
		if(image.deswidth > SCREENWIDTH)	
			image.deswidth = SCREENWIDTH;

	}
			
	return;
}
/****************************************************************************************************
*函数名	:BmpGetOneLine
*功能描述	:采样一行的图片
*参数参数	:无
*输出参数	:无
*版本说明	:作者		时间		备注
*		 kitchen	           12/12/2006		原版
******************************************************************************************************/
__attribute__((section(".bmp_text, \"ax\"")))
void	BmpGetOneLine()
{
	long	expendwidth;
	long	used = 0;
	long	wid;
	short sample_p = 0;
	short	j = 0;
	unsigned short 	temp_buff[320*6];
	short	bytes;
	unsigned short mask;
	expendwidth = 0; 
	switch (BmpInfoHead.biBitCount) {
		case 32 :
		case 24 :	
			bytes = BmpInfoHead.biBitCount/8;
			for(wid = image.expendwidth; wid > 0; ){
				short buff_size;
				short temp;
				
				buff_size =wid > 320*3 ?320*3:(short)wid;
					
				FileRead(temp_buff, buff_size, hhMainFile);
				expendwidth += buff_size;
				wid -= buff_size;
				temp = (used +buff_size-bytes +1)/ (bytes*image.sampling) - sample_p;
				for( j = 0; j<=temp;j++ ){
					RGB_Buffer[sample_p*3] = temp_buff[sample_p*bytes*image.sampling- used];
					RGB_Buffer[sample_p*3+1] = temp_buff[sample_p*bytes*image.sampling + 1 -used];
					RGB_Buffer[sample_p*3+2] = temp_buff[sample_p*bytes*image.sampling + 2 -used];
					sample_p++;

				}
				used += buff_size;
								
			}
			break;

		case 16 :
		{
			unsigned short	redmask,greenmask,bluemask;
						
			redmask = MASK_BITS[0];
			greenmask = MASK_BITS[1];
			bluemask = MASK_BITS[2];
				
			for(wid = image.expendwidth/2; wid > 0; ){
					short buff_size;
					short temp;
					
							
					buff_size =(wid) > 320*3 ?320*3:(short)(wid);
						
					FileRead(temp_buff, buff_size*2, hhMainFile);
					Byte2Word_SH(temp_buff, temp_buff, buff_size*2);
					expendwidth += buff_size;
					wid -= buff_size;
					temp = (used +buff_size -1)/ (image.sampling) - sample_p;
					for( j = 0; j<=temp;j++ ){
						RGB_Buffer[sample_p*3] = (temp_buff[sample_p*image.sampling- used]&bluemask)<<Bitsns[0];
						RGB_Buffer[sample_p*3+1] = (temp_buff[sample_p*image.sampling- used]&greenmask)>>Bitsns[1];
						RGB_Buffer[sample_p*3+2] = (temp_buff[sample_p*image.sampling- used]&redmask)>>Bitsns[2];
						sample_p++;
					}
					used += buff_size;
			}

			break;
		}
		case 8 :
		case 4 :
		case 1 :
			bytes = 8/ BmpInfoHead.biBitCount;
			mask = (1<<BmpInfoHead.biBitCount) - 1;
			
			for(wid = image.expendwidth; wid > 0; ){
				short buff_size;
				short temp;
					
				buff_size =wid > 320*3 ?320*3:(short)wid;
					
				FileRead(temp_buff, buff_size, hhMainFile);
				expendwidth += buff_size;

				wid -= buff_size;
				temp = ((used +buff_size)*8 - 1 )/ (BmpInfoHead.biBitCount*image.sampling) - sample_p;
				for( j = 0; j<=temp;j++ ){
					long	pindex;
					short	nindex;
					pindex = ((sample_p*image.sampling*BmpInfoHead.biBitCount)/8- used);
					nindex =8 -BmpInfoHead.biBitCount- (sample_p*image.sampling*BmpInfoHead.biBitCount)%8 ;
					
					nindex = (temp_buff[pindex] >> nindex)&mask;
					
					RGB_Buffer[sample_p*3] = RGB_index[nindex][0];
					RGB_Buffer[sample_p*3+1] = RGB_index[nindex][1];
					RGB_Buffer[sample_p*3+2] = RGB_index[nindex][2];
					sample_p++;
				}
				used += buff_size;
								
		}
	    break;
	  default:
	  	break;  
	}
}

__attribute__((section(".bmp_text, \"ax\"")))
short	BmpSamplingAndScale()
{
	short	i,j;
	long temp;
	short	verblank,horblank;
	short bmplines;
	short scalex,scaley;	
	short	flag = 0;
	short	lines = 0;
	unsigned	short	temp_buff[340*3];
	unsigned	short	*obj;
	unsigned 	short    *aline;
		
	scalex = SCREENWIDTH;
	scaley = SCREENHEIGHT;

	if(image.screw){
		scalex = SCREENHEIGHT;
		scaley = SCREENWIDTH;
	}
	BmpPrepareSacle();

	FileSeek(BmpFileHead.bfOffBits, SEEK_SET, hhMainFile);
	temp = (image.expendwidth)* (image.sampling -1);
	
	
	horblank = scalex - image.deswidth;
	verblank = scaley - image.desheight;
	
	aline = &one_line[horblank/2];
	
	obj = &obj_line[0];
	horblank = verblank/2;
	for(i = 0; i<SCREENWIDTH;i++){
		one_line[i] = 0;
	}

	for(i = 0; i<SCREENWIDTH*3;i++){
		obj_line[i] = 0;
	}
	for(i = 0; i< 9; i++){
		for(j = 0; j< SCREENWIDTH*3;j++){
			rgb_temp_buff[i][j] = 0;
		}
	}
	BMP2LCD = 0;
	for(i = 0; i<horblank;i++ ){
		while(BMP2LCD);
		BMP_ROM2LCD_ONE_LINE(one_line,scalex);
	}
	lines = -1;
	for(bmplines = 0; bmplines<image.desheight;){
		long wid;

		if(flag == 0){
			for(i = 0; i< MAX_LINES && lines < image.sampheight ;i++ ){
				short sample_p = 0;
				lines++;
				
				BmpGetOneLine();
				for(j = 0; j < image.deswidth;j++){		
					short	index;
					short	coefficient1;
					short	coefficient2;
					
					coefficient1 = (j*image.zoomtofit);
					
					index = coefficient1/SCANLEDOWN;
    		
					coefficient1 &= MASTBITS;
					coefficient2 = SCANLEDOWN - coefficient1;
    		
					rgb_temp_buff[i + 1][j*3] =( (coefficient1 * RGB_Buffer[index*3 + 3] + coefficient2 *  RGB_Buffer[index*3]))/SCANLEDOWN ;
					rgb_temp_buff[i + 1][j*3 + 1] =( (coefficient1 * RGB_Buffer[index*3 + 4] + coefficient2 *  RGB_Buffer[index*3 + 1]) )/SCANLEDOWN;
					rgb_temp_buff[i + 1][j*3 + 2] = ((coefficient1 * RGB_Buffer[index*3 + 5] + coefficient2 *  RGB_Buffer[index*3 + 2]))/SCANLEDOWN ;
				}
				FileSeek(temp, SEEK_CUR, hhMainFile);
			}
			flag = 1;	
		}
		else{
			short index;
			short coefficient2;
			short coefficient1;
			
			coefficient1 = (bmplines * image.zoomtofit);
			
			index = coefficient1/SCANLEDOWN;
			
			coefficient1 &= MASTBITS;
			
			if( index >= lines  ){
				for(j = 0; j < image.deswidth;j++){
					rgb_temp_buff[0][3*j] = rgb_temp_buff[MAX_LINES][3*j];
					rgb_temp_buff[0][3*j+1] = rgb_temp_buff[MAX_LINES][3*j+1];
					rgb_temp_buff[0][3*j+2] = rgb_temp_buff[MAX_LINES][3*j+2];
				}
				flag = 0;
			}else{
				coefficient2 = SCANLEDOWN - coefficient1;

			
				index =(index + 1)%MAX_LINES; 
				
				if(coefficient1 == 0){
					for(j = 0; j< image.deswidth;j++){
						obj[3*j] = rgb_temp_buff[index][3*j ];
						obj[3*j + 1] = rgb_temp_buff[index ][3*j + 1];
						obj[3*j + 2] = rgb_temp_buff[index][3*j + 2];
						aline[j] = (unsigned short )(((obj[3*j+2]<<8)&0xf800)|((obj[3*j+1]<<3)&0x7e0)|(obj[3*j]>>3));
					}
				}
				else{
					for(j = 0; j< image.deswidth;j++){
						obj[3*j] = (coefficient2 * rgb_temp_buff[index][3*j] + coefficient1 * rgb_temp_buff[index + 1][3*j])/SCANLEDOWN ;
						obj[3*j + 1] = (coefficient2 *  rgb_temp_buff[index][3*j + 1]+ coefficient1 * rgb_temp_buff[index + 1][3*j + 1])/SCANLEDOWN;
						obj[3*j + 2] = (coefficient2 * rgb_temp_buff[index][3*j + 2]+ coefficient1 * rgb_temp_buff[index + 1][3*j + 2])/SCANLEDOWN;
						aline[j] = (unsigned short )(((obj[3*j+2]<<8)&0xf800)|((obj[3*j+1]<<3)&0x7e0)|(obj[3*j]>>3));
					}
				}
				
				while(BMP2LCD);
				BMP_ROM2LCD_ONE_LINE(one_line,scalex);
				bmplines++;
			
			}
			
		}	
		
	}
	while(BMP2LCD);
	for(i = 0; i<scalex;i++)
		one_line[i] = 0;
		
	for(i = 0; i<verblank - horblank;i++ ){
		while(BMP2LCD);
		BMP_ROM2LCD_ONE_LINE(one_line,scalex);
	}
	while(BMP2LCD);
	return 0;
}


/****************************************************************************************************
*函数名	:main
*功能描述	:bmp解码入口
*参数参数	:无
*输出参数	:返回解码状态，0: 0k    -1: erro
*版本说明	:作者		时间		备注
*		 kitchen	           11/29/2006		原版
******************************************************************************************************/
__attribute__((section(".bmp_text, \"ax\"")))
void BmpDecoder(void)
{
	bitclear_creg(%imask,5);
	MusicSetFreq51();
    SystemInfo.bc.bSysIdleEn = 0;
    SystemInfo.bc.bBackLightOffEn = 1;
    bitset_creg(%imask,5);
	
	hhMainFile = hSlaveFile;
	if (DecoderBmpHead()) {
	//error
	    JpegFileFormatError = 1;
        PreDisplayFlag = DISP_JPEG;
        DisplayRecoveryTime = 0;
        LCD_Init(0/*LCD_INIT_NORMAL*/);
        LCD_SetColor(0);
        LCD_FillRect(0,0,LCD_MAX_XSIZE-1,LCD_MAX_YSIZE-1);
        SysDispDialogBox(0, 0, DIALOG_WARNING_TXT, DIALOG_FILEERR_TXT);
        PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_NONE,0,0,0/*,0,0,0*/);
	} else {
	    BmpPrepareSampling();
	    BmpSamplingAndScale();
	    PostFun2Message(MSG_FUNCTION2_JPEG_DECODE,MSG_FUNCTION2_JPEG_DECODE_END,0,0,0/*,0,0,0*/);
	}
	
	FileClose(hSlaveFile);
    bitclear_creg(%imask,5);
    SystemInfo.bc.bSysIdleEn = 1;
    SystemInfo.bc.bBackLightOffEn = 0;
    SysIdleCounter = 0;
    LightOffCounter = 0;
    JpegTimeReset();
    MusicSetFreq51Return();
    bitset_creg(%imask,5);
}
