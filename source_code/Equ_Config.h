
//#define     WMA_DECODE_EQ_NORMAL        0     // 0         no eq  
//#define     WMA_DECODE_EQ_3D            11     //  1        3d     
//#define     WMA_DECODE_EQ_ROCK          1     //  2        rock   
//#define     WMA_DECODE_EQ_POP           2     // 3         pop    
//#define     WMA_DECODE_EQ_CLASSIC       3     // 4         classic
//#define     WMA_DECODE_EQ_BASS          4     //  5        bass   
//#define     WMA_DECODE_EQ_JAZZ          5     //  6        jazz   
//#define     WMA_DECODE_EQ_METAL         6     //  7        metal  
//#define     WMA_DECODE_EQ_DANCE         7     //  8        dance  
//#define     WMA_DECODE_EQ_PARTY         8     //  9        party  
//#define     WMA_DECODE_EQ_CLUB         9     //  10        club  
//#define     WMA_DECODE_EQ_USER 		   10        //   11       user  


//#define		NORMAL			0
//#define		ROCK			1
//#define		POP             2
//#define		CLASSIC			3
//#define		JAZZ			5
//#define		BASS			4
//#define     METAL           6   
//#define     DANCE           7   
//#define     PARTY           8   
//#define     CLUB            9   
//#define		USER            10


//typedef struct	_tWMAEQMode{
//	short	MODE;
//	short	Usr_Def_Tab[5];
//}tWMAEQMode;
//extern tWMAEQMode WMAEQMode;

extern long		Gain[11];
extern long		NormalGain[11];
extern long 	RockGain[11];
extern long 	PopGain[11];
extern long 	ClassicGain[11];
extern long 	JazzGain[11];
extern long 	BassGain[11];
extern long 	MetalGain[11];
extern long 	DanceGain[11];
extern long 	PartyGain[11];
extern long 	ClubGain[11];
extern long 	Usr_Def1_Gain[5];	
extern long 	Usr_Def2_Gain[5];
extern long     GainMapper[19];

