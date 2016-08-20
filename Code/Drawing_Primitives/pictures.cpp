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

//pictures
//SB - Lightsoft SW 8/12/98

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <mactypes.h>
//oldmac//#include <resources.h>

//oldmac//#include <intrinsics.h>
#include "splat_control.h"
#include "pictures.h"
#include "Error_handler.h"
#include "3d_structures_def.h"
#include "texture_management.h"
#include "OGL.h"

#include "zex_misc.h"


void internal_erase_zex_rect_colour(int w, int h);






//print_pict splats a RAW picture to the current drawing buffer. The RAW file MUST
//have the width and hieght as the first two entries 
void print_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;

for (i=0;i<picture_h;i++)	//line loop
 {
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   *splat_pix=(*raw_data);
   raw_data++;
   splat_pix++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}


#if 1


extern int exec_watchdog;

char all_f[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char erase_colour;
//sets the colour erase_zex_rect fills with
void set_erase_colour(unsigned char colour_8)
{
int i;
 for (i=0;i<8;i++)
 all_f[i]=colour_8;
}

//assumes a 640 * 390 picture
void erase_zex_rect(int w, int h)
{
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
Ptr start_pix;
Float64 src_float,*dest_float;
Float64* load_float;
char *dest;
int pix,line,wdiv8;


//    reset_watchdog();	//have to keep reseting this else we'll error
    
load_float=(Float64*)&all_f[0];
src_float=*load_float;

start_pix=(char*)screenaddr;
dest=start_pix;
dest_float=(Float64 *) dest;
wdiv8=w>>3;

for (line=0;line<(h);line++)
    {

    for (pix=0;pix<wdiv8;pix++)
          *dest_float++=src_float;

//    for (pix=0;pix<(wdiv8>>3);pix++)
//    {
//          *dest_float++=src_float;
//          *dest_float++=src_float;
//          *dest_float++=src_float;
//          *dest_float++=src_float;
// 
//          *dest_float++=src_float;
//          *dest_float++=src_float;
//          *dest_float++=src_float;
//          *dest_float++=src_float;
// 
//  
//    }

    dest=dest+screenwidth;
    dest_float=(Float64 *) dest;




    }	//do next line

}

#endif


#if 0

// assumption - lines must be more than 64 bytes wide
// assumption2 - screenwidth is wholly divisible by 8 (octal aligned)
// assumption3 - screen address is wholly divisible by 8 (octal aligned)
// width is divisable by 8 (octal aligned) 

void erase_zex_rect(int w, int h)
{
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;

int line, count;
char *dest;
char *linedest;

if (erase_colour!=0) 	//if a bg colour is set, we can't use dcbz stuff
 {
  internal_erase_zex_rect_colour(w,h);
  return;
 }

// double check the assumptions

if(0x07 & screenwidth) { return; }
if(0x07 & (int)screenaddr) { return; }
if(w < 64) { return; }
if(w & 0x07) { return; }

linedest=screenaddr;
w=w>>3;			// don't care about less than octals

// ---- THE LOOP -----

for(line=0; line<h; line++)
  {
  count=w;		// set up line width
  dest=linedest;	// reload destination for line

  while(0x1f & (int)dest)    {    *(Float64 *)dest=0;  dest+=8; count--;  }	// start octals

  while(~0x1f&count)         {    __dcbz(dest,0);  dest+=32;  count-=4; }	// clear 32 bytes at a time using dcbz

  while(0x1f&count)    	{    *(Float64 *)dest=0;  dest+=8; count--;  }	// end octals

  linedest+=screenwidth;
  } 
  
}


#endif



unsigned char  double_colour[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char old_colour=0xff;
//assumes a 640 * 390 picture
void erase_zex_rect_colour(int w, int h, unsigned char colour_8)
{
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
Ptr start_pix;
Float64 src_float,*dest_float;
Float64* load_float;
char *dest;
int pix,line,wdiv8;
int i;

//    reset_watchdog();	//have to keep reseting this else we'll error

if (colour_8!=old_colour)
{
 for (i=0;i<8;i++)
 double_colour[i]=colour_8;
 old_colour=colour_8;
}


load_float=(Float64*)&double_colour[0];
src_float=*load_float;

start_pix=(char*)screenaddr;
dest=start_pix;
dest_float=(Float64 *) dest;
wdiv8=w>>3;

for (line=0;line<h;line++)
    {
   for (pix=0;pix<wdiv8;pix++)
         *dest_float++=src_float;

     dest=dest+screenwidth;
     dest_float=(Float64 *) dest;


    }	//do next line

}

//and clips and fills with black where picture is not drawn
void print_pict_quick(LSRAW *the_picture, int x, int y)
{
UInt32 picture_w,picture_h,i,j,line,pix;
Ptr raw_data;
Ptr splat_pix,start_pix;
Float64 *src_float,*dest_float;
char *src,*dest;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
int x_index;	//index into each line of the picture at where we start drawing
int x_fill;	//how many pixels at the start of the line we have to fill with black
int x_draw;	//how many pixels from the start of the line do we have to draw
int x_end;	//how many black pixels do we have to fill atthe ned of a line
int src_start_pix;	//the first pixel in the data we have to copy
//picture_w=(*the_picture).pwidth;
//picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr;
dest=start_pix;
src=raw_data;

src_float=(Float64 *) raw_data;
dest_float=(Float64 *) start_pix;


src_float=(Float64 *) raw_data;
dest_float=(Float64 *) start_pix;

for (line=0;line<390;line++)
    {
    //now the lines
   for (pix=0;pix<640/8;pix++)
         *dest_float++=*src_float++;

     src = src+640;	//point to next src line;
     dest=dest+screenwidth;

     src_float=(Float64 *) src;
     dest_float=(Float64 *) dest;

    }	//do next line

}

//Ptr black_line(Ptr dest,int pixel_count)
//{
//}
//
//Ptr copy_line(Ptr dest_float, Ptr src_float, int start_pix, int pixel_count)
//{
//}

//print_crunched_pict splats a crunched RAW picture to the current drawing buffer. The RAW file MUST
//have the width and hieght as the first two entries
//See ZEX_Cruncher for details. 
//clips in h

void print_crunched_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour=0;		// rle_colour is always reloaded but to avoid warnings load with 0 initially
int actual_x;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  actual_x=x;	 //so we can keep track of where we are x wise
 
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
   {
    rle_length=(*raw_data);
    raw_data++;
    rle_colour=(*raw_data);
    raw_data++;
   }
   if (actual_x>0 && actual_x<639)
   {
    *splat_pix=rle_colour;
    splat_pix++;
   }
    rle_length--;
    actual_x++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}


//cinemascope version of above
void print_crunched_pict_C(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 top, UInt32 bott)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour=0;		// rle_colour is always reloaded but to avoid warnings load with 0 initially
int actual_x;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  actual_x=x;	 //so we can keep track of where we are x wise
 
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
   {
    rle_length=(*raw_data);
    raw_data++;
    rle_colour=(*raw_data);
    raw_data++;
   }
   if (actual_x>0 && actual_x<639 && i>top && i<bott)
   {
    *splat_pix=rle_colour;
    splat_pix++;
   }
    rle_length--;
    actual_x++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}


#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING	// this function is not used and contains OGL commands.
void print_crunched32_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour;
int actual_x;
GLuint texture_name;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
extern int OGL_active;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
LSRAW* pictbuffer;
LSRAW* pictbuffer_rgba;

Ptr raw_rgba_data;
int err;
char pix;

picture_w=	(*the_picture).pwidth;
picture_h=	(*the_picture).pheight;	//now pointing to data


raw_rgba_data=(char *)&the_picture->raw_data;
if (OGL_active==1)
{

y=y+picture_h;	//ogl origin is bottom left



    raw_rgba_data=(char *)&the_picture->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
//    glPixelStorei(GL_PACK_ALIGNMENT, 1); 

    glRasterPos2f(int_to_ogl_x(x), int_to_ogl_y(y));
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);


   DisposePtr((Ptr)pictbuffer);
   DisposePtr((Ptr)pictbuffer_rgba);

}
}
#endif

//same as above but colour 255 is transparent
#if OGL==1
void print_crunched_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour;
int actual_x;
GLuint texture_name;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
extern int OGL_active;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
LSRAW* pictbuffer;
LSRAW* pictbuffer_rgba;

Ptr raw_rgba_data;
int err;
char pix;

picture_w=	(*the_picture).pwidth;
picture_h=	(*the_picture).pheight;	//now pointing to data


raw_data=(char *)&the_picture->raw_data;
if (OGL_active==1)
{

y=y+picture_h;	//ogl origin is bottom left

//expand the crunched pict into some memory
    pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
    if (pictbuffer==0) report_error_c("Memory Error in print_crunched_trans_pict(OGL),ID=1","",-1);
    decrunch_pict(the_picture,pictbuffer);
    invert_y_pict(pictbuffer);
    pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
    if (pictbuffer_rgba==0) report_error_c("Memory Error in print_crunched_trans_pict(OGL),ID=2","",-1);
//now load up the texture
    raw_data=(char *)&pictbuffer->raw_data;
    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
//
    convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);

    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
//    glPixelStorei(GL_PACK_ALIGNMENT, 1); 

    glRasterPos2f(int_to_ogl_x(x), int_to_ogl_y(y));
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);


   DisposePtr((Ptr)pictbuffer);
   DisposePtr((Ptr)pictbuffer_rgba);

}
else	//not ogl
{





start_pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  actual_x=x;	 //so we can keep track of where we are x wise
 
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
   {
    rle_length=(*raw_data);
    raw_data++;
    rle_colour=(*raw_data);
    raw_data++;
   }
   if (actual_x>0 && actual_x<639)
   { 
    if (rle_colour!=-1) *splat_pix=rle_colour;	//colour 255 is transparent
    splat_pix++;
   }
    rle_length--;
    actual_x++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}	//end of not ogl
}

#else
void print_crunched_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour=0;		// rle_colour is always reloaded but to avoid warnings load with 0 initially
int actual_x;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  actual_x=x;	 //so we can keep track of where we are x wise
 
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
   {
    rle_length=(*raw_data);
    raw_data++;
    rle_colour=(*raw_data);
    raw_data++;
   }
   if (actual_x>0 && actual_x<639)
   { 
    if (rle_colour!=-1) *splat_pix=rle_colour;	//colour 255 is transparent
    splat_pix++;
   }
    rle_length--;
    actual_x++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}
#endif



//same as above but translucent - 150999
void print_crunched_translucent_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour=0;		// rle_colour is always reloaded but to avoid warnings load with 0 initially
int actual_x;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
extern char transparency_table[];

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  actual_x=x;	 //so we can keep track of where we are x wise
 
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
   {
    rle_length=(*raw_data);
    raw_data++;
    rle_colour=(*raw_data);
    raw_data++;
   }
   if (actual_x>0 && actual_x<639)
   { 
    if (rle_colour!=-1) *splat_pix=transparency_table[((*(unsigned char *)splat_pix)<<8) + rle_colour];;	//colour 255 is transparent
    splat_pix++;
   }
    rle_length--;
    actual_x++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}



Handle GetZexPicture (unsigned long res_type, short resid)
{
Handle hpic;
 
 hpic = (Handle) LS_GetResource(res_type,resid);  //Get the Handle to the Resource 
 if (hpic==0 || ResError()!=0) report_error_c("GetZexPicture: Picture resource missing","",resid);
 LS_DetachResource (hpic);

#if ARCH_LITTLE_ENDIAN	//Just not natural IMNSFHO but we have to deal with it now. Still, 2 Meg of L2 cache!
	 LSRAW* temp_picture=(LSRAW*)*hpic;	//Handle
	 (*temp_picture).pwidth=Endian16_Swap((*temp_picture).pwidth);
	 (*temp_picture).pheight=Endian16_Swap((*temp_picture).pheight);
#elif !ARCH_BIG_ENDIAN
#error "Endian defines missing"
#endif

 return hpic;
}



//prints a raw scaled by sf - sf may be a number between 0 (no scaling)
//and about 20 which means we only take every 20th pixel and line
void print_pict_scaled(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 sf, UInt32 trans_on)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char colour;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
unsigned int pixel_skip_counter, line_skip_counter;
picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;
pixel_skip_counter=0;
line_skip_counter=0;

if (y>479) return;	//completely off the bottom of the screen

for (i=0;i<picture_h;i++)	//line loop
 {
  splat_pix=start_pix;		//splat pixels to here
  
  if(line_skip_counter==sf)	//only plot if pix and line =sf
  {
    line_skip_counter=0;
    for (j=0;j<picture_w;j++)	//pixel loop
     {
      
      if (pixel_skip_counter==sf)
       {
         pixel_skip_counter=0;	 //reset pixel skip
         colour=(*raw_data);
         if (trans_on==1)
         {
         if (colour!=-1) *splat_pix=colour;
         }
         else
         *splat_pix=colour;
         
         splat_pix++;
       }
       else    pixel_skip_counter++;
      
      raw_data++;
     }	//end of for
   start_pix+=screenwidth;	//next line of dest
   }	//end of if lineskip
   else	//we have to skip a line
   {
    raw_data+=picture_w;	//skip in source
    line_skip_counter++;
   }
//  line_skip_counter=0;
//  }	//end of line_skip==sf
//  else	//line skip<>sf
//  {
//   raw_data+=picture_h;	//skip to next line of source
//   line_skip_counter++;
//  }
  
  pixel_skip_counter=0;
 } //end of line loop
}


//magnifies a picture. Takes a floating magnification value
//which must be >=1.0
//wont shrink pictures, see above for that.
//does full clipping assuming y is zero ,if not you'll crash with a bad heap if picture extends off
//bottom of screen. Same for x.
//this can be speeded up by calculating start clip x and y
void print_pict_magnified(LSRAW *the_picture, UInt32 x, UInt32 y, int y_correction, int x_correction, float magnification)
{
UInt32 picture_w,picture_h;
int i,j;
Ptr raw_data;
Ptr splat_pix,start_pix,vram_start,vram_end;
char colour;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
int pixel_skip_counter, line_skip_counter;
int trans_on=0;
float source_x_index_inc,source_y_index_inc;
float source_x_index,source_y_index;
int start_x,actual_x,actual_y;
int dest_width,dest_h;
int temp_y;


// no picture or a stupid value, get out. Maybe should check mag < 1.0.
if(magnification <= 0) { report_error_c("Illegal magnification value in print_pict_magnified()","", 0); return; }

source_x_index=0; source_y_index=0;	//take data from here

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;


start_pix=(char*)screenaddr+(y*screenwidth)+x;

source_x_index_inc=1/magnification;	//index into source
source_y_index_inc=1/magnification;
dest_width= static_cast<signed int>(picture_w*magnification);	//how wide final picture is
dest_h= static_cast<signed int>(picture_h*magnification);
actual_x=x-(dest_width-640)/2;
actual_y=y-(dest_h-480)/2;
actual_y+=y_correction;
actual_x+=x_correction;
start_x=actual_x;

 for (i=0;i<dest_h;i++)	//line loop
  {
  if (actual_y>479) break;	//y clip - all done
  if (actual_y>=0)	//y clip
  {
   splat_pix=start_pix;		//splat pixels to here
   temp_y=(int)source_y_index*picture_w;
     for (j=0;j<dest_width;j++)	//pixel loop
      {
        if (actual_x>639) break;	//x clip - all done
         if (actual_x>=0)	//x clip
         {
          *splat_pix=*(raw_data+(int)source_x_index+temp_y);
          // *splat_pix=colour;
          splat_pix++;	 //inc vram pixel
         }
         
         source_x_index+=source_x_index_inc;	//inc in data x

         actual_x++;	//inc clipping variable
         
      }	//end of pixel loop

   start_pix+=screenwidth;	//vram to start of next line
   }	//end of y clipping
   
   source_x_index=0;	//data to start of line
   source_y_index+=source_y_index_inc;	//data inc
   actual_x=start_x;	//vram x start
   actual_y++;	//inc vram line
 } //end of line loop

}	//end of function


//invert_y_pict inverts an 8 bit picture in y for ogl mainly
//not finished - SB 17/10/99

void invert_y_pict(LSRAW *the_picture)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data_top, raw_data_bottom;
char source_from_bottom, source_from_top;
unsigned int x,y;
Ptr start_top,start_bottom;


picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data

raw_data_top=(char *)&the_picture->raw_data;
raw_data_bottom=(char *)&the_picture->raw_data;
raw_data_bottom+=picture_w*(picture_h-1);

for (y=0;y<picture_h/2;y++)
  {
   start_top=raw_data_top; start_bottom=raw_data_bottom;
   for (x=0;x<picture_w;x++)
     {
        source_from_top=*raw_data_top;
        
        source_from_bottom=*raw_data_bottom;
        
        
        *raw_data_bottom=source_from_top;
        raw_data_bottom++;
        *raw_data_top=source_from_bottom;
        raw_data_top++;
     
     }
     raw_data_bottom=start_bottom;
     raw_data_top=start_top;
     raw_data_bottom-=picture_w;
     raw_data_top+=picture_w;
  }
}

//decrunch_pict splats an RCZ to memory as a contiguous RAW file. The RCZ file MUST
//have the width and hieght as the first two entries (as output from cruncher)
//See ZEX_Cruncher for details. 
//SB 210199
//Used in print_pict_scaled which needs a decompressed pict to work from
void decrunch_pict(LSRAW *the_picture, LSRAW *where)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix;
char rle_length, rle_colour=0;		// rle_colour is always reloaded but to avoid warnings load with 0 initially

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
(*where).pwidth=picture_w;
(*where).pheight=picture_h;

raw_data=(char *)&the_picture->raw_data;

splat_pix=(char *)&where->raw_data;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
    {
      rle_length=(*raw_data);
      raw_data++;
      rle_colour=(*raw_data);
      raw_data++;
    }
    *splat_pix=rle_colour;
    splat_pix++;
    rle_length--;
   }
 } //end of line loop
}


