========================================================================
���� LCD �����͹���˵��
LCD ��������Ҫ���
    void Standby_lcd();
    void standby_return();
�����������ܡ�
���������������ϱ�д��Ӧ�Ĺ��ܡ�

Ŀǰ SDK ������������� LCD ����������ֻ�� LCD_SSD1289_QVGA.c ���Ӧ����
����ɸù��ܣ�
ע����ͬ�������ܹ��Ĳ�һ����
========================================================================

========2006-10-24========
1. ����RKUSBFS_1.36.A��
2. �޸ĺ���ʱ���ֲ��Ž���������Դ����������������
   BrowserSchedule.c
if (SystemInfo.bc.bDisplayRecovery) {
            SystemInfo.bc.bDisplayRecovery = 0;
            //�ָ���ʾͼƬ(���½���)
//            PostFun2Message(MSG_FUNCTION2_BROWSER,BROWSER_FUNC_RETURN,0,0,0/*,0,0,0*/);
            PostDisplayMessage(MSG_DISP_BROWSER, BROWSER_DISP_INIT, (unsigned int)(&DirTreeInfo), DirTreeInfo.DirNum + DirTreeInfo.FileNum,  DirTreeInfo.KeyCounter, 0, 0, 0);
        }
3. �޸��ı���������һ��ʱ��������ǩ������ǩ��ʾ����
   TxtSchedule.c �е� TxtBookMaskSelect()
            if(Text.NFBookMark[Text.BookMarkCont] != -1){
                Text.StartFileOffset = Text.NFBookMark[Text.BookMarkCont];
                Text.EndFileOffset = Text.StartFileOffset;
                for(i=0; i<TXT_DISP_LINE_MAX; i++) 
                {
                    Text.pTxtBuf->BytesPerLine = 0;
                    TxtKeyDownProc();
                }
//------------------------------------------------------------------------------
//  2006-10-27 �޸ģ����ı�����С��һ��ʱ��ȡ��ǩ����ʾ����!
//------------------------------------------------------------------------------                
                Text.pTxtBuf         = TxtBuf;
//------------------------------------------------------------------------------ 
            }

