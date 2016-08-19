// *************************************MODULE HEADER*********************************
// * FILENAME           : screen_splats.c
// * MODULE TITLE       : screen_splats.c
// * PROJECT            : Zex
// * DATE STARTED       : 06:36:10 on 14th June 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#error "This file is not in the Zex2005 build"

#include	<Resources.h>
#include <Palettes.h>
#include <QDOffscreen.h>
#include <mactypes.h>
#include <fixmath.h>
//#include <Intrinsics.h>


#include	"Error_handler.h"
#include	"splat_control.h"
#include	"splat_control_int.h"
#include	"zex_misc.h"
#include "pictures.h"
#include "screen_splats.h"


// ***********************************************************************************
// * CONSTANTS
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS
// *



int gupdate_cp;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
//¥¥¥¥¥¥¥Private bits
void	pixel_doubling_blit_8(
					const void * source_bitmap,
					int          source_rowbytes,
					int          source_width,
					int          source_height,
					void *       dest_bitmap,
					int          dest_rowbytes);




// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE              :
// | AUTHOR(s)          :
// | DATE STARTED       :
// +----------------------------------------------------------------------------------
// | DESCRIPTION:
// |
// |
// |
// |
// +----------------------------------------------------------------------------------
// | PARAM IN   : none
// |
// | PARAM OUT  : none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial                Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// |









//splats.c
void splat_320by240(char * src, int src_width, char * dest, int dest_width)
{
LS_CGrafPtr	frontBuff;
extern	      DSpContextReference		              mDisplayContext;
extern Ptr screenaddr;
extern int screenwidth,show_control_panel;
//char *dest;
//int *src;
//int dest_width;


//where are we splatting to
//    DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);
//    dest=((*(frontBuff)->portPixMap))->baseAddr;
//    dest_width=0x3fff & ((*(frontBuff)->portPixMap))->rowBytes;

//from
//src=(int *)screenaddr;

//while(vbl_now==0);	//wait for vbl
if (show_control_panel==0)
{
pixel_doubling_blit_8(src, src_width, 320, CONTROL_PANEL_TOP>>1, dest, dest_width);
splat_control_panel(dest,dest_width);	//do the cp splat as well...
}
else
pixel_doubling_blit_8(src, src_width, 320, 240, dest, dest_width);

//if(gupdate_cp==1)	//the control panel was updated
}

extern unsigned long splatProcInfo;
extern UniversalProcPtr splat_code_ptr;
//A (somehwat bad IMHO) C based pixel doubler. Attrib: Jon Watte from CSMP... does little and big endian...
void
pixel_doubling_blit_8
(const void * source_bitmap, int source_rowbytes, int source_width, int source_height, void * dest_bitmap,
int dest_rowbytes)

{
CallUniversalProc(splat_code_ptr,splatProcInfo, source_bitmap, source_rowbytes,
                                               source_width, source_height, dest_bitmap,
                                               dest_rowbytes,0);	//params for 320*240 splat

return;
//old code
////	  unsigned long oddmask =0x00ff00ff;
////	  unsigned long evenmask=0xff00ff00;
//	  union{
//	   double temp[1];
//	   unsigned int tempint[2];
//	   } myunion;
//
//	  register double temp2;
//	  while (--source_height >= 0)
//	  {
//	  	  unsigned long * src = ((unsigned long *)source_bitmap)-1;
//	  	  double * dst1 = ((double *)dest_bitmap)-1;
//	  	  double * dst2 = dst1+dest_rowbytes/8;
//	  	  int w = source_width;
//	  	  while ((w-=4) >= 0)
//	  	  {
//
//	  	  /* This loop might be optimizable by assembling the */
//	  	  /* "temp" double in another way; check profiling and */
//	  	  /* disassembly to make sure. I think this is close to */
//	  	  /* optimal, though. */
//
//	  	  /* If we're in little-endian mode, the bytes go the other way */
//#if this-should-be-replaced__LITTLE_ENDIAN__
//	  	  	  unsigned long pixx = *(++src);
//	  	  	  unsigned char * mid = (unsigned char *)&temp[0];
//	  	  	  *(mid++) = pixx;
//	  	  	  *(mid++) = pixx;
//	  	  	  pixx >>= 8;
//	  	  	  *(mid++) = pixx;
//	  	  	  *(mid++) = pixx;
//	  	  	  pixx >>= 8;
//	  	  	  *(mid++) = pixx;
//	  	  	  *(mid++) = pixx;
//	  	  	  pixx >>= 8;
//	  	  	  *(mid++) = pixx;
//	  	  	  *(mid++) = pixx;
//
//#error "not optimised on little endian"
//
//
//#else
//// new code...
////	  	  	  register unsigned long reg3;
////	  	  	  register unsigned long reg4;
////
////	  	  	  reg3=*(++src);
////	  	  	  reg4=reg3;
////	  	  	  //      dest,source,shift left,mbegin,mend --- BIG NOTE: mbegin and mend are mask positions for TARGET data, therefore represent the FINISHING positions AFTER the data is moved!
////	  	  	  __rlwimi(reg3,reg3,8,8,23);	  	  // assemble two copies of bottom byte (byte 4), plus shift third byte up to second byte position
////	  	  	  __rlwimi(reg3,reg3,8,0,7);	  	  // duplicate byte in second position and put into top position (first byte).
////	  	  	  __rlwimi(reg4,reg4,32-8,8,23);		  // 32-8 is right shift 8. top byte (first) is duplicated into second byte, plus second byte shifted into third position
////	  	  	  __rlwimi(reg4,reg4,32-8,24,31);		  // duplicate third into fourth.
////
////	  	  	  myunion.tempint[0]=reg4;
////	  	  	  myunion.tempint[1]=reg3;
//// old code...
//	  	  	  unsigned long pixx = *(++src);
//	  	  	  unsigned char * mid = (unsigned char *)&(myunion.temp[1]);
//	  	  	  *(--mid) = pixx;
//	  	  	  *(--mid) = pixx;
//	  	  	  pixx >>= 8;
//	  	  	  *(--mid) = pixx;
//	  	  	  *(--mid) = pixx;
//	  	  	  pixx >>= 8;
//	  	  	  *(--mid) = pixx;
//	  	  	  *(--mid) = pixx;
//	  	  	  pixx >>= 8;
//	  	  	  *(--mid) = pixx;
//	  	  	  *(--mid) = pixx;
//#endif
//
//	  	  /* This is the kick-ass magic part! Don't touch! */
//
//	  	  	  temp2 = myunion.temp[0];
//	  	  	  *(++dst1) = temp2;
//// following line commented out to skip alternate lines
////	  	  	  *(++dst2) = temp2;
//	  	  }
//	  	  source_bitmap = ((char *)source_bitmap)+source_rowbytes;
//	  	  dest_bitmap = ((char *)dest_bitmap)+2*dest_rowbytes;
//	  }

}





//SB 170599
//This is about 8% faster than drawsprockets best effort :)
//hardwired for 640 * 480
void splat_640by480(char * src, int src_width, char * dest, int dest_width)
{
LS_CGrafPtr	frontBuff;
extern	      DSpContextReference		              mDisplayContext;
extern Ptr screenaddr;
//extern int screenwidth;
char *src_now,*dest_now;
Float64 *src_float,*dest_float;
int line_count,line,pix;


//where are we splatting to
//    DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);
//    dest=((*(frontBuff)->portPixMap))->baseAddr;
//    dest_width=0x3fff & ((*(frontBuff)->portPixMap))->rowBytes;

//from
//src=screenaddr;

//dest_now=dest;
//src_now=src;
src_float=(Float64 *) src;
dest_float=(Float64 *) dest;

//how many lines
line_count=480;

//while(vbl_now==0);	//wait for vbl

//the below code has been tweaked so optimiser outputs
//mtctr 640/8
//lfdu
//stfdu
//bdnz
//cmp 480
//add
//add
//ble
//for the inner loop
//so don't touch.
//on my 8500 this is twice as fast as copybits
for (line=0;line<line_count;line++)
    {
     for (pix=0;pix<640/8;pix++)
         *dest_float++=*src_float++;

     src = src+src_width;	//point to next src line;
     dest=dest+dest_width;

     src_float=(Float64 *) src;
     dest_float=(Float64 *) dest;

    }	//do next line
//if(gupdate_cp==1)	//the control panel was updated
//splat_control_panel();	//do the cp splat as well...
//if(gupdate_cp==1)	//the control panel was updated
//splat_control_panel();	//do the cp splat as well...

}



//SB 170599
//This is about 8% faster than drawsprockets best effort :)
//hardwired for 640 * 480
void splat_control_panel(Ptr dest_vram, int dest_width)
{
LS_CGrafPtr	frontBuff;
extern	      DSpContextReference		              mDisplayContext;
extern Ptr screenaddr;
extern int screenwidth;
char *src,*dest,*src_now,*dest_now;
Float64 *src_float,*dest_float;
int line_count,line,pix;


//where are we splatting to
//    DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);
//    dest=((*(frontBuff)->portPixMap))->baseAddr;
//    dest_width=0x3fff & ((*(frontBuff)->portPixMap))->rowBytes;
dest=dest_vram;

//from
src=screenaddr;
src=src+(screenwidth*CONTROL_PANEL_TOP);
dest=dest+(dest_width*CONTROL_PANEL_TOP);
//dest_now=dest;
//src_now=src;
src_float=(Float64 *) src;
dest_float=(Float64 *) dest;

//how many lines
line_count=480-CONTROL_PANEL_TOP;
//the below code has been tweaked so optimiser outputs
//mtctr 640/8
//lfdu
//stfdu
//bdnz
//cmp 480
//add
//add
//ble
//for the inner loop
//so don't touch.
//on my 8500 this is twice as fast as copybits
for (line=0;line<line_count;line++)
    {
     for (pix=0;pix<640/8;pix++)
         *dest_float++=*src_float++;

     src = src+screenwidth;	//point to next src line;
     dest=dest+dest_width;

     src_float=(Float64 *) src;
     dest_float=(Float64 *) dest;

    }	//do next line

}




