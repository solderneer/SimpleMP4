//******************************************************************************
//  Global.inc          2005.08.09        copyright  rock-chip  authour zyz
//******************************************************************************
#ifndef  _MSGDEF_H
#define  _MSGDEF_H
//------------------------------------------------------------------------------
#define     MSG_SCHEDULER1_NONE                 0xFFFF
#define     MSG_SCHEDULER1_MP3_DECODE           0x0001
#define     MSG_SCHEDULER1_WMA_DECODE           0x0002
#define     MSG_SCHEDULER1_AVI                  0x0003
#define     MSG_SCHEDULER1_ENCODE               0x0005
#define     MSG_SCHEDULER1_FM                   0x0006
#define     MSG_SCHEDULER1_USB                  0x0007
#define     MSG_SCHEDULER1_UPGRADE              0x0008
#define     MSG_SCHEDULER1_WAV_DECODE           0x0009  //zyf 2005-12-6 9:52
//------------------------------------------------------------------------------
#define     MSG_SCHEDULER2_NONE                 0xFFFF
#define     MSG_SCHEDULER2_JPEG                 0x0001
#define     MSG_SCHEDULER2_TXT                  0x0002
#define     MSG_SCHEDULER2_LRC                  0x0003
#define     MSG_SCHEDULER2_SETMENU              0x0004
#define     MSG_SCHEDULER2_GAME                 0x0005
//------------------------------------------------------------------------------
#define     MSG_FUNCTION1_NONE                  0xFFFF

#define     MSG_FUNCTION1_FILE_FIND             0x0001
#define     MSG_FUNCTION1_FILE_CREAT            0x0002

#define     MSG_FUNCTION1_MUSIC_CHECK           0x0003
#define     MSG_FUNCTION1_MUSIC_MP3             0x0004
#define     MSG_FUNCTION1_MUSIC_WMA             0x0005
#define     MSG_FUNCTION1_MUSIC_WAV             0x0006

#define     MSG_FUNCTION1_AVI_CHECK             0x0008
#define     MSG_FUNCTION1_AVI_DECODE            0x0009

#define     MSG_FUNCTION1_ENCODE_CHECK          0x000E
#define     MSG_FUNCTION1_ENCODE_PROC           0x000F

#define     MSG_FUNCTION1_MIC_INIT              0x0010
#define     MSG_FUNCTION1_MIC_ENCODE            0x0011
#define     MSG_FUNCTION1_LINE_INIT             0x0012
#define     MSG_FUNCTION1_LINE_ENCODE           0x0013

#define     MSG_FUNCTION1_FM                    0x0014

#define     MSG_FUNCTION1_USB_INIT              0x0017
#define     MSG_FUNCTION1_USB_CONNECT           0x0018
#define     MSG_FUNCTION1_USB_TRANS             0x0019

#define     MSG_FUNCTION1_UPGRADE_INIT          0x001A
#define     MSG_FUNCTION1_UPGRADE_CONNECT       0x001B
#define     MSG_FUNCTION1_UPGRADE_TRANS         0x001C
//------------------------------------------------------------------------------
#define     MSG_FUNCTION2_NONE                  0xFFFF

#define     MSG_FUNCTION2_FILE_FIND             0x0001

#define     MSG_FUNCTION2_LRC_CHECK             0x0002
#define     MSG_FUNCTION2_LRC                   0x0003

#define     MSG_FUNCTION2_JPEG_CHECK            0x0004
#define     MSG_FUNCTION2_JPEG_DECODE           0x0005

#define     MSG_FUNCTION2_BROWSER               0x0006
#define     MSG_FUNCTION2_MENU                  0x0007
#define     MSG_FUNCTION2_TXT                   0x0008
#define     MSG_FUNCTION2_ENCODE                0x0009
#define     MSG_FUNCTION2_GAME                  0x000A

//--------------------------------------------------------------------------------
//      MENU 定义
//--------------------------------------------------------------------------------
#define     MENU_MUSIC_INTERFACE_ID             0x0001
#define     MENU_MOVIE_INTERFACE_ID             0x0002
#define     MENU_RADIO_INTERFACE_ID             0x0003
#define     MENU_PICTURE_INTERFACE_ID           0x0004
#define     MENU_TEXT_INTERFACE_ID              0x0005
#define     MENU_RECORD_INTERFACE_ID            0x0006
#define     MENU_BROWSER_INTERFACE_ID           0x0007
#define     MENU_GAME_INTERFACE_ID              0x0008
#define     MENU_SETTING_INTERFACE_ID           0x0009
//------------------------------------------------------------------------------
#define     MSG_KEY_NONE                        0x0000
#define     MSG_KEY_RUN                         0x0001

//------------------------------------------------------------------------------
//Key Proc Module Message Define
#define     MSG_KEY_PROC_NONE                   0xFFFF

#define     MSG_KEY_PROC_MUSIC                  MENU_MUSIC_INTERFACE_ID
#define     MSG_KEY_PROC_AVI                    MENU_MOVIE_INTERFACE_ID /*MENU_MOVIE_INTERFACE_ID*/
#define     MSG_KEY_PROC_ENC                    MENU_RECORD_INTERFACE_ID
#define     MSG_KEY_PROC_FM                     MENU_RADIO_INTERFACE_ID
                                                
#define     MSG_KEY_PROC_BROWSER                MENU_BROWSER_INTERFACE_ID
#define     MSG_KEY_PROC_TEXT                   MENU_TEXT_INTERFACE_ID
#define     MSG_KEY_PROC_JPEG                   MENU_PICTURE_INTERFACE_ID
#define     MSG_KEY_PROC_MOVIE                  MENU_MOVIE_INTERFACE_ID
#define     MSG_KEY_PROC_GAME                   MENU_GAME_INTERFACE_ID
#define     MSG_KEY_PROC_SET_MENU               MENU_SETTING_INTERFACE_ID
//------------------------------------------------------------------------------
#define     MSG_DISP_NONE                       0x0000//0xFFFF
                                                
#define     MSG_DISP_INTERFACE                  0x0001
                                                
#define     MSG_DISP_MUSIC_PLAY                 0x0002
#define     MSG_DISP_MP4                        0x0003
#define     MSG_DISP_FM                         0x0004
#define     MSG_DISP_PIC                        0x0005
#define     MSG_DISP_TXT                        0x0006
#define     MSG_DISP_RECORD                     0x0007
#define     MSG_DISP_BROWSER                    0x0008
#define     MSG_DISP_GAME                       0x0009
#define     MSG_DISP_SET                        0x000A
                                                
#define     MSG_DISP_MUSIC_LRC                  0x000B
#define     MSG_DISP_PICTURE                    0x000C
#define     MSG_DISP_UNICODE                    0x000D
#define     MSG_DISP_STRING                     0x000E
#define     MSG_DISP_TIME                       0x000F
#define     MSG_DISP_NUMBER                     0x0010
                                                
#define     MSG_DISP_MUSIC_FREQ                 0x0011

//--------------------------------------------------------------------------
#define     MSG_DISP_MUSIC_PLAY_INIT            0x0000
#define     MSG_DISP_MUSIC_PLAY_LOCK            0x0201
#define     MSG_DISP_MUCIC_PLAY_VOL             0x0202
#define     MSG_DISP_MUSIC_PLAY_VOLNUM          0x0203
#define     MSG_DISP_MUSIC_PLAY_GRADATION       0x0204
#define     MSG_DISP_MUSIC_PLAY_STATE           0x0205
#define     MSG_DISP_MUSIC_PLAY_CYCLE           0x0206
#define     MSG_DISP_MUSIC_PLAY_BATT            0x0207
#define     MSG_DISP_MUSIC_PLAY_FREQ            0x0208
#define     MSG_DISP_MUSIC_PLAY_BITRATE         0x0209
#define     MSG_DISP_MUSIC_PLAY_FREQ_T          0x020A
#define     MSG_DISP_MUSIC_PLAY_FILENAME        0x020B
#define     MSG_DISP_MUSIC_PLAY_EQ              0x020C
#define     MSG_DISP_MUSIC_PLAY_FILE            0x020D
#define     MSG_DISP_MUSIC_PLAY_A_B             0x020E
#define     MSG_DISP_MUSIC_PLAY_JDT             0x020F
#define     MSG_DISP_MUSIC_PLAY_CURTMR          0x0210
//--------------------------------------------------------------------------
#define     MSG_USB_DET_NONE                    0x0000
#define     MSG_USB_DET_PLUG                    0x0001
#define     MSG_UPGRADE_DET_PLUG                0x0002
#define     MSG_USB_DET_EJECT                   0x0003
//******************************************************************************
//------------------------------------------------------------------------------
//File Operation Message Define
//AVFileMsgArray[1]
#define     MSG_FIND_MUSIC_FILE                 0
#define     MSG_FIND_AVI_FILE                   1

#define     MSG_FIND_JPEG_FILE                  0
#define     MSG_FIND_TEXT_FILE                  1
#define     MSG_FIND_LRC_FILE                   2

//AVFileMsgArray[2]
#define     MSG_FIND_FILE_CURRENT               0
#define     MSG_FIND_FILE_NEXT                  1
#define     MSG_FIND_FILE_PREV                  2

//AVFileMsgArray[3]
#define     MSG_FIND_FILE_ONE                   0
#define     MSG_FIND_FILE_DIR                   1
#define     MSG_FIND_FILE_ALL                   2

//File1MsgArray[4] Message Define
#define     MSG_FIND_FILE_ORDER                 0
#define     MSG_FIND_FILE_RANDOM                1
//------------------------------------------------------------------------------
//MSG_FUNCTION1_MP3_DECODE Parameters Fun1MsgArray[1] Define
#define     MSG_FUNCTION1_MP3_NONE              0xffff
#define     MSG_FUNCTION1_MP3_INIT              0x0001
#define     MSG_FUNCTION1_MP3_HEAD              0x0002
#define     MSG_FUNCTION1_MP3_START             0x0003
#define     MSG_FUNCTION1_MP3_PLAY              0x0004
#define     MSG_FUNCTION1_MP3_END               0x0005
//Fun1MsgArray[2] Define
#define     MSG_FUNCTION1_MP3_DISPLAY_INIT      0x0001
//------------------------------------------------------------------------------
//MSG_FUNCTION1_MP3_DECODE Parameters Fun1MsgArray[1] Define
#define     MSG_FUNCTION1_WMA_NONE              0xffff
#define     MSG_FUNCTION1_WMA_INIT              0x0001
#define     MSG_FUNCTION1_WMA_HEAD              0x0002
#define     MSG_FUNCTION1_WMA_START             0x0003
#define     MSG_FUNCTION1_WMA_PLAY              0x0004
#define     MSG_FUNCTION1_WMA_END               0x0005
//Fun1MsgArray[2] Define
#define     MSG_FUNCTION1_WMA_DISPLAY_INIT      0x0001
//------------------------------------------------------------------------------
//MSG_FUNCTION1_MP3_DECODE Parameters Fun1MsgArray[1] Define
#define     MSG_FUNCTION1_WAV_NONE              0xffff
#define     MSG_FUNCTION1_WAV_INIT              0x0001
#define     MSG_FUNCTION1_WAV_HEAD              0x0002
#define     MSG_FUNCTION1_WAV_START             0x0003
#define     MSG_FUNCTION1_WAV_PLAY              0x0004
#define     MSG_FUNCTION1_WAV_END               0x0005
//Fun1MsgArray[2] Define
#define     MSG_FUNCTION1_WAV_DISPLAY_INIT      0x0001
//------------------------------------------------------------------------------
//MSG_FUNCTION1_AVI_DECODE Parameters Fun1MsgArray[1] Define
#define     MSG_FUNCTION1_AVI_NONE              0xffff
#define     MSG_FUNCTION1_AVI_DECODE_INIT       0x0001
#define     MSG_FUNCTION1_AVI_DECODE_HEAD       0x0002
#define     MSG_FUNCTION1_AVI_DECODE_START      0x0003
#define     MSG_FUNCTION1_AVI_DECODE_PLAY       0x0004
#define     MSG_FUNCTION1_AVI_DECODE_END        0x0005

//------------------------------------------------------------------------------
//MSG_FUNCTION1_ENCODE_PROC Parameters Fun1MsgArray[1] Define
#define     MSG_FUNCTION1_ENC_NONE              0xffff
#define     MSG_FUNCTION1_ENC_SELINIT           0x0001
#define     MSG_FUNCTION1_ENC_SEL               0x0002
#define     MSG_FUNCTION1_ENC_INIT              0x0003
#define     MSG_FUNCTION1_ENC_HEAD              0x0004
#define     MSG_FUNCTION1_ENC_FRAG              0x0005
#define     MSG_FUNCTION1_ENC_START             0x0006
#define     MSG_FUNCTION1_ENC_PLAY              0x0007
#define     MSG_FUNCTION1_ENC_END               0x0008

//------------------------------------------------------------------------------
//FM Parameters Fun2MsgArray[1] Define
#define     MSG_FUNCTION1_FM_NONE               0xffff

#define     MSG_FUNCTION1_FM_INIT               0x0001
#define     MSG_FUNCTION1_FM_START              0x0002
#define     MSG_FUNCTION1_FM_PLAY               0x0003
#define     MSG_FUNCTION1_FM_MENU               0x0004
#define     MSG_FUNCTION1_FM_DEL                0x0005
#define     MSG_FUNCTION1_FM_SAVE               0x0006
#define     MSG_FUNCTION1_FM_DIALOG             0x0007

#define     MSG_FUNCTION1_FM_SEND_FREQHSI       0x0001
#define     MSG_FUNCTION1_FM_SEND_FREQLSI       0x0002
#define     MSG_FUNCTION1_FM_READ_IF            0x0003
#define     MSG_FUNCTION1_FM_MUTE_ON            0x0004
#define     MSG_FUNCTION1_FM_MUTE_OFF           0x0005
#define     MSG_FUNCTION1_FM_READ_ST            0x0006
//------------------------------------------------------------------------------
//Browser Parameters Fun2MsgArray[1] Define
#define     BROWSER_FUNC_NONE                   0xffff
#define     BROWSER_FUNC_INIT                   1
#define     BROWSER_FUNC_PROC                   2
#define     BROWSER_FUNC_RETURN                 3
#define     BROWSER_FUNC_DEL_FILE               4
#define     BROWSER_FUNC_MEM_SELECT             5

//------------------------------------------------------------------------------
//Menu Parameters Fun2MsgArray[1] Define
#define     MSG_FUN2_MENU_NONE                  0xffff
#define     MSG_FUN2_MENU_INIT                  1
#define     MSG_FUN2_MENU_PROC                  2
#define     MSG_FUN2_MENU_END                   3

//------------------------------------------------------------------------------
//Lrc Parameters Fun2MsgArray[1] Define
#define     MSG_FUNCTION2_LRC_NONE              0xffff
#define     MSG_FUNCTION2_LRC_INIT              0x0001
#define     MSG_FUNCTION2_LRC_PLAY              0x0002
#define     MSG_FUNCTION2_LRC_END               0x0003

#define     MSG_DISP_ID3_INIT                   0x0001
#define     MSG_DISP_LRC                        0x0002
//------------------------------------------------------------------------------
//Lrc Parameters Fun2MsgArray[1] Define
#define     MSG_FUNCTION2_JPEG_NONE             0xffff
#define     MSG_FUNCTION2_JPEG_DECODE_INIT      0x0001
#define     MSG_FUNCTION2_JPEG_DECODE_HEAD      0x0002
#define     MSG_FUNCTION2_JPEG_DECODE_PLAY      0x0003
#define     MSG_FUNCTION2_JPEG_AUTO_PLAY        0x0004
#define     MSG_FUNCTION2_JPEG_DECODE_END       0x0005

//------------------------------------------------------------------------------
//Lrc Parameters Fun2MsgArray[1] Define
#define     MSG_FUNCTION2_TXT_NONE              0xffff
#define     MSG_FUNCTION2_TXT_INIT              0x0001
#define     MSG_FUNCTION2_TXT_HEAD              0x0002
#define     MSG_FUNCTION2_TXT_PLAY              0x0003
#define     MSG_FUNCTION2_TXT_END               0x0004
#define     MSG_FUNCTION2_BOOKMASK_SELECT       0x0005
//------------------------------------------------------------------------------
//Encode Parameters Fun2MsgArray[1] Define
#define     MSG_FUN2_ENCODE_NONE                0xffff
#define     MSG_FUN2_ENCODE_INIT                1
#define     MSG_FUN2_ENCODE_SELECT              2
#define     MSG_FUN2_ENCODE_END                 3

//------------------------------------------------------------------------------
////Game Parameters Fun2MsgArray[1] Define
#define     GAME_BLOCK_NONE                     0xffff
#define GAME_BLOCK_INIT                         1
#define GAME_BLOCK_STOP                         2
#define GAME_BLOCK_PAUSE                        3
#define GAME_BLOCK_RUN                          4
#define GAME_BLOCK_END                          5
//------------------------------------------------------------------------------
//MSG_KEY_RUN Parameters Struct Define
//  KeyMsgArray[0]:       
//  KeyMsgArray[1]:       KeyInterFace 
//
//------------------------------------------------------------------------------

#define     MSG_KEY_NO_PRM                      0x0000
#define     MSG_KEY_MUSIC_PLAY                  0x0001
#define     MSG_KEY_MP4_PLAY                    0x0002
#define     MSG_KEY_FM                          0x0003
#define     MSG_KEY_PIC                         0x0004
#define     MSG_KEY_TXT                         0x0005
#define     MSG_KEY_RECORD                      0x0006
#define     MSG_KEY_BROWSER                     0x0007
#define     MSG_KEY_GAME                        0x0008
#define     MSG_KEY_SETTING                     0x0009

//------------------------------------------------------------------------------
//  显示消息定义
//  DispMsgArray[0]:       界面消息
//  DispMsgArray[1]:       对于有下一级菜单的界面项，此参数做为其相对主界面
//                          下的子消息类型，其消息参数基本一样，具体参考下列
//                          子消息注释。 
//  DispMsgArray[2~7]:      做为消息参数
//------------------------------------------------------------------------------

#define     MSG_DISP_RECORD_INIT        0x0700
#define     MSG_DISP_RECORD_SELECT      0x0701

//------------------------------------------------------------------------------
//  进入文本显示的消息定义
#define     MSG_DISP_TXT_INIT           0x0001
#define     MSG_DISP_TXT_TEXT           0x0002

//------------------------------------------------------------------------------
//  进入设置菜单初始化消息
//  DispMsgArray[2]参数：进入设置菜单或退回时，所在菜单项id，值为：0~6
//------------------------------------------------------------------------------    
#define     BROWSER_DISP_INIT           1
#define     BROWSER_DISP_INIT1          2
#define     BROWSER_DISP_INIT2          3
#define     BROWSER_DISP_SELE           4

//------------------------------------------------------------------------------
//  进入设置菜单选择消息
//  DispMsgArray[2]参数：上一次选中菜单项id，值为：0~6
//  DispMsgArray[3]参数：当前选中菜单项id，　值为：0~6
//------------------------------------------------------------------------------
#define     MSG_DISP_SET_INIT           0x0A00          
#define     MSG_DISP_SET_SELECT         0x0A01
#define     MSG_DISP_SET_INIT2          0x0A02
#define     MSG_DISP_SET_SELECT2        0x0A03

//------------------------------------------------------------------------------
//  进入设置子菜单初始化消息
//  DispMsgArray[2]参数：子菜单项id，值为：1~7
//  DispMsgArray[3]参数：进入设置菜单或退回时，所在菜单项id，值为：根据各自菜单
//                       下的选项而定
//------------------------------------------------------------------------------
//#define       MSG_DISP_SET_SUB_INIT       0x0A02      

//------------------------------------------------------------------------------
//  进入设置子菜单选择消息
//  DispMsgArray[2]参数：子菜单项id，值为：1~7
//  DispMsgArray[3]参数：上一次选中菜单项id，值为：根据各自菜单下的选项而定
//  DispMsgArray[4]参数：当前选中菜单项id，　值为：根据各自菜单下的选项而定
//------------------------------------------------------------------------------
#define     MSG_DISP_SET_SUB_SELECT     0x0A03      

#define     MSG_DISP_SETTING_INIT       0x0A04      
#define     MSG_DISP_SETTING_SELECT     0x0A05      

#endif
//******************************************************************************
