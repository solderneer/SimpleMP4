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

//1�����ò���
#define 	MAX_OPEN_FILES		4		/*����ͬʱ�򿪵��ļ���Ŀ*/
#define 	MAX_LFN_ENTRIES   		3		/*���3*13���ַ�*/


//1��������
/* ��������ֵ */
#define	RETURN_OK                   	0x00    	/* �����ɹ�*/
#define	NOT_FIND_DISK               	0x01    	/* �߼��̲�����*/
#define	DISK_FULL                   	0x02    	/* �߼�����*/
#define	SECTOR_NOT_IN_CACHE   0x03    	/* ����û�б�cache  */
#define	NOT_EMPTY_CACHE          	0x04    	/* û�п���cache*/
#define	SECTOR_READ_ERR           	0x05    	/* ����������*/
#define	CLUSTER_NOT_IN_DISK    0x06    	/* �߼�����û�д˴�  */
#define 	NOT_FIND_FDT                	0x07    	/* û�з����ļ�(Ŀ¼)*/
#define 	NOT_FAT_DISK                	0x08    	/* ��FAT�ļ�ϵͳ*/
#define 	FDT_OVER                    	0x09    	/* FDT����������Χ*/
#define 	FDT_EXISTS                  	0x0a    	/* �ļ�(Ŀ¼)�Ѿ�����*/
#define 	ROOT_FDT_FULL               	0x0b    	/* ��Ŀ¼��*/
#define 	DIR_EMPTY                   	0x0C    	/* Ŀ¼��*/
#define 	DIR_NOT_EMPTY               	0x0d    	/* Ŀ¼����*/
#define 	PATH_NOT_FIND               0x0e    	/* ·��δ�ҵ�*/
#define 	FAT_ERR                     	0x0f    	/* FAT�����*/
#define 	FILE_NAME_ERR               	0x10    	/* �ļ�(Ŀ¼)������*/
#define 	FILE_EOF                    	0x11    	/* �ļ�����*/
#define 	FILE_LOCK                   	0x12    	/* �ļ�������*/
#define 	NOT_FIND_FILE               	0x13    	/* û�з���ָ���ļ�*/
#define 	NOT_FIND_DIR                	0x14    	/* û�з���ָ��Ŀ¼*/
#define 	NOT_RUN                     	0xfd    	/* ����δִ��*/
#define 	BAD_COMMAND                 	0xfe    	/* ��������*/
#define 	PARAMETER_ERR               0xff    	/* �Ƿ�����*/

#define 	NOT_OPEN_FILE			-1		//���ܴ��ļ�,�ļ������



#ifndef 	NULL
#define	NULL	0
#endif

/* FAT���� */
#ifndef 	FAT12
#define	FAT12	0
#endif
#ifndef 	FAT16
#define	FAT16	1
#endif
#ifndef 	FAT32
#define	FAT32	2
#endif

/* FDT�ļ����� */
#define 	ATTR_READ_ONLY             0x01
#define 	ATTR_HIDDEN                 	0x02
#define 	ATTR_SYSTEM                 	0x04
#define 	ATTR_VOLUME_ID             	0x08
#define 	ATTR_LFN_ENTRY    		0x0F      /* LFN entry attribute */
#define 	ATTR_DIRECTORY             	0x10
#define 	ATTR_ARCHIVE                	0x20

/* ������ */
#define 	EMPTY_CLUS               	0
#define 	EMPTY_CLUS_1           	1
#define 	BAD_CLUS                   	0x0ffffff7L
#define 	EOF_CLUS_1                  	0x0ffffff8L
#define 	EOF_CLUS_END             	0x0fffffffL

/* FDT���� */
#define 	FILE_NOT_EXIST			0
#define 	FILE_DELETED			0xe5
#define 	ESC_FDT                    		0x05

/* ���ļ���*/
#define 	MAX_FILENAME_LEN  	((MAX_LFN_ENTRIES * 13 * 2 + 2)/2)		//��β��NULռ��2B
#define 	LFN_SEQ_MASK			0x3f

/* �ļ��򿪷�ʽ */
#define 	FILE_FLAGS_READ          	1 << 0		//�ɶ�
#define 	FILE_FLAGS_WRITE        	1 << 1		//��д

/* �ļ�ָ�������ʽ */
#define 	SEEK_SET    				0           		//���ļ���ʼ���ƶ��ļ�ָ��
#define 	SEEK_CUR    				1           		//���ļ���ǰλ���ƶ��ļ�ָ��
#define 	SEEK_END    				2           		//���ļ�β�ƶ��ļ�ָ��
#define 	SEEK_REF    				3           		//���ļ��ο��㿪ʼ


//1�ṹ����
//Ŀ¼�����ݽṹ

typedef 		int8		HANDLE;				//�ļ�����������Ͷ���

//���ҽṹ��
typedef struct _FIND_DATA
{
	uint32	Index;
	uint32	Clus;     			//��ǰ�غ�
} FIND_DATA;

//Ŀ¼�����ӽṹ��
typedef struct _FDT_DATA
{
	uint32	DirClus;			//Ŀ¼�״�
	uint32	CurClus;     		//��ǰ�غ�
	uint16	Cnt;				//��������
} FDT_DATA;

typedef struct _FDT
{
	uint8	Name[11];		//���ļ������ļ���
	uint8	Attr;           		//�ļ�����
	uint8	NTRes;              	//������NT
	uint8	CrtTimeTenth;  	//����ʱ�䣨fat16������

	uint16	CrtTime;           	//����ʱ�䣨fat16������
//	uint16	reserve1;           	//����

	uint16	CrtDate;            	//�������ڣ�fat16������
//	uint16	reserve2;           	//����

	uint16	LstAccDate;      	//���������ڣ�fat16������
//	uint16	reserve3;           	//����

	uint16	FstClusHI;         	//��ʼ�غŸ������ֽڣ�fat16������
//	uint16	reserve4;           	//����

	uint16	WrtTime;           	//���дʱ��
//	uint16	reserve5;           	//����

	uint16	WrtDate;            	//���д����
//	uint16	reserve6;           	//����

	uint16	FstClusLO;          	//��ʼ��(cluster)�ŵ������ֽ�
//	uint16	reserve7;           	//����

	uint32	FileSize;         	//�ļ���С
//	uint16	reserve8;           	//����
//	uint16	reserve9;           	//����
} FDT;

//�ļ���Ϣ�ṹ��
typedef struct _FILE
{
	uint8	Flags;          		//һЩ��־
	uint8	Name[11];    		//�ļ���
	uint32	DirClus;	        	//����Ŀ¼��ʼ�غ�
	uint32	FileSize;		//�ļ���С
	uint32	FstClus;			//��ʼ�غ�
	uint32	Clus;     			//��ǰ�غ�
	uint32	Offset;			//�ļ�ָ��ƫ����
	uint32	RefClus;    		//��ǰ�غ�
	uint32	RefOffset;		//�ļ�ָ��ƫ����
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



//1ȫ�ֱ���
#undef	EXT
#ifdef	IN_FILE
		#define	EXT
#else
		#define	EXT		extern
#endif		
		
EXT		uint32 		CurDirClus	DRAM_FAT;						//��ǰĿ¼��
EXT		MY_FILE 		FileInfo[MAX_OPEN_FILES]	DRAM_FAT;			//ͬʱ���ļ���Ϣ��
EXT		uint8 		LongFileName[MAX_FILENAME_LEN]	DRAM_FAT;	//���ļ���
EXT     RCV_INFO    EncRcvInfo DRAM_FAT;
#undef	EXT
#ifdef	IN_nFAT
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		uint8 		FatBuf[512]	DRAM_FAT;		//FAT��CACHE
EXT		uint8 		FdtBuf[512]	DRAM_FAT;		//FDT��CACHE
EXT		uint32		FatCacheSec	DRAM_FAT;		//��ǰ��Cache FAT��������
EXT		uint32		Fat2CacheSec		DRAM_ENCODE;	//��ǰ��Cache FAT2��������
EXT		uint8 		Fat2Buf[512]		DRAM_ENCODE;	//FAT2��CACHE
EXT		uint16		Fat2WriteLen		DRAM_ENCODE;	//FAT2 ���ȼ���
EXT		uint8 		Fat2WriteBuf[2048]	DRAM_ENCODE;	//FAT2K BUF
EXT		uint16		FileWriteLen		DRAM_ENCODE;	//file write ���ȼ���
EXT		uint8 		FileWriteBuf[2048]	DRAM_ENCODE;	//file write BUF
EXT		uint32		FileWriteResidue	DRAM_ENCODE;	//file write ���ȼ���
EXT		uint32		FdtCacheSec	DRAM_FAT;		//��ǰ��Cache FDT��������
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
//1����ԭ������
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

