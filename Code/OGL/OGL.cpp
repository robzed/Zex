// *************************************MODULE HEADER*********************************
// * FILENAME           : OCL.c
// * MODULE TITLE       : 
// * PROJECT            : Zex Open GL
// * DATE STARTED       : 04:23:51 on 9th August 1999
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// * Zex Open GL Interface functions. Requires OGL to be 1 in OGL.ext
// * else the functions are not compiled, and our renderer is used instead.
// * Won't work with OGL versions belwo 1.2 because of glColor4d?????
// * Currently ONLY does Lambert and Gourad polys - no TM polys or DLP stuff.
// * Note to Stu: Find a spare Sunday to finish it.
// * 'Nuther note: OGL's RGB332 (or maybe 233, whatever) format is the same as Apple's 8 bit clut??? Rob?
// * So if we set interp indexed mode we should be able to use our exapnded RCZd's as-is?
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
//oldmac//#include <NumberFormatting.h>
#include "3d_structures_def.h"
#include    "fpoly4.h"
#include "genpoly_int.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "pictures.h"
#include "OGL.h"
#include "zex_misc.h"
#include "olg_text.h"
#include "Error_handler.h"
#include "texture_management.h"
#include "input_control.h"

// ***********************************************************************************
// * CONSTANTS 
// *

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
#if OGL==1
AGLContext     ctx;
#endif
int OGL_active=0;	//global - intially false

static Point start_mouse_xy;	//when we enable the mouse where is it so we can centre the thing
static int mouse_on;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

#if OGL==1
int	SufficientVideoMemory( AGLPixelFormat fmt );
#endif
void cut_out (LSRAW* source, LSRAW* destination, int source_start_x, int source_start_y);
int kill_tile(int tile_id);


void OGL_plot(int x1, int y1, int colour)
{
#if OGL==1
int red,green,blue;
float temp_r,temp_b,temp_g,x,y;

convert_to_rgb(colour, &red, &green, &blue);
     temp_r=red;
     temp_g=green;
     temp_b=blue;
     temp_r=temp_r/255;	//0-1
     temp_g=temp_g/255;	//0-1
     temp_b=temp_b/255;	//0-1

//   light=light/32000;
   
//   temp_r=r*light;
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

//   temp_g=g*light;
   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

//   temp_b=b*light;
   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
//   glLineWidth(2.0);
	glColor3d(temp_r, temp_g, temp_b);
//	glColor3d(1.0, 1.0, 1.0);

    glBegin(GL_POINTS);
          x=x1;
          x-=320;
          x/=320;

          y=480-y1;
          y-=240;
          y /=240;
          

	glVertex2d(x, y);

    glEnd();

#else
	report_error_c("No OGL_plot() on non-OGL build","",0);
	
#endif
}



void OGL_line(int x1, int y1, int x3, int y3, int colour)
{
#if OGL==1
int red,green,blue;
int startx, starty, endx, endy,clip,dx,x2,dy,xstep,ystep,y2,e;
float temp_r,temp_b,temp_g,x,y;

//clip=0;
//
////check for clipping
//if (x1<0 || x1>639) clip=1;
//if (x3<0 || x3>639) clip=1;
//if (y1<0 || y1>479) clip=1;
//if (y3<0 || y3>479) clip=1;
//
//startx=starty=endx=endy=-1;
//
//if (clip==1)
//{
////calc next x and y's
////our lines are short so we'll just pretend to draw it
//dx=x3-x1; dy=y3-y1;
//x2=x1; y2=y1;
//
//if (dx<0)
//{
// dx=-dx;
// xstep=-1;
//}
//else
//xstep=1;
//
//if (dy<0)
//{
// dy=-dy;
// ystep=-1;
//}
//else
//ystep=1;
//
//if (dy==0) e=-1;
//   else e=0;
//
//clip=0;
//while ((x2!=x3 || y2!=y3))
// {
////  clip=dlp_plot(x2,y2,colour);
//  if (startx==-1)   { if (x2>-1 && x2<640) startx=x2; }
//  if (starty==-1)   { if (y2>-1 && y2<480) starty=y2; }
//  
//  if (x2>-1 && x2<640) endx=x2;
//  if (y2>-1 && y2<480) endy=y2;
//  
//  
//  if (e>=0)
//  {
//    y2=y2+ystep;
//    e=e-dx;
//  }
//  else
//  {
//    x2=x2+xstep;
//    e=e+dy;
//  }
// }	//end of while
//
//x1=startx; x3=endx; y1=starty; y3=endy;
//}

convert_to_rgb(colour, &red, &green, &blue);
     temp_r=red;
     temp_g=green;
     temp_b=blue;
     temp_r=temp_r/255;	//0-1
     temp_g=temp_g/255;	//0-1
     temp_b=temp_b/255;	//0-1

	glColor3d(temp_r, temp_g, temp_b);
//	glColor3d(1.0, 1.0, 1.0);
//    glEnable(GL_LINE_SMOOTH);
//    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glLineWidth(1.5);
//   
    glBegin(GL_LINES);
          x=x1;
          x-=320;
          x/=320;

          y=480-y1;
          y-=240;
          y /=240;
          

	glVertex2d(x, y);
          x=x3;
          x-=320;
          x/=320;

          y=480-y3;
          y-=240;
          y /=240;
          
	glVertex2d(x,y);

    glEnd();

//   glDisable(GL_LINE_SMOOTH);
#else
	report_error_c("No OGL_line() on non-OGL build","",0);

#endif
}


void OGL_line_no_colour(int x1, int y1, int x3, int y3)
{
#if OGL==1
int red,green,blue;
int startx, starty, endx, endy,clip,dx,x2,dy,xstep,ystep,y2,e;
float temp_r,temp_b,temp_g,x,y;

//clip=0;
//
////check for clipping
//if (x1<0 || x1>639) clip=1;
//if (x3<0 || x3>639) clip=1;
//if (y1<0 || y1>479) clip=1;
//if (y3<0 || y3>479) clip=1;
//
//startx=starty=endx=endy=-1;
//
//if (clip==1)
//{
////calc next x and y's
////our lines are short so we'll just pretend to draw it
//dx=x3-x1; dy=y3-y1;
//x2=x1; y2=y1;
//
//if (dx<0)
//{
// dx=-dx;
// xstep=-1;
//}
//else
//xstep=1;
//
//if (dy<0)
//{
// dy=-dy;
// ystep=-1;
//}
//else
//ystep=1;
//
//if (dy==0) e=-1;
//   else e=0;
//
//clip=0;
//while ((x2!=x3 || y2!=y3))
// {
////  clip=dlp_plot(x2,y2,colour);
//  if (startx==-1)   { if (x2>-1 && x2<640) startx=x2; }
//  if (starty==-1)   { if (y2>-1 && y2<480) starty=y2; }
//  
//  if (x2>-1 && x2<640) endx=x2;
//  if (y2>-1 && y2<480) endy=y2;
//  
//  
//  if (e>=0)
//  {
//    y2=y2+ystep;
//    e=e-dx;
//  }
//  else
//  {
//    x2=x2+xstep;
//    e=e+dy;
//  }
// }	//end of while
//
//x1=startx; x3=endx; y1=starty; y3=endy;
//}
   
    glBegin(GL_LINES);
          x=x1;
          x-=320;
          x/=320;

          y=480-y1;
          y-=240;
          y /=240;
          

	glVertex2d(x, y);
          x=x3;
          x-=320;
          x/=320;

          y=480-y3;
          y-=240;
          y /=240;
          
	glVertex2d(x,y);

    glEnd();

//    glDisable(GL_LINE_SMOOTH);
#else
	report_error_c("No OGL_line_no_colour() on non-OGL build","",0);

#endif
}




void OGL_polygon(int numb_verts,fcoord *point_list, float r, float g, float b)
{
#if OGL==1

   int err,i;

//we need three points, so ignore num_lines
//we ignore screen_address and screen width
//    glBegin(GL_POLYGON);
//	  glColor3d(r, g, b);
//	  glVertex3d( 0.8,  0.8, 0.0);
//	  glColor3d(r, g, b);
//	  glVertex3d( 0.8, -0.8, 0.0);
//	  glColor3d(r, g, b);
//	  glVertex3d(-0.8, -0.8, 0.0);
//	  glColor3d(r, g, b);
//	  glVertex3d(-0.8,  0.8, 0.0);
//    glEnd();
	glColor3d(r, g, b);

	glBegin(GL_POLYGON);
	for (i=0;i<numb_verts;i++)
	{
	glVertex2d(point_list->x, point_list->y);
	point_list++;
	}
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
	glEnd();
//   err=aglGetError();
#else
	report_error_c("No OGL_polygon() on non-OGL build","",0);
#endif
}


void OGL_Tpolygon(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent)
{
#if OGL==1

   int err,i;
   float temp_r,temp_g,temp_b;
   glEnable(GL_DEPTH_TEST);
 
//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//glPixelStorei(GL_PACK_ALIGNMENT, 1); 

// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGBA,
// GL_UNSIGNED_BYTE, taddr);
// 
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glEnable(GL_TEXTURE_2D);      
    glBindTexture(GL_TEXTURE_2D, name);
	glBegin(GL_POLYGON);
	glColor3d(r, g, b);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
   float light;
   
   
	
	if (translucent!=2)
     {
    glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);
	}
	else
	{
//    glEnable (GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(r, g, b,0.5);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);
//	  glDisable(GL_BLEND);
	}
	
	point_list++;
	}
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
	glEnd();
    glDisable(GL_TEXTURE_2D);

//   err=aglGetError();
#else
	report_error_c("No OGL_Tpolygon() on non-OGL build","",0);
#endif
}




void OGL_Gpolygon(int numb_verts,fdcoord *point_list,float r, float g, float b, int translucent)
{
#if OGL==1

   int err,i;
   float temp_r,temp_g,temp_b;
      
	glBegin(GL_POLYGON);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
   float light;
   
   
   light=point_list->u;
   
     temp_r=(r+light)/65535;	//0-1
     temp_g=(g+light)/65535;	//0-1
     temp_b=(b+light)/65535;	//0-1

//   light=light/32000;
   
//   temp_r=r*light;
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

//   temp_g=g*light;
   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

//   temp_b=b*light;
   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
	
	if (translucent==0)
     {
	glColor3d(temp_r, temp_g, temp_b);
	glVertex2d(point_list->x, point_list->y);
	}
	else
	{
//    glEnable (GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(temp_r, temp_g, temp_b,0.5);
	glVertex2d(point_list->x, point_list->y);
//	glDisable(GL_BLEND);
	}
	
	point_list++;
	}
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
	glEnd();
//   err=aglGetError();
#else
	report_error_c("No OGL_Gpolygon() on non-OGL build","",0);
#endif
}


int init_ogl()	//A LS_CGrafPtr to you Stu.
{
#if OGL==1

//	GLint          attrib[] = { AGL_RGBA, AGL_ACCELERATED, AGL_ALL_RENDERERS, AGL_NONE };
	GLint          attrib[] = { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_FULLSCREEN,AGL_ACCELERATED, AGL_ALL_RENDERERS, AGL_NONE };

//	GLint          attrib[] = { AGL_RGBA, AGL_FULLSCREEN,AGL_DOUBLEBUFFER, AGL_NONE };


	AGLPixelFormat fmt;
	GLboolean      ok;
	int err;
	LS_CGrafPtr	frontBuff;
    extern DSpContextReference		mDisplayContext;
    extern GDHandle				mZexGDH;
    extern DSpContextReference		mDisplayContext;
extern WindowPtr our_window;

	/* Choose an rgb pixel format */
    DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);	//get vram address
//	SetGDevice(mZexGDH);
 //   SetPort(our_window);


	fmt = aglChoosePixelFormat(NULL, 0, attrib);
//	fmt = aglChoosePixelFormat(mZexGDH, 1, attrib);
	if(fmt==0) 
	{
	report_error ("init_ogl: Couldn't init OpenGL (aglChoosePixelFormat).\nNo 3d hardware found. \
	               ","Check you have all the required extensions needed for your graphics card.",666);
	return;
	}

	/* Create an AGL context but dont activate it yet*/
	ctx = aglCreateContext(fmt, NULL);
	if(ctx==0) 
	{
	report_error ("init_ogl: Couldn't init OpenGL (aglCreateContext). Error.","",(int)ctx);
	return;
	}

    if (SufficientVideoMemory( fmt )==0) 
	report_error ("init_ogl: . Error. SufficientVideoMemory failed.","",(int)ctx);

	/* Pixel format is no longer needed */
	aglDestroyPixelFormat(fmt);
	    OGL_active=0;
#endif
		return 1;	
}

void ogl_active()
{
#if OGL==1

    LS_CGrafPtr	frontBuff;
	int      err;
	long const one=1;
//	  	  Rect r;
//	  	  AuxWinHandle awh;
//	  	  
//	  	  
//	  	  r.top = r.left = 0;
//	  	  DSpContext_LocalToGlobal(mDisplayContext, (Point *)&r);
//	  	  r.right = r.left + SCREEN_WIDTH;
//	  	  r.bottom = r.top + SCREEN_HEIGHT;
//	  	  
//	  	  frontBuff = (LS_CGrafPtr)NewCWindow (NULL, &r, "", 0, plainDBox, (WindowPtr)-1, 0, 0);


    DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);	//get vram address
    SetPort((GrafPort*)frontBuff);
	/* Attach the window to the context */
	err = aglSetDrawable(ctx, frontBuff);
	if(err==0) 
	{
	err=aglGetError();

	report_error ("ogl_active: Couldn't activate OpenGL(aglSetDrawable). Error.","",err);
	return;
	}
	/* Make the context the current context */
	err = aglSetCurrentContext(ctx);
	if(err==0) 
	{
	err=aglGetError();
	report_error ("ogl_active: Couldn't activate OpenGL (aglSetCurrentContext). Error.","",err);
	return;
	}
//	glViewport (0,0,640,480);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_CULL_FACE);
//    glFrontFace(GL_CW);

glViewport(0, 0, 640, 480);

aglSetInteger (ctx, AGL_SWAP_INTERVAL, &one);

//glMatrixMode(GL_PROJECTION);
//glLoadIdentity();
//glOrtho(0, 640, 480, 0, -1, 1);
//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();

//    
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
////	glOrtho(0.0f, (GLfloat) 640, 0.0f, (GLfloat) 480, 0.0f, 1.0f);
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// 	glViewport(0, 0, 640, 480);
//       glDepthRange(0.0f, 1.0f);
//    glEnable(GL_SCISSOR_TEST);
//    glScissor(0,0,640,480);
//load helvetica 9
    err=init_ogl_text();

    OGL_active=1;
//       user_delay(30);
#endif
}


void ogl_inactive()
{
#if OGL==1

//	aglSetCurrentContext(NULL);
	 aglSetDrawable(ctx, NULL);
     OGL_active=0;
//       user_delay(30);
#endif
}



void OGL_show_it(void)	//as it says, wait for all rendering operations
						//to end and then show the result
{ 
#if OGL==1
   int err;
//   err=aglGetError();
	glFinish();
	aglSwapBuffers(ctx);
#else
	report_error_c("No OGL_show_it() on non-OGL build","",0);
#endif
}


void cleanupAGL()
{   
#if OGL==1
    if (ctx!=0)
    {
	 aglDestroyContext(ctx);
	 ctx=0;	//little note for myself
     OGL_active=0;
    }
#endif
}


float int_to_ogl_x(int coord_x)
{
//#if OGL==1
float return_val;
return_val=coord_x-320;
return_val=return_val/320;

return return_val;
//#endif
}


float int_to_ogl_y(int coord_y)
{
//#if OGL==1
float return_val;
return_val=480-coord_y;
return_val-=240;
return_val=return_val/240;

return return_val;
//#endif
}

//allow drawing in main area of screen
void OGL_set_stencil_for_above()
{
#if OGL==1
 glClear(GL_STENCIL_BUFFER_BIT);
/* create a full screen sized rectangle-shaped stencil area minus the control panel area */
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glStencilFunc (GL_ALWAYS, 0x1, 0x1);
 glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
 glBegin(GL_QUADS);
 glVertex3f (-1.0, -1.0, 0.0);
 glVertex3f (-1.0, 0.8, 0.0);
 glVertex3f (1.0, 0.8, 0.0);
 glVertex3f (1.0, -1.0, 0.0);
 glEnd();
#endif
}

//allow drawing in all of screen
void OGL_set_stencil_for_all()
{
#if OGL==1
 glClear(GL_STENCIL_BUFFER_BIT);
/* create a full screen sized rectangle-shaped stencil area */
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glStencilFunc (GL_ALWAYS, 0x1, 0x1);
 glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
 glBegin(GL_QUADS);
 glVertex3f (-1.0, -1.0, 0.0);
 glVertex3f (-1.0, 1.0, 0.0);
 glVertex3f (1.0, 1.0, 0.0);
 glVertex3f (1.0, -1.0, 0.0);
 glEnd();

#endif
}

void OGL_draw_pict(int x, int y,LSRAW* pictbuffer_rgba, int picture_w, int picture_h)
{
#if OGL==1
Ptr raw_rgba_data;

    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
//    glPixelStorei(GL_PACK_ALIGNMENT, 1); 

    glRasterPos2f(int_to_ogl_x(x), int_to_ogl_y(y));
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);
#else
	report_error_c("No OGL_draw_pict() on non-OGL build","",0);
#endif
}

//takes a clut index and sets drawing colour
void OGL_set_pen_colour_from_index(unsigned int colour)
{
#if OGL==1
int red,green,blue;
float temp_r,temp_b,temp_g;
                             
                             convert_to_rgb(colour, &red, &green, &blue);
     						temp_r=red;
     						temp_g=green;
     						temp_b=blue;
     						temp_r=temp_r/255;	//0-1
     						temp_g=temp_g/255;	//0-1
     						temp_b=temp_b/255;	//0-1
	glColor3d(temp_r, temp_g, temp_b);
#else
	report_error_c("No OGL_set_pen_colour_from_index() on non-OGL build","",0);
#endif
	
}

//takes int x and y
void OGL_moveto(int x, int y)
{
#if OGL==1
float fx,fy;

//calc pos
    fx=int_to_ogl_x(x);
    fy=int_to_ogl_y(y);
    
	glRasterPos2f(fx,fy);
#else
	report_error_c("No OGL_moveto() on non-OGL build","",0);
#endif
	
}

void OGL_move_and_set_colour(int x, int y, int colour)
{
#if OGL==1

int red,green,blue;
float temp_r,temp_b,temp_g;
float fx,fy;                             
                             convert_to_rgb(colour, &red, &green, &blue);
     						temp_r=red;
     						temp_g=green;
     						temp_b=blue;
     						temp_r=temp_r/255;	//0-1
     						temp_g=temp_g/255;	//0-1
     						temp_b=temp_b/255;	//0-1
	glColor3d(temp_r, temp_g, temp_b);

//calc pos
    fx=int_to_ogl_x(x);
    fy=int_to_ogl_y(y);
    
	glRasterPos2f(fx,fy);
#else
	report_error_c("No OGL_move_and_set_colour() on non-OGL build","",0);
#endif
	
}




static long textureMemory,videoMemory;

#if OGL==1
//check if we can run and report some geek stuff
int	SufficientVideoMemory( AGLPixelFormat fmt ) 
{
    AGLRendererInfo head_info, info;
	GLint 		accelerated;
	AGLDevice   *device;
	GLint 		i, ndevs;
	long vMajor, vMinor;
	unsigned char number_string[12];
	const GLubyte * ogl_string; 
    char renderer_string[300];
    char vendor_string[300];
    char version_string[300];

	device = aglDevicesOfPixelFormat(fmt, &ndevs);
	if (!device || ndevs < 1) 
	{
	show_text_in_loading_screen("(OGL): aglDevicesOfPixelFormat failed." );
		return 0;
	}
	show_text_in_loading_screen_nocr("OGL: Number of rendering devices: ");	
    ZexNumToString(ndevs,number_string);
    show_text_in_loading_screen_fcr(number_string);

	
	head_info =aglQueryRendererInfo( device, 1 );
	info = head_info;
	if (!info) 
	{
	show_text_in_loading_screen("(OGL): aglQueryRendererInfo failed." );
		return 0;
	}
	

 for ( i = 0 ; i < ndevs ; i++ ) 
 {
		// ignore the software renderer listing
		aglDescribeRenderer( info, AGL_ACCELERATED, &accelerated );
		if( accelerated ) 
		{		
		aglDescribeRenderer( info, AGL_TEXTURE_MEMORY, &textureMemory );
	    aglDescribeRenderer( info, AGL_VIDEO_MEMORY,&videoMemory );
		}
		info = aglNextRendererInfo(info);
	}


aglDestroyRendererInfo(head_info);

ogl_active();

ogl_string= glGetString (GL_RENDERER);
CToPascal((char*) ogl_string,renderer_string);

ogl_string= glGetString (GL_VENDOR);
CToPascal((char*) ogl_string,vendor_string);

ogl_string= glGetString (GL_VERSION);
CToPascal((char*) ogl_string,version_string);

	aglGetVersion (&vMajor, &vMinor);

ogl_inactive();


show_text_in_loading_screen_nocr("OGL: OpenGLª Version: ");

ZexNumToString(vMajor,number_string);
append_text_to_loading_screen(number_string);
append_text_to_loading_screen(".");
ZexNumToString(vMinor,number_string);
show_text_in_loading_screen_fcr(number_string);


show_text_in_loading_screen_nocr("OGL: Renderer name: ");
show_text_in_loading_screen_fcr((unsigned char *)renderer_string);

show_text_in_loading_screen_nocr("OGL: Renderer Version: ");
show_text_in_loading_screen_fcr((unsigned char *)version_string);


show_text_in_loading_screen_nocr("OGL: Vendor name: ");
show_text_in_loading_screen_fcr((unsigned char *)vendor_string);

show_text_in_loading_screen_nocr("OGL: Texture memory (K): ");

ZexNumToString(textureMemory/1024,number_string);
show_text_in_loading_screen_fcr(number_string);

show_text_in_loading_screen_nocr("OGL: Video memory (K): ");

ZexNumToString(videoMemory/1024,number_string);
show_text_in_loading_screen_fcr(number_string);

if (textureMemory<4000000) 
{
	report_error ("OGL: You need at least 4 Megs of texture memory to play Zex.","",-1);
    return 0;
}
show_text_in_loading_screen("OGL: Checks OK");

//  user_delay(60);

	return 1;
}

#endif


/* 7/10 \ /
         |
   __          __   .      __                 _._
  /  \        /     |     /  \                 |
  |  O P E N  | -G  L     | C  O N S T A N T   I M A G E S
  \__/        \__/  |__   \__/                _|_

*/
extern int cp_reference;


/* If possible I'd like you to accelerate the control panel work as I need to leverage some routines not 
yet written to speed up the inventory screen, dialogs, mouse cursor etc. I need a routine that is passed 
a pointer to a 32 bit rgba (i..e. ogl native) bitmap and tags it to an OGL_QUAD (or however you want to 
do it) for display; you'll need a list and a routine run every frame obviously - suggest you return some 
ID I can note and have a kill routine to remove things from the list - errm have an egg, sorry :) My 
pic_utils are way too slow as the data has to travel over PCI every frame rather than being in vram ready 
to bind to some vertices. */

//BIG NOTE. THE LARGEST TEXTURE OGL SEEMS TO COPE WITH IS 256*256
//so we need to split larger ones up into smaller ones
// ---- module data ----

// lets have a structure with various details in
typedef struct 
  {
  int slot_active;
  int context;
  LSRAW *rgba_image;
  int ci_name;
  int x1;
  int y1;
  int x2;
  int y2;
  int tile_id;	//SB 211199
  fdcoord output_points[6];
  } ci_struct;
  
#define CONSTANT_IMAGE_MAX 30

#define INACTIVE 0
#define ACTIVE 1

ci_struct constant_image_array[CONSTANT_IMAGE_MAX];


int current_context;

// ---- functions ----

//this onesits in front of set_as_constant_image and checks for textures>256 pixels
static int unique_tile_id;

// -1 returned means no constant image assigned
int set_as_constant_image(LSRAW *_rgba_image, int _x1, int _y1, int _x2, int _y2)
{
	int width, height,tiles_x, tiles_y, ix1,iy1;
	int number_of_tiles,cut_top_left_x, cut_top_left_y;
	int tx,return_val=-1;
	LSRAW * temp_texture_rgba;
	
	width=_x2-_x1;
	height=_y2-_y1;
	
	
	// Added for Zex2005 (by Rob) so that we can see where constant images (OGL only) are being loaded...
#if !OGL
	report_error_id_non_fatal("Loading constant images in a non-OGL build!", 0);
#endif
	
	tiles_x=width/256+1;
	tiles_y=height/256+1;
	if (tiles_x==1 && tiles_y==1)
	{
		//check is factor of 2 wide
		
		if (is_a_factor_of_2(width)==1)
			return set_as_constant_image1(_rgba_image, _x1, _y1, _x2, _y2,-1);
		
		else	//we have to pad the image out to be a factor of 2 wide (256 in our case)
			
		{
			temp_texture_rgba=(LSRAW*)NewPtr((256*256*4)+80);
			if (temp_texture_rgba==0)
			{
				report_error_c ("set_as_constant_image: Couldn't get temp memory.(2)","",-1);
				
				return -1;
			} 
			
			cut_out(_rgba_image, temp_texture_rgba,0, 0);	//cut out and pad to 256 wide
															//insert
			ix1=_x1; iy1=_y1;
			return_val=set_as_constant_image1(temp_texture_rgba, ix1, iy1, ix1+256, iy1+256, -1);
			DisposePtr((Ptr) temp_texture_rgba);
			return return_val;
		}
		
	}	//end of texture<256 pixels wide
	
	//texture >256 pixels
	else
	{
		
		//we cut each tile of the texture into a temp 256*256*4 buffer, calc its true position
		//relative to the parent tex and pass it to set_as_constant_image1
		//the remainder must be padded with 0's to a power of two boundary
		
		if (unique_tile_id>10000000) unique_tile_id=1;	//call me a cynic but...
		if (unique_tile_id<0) unique_tile_id=1;	//call me a cynic but...
		
		unique_tile_id++;	//couldnt care less what this is just as long as its unique
							//get temp
		
		temp_texture_rgba=(LSRAW*)NewPtr((256*256*4)+80);
		if (temp_texture_rgba==0)
		{
			report_error_c ("set_as_constant_image: Couldn't get temp memory.","",-1);
			
			return -1;
		} 
		cut_top_left_x=0;
		cut_top_left_y=0;
		// current_top_left_u=0;
		// current_top_left_v=0;
		ix1=_x1; iy1=_y1;
		
		while (tiles_y>0)
		{
			
			
			tx=tiles_x;	//reset horiz cutter
			while (tx>0)
			{
				//cut to temp_texture
				cut_out(_rgba_image, temp_texture_rgba,cut_top_left_x, cut_top_left_y);
				//insert
				return_val=set_as_constant_image1(temp_texture_rgba, ix1, iy1, ix1+256, iy1+256, unique_tile_id);
				
				tx--;
				ix1+=256;
				cut_top_left_x+=256;
				
			}
			ix1=_x1;
			iy1+=256;
			tiles_y--;
			cut_top_left_x=0;
			cut_top_left_y+=256;
		}
		DisposePtr((Ptr) temp_texture_rgba);
	}
	return return_val;
}

//returns 1 if so
int is_a_factor_of_2(int number)
{
int i,bit_count;

bit_count=0;
for (i=0;i<31;i++)
   {
   if ((number&1)==1) bit_count++;
   number=number>>1;
   }
if (bit_count>1 || bit_count==0) return 0;
else return 1;	
}

//cuts 256*256 pixels out of source into dest
void cut_out (LSRAW* source, LSRAW* destination, int source_start_x, int source_start_y)
{
int source_x, source_y;
int source_width;
int next_line_offset;
unsigned int * source_data;
unsigned int * source_reset;

unsigned int * dest_data;
unsigned int the_rgba,the_rgba_zero;
int x,y,width_pos,h_pos;

destination->pwidth=256; destination->pheight=256;
source_width=source->pwidth;
next_line_offset=source_width-(source_start_x+256);	//offset ot start of next line after we have cut this one (in unsigned ints)

source_data=(unsigned int *)(char *)&source->raw_data;
source_data+=source_width*source_start_y;
source_data+=source_start_x;	//ptr maths remember Stu.
dest_data=(unsigned int *)(char *)&destination->raw_data;

source_reset=source_data;

width_pos=source_start_x;
h_pos=source_start_y;

the_rgba_zero=0;
for (y=0;y<256;y++)
  {
    for (x=0;x<256;x++)
     {
     
     if (width_pos>=source->pwidth || h_pos>=source->pheight)
     {	//fill with transparent
      *dest_data=the_rgba_zero;
      dest_data++;
     }
     else //copy data
     {
     the_rgba=(*source_data);
     *dest_data=the_rgba;
     
     dest_data++;
     source_data++;
     }     
     
     width_pos++;
     }  
  h_pos++;  
  source_data=source_reset;
  source_data+=source_width;
  source_reset=source_data;
//  source_data+=next_line_offset;   
  width_pos=source_start_x;
  }


}

// -1 returned means no constant image assigned
int set_as_constant_image1(LSRAW *_rgba_image, int _x1, int _y1, int _x2, int _y2, int unique_id)
{
int my_glname;
int err;
int constant_image_index, height;
Ptr raw_rgba_data;

// lets try to find an inactive slot
constant_image_index=0; 
height=_y2-_y1;



while(constant_image_array[constant_image_index].slot_active == ACTIVE)
  {
  if(constant_image_index==CONSTANT_IMAGE_MAX)
    {
    return -1; // are there too many constant images to process??
    }
  constant_image_index++;
  }
  

// lets fill up our private array
constant_image_array[constant_image_index].rgba_image=_rgba_image;
constant_image_array[constant_image_index].x1=_x1;
constant_image_array[constant_image_index].y1=_y1;
constant_image_array[constant_image_index].x2=_x2;
constant_image_array[constant_image_index].y2=_y2;

constant_image_array[constant_image_index].output_points[0].x=((float)_x1-320)/320;
constant_image_array[constant_image_index].output_points[0].y=((float)_y1-240)/240;
constant_image_array[constant_image_index].output_points[0].u=0;
constant_image_array[constant_image_index].output_points[0].v=0;
constant_image_array[constant_image_index].output_points[1].x=((float)_x1-320)/320;
constant_image_array[constant_image_index].output_points[1].y=((float)_y2-240)/240;
constant_image_array[constant_image_index].output_points[1].u=0;
constant_image_array[constant_image_index].output_points[1].v=1;
constant_image_array[constant_image_index].output_points[2].x=((float)_x2-320)/320;
constant_image_array[constant_image_index].output_points[2].y=((float)_y2-240)/240;
constant_image_array[constant_image_index].output_points[2].u=1;
constant_image_array[constant_image_index].output_points[2].v=1;
constant_image_array[constant_image_index].output_points[3].x=((float)_x2-320)/320;
constant_image_array[constant_image_index].output_points[3].y=((float)_y1-240)/240;
constant_image_array[constant_image_index].output_points[3].u=1;
constant_image_array[constant_image_index].output_points[3].v=0;

constant_image_array[constant_image_index].tile_id=unique_id;

constant_image_array[constant_image_index].context=current_context;

constant_image_array[constant_image_index].slot_active=ACTIVE;

// construct a unique name
my_glname=get_unique_texture_name();
constant_image_array[constant_image_index].ci_name=my_glname;

// lets upload the image to GLgod
//	glEnable(GL_TEXTURE_2D);      
#if OGL==1
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, _x2-_x1);
raw_rgba_data=(char *)&_rgba_image->raw_data;
  glBindTexture(GL_TEXTURE_2D, my_glname);	//we supply the name...

//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1+_x2-_x1, 1+_y2-_y1, 0, GL_RGBA,
//  GL_UNSIGNED_BYTE, _rgba_image);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _x2-_x1, height, 0, GL_RGBA,
  GL_UNSIGNED_BYTE, raw_rgba_data);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   err=aglGetError();
#else
   err = 0;
#endif
   if (err!=0) report_error_c ("OGL:set as ci: AGL error.","",err);

//set_constant_image_pos(constant_image_index, _x1, _y1);
return constant_image_index;
}



void draw_constant_images(void) // - calls texturedpoly, or recreates plot?
{
int my_glname;
int i;



i=0;		// start from the beginning
while(i<CONSTANT_IMAGE_MAX)	// inly do it if there are some constant images!
  {

  if(constant_image_array[i].context==current_context
   && 
     constant_image_array[i].slot_active==ACTIVE)
    {
//print_crunched32_trans_pict(constant_image_array[i].rgba_image,100,100);

//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glPixelStorei(GL_UNPACK_ROW_LENGTH, 314);
////    glPixelStorei(GL_PACK_ALIGNMENT, 1); 
//
//    glRasterPos2f(int_to_ogl_x(100), int_to_ogl_y(100));
//    glDrawPixels(314,278,GL_RGBA, GL_UNSIGNED_BYTE,
//    constant_image_array[i].rgba_image);


    // get unique name
    my_glname=constant_image_array[i].ci_name;
  
    // physically draw this quaderlateral (That's a quadrilateral in english)
    OGL_Tpolygon  (4,
               constant_image_array[i].rgba_image->raw_data, /* texture address (not used) */
               constant_image_array[i].output_points,
               my_glname, /* name */
               0.5, /* redness */
               0.5, /* greeness */
               0.5, /* blueness */ 
               0 /* not translucent */ 
               );
     }
     i++;          
       
   } // end of while loop
   
}


//has to check for unique tile id's
void delete_constant_image(int constant_image_id)
{
GLuint the_name;
int i;

if((constant_image_id<=-1) || (constant_image_id>=CONSTANT_IMAGE_MAX)) return; // protect against unassigned constant images

if (constant_image_array[constant_image_id].slot_active==INACTIVE) return;	 //this is an "oops" situation from main loop on first frame
// unload all images from the GLgod
if (OGL_active==1)
  {

  the_name=constant_image_array[constant_image_id].ci_name;
#if OGL==1
  glDeleteTextures(1,&the_name);
#endif
 

  }	//end of OGL active

// remove from the ci_array
constant_image_array[constant_image_id].slot_active=INACTIVE;

//texs loaded as a single tile have a tile id of -1
//tex's loaded as multiple tiles have a unique +ve id
if (constant_image_array[constant_image_id].tile_id!=-1)
 {
 int continue_delete,tile_id;
 //search for thers with similar tile id's 
  tile_id=constant_image_array[constant_image_id].tile_id; //cause it's about to get set to -1!
  continue_delete=1;
  while (continue_delete==1)
  {
  int result;
     result=kill_tile(tile_id);
     if (result==0) continue_delete=0;
  }
 }
}


//search const_images for something with the tile id
//return 1 if deleted else 0
int kill_tile(int tile_id)
{
int i;
GLuint the_name;

  i=0;		// start from the beginning
  while(i<CONSTANT_IMAGE_MAX)	  	  // inly do it if there are some constant images!
    {
    if (constant_image_array[i].tile_id==tile_id && constant_image_array[i].slot_active==ACTIVE)
     {
      the_name=constant_image_array[i].ci_name;
#if OGL==1
      glDeleteTextures(1,&the_name);
#endif
     // remove from the ci_array
      constant_image_array[i].slot_active=INACTIVE;
      constant_image_array[i].tile_id=-1;
      return 1;
     }
     i++;
    }
return 0;
}


void delete_all_constant_images(void)
{
GLuint the_name;
int i;


// unload all images from the GLgod
if (OGL_active==1)
 {

  i=0;		// start from the beginning
  while(i<CONSTANT_IMAGE_MAX)	  	  // inly do it if there are some constant images!
    {
     delete_constant_image(i);
     i++;
    }

  }	//end of OGL active


// reinit the entire ci_array
i=current_context;
init_constant_image();
current_context=i;
}

extern int inv_pict_reference;	//from constant images or -1 if not set
extern int inv_cursor_reference;	//same
extern int short_range_reticule_ref,long_range_reticule_ref;



// init makes all constant images inactive
void init_constant_image(void)
{
int i;

for(i=0; i<CONSTANT_IMAGE_MAX; i++)
  {
  constant_image_array[i].slot_active=INACTIVE;
  constant_image_array[i].tile_id=-1;
  }
  
current_context=0; mouse_on=0;
 inv_pict_reference=-1;	//from constant images or -1 if not set
 inv_cursor_reference=-1;	//same
 cp_reference=-1;
long_range_reticule_ref=-1;
short_range_reticule_ref=-1;

}


void set_constant_image_pos(int constant_image_id, int x, int y)
{
GLuint the_name;
int i,the_width, the_height;
int _x1,_x2,_y1,_y2,constant_image_index;

if((constant_image_id<=-1) || (constant_image_id>=CONSTANT_IMAGE_MAX)) return; // protect against unassigned constant images

y=480-y;

constant_image_index=constant_image_id;	//I'm lazy and pasted the code below in



  the_width=constant_image_array[constant_image_id].x2-constant_image_array[constant_image_id].x1;
  the_height=constant_image_array[constant_image_id].y2-constant_image_array[constant_image_id].y1;
 
  _x1=x;
  _x2=x+the_width;
  _y1=y;
  _y2=y+the_height;

constant_image_array[constant_image_index].x1=_x1;
constant_image_array[constant_image_index].y1=_y1;
constant_image_array[constant_image_index].x2=_x2;
constant_image_array[constant_image_index].y2=_y2;

constant_image_array[constant_image_index].output_points[0].x=((float)_x1-320)/320;
constant_image_array[constant_image_index].output_points[0].y=((float)_y1-240)/240;
//constant_image_array[constant_image_index].output_points[0].u=0;
//constant_image_array[constant_image_index].output_points[0].v=0;
constant_image_array[constant_image_index].output_points[1].x=((float)_x1-320)/320;
constant_image_array[constant_image_index].output_points[1].y=((float)_y2-240)/240;
//constant_image_array[constant_image_index].output_points[1].u=0;
//constant_image_array[constant_image_index].output_points[1].v=1;
constant_image_array[constant_image_index].output_points[2].x=((float)_x2-320)/320;
constant_image_array[constant_image_index].output_points[2].y=((float)_y2-240)/240;
//constant_image_array[constant_image_index].output_points[2].u=1;
//constant_image_array[constant_image_index].output_points[2].v=1;
constant_image_array[constant_image_index].output_points[3].x=((float)_x2-320)/320;
constant_image_array[constant_image_index].output_points[3].y=((float)_y1-240)/240;
//constant_image_array[constant_image_index].output_points[3].u=1;
//constant_image_array[constant_image_index].output_points[3].v=0;



}



void swap_context(int context) /* 2 or 3 contexts, so we can swap between the game screen, and say a fixed image without 
	               reiniting the entire specific game ci array */
{
  current_context=context;
}

int get_context(void)
{
return current_context;
}



extern int inv_cursor_reference;
extern LSRAW *inv_cursor_picture_rgba;


//ogl mouse cursor support
void ogl_show_mouse_cursor()
{
    if (mouse_on==0)
    {
	if (inv_cursor_reference==-1)
#if OGL==1
	inv_cursor_reference = set_as_constant_image(inv_cursor_picture_rgba, 100, 100, 100+inv_cursor_picture_rgba->pwidth, 100+inv_cursor_picture_rgba->pheight);
#endif
    ZexGetMouse (&start_mouse_xy);
    mouse_on=1;
    }
}

void ogl_hide_mouse_cursor()
{
 if (inv_cursor_reference!=-1)	 //the inventory picture is not resident
 {
 delete_constant_image(inv_cursor_reference);
 inv_cursor_reference=-1;
 }
 mouse_on=0;
}

void ogl_update_mouse_cursor()
{
Point mouse_xy;
//we take it that the start_mouse_xy is the center of the screen - i.e. 320,240
//and take new positions relative to there

    ZexGetMouse (&mouse_xy);
    mouse_xy.h=(mouse_xy.h-start_mouse_xy.h)+320;
    mouse_xy.v=(mouse_xy.v-start_mouse_xy.v)+240;
    set_constant_image_pos(inv_cursor_reference,mouse_xy.h,mouse_xy.v);
}

extern LSRAW* raw_rgba_data_CP;	//ptr to rgba control panel data

void ogl_load_control_panel()
{


	if (cp_reference==-1)
	cp_reference = set_as_constant_image(raw_rgba_data_CP, 0, 0, raw_rgba_data_CP->pwidth, 
    raw_rgba_data_CP->pheight);	//remember OGL origin is bottom left
}


void ogl_hide_control_panel()
{

 if (cp_reference!=-1)	//the inventory picture is not resident
 {
 delete_constant_image(cp_reference);
 cp_reference=-1;
 }
}


void ogl_hide_radar_reticules()
{
extern int short_range_reticule_ref,long_range_reticule_ref;
extern LSRAW* short_range_ret_rgba;
extern LSRAW* long_range_ret_rgba;

       delete_constant_image(short_range_reticule_ref);
       short_range_reticule_ref=-1;   
       delete_constant_image(long_range_reticule_ref); 
       long_range_reticule_ref=-1;   

}


