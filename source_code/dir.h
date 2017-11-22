/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	dir.h
Author:		XUESHAN LIN
Created:		20th Jan 2005
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/

#ifndef _DIR_H
#define _DIR_H
//1�����ò���
#define	MAX_DIR_DEPTH		4				//Ŀ¼���4��


//1�ṹ����
typedef struct
{
	uint16 TotalFile;
	uint16 TotalSubDir;
	uint16 CurDirNum;
	uint8 DirName[11];						//��Ŀ¼��ռ��11���ַ�
	uint8 LongDirName[MAX_FILENAME_LEN];		//��Ŀ¼��
}_subdir;




//1ȫ�ֱ���
#undef	EXT
#ifdef	IN_DIR
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint8   	CurDirDeep					DRAM_FAT;	//��ǰĿ¼���
EXT		_subdir	SubDirInfo[MAX_DIR_DEPTH]	DRAM_FAT;	//��Ŀ¼��Ϣ�б�

//1����ԭ������
extern	uint16 	BuildDirInfo(uint8* ExtName);
extern	void 	GotoNextDir(uint8* ExtName);
extern	void 	GotoPrevDir(uint8* ExtName);
extern	void 	GotoLastDir(uint8* ExtName);
extern	void 	GotoRootDir(uint8* ExtName);
extern	uint32 	ChangeDir(uint16 SubDirIndex);
extern	uint8 	MakeDir(uint8 *Path, uint8 *DirFileName);
extern	uint16 	GetTotalSubDir(uint8 *Path);
extern	uint32 	GetDirClusIndex(uint8 *Path);


//1�����
#ifdef IN_DIR
#else
#endif
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

