========================================================================
关于 LCD 屏降低功耗说明
LCD 屏驱动需要完成
    void Standby_lcd();
    void standby_return();
两个函数功能。
请根据屏的相关资料编写相应的功能。

目前 SDK 软件包里有三个 LCD 屏驱动其中只有 LCD_SSD1289_QVGA.c 相对应的屏
有完成该功能！
注：不同的屏可能功耗不一样。
========================================================================

========2006-10-24========
1. 增加RKUSBFS_1.36.A库
2. 修改黑屏时音乐播放结束返回资源管理器出现绿屏！
   BrowserSchedule.c
if (SystemInfo.bc.bDisplayRecovery) {
            SystemInfo.bc.bDisplayRecovery = 0;
            //恢复显示图片(重新解码)
//            PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum,  DirTreeInfo.KeyCounter, 0, 0, 0);
        }
3. 修改文本内容少于一屏时，保存书签再提书签显示出错！
   TxtSchedule.c 中的 TxtBookMaskSelect()
            if(Text.NFBookMark[Text.BookMarkCont] != -1){
                Text.StartFileOffset = Text.NFBookMark[Text.BookMarkCont];
                Text.EndFileOffset = Text.StartFileOffset;
                for(i=0; i<TXT_DISP_LINE_MAX; i++) 
                {
                    Text.pTxtBuf->BytesPerLine = 0;
                    TxtKeyDownProc();
                }
//------------------------------------------------------------------------------
//  2006-10-27 修改：当文本内容小于一屏时读取书签后显示出错!
//------------------------------------------------------------------------------                
                Text.pTxtBuf         = TxtBuf;
//------------------------------------------------------------------------------ 
            }

