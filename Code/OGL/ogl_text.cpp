// *************************************MODULE HEADER*********************************
// * FILENAME           : ogl_text.c
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex OGL
// * DATE STARTED       : 16:19:47 on 16th October 1999
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

//oldmac//#include <drawsprocket.h>
//oldmac//#include <quickdraw.h>
//oldmac//#include <fonts.h>
#include    "fpoly4.h"
#include "Error_handler.h"
#include "genpoly_int.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "pictures.h"
#include "OGL.h"
#include "zex_misc.h"

#include "olg_text.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
GLuint gGLCharStart;
#if OGL==1
extern AGLContext     ctx;
#endif

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *



#if OGL==1
int init_ogl_text()
{
#define MONACO_BASE 1
#define HELVETICA_BASE 128+1
#define HELVETICA_BASE_7 128+128+1

int err;
//load in the Helvetica font as a display list
	gGLCharStart = glGenLists(128);
	err=aglUseFont(ctx, kFontIDMonaco, bold, 9, 0, 128,gGLCharStart);

   err=aglGetError();
   if (err!=0) report_error ("init_ogl_text: AGL error (1).","",err);

	gGLCharStart = glGenLists(128);
	err=aglUseFont(ctx, kFontIDHelvetica, normal, 9, 0, 128,gGLCharStart);
   err=aglGetError();
   if (err!=0) report_error ("init_ogl_text: AGL error (2).","",err);

	gGLCharStart = glGenLists(128);
	err=aglUseFont(ctx, kFontIDHelvetica, extend, 9, 0, 128,gGLCharStart);

   err=aglGetError();
   if (err!=0) report_error ("init_ogl_text: AGL error (3).","",err);

    if (err!=0)
	return(-1);
	
return 0;
}
#endif


void OGL_draw_monaco_bold(const char* the_string)
{
#if OGL==1
int len,stop;


	len = 0;
	stop=0;
	while (stop==0)
	 {
	  if (the_string[len]==0)
	  stop=1;
	  else
	  len++;
	 }
	glListBase(MONACO_BASE);	
	glCallLists(len, GL_BYTE, the_string);
#else
	report_error_c("No OGL_draw_monaco_bold() on non-OGL build","",0);
#endif
}

void OGL_draw_helvetica(char* the_string)
{
#if OGL==1
int len,stop;

	len = 0;
	stop=0;
	while (stop==0)
	 {
	  if (the_string[len]==0)
	  stop=1;
	  else
	  len++;
	 }
	glListBase(HELVETICA_BASE);	
	glCallLists(len, GL_BYTE, the_string);
#else
	report_error_c("No OGL_draw_helvetica() on non-OGL build","",0);
#endif
}

void OGL_draw_helvetica_small(char* the_string)
{
#if OGL==1
int len,stop;

	len = 0;
	stop=0;
	while (stop==0)
	 {
	  if (the_string[len]==0)
	  stop=1;
	  else
	  len++;
	 }
	glListBase(HELVETICA_BASE_7);	
	glCallLists(len, GL_BYTE, the_string);
#else
	report_error_c("No OGL_draw_helvetica_small() on non-OGL build","",0);
#endif
}

//Pstring version
void OGL_draw_helveticaP(unsigned char * the_string)
{
#if OGL==1
	glListBase(HELVETICA_BASE);	
	glCallLists(the_string[0], GL_BYTE, the_string+1);
#else
	report_error_c("No OGL_draw_helveticaP() on non-OGL build","",0);
#endif
}

//void OGL_DrawStr(char *str,int the_len,int x, int y)
//{
//	  static int bitmapBase = 0;
//	  
//	  if(bitmapBase == 0)
//	  {
//	  	  bitmapBase = glGenLists(256);
//	  	  aglUseFont(aglGetCurrentContext(), kFontIDNewYork, normal, 10, 0, 256, bitmapBase);
//	  }
//	  
//	  glListBase(bitmapBase);
//	  glCallLists(the_len, GL_UNSIGNED_BYTE, (GLubyte *) str);
//}


//replacements for moveto and drawstring that do the right thing if ogl is active
static int last_x, last_y;
void ZMoveTo(int x, int y)
{
extern int OGL_active;
last_x=x;last_y=y;

#if OGL==1
if (OGL_active==1)
  {
  OGL_moveto(x,y);
  }
  else
#endif
  MoveTo(x,y);
}


void ZLineTo(int x, int y)
{
extern int OGL_active;
#if OGL==1
if (OGL_active==1)
  {
   if (single_line_clip(&last_x,&last_y,&x,&y)==0) return;
   OGL_line_no_colour( last_x, last_y, x, y);
  }
  else
#endif
  LineTo(x,y);
}

void ZDrawString_c(const char * the_string)
{
	DrawString_c(the_string);
}

void ZDrawString(const unsigned char * the_string)
{
#if OGL==1
if (OGL_active==1)
  {
   OGL_draw_helveticaP(the_string);

  }
  else
#endif
  DrawString(the_string);
}

void ZRGBForeColor (const RGBColor * RGB_Colour)
{
#if OGL==1
float temp_r,temp_b,temp_g;

if (OGL_active==1)
  {
     temp_r=RGB_Colour->red;
     temp_g=RGB_Colour->green;
     temp_b=RGB_Colour->blue;
     temp_r=temp_r/255;	//0-1
     temp_g=temp_g/255;	//0-1
     temp_b=temp_b/255;	//0-1

	glColor3d(temp_r, temp_g, temp_b);
  }
  else
  RGBForeColor(RGB_Colour);
#else
  RGBForeColor(RGB_Colour);
#endif



}

