//***************************************************************************************************
//MP3_api.h          2005-9-19 9:59        copyright  rock-chip  authour zyz
//***************************************************************************************************
#ifndef     _MP3DECODEASM_H
#define     _MP3DECODEASM_H

void PCMBufferDataChangeStereo(unsigned int *pDecodeBuffer,unsigned int *pPCMBuffer,unsigned int GrainLen);
void PCMBufferDataChangeMono(unsigned int *pDecodeBuffer,unsigned int *pPCMBuffer,unsigned int GrainLen);
void MP3InputBufferToStart(void);

#endif
//***************************************************************************************************
