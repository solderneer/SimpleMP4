/*
********************************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  File name :     avifile.c
*  Author:         Jiliu Huang
*  Description:    Parse .avi file, get the startaddress of video and audio data in this
*                  file.
*  Others:         
*  Function list:  AviFileIndentify(),GetNextAudioChunk(),
*                  GetNextVideoChunk(),
*                  GetNextVideoIFrameChunk(), GetPreVideoIFrameChunk().
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/01      1.0           ORG
*           Jiliu Huang  06/04/10      2.0           �޸��˿�����˹��ܵķ�ʽ���ԸĽ�������˺�
*                                                    ��ͬ�������⡣
********************************************************************************************
*      [1] ֻ����Ƶ���������š���������˾���CHUNK���У�����������ʱ����Offset�ķ�����һ��
*          ��Ϊ�ڽ��п������ʱ�����н��룬����Ҫ��ȡ�ļ����ݣ�����ڽ�����һ�β���ʱ��
*         ��GetNextAudioChunk,GetFFWAudioChunk,GetFFDAudioChunk��
*      [2] ֻ����Ƶ����������ʱ����CHUNK���У���������˰��ؼ�֡���в��ţ���ֻ�йؼ�֡��������
*          ��GetNextVideoChunk,GetPrevAudioChunk��
*      [3] ���߶��У���������ʱ�������Ե�CHUNK˳����У���������˰���Ƶ�Ĺؼ�֡���С�
*********************************************************************************************
*/
#define _IN_AVIFILE
//*******************************************************************************************
#include "avistruct.h"
#include "aviglobal.h"
#include "config.h"
#include "AviMacro.h"
#include "avistruct.h"

extern signed char         hAudioIndex;
extern signed char         hVideoIndex;
/********************************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : AviFileIdentify()
*  Author:         Jiliu Huang
*  Description:    Parse an .AVI file      
*  Input:          Handle, avi_file_information
*  Output:         struct :avi_file_information
*  Return:         0: can be decoded file; 1: file too short; 2: file format error
*  Others: 
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/01      1.0           ORG
*
********************************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
int AviFileIdentify(signed char Handle,AVI_STREAM_INFO *avi_file_information)
{
    signed 		char    hAVIFile;
    unsigned 	char   	tmpDataReadout[8];
    unsigned 	char   	ReadoutNum;
    unsigned 	long   	MicroSecPerFrame;
    unsigned 	int    	blHaveIndex;
    unsigned 	int    	AudioVedioInfo = 0;     
    unsigned 	long   	TotalFrames;
    unsigned 	int    	TotalStreams;
    int             	i;
    unsigned 	long   	Seektmp;   
    unsigned 	long   	SeekSmall;       
    unsigned 	long   	MoviStartPos = 0;    
    unsigned 	long   	IndexStartPos = 0;
    
    avi_file_information -> AviFileFormat = 0;          // Set it has no audio stream or video stream
        
    hAVIFile = Handle;
    
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
    MoviStartPos += 8;
    if(ReadoutNum < 8){
        return 0x01;                             // File too short    
    }    
    if ((tmpDataReadout[0] != 'R') || (tmpDataReadout[1] != 'I')
      ||(tmpDataReadout[2] != 'F') || (tmpDataReadout[3] != 'F')){ 
        return 0x02;                            // Not right AVI file
    }    
    
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
    MoviStartPos += 8;
    if(ReadoutNum < 8){
        return 0x01;                             // File too short    
    }    
    if((tmpDataReadout[0] != 'A') || (tmpDataReadout[1] != 'V') || (tmpDataReadout[2] != 'I')){
        return 0x02;                            // Not right AVI file
    }        
    if ((tmpDataReadout[4] != 'L') || (tmpDataReadout[5] != 'I')
     || (tmpDataReadout[6] != 'S') || (tmpDataReadout[7] != 'T')){
        return 0x02;        
    }
    
    ReadoutNum = FileRead(tmpDataReadout, 4, hAVIFile);     // Read out the LIST size
    MoviStartPos += 4;
    
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile); 
    MoviStartPos += 8;    
    if ((tmpDataReadout[0] != 'h') || (tmpDataReadout[1] != 'd')
      ||(tmpDataReadout[2] != 'r') || (tmpDataReadout[3] != 'l')){ 
        return 0x02;                            // Not right AVI file
    }
    if ((tmpDataReadout[4] != 'a') || (tmpDataReadout[5] != 'v')
     || (tmpDataReadout[6] != 'i') || (tmpDataReadout[7] != 'h')){
        return 0x02;        
    }
    
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
    MoviStartPos += 8; 
    MicroSecPerFrame = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                      | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]); 
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile); 
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
    MoviStartPos += 16; 
    if((((((unsigned long)((tmpDataReadout[3] << 8) | tmpDataReadout[2]))<< 16) 
                        | ((tmpDataReadout[1] << 8) | tmpDataReadout[0])) & 0x00000010) != 0){
        blHaveIndex = 1;        // Have index chunk        
    }
    else
        return -1;          // ���û��INDEX���֣��ݲ�֧��.  20060313@hjl
    TotalFrames = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                 | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);
    
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
    MoviStartPos += 8; 
    TotalStreams = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                  | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);
    
    ReadoutNum = FileRead(tmpDataReadout, 4, hAVIFile);     // Read out dwSuggestedBufferSize
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile); 
    MoviStartPos += 12;
    if((((((unsigned long)((tmpDataReadout[3] << 8) | tmpDataReadout[2]))<< 16) 
                        | ((tmpDataReadout[1] << 8) | tmpDataReadout[0])) != 320)
                       || (((((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                           | ((tmpDataReadout[5] << 8) | tmpDataReadout[4])) != 240)){
        return 0x02;        // width not equal 160 or heigth not equal 120
    }
    FileSeek(16, SEEK_CUR, hAVIFile);       // dwReserved[4] 
    MoviStartPos += 16; 
    
    for(i=0; i<TotalStreams; i++){
        
        ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);             // 'LIST' and list length
        MoviStartPos += 8;
        Seektmp = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                 | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);
                                     
        ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile); 
        MoviStartPos += 8;
        Seektmp -= 8;
        
        if ((tmpDataReadout[0] != 's') || (tmpDataReadout[1] != 't')
         || (tmpDataReadout[2] != 'r') || (tmpDataReadout[3] != 'l')){
            return 0x02;        
        }
        if ((tmpDataReadout[4] != 's') || (tmpDataReadout[5] != 't')
         || (tmpDataReadout[6] != 'r') || (tmpDataReadout[7] != 'h')){
            return 0x02;        
        }
        ReadoutNum = FileRead(tmpDataReadout, 4, hAVIFile);			// Read out size of 'strh' 
        SeekSmall =  (((unsigned long)((tmpDataReadout[3] << 8) | tmpDataReadout[2]))<< 16) 
                                    | ((tmpDataReadout[1] << 8) | tmpDataReadout[0]);
        MoviStartPos += 4;
        Seektmp -= 4;
                                 
        ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);        
        MoviStartPos += 8; 
        Seektmp -= 8;
        SeekSmall -= 8;
        if ((tmpDataReadout[0] == 'v') && (tmpDataReadout[1] == 'i')
         && (tmpDataReadout[2] == 'd') && (tmpDataReadout[3] == 's')){
            if ((tmpDataReadout[4] != 'X') || (tmpDataReadout[5] != 'V')
             || (tmpDataReadout[6] != 'I') || (tmpDataReadout[7] != 'D')){
                return 0x02;                // Video format is not supported
            }
            (avi_file_information -> AviFileFormat) |= 2;           // Have Video stream, set the flag
        }
        else if((tmpDataReadout[0] == 'a') && (tmpDataReadout[1] == 'u')
             && (tmpDataReadout[2] == 'd') && (tmpDataReadout[3] == 's')){
            FileSeek(SeekSmall, SEEK_CUR, hAVIFile);
            MoviStartPos += SeekSmall;
            Seektmp -= SeekSmall;
            
            ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
            MoviStartPos += 8;
            Seektmp -= 8;
             
            if ((tmpDataReadout[0] != 's') || (tmpDataReadout[1] != 't')
             || (tmpDataReadout[2] != 'r') || (tmpDataReadout[3] != 'f')){
                return 0x02;                // File format error
            }
            ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
            MoviStartPos += 8;
            Seektmp -= 8;                       
            if ((tmpDataReadout[0] != 0x50) || (tmpDataReadout[1] != 0)){
                return 0x02;                // Audio format is not supported
            }
            avi_file_information -> AudioSamplingRate = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16)
                                                                   | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);
            (avi_file_information -> AviFileFormat) |= 1;           // Have audio stream, set the flag
//            ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
//            MoviStartPos += 8; 
//            Seektmp -= 8;            
        }
        else{
            return 0x02;   
        }
        
        FileSeek(Seektmp, SEEK_CUR, hAVIFile);
        MoviStartPos += Seektmp;
        
        while(1){
            ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
            if(ReadoutNum < 8){
                return 0x02;   
            }
            //MoviStartPos += 8; 
            if ((tmpDataReadout[0] == 'J') && (tmpDataReadout[1] == 'U')
              &&(tmpDataReadout[2] == 'N') && (tmpDataReadout[3] == 'K')){
                Seektmp = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                         | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);
                FileSeek(Seektmp, SEEK_CUR, hAVIFile);
                MoviStartPos += (Seektmp + 8);                                                         
            }
            else{
            	FileSeek(-8, SEEK_CUR, hAVIFile);			// �������JUNK����ָ�����8�ֽ�֮ǰ���ļ�λ��
                break;   
            }
        }        
    }
    
    //////////////  Find movi chunk ////////////////
    ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
    MoviStartPos += 8;
    while(!FileEof(hAVIFile)){
        if ((tmpDataReadout[0] == 'L') && (tmpDataReadout[1] == 'I')
          &&(tmpDataReadout[2] == 'S') && (tmpDataReadout[3] == 'T')){
          	
               Seektmp = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                        | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);                                                      
               ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
               MoviStartPos += 8; 
               Seektmp -= 8;
            if ((tmpDataReadout[0] == 'm') && (tmpDataReadout[1] == 'o')
              &&(tmpDataReadout[2] == 'v') && (tmpDataReadout[3] == 'i')){
                IndexStartPos = Seektmp + MoviStartPos;
                break;                      // Find FOURCC 'movi'
            }
            FileSeek(Seektmp, SEEK_CUR, hAVIFile);
            ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile); 
            MoviStartPos += (8 + Seektmp);
        }      
        else if ((tmpDataReadout[0] == 'J') && (tmpDataReadout[1] == 'U')
               &&(tmpDataReadout[2] == 'N') && (tmpDataReadout[3] == 'K')){
              Seektmp = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                       | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);
              FileSeek(Seektmp, SEEK_CUR, hAVIFile);
              ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile); 
              MoviStartPos += (8 + Seektmp);
        }
        else {
              Seektmp = (((unsigned long)((tmpDataReadout[7] << 8) | tmpDataReadout[6]))<< 16) 
                                       | ((tmpDataReadout[5] << 8) | tmpDataReadout[4]);
              FileSeek(Seektmp, SEEK_CUR, hAVIFile);
              ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);
              MoviStartPos += (8 + Seektmp);
        }  
    }
    if(FileEof(hAVIFile)){              // If find 'movi' in the end of file ,return error.
        return -1;   
    }
    if (((avi_file_information -> AviFileFormat) & 0x3) == 0){
        return -1;                      // No audio and video stream, not support it
    }
    
    //�������������'movi'����ʼ��ַ,4Ϊ���ݳ��ȣ����ܲ�����룩
    avi_file_information -> StartDataPos = MoviStartPos + 4;
              
    if(blHaveIndex = 1){
        avi_file_information -> StartIndexPos = (IndexStartPos + 8);
        
        //��ʼ��Audio��Video��index�ļ�ָ��
        FileSeek(avi_file_information -> StartIndexPos, SEEK_SET, hAudioIndex); 
        FileRefSet(hAudioIndex);
        if(FileEof(hAudioIndex)){
            return -1;   
        }
        FileSeek(avi_file_information -> StartIndexPos, SEEK_SET, hVideoIndex);
        FileRefSet(hVideoIndex);
        VideoIndexBuff_valid = 0;
        VideoIndexBuff_ptr = 0;
      	AudioIndexBuff_ptr = 0;
        AudioIndexBuff_valid = 0;
        FFDWFramecount = 240;       //10s
        if(FileEof(hVideoIndex)){
            return -1;   
        }
        VideoIndexCount = 0;        //�����Ƶ��INDEX������,�ü�������Ҫ���ڿ��˵Ľ������ж�
        AudioIndexCount = 0;
    }
    else{
        avi_file_information -> StartIndexPos = 0;  // AVI�ļ�û��index ����
        //return -1;          // ���û��INDEX���֣��ݲ�֧��.  20060313@hjl
    }
    
    //��ʼ������Ƶ�����'movi'��ƫ����
    AudioFilePos = 4;           
    VideoFilePos = 4;     
    AudioChunkLen = 0;
    VideoChunkLen = 0;
    
    
    avi_file_information -> TotalFrameCount = TotalFrames;
    avi_file_information -> MicroSecondPerFrame = MicroSecPerFrame / 10;
    return 0;
    //FileSeek(IndexStartPos, SEEK_SET, hAVIFile);    
    //ReadoutNum = FileRead(tmpDataReadout, 8, hAVIFile);       // For test only,read out data for confirming
}

/******************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : GetNextAudioChunk()
*  Author:         Jiliu Huang
*  Description:     
*  Input:       Ҫ��ȫ�ֱ���hAudioIndex��֧��     
*  Output:      
*  Return:         
*  Others: 
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/01      1.0           ORG
*
******************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
int GetNextAudioChunk(unsigned long *AudioChunkOffset, unsigned long *ChunkLen)
{
    unsigned long   tmpAudioFilePos;
    unsigned long   tmpAudioChunkLen;
    int             i;
        
    *AudioChunkOffset = 0;
    
    tmpAudioFilePos = AudioFilePos;      //����Audio��indexֵ
    tmpAudioChunkLen = AudioChunkLen;    //�����ϴε�Chunk���ȣ����ڼ���ƫ����
    
    while((!FileEof(hAudioIndex))||(AudioIndexBuff_ptr<AudioIndexBuff_valid)){        // hAudioIndex:ȫ�ֱ���
        i = AudioIndexBuff_ptr;
        for(; i < AudioIndexBuff_valid; i+=16){
        	 AudioIndexBuff_ptr+=16;
        	 AudioIndexCount++;
           if ((AudioIndexBuff[i+2] == 'w') && (AudioIndexBuff[i+3] == 'b')){
              AviAudioCurrFrameNum++;
              AudioFilePos = (((unsigned long)((AudioIndexBuff[i+11] << 8) | AudioIndexBuff[i+10]))<< 16) 
                                           | ((AudioIndexBuff[i+9] << 8) | AudioIndexBuff[i+8]);
              AudioChunkLen = (((unsigned long)((AudioIndexBuff[i+15] << 8) | AudioIndexBuff[i+14]))<< 16) 
                                           | ((AudioIndexBuff[i+13] << 8) | AudioIndexBuff[i+12]);
                                                                                    
                                           
            // ������Ƶ��ƫ����=���ε��ļ�λ��-�ϴ��ļ���λ��+�ϴε�Chunk����
            *AudioChunkOffset = AudioFilePos - tmpAudioFilePos - tmpAudioChunkLen;
            *ChunkLen = AudioChunkLen;
            return 0;
        }else if((AudioIndexBuff[i+2] != 'd') || (AudioIndexBuff[i+3] != 'c')){
            return -1;
        }
      }
        AudioIndexBuff_ptr =0;
        if(!(AudioIndexBuff_valid = FileRead(AudioIndexBuff, 512, hAudioIndex))){
        	return -1;
        }
    }
    return -1;      //ֱ���ļ�������û�ҵ���ƵINDEX���
}

/******************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : GetNextVideoChunk()
*  Author:         Jiliu Huang
*  Description:     
*  Input:          Ҫ��ȫ�ֱ���hVideoIndex��֧��                  
*  Output:         
*  Return:         0: P/N; 1: I; -1:Error
*  Others:          VideoFilePos.��Ƶ
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/01      1.0           ORG
*  modify   zcs          06/04/22
******************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
int GetNextVideoChunk(unsigned long *VideoChunkOffset, unsigned long *ChunkLen)
{
    unsigned long   tmpVideoFilePos;
    unsigned long   tmpVideoChunkLen;
    unsigned long   tmpFrameFlag;        //
    int             i;

    *VideoChunkOffset = 0;
    
    tmpVideoFilePos = VideoFilePos;      //����Video��indexֵ
    tmpVideoChunkLen = VideoChunkLen;    //�����ϴε�Chunk���ȣ����ڼ���ƫ����
    while((!FileEof(hVideoIndex))||(VideoIndexBuff_ptr<VideoIndexBuff_valid)){
        i = VideoIndexBuff_ptr;
        for(; i < VideoIndexBuff_valid; i+=16){
        	 VideoIndexBuff_ptr+=16;
        	 VideoIndexCount++;
           if ((VideoIndexBuff[i+2] == 'd') && (VideoIndexBuff[i+3] == 'c')){
              AviCurrFrameNum++;
              VideoFilePos = (((unsigned long)((VideoIndexBuff[i+11] << 8) | VideoIndexBuff[i+10]))<< 16) 
                                           | ((VideoIndexBuff[i+9] << 8) | VideoIndexBuff[i+8]);
              VideoChunkLen = (((unsigned long)((VideoIndexBuff[i+15] << 8) | VideoIndexBuff[i+14]))<< 16) 
                                           | ((VideoIndexBuff[i+13] << 8) | VideoIndexBuff[i+12]);
                                                                                    
              // ������Ƶ��ƫ����=���ε��ļ�λ��-�ϴ��ļ���λ��+�ϴε�Chunk����
              *VideoChunkOffset = VideoFilePos - tmpVideoFilePos - tmpVideoChunkLen;
              *ChunkLen = VideoChunkLen;
            
              if((((((unsigned long)((VideoIndexBuff[i+7] << 8) | VideoIndexBuff[i+6]))<< 16)
                                 | ((VideoIndexBuff[i+5] << 8) | VideoIndexBuff[i+4])) 
                                 && 0x00000010) != 0){
                return 1;       // I
              }
              else{
                return 0;      // P/N
            }
        }
        else if((VideoIndexBuff[i+2] != 'w') || (VideoIndexBuff[i+3] != 'b')){
            return -1;
        }
        }
        VideoIndexBuff_ptr =0;
        if(!(VideoIndexBuff_valid = FileRead(VideoIndexBuff, 512, hVideoIndex))){
        	return -1;
        }
      }
      return -1;      //ֱ���ļ�������û�ҵ���ƵINDEX���
}

/******************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : GetNextVideoIFrameChunk()
*  Author:         Jiliu Huang
*  Description:     
*  Input:          
*  Output:         
*  Return:         
*  Others: 
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/14      1.0           ORG
*           Jiliu Huang  06/04/10      2.0           �޸��˿�����˹��ܵķ�ʽ���ԸĽ�������˺�
*                                                    ��ͬ�������⡣
******************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
int GetNextVideoIFrameChunk(unsigned long *VideoChunkOffset, unsigned long *ChunkLen)
{
    unsigned long   tmpVideoFilePos;
    unsigned long   tmpVideoChunkLen;
    unsigned long   tmpFrameFlag = 0;           // 0:����I֡
    unsigned long   tmpVideoChunkLenLeft;
    int             i;
    unsigned int    Framecount;
    *VideoChunkOffset = 0;
    
    tmpVideoFilePos = VideoFilePos;     //����Video��indexֵ
    tmpVideoChunkLen = VideoChunkLen;   //�����ϴε�Chunk���ȣ����ڼ���ƫ����
    
    // ��¼��ý��ĵ�ǰλ�ã���Ϊ��������������û�ж�ȡ���ϴε����ݳ��ȣ�����ܻ����
    tmpVideoChunkLenLeft = *ChunkLen;      
    Framecount =0;
    while((!FileEof(hVideoIndex))||(VideoIndexBuff_ptr<VideoIndexBuff_valid)){
        i = VideoIndexBuff_ptr;
        for(; i < VideoIndexBuff_valid; i+=16){
        	 VideoIndexBuff_ptr+=16;
        	 VideoIndexCount++;
           if ((VideoIndexBuff[i+2] == 'd') && (VideoIndexBuff[i+3] == 'c')){
              AviCurrFrameNum++;
              AviCurr_IFrameNum_FrameNum = AviCurrFrameNum;
              Framecount++;
              if(((((((unsigned long)((VideoIndexBuff[i+7] << 8) | VideoIndexBuff[i+6]))<< 16)
                                 | ((VideoIndexBuff[i+5] << 8) | VideoIndexBuff[i+4])) 
                                 && 0x00000010) != 0)){
                VideoFilePos = (((unsigned long)((VideoIndexBuff[i+11] << 8) | VideoIndexBuff[i+10]))<< 16) 
                                             | ((VideoIndexBuff[i+9] << 8) | VideoIndexBuff[i+8]);
                VideoChunkLen = (((unsigned long)((VideoIndexBuff[i+15] << 8) | VideoIndexBuff[i+14]))<< 16) 
                                             | ((VideoIndexBuff[i+13] << 8) | VideoIndexBuff[i+12]);
                                                                                    
                // ������Ƶ��ƫ����=���ε��ļ�λ��-�ϴ��ļ���λ��-�ϴε�Chunk����
                *VideoChunkOffset = VideoFilePos - tmpVideoFilePos - tmpVideoChunkLen + tmpVideoChunkLenLeft;
                *ChunkLen = VideoChunkLen;
                if(Framecount > FFDWFramecount){
                	 return 0;           // �ҵ���I֡
                	}
              }
          }
          else if((VideoIndexBuff[i+2] != 'w') || (VideoIndexBuff[i+3] != 'b')){
            return -1;
          }
      }
      VideoIndexBuff_ptr =0;
      if(!(VideoIndexBuff_valid = FileRead(VideoIndexBuff, 512, hVideoIndex))){
      	return -1;
      }
   }
   return -1;      //ֱ���ļ�������û�ҵ���ƵINDEX���
}

/******************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : GetPreVideoIFrameChunk()
*  Author:         Jiliu Huang
*  Description:     
*  Input:          
*  Output:         
*  Return:   0.�ҵ�����ƵI֡����Ӧ����Ƶ֡
*            1.�ҵ�����ƵI֡��û����Ƶ֡
*            -1.û���ҵ���ƵI֡      
*  Others: 
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  05/11/14      1.0           ORG
*           Jiliu Huang  06/04/10      2.0           �޸��˿�����˹��ܵķ�ʽ���ԸĽ�������˺�
*                                                    ��ͬ�������⡣
******************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
int GetPreVideoIFrameChunk( unsigned long *VideoChunkOffset, 
                             unsigned long *ChunkLen)
{
    unsigned long   tmp;
    unsigned long   tmpVideoFilePos;
    unsigned long   tmpVideoChunkLen;
    unsigned long   tmpFrameFlag;        //
    int             i;
    unsigned int    Framecount,IFrame_count;
    *VideoChunkOffset = 0;
    IFrame_count = 0;
    Framecount = 0;
    tmpVideoFilePos = VideoFilePos;      //����Video��indexֵ
    tmpVideoChunkLen = VideoChunkLen;    //�����ϴε�Chunk���ȣ����ڼ���ƫ����
    tmp = 512;
 
    while(VideoIndexCount){
       i = VideoIndexBuff_ptr-16;
       for(; i >= 0; i-=16){        	 
        	 VideoIndexBuff_ptr-=16;
           VideoIndexCount--;
           if ((VideoIndexBuff[i+2] == 'd') && (VideoIndexBuff[i+3] == 'c')){
           	  Framecount++;
              AviCurrFrameNum--;         //ke nen cun zai wenti
              AviCurr_IFrameNum_FrameNum = AviCurrFrameNum;
              if((((((unsigned long)((VideoIndexBuff[i+7] << 8) | VideoIndexBuff[i+6]))<< 16)
                                 | ((VideoIndexBuff[i+5] << 8) | VideoIndexBuff[i+4])) 
                                 && 0x00000010) != 0){
  	              VideoFilePos = (((unsigned long)((VideoIndexBuff[i+11] << 8) | VideoIndexBuff[i+10]))<< 16) 
                                           | ((VideoIndexBuff[i+9] << 8) | VideoIndexBuff[i+8]);
		              VideoChunkLen = (((unsigned long)((VideoIndexBuff[i+15] << 8) | VideoIndexBuff[i+14]))<< 16) 
                                           | ((VideoIndexBuff[i+13] << 8) | VideoIndexBuff[i+12]);
                                                                                    
		              // ������Ƶ��ƫ����=���ε��ļ�λ��-�ϴ��ļ���λ��+�ϴε�Chunk����
		             *VideoChunkOffset = VideoFilePos + AviStreamInfo.StartDataPos - 4;   
		             *ChunkLen = VideoChunkLen;
                 if((IFrame_count == 1) && (Framecount > FFDWFramecount)){            
			                return 0;       // I
			            }else{
	    		            IFrame_count = 1;
	    		        }
              }
        }
        else if((VideoIndexBuff[i+2] != 'w') || (VideoIndexBuff[i+3] != 'b')){
            return -1;
        }
        }
           FileSeek((-tmp-VideoIndexBuff_valid), SEEK_REF, hVideoIndex);
           if(!(VideoIndexBuff_valid = FileRead(VideoIndexBuff, 512, hVideoIndex))){
           	   return -1; 
           	  }
           VideoIndexBuff_ptr = VideoIndexBuff_valid;
      }
      if(IFrame_count == 0){
      	return -1;      //ֱ���ļ�������û�ҵ���ƵINDEX���
      }else{
      	return 0;
      }
}

/******************************************************************************
*  Copyright (C),2004-2005, Fuzhou Rockchip Co.,Ltd.
*  Function name : SynAudio2VideoChunk()
*  Author:         Jiliu Huang
*  Description:    ��GetNextVideoIFrameChunk()��GetPreVideoIFrameChunk()��������
*                  �󣬱���ִ�иú�����ͬ����Ƶ�źš�
*  Input:          
*  Output:         
*  Return:   0.�ҵ��˿������˺�����Ƶ֡��Ӧ����Ƶ֡
*            -1.û���ҵ��˿������˺�����Ƶ֡��Ӧ����Ƶ֡
*  Others: 
*  History:        
*           <author>      <time>     <version>       <desc>
*           Jiliu Huang  06/04/10      1.0           ORG
******************************************************************************/
__attribute__((section(".avi_v_text, \"ax\"")))
int SynAudio2VideoChunk(unsigned long       *AudioChunkOffset, 
                        unsigned long       *ChunkLen,
                        unsigned long       *CurrAviFrameNum,
                        unsigned long       *AviLastChunkNum,
                        AVI_STREAM_INFO     *avi_file_information)
{
    unsigned long   tmpAudioFilePos;
    unsigned long   tmpAudioChunkLen;
    unsigned long   tmpAudioChunkLenLeft;    
    tmpAudioFilePos = AudioFilePos;
    tmpAudioChunkLen = AudioChunkLen;
    tmpAudioChunkLenLeft = *ChunkLen;
    
    unsigned long   uiAudioJumpBackChunksNeeded;
    int             i;
    unsigned long   uiAudioChunkJumped = 0;
    unsigned long   tmp;
    unsigned long   ulAudioPosition;
    //ulAudioPosition = (((*CurrAviFrameNum)*5) * (avi_file_information -> MicroSecondPerFrame)) / AVI_AUDIO_FRAME_TIME;// + 20;
    //ulAudioPosition = ((ulAudioPosition<<8) - (ulAudioPosition>>8))>>8;
    Timeadjust = 0;
//    if(Play_Frame_Rate == 20)
//        ulAudioPosition = ((*CurrAviFrameNum)*245)>>7;
//    else if(Play_Frame_Rate == 24)
//        ulAudioPosition = (((*CurrAviFrameNum)*1225)>>8)/3;
//    else if(Play_Frame_Rate == 22)
//        ulAudioPosition = (((*CurrAviFrameNum)*1225)>>6)/11;
     ulAudioPosition = (((*CurrAviFrameNum)*1225)>>5)/Play_Frame_Rate;      
     
    if(ulAudioPosition >= AviAudioCurrFrameNum){
        uiAudioJumpBackChunksNeeded = ulAudioPosition - AviAudioCurrFrameNum;
        while((!FileEof(hAudioIndex))||(AudioIndexBuff_ptr<AudioIndexBuff_valid)){        // hAudioIndex:ȫ�ֱ���
        	 i = AudioIndexBuff_ptr;
           for(; i < AudioIndexBuff_valid; i+=16){
           	   AudioIndexBuff_ptr+=16;
        	     AudioIndexCount++;
               if ((AudioIndexBuff[i+2] == 'w') && (AudioIndexBuff[i+3] == 'b')){
            	    uiAudioChunkJumped++;
                    AviAudioCurrFrameNum++;
                    m_audio_framenum = AviAudioCurrFrameNum;
//                    if(Play_Frame_Rate == 20){
//                        m_video_framenum = (((long)AviAudioCurrFrameNum<<7)+123)/245;
//                    }else if(Play_Frame_Rate == 24){
//                        m_video_framenum = (((long)AviAudioCurrFrameNum<<8)*3+613)/1225;
//                    }else if(Play_Frame_Rate == 22){
//                        m_video_framenum = (((long)AviAudioCurrFrameNum<<6)*11+613)/1225;
//                    }
                    m_video_framenum = (((long)AviAudioCurrFrameNum<<5)*Play_Frame_Rate+613)/1225;

                  if(uiAudioChunkJumped >= uiAudioJumpBackChunksNeeded){
                      AudioFilePos = (((unsigned long)((AudioIndexBuff[i+11] << 8) | AudioIndexBuff[i+10]))<< 16) 
                                              | ((AudioIndexBuff[i+9] << 8) | AudioIndexBuff[i+8]);
                      AudioChunkLen = (((unsigned long)((AudioIndexBuff[i+15] << 8) | AudioIndexBuff[i+14]))<< 16) 
                                              | ((AudioIndexBuff[i+13] << 8) | AudioIndexBuff[i+12]);
                                                                                     
                      // ������Ƶ��ƫ����=���ε��ļ�λ��-�ϴ��ļ���λ��+�ϴε�Chunk����
                     *AudioChunkOffset = AudioFilePos - tmpAudioFilePos - tmpAudioChunkLen + tmpAudioChunkLenLeft;
                     *ChunkLen = AudioChunkLen;  
                     return 0;
                }
              }else if((AudioIndexBuff[i+2] != 'd') || (AudioIndexBuff[i+3] != 'c')){
                  return -1;
              }
            }
           AudioIndexBuff_ptr =0;
           if(!(AudioIndexBuff_valid = FileRead(AudioIndexBuff, 512, hAudioIndex))){
           	  return -1; 
           	}
           	        	
        }
        return -1;      //ֱ���ļ�������û�ҵ���ƵINDEX���
      }else{
        FileRefReset(hAudioIndex);
//      	FileSeek(avi_file_information -> StartIndexPos, SEEK_SET, hAudioIndex);
//        tmpAudioChunkLenLeft =0;
        uiAudioJumpBackChunksNeeded = ulAudioPosition;
        AviAudioCurrFrameNum = 0;
        AudioIndexCount = 0;
        AudioIndexBuff_ptr =0;
        AudioIndexBuff_valid =0;
//        Timeadjust = 0;
        m_audio_framenum = 0;
        while((!FileEof(hAudioIndex))||(AudioIndexBuff_ptr<AudioIndexBuff_valid)){        // hAudioIndex:ȫ�ֱ���
        	 i = AudioIndexBuff_ptr;
           for(; i < AudioIndexBuff_valid; i+=16){
           	   AudioIndexBuff_ptr+=16;
        	     AudioIndexCount++;
               if ((AudioIndexBuff[i+2] == 'w') && (AudioIndexBuff[i+3] == 'b')){
            	     uiAudioChunkJumped++;
                   AviAudioCurrFrameNum++;
                   m_audio_framenum = AviAudioCurrFrameNum;
//                   if(Play_Frame_Rate == 20){
//                        m_video_framenum = (((long)AviAudioCurrFrameNum<<7)+123)/245;
//                   }else if(Play_Frame_Rate == 24){
//                        m_video_framenum = (((long)AviAudioCurrFrameNum<<8)*3+613)/1225;
//                   }else if(Play_Frame_Rate == 22){
//                        m_video_framenum = (((long)AviAudioCurrFrameNum<<6)*11+613)/1225;
//                    } 
                    m_video_framenum = (((long)AviAudioCurrFrameNum<<5)*Play_Frame_Rate+613)/1225;

                  if(uiAudioChunkJumped >= uiAudioJumpBackChunksNeeded){
                      AudioFilePos = (((unsigned long)((AudioIndexBuff[i+11] << 8) | AudioIndexBuff[i+10]))<< 16) 
                                              | ((AudioIndexBuff[i+9] << 8) | AudioIndexBuff[i+8]);
                      AudioChunkLen = (((unsigned long)((AudioIndexBuff[i+15] << 8) | AudioIndexBuff[i+14]))<< 16) 
                                              | ((AudioIndexBuff[i+13] << 8) | AudioIndexBuff[i+12]);
                                                                                     
                      // ������Ƶ��ƫ����=���ε��ļ�λ��-�ϴ��ļ���λ��+�ϴε�Chunk����
                     *AudioChunkOffset = AudioFilePos + avi_file_information -> StartDataPos - 4;
                     *ChunkLen = AudioChunkLen;  
                     return 0;
                }
              }else if((AudioIndexBuff[i+2] != 'd') || (AudioIndexBuff[i+3] != 'c')){
                  return -1;
              }
            }
           AudioIndexBuff_ptr =0;
           if(!(AudioIndexBuff_valid = FileRead(AudioIndexBuff, 512, hAudioIndex))){
           	   return -1; 
           	  }
        }
        return -1;      //ֱ���ļ�������û�ҵ���ƵINDEX���
      }
    return -1; 
}

