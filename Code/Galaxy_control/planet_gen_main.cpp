// *************************************MODULE HEADER*********************************
// * FILENAME           : planet_gen_main.fth
// * MODULE TITLE       : planet_gen_main
// * PROJECT            : Zex
// * DATE STARTED       : 22:26:28 on 29th August 1999
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
// * MISC NOTES
// *



#define PLANET_GEN_DEBUG 0

// ***********************************************************************************
// * INCLUDED FILES
// *

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//#include <stdlib.h>
#include <math.h>

//oldmac//#include <types.h>

#if PLANET_GEN_DEBUG

#include <quickdraw.h>
#include <windows.h>
#include <menus.h>
#include <fonts.h>
#include <dialogs.h>
#include <textedit.h>
#include <Events.h>
#include <stdio.h>

//#include <stdlib.h>  // used for rand16 and srand

#endif

//oldmac//#include <fp.h>

#include "Planet_Generator.h"	//defs for this file
#include "zex_misc.h"
#include "fpoly4.h"
#include "genpoly_int.h"
#include "fdistort.h"
#include "fgouraud.h"


// ***********************************************************************************
// * CONSTANTS
// *

#define MAX_ANGLE 255				// angles go from -512 to 511, or from 0 to 1023
									// (depending on how you look at it)

#define PLANET_VIEW_DIAMETER 128
#define TEXTURE_SIZE 128			// should be same as planet_view_diameter


#define CHECKER_PLANET_BACKGROUND	0
#define TEST_TEST_DISPLAY 0

//#define PI 3.14159

#define SPECIAL_BLACK 244        //which is very dark blue
#define TRANSPARENT_COLOUR 255

#define NUM_GALAXIES_COLOURS 3

typedef struct { unsigned char red; unsigned char green; unsigned char blue; } colours_struct;

#define NUM_GALAXY_COLOURS 8

colours_struct galaxy_ctable[NUM_GALAXY_COLOURS][8]=
	{
	  { // black-grey-white
	    {   0,  0,  0 } , // 0
	    {  20, 20, 20 } , // 1
	    {  40, 40, 40 } , // 2
	    {  60, 40, 40 } , // 3
	    {  80, 40, 40 } , // 4
	    {  80, 80, 80 } , // 5
	    {  80, 80, 80 } , // 6
	    { 100,100,100 }   // 7
	  } ,

	  { // black-grey-white
	    {   0,  0,  0 } , // 0
	    { 100,100,100 } , // 1
	    { 150,150,150 } , // 2
	    { 200,200,200 } , // 3
	    { 200,200,200 } , // 4
	    { 200,200,200 } , // 5
	    { 200,200,200 } , // 6
	    { 200,200,200 }   // 7
	  } ,

  	  { // black-grey-white
	    {   0,  0,  0 } , // 0
	    {  36, 36, 36 } , // 1
	    {  72, 72, 72 } , // 2
	    { 108,108,108 } , // 3
	    { 144,144,144 } , // 4
	    { 180,180,180 } , // 5
	    { 200,200,200 } , // 6
	    { 200,200,200 }   // 7
	  } ,

	  { // black-red-white
	    {   0,  0,  0 } ,  // 0
	    { 100,100,100 } ,  // 1
	    { 200,100,100 } ,  // 2
	    { 250,150,150 } ,  // 3
	    { 200,150,150 } ,  // 4
	    { 200,150,150 } ,  // 5
	    { 200,200,200 } ,  // 6
	    { 200,200,200 }    // 7
	  } ,

	  { // black-orange
	    {   0,  0,  0 } , // 0
	    { 100, 50,  0 } , // 1
	    { 150,100,  0 } , // 2
	    { 180,120,  0 } , // 3
	    { 200,130,  0 } , // 4
	    { 200,130,  0 } , // 5
	    { 200,100,  0 } , // 6
	    { 200,125,  0 }   // 7
	  } ,

	  { // black-blue-purple
	    {   0,  0,  0 } , // 0
	    {  70,  0,150 } , // 1
	    {  70,  0,200 } , // 2
	    {  70,  0,200 } , // 3
	    {  70,  0,200 } , // 4
	    { 100,  0,200 } , // 5
	    { 120,  0,200 } , // 6
	    { 150,  0,200 }   // 7
	  } ,

	  { // black-blue-white
	    {   0,  0,  0 } , // 0
	    {   0,  0,150 } , // 1
	    {  50, 50,200 } , // 2
	    {  50, 50,200 } , // 3
	    { 100,100,200 } , // 4
	    { 150,150,200 } , // 5
	    { 200,200,200 } , // 6
	    { 200,200,200 }   // 7
	  } ,

	  { // black-green-yellow-white
	    {   0,  0,  0 } , // 0
	    {  80,100, 80 } , // 1
	    { 100,100,  0 } , // 2
	    { 100,150,  0 } , // 3
	    { 150,150, 50 } , // 4
	    { 180,180,100 } , // 5
	    { 200,200,200 } , // 6
	    { 200,200,200 }   // 7
	  }

    };



// --------------------------------------------------------

// levels for the different levels of the planet
#define SEA_START 0
#define SEA_END 0
#define SHORE_START 1
#define SHORE_END 1
#define PLAINS_START 2
#define PLAINS_END 3
#define MOUNTAINS_START 4
#define MOUNTAINS_END 5
#define CLOUDS_START 6
#define CLOUDS_END 7


#define NUM_PLANET_COLOURS 15

// which planets are
#define NUM_INHABITED_COLOURS 4
#define NUM_UNINHABITED_COLOURS (NUM_PLANET_COLOURS-NUM_INHABITED_COLOURS)

colours_struct planet_table[NUM_PLANET_COLOURS][8]=
  {

	  { // forest moon
	    {   0, 32,100 } , // 0
	    {   0, 50,  0 } , // 1
	    {   0,100,  0 } , // 2
	    {   0,150,  0 } , // 3
	    {   0,150,  0 } , // 4
	    {   0,180,  0 } , // 5
	    {   0,200,  0 } , // 6
	    { 100,255,  0 }   // 7
	  } ,

	  { // black-green-yellow-white
	    {   0,  0,  0 } , // 0
	    {   0, 50,  0 } , // 1
	    {   0,100,  0 } , // 2
	    { 100,150,  0 } , // 3
	    { 150,150, 50 } , // 4
	    { 180,180,100 } , // 5
	    { 200,200,200 } , // 6
	    { 255,255,255 }   // 7
	  } ,

	  { // (almost) original colour set
	    {   0, 32,100 } , // 0
	    {  50, 50, 50 } , // 1
	    { 100,100,100 } , // 2
	    { 150,150,150 } , // 3
	    { 200,200,200 } , // 4
	    { 250,250,250 } , // 5
	    {   0,  0,  0 } , // 6
	    { 255,255,255 }   // 7
	  } ,


	  { // black-blue-white
	    {   0,  0,  0 } , // 0
	    {   0, 20,  0 } , // 1
	    {   0,  0, 50 } , // 2
	    {  40, 40,100 } , // 3
	    {  40, 40,100 } , // 4
	    { 100,100,150 } , // 5
	    { 120,120,150 } , // 6
	    { 150,150,150 }   // 7
	  } ,


//
// NOW UNINHABITED PLANETS
//
	  { // black-blue-white
	    {   0,  0,  0 } , // 0
	    {   0,  0, 50 } , // 1
	    {  40, 40,100 } , // 2
	    {  50, 50,150 } , // 3
	    {  80, 80,150 } , // 4
	    { 100,100,150 } , // 5
	    { 120,120,150 } , // 6
	    { 150,150,150 }   // 7
	  } ,


	  { // yellow to white
	    { 200,160,  0 } , // 0
	    { 200,160, 25 } , // 1
	    { 200,160, 50 } , // 2
	    { 200,160,100 } , // 3
	    { 200,160,125 } , // 4
	    { 200,160,150 } , // 5
	    { 200,160,175 } , // 6
	    { 200,160,200 }   // 7
	  } ,


	  { // brown - mercury
	    { 100, 40, 20 } , // 0
	    {  80, 30, 20 } , // 1
	    {  60, 20, 10 } , // 2
	    {  20, 10,  0 } , // 3
	    {  20,  0,  0 } , // 4
	    {   0,  0,  0 } , // 5
	    {   0,  0,  0 } , // 6
	    {   0,  0,  0 }   // 7
	  } ,

	  { // yellow-red-black
	    { 250,125,  0 } , // 0
	    { 250,250,  0 } , // 1
	    { 250,  0,  0 } , // 2
	    {   0,  0,  0 } , // 3
	    { 250,125,  0 } , // 4
	    { 250,250,  0 } , // 5
	    { 250,  0,  0 } , // 6
	    {   0,  0,  0 }   // 7
	  } ,

	  { // brickred mars
	    { 100, 40,  0 } , // 0
	    {  90, 20,  0 } , // 1
	    {  80,  0,  0 } , // 2
	    {  70,  0,  0 } , // 3
	    {  60,  0,  0 } , // 4
	    {  50,  0,  0 } , // 5
	    {  40,  0,  0 } , // 6
	    {  30,  0,  0 }   // 7
	  } ,

	  { // blue neptune
	    {   0, 20,100 } , // 0
	    {   0, 20,100 } , // 1
	    {   0, 20,100 } , // 2
	    {   0, 20,100 } , // 3
	    {   0, 20,100 } , // 4
	    {   0, 20,100 } , // 5
	    {   0, 40,100 } , // 6
	    { 200,200,200 }   // 7
	  } ,

	  { // green uranus
	    {  10,100, 20 } , // 0
	    {  10,100, 20 } , // 1
	    {  10,100, 20 } , // 2
	    {  10,100, 20 } , // 3
	    {  10,100, 20 } , // 4
	    {  10,100, 20 } , // 5
	    {   0, 80,  0 } , // 6
	    { 100,200,  0 }   // 7
	  } ,

	{ // red, blue point
	    {  40,  0,  0 } , // 0
	    {  50,  0,  0 } , // 1
	    {   0,  0,  0 } , // 2
	    {   0,  0,  0 } , // 3
	    {   0,  0,  0 } , // 4
	    {   0,  0,  0 } , // 5
	    {   0,  0,250 } , // 6
	    { 250,250,250 }   // 7
	  } ,

	{ // grey rock
	    {  10, 10, 10 } , // 0
	    {  20, 20, 20 } , // 1
	    {  30, 30, 30 } , // 2
	    {  30, 30, 30 } , // 3
	    {  30, 30, 30 } , // 4
	    {  30, 30, 30 } , // 5
	    {  30, 30, 30 } , // 6
	    {   0,  0,  0 }   // 7
	  } ,

	{ // purple rock
	    {  30,  0, 50 } , // 0
	    {  30,  0, 50 } , // 1
	    {  20,  0, 60 } , // 2
	    {  50,  0, 60 } , // 3
	    {  50,  0, 70 } , // 4
	    {  60,  0, 70 } , // 5
	    {  00,  0, 00 } , // 6
	    {   0, 30,  0 }   // 7
	  } ,

	{ // orange rock
	    {  80, 30,  0 } , // 0
	    {   0,  0,  0 } , // 1
	    {  80, 30,  0 } , // 2
	    {   0,  0,  0 } , // 3
	    {  60,  0,  0 } , // 4
	    {  80,  0,  0 } , // 5
	    {   0,  0,  0 } , // 6
	    {   0,  0,  0 }   // 7
	  }

  };



// types to pass
#define UNINHABITED_PLANET 0			// inhabited planet is not this


// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


void planet_gen(int seed, int is_inhabited, unsigned char *scratch_address, unsigned char *output_address);
void planet_paint(int colour);
void translate_planet(unsigned char *output_address, int diameter, int angle, int inhabited);
void planet_plot(int lati, int longi, int colour);
int planet_get_point(int lati, int longi);
void sphere_line(int long1, int lat1, int long2, int lat2, int colour);
void build_square(int x, int y, int height);
void strip_land(void);
void galaxy_gen(int seed, unsigned char *output_address, int squish_flag);
void pitted_land(void);

// +--------------------------------+--------------------------+----------------------
// | TITLE: galaxy_gen              | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  handle generation of galaxies into an array.
// | The output-address should be 128*128 bytes = 16384 bytes
// +----------------------------------------------------------------ROUTINE HEADER----

void galaxy_gen(int seed, unsigned char *output_address, int squish_flag)
{
char my_text[80];



int i, colour_sel,j,arms;
long *temp_ptr;
float theta, r, randx, randy;
float x,y,theta2;
int red,green,blue;
unsigned char *temp_address;

// the parameters

int max_iterations,cut;
float in,fuzziness,scale, scaleb;


// fill the texture with nothings

temp_ptr=(long *)output_address;
for(i=0; i< ((TEXTURE_SIZE*TEXTURE_SIZE)>>2); i++)
  {
    *temp_ptr=0; temp_ptr++;
  }


internal_srand(seed);


// ==== input parameters ====

max_iterations=1200;

switch(((unsigned)rand16())%8)
 {
 case 0: fuzziness=4; break;
 case 1: fuzziness=6; break;
 case 2: fuzziness=7; break;
 case 3: fuzziness=8; break;
 case 4: fuzziness=10; break;
 case 5: fuzziness=20; break;
 case 6: fuzziness=20; break;
 default: fuzziness=25; break;
 }

cut=((TEXTURE_SIZE>>1)-5);  // +/- 63
scale=3;

if(squish_flag!=0)
  {
  scaleb=((float)((((unsigned)rand16())%6)+1))/6.0;
  }
else
  {
  scaleb=1;
  }

// ========================
// actually draw the galaxy
// ------------------------

// keep stars away from edges  [poss done...]
// backgrounds not black		[poss done...]
// get rid of dots (anti-alias) [poss done...]
// more variety of of colour
// make gas clouds
// alter arm twists [poss done...]

// planets					[poss done...]
// planet clipping
// colours					[poss done...]
// more sizeable islands	[poss done...]

//opengl
//transparent overlaying


arms = (((unsigned)rand16()) %5) +2;		// 2, 3, 4, 5 or 6 arms

switch(arms)
  {
  case 2: i=6;
  case 3: i=5;
  case 4: i=8;
  case 5: i=10;
  default: i=11;
  }

in=0.05 + (0.05 * ((((unsigned)rand16())%3)+i));

for(j=0; j<arms; j++)
  {
  /* arms */
  for(i=0; i<max_iterations; i++)
    {
    theta=((float)i) /50;
    theta2=(((float)i) /50) - (2*PI*((float)j/(float)arms)); // e.g. 0/5pi 1/5pi 2/5pi 3/5pi 4/5pi
    r=scale*exp(theta2 * tan(in));
    if(r <= cut)
      {
      x=r*cos(theta);  y=scaleb*r*sin(theta);
      x+=(TEXTURE_SIZE>>1); y+=(TEXTURE_SIZE>>1);
      randx=((float)(((unsigned)rand16())&65535))/65535;
      randy=((float)(((unsigned)rand16())&65535))/65535;

      temp_address=output_address+(127&(int)(x+fuzziness*randx)) + 128*(127&(int)(y+fuzziness*randy));
      *temp_address+=2;
      temp_address++; *temp_address+=1;
      temp_address-=2; *temp_address+=1;
      temp_address+=TEXTURE_SIZE+1; *temp_address+=1;
      temp_address-=2*TEXTURE_SIZE; *temp_address+=1;
      }
    }
  }

// now we need to scan the whole texture, and convert it to real colours.

colour_sel = (((unsigned)rand16()) % (NUM_GALAXY_COLOURS-1));

for(i=0; i< (TEXTURE_SIZE*TEXTURE_SIZE); i++)
  {

  if(*output_address>7)			// *output_address is never less than 0 ... because it's unsigned
    {
    *output_address=7;
    }


  red = galaxy_ctable[colour_sel][*output_address].red;
  green = galaxy_ctable[colour_sel][*output_address].green;
  blue = galaxy_ctable[colour_sel][*output_address].blue;

  *output_address=convert_to_palette(red,green,blue);
  output_address++;
  }

}



// +--------------------------------+--------------------------+----------------------
// | TITLE: planet_gen              | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  handle generation of planets into an array.
// | The output-address should be 128*128 bytes
// | The scratch-address should be 256*256 = 64Kbytes            ??????+128*128????
// +----------------------------------------------------------------ROUTINE HEADER----

unsigned char *planet_address;
unsigned char *sunlite_address;


void planet_gen(int seed, int is_inhabited, unsigned char *scratch_address, unsigned char *output_address)
{
int i,flag;
int single_colour;
int colour1, colour2;
int rd, gr, bl;

unsigned char *sunlite_address_copy;
unsigned char *output_address_copy;

planet_address=scratch_address;		 	 	 	 // surface of planet is defined in scratch address
sunlite_address=scratch_address+((MAX_ANGLE+1)*(MAX_ANGLE+1));


internal_srand(seed);




//sphere_line(10,10,300,150, 253);

//switch(((unsigned)rand16()) % 5)
//  {
//  case 0:
			planet_paint(0);							// colour the whole planet, say, pink.
 		   strip_land();
//			break;

//  case 1:
//  		  	  planet_paint(0);
//			pitted_land();
//  		  	  break;

/*  case 2:	planet_paint(0);
			continents_land();
  			break;

  case 3: 	planet_paint(0);
			cracked_land();
  			break;

  case 4:	planet_paint(0);
			break;

  default: 	planet_paint(0);
			break;
  }
 */

translate_planet(output_address, PLANET_VIEW_DIAMETER, 0, is_inhabited);	// translate the planet to a 2d view


// check for a totally black planet

flag=0;
output_address_copy=output_address;
single_colour=SPECIAL_BLACK;
for(i=0; i<(TEXTURE_SIZE*TEXTURE_SIZE); i++)
  {
  if((*output_address_copy)!=255 && (*output_address_copy)!=single_colour)
    {
    if(single_colour==SPECIAL_BLACK)
      {
      single_colour=*output_address_copy;
      }
    else
      {
      flag=1;
      break;
      }
    }
  output_address_copy++;
  }

// --------------- make black planets a colour
sunlite_address_copy=sunlite_address;
output_address_copy=output_address;

if(flag==0)
  {

  colour1 = ((unsigned)rand16())%217;  // 0-253
//  rd = (colour1/36);
//  gr = ((colour1/6)%6);
//  bl = (colour1 % 6);

//  if(rd<3) { rd=4; } else { rd=5; }
//  if(gr<3) { gr=4; } else { gr=5; }
//  if(bl<3) { bl=4; } else { bl=5; }

  colour2 = SPECIAL_BLACK; // 36 * rd + 6 * gr + bl;
//  if(colour2==216) colour2=SPECIAL_BLACK;


//  y_ang=((rand16()%100)/100)*PI;
//  x_ang=((rand16()%100)/100)*PI;
//  radius=TEXTURE_SIZE/2;

  for(i=0; i<(TEXTURE_SIZE*TEXTURE_SIZE); i++)
    {
    if(*output_address_copy==SPECIAL_BLACK || *output_address_copy==single_colour)
      {
        if(*sunlite_address_copy==0)
        {
	      *output_address_copy=colour2;
        }
	    else
	    {
	      *output_address_copy=colour1;
	    }
      }
    output_address_copy++; sunlite_address_copy++;
    }


/* for shading --- needs fixing
  for(y=-radius; y<=radius; y+=1.005)
    {
    for(x=-radius; x<=radius; x+=1.005)
      {
      if(asin(sqrt(y*y+x*x)/radius)<
      }
    }  */

  }





}


// +--------------------------------+--------------------------+----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----
//

#define PITT_HEIGHT_MASK 15

void pitted_land(void)
{
int i;
int x, y;

for(i=0; i<((rand16()%800)+200); i++)
  {
  x=((unsigned)rand16()) % MAX_ANGLE;
  y=((unsigned)rand16()) % MAX_ANGLE;

//  build_circle(x, y, PITT_HEIGHT_MASK & ((unsigned)rand16()) );
  }

}


// +--------------------------------+--------------------------+----------------------
// | TITLE: mountain_planet         | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  draw mountains on the planet
// +----------------------------------------------------------------ROUTINE HEADER----
// based on triangles


// we can build triangles using squares ...

//  _________  or  _________
//  |\      |      |      /|
//  |  \    |      |    /  |
//  |    \  |      |  /    |
//  |      \|      |/      |
//  ---------      ---------


// +--------------------------------+--------------------------+----------------------
// | TITLE: cloud_planet            | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  draw clouds on the planet
// +----------------------------------------------------------------ROUTINE HEADER----
// based on squares





// +--------------------------------+--------------------------+----------------------
// | TITLE: trial_random            | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  draws junk on planets
// +----------------------------------------------------------------ROUTINE HEADER----
// based on squares

// from c stdlib
//
//int rand16 (void);
//void internal_srand (unsigned int seed);

#define HEIGHT_MASK 31
#define MOVEMENT_MASK 7
#define ITERATIONS_PER_TREE 30

#define NUM_LANDMASSES_MASK 15

#define DIR_LIMITER 31

void strip_land(void)
 {
 int x, y;
 float dir_x;
 float dir_y;
 int a;
 int lands;
 int lands_count;
 int iter_per_tree;

 lands=((unsigned)rand16()) & NUM_LANDMASSES_MASK;
 lands+=10;
 lands_count=0;
 iter_per_tree=(rand16()%ITERATIONS_PER_TREE)+10;

 while(lands_count<lands)
   {
   x=((unsigned)rand16()) & MAX_ANGLE;
   y=((unsigned)rand16()) & MAX_ANGLE;

   dir_x=(((float)(((unsigned)rand16()) % DIR_LIMITER)) ) +10;
   dir_y=(((float)(((unsigned)rand16()) % DIR_LIMITER)) ) ;


   for(a=0; a<iter_per_tree; a++)
     {
     build_square(x, y, HEIGHT_MASK & ((unsigned)rand16()) );
     x +=  static_cast<int>(dir_x + (((unsigned)rand16()) & MOVEMENT_MASK) - (MOVEMENT_MASK/2));
     y +=  static_cast<int>(dir_x + (((unsigned)rand16()) & MOVEMENT_MASK) - (MOVEMENT_MASK/2));
     }

   lands_count++;
   }


 }



// +--------------------------------+--------------------------+----------------------
// | TITLE: build_square            | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  build a square pyramid on the planet
// +----------------------------------------------------------------ROUTINE HEADER----

void build_square(int x, int y, int height)
{
int step=1;
int count;

planet_plot(x,y,planet_get_point(x,y)+height);
x++; y++; step+=2; height-=1;


while(height>0)
  {
  for(count=step; count>0; count--)  // up the right side */
    {
    planet_plot(x,y,planet_get_point(x,y)+height);
    y--;
    }

  for(count=step; count>0; count--)  // right on the top */
    {
    planet_plot(x,y,planet_get_point(x,y)+height);
    x--;
    }

  for(count=step; count>0; count--)  // down the left side */
    {
    planet_plot(x,y,planet_get_point(x,y)+height);
    y++;
    }

  for(count=step; count>0; count--)  // left on the bottom */
    {
    planet_plot(x,y,planet_get_point(x,y)+height);
    x++;
    }

  x++; y++; step+=2; height-=1;

  } // until all the height is exhusted

} // end of build_square()

// +--------------------------------+--------------------------+----------------------
// | TITLE: sphere_line             | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  draw a line on the sphere
// +----------------------------------------------------------------ROUTINE HEADER----

void sphere_line(int long1, int lat1, int long2, int lat2, int colour)
{
float slope_lat, slope_long;
int distance_long, distance_lat;
int abs_distance_long, abs_distance_lat;
int sign_long, sign_lat;

distance_long=long2-long1;
distance_lat=lat2-lat1;


if(distance_long >= 0) { abs_distance_long = distance_long; sign_long=1;}
  else { abs_distance_long = -distance_long; sign_long=-1;}

if(distance_lat >= 0) { abs_distance_lat = distance_lat; sign_lat=1; }
  else { abs_distance_lat = -distance_lat; sign_lat=-1; }

// need to step along longest side
if(abs_distance_long > abs_distance_lat)
  {
  slope_lat=distance_lat/abs_distance_long;

  planet_plot(lat1,long1,colour);
  while(abs_distance_long>0)
    {
    lat1+= static_cast<int>(slope_lat);
    long1+=sign_long;
    planet_plot(lat1,long1,colour);
    abs_distance_long--;
    }
  }
else
  {
  slope_long=distance_long/abs_distance_lat;

  planet_plot(lat1,long1,colour);
  while(abs_distance_lat>0)
    {
    long1+= static_cast<int>(slope_long);
    lat1+=sign_lat;
    planet_plot(lat1,long1,colour);
    abs_distance_lat--;
    }
  }

}



// +--------------------------------+--------------------------+----------------------
// | TITLE: planet_paint            | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  colour the surface of the planet a certain colour
// +----------------------------------------------------------------ROUTINE HEADER----

void planet_paint(int colour)
{
int lati, longi;

#if CHECKER_PLANET_BACKGROUND
  int bool1, bool2;
#else
  unsigned long *planet_address_copy;
  unsigned long long_colour;
#endif



#if !CHECKER_PLANET_BACKGROUND

planet_address_copy=(unsigned long *)planet_address;
colour=colour&255;
long_colour=colour<<24;
long_colour|=colour<<16;
long_colour|=colour<<8;
long_colour|=colour;

for(lati=0; lati<=((MAX_ANGLE*MAX_ANGLE)>>2); lati++)
  {
  *planet_address_copy=long_colour;
  planet_address_copy++;
  }

#else		// for a bit of fun, you can have the background as a checkboard effect - for testing only of course!

for(lati=0; lati<=MAX_ANGLE; lati++)
  {
  for(longi=0; longi<=MAX_ANGLE; longi++)
    {
    bool1=(((20*lati)/MAX_ANGLE)%2)==0;
    bool2=(((20*longi)/MAX_ANGLE)%2)==0;

    if ( (bool1 || bool2) && ( ! (bool1 && bool2 )) )  // xor
      {
      planet_plot(lati, longi, colour);
      }
    else
      {
      planet_plot(lati, longi, 255-colour);
      }
    }
  }

#endif


}


// +--------------------------------+--------------------------+----------------------
// | TITLE: translate_planet        | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  translate the surface of the plant to a 2d view
// +----------------------------------------------------------------ROUTINE HEADER----
// Other notes:
//
// If we need to perform colour translation from 32 bit to 8 bit at a later stage,
// this is where it will need to go.
//
void translate_planet(unsigned char *output_address, int diameter, int angle, int inhabited)
{
float lati, longi;

float y, x;
//int yc,xc;
float radius;
float radius_horizontal_section,radius_vertical_section,front_distance_to_point;
int pixel;
int red,green,blue;
int colour_sel;
unsigned char *sunlite_address_copy;
float angle_select;

radius=diameter/2;

sunlite_address_copy=sunlite_address;

angle_select=5+((rand16()%15)/2.0);  // 7.5 - 0, in 0.5 steps. total range 5 to 12.5


if(inhabited==UNINHABITED_PLANET)
  {
  colour_sel=(rand16()%NUM_UNINHABITED_COLOURS)+NUM_INHABITED_COLOURS;
  }
else
  {
  colour_sel=rand16()%NUM_INHABITED_COLOURS;
  }

 //4test colour_sel=13;

// lets plot for every front point
for(y=-radius; y<=(radius+3); y+=1.025) //1.005)		// there is a bug in the texture mapper - it doesn't plot
  {														// the last point on a 128*128 in either vertical horizontal
  for(x=-radius; x<=(radius+3); x+=1.025) //1.005)		// directions. Kludge here (radius+1) rather than radius.
    {
    radius_horizontal_section = sqrt(radius * radius - y * y);	// horizontal chop
    radius_vertical_section = sqrt(radius * radius - x * x); // veritcal chop
    front_distance_to_point = sqrt(x * x + y * y);

    if(front_distance_to_point > radius)		// if its outside the radius, don't calculate point on sphere
      {
      *output_address=TRANSPARENT_COLOUR;
     *sunlite_address_copy=0;
      }
    else
      {
      longi=acos(x/radius_horizontal_section);	// chopped horizontally at point on sphere, to determine angle of longitude disk (vertical)
      longi=(longi*(MAX_ANGLE+1)) / (2*PI);

      lati=acos(y/radius_vertical_section);		// latitude measurement - how far up the disk.
	  lati=(lati*(MAX_ANGLE+1)) / (2*PI);


	  // ====== CONVERT COLOUR =======
 	  pixel=255&(unsigned int)planet_get_point( static_cast<int>(lati+angle),  static_cast<int>(longi));


/*      if(pixel<10)
        {
        red=planet_table[colour_sel][0].red;
        green=planet_table[colour_sel][0].green;
        blue=planet_table[colour_sel][0].blue;
        }
      else
        { */
        red=planet_table[colour_sel][pixel/32].red;
        green=planet_table[colour_sel][pixel/32].green;
        blue=planet_table[colour_sel][pixel/32].blue;
 //       }


      pixel=convert_to_palette(red,green,blue);
      if(pixel==255) pixel=SPECIAL_BLACK;
      *output_address=pixel;

	if(lati>(MAX_ANGLE/angle_select) )
			{ *sunlite_address_copy=0; }
		else { *sunlite_address_copy=1; }  // debug line *output_address=0; }


      }

    output_address++;
    sunlite_address_copy++;
    } // for x

  } // for y

} // end of translate planet


// +--------------------------------+--------------------------+----------------------
// | TITLE: planet_plot              | AUTHOR(s): Rob Probin    | DATE STARTED: 4/8/99
// +
// | DESCRIPTION:  Plot a point on the surface of the sphere
// +----------------------------------------------------------------ROUTINE HEADER----

void planet_plot(int lati, int longi, int colour)
{

lati &= MAX_ANGLE;  longi &= MAX_ANGLE;			// ensure within range - including negatives

lati *= (MAX_ANGLE+1);							// find the line offset

*(planet_address + longi + lati) = colour;

}

// +--------------------------------+--------------------------+----------------------
// | TITLE: planet_get_point        | AUTHOR(s): Rob Probin    | DATE STARTED: 4/8/99
// +
// | DESCRIPTION:  Plot a point on the surface of the sphere
// +----------------------------------------------------------------ROUTINE HEADER----

int planet_get_point(int lati, int longi)
{

lati &= MAX_ANGLE;  longi &= MAX_ANGLE;			// ensure within range - including negatives

lati *= (MAX_ANGLE+1);							// find the line offset

return *(planet_address + longi + lati);

}




// ==============================================================================

#if PLANET_GEN_DEBUG

// ***   **** ****  *   *  ***      ***   ***  ****  ****
// *  *  *    *   * *   * *   *    *   * *   * *   * *
// *   * **   ****  *   * *        *     *   * *   * **
// *   * *    *   * *   * *  ***   *     *   * *   * *
// *  *  *    *   * *   * *   *    *   * *   * *   * *
// ***   **** ****   ***   ***      ***   ***  ****  ****

void test_display(unsigned char *pixel_address, int u, int v);
void init_mac(void);
void main(void);

// +--------------------------------+--------------------------+----------------------
// | TITLE: main                    | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION: sets up the environment to allow the code to be tested.
// |
// +----------------------------------------------------------------ROUTINE HEADER----


unsigned char output_stuff[ PLANET_VIEW_DIAMETER*PLANET_VIEW_DIAMETER ];
unsigned char input_buffer[ ((MAX_ANGLE+1)*(MAX_ANGLE+1))*2 ];

void main(void)
{
Ptr screenaddr;
int screenwidth;
GDHandle mygdhandle;

EventRecord	theEvent;
WindowPtr ww;
Rect my_rect;
char my_text[80];
int a;

init_mac();

// don't need screen address as we are NOT testing a screen primitive
//
//mygdhandle=GetMainDevice();
//
//screenaddr=(*((*mygdhandle)->gdPMap))->baseAddr;
//screenwidth=0x3fff & (*((*mygdhandle)->gdPMap))->rowBytes;
//

//setup_fgouraud();

SetRect(&my_rect,50,50,400,400);

ww=NewCWindow(0,&my_rect,"title",1,0,(struct GrafPort *)-1,0,0);
if(ww==0) { return; }

SetPort(ww);
MoveTo(20,20);

sprintf(my_text,"some test text       ");
DrawText(my_text,0,20);

LineTo(30,10);

// put test code in here

for(a=30; a<70; a++)
  {
  planet_gen(a,rand16()%2,input_buffer,output_stuff);
  test_display(output_stuff,40,40);
  }

//for(a=0; a<160; a+=80)
//  {
//  translate_planet(output_stuff, PLANET_VIEW_DIAMETER, a);	// translate the planet to a 2d view
//  test_display(output_stuff,40,40);
//
//  }

#if 0

for(a=0; a<10; a++)
  {

a=0;

do
  {
galaxy_gen(a+200, output_stuff, 0);
test_display(output_stuff,40,200);

a++;

  WaitNextEvent(everyEvent, &theEvent, 0xffffffff, 0);

} while(theEvent.what!=mouseDown);




  }
#endif
// end of test

MoveTo(100,350);
sprintf(my_text,"Click mouse to finish");
DrawText(my_text,0,22);


do {
	WaitNextEvent(everyEvent, &theEvent, 0xffffffff, 0);
	} while(theEvent.what!=mouseDown);


}


// +--------------------------------+--------------------------+----------------------
// | TITLE: test_display            | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION:  Actaully calls the planet_gen and displays the results in the window
// | provided by main.
// +----------------------------------------------------------------ROUTINE HEADER----

void test_display(unsigned char *pixel_address, int u, int v)
{
int x, y;
int red, green, blue;
int pixel;

RGBColor my_colour;

for(x=0; x<PLANET_VIEW_DIAMETER; x++)
  {
  for(y=0; y<PLANET_VIEW_DIAMETER; y++)
    {

#if TEST_TEST_DISPLAY
     *pixel_address=(x>>3)+(y>>3)*16;
#endif

    convert_to_rgb(255&(unsigned int)*pixel_address, &red, &green, &blue);

    my_colour.red=red<<8;
    my_colour.green=green<<8;
    my_colour.blue=blue<<8;




    RGBForeColor(&my_colour);
    MoveTo(x+u,y+v);
    LineTo(x+u,y+v);

    pixel_address++;

    }

  }

}





// +--------------------------------+--------------------------+----------------------
// | TITLE: init_mac                | AUTHOR(s): Rob Probin    | DATE STARTED: 30/8/99
// +
// | DESCRIPTION: standard mac setup code
// |
// +----------------------------------------------------------------ROUTINE HEADER----


#if !(MAC68K)
QDGlobals qd;
#endif

void	init_mac(void)
{
	InitGraf(&qd.thePort);
	InitFonts();
	LS_FlushEvents(everyEvent, 0);
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();
	MaxApplZone();

}


#endif





