//******************************************************************************
//SysFindFile.h          2005-6-24 11:49        copyright  rock-chip  authour zyz
//******************************************************************************

#ifndef	_FILEMODULE_H
#define	_FILEMODULE_H

void FindMusicFile(unsigned int FileType, unsigned int FindDirection,unsigned int FindRange,unsigned int FindOrder);
void FindDisplayFile(unsigned int FileType, unsigned int FindDirection,unsigned int FindRange,unsigned int FindOrder);
void CreatRandomFileList(unsigned int CurrentFileNum,unsigned int TotalFileNum,unsigned int *pBuffer);

#endif
//******************************************************************************
