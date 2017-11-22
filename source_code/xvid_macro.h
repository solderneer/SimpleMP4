/*****************************************************************************
 *
 *  XVID MPEG-4@QVGA VIDEO DECODER ON ROCKCHIP RK2606
 *	
 *	Author:
 *		Jian Huan	<jh@rock-chips.com>
 *
 *	Date:
 *		2005-11-27 10:28
 ****************************************************************************/
 
 /////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 * Constants
 ****************************************************************************/

/* comment any #defs we don't use */

#define VIDOBJ_START_CODE				0x00000100	/* ..0x0000011f  */
#define VIDOBJLAY_START_CODE			0x00000120	/* ..0x0000012f */
#define VIDOBJLAY_AR_EXTPAR				0xf
#define VISOBJSEQ_START_CODE			0x000001b0
#define VISOBJSEQ_STOP_CODE				0x000001b1	/* ??? */
#define USERDATA_START_CODE				0x000001b2
#define VISOBJ_START_CODE				0x000001b5

#define VISOBJ_TYPE_VIDEO				1
#define VIDOBJLAY_TYPE_SIMPLE			1
#define VIDOBJLAY_SHAPE_RECTANGULAR		0
#define VOP_START_CODE					0x000001b6

#define	VOPOBJSEQ_NO_DATA				0x000001b7      /*ROCKCHIP*/
/* vop coding types  */
/* intra, prediction, backward, sprite, not_coded */
#define I_VOP							0
#define P_VOP							1
#define B_VOP							2
#define	S_VOP						    3
#define	N_VOP						    4
#define	ND_VOP						    5
	
#define AVI_FILE_END					1
/*****************************************************************************
 *				 Error codes
 ****************************************************************************/
/*****************************************************************************
 *				 Error codes
 ****************************************************************************/
#define	XVID_DEC_END					1
#define	XVID_DEC_OK						0
#define	XVID_DEC_FAIL					-1
#define	XVID_ERR_FORMAT					-2
#define	XVID_ERR_HUFFMAN				-3
#define	XVID_ERR_MCBPC					-4

/*****************************************************************************
*			MACROBLOCK MODE
 ****************************************************************************/
#define MODE_INTER						0
#define MODE_INTER_Q					1
#define MODE_INTER4V					2
#define	MODE_INTRA						3
#define MODE_INTRA_Q					4
#define MODE_NOT_CODED					5

/*****************************************************************************
 *		MACROBLOCK PREDICTION SIZE
 ****************************************************************************/
#define MBPRED_SIZE						15

/*****************************************************************************
*			USEFUL MACRO
 ****************************************************************************/
 /*
#define BSWAP(a)		((a) = (((a) & 0xff) << 24)  | (((a) & 0xff00) << 8) | \
						(((a) >> 8) & 0xff00) | (((a) >> 24) & 0xff))
*/
#define BSWAP(a)		((a) = ( ( (a) & 0xffff ) << 16)  |  ( ( (a) >> 16 ) & 0xffff) )

/************************************************************************
 * Image Height and Width
  ************************************************************************/
#define		ImgX		320
#define		ImgY		240
#define		ImgSize		115200				//320*240*1.5



/***********************************************************************
 * Size of Internal Memory
 ***********************************************************************/
#define INT_MEM_SIZE		        ImgX*24			//ImgX*16*1.5
