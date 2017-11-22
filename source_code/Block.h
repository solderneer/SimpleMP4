
#ifdef GAME_MODULE

#ifndef _IN_BLOCK_H
#define _IN_BLOCK_H
#define _GAME_BLOCK_OVER
#define ROCK2_OVERLAY

#ifndef _GAME_BLOCK_OVER
    #define GAME_BLOCK_CODE 
    #define GAME_BLOCK_DATA
#else
    #define GAME_BLOCK_CODE     __attribute__((section(".game_text, \"ax\"")))
    #define GAME_BLOCK_DATA     __attribute__((section(".game_data, \"aw\"")))
    #define GAME_BLOCK_BSS      __attribute__((section(".game_bss, \"aw\"")))
#endif

/*
********************************************************************************
    定义游戏基本块的大小
********************************************************************************
*/
/*
********************************************************************************
    定义游戏区域大小
********************************************************************************    
*/
#define GAME_SCOPE_ROW      20
#define GAME_SCOPE_COL      10

#define BASE_BLOCK_XSIZE    12
#define BASE_BLOCK_YSIZE    12

//#define GAME_DISPLAY_ASP

#ifdef GAME_DISPLAY_ASP
    #define GAME_DISP_OFFSET_X  16
    #define GAME_DISP_OFFSET_Y  16
#else
    #define GAME_DISP_OFFSET_X  32
    #define GAME_DISP_OFFSET_Y  32
#endif
#define GAME_DISP_NEXT_BLOCK_X      GAME_DISP_OFFSET_X+GAME_SCOPE_COL*BASE_BLOCK_XSIZE+16
#define GAME_DISP_NEXT_BLOCK_Y      32

#define GAME_DISP_SCORE_X						GAME_DISP_NEXT_BLOCK_X+1
#define	GAME_DISP_SCORE_Y						GAME_DISP_NEXT_BLOCK_Y + BASE_BLOCK_YSIZE*4 + 4

#define GAME_DISP_LEVEL_X						GAME_DISP_SCORE_X
#define GAME_DISP_LEVEL_Y						GAME_DISP_SCORE_Y + 28

#define	GAME_DISP_LINE_X						GAME_DISP_SCORE_X
#define GAME_DISP_LINE_Y						GAME_DISP_LEVEL_Y + 28

#define GAME_DISP_STATE_X                       GAME_DISP_LINE_X
#define GAME_DISP_STATE_Y                       GAME_DISP_LINE_Y

#define GAME_STOP           (1<<0)
#define GAME_RUN            (1<<1)
#define GAME_PAUSE          (1<<2)



#define BLOCK_CHK_NULL      (1<<0)
#define BLOCK_CHK_LEFT      (1<<1)
#define BLOCK_CHK_RIGHT     (1<<2)
#define BLOCK_CHK_DOWN      (1<<3)

#define BLOCK_IMAGE_NULL    0xFFFF

#define BLOCK_LIMIT_MASK    0xE007




//#define GAME_BLOCK_INIT     1
//#define GAME_BLOCK_STOP     2
//#define GAME_BLOCK_PAUSE    3
//#define GAME_BLOCK_RUN      4
//#define GAME_BLOCK_END      5



extern unsigned int GameMsgArray[8];
#endif

#endif

