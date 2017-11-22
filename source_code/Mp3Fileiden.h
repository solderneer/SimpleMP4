//******************************************************************************
//  Mp3Fileiden.h          2005.11.05        copyright  rock-chip  authour hxd
//******************************************************************************
#ifndef  _MP3FILEIDEN_H
#define  _MP3FILEIDEN_H
//------------------------------------------------------------------------------
short filemp3iden(short *startpointer,long* mp3filesampling,short* mp3filebitrate);
//------------------------------------------------------------------------------


///xing vbr header
#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008
///xing vbr header 


typedef struct fileiden_mp3_decoder { /* size 6230 */
  short int head_pack_state; /* bitsize 16, bitpos 27920 */
  short int *head_pkptr; /* bitsize 16, bitpos 27904 */
  short int head_totbit; /* bitsize 16, bitpos 27936 */
  short int nslots;
  short int Filemp3nosync;
} fileiden_MP3_DECODER_STRUCT;

typedef struct fileiden_mp3_layer { /* size 12 */
  short int version; /* bitsize 16, bitpos 0 */
  short int lay; /* bitsize 16, bitpos 16 */
  short int error_protection; /* bitsize 16, bitpos 32 */
  short int bitrate_index; /* bitsize 16, bitpos 48 */
  short int sampling_frequency; /* bitsize 16, bitpos 64 */
  short int padding; /* bitsize 16, bitpos 80 */
  short int extension; /* bitsize 16, bitpos 96 */
  short int mode; /* bitsize 16, bitpos 112 */
  short int mode_ext; /* bitsize 16, bitpos 128 */
  short int copyright; /* bitsize 16, bitpos 144 */
  short int original; /* bitsize 16, bitpos 160 */
  short int emphasis; /* bitsize 16, bitpos 176 */
} fileiden_layer;

typedef struct fileiden_mp3_frame { /* size 19 */
  fileiden_layer header; /* bitsize 192, bitpos 0 */
} fileiden_frame_params;

#endif
//******************************************************************************
