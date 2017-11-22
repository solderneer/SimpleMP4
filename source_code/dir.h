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
//1可配置参数
#define	MAX_DIR_DEPTH		4				//目录深度4级


//1结构定义
typedef struct
{
	uint16 TotalFile;
	uint16 TotalSubDir;
	uint16 CurDirNum;
	uint8 DirName[11];						//短目录名占用11个字符
	uint8 LongDirName[MAX_FILENAME_LEN];		//长目录名
}_subdir;




//1全局变量
#undef	EXT
#ifdef	IN_DIR
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint8   	CurDirDeep					DRAM_FAT;	//当前目录深度
EXT		_subdir	SubDirInfo[MAX_DIR_DEPTH]	DRAM_FAT;	//子目录信息列表

//1函数原型声明
extern	uint16 	BuildDirInfo(uint8* ExtName);
extern	void 	GotoNextDir(uint8* ExtName);
extern	void 	GotoPrevDir(uint8* ExtName);
extern	void 	GotoLastDir(uint8* ExtName);
extern	void 	GotoRootDir(uint8* ExtName);
extern	uint32 	ChangeDir(uint16 SubDirIndex);
extern	uint8 	MakeDir(uint8 *Path, uint8 *DirFileName);
extern	uint16 	GetTotalSubDir(uint8 *Path);
extern	uint32 	GetDirClusIndex(uint8 *Path);


//1表格定义
#ifdef IN_DIR
#else
#endif
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

