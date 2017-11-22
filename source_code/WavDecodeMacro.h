/*
********************************************************************************
*          Copyright (C),2004-2005, tonyzyz, Fuzhou Rockchip Co.,Ltd.
*                             All Rights Reserved
*                                    V1.00
* FileName   : WavDecodeMacro.h
* Author     : zyf
* Description: 
* History    :        
*           <author>        <time>     <version>       <desc>
*            zyf       2005-12-6       1.0            ORG
*
********************************************************************************
*/
#ifndef _WAVDECODEMACRO_H
#define _WAVDECODEMACRO_H

//------------------------------------------------------------------------------

#define MUSIC_PLAY_STATE_FFW    1
#define MUSIC_PLAY_STATE_FFD    0

#define WAV_SUCCEEDED(Status) ((Status) >= 0)
#define WAV_FAILED(Status) ((Status)<0)

#define     TIME_GUAGE_CONST                205              //Time Guage Count


#define abs(x) ((x)>0?(x):-(x))

#define i8  char
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned long
#define i16  short
#define i32  long
#define	MAX_INT16  32767
#define MIN_INT16  -32768
	

#define IDELTA_COUNT 3

typedef struct waveformat_extended_tag {
i16 wformattag; /* format type *///[20]
i16 nchannels; /* number of channels (i.e. mono, stereo...) */
i32 nsamplespersec; /* sample rate *///[24]
i32 navgbytespersec; /* for buffer estimation */
i16 nblockalign; /* block size of data */
i16 wbitspersample; /* number of bits per sample of mono data */
i16 cbsize; /* the count in bytes of the extra size */}
waveformatex;


typedef struct ms_adpcm_data
{
	//_Track *track;
	//AFvirtualfile *fh;
	//adpcmwaveformat adpcminfo;
	/*
		We set framesToIgnore during a reset1 and add it to
		framesToIgnore during a reset2.
	*/
	//AFframecount	framesToIgnore;
	//waveformatex wfxx;

	i16 wformattag; /* format type *///[20]
	i16	channelCount;/* number of channels (i.e. mono, stereo...) */
	i32 nsamplespersec; /* sample rate *///[24]
	i32 navgbytespersec; /* for buffer estimation */
	i16 blockalign; /* block size of data */
	i16 wbitspersample; /* number of bits per sample of mono data */
	i16 cbsize; /* the count in bytes of the extra size */

	i16 samplesPerBlock;
//	i32	blockAlign;//, samplesPerBlock;
	
	/* a is an array of numCoefficients ADPCM coefficient pairs. */
	i16 numCoefficients;
//	i16 coefficients[256][2];
} ms_adpcm_data;

typedef struct ms_adpcm_state
{
	u16	predictor;
	u16	delta;
	i16	sample1, sample2;
	i16	coefficient1,coefficient2;	
} ms_adpcm_state;



i16 WAVms_adpcm_decode_sample (struct ms_adpcm_state *state,	u16 code);
i16 WAVms_adpcm_decode_block (ms_adpcm_data *msadpcm, u16 *encoded,	i16 *decoded);
#endif
//******************************************************************************
