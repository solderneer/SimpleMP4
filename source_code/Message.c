//******************************************************************************
//Message.c          2005-6-21 14:52        copyright  rock-chip  authour zyz
//******************************************************************************
#define     IN_MESSAGE

#include <creg.h>
#include "Macro.h"
#include "Global.h"

/*
void PostFile1Message(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8)
{
    File1MsgArray[0] = Param1;
    File1MsgArray[1] = Param2;
    File1MsgArray[2] = Param3;
    File1MsgArray[3] = Param4;
    File1MsgArray[4] = Param5;
    File1MsgArray[5] = Param6;
    File1MsgArray[6] = Param7;
    File1MsgArray[7] = Param8;
}

void PostFile2Message(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8)
{
    File2MsgArray[0] = Param1;
    File2MsgArray[1] = Param2;
    File2MsgArray[2] = Param3;
    File2MsgArray[3] = Param4;
    File2MsgArray[4] = Param5;
    File2MsgArray[5] = Param6;
    File2MsgArray[6] = Param7;
    File2MsgArray[7] = Param8;
}
*/
/*
//__attribute__((section(".sys_text,\"ax\"")))
void PostKeyProcMessage(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8)
{
    KeyProcMsgArray[0] = Param1;
    KeyProcMsgArray[1] = Param2;
    KeyProcMsgArray[2] = Param3;
    KeyProcMsgArray[3] = Param4;
    KeyProcMsgArray[4] = Param5;
    KeyProcMsgArray[5] = Param6;
    KeyProcMsgArray[6] = Param7;
    KeyProcMsgArray[7] = Param8;
}
*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void PostSche1Message(unsigned int Param1/*,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8*/)
{
    AVI_MP3_DECODE_DISABLE;
    Sche1MsgArray[0] = Param1;
//    Sche1MsgArray[1] = Param2;
//    Sche1MsgArray[2] = Param3;
//    Sche1MsgArray[3] = Param4;
//    Sche1MsgArray[4] = Param5;
//    Sche1MsgArray[5] = Param6;
//    Sche1MsgArray[6] = Param7;
//    Sche1MsgArray[7] = Param8;
    AVI_MP3_DECODE_ENABLE;
}

//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void PostSche2Message(unsigned int Param1/*,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8*/)
{
    AVI_MP3_DECODE_DISABLE;
    Sche2MsgArray[0] = Param1;
//    Sche2MsgArray[1] = Param2;
//    Sche2MsgArray[2] = Param3;
//    Sche2MsgArray[3] = Param4;
//    Sche2MsgArray[4] = Param5;
//    Sche2MsgArray[5] = Param6;
//    Sche2MsgArray[6] = Param7;
//    Sche2MsgArray[7] = Param8;
    AVI_MP3_DECODE_ENABLE;
}

//__attribute__((section(".sys_text,\"ax\"")))
void PostFun1Message(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5/*,unsigned int Param6,unsigned int Param7,unsigned int Param8*/)
{
    AVI_MP3_DECODE_DISABLE;
    Fun1MsgArray[0] = Param1;
    Fun1MsgArray[1] = Param2;
    Fun1MsgArray[2] = Param3;
    Fun1MsgArray[3] = Param4;
    Fun1MsgArray[4] = Param5;
//    Fun1MsgArray[5] = Param6;
//    Fun1MsgArray[6] = Param7;
//    Fun1MsgArray[7] = Param8;
    AVI_MP3_DECODE_ENABLE;
}

//__attribute__((section(".sys_text,\"ax\"")))
void PostFun2Message(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5/*,unsigned int Param6,unsigned int Param7,unsigned int Param8*/)
{
    AVI_MP3_DECODE_DISABLE;
    Fun2MsgArray[0] = Param1;
    Fun2MsgArray[1] = Param2;
    Fun2MsgArray[2] = Param3;
    Fun2MsgArray[3] = Param4;
    Fun2MsgArray[4] = Param5;
//    Fun2MsgArray[5] = Param6;
//    Fun2MsgArray[6] = Param7;
//    Fun2MsgArray[7] = Param8;
    AVI_MP3_DECODE_ENABLE;
}
/*
//__attribute__((section(".sys_text,\"ax\"")))
void PostKeyMessage(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8)
{
    KeyMsgArray[0] = Param1;
    KeyMsgArray[1] = Param2;
    KeyMsgArray[2] = Param3;
    KeyMsgArray[3] = Param4;
    KeyMsgArray[4] = Param5;
    KeyMsgArray[5] = Param6;
    KeyMsgArray[6] = Param7;
    KeyMsgArray[7] = Param8;
}
*/
//__attribute__((section(".sys_text,\"ax\"")))
SYSTEM_TEXT_SECTION
void PostDisplayMessage(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8)
{
    AVI_MP3_DECODE_DISABLE;
    DispMsgArray[0] = Param1;
    DispMsgArray[1] = Param2;
    DispMsgArray[2] = Param3;
    DispMsgArray[3] = Param4;
    DispMsgArray[4] = Param5;
    DispMsgArray[5] = Param6;
    DispMsgArray[6] = Param7;
    DispMsgArray[7] = Param8;
    AVI_MP3_DECODE_ENABLE;
}
/*
//__attribute__((section(".sys_text,\"ax\"")))
void PostUsbDetMessage(unsigned int Param1,unsigned int Param2,unsigned int Param3,unsigned int Param4,unsigned int Param5,unsigned int Param6,unsigned int Param7,unsigned int Param8)
{
    USBDetMsgArray[0] = Param1;
    USBDetMsgArray[1] = Param2;
    USBDetMsgArray[2] = Param3;
    USBDetMsgArray[3] = Param4;
    USBDetMsgArray[4] = Param5;
    USBDetMsgArray[5] = Param6;
    USBDetMsgArray[6] = Param7;
    USBDetMsgArray[7] = Param8;
}
*/
//******************************************************************************
