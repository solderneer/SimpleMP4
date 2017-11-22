/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WmaDecodeStruct.h
* Author     : tonyzyz
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            tonyzyz       05/11/24       1.0            ORG
*
********************************************************************************
*/
#ifndef _WMADECODESTRUCT_H
#define _WMADECODESTRUCT_H
//------------------------------------------------------------------------------
//in wma.h
#include "Macro.h"
#ifdef _DRM9_ENABLE_
    #define PD_DRM
#endif

#ifdef PD_DRM
    #include "drmpd.h"
#endif

#define     MAX_STRING              30
#define     ID3_TAG_SIZE            30
#define     ID3_GENREN_INFO         10

//Entropy Mode
#define FOURTYFOURS_QB 	1
#define FOURTYFOURS_OB 	2
#define SIXTEENS_OB     3

typedef struct 
{
    //WMA FILE INFORMATION
    int title_len ;
    int author_len ;
    int title[MAX_STRING];
    int author[MAX_STRING];
    long TotalPlayTime ;
    int min,sec ;
}
WMA_FILE;

typedef struct{
    long   id3_size;
    short  id3_title_size;
    short  id3_singer_size;
    short  id3_album_size;
    short  id3_title[ID3_TAG_SIZE];
    short  id3_singer[ID3_TAG_SIZE];
    short  id3_album[ID3_TAG_SIZE];
    short  id3_genren;
    short  id3_genren_info[ID3_GENREN_INFO];
    short  id3_ver;
    short  id3_check;
}WMA_ID3_TAG;


typedef struct tagWMAFileHdrStateInternal 
{
    long currPacketOffset ; //U64    /* not just for packets */
    long nextPacketOffset ;    //U64       /* ASF header */
    long cbHeader ;    //ASF header size
    int cbPacketSize ;    //ASF packet size
    //long cbAudioSize ;
    int cPackets ;    //the num of the ASF packets    
    long cbFirstPacketOffset ;    //U64
    long cbLastPacketOffset ;    //U64
   // long dwFilePropertiesFlags ;    
    /* audio prop */
    int nVersion ;    //Stream Version	
    int wFormatTag ;    //WAV Format tag , v1:0x160; v2:0x161
    long nSamplesPerSec ;    //Sample rate
    long nAvgBytesPerSec ;
    long nBlockAlign ;
    int nChannels ;
    long nSamplesPerBlock ;
    int nEncodeOpt ;
    
    // Added in V9
    
    int wBitsPerSample ;    // container size
    int wValidBitsPerSample ;    // actual valid data depth in the decoded stream
    //long dwChannelMask ;
    
    //int wOriginalBitDepth ;    // actual valid data depth in the original source (informational)  
    
    /* Marker */
    //long m_dwMarkerNum ;
    //MarkerEntry *m_pMarker;
    
    // Members added by Amit 
    int wAudioStreamId ;
    
    // Members added by Naveen for MBR support
    ////9-8int wTargetAudioStreamNumber ;
    //int cAudioStreams ;
    
//add by zyf 2005-10-12 for system use
        int	m_iEntropyMode;
    	int     m_cFrameSampleHalf;
		int     decoder_reset;
        int	wLastPacketNum;		
    /* DRM */
#ifdef PD_DRM
    //Word16    pbSecretData[32]; //used in drm 10
    long    cbSecretData;
    int    pbType[16];
    int    pbKeyID[__CB_DECL(32)];

    //used in DRM 10
    //Word16    cbCEncExData;
    //Word16   *pbCEncExData;

    /* License Store */
    long   m_dwLicenseLen;
    unsigned int   m_pLicData[__CB_DECL(256)];  // Is it enough?
#endif
}
tWMAFileHdrStateInternal ;



/*******************Buffer Stream Struct********************/
typedef struct //ZSP_stream 
{
    int *Pbuf_begin; 
    int	offset_words ; // the offset on data[]
    int use_bits ;//use the bits <15
    int validdata ;//how many data on validdata    
    int	*pbuf_end ;//the end of the data[]
    
    int	buffersize;
    int Pbuf_WRoffset;
    
    int packetoffset;
    int packetoffsetT;    
    int offset_wordsT;
    int use_bitsT;
    int asfpacketoffset;//the size of ASF packet header
#ifdef PD_DRM
    //// add by hwg, for drm	
    // the input buffer address of the begining of the packet
    int offset_words_backup ;
    int use_bits_backup ;
    int validdata_backup ;  
    // 	
    int UndecryptPayloadByte;	

    // only for debug
    //FILE *pDecryptFile;
    //FILE *pUndecryptFile;
#endif        
    //int * WMAfile;//
}
zspstream ;

typedef struct ZSP_GETPCM {
    int con_left_pcm_num;
    int con_pcm_start;
    int buff_have_pcm_num;
    int PCM_BUFFER_ready;
    int *PCM_buffer0;
    int *PCM_buffer1;
}ZSP_getpcm;

//------------------------------------------------------------------------------
//in wma.h
typedef enum tagWMAFileStatus
{
    cWMA_NoErr,                 /* -> always first entry */
                                /* remaining entry order is not guaranteed */
    cWMA_Failed=100,
    cWMA_BadArgument,
    cWMA_BadAsfHeader,
    cWMA_BadPacketHeader,
    cWMA_BrokenFrame,
    cWMA_NoMoreFrames,
    cWMA_BadSamplingRate,
    cWMA_BadNumberOfChannels,
    cWMA_BadVersionNumber,
    cWMA_BadWeightingMode,
    cWMA_BadPacketization,

    cWMA_BadDRMType,
    cWMA_DRMFailed,
    cWMA_DRMUnsupported,

    cWMA_DemoExpired,

    cWMA_BadState,
    cWMA_Internal,               /* really bad */
    cWMA_NoMoreDataThisTime
} tWMAFileStatus;

//------------------------------------------------------------------------------
typedef union {
    struct 
    {
        unsigned int    bPcmBufEmpty:       1,
                        bPcmBufFull:        1,
                        bBufferIndex:       1,
                        bWMAFileFFW:        1,
                        bWMADecodeInit:     1,
                        bWMADecodeEnable:   1,
                        bWMAId3Have:        1,
                        bWMALrcHave:        1,
                        bReserved:          8;
    }bc;
    unsigned int word;
} _WMA_FLAG;

typedef enum _WmaFileRate 
{
    WMAHighRate=0,
    WMAMidRate,
    WMALowRate
}WmaFileRate ;

extern int g_WmaHas_DRM;
extern int g_WmaFileRate;
//------------------------------------------------------------------------------
#endif
//******************************************************************************
