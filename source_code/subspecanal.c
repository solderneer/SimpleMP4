/************************************************************
  Copyright (C), 2005-2006, ROCK-CHIPS Semi. Co., Ltd.
  FileName: subspecanal.c
  Author:Huangxudong        Version :1.0     Date:2005-10-7
  Description:      对pcm波形数据进行频谱分析,并从中抽取10段
  频谱线显示      
  Version:          1.0
  Function List:   
    1. specline10()  对pcm波形数据进行128点傅立叶变换并获取10段
    频谱线DB值          
***********************************************************/

//#include <stdio.h>

#define  FFTSIZE     128
#define  Nums_Band   17

void   fft128(short *fftinbuffer,short k);

__attribute__((section(".mp3_text,\"ax\"")))
specline10(long *bufferin,short *yline10)
{
    long  fftspec[Nums_Band];
    long  powervalue1,powervalue2,powervalue3;
    short  y10log10[Nums_Band];
    short fftinfirst[256];
    short fftinsecond[256];
    short fftinthird[256];
    long  fftdb[23]={   63096,  158489,    251188,    398107,    514532,    630957,    999999,  1584893,
                      2511886,  3246478,  3981071,   6309573,   9999999,  15848931,  20483897, 25118864, 
                     39810717, 63095734, 99999999, 129244659, 158489319, 251188645, 398107172           };
    short i,k;
    
    for(i = 0; i < (FFTSIZE << 1); i = i + 2)
    {
          k=i;                              //k=i/2;
          fftinfirst[i]=bufferin[k]>>21;     //fftinfirst[i]=bufferin[k]>>5;
          fftinfirst[i+1]=0;                //fftinfirst[i+1]=0;
          fftinsecond[i]=bufferin[256+k]>>21;//fftinsecond[i]=bufferin[128+k]>>5;
          fftinsecond[i+1]=0;               //fftinsecond[i+1]=0;
          fftinthird[i]=bufferin[512+k]>>21; //fftinthird[i]=bufferin[256+k]>>5;
          fftinthird[i+1]=0;                //fftinthird[i+1]=0;
    }
    
    fft128(fftinfirst,k);
    fft128(fftinsecond,k);
    fft128(fftinthird,k);
    
    for(i = 0; i < (Nums_Band << 1); i = i + 2) {
        powervalue1=(long)fftinfirst[i]*fftinfirst[i]+(long)fftinfirst[i+1]*fftinfirst[i+1];
        powervalue2=(long)fftinsecond[i]*fftinsecond[i]+(long)fftinsecond[i+1]*fftinsecond[i+1];
        powervalue3=(long)fftinthird[i]*fftinthird[i]+(long)fftinthird[i+1]*fftinthird[i+1];
        fftspec[i>>1]=(powervalue1>powervalue2)?powervalue1:powervalue2;
        if(fftspec[i>>1]<powervalue3) fftspec[i>>1]=powervalue3;
          
        if(fftspec[i>>1] >= 398107172) {
                y10log10[i>>1] = 23;    
        } else {
            if(fftspec[i>>1] < 63096) {
                y10log10[i>>1] = 0;             
            } else {
                for(k=0;k<=22;k++) {
                    if((fftspec[i>>1] >= fftdb[k])&&(fftspec[i>>1] < fftdb[k+1])){
                        y10log10[i>>1] = k+1;
                        break;
                    }
                }                
           }       
        }
    }
    
    for(i = 0; i < Nums_Band; i ++) {
      yline10[i ]=  y10log10[i];
    }                                                    
}
