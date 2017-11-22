//***************************************************************************************************
//MP3_api.h          2005-9-19 9:59        copyright  rock-chip  authour zyz
//***************************************************************************************************
#ifndef _MP3API_H
#define _MP3API_H
//---------------------------------------------------------------------------------------------------
#define MPEG_PHASE2_LSF 0

typedef struct mp3_decoder {        /* size 6230 */
  short int frame_start;            /* bitsize 16, bitpos 0 */
  short int main_data_end;          /* bitsize 16, bitpos 16 */
  short int main_init;              /* bitsize 16, bitpos 32 */
  short int odd_offset;             /* bitsize 16, bitpos 48 */
  short int *load_main_pkptr;       /* bitsize 16, bitpos 64 */
  short int MainBuffer[1736];       /*:uint16 bitsize 27776, bitpos 80 */
  short int *main_pkptr;            /* bitsize 16, bitpos 27856 */
  short int main_pack_state;        /* bitsize 16, bitpos 27872 */
  short int main_totbit;            /* bitsize 16, bitpos 27888 */
  short int *head_pkptr;            /* bitsize 16, bitpos 27904 */
  short int head_pack_state;        /* bitsize 16, bitpos 27920 */
  short int head_totbit;            /* bitsize 16, bitpos 27936 */
  short int old_x;                  /* bitsize 16, bitpos 27952 */
  long  int delay1[2][2][17][8];       /*:uint16:uint16:uint16:uint16 bitsize 17408, bitpos 27968 */
  long  int delay2[2][2][17][8];       /* :uint16:uint16:uint16:uint16 bitsize 17408, bitpos 45376 */
  short int delay_index[2];         /*:uint16  bitsize 32, bitpos 62784 */
  long  int prevblck[2][32][18];       /*:uint16:uint16:uint16 * bitsize 36864, bitpos 62816 */
} MP3_DECODER_STRUCT;

typedef short int al_table[16][16]; /*:uint16:uint16*/

typedef struct mp3_layer {          /* size 12 */
  short int version;                /* bitsize 16, bitpos 0 */
  short int lay;                    /* bitsize 16, bitpos 16 */
  short int error_protection;       /* bitsize 16, bitpos 32 */
  short int bitrate_index;          /* bitsize 16, bitpos 48 */
  short int sampling_frequency;     /* bitsize 16, bitpos 64 */
  short int padding;                /* bitsize 16, bitpos 80 */
  short int extension;              /* bitsize 16, bitpos 96 */
  short int mode;                   /* bitsize 16, bitpos 112 */
  short int mode_ext;               /* bitsize 16, bitpos 128 */
  short int copyright;              /* bitsize 16, bitpos 144 */
  short int original;               /* bitsize 16, bitpos 160 */
  short int emphasis;               /* bitsize 16, bitpos 176 */
} layer;

typedef struct mp3_layer *the_layer;

typedef struct mp3_frame {          /* size 19 */
  layer header;                     /* bitsize 192, bitpos 0 */
  short int actual_mode;            /* bitsize 16, bitpos 192 */
  al_table *alloc;                  /* bitsize 16, bitpos 208 */
  short int tab_num;                /* bitsize 16, bitpos 224 */
  short int stereo;                 /* bitsize 16, bitpos 240 */
  short int jsbound;                /* bitsize 16, bitpos 256 */
  short int sblimit;                /* bitsize 16, bitpos 272 */
  short int MP3_ErrCode;            /* bitsize 16, bitpos 288 */
  short int mpeg2_5;
  short int Bitrate;                /*bitrate value*/
} frame_params;

typedef struct mp3_info {           /* size 18 */
  short int part2_3_length;         /* bitsize 16, bitpos 0 */
  short int big_values;             /* bitsize 16, bitpos 16 */
  short int global_gain;            /* bitsize 16, bitpos 32 */
  short int scalefac_compress;      /* bitsize 16, bitpos 48 */
  short int window_switching_flag;  /* bitsize 16, bitpos 64 */
  short int block_type;             /* bitsize 16, bitpos 80 */
  short int mixed_block_flag;       /* bitsize 16, bitpos 96 */
  short int table_select[3];        /*:uint16 bitsize 48, bitpos 112 */
  short int subblock_gain[3];       /* :uint16 bitsize 48, bitpos 160 */
  short int region0_count;          /* bitsize 16, bitpos 208 */
  short int region1_count;          /* bitsize 16, bitpos 224 */
  short int preflag;                /* bitsize 16, bitpos 240 */
  short int scalefac_scale;         /* bitsize 16, bitpos 256 */
  short int count1table_select;     /* bitsize 16, bitpos 272 */
} my_gr_info;

struct gr_info_s {                  /* size 18 id 53 */
  short int part2_3_length;         /* bitsize 16, bitpos 0 */
  short int big_values;             /* bitsize 16, bitpos 16 */
  short int global_gain;            /* bitsize 16, bitpos 32 */
  short int scalefac_compress;      /* bitsize 16, bitpos 48 */
  short int window_switching_flag;  /* bitsize 16, bitpos 64 */
  short int block_type;             /* bitsize 16, bitpos 80 */
  short int mixed_block_flag;       /* bitsize 16, bitpos 96 */
  short int table_select[3];        /* :uint16 bitsize 48, bitpos 112 */
  short int subblock_gain[3];       /*:uint16 bitsize 48, bitpos 160 */
  short int region0_count;          /* bitsize 16, bitpos 208 */
  short int region1_count;          /* bitsize 16, bitpos 224 */
  short int preflag;                /* bitsize 16, bitpos 240 */
  short int scalefac_scale;         /* bitsize 16, bitpos 256 */
  short int count1table_select;     /* bitsize 16, bitpos 272 */
};

typedef struct mp3_side_info {      /* size 82 */
  short int main_data_begin;        /* bitsize 16, bitpos 0 */
  short int private_bits;           /* bitsize 16, bitpos 16 */
  struct mp3_scf {                  /* size 40 */
    short int scfsi[4];                 /* :uint16 bitsize 64, bitpos 0 */
    struct gr_info_s gr[2];         /* bitsize 576, bitpos 64 */ 
  } ch[2];                          /*:uint16 bitsize 1280, bitpos 32 */
} III_side_info_t;

typedef struct iii_scalefac {       /* size 62 */
  short int l[23];                  /* :uint16 bitsize 368, bitpos 0 */
  short int s[13][3];               /* :uint16:uint16 bitsize 624, bitpos 368 */
} III_scalefac_t[2];                /*:uint16*/

typedef struct bandindex {          /* size 37 */
  short int l[23];                  /* bitsize 368, bitpos 0 */
  short int s[14];                  /* bitsize 224, bitpos 368 */
} BandIndexStruct;

struct huffcodetab {                /* size 3 id 58 */
  short int *val;                   /* bitsize 16, bitpos 0 */
  short int linbits;                /* bitsize 16, bitpos 16 */
  short int treelen;                /* bitsize 16, bitpos 32 */
};

//---------------------------------------------------------------------------------------------------
#endif
//***************************************************************************************************
