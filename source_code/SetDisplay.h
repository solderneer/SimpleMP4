//******************************************************************************
#ifndef     _SET_DISPLAY_H
#define     _SET_DISPLAY_H

//------------------------------------------------------------------------------
void DISP_Set(void);
void ConvertMemMB(unsigned int *pBuff, unsigned long *Mem);
void DispCopyRightFunc(unsigned int key, unsigned long flash_addr, unsigned int *pValue);
void UserEqSettingDisplay(unsigned int cont, unsigned int *pValue);
//------------------------------------------------------------------------------
#endif
//******************************************************************************

