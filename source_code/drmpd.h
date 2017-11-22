/*
************************************************************************************************************************
*
*  Copyright (C),2006, Fuzhou Rockchip Co.,Ltd.
*
*  File name :     drmpd.h
*  Author:         hwg, zsx, zym
*  Description:     
*  Remark:         
*                       
*  History:        
*           <author>      <time>     <version>       <desc>
*                         06/8/21       1.0           
*
************************************************************************************************************************
*/

#ifndef __DRMPD_H__
#define __DRMPD_H__

//#include "string.h"
//#include "memory.h"
//#include "assert.h"
#include "drmtype.h"

/*
 *     DEFINES
 */

#define IN
#define OUT

#define SIXTEEN_BIT_ADDRESSING 1
#define DEBUG 0

#define Null    ((void *)0)

#define DRM_SUCCESS        0
#define BUFFER_TOO_SMALL   1
#define DRM_NOTINITIALIZED 3


#define RC4_TABLESIZE      256
#define DES_KEYSIZE		     8
#define SHA_BLOCK_SIZE     64	/*Sha operates on 64 BYTE blocks */
#define SHA_DIGEST_LEN     20
#define DES_BLOCKLEN	     8
#define DES_DECRYPT        0

//#define COPYRIGHT_STRING_OFFSET 6
//#define COPYRIGHT_STRING_CHUNK_LEN 5


// license right
#define CHECK_NONSDMI_LIC   1
#define CHECK_SDMI_LIC         2
#define CHECK_ALL_LICENSE    3

#define APPSEC_LEN 4
#define DATE_LEN     4
#define RIGHTS_LEN 4


/*
 *     MACROS
 */
#if SIXTEEN_BIT_ADDRESSING
#define __CB_DECL(x) ((x)/2 + ((x) & 1))
#define CB_NATIVE_BYTE 2
#define GET_BYTE(pb,ib)              DRM_16B_Read8BitByte((pb),(ib))
#define PUT_BYTE(pb,ib,b)            DRM_16B_Write8BitByte((pb),(ib),(b))
#define STRLEN(x) ZSP_DRM_strlen(x)
#define MEMCMP(x, y, l) ZSP_DRM_memcmp(x, y, l)
#define MEMCPY(to, from, l) ZSP_DRM_memcpy(to, from, l)


#else
#define __CB_DECL(x) (x)
#define CB_NATIVE_BYTE 1
//#define UWORD32_TO_UWORD64(x) (UWORD64)(x)
//#define SIZEOF(x) sizeof(x)  
#define STRLEN(x) strlen(x)
#define MEMCMP(x, y, l) memcmp(x, y, l)
#define MEMCPY(to, from, l) memcpy(to, from, l)
#define GET_BYTE(pb,ib)             (pb)[(ib)]
#define PUT_BYTE(pb,ib,b)           (pb)[(ib)]=(b)
#define DRM_BYT_CopyBytes(to,tooffset,from,fromoffset,count)   \
                      memcpy(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))                      
#endif


//#define BYTE2LITTLEENDIAN(dword, byte)                         \
//                      DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),0,4)                                
//#define BYTES_TO_DWORD BYTE2LITTLEENDIAN
//#define DWORD2LITTLEENDIAN( byte, dword )                      \
//                      DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,4)
//#define DWORD_TO_BYTES DWORD2LITTLEENDIAN
#define WORDSWAP(d) ( ( ( (d) >> 16 ) | ( (d) << 16 ) & 0xFFFF0000) )
//#define WORDSWAP(d) ( ( ( (d) >> 16 ) | ( (d) << 16 ) ) )

/* pairwise independent function and summing step */
#define MP_C_STEP_P(pbData,L1, L2, L3, L4, L5, L6, t, sum) \
{\
    UWORD32   dwTemp = 0;\
    dwTemp = *((DRM_DWORD*)pbData);\
    pbData += 4/CB_NATIVE_BYTE;\
    t += dwTemp; \
    t *= L1; \
    t = WORDSWAP(t); \
    t *= L2; \
    t = WORDSWAP(t); \
    t *= L3; \
    t = WORDSWAP(t); \
    t *= L4; \
    t = WORDSWAP(t); \
    t *= L5; \
    t += L6; \
    sum += t; \
}

#define MP_C_STEP_P__(pbData, flag, L1, L2, L3, L4, L5, L6, t, sum) \
{\
    UWORD32   dwTemp = 0;\
    DRM_BYT_CopyBytes( (DRM_BYTE*)&dwTemp, 0, pbData, flag, 4 );\
    pbData += 4/CB_NATIVE_BYTE;\
    t += dwTemp; \
    t *= L1; \
    t = WORDSWAP(t); \
    t *= L2; \
    t = WORDSWAP(t); \
    t *= L3; \
    t = WORDSWAP(t); \
    t *= L4; \
    t = WORDSWAP(t); \
    t *= L5; \
    t += L6; \
    sum += t; \
}

#define MP_C_STEP(Data,L1, L2, L3, L4, L5, L6, t, sum) \
    t += Data; \
    t *= L1; \
    t = WORDSWAP(t); \
    t *= L2; \
    t = WORDSWAP(t); \
    t *= L3; \
    t = WORDSWAP(t); \
    t *= L4; \
    t = WORDSWAP(t); \
    t *= L5; \
    t += L6; \
    sum += t;	

/* step to reverse action of multiply-&-swap rounds */
#define INV_STEP_C(iL1, iL2, iL3, iL4, iL5) \
    tmp *= iL5; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL4; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL3; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL2; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL1;

//void memcpy(
//       OUT   void  *dst,
//    IN const void  *src,
//    IN       UWORD32  count )
//{
//    while( count > 0 )
//    {
//        *(DRM_BYTE*)dst = *(DRM_BYTE*)src;
//        dst = (char *)dst + 1;
//        src = (char *)src + 1;
//        count--;
//    }
//}

//#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define ROTATE_LEFT(x, n)  rotate_left(x, n)

//#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))
/*
#define F(l,r,S) { \
    work = ((DRM_DWORD *)pTable)[S  ]; \
    zork = ((DRM_DWORD *)pTable)[S+1];  \
    work ^= r; \
    zork ^= r; \
    zork = ((zork<<28) & 0xF0000000) | (zork>>4); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[0] + __CB_DECL((work      & 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[1] + __CB_DECL((zork      & 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[2] + __CB_DECL(((work>> 8)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[3] + __CB_DECL(((zork>> 8)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[4] + __CB_DECL(((work>>16)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[5] + __CB_DECL(((zork>>16)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[6] + __CB_DECL(((work>>24)& 0xfc))); \
    l ^= *(DRM_DWORD *)((DRM_BYTE*)_DRM_Spbox[7] + __CB_DECL(((zork>>24)& 0xfc))); }
*/
#define PERM_OP(a,b,t,n,m) ((t)=((((a)>>(n))^(b))&(m)),\
    (b)^=(t),\
    (a)^=((t)<<(n)))

#define HPERM_OP(a,t,n,m) ((t)=((((a)<<(16-(n)))^(a))&(m)),\
    (a)=(a)^(t)^(t>>(16-(n))))


/*
 *     DATA
 */

//extern const DRM_BYTE PMSECRET_SEG0__[5];
//extern const DRM_BYTE pszCopyrightString[22]; 

//extern UWORD16 g_pmid[__CB_DECL(16)];
//extern UWORD32 unknown2[32];
extern const DRM_DWORD _DRM_Spbox[8][64];
extern const DRM_DWORD _DRM_Sel[8][64];


/*
 *     STRUCTURES
 */

typedef struct _CBCKey 
{
    UWORD32 a1, b1, c1, d1, e1, f1, a2, b2, c2, d2, e2, f2;
} DRM_CBCKey;		// 24 WORD16

typedef struct __tagCBCState
{
    UWORD32 sum,t;
    DRM_BYTE buf[__CB_DECL(8)];
    LENGTH_TYPE dwBufLen;
} DRM_CBCState;		// 10 WORD16

typedef struct tag_RC4_KEYSTRUCT
{
	DRM_BYTE S[__CB_DECL(256)];     /* State table */
	DRM_BYTE i, j;        /* Indices */
} RC4_KEYSTRUCT;	// 129 WORD16

typedef struct _destable
{
	UWORD32 keytab[16][2];		// 64 WORD16
	DRM_BYTE mem2[__CB_DECL(16)];				// 8 WORD16
} DESTable;			// 72 WORD16

typedef struct __tagSHA_CONTEXT
{	
	DRM_BYTE id[__CB_DECL(24)];				// 12 WORD16
	UWORD32 ABCDE[5];			// 10 WORD16
	UWORD32 dwHighByteCount;	// 2 WORD16
	UWORD32 dwLowByteCount;		// 2 WORD16
	DRM_BYTE  bTempBuffer[__CB_DECL(64)];		// 32 WORD16
} SHA_CONTEXT;		// 58 WORD16

typedef struct tag_LIC_Data
{
	DRM_BYTE KID[__CB_DECL(25)];    			// 25 byte
	DRM_BYTE PMContentKey[__CB_DECL(16)];       // 16 byte
	DRM_BYTE rights[__CB_DECL(4)]; 			// 2 WORD16
	DRM_BYTE appSec[__CB_DECL(4)];				// 2 WORD16
	DRM_BYTE expiryDate[__CB_DECL(4)];			// 2 WORD16
	DRM_BYTE mem4;            		// 11 byte	好像没用
	UWORD16 mem5;		// 好像没用
	UWORD16 cbContentKey;
	UWORD16 mem7;		// 好像没用
	UWORD16 mem8;
	UWORD16 mem9;
} LIC_Data;		// 32 WORD16

typedef struct tag_CIPHER_CONTEXT
{	// 与DRM_CIPHER_CONTEXT类似
	DRM_CBCKey m_mackey,	// 24 WORD16
               m_invmackey;	// 24 WORD16	#0x30
    DRM_CBCState m_cbcstate;// 10 WORD16	#0x60
	UWORD32 m_rc4key[2];	// 4 WORD16		#0x74
	DRM_BYTE m_shaOut[__CB_DECL(20)];		// 10 WORD16	#0x7c
	DRM_BYTE m_fInited;			// 1 Byte		#0x90
	DRM_BYTE m_fDecryptInited;	// 1 Byte		#0x91
//	WORD16 mem31;			// 1 WORD16		可能没用
	LENGTH_TYPE m_cbPacket;		// 2 WORD16		#0x94
	LENGTH_TYPE m_iPacket;		// 2 WORD16		#0x98
	UWORD32 m_desS1[2],		// 4 WORD16		#0x9c
			m_desS2[2];		// 4 WORD16		#0xa4
	RC4_KEYSTRUCT m_rc4ks;	// 129 WORD16	#0xac
//	WORD16 mem341;   		// 1 WORD16		可能没用
	DESTable m_destable;    // 72 WORD16	#0x1b0
//	UWORD32 mem342;			// 2 WORD16		#0x240	可能没用
	SHA_CONTEXT m_shadata;	// 58 WORD16	#0x244
	UWORD32 mem4[24];		// 48 WORD16	#0x2b8	to generate mackey
//	WORD16 mem5[116];		// 116 WORD16	#0x318	好像没用
} CIPHER_CONTEXT;		// 512 WORD16 - 116 WORD16

typedef struct tag_structX_unknown3
{
	DRM_BYTE mem11;				// 1 DRM_BYTE		errorResult
//	DRM_BYTE mem12;				// 1 DRM_BYTE		可能没用
//	WORD16 mem13;			// 1 WORD16		可能没用
	LIC_Data ld;			// 32 WORD16	#0x4
	DRM_BYTE *pbPMID;			// 2 WORD16		#0x44
	LENGTH_TYPE cbPMID;			// 2 WORD16		#0x48
	UWORD32 mem5;			// 2 WORD16		#0x4c lic中无信息位数54
	UWORD32 mem6;			// 2 WORD16		#0x50 lic counter
	UWORD32 mem7;			// 2 WORD16		#0x54
	UWORD32 mem8;			// 2 WORD16		#0x58
	UWORD32 mem9;			// 2 WORD16		#0x5c
} structX_unknown3;		// 48 WORD16

typedef struct tag_structX_unknown2
{
//	WORD16 mem1[34];		// 34 WORD16	可能没用
	structX_unknown3 mem2;	// 48 WORD16
} structX_unknown2;		// 82 WORD16

typedef struct tag_struct_DRM_state
{
	DRM_BYTE m_DecryptInited;		// 1 DRM_BYTE
//	DRM_BYTE mem12;					// 1 DRM_BYTE	可能没用
//	WORD16 mem13;				// 1 WORD16	可能没用
	CIPHER_CONTEXT mem2;		// 512 WORD16	#0x4
	structX_unknown2 mem3;		// 82 WORD16	#0x404
//	DRM_BYTE* mem4;					// 2 WORD16		#0x4a8
} struct_DRM_state;

typedef struct tag_PMLICENSE
{
	UWORD32 mem1;		// 2 WORD16
	UWORD32 mem2;		// 2 WORD16
	LIC_Data ld;		// 32 WORD16 
} PMLICENSE;		// 36 WORD16

//typedef struct tagLD
//{
//	DRM_BYTE KID[32];
//	DRM_BYTE member1[9];
//	DRM_BYTE rights[4];
//	DRM_BYTE appSec[4];
//	DRM_BYTE expiryDate[4];
//	DRM_BYTE member2[11];
//} LD;
//
//typedef struct tagPMLICENSE
//{
//	DRM_BYTE member1[8];
//	LD ld;	  
//} PMLICENSE;



/*
 *     FUNCTIONS
 */
#if SIXTEEN_BIT_ADDRESSING
void F(DRM_DWORD* pl, DRM_DWORD* pr, COUNT_TYPE S, DRM_DWORD* pwork, 
	DRM_DWORD* pzork, DRM_DWORD* pTable); 
UWORD32 rotate_left(UWORD32 a, UWORD16 n);
extern void ZSP_swapWORD(UWORD16 *pdata, LENGTH_TYPE len);
void memcpyDWORD(UWORD32* pto, UWORD32* pfrom, LENGTH_TYPE len);
LENGTH_TYPE ZSP_DRM_strlen(DRM_BYTE *string);
UWORD16 ZSP_DRM_memcmp(DRM_BYTE* p1, DRM_BYTE* p2, LENGTH_TYPE len);
void ZSP_DRM_memcpy(DRM_BYTE* pto, DRM_BYTE* pfrom, LENGTH_TYPE len);
void DRM_BYT_CopyBytes( 
       OUT   void 	*f_pbTo, 
    IN       COUNT_TYPE f_ibTo, 
    IN const void 	*f_pbFrom,
    IN       COUNT_TYPE f_ibFrom,
    IN       LENGTH_TYPE f_cb );
DRM_BYTE DRM_16B_Read8BitByte(
    IN  const void	  *f_pv,
    IN        COUNT_TYPE  f_ib);
void DRM_16B_Write8BitByte(
    OUT void        *f_pv,
    IN  COUNT_TYPE        f_ib,
    IN  DRM_BYTE  		f_mb);
#endif

//// drmpd
DRM_RESULT  CDrmPD_Init__(void* pDRM_state);
DRM_RESULT  CDrmPD_InitDecrypt__( void* pDRM_state,
                            PMLICENSE* pmlic,
                            DRM_BYTE* pPMID,
                            LENGTH_TYPE cbPMID,
                            DRM_BYTE* pLicData,
                            UWORD32* pOffset,
                            UWORD32* pActual);
                           
DRM_RESULT  CDrmPD_Decrypt__ (/*void* pDRM_state, */DRM_BYTE* pbBuffer, LENGTH_TYPE cbBuffer);  
DRM_RESULT  CDrmPD_InitPacket__(void* pDRM_state, DRM_BYTE* pLast15, UWORD16 cbPayloadSize);                         

//// pdlicstr
//void    SetSecret_1of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_2of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_3of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_4of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_5of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_6of7__(struct_DRM_state* pin);
//DRM_RESULT  SetSecret_7of7__(struct_DRM_state* pin);
UWORD32 hash__(DRM_BYTE* pin);

DRM_RESULT  CPDLicStore_Init__(structX_unknown3* pin);
DRM_RESULT MatchLicense__(LIC_Data* pld, 
					DRM_BYTE* pLicData);
DRM_RESULT PMIDToPMKey__(struct_DRM_state* pDRMState,
//				   UWORD32 temp1,
				   UWORD16 temp,
				   SHA_CONTEXT* pContextSHA,
				   DRM_BYTE* pPMID,
				   LENGTH_TYPE cbPMID,
				   DRM_BYTE* pLicData,
				   DRM_BYTE* pbPMKey,
				   LENGTH_TYPE cbPMKey);
void FlipBits__(DRM_BYTE* pbIn, LENGTH_TYPE cbIn);
DRM_RESULT PMContentKeyToContentKey__(struct_DRM_state* pDRMState,
							    DRM_BYTE* pbPMKey,
							    LENGTH_TYPE cbKey,
							    DRM_BYTE* pbKey);

DRM_RESULT  CPDLicStore_Lookup__(
              				structX_unknown3* pin,
							struct_DRM_state* pDRMState,
							DRM_BYTE* pPMID,
							LENGTH_TYPE cbPMID,
							LIC_Data* pld,
							PMLICENSE* pmlic,
							DRM_BYTE* pLicData,
							UWORD32* pOffset,
							UWORD32* pActual,
							DRM_BYTE* pbKey);
//// dess
void  desSkey__(
    IN OUT DESTable      *pTable,    
    IN OUT DRM_BYTE      *p, 
    IN     DRM_UINT      c );
void desSkey_LS__(struct_DRM_state* pDRMState, 
				DESTable* pdestable, 
				DRM_BYTE* pbKey, 
				LENGTH_TYPE cbKey);
void desS__(
    DRM_BYTE*      rgbOut,//8byte 
    DRM_BYTE*      p,//可能为8byte            
    DESTable*      pTable,
    DRM_INT        op );
//// xsum
DRM_RESULT checksum2__(struct_DRM_state* pDRMState,
			     DRM_BYTE* pbPMKey,
			     LENGTH_TYPE cbPMKey,
			     DRM_BYTE* pLicData,
			     LENGTH_TYPE licDataLen,	// 实际上固定为64
			     DRM_BYTE* pbCheckSum);

//// so1
//DRM_BYTE    SO1_GetB0__(void);
//DRM_BYTE    SO1_GetB1__(void);
//DRM_BYTE    SO1_GetB2__(void);
//DRM_BYTE    SO1_GetB3__(void);
//DRM_BYTE    SO1_GetB4__(void);

//// so2
//UWORD32 SO2_BiggerAction__(UWORD32* pin);
//UWORD32 SO2_Action__(UWORD32 in);

//// so3
//COUNT_TYPE  SO3_CheapRec__(COUNT_TYPE in);
//COUNT_TYPE  SO3_ErsatzRec__(COUNT_TYPE in);


//// so4
//DRM_BYTE    SO4_Random__(UWORD32* pin);
//UWORD16 SO4_scp_rand__(UWORD32* pin);

//// soalter
//void    SOAlter_Init__(DRM_BYTE* pin);
//void    SOAlter_Init2__(DRM_BYTE* pin, DRM_BYTE a1, DRM_BYTE a2);
//DRM_BYTE    SOAlter_Alter__(DRM_BYTE* pin, DRM_BYTE a1);

//// sobuf
//void    SOBuf_Init__(DRM_BYTE** ppin);
//DRM_RESULT  SOBuf_SetByte__(DRM_BYTE** ppin, COUNT_TYPE index, DRM_BYTE val);
//DRM_RESULT  SOBuf_GetByte__(DRM_BYTE** ppin, COUNT_TYPE index, DRM_BYTE* out);


///// blackbox
//void    DRMInit__(CIPHER_CONTEXT* pin);
DRM_RESULT  DRMDecrypt__(CIPHER_CONTEXT* pin, LENGTH_TYPE cbBuffer, DRM_BYTE* pbBuffer);
DRM_RESULT  DRMInitPacket__(CIPHER_CONTEXT* pin, UWORD16 cbPayloadSize, DRM_BYTE* pLast15);
UWORD32 AllignedDWORD__(DRM_BYTE* pin);
DRM_RESULT DRMKeySetup__(CIPHER_CONTEXT* pContext, DRM_BYTE cbData, DRM_BYTE* pbData);

//// cbc64ws4
void CBC64WS4_asm__(UWORD32 *pbData,
				  LENGTH_TYPE cBlocks,
				  UWORD32 rgdwKeys[2],
				  DRM_CBCKey *pCBCKey);
//void egcd__(UWORD64 a, UWORD64 b, UWORD64* p1, UWORD64* p2, UWORD64* p3);
void CBC64Init__( DRM_CBCKey *cbckey, DRM_CBCState *cbcstate, UWORD32 *pKey );
void CBC64InvKey__( DRM_CBCKey *cbckey, DRM_CBCKey *cbcInvKey );
void   CBC64Update__( 
     IN      DRM_CBCKey *key, 
     IN  OUT DRM_CBCState *cbcstate,
     IN      LENGTH_TYPE cbData, 
     IN      DRM_BYTE *pbData );
    
UWORD32 CBC64Finalize__( DRM_CBCKey *key, DRM_CBCState *cbcstate, UWORD32 *pKey2 );
UWORD32 inv32__( UWORD32 n );
UWORD32 CBC64Invert__( DRM_CBCKey *key, DRM_CBCKey *ikey, 
                      UWORD32 MacA1,   UWORD32 MacA2,
                      UWORD32 MacB1,   UWORD32 MacB2,    UWORD32 *pInvKey2 );    
void    CBC64InitState__( DRM_CBCState *cbcstate );

//// desport
void  des__(
          DRM_BYTE  rgbOut[__CB_DECL(DES_BLOCKLEN)],
    const DRM_BYTE  rgbIn [__CB_DECL(DES_BLOCKLEN)],          
          DESTable *pTable,
          DRM_INT   op );   
            
//// deskey
void deskey__(
    OUT       DESTable *pTable,
    IN  const DRM_BYTE  rgbKey[__CB_DECL(DES_KEYSIZE)] );

//// msrc4
extern void  rc4__(
    IN OUT RC4_KEYSTRUCT *pKS,
    IN     LENGTH_TYPE       cbBuffer,
    IN OUT DRM_BYTE      	 *pbBuffer );
    
void  rc4_key__(
        OUT   RC4_KEYSTRUCT  *pKS,
    IN        LENGTH_TYPE       cbKey,
    IN  const DRM_BYTE       *pbKey );

//// sha
void  A_SHAInit__(
    IN OUT SHA_CONTEXT* pShaContext );
    
void  A_SHAUpdate__(
    IN OUT   SHA_CONTEXT *pShaContext,  
    IN const DRM_BYTE    *pbData,       
    IN       LENGTH_TYPE    cbData );

void  SHA_UpdateOffset__(
    IN const DRM_BYTE    *pbData,
    IN       COUNT_TYPE    ibData,
    IN       LENGTH_TYPE    cbData,
    IN OUT   SHA_CONTEXT *pShaContext );

static void SHATransform__(
    IN       DRM_DWORD ABCDE[5],
    IN const DRM_BYTE  bInput[__CB_DECL(SHA_BLOCK_SIZE)] );

static void _GetDRMDWORD(
    OUT      UWORD32 *dwTo,
    IN       LENGTH_TYPE  dwCount,
    IN const DRM_BYTE  *bIn );
    
void  _PackDRMDWORD(
    IN const DRM_DWORD *dwFrom,
    IN       LENGTH_TYPE  dwCount, 
    OUT      DRM_BYTE  *bOut);

static WORD32 _ft(WORD32 b, WORD32 c, WORD32 d, COUNT_TYPE t);

static UWORD32 _K(COUNT_TYPE t);

void A_SHAFinal__(
    IN  SHA_CONTEXT *pShaContext,
    OUT DRM_BYTE     rgbDigest[__CB_DECL(SHA_DIGEST_LEN)] );

       
    

#endif

