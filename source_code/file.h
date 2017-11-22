/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	file.h
Author:		XUESHAN LIN
Created:		20th Jan 2005
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/
#ifndef _FILE_H
#define _FILE_H

//1可配置参数
#define 	MAX_OPEN_FILES		4		/*可以同时打开的文件数目*/
#define 	MAX_LFN_ENTRIES   		3		/*最长就3*13个字符*/


//1常量定义
/* 函数返回值 */
#define	RETURN_OK                   	0x00    	/* 操作成功*/
#define	NOT_FIND_DISK               	0x01    	/* 逻辑盘不存在*/
#define	DISK_FULL                   	0x02    	/* 逻辑盘满*/
#define	SECTOR_NOT_IN_CACHE   0x03    	/* 扇区没有被cache  */
#define	NOT_EMPTY_CACHE          	0x04    	/* 没有空闲cache*/
#define	SECTOR_READ_ERR           	0x05    	/* 读扇区错误*/
#define	CLUSTER_NOT_IN_DISK    0x06    	/* 逻辑盘中没有此簇  */
#define 	NOT_FIND_FDT                	0x07    	/* 没有发现文件(目录)*/
#define 	NOT_FAT_DISK                	0x08    	/* 非FAT文件系统*/
#define 	FDT_OVER                    	0x09    	/* FDT索引超出范围*/
#define 	FDT_EXISTS                  	0x0a    	/* 文件(目录)已经存在*/
#define 	ROOT_FDT_FULL               	0x0b    	/* 根目录满*/
#define 	DIR_EMPTY                   	0x0C    	/* 目录空*/
#define 	DIR_NOT_EMPTY               	0x0d    	/* 目录不空*/
#define 	PATH_NOT_FIND               0x0e    	/* 路径未找到*/
#define 	FAT_ERR                     	0x0f    	/* FAT表错误*/
#define 	FILE_NAME_ERR               	0x10    	/* 文件(目录)名错误*/
#define 	FILE_EOF                    	0x11    	/* 文件结束*/
#define 	FILE_LOCK                   	0x12    	/* 文件被锁定*/
#define 	NOT_FIND_FILE               	0x13    	/* 没有发现指定文件*/
#define 	NOT_FIND_DIR                	0x14    	/* 没有发现指定目录*/
#define 	NOT_RUN                     	0xfd    	/* 命令未执行*/
#define 	BAD_COMMAND                 	0xfe    	/* 错误命令*/
#define 	PARAMETER_ERR               0xff    	/* 非法参数*/

#define 	NOT_OPEN_FILE			-1		//不能打开文件,文件句柄满



#ifndef 	NULL
#define	NULL	0
#endif

/* FAT类型 */
#ifndef 	FAT12
#define	FAT12	0
#endif
#ifndef 	FAT16
#define	FAT16	1
#endif
#ifndef 	FAT32
#define	FAT32	2
#endif

/* FDT文件属性 */
#define 	ATTR_READ_ONLY             0x01
#define 	ATTR_HIDDEN                 	0x02
#define 	ATTR_SYSTEM                 	0x04
#define 	ATTR_VOLUME_ID             	0x08
#define 	ATTR_LFN_ENTRY    		0x0F      /* LFN entry attribute */
#define 	ATTR_DIRECTORY             	0x10
#define 	ATTR_ARCHIVE                	0x20

/* 簇类型 */
#define 	EMPTY_CLUS               	0
#define 	EMPTY_CLUS_1           	1
#define 	BAD_CLUS                   	0x0ffffff7L
#define 	EOF_CLUS_1                  	0x0ffffff8L
#define 	EOF_CLUS_END             	0x0fffffffL

/* FDT类型 */
#define 	FILE_NOT_EXIST			0
#define 	FILE_DELETED			0xe5
#define 	ESC_FDT                    		0x05

/* 长文件名*/
#define 	MAX_FILENAME_LEN  	((MAX_LFN_ENTRIES * 13 * 2 + 2)/2)		//结尾的NUL占用2B
#define 	LFN_SEQ_MASK			0x3f

/* 文件打开方式 */
#define 	FILE_FLAGS_READ          	1 << 0		//可读
#define 	FILE_FLAGS_WRITE        	1 << 1		//可写

/* 文件指针调整方式 */
#define 	SEEK_SET    				0           		//从文件开始处移动文件指针
#define 	SEEK_CUR    				1           		//从文件当前位置移动文件指针
#define 	SEEK_END    				2           		//从文件尾移动文件指针
#define 	SEEK_REF    				3           		//从文件参考点开始


//1结构定义
//目录表数据结构

typedef 		int8		HANDLE;				//文件句柄数据类型定义

//查找结构体
typedef struct _FIND_DATA
{
	uint32	Index;
	uint32	Clus;     			//当前簇号
} FIND_DATA;

//目录项连接结构体
typedef struct _FDT_DATA
{
	uint32	DirClus;			//目录首簇
	uint32	CurClus;     		//当前簇号
	uint16	Cnt;				//计数簇链
} FDT_DATA;

typedef struct _FDT
{
	uint8	Name[11];		//短文件名主文件名
	uint8	Attr;           		//文件属性
	uint8	NTRes;              	//保留给NT
	uint8	CrtTimeTenth;  	//建立时间（fat16保留）

	uint16	CrtTime;           	//建立时间（fat16保留）
//	uint16	reserve1;           	//保留

	uint16	CrtDate;            	//建立日期（fat16保留）
//	uint16	reserve2;           	//保留

	uint16	LstAccDate;      	//最后访问日期（fat16保留）
//	uint16	reserve3;           	//保留

	uint16	FstClusHI;         	//起始簇号高两个字节（fat16保留）
//	uint16	reserve4;           	//保留

	uint16	WrtTime;           	//最后写时间
//	uint16	reserve5;           	//保留

	uint16	WrtDate;            	//最后写日期
//	uint16	reserve6;           	//保留

	uint16	FstClusLO;          	//起始簇(cluster)号低两个字节
//	uint16	reserve7;           	//保留

	uint32	FileSize;         	//文件大小
//	uint16	reserve8;           	//保留
//	uint16	reserve9;           	//保留
} FDT;

//文件信息结构体
typedef struct _FILE
{
	uint8	Flags;          		//一些标志
	uint8	Name[11];    		//文件名
	uint32	DirClus;	        	//所在目录开始簇号
	uint32	FileSize;		//文件大小
	uint32	FstClus;			//起始簇号
	uint32	Clus;     			//当前簇号
	uint32	Offset;			//文件指针偏移量
	uint32	RefClus;    		//当前簇号
	uint32	RefOffset;		//文件指针偏移量
} MY_FILE;


typedef struct _recovery_info
{
	uint8 Media;
	uint8 Flag;
	uint8 FileName[11];
	uint8 FstClus0;
	uint8 FstClus1;
	uint8 FstClus2;
	uint8 FstClus3;
	uint8 DirClus0;
	uint8 DirClus1;
	uint8 DirClus2;
	uint8 DirClus3;
} RCV_INFO, *PRCV_INFO;



//1全局变量
#undef	EXT
#ifdef	IN_FILE
		#define	EXT
#else
		#define	EXT		extern
#endif		
		
EXT		uint32 		CurDirClus	DRAM_FAT;						//当前目录簇
EXT		MY_FILE 		FileInfo[MAX_OPEN_FILES]	DRAM_FAT;			//同时打开文件信息表
EXT		uint8 		LongFileName[MAX_FILENAME_LEN]	DRAM_FAT;	//长文件名
EXT     RCV_INFO    EncRcvInfo DRAM_FAT;
#undef	EXT
#ifdef	IN_nFAT
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint8 		FatBuf[512]	DRAM_FAT;		//FAT表CACHE
EXT		uint8 		FdtBuf[512]	DRAM_FAT;		//FDT表CACHE
EXT		uint32		FatCacheSec	DRAM_FAT;		//当前的Cache FAT表扇区号
EXT		uint32		Fat2CacheSec		DRAM_ENCODE;	//当前的Cache FAT2表扇区号
EXT		uint8 		Fat2Buf[512]		DRAM_ENCODE;	//FAT2表CACHE
EXT		uint16		Fat2WriteLen		DRAM_ENCODE;	//FAT2 长度计数
EXT		uint8 		Fat2WriteBuf[2048]	DRAM_ENCODE;	//FAT2K BUF
EXT		uint16		FileWriteLen		DRAM_ENCODE;	//file write 长度计数
EXT		uint8 		FileWriteBuf[2048]	DRAM_ENCODE;	//file write BUF
EXT		uint32		FileWriteResidue	DRAM_ENCODE;	//file write 长度计数
EXT		uint32		FdtCacheSec	DRAM_FAT;		//当前的Cache FDT表扇区号
EXT		uint8 		Progress		DRAM_FAT;
EXT		uint32 		ScanStartClus	DRAM_FAT;
EXT		uint32 		ScanEndClus		DRAM_FAT;
EXT		uint32 		ScanLBA			DRAM_FAT;
EXT		uint32 		Copy1Src		DRAM_FAT;
EXT		uint32 		Copy2Src		DRAM_FAT;
EXT		uint32 		Copy1Dest		DRAM_FAT;
EXT		uint32 		Copy2Dest		DRAM_FAT;
EXT		uint16 		Copy1Len		DRAM_FAT;
EXT		uint16 		Copy2Len		DRAM_FAT;
EXT		uint32 		TotalEmptySectors	DRAM_FAT;
EXT		uint16 		ScanEraseSectors	DRAM_FAT;
EXT		FDT_DATA	FdtData			DRAM_FAT;
EXT		uint32 		FreeMem[2]		DRAM_FAT;


/*******************************************************************************************************/
//1函数原型声明
//#ifndef IN_FILE
extern 	void 	FileInit(void);
extern 	void 	FileFragClean(void);
extern 	HANDLE 	FileCreate(uint8 *Path, uint8 *DirFileName);
extern 	uint8 	FileDelete(uint8 *Path, uint8 *DirFileName);
extern 	HANDLE 	FileOpen(uint8 *Path, uint8 *DirFileName, uint8 *Type);
extern 	uint8 	FileClose(HANDLE Handle);
extern 	uint16 	FileRead(uint8 *Buf, uint16 Size, HANDLE Handle);
extern 	uint16 	FileWrite(uint8 *Buf, uint16 Size, HANDLE Handle);
extern 	uint8 	FileCopy(uint8 *SrcPath, uint8 *DestPath, FDT *SrcFile);
extern 	bool 	FileEof(HANDLE Handle);
extern 	uint8 	FileSeek(int32 offset, uint8 Whence, HANDLE Handle);
extern 	HANDLE 	FindOpenFile(uint32 DirClus, uint8 *FileName);
extern 	bool 	FileExtNameMatch(uint8 *SrcExtName, uint8 *Filter);
extern 	uint8 	FindFile(FDT *Rt, uint16 FileNum, uint8 *Path, uint8 *ExtName);
extern 	uint8 	FindFileBrowser(FDT *Rt, uint16 FileNum, uint8 *Path, uint8 *ExtName);
extern 	uint8 	FindFirst(FDT *Rt, FIND_DATA* FindData, uint8 *Path, uint8 *ExtName);
extern 	uint8 	FindNext(FDT *Rt, FIND_DATA* FindData, uint8 *ExtName);
extern 	uint8 	FindDirFirst(FDT *Rt, FIND_DATA* FindData, uint8 *Path);
extern 	uint8 	FindDirNext(FDT *Rt, FIND_DATA* FindData);
extern 	uint16 	GetTotalFiles(uint8 *Path, uint8 *ExtName);
extern 	void 	GetLongFileName(uint8 *lfn);
extern 	uint16 	GetCurFileNum(uint16 FileNum, uint8 *ExtName);
extern 	uint16 	GetGlobeFileNum(uint16 FileNum, uint8 *Path, uint8 *ExtName);
extern 	void 	StrUprCase(uint8 *name);

extern 	HANDLE 	FileOpenSys(uint8 *Path, uint8 *DirFileName, uint8 Attribute, uint8 *Type);
extern 	uint16 	FileWriteSys(uint8 *Buf, uint16 Size, HANDLE Handle);
extern 	uint8 	FileCloseSys(HANDLE Handle);
extern 	uint8 	FileRefSet(HANDLE Handle);
extern 	uint8 	FileRefReset(HANDLE Handle);
#ifdef RECOVERY
extern 	uint8	RecoveryEncInfo(RCV_INFO* RecoveryInfo);
extern 	void 	SaveEncInfo(RCV_INFO* RecoveryInfo);
#endif
//#endif


/*******************************************************************************************************/
/*******************************************************************************************************/
//#ifndef IN_FDT
extern 	uint8 	ChangeFDT(uint32 ClusIndex, FDT *FDTData);
extern 	uint8 	ReadFDTInfo(FDT *Rt, uint32 SecIndex, uint16 ByteIndex);
extern 	uint8 	WriteFDTInfo(FDT *FDTData, uint32 SecIndex, uint16 ByteIndex);
extern 	uint8 	GetFDTInfo(FDT *Rt, uint32 ClusIndex, uint32 Index);
extern 	uint8 	SetFDTInfo(uint32 ClusIndex, uint32 Index, FDT *FDTData);
extern 	uint8 	FindFDTInfo(FDT *Rt, uint32 ClusIndex, uint8 *FileName);
extern 	uint8 	AddFDT(uint32 ClusIndex, FDT *FDTData);
extern 	uint8 	DelFDT(uint32 ClusIndex, uint8 *FileName);
extern 	uint8 	FDTIsLie(uint32 ClusIndex, uint8 *FileName);
//#endif

/*******************************************************************************************************/
//#ifndef IN_nFAT
extern 	uint32 	FATGetNextClus(uint32 Index);
extern 	uint32 	FAT16GetClus(uint32 Index, uint32 Count);
extern 	uint32 	FAT32GetClus(uint32 Index, uint32 Count);
#ifdef ENCODE
extern 	uint32 	GetInvaildMem(uint8 memory);
extern 	void 	FATSetNextClus(uint32 Index, uint32 Next);
extern 	uint32 	FATAddClus(uint32 Index);
extern 	uint32 	FAT2GetNextClus(uint32 Index);
extern 	void 	FAT2SetNextClus(uint32 Index, uint32 Next);
extern 	uint32 	FAT2AddClus(uint32 Index);
extern 	void 	GarbageCollectionInit(void);
extern 	void 	GarbageCollectionFinish(void);
extern 	void 	GarbageCollection(void);
extern 	void 	EraseFAT(uint8 FatNum);
extern 	uint32 	SaveToFAT1(uint32 FirstClus);
#endif
extern 	void 	FATDelClusChain(uint32 Index);
extern 	uint32 	GetFreeMem(uint8 memory);
extern 	uint32 	GetTotalMem(uint8 memory);
extern 	uint32 	GetFreeMemFast(uint8 memory);
extern 	void 	UpdataFreeMem(int32 size);
extern 	void 	UpdataFreeMemFast(uint32 size);
//#endif
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

