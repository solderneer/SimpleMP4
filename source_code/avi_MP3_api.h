//***************************************************************************************************
//MP3_api.h          2005-9-19 9:59        copyright  rock-chip  authour zyz
//***************************************************************************************************
#ifndef _MP3API_H
#define _MP3API_H
//---------------------------------------------------------------------------------------------------
#define MPEG_PHASE2_LSF 0


typedef struct mp3_decoder { 
  short int *head_pkptr; 
  short int head_pack_state; 
  short int head_totbit; 
  short int old_x; 
  long delay1[2][2][17][8]; 
  long delay2[2][2][17][8]; 
  short int delay_index[2]; 
  long prevblck[2][32][18]; 
} MP3_DECODER_STRUCT;

typedef short int al_table[16][16];
typedef struct mp3_layer { 
  short int version; 
  short int lay; 
  short int error_protection; 
  short int bitrate_index;
  short int sampling_frequency; 
  short int padding; 
  short int extension; 
  short int mode; 
  short int mode_ext; 
  short int copyright; 
  short int original; 
  short int emphasis; 
} layer;
typedef struct mp3_layer *the_layer;
typedef struct mp3_frame { 
  layer header; 
  short int actual_mode; 
  al_table *alloc; 
  short int tab_num; 
  short int stereo; 
  short int jsbound; 
  short int sblimit; 
  short int MP3_ErrCode; 
  short int mpeg2_5;
  short int Bitrate;
} frame_params;
//---------------------------------------------------------------------------------------------------
#endif
//***************************************************************************************************
