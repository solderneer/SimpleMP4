 
 
 
 .data
 .global    _WMATableRunDram
  .section ".WMATableRunDram","aw"
 //_WMATableRunDram: //2006-11-6 11:43 define in file "WmaDecodeGlobal.h" 
 
   /* add by hwg, 06/10/16 to save huffuman space */
  //  .wspace   0x25d0
   
//   .wspace   0x1c60                /* the space must > 1c5f, otherwise may cause noise, we should aligned memory */


