#include "Resource.h"
#include <stdlib.h>
#include <creg.h>
#include "macro.h"
#include "KeyMacro.h"
#include "KeyGlobal.h"
#include "Global.h"
#include "Lcd.h"

#include "MsgDef.h"

#ifdef GAME_MODULE
    #include "Block.h"

void GameFreqUp(void);
void GameFreqDown(void);

#define GAME_TMR_CHANNEL       4
#define GAME_TMR_CHANNEL2      5

#define GAME_KEY_PLAY_DOWN       KEY_VAL_PLAY_DOWN//KEY_VAL_EQ_DOWN
#define GAME_KEY_DOWN_DOWN       KEY_VAL_FFD_DOWN//KEY_VAL_DOWN_DOWN
#define GAME_KEY_UP_DOWN         KEY_VAL_FFW_DOWN//KEY_VAL_UP_DOWN
#define GAME_KEY_LEFT_DOWN       KEY_VAL_DOWN_DOWN//KEY_VAL_FFW_DOWN
#define GAME_KEY_RIGHT_DOWN      KEY_VAL_UP_DOWN//KEY_VAL_FFD_DOWN
#define GAME_KEY_DROP_OFF_DOWN   KEY_VAL_MENU_DOWN //KEY_VAL_MODE_DOWN

#define GAME_KEY_PLAY            KEY_VAL_PLAY_SHORT_UP//KEY_VAL_EQ_SHORT_UP
#define GAME_KEY_DOWN            KEY_VAL_FFD_SHORT_UP//KEY_VAL_DOWN_SHORT_UP
#define GAME_KEY_UP              KEY_VAL_FFW_SHORT_UP//KEY_VAL_UP_SHORT_UP
#define GAME_KEY_LEFT            KEY_VAL_DOWN_SHORT_UP//KEY_VAL_FFW_SHORT_UP
#define GAME_KEY_RIGHT           KEY_VAL_UP_SHORT_UP//KEY_VAL_FFD_SHORT_UP
#define GAME_KEY_DROP_OFF        KEY_VAL_MENU_SHORT_UP//KEY_VAL_MODE_SHORT_UP

#define GAME_KEY_PLAY_L          KEY_VAL_PLAY_PRESS_START
#define GAME_KEY_DOWN_L          KEY_VAL_FFD_PRESS_START//KEY_VAL_DOWN_PRESS_START
#define GAME_KEY_UP_L            KEY_VAL_FFW_PRESS_START//KEY_VAL_UP_PRESS_START
#define GAME_KEY_LEFT_L          KEY_VAL_DOWN_PRESS_START//KEY_VAL_FFW_PRESS_START
#define GAME_KEY_RIGHT_L         KEY_VAL_UP_PRESS_START//KEY_VAL_FFD_PRESS_START
#define GAME_KEY_DROP_OFF_L      KEY_VAL_MENU_PRESS_START//KEY_VAL_MODE_PRESS_START

#define GAME_KEY_PLAY_L_UP       KEY_VAL_PLAY_LONG_UP//KEY_VAL_EQ_LONG_UP
#define GAME_KEY_DOWN_L_UP       KEY_VAL_FFW_LONG_UP//KEY_VAL_DOWN_LONG_UP
#define GAME_KEY_UP_L_UP         KEY_VAL_FFD_LONG_UP//KEY_VAL_UP_LONG_UP
#define GAME_KEY_LEFT_L_UP       KEY_VAL_DOWN_LONG_UP//KEY_VAL_FFW_LONG_UP
#define GAME_KEY_RIGHT_L_UP      KEY_VAL_UP_LONG_UP//KEY_VAL_FFD_LONG_UP
#define GAME_KEY_DROP_OFF_L_UP   KEY_VAL_MENU_LONG_UP//KEY_VAL_MODE_LONG_UP

GAME_BLOCK_BSS unsigned int  BlockScopeBuf[GAME_SCOPE_ROW][GAME_SCOPE_COL];
GAME_BLOCK_BSS unsigned int  BlockInsertFlag[GAME_SCOPE_ROW+4];
GAME_BLOCK_BSS unsigned int  NextBlock;
GAME_BLOCK_BSS unsigned int  NextBlockAspect;
GAME_BLOCK_BSS unsigned int  CurBlock;
GAME_BLOCK_BSS unsigned int  BlockAspect;

GAME_BLOCK_BSS unsigned int  MoveBlock_X;
GAME_BLOCK_BSS unsigned int  MoveBlock_Y;

GAME_BLOCK_BSS unsigned int  DownFlag;
GAME_BLOCK_BSS unsigned int  GameKeyFlag;
GAME_BLOCK_BSS unsigned char GameLevel;
GAME_BLOCK_BSS unsigned long GameScore;
GAME_BLOCK_BSS unsigned char GameState;
GAME_BLOCK_BSS unsigned int  GameKeyDlyTmr;
GAME_BLOCK_BSS unsigned int  GameKeyValue;




/*　放在定时器中断　*/
GAME_BLOCK_BSS unsigned int TimerModuleTimer;

GAME_BLOCK_DATA unsigned char StringPAUSE[] = "PAUSE";
GAME_BLOCK_DATA unsigned char StringLevel[] = "Level";
GAME_BLOCK_DATA unsigned char StringNEXT[] = "NEXT";
GAME_BLOCK_DATA unsigned char StringScore[] = "Score";





/*　执行周期为定时器中断周期的倍数　*/


GAME_BLOCK_BSS
unsigned int GameMsgArray[8];

GAME_BLOCK_CODE
void GameExit()
{
   // DisplayFlag = DISP_MENU;
    Fun2MsgArray[0] = MSG_FUNCTION2_NONE;//2006-2-10 15:35
    Fun2ModuleChange();

}
/*
********************************************************************************
    定义7种基本方块形状，4个方向的值
********************************************************************************
*/
GAME_BLOCK_DATA
unsigned int Block[7][4][4]={
    {   //BLOCK _|
        {0x04,0x07,0x00,0x00},
        {0x06,0x04,0x04,0x00},
        {0x0E,0x02,0x00,0x00},
        {0x02,0x02,0x06,0x00}
    },
    {   //BLOCK |_
        {0x02,0x0E,0x00,0x00},
        {0x04,0x04,0x06,0x00},
        {0x07,0x04,0x00,0x00},
        {0x06,0x02,0x02,0x00}    
    },
    {   //BLOCK 田
        {0x06,0x06,0x00,0x00},
        {0x06,0x06,0x00,0x00},
        {0x06,0x06,0x00,0x00},
        {0x06,0x06,0x00,0x00}    
    },
    {   //BLOCK |_
        //        |
        {0x06,0x0C,0x00,0x00},
        {0x04,0x06,0x02,0x00},
        {0x06,0x0C,0x00,0x00},
        {0x04,0x06,0x02,0x00}    
    },
    {   //BLOCK _|
        //     |
        {0x06,0x03,0x00,0x00},
        {0x02,0x06,0x04,0x00},
        {0x06,0x03,0x00,0x00},
        {0x02,0x06,0x04,0x00}    
    },
    {   //BLOCK T
        {0x02,0x07,0x00,0x00},
        {0x04,0x06,0x04,0x00},
        {0x07,0x02,0x00,0x00},
        {0x02,0x06,0x02,0x00}    
    },
    {
        {0x00,0x0F,0x00,0x00},
        {0x04,0x04,0x04,0x04},
        {0x00,0x0F,0x00,0x00},
        {0x04,0x04,0x04,0x04}
    }
};
GAME_BLOCK_DATA
unsigned int BlockImage[8][BASE_BLOCK_XSIZE*BASE_BLOCK_YSIZE]={
    {
        0xFE19, 0xFCD3, 0xFCD3, 0xFC10, 0xFB6D, 0xFB6D, 0xFACB, 0xF9C7, 0xF9C7, 0xF882, 0xF882, 0xF882,
        0xFE19, 0xFCD3, 0xFCD3, 0xFC10, 0xFB6D, 0xFB6D, 0xFACB, 0xF9C7, 0xF9C7, 0xF882, 0xF882, 0xF882,
        0xFCB3, 0xD800, 0xD800, 0xE800, 0xE800, 0xE800, 0xF000, 0xF800, 0xF800, 0xF820, 0xF820, 0xF820,
        0xFC10, 0xE800, 0xE800, 0xF000, 0xF000, 0xF000, 0xF800, 0xF945, 0xF945, 0xF861, 0xF861, 0xF861,
        0xFC10, 0xE800, 0xE800, 0xF000, 0xF000, 0xF000, 0xF800, 0xF945, 0xF945, 0xF861, 0xF861, 0xF861,
        0xFB4E, 0xF000, 0xF000, 0xF800, 0xF820, 0xF820, 0xF924, 0xFA69, 0xFA69, 0xF882, 0xF882, 0xF882,
        0xFAAB, 0xF000, 0xF000, 0xF800, 0xF924, 0xF924, 0xFA49, 0xFB4D, 0xFB4D, 0xF8C3, 0xF8C3, 0xF8C3,
        0xFAAB, 0xF000, 0xF000, 0xF800, 0xF924, 0xF924, 0xFA49, 0xFB4D, 0xFB4D, 0xF8C3, 0xF8C3, 0xF8C3,
        0xF9A7, 0xF820, 0xF820, 0xF965, 0xFA69, 0xFA69, 0xFB4D, 0xFC30, 0xFC30, 0xF924, 0xF924, 0xF924,
        0xF882, 0xF841, 0xF841, 0xF082, 0xF0C3, 0xF0C3, 0xE945, 0xE1E7, 0xE1E7, 0xF0A2, 0xF0A2, 0xF0A2,
        0xF882, 0xF841, 0xF841, 0xF082, 0xF0C3, 0xF0C3, 0xE945, 0xE1E7, 0xE1E7, 0xF0A2, 0xF0A2, 0xF0A2,
        0xF882, 0xF841, 0xF841, 0xF082, 0xF0C3, 0xF0C3, 0xE945, 0xE1E7, 0xE1E7, 0xF0A2, 0xF0A2, 0xF0A2
    },                                                                                          
    {                                                                                           
        0xFFFF, 0xFFFA, 0xFFFA, 0xFFF6, 0xFFF4, 0xFFF4, 0xFFF0, 0xFFEC, 0xFFEC, 0xFFE3, 0xFFE3, 0xFFE3, 
        0xFFFF, 0xFFFA, 0xFFFA, 0xFFF6, 0xFFF4, 0xFFF4, 0xFFF0, 0xFFEC, 0xFFEC, 0xFFE3, 0xFFE3, 0xFFE3, 
        0xFFFA, 0xFFE2, 0xFFE2, 0xFFE1, 0xFFE2, 0xFFE2, 0xFFE1, 0xFFE2, 0xFFE2, 0xFFE1, 0xFFE1, 0xFFE1, 
        0xFFF6, 0xFFE2, 0xFFE2, 0xFFE2, 0xFFE1, 0xFFE1, 0xFFE2, 0xFFE9, 0xFFE9, 0xFFE6, 0xFFE6, 0xFFE6, 
        0xFFF6, 0xFFE2, 0xFFE2, 0xFFE2, 0xFFE1, 0xFFE1, 0xFFE2, 0xFFE9, 0xFFE9, 0xFFE6, 0xFFE6, 0xFFE6, 
        0xFFF3, 0xFFE3, 0xFFE3, 0xFFE1, 0xFFE2, 0xFFE2, 0xFFE8, 0xFFEF, 0xFFEF, 0xFFE8, 0xFFE8, 0xFFE8, 
        0xFFF0, 0xFFE2, 0xFFE2, 0xFFE1, 0xFFE8, 0xFFE8, 0xFFEE, 0xFFF3, 0xFFF3, 0xFFEA, 0xFFEA, 0xFFEA, 
        0xFFF0, 0xFFE2, 0xFFE2, 0xFFE1, 0xFFE8, 0xFFE8, 0xFFEE, 0xFFF3, 0xFFF3, 0xFFEA, 0xFFEA, 0xFFEA,
        0xFFEB, 0xFFE3, 0xFFE3, 0xFFE9, 0xFFEF, 0xFFEF, 0xFFF3, 0xFFF7, 0xFFF7, 0xFFED, 0xFFED, 0xFFED,
        0xFFE6, 0xFFE3, 0xFFE3, 0xFFE6, 0xFFE7, 0xFFE7, 0xFFE8, 0xFFEA, 0xFFEA, 0xFFE7, 0xFFE7, 0xFFE7,
        0xFFE6, 0xFFE3, 0xFFE3, 0xFFE6, 0xFFE7, 0xFFE7, 0xFFE8, 0xFFEA, 0xFFEA, 0xFFE7, 0xFFE7, 0xFFE7, 
        0xFFE6, 0xFFE3, 0xFFE3, 0xFFE6, 0xFFE7, 0xFFE7, 0xFFE8, 0xFFEA, 0xFFEA, 0xFFE7, 0xFFE7, 0xFFE7  
    },                                                                                          
    {                                                                                           
        0xD67F, 0x9D9F, 0x9D9F, 0x851F, 0x6CBF, 0x6CBF, 0x5C3F, 0x3BBF, 0x3BBF, 0x131F, 0x131F, 0x131F,
        0xD67F, 0x9D9F, 0x9D9F, 0x851F, 0x6CBF, 0x6CBF, 0x5C3F, 0x3BBF, 0x3BBF, 0x131F, 0x131F, 0x131F, 
        0x9D1E, 0x025B, 0x025B, 0x029D, 0x02BD, 0x02BD, 0x02DE, 0x02FF, 0x02FF, 0x02DF, 0x02DF, 0x02DF,
        0x84BF, 0x02DD, 0x02DD, 0x02FE, 0x02FF, 0x02FF, 0x031F, 0x2B9F, 0x2B9F, 0x0B3F, 0x0B3F, 0x0B3F,
        0x84BF, 0x02DD, 0x02DD, 0x02FE, 0x02FF, 0x02FF, 0x031F, 0x2B9F, 0x2B9F, 0x0B3F, 0x0B3F, 0x0B3F,
        0x745F, 0x02DE, 0x02DE, 0x02FF, 0x031F, 0x031F, 0x237F, 0x541F, 0x541F, 0x135E, 0x135E, 0x135E,
        0x5BFF, 0x02FE, 0x02FE, 0x031F, 0x2B7F, 0x2B7F, 0x4BFF, 0x6C9F, 0x6C9F, 0x139D, 0x139D, 0x139D,
        0x5BFF, 0x02FE, 0x02FE, 0x031F, 0x2B7F, 0x2B7F, 0x4BFF, 0x6C9F, 0x6C9F, 0x139D, 0x139D, 0x139D,
        0x3B7F, 0x031F, 0x031F, 0x2B9F, 0x541F, 0x541F, 0x6C9F, 0x8D3F, 0x8D3F, 0x1BFD, 0x1BFD, 0x1BFD,
        0x127F, 0x0ABF, 0x0ABF, 0x12FE, 0x1B3E, 0x1B3E, 0x2B5D, 0x3BBD, 0x3BBD, 0x137B, 0x137B, 0x137B,
        0x127F, 0x0ABF, 0x0ABF, 0x12FE, 0x1B3E, 0x1B3E, 0x2B5D, 0x3BBD, 0x3BBD, 0x137B, 0x137B, 0x137B, 
        0x127F, 0x0ABF, 0x0ABF, 0x12FE, 0x1B3E, 0x1B3E, 0x2B5D, 0x3BBD, 0x3BBD, 0x137B, 0x137B, 0x137B
    },                                                                                          
    {                                                                                           
        0xBFF7, 0x9FD3, 0x9FD3, 0x87F0, 0x6FED, 0x6FED, 0x57EA, 0x3FE7, 0x3FE7, 0x17E2, 0x17E2, 0x17E2,
        0xBFF7, 0x9FD3, 0x9FD3, 0x87F0, 0x6FED, 0x6FED, 0x57EA, 0x3FE7, 0x3FE7, 0x17E2, 0x17E2, 0x17E2,
        0x77CF, 0x06E0, 0x06E0, 0x0740, 0x0760, 0x0760, 0x07A0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
        0x67ED, 0x0740, 0x0740, 0x0780, 0x07C0, 0x07C0, 0x07E0, 0x37E6, 0x37E6, 0x0FC1, 0x0FC1, 0x0FC1,
        0x67ED, 0x0740, 0x0740, 0x0780, 0x07C0, 0x07C0, 0x07E0, 0x37E6, 0x37E6, 0x0FC1, 0x0FC1, 0x0FC1,
        0x57EA, 0x0780, 0x0780, 0x07C0, 0x07E0, 0x07E0, 0x2FE5, 0x57EA, 0x57EA, 0x17A2, 0x17A2, 0x17A2,
        0x3FE8, 0x07A0, 0x07A0, 0x07E0, 0x2FE5, 0x2FE5, 0x4FE9, 0x77EE, 0x77EE, 0x1763, 0x1763, 0x1763,
        0x3FE8, 0x07A0, 0x07A0, 0x07E0, 0x2FE5, 0x2FE5, 0x4FE9, 0x77EE, 0x77EE, 0x1763, 0x1763, 0x1763,
        0x1FE4, 0x07E0, 0x07E0, 0x37E6, 0x57EA, 0x57EA, 0x6FED, 0x97F1, 0x97F1, 0x1F44, 0x1F44, 0x1F44,
        0x0FE1, 0x0FC1, 0x0FC1, 0x17A2, 0x1F83, 0x1F83, 0x2F65, 0x4748, 0x4748, 0x16C3, 0x16C3, 0x16C3,
        0x0FE1, 0x0FC1, 0x0FC1, 0x17A2, 0x1F83, 0x1F83, 0x2F65, 0x4748, 0x4748, 0x16C3, 0x16C3, 0x16C3, 
        0x0FE1, 0x0FC1, 0x0FC1, 0x17A2, 0x1F83, 0x1F83, 0x2F65, 0x4748, 0x4748, 0x16C3, 0x16C3, 0x16C3   
    },                                                                                          
    {                                                                                           
        0xBFFF, 0x9FBF, 0x9FBF, 0x87DF, 0x6FDF, 0x6FDF, 0x5FDF, 0x3FDF, 0x3FDF, 0x17FF, 0x17FF, 0x17FF,
        0xBFFF, 0x9FBF, 0x9FBF, 0x87DF, 0x6FDF, 0x6FDF, 0x5FDF, 0x3FDF, 0x3FDF, 0x17FF, 0x17FF, 0x17FF,
        0x8FDF, 0x06DB, 0x06DB, 0x073D, 0x075D, 0x075D, 0x079E, 0x07BF, 0x07BF, 0x07FF, 0x07FF, 0x07FF,
        0x7FFF, 0x073D, 0x073D, 0x077E, 0x079E, 0x079E, 0x07DF, 0x2FDF, 0x2FDF, 0x17DE, 0x17DE, 0x17DE,
        0x7FFF, 0x073D, 0x073D, 0x077E, 0x079E, 0x079E, 0x07DF, 0x2FDF, 0x2FDF, 0x17DE, 0x17DE, 0x17DE,
        0x67FF, 0x075D, 0x075D, 0x079F, 0x07DF, 0x07DF, 0x27DF, 0x57DF, 0x57DF, 0x1FBE, 0x1FBE, 0x1FBE,
        0x4FFF, 0x079E, 0x079E, 0x07DF, 0x27DF, 0x27DF, 0x4FDF, 0x6FDF, 0x6FDF, 0x27BD, 0x27BD, 0x27BD,
        0x4FFF, 0x079E, 0x079E, 0x07DF, 0x27DF, 0x27DF, 0x4FDF, 0x6FDF, 0x6FDF, 0x27BD, 0x27BD, 0x27BD,
        0x2FFF, 0x07BF, 0x07BF, 0x2FDF, 0x57DF, 0x57DF, 0x6FFF, 0x87FF, 0x87FF, 0x379C, 0x379C, 0x379C,
        0x17FF, 0x0F9F, 0x0F9F, 0x179E, 0x1F7E, 0x1F7E, 0x2F5D, 0x3F3D, 0x3F3D, 0x2F1A, 0x2F1A, 0x2F1A,
        0x17FF, 0x0F9F, 0x0F9F, 0x179E, 0x1F7E, 0x1F7E, 0x2F5D, 0x3F3D, 0x3F3D, 0x2F1A, 0x2F1A, 0x2F1A,
        0x17FF, 0x0F9F, 0x0F9F, 0x0F9F, 0x179E, 0x179E, 0x1F7E, 0x2F5D, 0x2F5D, 0x2F1A, 0x2F1A, 0x2F1A
    },
    {
        0xFEFA, 0xFE92, 0xFE92, 0xFE50, 0xFE0D, 0xFE0D, 0xFDCA, 0xFD86, 0xFD86, 0xFD22, 0xFD22, 0xFD22,
        0xFEFA, 0xFE92, 0xFE92, 0xFE50, 0xFE0D, 0xFE0D, 0xFDCA, 0xFD86, 0xFD86, 0xFD22, 0xFD22, 0xFD22,  
        0xFDF2, 0xE480, 0xE480, 0xECC0, 0xF4E0, 0xF4E0, 0xF500, 0xFD40, 0xFD40, 0xFD20, 0xFD20, 0xFD20,
        0xFDF0, 0xECE0, 0xECE0, 0xF500, 0xFD20, 0xFD20, 0xFD40, 0xFD86, 0xFD86, 0xFD22, 0xFD22, 0xFD22,
        0xFDF0, 0xECE0, 0xECE0, 0xF500, 0xFD20, 0xFD20, 0xFD40, 0xFD86, 0xFD86, 0xFD22, 0xFD22, 0xFD22,
        0xFDAD, 0xF500, 0xF500, 0xFD20, 0xFD40, 0xFD40, 0xFD65, 0xFDCB, 0xFDCB, 0xFD42, 0xFD42, 0xFD42,
        0xFD8A, 0xF520, 0xF520, 0xFD40, 0xFD85, 0xFD85, 0xFDCA, 0xFE0E, 0xFE0E, 0xF523, 0xF523, 0xF523,
        0xFD8A, 0xF520, 0xF520, 0xFD40, 0xFD85, 0xFD85, 0xFDCA, 0xFE0E, 0xFE0E, 0xF523, 0xF523, 0xF523,
        0xFD67, 0xFD40, 0xFD40, 0xFD86, 0xFDCA, 0xFDCA, 0xFE0D, 0xFE72, 0xFE72, 0xED43, 0xED43, 0xED43,
        0xFCE2, 0xFD01, 0xFD01, 0xF502, 0xF503, 0xF503, 0xED05, 0xED28, 0xED28, 0xE4A2, 0xE4A2, 0xE4A2, 
        0xFCE2, 0xFD01, 0xFD01, 0xF502, 0xF503, 0xF503, 0xED05, 0xED28, 0xED28, 0xE4A2, 0xE4A2, 0xE4A2,
        0xFCE2, 0xFD01, 0xFD01, 0xF502, 0xF503, 0xF503, 0xED05, 0xED28, 0xED28, 0xE4A2, 0xE4A2, 0xE4A2  
    },                                                                                         
    {                                                                                          
        0xFFDF, 0xFE7F, 0xFE7F, 0xFDBF, 0xFCFF, 0xFCFF, 0xFC1F, 0xFAFF, 0xFAFF, 0xF8DF, 0xF8DF, 0xF8DF,
        0xFFDF, 0xFE7F, 0xFE7F, 0xFDBF, 0xFCFF, 0xFCFF, 0xFC1F, 0xFAFF, 0xFAFF, 0xF8DF, 0xF8DF, 0xF8DF,
        0xFE1F, 0xF87E, 0xF87E, 0xF87F, 0xF87F, 0xF87F, 0xF85F, 0xF8DF, 0xF8DF, 0xF85F, 0xF85F, 0xF85F,
        0xFD3F, 0xF8BF, 0xF8BF, 0xF89F, 0xF87F, 0xF87F, 0xF89F, 0xFADF, 0xFADF, 0xF8DF, 0xF8DF, 0xF8DF,
        0xFD3F, 0xF8BF, 0xF8BF, 0xF89F, 0xF87F, 0xF87F, 0xF89F, 0xFADF, 0xFADF, 0xF8DF, 0xF8DF, 0xF8DF,
        0xFC7F, 0xF8BF, 0xF8BF, 0xF85F, 0xF8DF, 0xF8DF, 0xFA7F, 0xFC3F, 0xFC3F, 0xF91F, 0xF91F, 0xF91F,
        0xFBBF, 0xF87F, 0xF87F, 0xF89F, 0xFA7F, 0xFA7F, 0xFBFF, 0xFD1F, 0xFD1F, 0xF95F, 0xF95F, 0xF95F,
        0xFBBF, 0xF87F, 0xF87F, 0xF89F, 0xFA7F, 0xFA7F, 0xFBFF, 0xFD1F, 0xFD1F, 0xF95F, 0xF95F, 0xF95F,
        0xFA7F, 0xF8DF, 0xF8DF, 0xFADF, 0xFC1F, 0xFC1F, 0xFD1F, 0xFE5F, 0xFE5F, 0xF9DF, 0xF9DF, 0xF9DF,
        0xF93F, 0xF91F, 0xF91F, 0xF9DF, 0xFA1F, 0xFA1F, 0xFA5F, 0xFADF, 0xFADF, 0xF8FF, 0xF8FF, 0xF8FF, 
        0xF93F, 0xF91F, 0xF91F, 0xF9DF, 0xFA1F, 0xFA1F, 0xFA5F, 0xFADF, 0xFADF, 0xF8FF, 0xF8FF, 0xF8FF,
        0xF93F, 0xF91F, 0xF91F, 0xF9DF, 0xFA1F, 0xFA1F, 0xFA5F, 0xFADF, 0xFADF, 0xF8FF, 0xF8FF, 0xF8FF 
    },                                                                                         
    {                                                                                          
        0x0000, 0x0821, 0x0821, 0x1863, 0x2084, 0x2084, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x0000, 0x0821, 0x0821, 0x1863, 0x2084, 0x2084, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x0821, 0x0821, 0x0821, 0x1863, 0x2084, 0x2084, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x1863, 0x1863, 0x1863, 0x1863, 0x2084, 0x2084, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x1863, 0x1863, 0x1863, 0x1863, 0x2084, 0x2084, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x2084, 0x2084, 0x2084, 0x2084, 0x2084, 0x2084, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x28A5, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x38E7, 0x38E7, 0x38E7, 0x38E7, 0x38E7, 0x38E7, 0x38E7, 0x38E7, 0x38E7, 0x4929, 0x514A, 0x514A,
        0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x514A, 0x514A,
        0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x4929, 0x514A, 0x514A,
        0x514A, 0x514A, 0x514A, 0x514A, 0x514A, 0x514A, 0x514A, 0x514A, 0x514A, 0x514A, 0x514A, 0x514A
    }
};

GAME_BLOCK_CODE
void GameTmrFunc(void *parg)
{
    DownFlag++;
    TimerReset(GAME_TMR_CHANNEL);
    TimerStart(GAME_TMR_CHANNEL);
}

GAME_BLOCK_CODE
void GameKeyDlyFunc(void *parg)
{
    GameKeyFlag++;
    TimerReset(GAME_TMR_CHANNEL2);
    TimerStart(GAME_TMR_CHANNEL2);
}

//------------------------------------------------------------------------------
//  显示下一个方块（只作显示用）
//------------------------------------------------------------------------------
GAME_BLOCK_CODE
void BlockDispNext(void)
{
    unsigned int i,j;
    unsigned int *pBlock;
    unsigned int *pBlockImage;
    unsigned int DMask=0x08;
    
    pBlock      = &Block[NextBlock][NextBlockAspect][0];    
    pBlockImage = &BlockImage[NextBlock][0];
    
//此处需先对下一方块显示位置的底图进行重绘

    //LCD_FillRect(GAME_DISP_NEXT_BLOCK_X, GAME_DISP_NEXT_BLOCK_Y,
    //                GAME_DISP_NEXT_BLOCK_X+28, GAME_DISP_NEXT_BLOCK_Y+28);

    for(i=0; i<4; i++){
        DMask = 0x08;
        for(j=0; j<4; j++){
            if(*pBlock & DMask){
       		    LCDDEV_DrawMem(GAME_DISP_NEXT_BLOCK_X+j*BASE_BLOCK_XSIZE, 
       		               GAME_DISP_NEXT_BLOCK_Y+i*BASE_BLOCK_YSIZE, 
       		               BASE_BLOCK_XSIZE, BASE_BLOCK_YSIZE, 16, BASE_BLOCK_XSIZE, pBlockImage);
       		}else{
       		    LCDDEV_DrawMem(GAME_DISP_NEXT_BLOCK_X+j*BASE_BLOCK_XSIZE, 
       		               GAME_DISP_NEXT_BLOCK_Y+i*BASE_BLOCK_YSIZE, 
       		               BASE_BLOCK_XSIZE, BASE_BLOCK_YSIZE, 16, BASE_BLOCK_XSIZE, &BlockImage[7][0]);
       		}
            DMask >>= 1;
        }
        pBlock++;
    }
}

//------------------------------------------------------------------------------
//  
//------------------------------------------------------------------------------


GAME_BLOCK_CODE
void DrawGameArea(void)
{
    unsigned int i,j;
    unsigned int *pBlockImage;
    for(i=0; i<GAME_SCOPE_ROW; i++){
        for(j=0; j<GAME_SCOPE_COL; j++){
            if(BlockScopeBuf[i][j] != BLOCK_IMAGE_NULL){
           	    pBlockImage = &BlockImage[BlockScopeBuf[i][j]][0];
           	    LCDDEV_DrawMem(j*BASE_BLOCK_XSIZE+GAME_DISP_OFFSET_X, i*BASE_BLOCK_YSIZE+GAME_DISP_OFFSET_Y, 
                            BASE_BLOCK_XSIZE, BASE_BLOCK_YSIZE, 16, BASE_BLOCK_XSIZE, pBlockImage);
           	}else{
           	    LCDDEV_DrawMem(j*BASE_BLOCK_XSIZE+GAME_DISP_OFFSET_X, i*BASE_BLOCK_YSIZE+GAME_DISP_OFFSET_Y, 
                            BASE_BLOCK_XSIZE, BASE_BLOCK_YSIZE, 16, BASE_BLOCK_XSIZE, &BlockImage[7][0]);
           	}
        }
    }
}


GAME_BLOCK_CODE
void GameValueInit(void)
{
    unsigned int i,j;
    unsigned int color_tmp;
    unsigned int tmpbuf[8];
    
    Sys2InitFlag.bc.bGameModuleInitEnable = 0;
    Sys2InitFlag.bc.bGameSysInitEnable = 0;
    
    for(i=0; i<GAME_SCOPE_ROW; i++){
        for(j=0; j<GAME_SCOPE_COL; j++){
            BlockScopeBuf[i][j] = BLOCK_IMAGE_NULL;
        }
        BlockInsertFlag[i] = 0;
    }
//------------------------------------------------------------------------------
//    TimerInit();
//------------------------------------------------------------------------------
    
    //------2006-2-14 9:48
    read_creg(%timer0 , j);	    //10 * 1000 * 1us = 10ms
    srand( j );
    //-----------------
    BlockInsertFlag[i++] = 0xFFFF;
    BlockInsertFlag[i++] = 0xFFFF;
    BlockInsertFlag[i++] = 0xFFFF;
    BlockInsertFlag[i++] = 0xFFFF;
    CurBlock        = 0;
    BlockAspect     = 0;
    NextBlock       = ((unsigned int)rand())%7;//1;  //  获取0-6之间的随机数
    NextBlockAspect = ((unsigned int)rand())%4;//2;  //  获取0-3之间的随机数
    DownFlag        = 0;
    GameKeyFlag     = 0;
    GameLevel       = 0;
    GameScore       = 0;
    GameState       = GAME_STOP;
    MoveBlock_Y     = 0;
    MoveBlock_X     = 6;
    TimerCfgFunc(GAME_TMR_CHANNEL, GameTmrFunc, 0);
    TimerCfgFunc(GAME_TMR_CHANNEL2, GameKeyDlyFunc, 0);
}

GAME_BLOCK_CODE
void ClearGameArea(void)
{
    unsigned int i;
    unsigned int j;
    
    for(i=0; i<GAME_SCOPE_ROW; i++){
        for(j=0; j<GAME_SCOPE_COL; j++){
            BlockScopeBuf[i][j] = BLOCK_IMAGE_NULL;
        }
        BlockInsertFlag[i] = 0;
    }
    //------2006-2-14 9:48
    read_creg(%timer0 , j);	    //10 * 1000 * 1us = 10ms
    srand( j );
    //-----------------
    BlockInsertFlag[i++] = 0xFFFF;
    BlockInsertFlag[i++] = 0xFFFF;
    BlockInsertFlag[i++] = 0xFFFF;
    BlockInsertFlag[i++] = 0xFFFF;
}

GAME_BLOCK_CODE
void GameDispInit(void)
{
    unsigned int color_tmp;
    unsigned int tmpbuf[8];
    unsigned long score_tmp;
//------------------------------------------------------------------------------
//  以下为界面相关的初始化（测试用的简单）
//------------------------------------------------------------------------------
    color_tmp = LCD_GetColor();
    LCD_SetColor(0xFFFF);
    
    LCD_DrawHLine(GAME_DISP_OFFSET_X-2, GAME_DISP_OFFSET_Y-2, 
                    GAME_DISP_OFFSET_X+BASE_BLOCK_XSIZE*10+1);
    LCD_DrawHLine(GAME_DISP_OFFSET_X-2, GAME_DISP_OFFSET_Y+BASE_BLOCK_YSIZE*20+1, 
                    GAME_DISP_OFFSET_X+BASE_BLOCK_XSIZE*10+1);
    LCD_DrawVLine(GAME_DISP_OFFSET_X-2, GAME_DISP_OFFSET_Y-2, 
                    GAME_DISP_OFFSET_Y+BASE_BLOCK_YSIZE*20+1);
    LCD_DrawVLine(GAME_DISP_OFFSET_X+BASE_BLOCK_XSIZE*10+1, GAME_DISP_OFFSET_Y-2, 
                    GAME_DISP_OFFSET_Y+BASE_BLOCK_YSIZE*20+1);
    
    LCD_NFDispStringAt(GAME_DISP_NEXT_BLOCK_X+1,GAME_DISP_NEXT_BLOCK_Y-16, StringNEXT);   
    LCD_NFDispStringAt(GAME_DISP_SCORE_X,GAME_DISP_SCORE_Y,  StringScore);
    score_tmp = GameScore;
    tmpbuf[0] = score_tmp/10000+'0';
    score_tmp = score_tmp%10000;
    tmpbuf[1] = score_tmp/1000 +'0';
    score_tmp = score_tmp%1000;
    tmpbuf[2] = score_tmp/100  +'0';
    score_tmp = score_tmp%100;
    tmpbuf[3] = score_tmp/10   +'0';
    score_tmp = score_tmp%10   +'0';
    tmpbuf[4] = score_tmp;
    tmpbuf[5] = 0;

    LCD_SetColor(0);
    LCD_FillRect(GAME_DISP_SCORE_X, GAME_DISP_SCORE_Y+14, GAME_DISP_SCORE_X+32, GAME_DISP_SCORE_Y+14+13);
    LCD_SetColor(0xFFFF);
    LCD_NFDispStringAt(GAME_DISP_SCORE_X, GAME_DISP_SCORE_Y+14, tmpbuf);
    LCD_NFDispStringAt(GAME_DISP_LEVEL_X,GAME_DISP_LEVEL_Y,  StringLevel); 
    
    tmpbuf[0] = GameLevel%10+'0';
    tmpbuf[1] = 0;
    
    LCD_SetColor(0);
    LCD_FillRect(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, GAME_DISP_LEVEL_X+12+8, GAME_DISP_LEVEL_Y+14+13);
    LCD_SetColor(0xFFFF);
    LCD_NFDispStringAt(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, tmpbuf);
         
    LCD_SetColor(0);
    LCD_FillRect(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, GAME_DISP_STATE_X+48, GAME_DISP_STATE_Y+14);
    LCD_SetColor(0xF800);
    LCD_NFDispStringAt(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, StringPAUSE);     
    LCD_SetColor(color_tmp);  
    LCD_SetTextMode(LCD_DRAWMODE_TRANS);
    BlockDispNext();
    DrawGameArea();
}


/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : BlockInsert()
*  Author:         ZhenFu Fang
*  Description:    方块插入
*  Calls:          
*  Input:          
*  Return:  非　0　时方块移动的空间已经有其它方块。
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          06/03/17      1.0           ORG
*
***********************************************************
*/
GAME_BLOCK_CODE
unsigned int BlockInsert(void)
{
    unsigned int i,j;
    unsigned int BMask;
    unsigned int ChkBit = 0;
    unsigned int BlockTemp;
    unsigned int *pBlockScopeBuf;
    unsigned int *pBlockInsertFlag;
    unsigned int *pBlock;
    
    pBlockInsertFlag = &BlockInsertFlag[MoveBlock_Y];
    pBlock           = &Block[CurBlock][BlockAspect][0];
    for(i=0; i<4; i++){
        BlockTemp = ((*pBlock)<<MoveBlock_X);
        if(BlockTemp & BLOCK_LIMIT_MASK){
            ChkBit = 1;
            break;
        }
        if((BlockTemp+(*pBlockInsertFlag)) != (BlockTemp | (*pBlockInsertFlag))){
            ChkBit = 1;
            break;
        }
        pBlockInsertFlag++;
        pBlock++;
    } 
    if(ChkBit == 0){
        pBlock           = &Block[CurBlock][BlockAspect][0];
        pBlockScopeBuf   = &BlockScopeBuf[MoveBlock_Y][0];
        pBlockInsertFlag = &BlockInsertFlag[MoveBlock_Y];
        
        for(i=0; i<4; i++){
            pBlockScopeBuf = &BlockScopeBuf[MoveBlock_Y+i][0];
            BlockTemp = ((*pBlock)<<MoveBlock_X);
            *pBlockInsertFlag |= BlockTemp;
            BMask = 0x1000;
            for(j=0; j<10; j++){
                if(BlockTemp&BMask){
                    *(pBlockScopeBuf+j) = CurBlock;   
                } 
                BMask >>= 1;
            }
            pBlockInsertFlag++;
            pBlock++;
        }
    } 
    return(ChkBit);  
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : BlockChk()
*  Author:         ZhenFu Fang
*  Description:    方块移动检测
*  Calls:          
*  Input:          
*           mode:   方块方向
*           x,y:    方块在游戏区域的坐标
*  Return:  非　0　时方块移动的空间已经有其它方块。
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          06/03/17      1.0           ORG
*
***********************************************************
*/
GAME_BLOCK_CODE
unsigned int BlockChk(unsigned int mode, unsigned int x, unsigned int y)
{
    unsigned int i,j;
    unsigned int BlockTemp;
    unsigned int BMask=0x1000;
    unsigned int *pBlock;
    unsigned int *pBlockScopeBuf;
    unsigned int *pBlockInsertFlag;
    unsigned int ChkBit=0;
    
    pBlock           = &Block[CurBlock][mode][0];
    pBlockScopeBuf   = &BlockScopeBuf[y][0];
    pBlockInsertFlag = &BlockInsertFlag[y];
    for(i=0; i<4; i++){
        pBlockScopeBuf = &BlockScopeBuf[y+i][0];
        BlockTemp = ((*pBlock)<<x);
        *pBlockInsertFlag &= ~(BlockTemp | BLOCK_LIMIT_MASK);
        BMask = 0x1000;
        for(j=0; j<10; j++){
            if(BlockTemp&BMask){
                *(pBlockScopeBuf+j) = BLOCK_IMAGE_NULL;   
            } 
            BMask >>= 1;
        }
        pBlockInsertFlag++;
        pBlock++;
    } 
     
    pBlockInsertFlag = &BlockInsertFlag[MoveBlock_Y];
    pBlock           = &Block[CurBlock][BlockAspect][0];
    //j                = MoveBlock_Y;
    for(i=0; i<4; i++){
        BlockTemp = ((*pBlock)<<MoveBlock_X);
        if(BlockTemp)
                j = MoveBlock_Y + i;
        if(BlockTemp & BLOCK_LIMIT_MASK){
            ChkBit = 1;
            break;
        }
        if((BlockTemp+(*pBlockInsertFlag)) != (BlockTemp | (*pBlockInsertFlag))){
            ChkBit = 1;
            break;
        }

        pBlockInsertFlag++;
        pBlock++;
    }    
    if(j >= GAME_SCOPE_ROW)
        ChkBit = 1;
        
    if(ChkBit == 0){
        pBlock           = &Block[CurBlock][BlockAspect][0];
        pBlockScopeBuf   = &BlockScopeBuf[MoveBlock_Y][0];
        pBlockInsertFlag = &BlockInsertFlag[MoveBlock_Y];
        
        for(i=0; i<4; i++){
            pBlockScopeBuf = &BlockScopeBuf[MoveBlock_Y+i][0];
            BlockTemp = ((*pBlock)<<MoveBlock_X);
            *pBlockInsertFlag |= BlockTemp;
            BMask = 0x1000;
            for(j=0; j<10; j++){
                if(BlockTemp&BMask){
                    *(pBlockScopeBuf+j) = CurBlock;   
                } 
                BMask >>= 1;
            }
            pBlockInsertFlag++;
            pBlock++;
        }
    }else{
        pBlock           = &Block[CurBlock][mode][0];
        pBlockScopeBuf   = &BlockScopeBuf[y][0];
        pBlockInsertFlag = &BlockInsertFlag[y];
        for(i=0; i<4; i++){
            pBlockScopeBuf = &BlockScopeBuf[y+i][0];
            BlockTemp = ((*pBlock)<<x);
            *pBlockInsertFlag |= BlockTemp;
            BMask = 0x1000;
            for(j=0; j<10; j++){
                if(BlockTemp&BMask){
                    *(pBlockScopeBuf+j) = CurBlock;   
                } 
                BMask >>= 1;
            }
            pBlockInsertFlag++;
            pBlock++;
        } 
    }
    return(ChkBit);
}

/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : BlockRefLine()
*  Author:         ZhenFu Fang
*  Description:    消除一行
*  Calls:          
*  Input:          yy:  方块要消除的所在行
*  Return:  
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          06/03/17      1.0           ORG
*
***********************************************************
*/

GAME_BLOCK_CODE
void BlockRefLine(unsigned int yy)                  
{
    unsigned int i,j;
    unsigned int *pBlockBuf;
    unsigned int *pBlockBuf2;
    unsigned int *pBlockInsertFlag;
    
    pBlockBuf        = &BlockScopeBuf[yy][0];
    pBlockInsertFlag = &BlockInsertFlag[yy];
    *pBlockInsertFlag = 0;
    for(j=0; j<GAME_SCOPE_COL; j++){
        *pBlockBuf++ = BLOCK_IMAGE_NULL;  
    }
    pBlockBuf   = &BlockScopeBuf[yy][0];
    pBlockInsertFlag = &BlockInsertFlag[yy];
    for(i=yy; i>0; i--){
        pBlockBuf  = &BlockScopeBuf[i][0];
        pBlockBuf2 = &BlockScopeBuf[i-1][0];
        *pBlockInsertFlag = *(pBlockInsertFlag-1);
        
        for(j=0; j<GAME_SCOPE_COL; j++){
            *pBlockBuf++ = *pBlockBuf2++;
        }
        pBlockInsertFlag--;
    }
//------------------------------------------------------------------------------
    pBlockBuf = &BlockScopeBuf[0][0];
    pBlockInsertFlag  = &BlockInsertFlag[0];
    *pBlockInsertFlag = 0;
    for(j=0; j<GAME_SCOPE_COL; j++){
        *pBlockBuf++ = BLOCK_IMAGE_NULL;  
    }
//------------------------------------------------------------------------------    
}


/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : ShowScore()
*  Author:         ZhenFu Fang
*  Description:    显示游戏分数
*  Calls:          
*  Input:          Score_tmp:  游戏分数
*  Return:  
*  Remark:         根据需要修改     
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          06/03/17      1.0           ORG
*
***********************************************************
*/
GAME_BLOCK_CODE
void ShowScore(unsigned long Score_tmp)                 //????????
{
    unsigned int color_tmp;
    unsigned int pScore[6];
    if(Score_tmp >= 100000)
        Score_tmp = 99999;
    pScore[0] = Score_tmp/10000+'0';
    Score_tmp = Score_tmp%10000;
    pScore[1] = Score_tmp/1000 +'0';
    Score_tmp = Score_tmp%1000;
    pScore[2] = Score_tmp/100  +'0';
    Score_tmp = Score_tmp%100;
    pScore[3] = Score_tmp/10   +'0';
    Score_tmp = Score_tmp%10   +'0';
    pScore[4] = Score_tmp;
    pScore[5] = 0;
//--------------------------------------------------------------------------
//  显示分数和等级（10000分为一级） 
//--------------------------------------------------------------------------    

    TimerStop(GAME_TMR_CHANNEL);
    color_tmp = LCD_GetColor();
    LCD_SetColor(0);
    LCD_FillRect(GAME_DISP_SCORE_X, GAME_DISP_SCORE_Y+14, GAME_DISP_SCORE_X+32, GAME_DISP_SCORE_Y+14+13);
    LCD_SetColor(0xFFFF);
    LCD_NFDispStringAt(GAME_DISP_SCORE_X, GAME_DISP_SCORE_Y+14, pScore);
    if(GameLevel != (pScore[0]-'0')){
        GameLevel = pScore[0]-'0';
        pScore[0] = GameLevel + '0';
        pScore[1] = 0;
        LCD_SetColor(0);
        LCD_FillRect(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, GAME_DISP_LEVEL_X+12+8, GAME_DISP_LEVEL_Y+14+12);
        LCD_SetColor(0xFFFF);
        LCD_NFDispStringAt(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, pScore);
    }
    LCD_SetColor(color_tmp);
    TimerSetTimer(GAME_TMR_CHANNEL, 40-GameLevel*4);//根据　等级来设定时间
    TimerReset(GAME_TMR_CHANNEL);
    TimerStart(GAME_TMR_CHANNEL);
}


/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : BlockRemove()
*  Author:         ZhenFu Fang
*  Description:    消除方块
*  Calls:          BlockRefLine()
*  Input:          
*  Return:  
*  Remark:           
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          06/03/17      1.0           ORG
*
***********************************************************
*/
GAME_BLOCK_CODE
void BlockRemove(void)
{
    unsigned int i,j;
    unsigned int y_tmp;
    unsigned int RemoveLines=0;
    unsigned int *pBlockInsertFlag;
    
    y_tmp            = MoveBlock_Y;
    pBlockInsertFlag = &BlockInsertFlag[y_tmp]; 
    
    for(i=0; i<4; i++){
        if((*pBlockInsertFlag & ~BLOCK_LIMIT_MASK) == ~BLOCK_LIMIT_MASK){
            BlockRefLine(y_tmp);    
            RemoveLines++;
        }
        if(++y_tmp >= GAME_SCOPE_ROW)
            break;
        pBlockInsertFlag++;
    }
    if(RemoveLines){
        i = (1<<(RemoveLines-1));
        i = i*2;
        GameScore += (unsigned long)i * 100 - 100;
        ShowScore(GameScore); 
    }
}


/*
**********************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : GetNewBlock()
*  Author:         ZhenFu Fang
*  Description:    获取新的方块
*  Calls:          
*  Input:          
*  Return:          非0值时，获取新方块失败（游戏结束） 
*  Remark:          在         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*           FZF          06/03/17      1.0           ORG
*
***********************************************************
*/
GAME_BLOCK_CODE
unsigned int GetNewBlock(void)
{
    CurBlock        = NextBlock;
    BlockAspect     = NextBlockAspect;
    //GameLevel       = GameScore/10000;
    NextBlock       = ((unsigned int)rand())%7;//1;  //  获取0-6之间的随机数
    NextBlockAspect = ((unsigned int)rand())%4;//2;  //  获取0-3之间的随机数

    MoveBlock_Y     = 0;
    MoveBlock_X     = 6;
    BlockDispNext();
    if(GameState == GAME_RUN){
        if(BlockInsert()){
            GameState = GAME_STOP;
            GameFreqDown();
            GameDispInit();
            ClearGameArea();
            DrawGameArea();
            GameValueInit();
            return 1; // GAME OVER
        }
        //BlockInsert(MoveBlock_X,MoveBlock_Y);
        GameKeyFlag = 0;
        //TimerStop(GAME_TMR_CHANNEL2);
        TimerReset(GAME_TMR_CHANNEL2);
        //GameKeyValue = KEY_NULL;
        DrawGameArea();
    }
    return 0;
}

GAME_BLOCK_CODE
void GameSetting(void)
{
    unsigned int color_tmp;
    unsigned int tmpbuf[4];
    
    if(KeyFlag.bc.bKeyProssed == 0)
        return;
    KeyFlag.bc.bKeyProssed = 0;
    
    if (SystemInfo.bc.bHoldOn) {
        return;
    }
        
    switch(KeyVal){
        case GAME_KEY_PLAY:
            if(GameState == GAME_RUN){
                GameState = GAME_PAUSE;
                GameFreqDown();
                TimerStop(GAME_TMR_CHANNEL);
                color_tmp = LCD_GetColor();
                LCD_SetColor(0);
                LCD_FillRect(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, GAME_DISP_STATE_X+48, GAME_DISP_STATE_Y+14);
                LCD_SetColor(0xF800);
                LCD_NFDispStringAt(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, StringPAUSE);  
                LCD_SetColor(color_tmp);
            }else if(GameState == GAME_STOP){
                GameState = GAME_RUN;
                GameFreqUp();
                GameDispInit();
                GetNewBlock();
                color_tmp = LCD_GetColor();
                LCD_SetColor(0);
                LCD_FillRect(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, GAME_DISP_STATE_X+48, GAME_DISP_STATE_Y+14);
                LCD_SetColor(color_tmp);
                TimerSetTimer(GAME_TMR_CHANNEL, 40-GameLevel*4);//根据　等级来设定时间
                TimerStart(GAME_TMR_CHANNEL);
            }else{
                GameState = GAME_RUN;
                GameFreqUp();
                color_tmp = LCD_GetColor();
                LCD_SetColor(0);
                LCD_FillRect(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, GAME_DISP_STATE_X+48, GAME_DISP_STATE_Y+14);
                LCD_SetColor(color_tmp);
                TimerStart(GAME_TMR_CHANNEL);
            }
            break;
            
        case KEY_VAL_MENU_PRESS_START://KEY_VAL_MODE_PRESS_START:
            //if (KeyProcMsgArray[2] == KEY_STATE_DOWN){
            //    DisplayFlag = DISP_MENU;
                if (GameState != GAME_STOP) {
                    GameState = GAME_PAUSE;
                    GameFreqDown();
                    TimerStop(GAME_TMR_CHANNEL);
                    TimerStop(GAME_TMR_CHANNEL2);
                    DownFlag = 0;
                    GameKeyFlag = 0;
                    GameKeyValue = KEY_NULL;
                    //SystemInfo.bit.bBklightOffEn = 1;
                    //PostFun2Message(MSG_FUNCTION2_GAME,GAME_BLOCK_STOP,0,0,0,0,0,0);
                }
            //}
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
            Fun2ModuleChange();
            MainMenuState = 0;
            DisplayFlag = DISP_MENU;
            PreDisplayFlag = DisplayFlag;
            LCD_Init(0);
            LCD_SetDiaplayMode(0);
            break;
/*        case KEY_UP:
            if(KeyProcMsgArray[2] != KEY_STATE_UP)
                break;
            if(GameLevel < 9)
                GameLevel++;
            tmpbuf[0] = GameLevel%10+'0';
            tmpbuf[1] = 0;
            color_tmp = LCD_GetColor();
            LCD_SetColor(0);
            LCD_FillRect(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, GAME_DISP_LEVEL_X+12+8, GAME_DISP_LEVEL_Y+14+13);
            LCD_SetColor(color_tmp);
            LCD_NFDispStringAt(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, tmpbuf);
            break;
        case KEY_DOWN:
            if(KeyProcMsgArray[2] != KEY_STATE_UP)
                break;
            if(GameLevel > 0)
                GameLevel--;
            tmpbuf[0] = GameLevel%10+'0';
            tmpbuf[1] = 0;
            color_tmp = LCD_GetColor();
            LCD_SetColor(0);
            LCD_FillRect(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, GAME_DISP_LEVEL_X+12+8, GAME_DISP_LEVEL_Y+14+13);
            LCD_SetColor(color_tmp);
            LCD_NFDispStringAt(GAME_DISP_LEVEL_X+12, GAME_DISP_LEVEL_Y+14, tmpbuf);
            break;*/
        default:
            break;
    }
    //KeyProcMsgArray[1] = KEY_NULL;  
}    

GAME_BLOCK_CODE
void BlockMove(unsigned int key)
{
    unsigned int temp;
    
    switch(key){
        case GAME_KEY_UP_DOWN:
        case GAME_KEY_UP:
        case GAME_KEY_UP_L:
            temp = BlockAspect;
            if(++BlockAspect >= 4){
                BlockAspect = 0;
            }
            if(BlockChk(temp, MoveBlock_X, MoveBlock_Y)){
                BlockAspect = temp;
            }else{
                DrawGameArea();
            }
            break;
        case GAME_KEY_DOWN_DOWN:
        case GAME_KEY_DOWN:
        case GAME_KEY_DOWN_L:
            temp = MoveBlock_Y;
            MoveBlock_Y++;
            if(BlockChk(BlockAspect, MoveBlock_X, temp)){
                MoveBlock_Y = temp;
                BlockRemove();
                if(GetNewBlock()){
                    //GameState = GAME_PAUSE;
                    //GetNewBlock();
                    TimerStop(GAME_TMR_CHANNEL);
                    break;
                }                
            }
            DrawGameArea();
            break;
        //case GAME_KEY_DROP_OFF_DOWN:
        case GAME_KEY_DROP_OFF:
        //case GAME_KEY_DROP_OFF_L:
            do{
                DrawGameArea();
                temp = MoveBlock_Y;
                MoveBlock_Y++;
            }while(BlockChk(BlockAspect, MoveBlock_X, temp)==0);
                MoveBlock_Y = temp;
                BlockRemove();
                if(GetNewBlock()){
                    //GameState = GAME_RUN;
                    //GetNewBlock();
                    TimerStop(GAME_TMR_CHANNEL);
                    break;
                }
            DrawGameArea();
            break;
        case GAME_KEY_RIGHT_DOWN:
        case GAME_KEY_RIGHT:
        case GAME_KEY_RIGHT_L:
            temp = MoveBlock_X;
            if(MoveBlock_X > 0)
                MoveBlock_X--;
            if(BlockChk(BlockAspect, temp, MoveBlock_Y)){
                MoveBlock_X = temp;
            }else{
                DrawGameArea();
            }
            break;
        case GAME_KEY_LEFT_DOWN:
        case GAME_KEY_LEFT:
        case GAME_KEY_LEFT_L:
            temp = MoveBlock_X;
            if(MoveBlock_X < 16)
                MoveBlock_X++;
            if(BlockChk(BlockAspect, temp, MoveBlock_Y)){
                MoveBlock_X = temp;
            }else{
                DrawGameArea();
            }
            break;
        case GAME_KEY_PLAY_DOWN:
        case GAME_KEY_PLAY:
            if(GameState != GAME_RUN){
                GameState = GAME_RUN;
                GameFreqUp();
                TimerSetTimer(GAME_TMR_CHANNEL, 40-GameLevel*4);//根据　等级来设定时间
                TimerStart(GAME_TMR_CHANNEL);
            }else{
                GameState = GAME_PAUSE;
                GameFreqDown();
                TimerStop(GAME_TMR_CHANNEL);
            }
            break;
        default:
            break;
    }
}


GAME_BLOCK_CODE
void GameProcess(void)
{
    unsigned int imask_tmp;
    unsigned int color_tmp;
    unsigned int KeyValue;
    unsigned int KeyState;   
    
    if(GameState != GAME_RUN){
        if(KeyFlag.bc.bKeyProssed)
            KeyFlag.bc.bKeyProssed = 0;
        return;
        
        if (SystemInfo.bc.bHoldOn) {
            return;
        }
    }
    if(DownFlag != 0){
        read_creg(%imask, imask_tmp);
        bitclear_creg(%imask, 15);
        DownFlag = 0;
        write_creg(%imask, imask_tmp);
        BlockMove(GAME_KEY_DOWN);
    }
    
    if(KeyFlag.bc.bKeyProssed == 0)
        return;
        
    KeyFlag.bc.bKeyProssed = 0;
    
    switch(KeyVal){
        case GAME_KEY_PLAY:
            if(GameState != GAME_RUN){
                GameState = GAME_RUN;
                GameFreqUp();
                color_tmp = LCD_GetColor();
                LCD_SetColor(0);
                LCD_FillRect(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, GAME_DISP_STATE_X+48, GAME_DISP_STATE_Y+14);
                LCD_SetColor(color_tmp);
                TimerSetTimer(GAME_TMR_CHANNEL, 40-GameLevel*4);//根据　等级来设定时间
                TimerStart(GAME_TMR_CHANNEL);
                TimerReset(GAME_TMR_CHANNEL2);
                TimerStart(GAME_TMR_CHANNEL2);
            }else{
                GameState = GAME_PAUSE;
                GameFreqDown();
                TimerStop(GAME_TMR_CHANNEL);
                TimerStop(GAME_TMR_CHANNEL2);
                GameKeyFlag = 0;
                GameKeyValue = KEY_NULL;
                color_tmp = LCD_GetColor();
                LCD_SetColor(0);
                LCD_FillRect(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, GAME_DISP_STATE_X+48, GAME_DISP_STATE_Y+14);
                LCD_SetColor(0xF800);
                LCD_NFDispStringAt(GAME_DISP_STATE_X, GAME_DISP_STATE_Y, StringPAUSE); 
                LCD_SetColor(color_tmp);
            }
            break;
            
        case KEY_VAL_MENU_PRESS_START://KEY_VAL_MODE_PRESS_START:
            //if (KeyProcMsgArray[2] == KEY_STATE_DOWN){
            //    DisplayFlag = DISP_MENU;
                if (GameState != GAME_STOP) {
                    GameState = GAME_PAUSE;
                    GameFreqDown();
                    TimerStop(GAME_TMR_CHANNEL);
                    TimerStop(GAME_TMR_CHANNEL2);
                    DownFlag = 0;
                    GameKeyFlag = 0;
                    GameKeyValue = KEY_NULL;
                    //SystemInfo.bit.bBklightOffEn = 1;
                    //PostFun2Message(MSG_FUNCTION2_GAME,GAME_BLOCK_STOP,0,0,0,0,0,0);
                }
            //}
            Fun2MsgArray[0] = MSG_FUNCTION2_NONE;
            Fun2ModuleChange();
            MainMenuState = 0;
            DisplayFlag = DISP_MENU;
            PreDisplayFlag = DisplayFlag;
            LCD_Init(0);
            LCD_SetDiaplayMode(0);
//            LCD_SetColor(0);
            break;
            
        case GAME_KEY_RIGHT_DOWN:
        case GAME_KEY_LEFT_DOWN:
        case GAME_KEY_DOWN_DOWN:
        case GAME_KEY_UP_DOWN:
        case GAME_KEY_DROP_OFF_DOWN:
            GameKeyValue = KeyVal;
            GameKeyFlag = 0;
            BlockMove(GameKeyValue);
            TimerSetTimer(GAME_TMR_CHANNEL2, 4);//根据　等级来设定时间
            TimerStart(GAME_TMR_CHANNEL2);
            break;
            
        case GAME_KEY_RIGHT:
        case GAME_KEY_LEFT:
        case GAME_KEY_DOWN:
        case GAME_KEY_UP:
        case GAME_KEY_DROP_OFF:
        case GAME_KEY_RIGHT_L_UP:
        case GAME_KEY_LEFT_L_UP:
        case GAME_KEY_DOWN_L_UP:
        case GAME_KEY_UP_L_UP:
                GameKeyFlag = 0;
                TimerStop(GAME_TMR_CHANNEL2);
                TimerReset(GAME_TMR_CHANNEL2);
                GameKeyValue = KEY_NULL;
                if(KeyVal == GAME_KEY_DROP_OFF){
                    BlockMove(GAME_KEY_DROP_OFF);
                }
            break;
        default:
            break;        
    }
    
    if(GameKeyFlag != 0){
        read_creg(%imask, imask_tmp);
        bitclear_creg(%imask, 15);
        GameKeyFlag = 0;
        write_creg(%imask, imask_tmp);
        BlockMove(GameKeyValue);
    }
}



GAME_BLOCK_CODE
void GameKeyProc(unsigned int Key)
{
  
}


GAME_BLOCK_CODE
void GameBlockModule(void)
{
    unsigned int color_tmp;
    
    if (DisplayFlag == DISP_GAME) {
        if (SystemInfo.bc.bBackLightOffStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOffStart = 0;
            LcdBackLightOff();
            bitset_creg(%imask,5);
        }
        
        if ((SystemInfo.bc.bDisplayRecovery) && (SystemInfo.bc.bHoldDisplay == 0)) {
            SystemInfo.bc.bDisplayRecovery = 0;
            //恢复显示图片(重新解码)
            PostFun2Message(MSG_FUNCTION2_GAME,GAME_BLOCK_INIT,0,0,0/*,0,0,0*/);
        }
        
        if (SystemInfo.bc.bBackLightOnStart == 0) {
            
            switch(Fun2MsgArray[1]){        
                case GAME_BLOCK_INIT:
                    //LCD_Init();
                    LCD_SetDiaplayMode(1);
                    DMA_LcdJpegInit();
                    if (Sys1InitFlag.word == 0xffff) {
//                        SysCpuSetFreq(24);
//                        SysTimer0Set(24,25000);
                        SysCpuSetFreq(51);
                        SysTimer0Set(51,25000);
                    }
                    color_tmp = LCD_GetColor();
                    LCD_SetColor(0);
                    LCD_FillRect(0,0, LCD_MAX_YSIZE-1, LCD_MAX_XSIZE-1);
                #ifdef LCD_MEMORY_DEV
                    DmaFillImage2Lcd();
                #endif
                    LCD_SetColor(color_tmp);
                    if (Sys2InitFlag.bc.bGameModuleInitEnable) {
                        GameValueInit();
                    }
                    GameDispInit();
                    PostFun2Message(MSG_FUNCTION2_GAME,GAME_BLOCK_STOP,0,0,0/*,0,0,0*/);
                    
                    break;
                case GAME_BLOCK_STOP:
                    GameSetting();
                    if(GameState == GAME_RUN){
                        PostFun2Message(MSG_FUNCTION2_GAME,GAME_BLOCK_RUN,0,0,0/*,0,0,0*/);
                    }
                    break;
                case GAME_BLOCK_RUN:
                    //SystemInfo.bit.bBklightOffEn = 0;
                    GameProcess();
                    if((GameState != GAME_RUN)&&(MSG_FUNCTION2_GAME == Fun2MsgArray[0])){
                    PostFun2Message(MSG_FUNCTION2_GAME,GAME_BLOCK_STOP,0,0,0/*,0,0,0*/);
                    }
                    break;
                case GAME_BLOCK_END:
                    GameExit();
                default:
                    break;
            }
        }
        
        if (SystemInfo.bc.bHoldDisplay) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bHoldDisplay = 0;
            bitset_creg(%imask,5);
            
            DisplayRecoveryTime = 0;
            PreDisplayFlag = DisplayFlag;
            DisplayFlag = DISP_HOLD;
            
            MusicSetFreq51();
            LCD_Init(0);
            LCD_SetDiaplayMode(0);
            DisplayPicture(0, 0, MAX_PIC_XSIZE, MAX_PIC_YSIZE, HOLD_OFF_PIC + (unsigned long)(SystemInfo.bc.bHoldOn) * MAX_PIC_SIZE);
            MusicSetFreq51Return();
            return;
        }
        
        if (SystemInfo.bc.bBackLightOnStart) {
            bitclear_creg(%imask,5);
            SystemInfo.bc.bBackLightOnStart = 0;
            LcdBackLightOn();
            KeyReset();
            bitset_creg(%imask,5);
        }
    }
}

GAME_BLOCK_CODE
void GameFreqUp(void)
{
    MusicSetFreq51();
    bitclear_creg(%imask,5);
    SystemInfo.bc.bBackLightOffEn = 1;
    SystemInfo.bc.bSysIdleEn = 0;
    bitset_creg(%imask,5);
}

GAME_BLOCK_CODE
void GameFreqDown(void)
{
    bitclear_creg(%imask,5);
    SystemInfo.bc.bBackLightOffEn = 0;
    LightOffCounter = 0;
    if ((Sys1InitFlag.word == 0xffff) || ((Sys1InitFlag.bc.bMusicModuleInitEnable == 0) && (MusicPlayInfo.PlayState != MUSIC_DECODE_PLAY))) {
        SystemInfo.bc.bSysIdleEn = 1;
        SysIdleCounter = 0;
    }
    bitset_creg(%imask,5);
    MusicSetFreq51Return();
}
#endif
