// Zex - 2D Space Trading Game with 3D graphics
// Copyright (C) 1998-2005  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

// *****************************************************************MODULE HEADER*****
// * FILENAME		: fcircle.c
// * MODULE TITLE	: Circle drawing
// * PROJECT		: Zex
// * DATE STARTED	: 10th April 1999, Sat 10:12pm
// * ORIGINAL AUTHOR: Rob Probin
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *		   (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial		  Descrip.
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
#include <new>
using namespace std;

#include "fpoly4.h"
#include "genpoly_int.h"
#include "fcircle.h"
#include "f_light_int.h"
//oldmac//#include <memory.h>
#include "Error_handler.h"
#include "pictures.h"
#include "OGL.h"
#if OGL
#include <agl.h>
#endif
#include "3d_structures_def.h"
#include "fdistort.h"

extern	float SinTable[360], CosTable[360];

// ***********************************************************************************
// * CONSTANTS
// *

#define CIRCLE_ENTRIES 1024

// ***********************************************************************************
// * TYPE DEFINITIONS
// *

#define I_TO_FIX(x) (  ((int)(x))  <<  16  )
#define FIX_TO_I(x) ((x)>>16)

// ***********************************************************************************
// * VARIABLE DECLARATIONS
// *

typedef float circle_type;

circle_type *circle_table_ptr;
void fcircle_test(void);



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

float squareroot( float R);
void tpline(int x1, int x2, char *line_screen_address, unsigned char colour);


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: fcircle_test
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Tests the circle drawing
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	:
// |
// |
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------

void fcircle_test(void)
{
static int pause_count=0;
static int size_count=20;
static int colour_count=0;

fcircle(100,100,size_count,colour_count,MODE_SOLID);

pause_count++;
if(pause_count>5)
  {
  pause_count=0;
  size_count++; if(size_count>10) size_count=0;
  colour_count++; colour_count&=0xff;
  }

}

// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: create_outer_flare
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Draws various sizes of circle to create the outside of a lens flair effect
// | as used in the explosion routines.
// +----------------------------------------------------------------------------------
// | PARAM IN	: x = x center of the effect. Effect is as if the screen center is the lens center
// |              y = y center of the effect.
// |              size = size of the effect 0-
// |              brightness = brightness of the effect. Currently used, but probably should be in
// |              relation to the size of the effect, and also the "power" of the effect. No range yet.
// |
// | PARAM OUT	: none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------

int screen_x_center;
int screen_y_center;			// init by init circle.


void create_outer_flare(int x, int y, int size, int brightness)
{
int tempx,tempy;
extern int Mode320;

// plot five circles, three further size, two close size.

if(Mode320==0)
  {
  size*=2;
  }

// close size circle, further from center than effect location
tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=static_cast<int>(tempx*1.3); tempy=static_cast<int>(tempy*1.3);
tempx=tempx+screen_x_center; tempy=tempy+screen_y_center;
fcircle(tempx,tempy,size/3,231,MODE_TRANSPARENT);

// close size circle, closer to center than effect location
tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=static_cast<int>(tempx*0.75); tempy=static_cast<int>(tempy*0.75);
tempx=tempx+screen_x_center; tempy=tempy+screen_y_center;
fcircle(tempx,tempy,size/4,141,MODE_TRANSPARENT);

// further size circles
tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=static_cast<int>(tempx*1.4); tempy=static_cast<int>(tempy*1.4);
tempx=screen_x_center-tempx; tempy=screen_y_center-tempy;
fcircle(tempx,tempy,size/3,170,MODE_TRANSPARENT);

tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=static_cast<int>(tempx*0.4); tempy=static_cast<int>(tempy*0.4);
tempx=screen_x_center-tempx; tempy=screen_y_center-tempy;
fcircle(tempx,tempy,size/4,137,MODE_TRANSPARENT);

tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=static_cast<int>(tempx*0.5); tempy=static_cast<int>(tempy*0.5);
tempx=screen_x_center-tempx; tempy=screen_y_center-tempy;
fcircle(tempx,tempy,size/5,227,MODE_TRANSPARENT);

// center circle
tempx=x; tempy=y;
fcircle(tempx,tempy,size/2,0,MODE_TRANSPARENT);

}

void fcircle_ogl(int x, int y, int r, unsigned char colour, int mode);
void fcircle_sw(int x, int y, int r, unsigned char colour, int mode);
void strange_ogl_poly(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int mode);
void convert_to_rgb(int colour, int *r, int *g, int *b);

void fcircle(int x, int y, int r, unsigned char colour, int mode)
{
extern int OGL_active;

if(OGL_active==1)
  {
  fcircle_ogl(x,y,r,colour,mode);
  }
else
  {
  fcircle_sw(x,y,r,colour,mode);
  }

}

#define NUM_SIDES 20

void fcircle_ogl(int x, int y, int r, unsigned char colour, int mode)
{
int angle,angle_incr;
int last_x,last_y;
int curr_x,curr_y;
int red,green,blue;

convert_to_rgb(colour, &red, &green, &blue);


red<<=8; green<<=8; blue<<=8;

angle=0;
last_x=x+r; last_y=y;
angle_incr=360/NUM_SIDES;

while(angle<360)
  {
  curr_x=static_cast<int>((r*COS(angle))+x);
  curr_y=static_cast<int>((r*SIN(angle))+y);

  strange_ogl_poly(x,y,last_x,last_y,curr_x,curr_y,red,green,blue,mode);

  angle+=angle_incr;
  last_x=curr_x; last_y=curr_y;
  }

// now draw last poly...
curr_x=x+r; curr_y=y;		        	// start position
strange_ogl_poly(x,y,last_x,last_y,curr_x,curr_y,red,green,blue,mode);

}




dcoord *point_list;
dcoord points[10];
dcoord temp_points[10];
dcoord output_points[30];
int polys;
#if OGL==1
fdcoord ftoutput_points[30];	 //float temp out points (OGL)
#endif

void strange_ogl_poly(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int mode)
{
#if OGL==1
point_list=points;
point_list->x=x1; point_list->y=y1; point_list->u=20000; point_list++;
point_list->x=x2; point_list->y=y2; point_list->u=20000; point_list++;
point_list->x=x3; point_list->y=y3; point_list->u=20000; point_list++;

distort_window_clipper3(points,temp_points,output_points,&polys);
point_list=output_points;

while(polys>0)
  {
//  gouraud3((char *)screenaddr,screenwidth, point_list, r, g, b, 1);       	    // red polygon
	  dcoord * op;
	  fdcoord * fop;

//ogl likes all points in the range -1 to 1, where 0 is center, so we have to convert our integer points to floating points in this range
//y is also reversed
          int i;


          op=point_list;
          fop=ftoutput_points;

          for (i=0;i<3;i++)
          {
          fop->x=op->x;
          fop->x-=320;
          fop->x = fop->x/320;

          fop->y=480-op->y;
          fop->y-=240;
          fop->y = fop->y/240;

          fop->u=op->u;	//lighting
//          fop->u=fop->u;
//          fop->v=op->v;
          op++;
          fop++;
          }
//       polygon(output_lines,(coord *)outpoints,fpoly_colour,screenaddr,screenwidth);
       OGL_Gpolygon(3,(fdcoord *)ftoutput_points,r,g,b, mode);

  point_list+=3;
  polys--;
  }
#endif
}

// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: fcircle
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Draws a circle on the screen
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	:
// |
// |
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS: 1. It is clipped with the screen edges BUT this is a slow clipping method.
// |              2. Requires fpoly for the pline routine.
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------


void fcircle_sw(int x, int y, int r, unsigned char colour, int mode)
{
int ytop,ybot,xstart,xend;
int ypos;
int accumulator,increment;
circle_type width;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
char *line_screen_address;

ytop=y-r;  ybot=y+r;  						// top and bottom of circle (line y)

if(ybot<screen_y_min) return;				// quit if the circle is off the top

increment=I_TO_FIX(CIRCLE_ENTRIES)/(r*2);	// increment between lines in the circle table
accumulator=0;							// accumulator of the lines to access the circle table

line_screen_address = screenaddr+(screenwidth*ytop);	// calculate the physical screen address of the top line of the circle

if(mode==0)
  {
  for(ypos=ytop; ypos<ybot; ypos++)		  	  // go from the top line to "one below" the bottom, since r=2, ytop=-1, ybot=0 NOT ybot=1!
    {
    if(ypos>screen_y_max) break; 		  	  	  // skip if off screen to the bottom
    if(ypos>=screen_y_min) 		  	  	  	  // skip this line if off the top
      {
      width=*(circle_table_ptr+FIX_TO_I(accumulator));  // get value out of circle table
      width*=r;								// scale returned value

      xstart=static_cast<int>(x-width); xend=static_cast<int>(x+width);
											// clipping for x at each line
      if(xstart<screen_x_min)					// if start is less than the minimum, make it the minimum
        { xstart=screen_x_min; }
      if(xend>screen_x_max)					// if end is greater than the maximum, make it the maximum
        { xend=screen_x_max; }
													// only print if the line is on screen
      if(xstart<=screen_x_max && xend>=screen_x_min)	// start must be before the end of the line, and the end before the start!
        {
        pline(xstart, xend, line_screen_address, colour);
        }
      } /* end of if */

    line_screen_address+=screenwidth;
    accumulator+=increment;

    } /* end of for */
  }
else
  {
  for(ypos=ytop; ypos<ybot; ypos++)		  	  // go from the top line to "one below" the bottom, since r=2, ytop=-1, ybot=0 NOT ybot=1!
    {
    if(ypos>screen_y_max) break; 		  	  	  // skip if off screen to the bottom
    if(ypos>=screen_y_min) 		  	  	  	  // skip this line if off the top
      {
      width=*(circle_table_ptr+FIX_TO_I(accumulator));  // get value out of circle table
      width*=r;								// scale returned value

      xstart=static_cast<int>(x-width); xend=static_cast<int>(x+width);
											// clipping for x at each line
      if(xstart<screen_x_min)					// if start is less than the minimum, make it the minimum
        { xstart=screen_x_min; }
      if(xend>screen_x_max)					// if end is greater than the maximum, make it the maximum
        { xend=screen_x_max; }
													// only print if the line is on screen
      if(xstart<=screen_x_max && xend>=screen_x_min)	// start must be before the end of the line, and the end before the start!
        {
        tpline(xstart, xend, line_screen_address, colour);
        }
      } /* end of if */

    line_screen_address+=screenwidth;
    accumulator+=increment;

    } /* end of for */

  } /* end for mode if */

} /* end of fcircle */



// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: init_circle
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Allocates memory for, and init the data table required for ftable
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	:
// |
// |
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------



void init_circle(void)
{
	int element;
	circle_type *ctp_copy;
	//oldmac//Handle temp_h;

	float fractional_element;



	// init 2048 bytes of memory, pointed to by circle_table_ptr

	//oldmac//temp_h=NewHandleClear(CIRCLE_ENTRIES*sizeof(circle_type));
	//oldmac//if (temp_h!=0)
	//oldmac//	{
	//oldmac//	LS_HLock(temp_h);
	//oldmac//	circle_table_ptr=(float *)*temp_h;
	//oldmac//	}
	//oldmac//else


	circle_table_ptr = new(nothrow) circle_type[CIRCLE_ENTRIES];
	if(!circle_table_ptr)
	{
		report_error_c("Not enough memory (init_ocbs:init_circle).","",3);
	}

	// fill array with values

	ctp_copy=circle_table_ptr;


	for(element=0; element<CIRCLE_ENTRIES; element++)
	{
		fractional_element = ((float)element) - (( ((float)CIRCLE_ENTRIES) -1)/2);	// after this ranges from -511.5 to 511.5 in steps of 1
		fractional_element /=  (( ((float)CIRCLE_ENTRIES) -1)/2);			// after this ranges from -1 to 1

		*ctp_copy=squareroot(1 - fractional_element*fractional_element);		// n.b. 1*1=1
		ctp_copy++;

	}
}


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: squareroot
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Does a squareroot, stupid.
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	:
// |
// |
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------

float squareroot( float R)
{
float x_1,x_0;
int count;

x_0=0.2;		// starting position chosen at random (-ish)

// magic magic oh my god, magic.
for(count=0; count<8; count++)
  {
    x_1=0.5*(x_0 + R/x_0);
    x_0=x_1;
  }

return x_1;
}



// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: tpline
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Like fpoly pline, but produces a transparent line
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	:
// |
// |
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS: Does not check for x1>x2
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------

void tpline(int x1, int x2, char *line_screen_address, unsigned char colour)
{
int width;

width=x2-x1;

line_screen_address+=x1;

while(width>0)
  {
  *line_screen_address = transparency_table[((*line_screen_address)<<8)+colour];
  line_screen_address++;
  width--;
  }
}


