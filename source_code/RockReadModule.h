/*
********************************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  File name :     RockReadModule.h
*  Author:         Jiliu Huang
*  Description:    模块调度功能
*  Others:         
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  06/02/07      0.5           ORG
********************************************************************************************
*/
#ifndef         _ROCKREADMODULE_H
#define         _ROCKREADMODULE_H

extern          int ModuleOverlay(unsigned int CodeModuleNum);
extern          int GetResourceAddress( unsigned long       *ImageLogicAddress,
                                        unsigned long       *MenuLogicAddress,
                                        unsigned long       *FontLogicAddress,
                                        unsigned long       *GBKLogicAddress,
                                        unsigned long       *SysInformationAddress);
extern          int GetFirmwareVersion( unsigned int        *MasterVersion, 
                                        unsigned int        *SlaveVersion,
                                        unsigned int        *SmallVersion, 
                                        unsigned int        *FirmwareYear,
                                        unsigned int        *FirmwareMonth,
                                        unsigned int        *FirmwareDate);
extern          long FindIDBlock(void);

#endif
