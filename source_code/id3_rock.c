#include "Mp3DecodeGlobal.h"
#include "MP3DecodeStruct.h"
#include "Global.h"
#include "Config.h"

#define Word32  long
#define Word16  short

__attribute__((section(".mp3_text,\"ax\"")))
void Find_ID3_TAG(ID3_TAG *id3_info,Word16 *Buffer)
{

	Word16 j=0;
	Word16 m=0;
	Word16 i=10;
	Word16 k=0;
	Word16 h=0;

	Word32 title_size=0, singer_size=0, album_size=0,liupai_size=0,min_size=0;
	Word16 id3_find_mode=0xff;
	Word16 id3_find_status=0,num=0;
	Word16 id3_pic_status=0;
	Word16 image_size;

	
	for(j=0; j<ID3_TAG_SIZE; j++) id3_info->id3_title[j] = 0x00;
	for(j=0; j<ID3_TAG_SIZE; j++) id3_info->id3_singer[j] = 0x00;
	for(j=0; j<ID3_TAG_SIZE; j++) id3_info->id3_album[j] = 0x00;
    for(j=0; j<ID3_GENREN_INFO;j++) id3_info->id3_genren_info[j]=0x00;  //zyz 20060223
    //for(j=0; j<ID3_GENREN_INFO;j++) id3_info->id3_genren_info[ID3_GENREN_INFO]=0x00;
  
	id3_info->id3_title_size=0;
	id3_info->id3_singer_size=0;
	id3_info->id3_album_size=0;

	id3_info->id3_ver = 0xff;
    id3_info->id3_check = 0x00; //0 is right ,else is null
 

	j=0;
  //??4096????????BUFFER??
	if(Buffer[0]=='I' && Buffer[1]=='D' && Buffer[2]=='3')
	{
	
	    id3_info->id3_size = (((Word32)Buffer[6]&0x7F)<<21)+(((Word32)Buffer[7]&0x7F)<<10)+(((Word32)Buffer[8]&0x7F)<<7)+((Word32)Buffer[9]&0x7F);	
		id3_info->id3_ver = Buffer[3];
	 
		min_size=id3_info->id3_size; 
        if(id3_info->id3_size>ID3_V2_INBUFFER_SIZE) min_size=ID3_V2_INBUFFER_SIZE;


        if(id3_info->id3_size)
				for(; i<min_size; i++){
					switch(k)
					{
						case 0:
							if(Buffer[i]=='T') {k=1;}							
							break;
						case 1:
							if(Buffer[i]=='P') {k=2; id3_find_mode=0;}		//singer
		 				    else if(Buffer[i]=='I') {k=2; id3_find_mode=1;}	//title
							else if(Buffer[i]=='A') {k=2; id3_find_mode=2;}	//album
							else if(Buffer[i]=='C') {k=6;}
							else k=0;
							break;
						case 2:
							if(id3_find_mode==0){		//singer
								if(Buffer[i]=='E'){k=3;}
								else k=0;
							}
							else if(id3_find_mode==1){	//title
								if(Buffer[i]=='T'){k=3;}
								else k=0;
							}
							else if(id3_find_mode==2){	//album
								if(Buffer[i]=='L'){k=3;}
								else k=0;
							}
							else k=0;
							break;
						case 3:
							if(id3_find_mode==0){		//singer
								if((Buffer[i]>0x30) && (Buffer[i]<0x35)){k=4; h=0;}
								else k=0;
							}
							else if(id3_find_mode==1){	//title
								if((Buffer[i]>0x30) && (Buffer[i]<0x34)){k=4; h=0;}
								else k=0;
							}
							else if(id3_find_mode==2){	//album
								if(Buffer[i]=='B'){k=4; h=0;}
								else k=0;
							}
							else k=0;
							break;
						case 4:
							if(id3_find_mode==0){		//singer
								if(h<4) singer_size +=(Word32) Buffer[i]<<(24-(h<<3));
								h++;
								if(h==7) {
									if(singer_size>1) {
									k=5;
									singer_size--;
									if(singer_size>ID3_TAG_SIZE) singer_size = ID3_TAG_SIZE-1;
									h=0;
									}
									else {h=0; k=0;}
								}
							}
							else if(id3_find_mode==1){	//title
								if(h<4) title_size+=(Word32) Buffer[i]<<(24-(h<<3));
								h++;
								if(h==7) {
									if(title_size>1) {
									k=5;
									title_size--;
									if(title_size>ID3_TAG_SIZE) title_size = ID3_TAG_SIZE-1;
									h=0;
									}
									else {h=0; k=0;} 
								}
							}
							else if(id3_find_mode==2){	//album
								if(h<4) album_size +=(Word32) Buffer[i]<<(24-(h<<3));
								h++;
								if(h==7) {
									if(album_size>1) {
										k=5;
										album_size--;
										if(album_size>ID3_TAG_SIZE) album_size = ID3_TAG_SIZE-1;
										h=0;
									}
									else {h=0; k=0;}
								}
							}
							break;
						case 5:
							if(id3_find_mode==0){		//singer
								id3_info->id3_singer[h]= Buffer[i];
								h++;

								if(singer_size==h) {
									id3_info->id3_singer_size=singer_size;
									id3_find_status++;

									if(id3_find_status==4);
									else {h=0; k=0;}
								}
							}
							else if(id3_find_mode==1){	//title
								id3_info->id3_title[h] = Buffer[i];
								h++;

								if(title_size==h) {
									id3_info->id3_title_size = title_size;
									id3_find_status++;
									if(id3_find_status==4) ;
									else {h=0; k=0;}
								}
							}
							else if(id3_find_mode==2){	//album
								id3_info->id3_album[h] = Buffer[i];
								h++;

								if(album_size==h) {
									id3_info->id3_album_size = album_size;
									id3_find_status++;
									if(id3_find_status==4) ;
									else {h=0; k=0;}
								}
							}

							break;						
						case 6:
						      if(Buffer[i]=='O') {k=7;}
						      else k=0;
						      break;
						case 7:
						      if(Buffer[i]=='N'){k=8;h=0;}
						      else k=0;
						      break;
						case 8:
						      if(h<4) liupai_size +=(Word32) Buffer[i]<<(24-(h<<3));
								h++;
								if(h==7) {
									if(liupai_size>1) {
										k=9;
										liupai_size--;
										if(liupai_size>ID3_TAG_SIZE) liupai_size = ID3_TAG_SIZE-1;
										h=0;
									}
									else {h=0; k=0;}
								}
						      break;
                         case 9:
                               id3_info->id3_genren_info[h] = Buffer[i];
								h++;

								if(liupai_size==h) {								
									id3_find_status++;
                                        if(liupai_size>2&&liupai_size<6)
                                        {
                                             liupai_size-=2;
                                             if(liupai_size==1)
                                                  num=(id3_info->id3_genren_info[1]-48);
                                             else if(liupai_size==2)
                                                  num=(id3_info->id3_genren_info[1]-48)*10+(id3_info->id3_genren_info[2]-48);
                                             else if(liupai_size==3)
                                                  num=(id3_info->id3_genren_info[1]-48)*100+(id3_info->id3_genren_info[2]-48)*10
                                                     +(id3_info->id3_genren_info[3]-48);
                                             else 
                                                  num=0;
 
                                             id3_info->id3_genren=num;

                                             //if(num==8)
						                     //{ id3_info->id3_genren_info[0]='J';
                                             //  id3_info->id3_genren_info[1]='a';
                                             //  id3_info->id3_genren_info[2]='z';
                                             //  id3_info->id3_genren_info[3]='z';
                                             // }
						                     //else if(num==13)
						                     //{ id3_info->id3_genren_info[0]='P';
                                             //  id3_info->id3_genren_info[1]='o';
                                             //  id3_info->id3_genren_info[2]='p';                              
                                             // }
						                     //else if(num==17)
						                     //{ 
						                     //  id3_info->id3_genren_info[0]='R';
                                             //  id3_info->id3_genren_info[1]='o';
                                             //  id3_info->id3_genren_info[2]='c';
                                             //  id3_info->id3_genren_info[3]='k'; 
                                             //}
                                             //else if(num==32)
						                     //{ 
						                     //  id3_info->id3_genren_info[0]='C';
                                             //  id3_info->id3_genren_info[1]='l';
                                             //  id3_info->id3_genren_info[2]='a';
                                             //  id3_info->id3_genren_info[3]='s';
                                             //  id3_info->id3_genren_info[4]='s';
                                             //  id3_info->id3_genren_info[5]='i';
                                             //  id3_info->id3_genren_info[6]='c';
                                             //  id3_info->id3_genren_info[7]='a';
                                             //  id3_info->id3_genren_info[8]='l';
                                             //}
                                             //else if(num==41)
						                     //{ 
						                     //  id3_info->id3_genren_info[0]='B';
                                             //  id3_info->id3_genren_info[1]='a';
                                             //  id3_info->id3_genren_info[2]='s';
                                             //  id3_info->id3_genren_info[3]='s';
                                             // }
                                             //else 
						                     //{
                                             //  id3_info->id3_genren_info[0]='N';
                                             //  id3_info->id3_genren_info[1]='o';
                                             //  id3_info->id3_genren_info[2]='r';
						                     // }
                                            }
					                 if(id3_find_status==4) ;
					                 else {h=0; k=0;}
								}
                              break;

						default:
							break;
					}//switch(k)

                

					if(id3_find_status==4)
                    {					
						i=min_size;                 
                     }
					
				}//for(; i<512*8; i++)
	
	}
	else {	//Find ID3V1
	    FileSeek(128, SEEK_END, hMainFile);
	    FileRead((unsigned char*)Buffer, 128,hMainFile);
		k=0;
		id3_find_mode = 0;		
	  id3_find_status = 0;
    
				for(i=0; i<ID3_V1_INBUFFER_SIZE; i++){
					switch(k)
					{
						case 0:
							if(Buffer[i]=='T') {k=1;}
							break;
						case 1:
							if(Buffer[i]=='A') {k=2;}
							else k=0;
							break;
						case 2:
							if(Buffer[i]=='G') {k=3; id3_find_mode=1; id3_info->id3_ver = 0x11;}
							else k=0;
							break;
						case 3:
							if(id3_find_status<30) id3_info->id3_title[id3_find_status] = Buffer[i];
							else if(id3_find_status<60) id3_info->id3_singer[id3_find_status-30] = Buffer[i];
							else if(id3_find_status<90) id3_info->id3_album[id3_find_status-60] = Buffer[i];
							id3_find_status++;
							if(id3_find_status==90) 
							   { 
							    id3_find_mode=3;k=4;
							    id3_info->id3_title_size=30;
							    id3_info->id3_singer_size=30;
							    id3_info->id3_album_size=30;
							    }
							break;
					  case 4: 
					     i+=4;//offset the year[4]
					     i+=30;//offset the comment[30];
							 k=5;
							 break;
						case 5:
						   id3_info->id3_genren=Buffer[i];
						   //if(id3_info->id3_genren==8)
						   //  { id3_info->id3_genren_info[0]='J';
                           //    id3_info->id3_genren_info[1]='a';
                           //    id3_info->id3_genren_info[2]='z';
                           //    id3_info->id3_genren_info[3]='z';
                           //   }
						   //else if(id3_info->id3_genren==13)
						   //  { id3_info->id3_genren_info[0]='P';
                           //    id3_info->id3_genren_info[1]='o';
                           //    id3_info->id3_genren_info[2]='p';                              
                           //   }
						   //else if(id3_info->id3_genren==17)
						   //   { 
						   //     id3_info->id3_genren_info[0]='R';
                           //     id3_info->id3_genren_info[1]='o';
                           //     id3_info->id3_genren_info[2]='c';
                           //     id3_info->id3_genren_info[3]='k'; 
                           //   }
                           //else if(id3_info->id3_genren==32)
						   //   { 
						   //     id3_info->id3_genren_info[0]='C';
                           //     id3_info->id3_genren_info[1]='l';
                           //     id3_info->id3_genren_info[2]='a';
                           //     id3_info->id3_genren_info[3]='s';
                           //     id3_info->id3_genren_info[4]='s';
                           //     id3_info->id3_genren_info[5]='i';
                           //     id3_info->id3_genren_info[6]='c';
                           //     id3_info->id3_genren_info[7]='a';
                           //     id3_info->id3_genren_info[8]='l';
                           //    }
                           //else if(id3_info->id3_genren==41)
						   //   { 
						   //     id3_info->id3_genren_info[0]='B';
                           //     id3_info->id3_genren_info[1]='a';
                           //     id3_info->id3_genren_info[2]='s';
                           //     id3_info->id3_genren_info[3]='s';
                           //    }
                           // else 
						   //    {
                           //     id3_info->id3_genren_info[0]='N';
                           //     id3_info->id3_genren_info[1]='o';
                           //     id3_info->id3_genren_info[2]='r';
						   //     }
                            k=6;
						   break;
							
						default:
							break;
					}
					
				}//for(i=0; i<512; i++) 
	}
	
	if(/*(id3_info->id3_title_size   != 0) ||*/
	    (id3_info->id3_singer_size != 0) || 
	    (id3_info->id3_album_size  !=0 )) {
	    id3_info->id3_check = 0x01;
	}
}
