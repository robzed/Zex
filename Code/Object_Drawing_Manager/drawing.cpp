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

//drawing.c
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include	<types.h>
//oldmac//#include    <QuickDraw.h>
#include	"splat_control.h"
#include	"splat_control_int.h"
#include	"drawing.h"
#include	"3d_structures_def.h"
#include    "stu_poly.h"
#include "genpoly_int.h"
#include    "fpoly4.h"
#include "txtsplat.h"
#include "pictures.h"
#include "file_io.h"
#include "Damage.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "dlp.h"

#if OGL
#include <agl.h>
#include "ogl.h"
#endif

Ptr screenaddr;
int screenwidth;
//LS_CGrafPtr the_drawing_buffer;


//Main poly draw routine
void Draw_it(int width, int height)
{
extern Ptr screenaddr;
extern int screenwidth;
//oldmac//extern GDHandle displaydevice;
extern const RGBColor	rgbBlack;
extern LSRAW *bpic_ptr;
extern int have_bpic,flash_screen;
extern int planet_on_screen,training_mission, pilot_asleep;

LS_CGrafPtr the_drawing_buffer;

Rect action_rect;	//the rect we draw into - the bottom of the screen is the control panel


//oldmac//GrafPtr		savePort;
//oldmac//GDHandle	saveGDH;
int a;

#if OGL
extern LSRAW* raw_rgba_data_CP;	//ptr to rgba control panel data

    if (flash_screen==0 || planet_on_screen==1)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//black

//    erase_zex_rect(width,height);	  //in pictures
    else
    if (flash_screen>1)
    {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//white
    flash_screen--;
    }
    else
    if (flash_screen==1)
    {
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);	//red
    flash_screen--;
    }
	glClear(GL_COLOR_BUFFER_BIT);


if (training_mission==1 || pilot_asleep==0)
    {
    plot_pts();
//    OGL_draw_pict(0, CONTROL_PANEL_TOP+(480-CONTROL_PANEL_TOP),
//            (LSRAW *) raw_rgba_data_CP, 640, 480-CONTROL_PANEL_TOP);
    }
        display_text_display();	//show any text message

#else	//not OGl
	the_drawing_buffer=Get_back_buffer();

    if (flash_screen==0 || planet_on_screen==1)	//if we flash with a planet on  the screen we see it is really square, so we don't..
    erase_zex_rect(width,height);	//in pictures
    else
    if (flash_screen>1)
    {
    erase_zex_rect_colour(width,height,250);	//white
    flash_screen--;
    }
    else
    if (flash_screen==1)
    {
    erase_zex_rect_colour(width,height,215);	//red
    flash_screen--;
    }

if (training_mission==1 || pilot_asleep==0)
    plot_pts();

    display_text_display();	//show any text message
#endif	//ogl

}



//Main poly draw routine
void Draw_it_no_erase(int width, int height)
{
extern Ptr screenaddr;
extern int screenwidth;
//oldmac//extern GDHandle displaydevice;
extern const RGBColor	rgbBlack;
extern LSRAW *bpic_ptr;
extern int have_bpic;

LS_CGrafPtr the_drawing_buffer;



    the_drawing_buffer=Get_back_buffer();
    plot_pts();

    display_text_display();	//show any text message


}

//Main poly draw routine
void Draw_it_colour_bg(int width, int height, int colour)
{
extern Ptr screenaddr;
extern int screenwidth;
//oldmac//extern GDHandle displaydevice;
extern const RGBColor	rgbBlack;
extern LSRAW *bpic_ptr;
extern int have_bpic;

LS_CGrafPtr the_drawing_buffer;



    the_drawing_buffer=Get_back_buffer();

    erase_zex_rect_colour(width,height,colour);	//white

    plot_pts();

    display_text_display();	//show any text message


}

//еееееееееееееееееееееееееееееееееееееееееееееееееееееее
//heavy modification at 080399 to run dlp at ship z time
//so lasers and thingsdont overdraw other things and look all yucky
#if OGL==1
void plot_pts()
{
extern _2DPoly *view_points_list_ptr;
extern	int number_of_polys_displayed;
extern _ZPolySort zsort_list[2000];	//max of 2000 polys
extern int zsort_index;
extern Ptr screenaddr;
extern int screenwidth;
extern float zex_z;	//where z is so we know when to draw the dlp


_2DPoly *drawing_info;
//_2DPoly *view_points;
//int poly_count;
//int startY;
_ZPoly *the_poly_info;
_2D point1,point2,point3;
_2D *point_list;
_2D points[10];
_2D temppoints[10];
_2D outpoints[10];
_float2D foutpoints[10];

//texture mapping I/F - SB 020199
dcoord *tpoint_list;
dcoord tpoints[10];
dcoord ttemp_points[10];
dcoord toutput_points[30];
fdcoord ftoutput_points[30];	 //float temp out points (OGL)
int tpolys;
textured_connections *pconnections;
int point_u,point_v;

//fpoly I/F
int output_lines;
short fpoly_colour;
RGBColor RGB;
int light_value,temp_red,temp_blue,temp_green;
extern int zsort_start_index;	//top of polys linked list
int poly_index;
int poly_type, translucent;
int done_dlp=0;
extern char * taddr;
extern int OGL_active;

//run through zsort_list, drawing the polys
poly_index=zsort_start_index;

//for (poly_count=0;poly_count<number_of_polys_displayed;poly_count++)
//for (poly_count=0;poly_count<6;poly_count++)
    the_poly_info=zsort_list[poly_index].poly_data;	//get us going

while (the_poly_info!=0)
{
    if (done_dlp==0 && zsort_list[poly_index].z_value<zex_z)
    {
    done_dlp=1;
    run_dlp();	//run the 2d display processor - 2d stuff appears under 3d stuff

    }

    the_poly_info=zsort_list[poly_index].poly_data;	//this poly
    if (the_poly_info!=0)	//prevent those pesky access faults
    {
     poly_index=zsort_list[poly_index].next_slot;

     drawing_info=view_points_list_ptr+(*the_poly_info).PPoints_offset;

      pconnections=(*the_poly_info).poly_connections_ptr;
      translucent=(*the_poly_info).draw_code;
      tpoint_list=tpoints;
      poly_type=(*pconnections).type;


//-----------START HARD CODE-----------//
//polys that are not texture mapped have point_u= -1


if (OGL_active==1)
{
     float fpoly_colour_r,fpoly_colour_g,fpoly_colour_b;

//ogl renderererererer...oooh mummy!!!
     if (poly_type==TEXTURE)	//Oh no, it's texture mapped. Don't Panic boys!!!
     {
      taddr=(*the_poly_info).Ptexture;
      pconnections=(*the_poly_info).poly_connections_ptr;
//     DebugStr	("texture!");
      tpoint_list=tpoints;
//point_list->x=1208; point_list->y=960; point_list->u=0; point_list->v=0; point_list++;
//point_list->x=320; point_list->y=-300; point_list->u=0; point_list->v=15; point_list++;
//point_list->x=-400; point_list->y=0; point_list->u=15; point_list->v=0; point_list++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x1; tpoint_list->y=(*(drawing_info)).y1;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x2; tpoint_list->y=(*(drawing_info)).y2;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;

      tpoint_list->x=(*(drawing_info)).x3; tpoint_list->y=(*(drawing_info)).y3;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;

      tpoint_list++;

distort_window_clipper3(tpoints,ttemp_points,toutput_points,&tpolys);


//clamp light
if ((*the_poly_info).Plight_value>32760) (*the_poly_info).Plight_value=32760;
else
if ((*the_poly_info).Plight_value<-32760) (*the_poly_info).Plight_value=-32760;

     light_value=(*the_poly_info).Plight_value;

//    the_poly_info=(*(view_points))._2DPoly_ptr;
//	   temp_red=(*the_poly_info).Prgb.red+light_value;
//	   temp_green=(*the_poly_info).Prgb.green+light_value;
//	   temp_blue=(*the_poly_info).Prgb.blue+light_value;
	 temp_red=light_value+32767;
//	temp_green=light_value+32767;
//	temp_blue=light_value+32767;

     if (temp_red>65535) temp_red=65535;
     else
     if (temp_red<0) temp_red=0;

//     if (temp_blue>65535) temp_blue=65535;
//	   else
//	   if (temp_blue<0) temp_blue=0;
//
//     if (temp_green>65535) temp_green=65535;
//     else
//     if (temp_green<0) temp_green=0;
//
     fpoly_colour_r=temp_red;
//     fpoly_colour_g=temp_green;
//     fpoly_colour_b=temp_blue;

     fpoly_colour_r=fpoly_colour_r/65536;	//0-1
     fpoly_colour_g=fpoly_colour_r;	  //0-1
     fpoly_colour_b=fpoly_colour_r;	  //0-1

          tpoint_list=toutput_points;

while(tpolys>0)
  {
  //  gouraud3(screenaddr,screenwidth, tpoint_list, temp_red,temp_green,temp_blue,translucent);
	  dcoord * op;	//x,y,u,v
	  fdcoord * fop;	//x,y,u,v

//ogl likes all points in the range -1 to 1, where 0 is center, so we have to convert our integer points to floating points in this range
//y is also reversed
          int i;
//tpoint_list=toutput_points;

          op=tpoint_list;	//from clipper
          fop=ftoutput_points;

          for (i=0;i<3;i++)
          {
          fop->x=op->x;
          fop->x-=320;
          fop->x = fop->x/320;

          fop->y=480-op->y;
          fop->y-=240;
          fop->y = fop->y/240;

          fop->u=op->u>>16;	//texel x
          fop->u=fop->u/128;

          fop->v=op->v>>16;	//texel y
          fop->v=fop->v/128;

//          fop->v=op->v;
          op++;
          fop++;
          }

//  distort3(screenaddr,screenwidth, tpoint_list, taddr, BITMAP_SIZE,(*the_poly_info).Plight_value,translucent);
        OGL_Tpolygon(  3,taddr, (fdcoord *)ftoutput_points,(*the_poly_info).Pid,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, translucent);

  tpoint_list+=3;
//  toutput_points+=3;
  tpolys--;

  }

     }
     else	//this is a non-tm'd poly
     if (poly_type==GOURAD)	//Gourad flag in point v, light values in u
     {
     //Gourad polyy

	 temp_red=(*the_poly_info).Prgb.red;
	 temp_green=(*the_poly_info).Prgb.green;
	 temp_blue=(*the_poly_info).Prgb.blue;

     fpoly_colour_r=temp_red;
     fpoly_colour_g=temp_green;
     fpoly_colour_b=temp_blue;


      pconnections=(*the_poly_info).poly_connections_ptr;
//     DebugStr	("texture!");
      tpoint_list=tpoints;
//point_list->x=1208; point_list->y=960; point_list->u=0; point_list->v=0; point_list++;
//point_list->x=320; point_list->y=-300; point_list->u=0; point_list->v=15; point_list++;
//point_list->x=-400; point_list->y=0; point_list->u=15; point_list->v=0; point_list++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x1; tpoint_list->y=(*(drawing_info)).y1;
      tpoint_list->u=point_u;	//light value

      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x2; tpoint_list->y=(*(drawing_info)).y2;
      tpoint_list->u=point_u;

//      tpoint_list->u=point_u;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;

      tpoint_list->x=(*(drawing_info)).x3; tpoint_list->y=(*(drawing_info)).y3;
      tpoint_list->u=point_u;

//      tpoint_list->u=point_u;

      tpoint_list++;

//point_list=points;
//point_list->x=x1; point_list->y=y1; point_list->u=sa; point_list++;
//point_list->x=x2; point_list->y=y2; point_list->u=sb; point_list++;
//point_list->x=x3; point_list->y=y3; point_list->u=sc; point_list++;

distort_window_clipper3(tpoints,ttemp_points,toutput_points,&tpolys);

          tpoint_list=toutput_points;

while(tpolys>0)
  {
//  gouraud3(screenaddr,screenwidth, tpoint_list, temp_red,temp_green,temp_blue,translucent);
	  dcoord * op;
	  fdcoord * fop;

//ogl likes all points in the range -1 to 1, where 0 is center, so we have to convert our integer points to floating points in this range
//y is also reversed
          int i;


          op=tpoint_list;
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
       OGL_Gpolygon(3,(fdcoord *)ftoutput_points,fpoly_colour_r,fpoly_colour_g,fpoly_colour_b, translucent);

  tpoint_list+=3;
  tpolys--;
  }

     }
     else	//this is a flat coloured poly
     {


     light_value=(*the_poly_info).Plight_value;

//    the_poly_info=(*(view_points))._2DPoly_ptr;
	 temp_red=(*the_poly_info).Prgb.red+light_value;
	 temp_green=(*the_poly_info).Prgb.green+light_value;
	 temp_blue=(*the_poly_info).Prgb.blue+light_value;
     if (temp_red>65535) temp_red=65535;
     else
     if (temp_red<0) temp_red=0;

     if (temp_blue>65535) temp_blue=65535;
	 else
	 if (temp_blue<0) temp_blue=0;

     if (temp_green>65535) temp_green=65535;
     else
     if (temp_green<0) temp_green=0;

     fpoly_colour_r=temp_red;
     fpoly_colour_g=temp_green;
     fpoly_colour_b=temp_blue;

     fpoly_colour_r=fpoly_colour_r/65536;	//0-1
     fpoly_colour_g=fpoly_colour_g/65536;	//0-1
     fpoly_colour_b=fpoly_colour_b/65536;	//0-1

//we need to get these in the range 1..0

//     RGB.red=temp_red;
//     RGB.green=temp_green;
//     RGB.blue=temp_blue;

      point_list=points;
//      orig_x=(*(drawing_info)).x1;
//      accum_x=0;
      point_list->x=(*(drawing_info)).x1;  point_list->y=(*(drawing_info)).y1; point_list++;
      point_list->x=(*(drawing_info)).x2;  point_list->y=(*(drawing_info)).y2; point_list++;
      point_list->x=(*(drawing_info)).x3;  point_list->y=(*(drawing_info)).y3; point_list++;
      point_list=points;
//      if (accum_x==orig_x)
//      point_list->x-=1;	//ogl draws nothing if all points are the same

//      fpoly_colour=Color2Index (&RGB);	//convert rgb to clut index
//      fpoly_colour=convert_to_palette(RGB.red>>8,RGB.green>>8,RGB.blue>>8);
//      fpoly_colour=convert_to_palette(temp_red>>8,temp_green>>8,temp_blue>>8);

	  window_clipper(3, (coord *)points, (coord *)temppoints, (coord *)outpoints, &output_lines);

	  if(output_lines!=0)
	  {
	  _2D * op;
	  _float2D * fop;

//ogl likes all points in the range -1 to 1, where 0 is center, so we have to convert our integer points to floating points in this range
//y is also reversed
          int i;
          int orig_x,orig_y;
          int anded_x, anded_y;
          op=outpoints;
          fop=foutpoints;
//          orig_x=anded_x=op->x;
//          orig_y=anded_y=op->y;

          for (i=0;i<output_lines;i++)
          {
          fop->x=op->x;
//          anded_x&=op->x;

          fop->x-=320;
          fop->x = fop->x/320;

          fop->y=480-op->y;
//          anded_y&=op->y;
          fop->y-=240;
          fop->y = fop->y/240;

          op++;
          fop++;
          }
//          if (orig_x==anded_x && orig_y==anded_y)
//          {
//          fop=foutpoints;
//          fop->x+=0.01;
//          fop->y-=0.01;
//          fop++;
//          fop->x+=0.01;
//          fop++;
//          fop->x-=0.01;
//
//          }
//          if (orig_y==anded_y) foutpoints->y*=0.01;

//       polygon(output_lines,(coord *)outpoints,fpoly_colour,screenaddr,screenwidth);
       OGL_polygon(output_lines,(fcoord *)foutpoints,fpoly_colour_r,fpoly_colour_g,fpoly_colour_b);
           }
     }

}

else
//software render
{




     if (poly_type==TEXTURE)	//Oh no, it's texture mapped. Don't Panic boys!!!
     {
      taddr=(*the_poly_info).Ptexture;
      pconnections=(*the_poly_info).poly_connections_ptr;
//     DebugStr	("texture!");
      tpoint_list=tpoints;
//point_list->x=1208; point_list->y=960; point_list->u=0; point_list->v=0; point_list++;
//point_list->x=320; point_list->y=-300; point_list->u=0; point_list->v=15; point_list++;
//point_list->x=-400; point_list->y=0; point_list->u=15; point_list->v=0; point_list++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x1; tpoint_list->y=(*(drawing_info)).y1;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x2; tpoint_list->y=(*(drawing_info)).y2;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;

      tpoint_list->x=(*(drawing_info)).x3; tpoint_list->y=(*(drawing_info)).y3;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;

      tpoint_list++;

distort_window_clipper_m(tpoints,ttemp_points,toutput_points,&tpolys);

tpoint_list=toutput_points;

//clamp light
if ((*the_poly_info).Plight_value>32760) (*the_poly_info).Plight_value=32760;
else
if ((*the_poly_info).Plight_value<-32760) (*the_poly_info).Plight_value=-32760;

//while(tpolys>0)
//  {
  distortm(screenaddr,screenwidth, tpoint_list, taddr, BITMAP_SIZE,(*the_poly_info).Plight_value,translucent,tpolys);

//  tpoint_list+=3;
//  tpolys--;
//  }

     }
     else	//this is a non-tm'd poly
     if (poly_type==GOURAD)	//Gourad flag in point v, light values in u
     {
     //Gourad polyy

	 temp_red=(*the_poly_info).Prgb.red;
	 temp_green=(*the_poly_info).Prgb.green;
	 temp_blue=(*the_poly_info).Prgb.blue;

      pconnections=(*the_poly_info).poly_connections_ptr;
//     DebugStr	("texture!");
      tpoint_list=tpoints;
//point_list->x=1208; point_list->y=960; point_list->u=0; point_list->v=0; point_list++;
//point_list->x=320; point_list->y=-300; point_list->u=0; point_list->v=15; point_list++;
//point_list->x=-400; point_list->y=0; point_list->u=15; point_list->v=0; point_list++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x1; tpoint_list->y=(*(drawing_info)).y1;
      tpoint_list->u=point_u<<16;
//      tpoint_list->u=point_u;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x2; tpoint_list->y=(*(drawing_info)).y2;
      tpoint_list->u=point_u<<16;
//      tpoint_list->u=point_u;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;

      tpoint_list->x=(*(drawing_info)).x3; tpoint_list->y=(*(drawing_info)).y3;
      tpoint_list->u=point_u<<16;
//      tpoint_list->u=point_u;

      tpoint_list++;

//point_list=points;
//point_list->x=x1; point_list->y=y1; point_list->u=sa; point_list++;
//point_list->x=x2; point_list->y=y2; point_list->u=sb; point_list++;
//point_list->x=x3; point_list->y=y3; point_list->u=sc; point_list++;

distort_window_clipper3(tpoints,ttemp_points,toutput_points,&tpolys);

tpoint_list=toutput_points;
while(tpolys>0)
  {
  gouraud3(screenaddr,screenwidth, tpoint_list, temp_red,temp_green,temp_blue,translucent);

  tpoint_list+=3;
  tpolys--;
  }

     }
     else	//this is a flat coloured poly
     {
     light_value=(*the_poly_info).Plight_value;

//    the_poly_info=(*(view_points))._2DPoly_ptr;
	 temp_red=(*the_poly_info).Prgb.red+light_value;
	 temp_green=(*the_poly_info).Prgb.green+light_value;
	 temp_blue=(*the_poly_info).Prgb.blue+light_value;
     if (temp_red>65535) temp_red=65535;
     else
     if (temp_red<0) temp_red=0;

     if (temp_blue>65535) temp_blue=65535;
	 else
	 if (temp_blue<0) temp_blue=0;

     if (temp_green>65535) temp_green=65535;
     else
     if (temp_green<0) temp_green=0;

//     RGB.red=temp_red;
//     RGB.green=temp_green;
//     RGB.blue=temp_blue;

      point_list=points;

      point_list->x=(*(drawing_info)).x1; point_list->y=(*(drawing_info)).y1; point_list++;
      point_list->x=(*(drawing_info)).x2; point_list->y=(*(drawing_info)).y2; point_list++;
      point_list->x=(*(drawing_info)).x3; point_list->y=(*(drawing_info)).y3; point_list++;
//      fpoly_colour=Color2Index (&RGB);	//convert rgb to clut index
//      fpoly_colour=convert_to_palette(RGB.red>>8,RGB.green>>8,RGB.blue>>8);
      fpoly_colour=convert_to_palette(temp_red>>8,temp_green>>8,temp_blue>>8);

	  window_clipper(3, (coord *)points, (coord *)temppoints, (coord *)outpoints, &output_lines);

	  if(output_lines!=0)
	  {
       polygon(output_lines,(coord *)outpoints,fpoly_colour,screenaddr,screenwidth);
           }
     }



}	//end of software rendererererer



    } //end of if poly!=0

   }	// end of while loop

}


#endif //ogl==1



#if OGL==0
void plot_pts()
{
extern _2DPoly *view_points_list_ptr;
extern	int number_of_polys_displayed;
extern _ZPolySort zsort_list[2000];	//max of 2000 polys
extern int zsort_index;
extern Ptr screenaddr;
extern int screenwidth;
extern float zex_z;	//where z is so we know when to draw the dlp


_2DPoly *drawing_info;
//_2DPoly *view_points;
//int poly_count;
//int startY;
_ZPoly *the_poly_info;
_2D point1,point2,point3;
_2D *point_list;
_2D points[10];
_2D temppoints[10];
_2D outpoints[10];
_float2D foutpoints[10];

//texture mapping I/F - SB 020199
dcoord *tpoint_list;
dcoord tpoints[10];
dcoord ttemp_points[10];
dcoord toutput_points[30];
//fdcoord ftoutput_points[30];	   //float temp out points (OGL)
int tpolys;
textured_connections *pconnections;
int point_u,point_v;

//fpoly I/F
int output_lines;
short fpoly_colour;
RGBColor RGB;
int light_value,temp_red,temp_blue,temp_green;
extern int zsort_start_index;	//top of polys linked list
int poly_index;
int poly_type, translucent;
int done_dlp=0;
extern char * taddr;
extern int OGL_active;

//run through zsort_list, drawing the polys
poly_index=zsort_start_index;

//for (poly_count=0;poly_count<number_of_polys_displayed;poly_count++)
//for (poly_count=0;poly_count<6;poly_count++)
    the_poly_info=zsort_list[poly_index].poly_data;	//get us going

while (the_poly_info!=0)
{
    if (done_dlp==0 && zsort_list[poly_index].z_value<zex_z)
    {
    done_dlp=1;
    run_dlp();	//run the 2d display processor - 2d stuff appears under 3d stuff

    }

    the_poly_info=zsort_list[poly_index].poly_data;	//this poly
    if (the_poly_info!=0)	//prevent those pesky access faults
    {
     poly_index=zsort_list[poly_index].next_slot;

     drawing_info=view_points_list_ptr+(*the_poly_info).PPoints_offset;

      pconnections=(*the_poly_info).poly_connections_ptr;
      translucent=(*the_poly_info).draw_code;
      tpoint_list=tpoints;
      poly_type=(*pconnections).type;






     if (poly_type==TEXTURE)	//Oh no, it's texture mapped. Don't Panic boys!!!
     {
      taddr=(*the_poly_info).Ptexture;
      pconnections=(*the_poly_info).poly_connections_ptr;
//     DebugStr	("texture!");
      tpoint_list=tpoints;
//point_list->x=1208; point_list->y=960; point_list->u=0; point_list->v=0; point_list++;
//point_list->x=320; point_list->y=-300; point_list->u=0; point_list->v=15; point_list++;
//point_list->x=-400; point_list->y=0; point_list->u=15; point_list->v=0; point_list++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x1; tpoint_list->y=(*(drawing_info)).y1;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x2; tpoint_list->y=(*(drawing_info)).y2;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
      point_v=(*pconnections).v;

      tpoint_list->x=(*(drawing_info)).x3; tpoint_list->y=(*(drawing_info)).y3;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;

      tpoint_list++;

if (translucent==2)	//colour 255 transparent?
  {
  distort_window_clipper3(tpoints,ttemp_points,toutput_points,&tpolys);

  tpoint_list=toutput_points;

  //clamp light
  if ((*the_poly_info).Plight_value>32760) (*the_poly_info).Plight_value=32760;
  else
  if ((*the_poly_info).Plight_value<-32760) (*the_poly_info).Plight_value=-32760;

  while(tpolys>0)
    {
//    if (translucent==2)	//colour 255 transparent?
    distort3_mask(screenaddr, screenwidth, tpoint_list, taddr, BITMAP_SIZE, (*the_poly_info).Plight_value, 0);
//    else
//    distort3(screenaddr,screenwidth, tpoint_list, taddr, BITMAP_SIZE,(*the_poly_info).Plight_value,translucent);

    tpoint_list+=3;
    tpolys--;
    }
  }
else
  {
  distort_window_clipper_m(tpoints,ttemp_points,toutput_points,&tpolys);

  tpoint_list=toutput_points;

  //clamp light
  if ((*the_poly_info).Plight_value>32760) (*the_poly_info).Plight_value=32760;
  else
  if ((*the_poly_info).Plight_value<-32760) (*the_poly_info).Plight_value=-32760;

  distortm(screenaddr,screenwidth, tpoint_list, taddr, BITMAP_SIZE,(*the_poly_info).Plight_value,translucent,tpolys);

  }

     }
     else	//this is a non-tm'd poly
     if (poly_type==GOURAD)	//Gourad flag in point v, light values in u
     {
     //Gourad polyy

	 temp_red=(*the_poly_info).Prgb.red;
	 temp_green=(*the_poly_info).Prgb.green;
	 temp_blue=(*the_poly_info).Prgb.blue;

      pconnections=(*the_poly_info).poly_connections_ptr;
//     DebugStr	("texture!");
      tpoint_list=tpoints;
//point_list->x=1208; point_list->y=960; point_list->u=0; point_list->v=0; point_list++;
//point_list->x=320; point_list->y=-300; point_list->u=0; point_list->v=15; point_list++;
//point_list->x=-400; point_list->y=0; point_list->u=15; point_list->v=0; point_list++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x1; tpoint_list->y=(*(drawing_info)).y1;
      tpoint_list->u=point_u<<16;
//      tpoint_list->u=point_u;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;
      tpoint_list->x=(*(drawing_info)).x2; tpoint_list->y=(*(drawing_info)).y2;
      tpoint_list->u=point_u<<16;
//      tpoint_list->u=point_u;
      tpoint_list++;
      pconnections++;

      point_u=(*pconnections).u;
//      point_v=(*pconnections).v;

      tpoint_list->x=(*(drawing_info)).x3; tpoint_list->y=(*(drawing_info)).y3;
      tpoint_list->u=point_u<<16;
//      tpoint_list->u=point_u;

      tpoint_list++;

//point_list=points;
//point_list->x=x1; point_list->y=y1; point_list->u=sa; point_list++;
//point_list->x=x2; point_list->y=y2; point_list->u=sb; point_list++;
//point_list->x=x3; point_list->y=y3; point_list->u=sc; point_list++;

distort_window_clipper3(tpoints,ttemp_points,toutput_points,&tpolys);

tpoint_list=toutput_points;
while(tpolys>0)
  {
  gouraud3(screenaddr,screenwidth, tpoint_list, temp_red,temp_green,temp_blue,translucent);

  tpoint_list+=3;
  tpolys--;
  }

     }
     else	//this is a flat coloured poly
     {
     light_value=(*the_poly_info).Plight_value;

//    the_poly_info=(*(view_points))._2DPoly_ptr;
	 temp_red=(*the_poly_info).Prgb.red+light_value;
	 temp_green=(*the_poly_info).Prgb.green+light_value;
	 temp_blue=(*the_poly_info).Prgb.blue+light_value;
     if (temp_red>65535) temp_red=65535;
     else
     if (temp_red<0) temp_red=0;

     if (temp_blue>65535) temp_blue=65535;
	 else
	 if (temp_blue<0) temp_blue=0;

     if (temp_green>65535) temp_green=65535;
     else
     if (temp_green<0) temp_green=0;

//     RGB.red=temp_red;
//     RGB.green=temp_green;
//     RGB.blue=temp_blue;

      point_list=points;

      point_list->x=(*(drawing_info)).x1; point_list->y=(*(drawing_info)).y1; point_list++;
      point_list->x=(*(drawing_info)).x2; point_list->y=(*(drawing_info)).y2; point_list++;
      point_list->x=(*(drawing_info)).x3; point_list->y=(*(drawing_info)).y3; point_list++;
//      fpoly_colour=Color2Index (&RGB);	//convert rgb to clut index
//      fpoly_colour=convert_to_palette(RGB.red>>8,RGB.green>>8,RGB.blue>>8);
      fpoly_colour=convert_to_palette(temp_red>>8,temp_green>>8,temp_blue>>8);

	  window_clipper(3, (coord *)points, (coord *)temppoints, (coord *)outpoints, &output_lines);

	  if(output_lines!=0)
	  {
       polygon(output_lines,(coord *)outpoints,fpoly_colour,screenaddr,screenwidth);
           }
     }



    } //end of if poly!=0

   }	// end of while loop

}


#endif //ogl==0



/*
for (object_count=0;object_count<dynamic_object_index;object_count++)
{
 view_points=view_points_list_ptr;
 how_many_polys=(*ocb_ptr).object_list[object_count].number_of_polys;
 the_polys=(*ocb_ptr).object_list[object_count].polys_ptr;
//offset as xyz's
 points_offset=(int)(*ocb_ptr).object_list[object_count].points_ptr - (int)points_list_ptr_base;	//the points for this object
//and we want offset as xy
 points_offset=points_offset/sizeof(_3D);
 view_points+=points_offset;	//ptr to the points of this object

 for (poly_count=0;poly_count<how_many_polys;poly_count++)
 {
  this_poly_visible=(*the_polys).p_visible;

  if (this_poly_visible)
   {
    RGBForeColor(&(*the_polys).Prgb);	//set drawing colour

    this_poly_connections=(*the_polys).poly_connections_ptr;
//    this_poly_number_of_connections=(*this_poly_connections++);


//    for (i=0; i<3; i++)	//draw the complete poly
//     {
     c1=(*this_poly_connections++);	//first point
     c2=(*this_poly_connections++);	//second point
     c3=(*this_poly_connections++);	//third point

//now get view_points+con_start
     pX=(*(view_points+c1)).x;	//talk about learning C the hard way...
     pY=(*(view_points+c1)).y;
     startX=pX;
     startY=pY;
     MoveTo(pX,pY);	//to first point
//now get view_points+con_end
     pX=(*(view_points+c2)).x;
     pY=(*(view_points+c2)).y;
     LineTo(pX,pY);	//line to second point
//line 2
//     con_start=(*this_poly_connections++);
//     con_end=(*this_poly_connections++);
//now get view_points+con_start
//     pX=(*(view_points+con_start)).x;	//talk about learning C the hard way...
//     pY=(*(view_points+con_start)).y;
//     MoveTo(pX,pY);
//now get view_points+con_end
     pX=(*(view_points+c3)).x;
     pY=(*(view_points+c3)).y;
     LineTo(pX,pY);
//line 3
//     con_start=(*this_poly_connections++);
//     con_end=(*this_poly_connections++);

     LineTo (startX,startY);

//     }	//end of plane loop
    }	// end of this_poly_visible

   the_polys+=1;	//(*ocb_ptr).object_list[object_count].polys_ptr;
   }	//end of poly loop - do next

  }	//next object
}

 */


//old routine
/*
void plot_pts()
{
extern _2D *view_points_list_ptr;
extern	int	total_points_counter;
extern	DynObjectsFixedSize *ocb_ptr;
extern	_3D *points_list_ptr_base;
extern int dynamic_object_index;	//how many objects to process

_Connection *this_poly_connections;
_2D *view_points, temp_point;
_ZPoly *the_polys;
int	points_offset;	//offset to points for the polys in the object
int object_count;
int i=0,poly_count=0;
int pX,pY,startX,startY;
int	how_many_polys;
int	this_poly_number_of_connections;
_Connection c1,c2,c3;
UInt16 poly_colour;
int this_poly_visible;
//scan 	extern	DynObjectsFixedSize *ocb_ptr;
//for each object..
//for each poly
//get connections and points
//for each connect, get points and draw line

for (object_count=0;object_count<dynamic_object_index;object_count++)
{
 view_points=view_points_list_ptr;
 how_many_polys=(*ocb_ptr).object_list[object_count].number_of_polys;
 the_polys=(*ocb_ptr).object_list[object_count].polys_ptr;
//offset as xyz's
 points_offset=(int)(*ocb_ptr).object_list[object_count].points_ptr - (int)points_list_ptr_base;	//the points for this object
//and we want offset as xy
 points_offset=points_offset/sizeof(_3D);
 view_points+=points_offset;	//ptr to the points of this object

 for (poly_count=0;poly_count<how_many_polys;poly_count++)
 {
  this_poly_visible=(*the_polys).p_visible;

  if (this_poly_visible)
   {
    RGBForeColor(&(*the_polys).Prgb);	//set drawing colour

    this_poly_connections=(*the_polys).poly_connections_ptr;
//    this_poly_number_of_connections=(*this_poly_connections++);


//    for (i=0; i<3; i++)	//draw the complete poly
//     {
     c1=(*this_poly_connections++);	//first point
     c2=(*this_poly_connections++);	//second point
     c3=(*this_poly_connections++);	//third point

//now get view_points+con_start
     pX=(*(view_points+c1)).x;	//talk about learning C the hard way...
     pY=(*(view_points+c1)).y;
     startX=pX;
     startY=pY;
     MoveTo(pX,pY);	//to first point
//now get view_points+con_end
     pX=(*(view_points+c2)).x;
     pY=(*(view_points+c2)).y;
     LineTo(pX,pY);	//line to second point
//line 2
//     con_start=(*this_poly_connections++);
//     con_end=(*this_poly_connections++);
//now get view_points+con_start
//     pX=(*(view_points+con_start)).x;	//talk about learning C the hard way...
//     pY=(*(view_points+con_start)).y;
//     MoveTo(pX,pY);
//now get view_points+con_end
     pX=(*(view_points+c3)).x;
     pY=(*(view_points+c3)).y;
     LineTo(pX,pY);
//line 3
//     con_start=(*this_poly_connections++);
//     con_end=(*this_poly_connections++);

     LineTo (startX,startY);

//     }	//end of plane loop
    }	// end of this_poly_visible

   the_polys+=1;	//(*ocb_ptr).object_list[object_count].polys_ptr;
   }	//end of poly loop - do next

  }	//next object
}
 */

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
Ptr screenaddr1;
Ptr screenaddr2;
Ptr screenaddr3;

void reinit_screen()
{
extern Ptr screenaddr;
extern int screenwidth,show_control_panel;
LSRAW *the_picture;
//FSSpec the_file;
Handle the_file_data_H;

extern FSSpec Zex_FSSpec;

LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;

#if OGL==1
if (OGL_active==1)
{

//set control panel as a static image
    if (show_control_panel==0)
    ogl_load_control_panel();


//OGL_draw_pict(0, CONTROL_PANEL_TOP, (LSRAW *) raw_rgba_data_CP, 640, 480-CONTROL_PANEL_TOP);

// OGL_set_stencil_for_all();	  //allow drawing to all of screen
// //draw the control panel
// OGL_set_stencil_for_above();	//now drawing above CP only allowed (it'll never get wiped is the theory)
}
else
{
    the_file_data_H=GetZexPicture ('ZRAW',400);	//control panel
    LS_HLock(the_file_data_H);

//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"zcp1.RAW",&the_file);
//	the_file_data_H=read_file(the_file);
	the_picture = (LSRAW*)*the_file_data_H;

	the_drawing_buffer=Get_back_buffer();

//	SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

//	print_pict(the_picture,0,CONTROL_PANEL_TOP);
//    Hull_NewLife();
//     show_and_get_next_screen();	  //splat control panel to 1st buffer


	print_pict(the_picture,0,CONTROL_PANEL_TOP);
    Hull_NewLife();
     show_and_get_next_screen();	//splat control panel to 2nd buffer


	print_pict(the_picture,0,CONTROL_PANEL_TOP);
    Hull_NewLife();
//	show_and_get_next_screen();
     Show_it();	//splat control panel to 3rd buffer (or 1st again)
    LS_DisposeHandle(the_file_data_H);	//dispose control panel data
}
#else

//load control panel into lower bit of screens
    the_file_data_H=GetZexPicture ('ZRAW',400);	//control panel
    LS_HLock(the_file_data_H);

//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"zcp1.RAW",&the_file);
//	the_file_data_H=read_file(the_file);
	the_picture = (LSRAW*)*the_file_data_H;

	the_drawing_buffer=Get_back_buffer();

//	SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

//	print_pict(the_picture,0,CONTROL_PANEL_TOP);
//    Hull_NewLife();
//     show_and_get_next_screen();	  //splat control panel to 1st buffer


	print_pict(the_picture,0,CONTROL_PANEL_TOP);
    Hull_NewLife();
     show_and_get_next_screen();	//splat control panel to 2nd buffer


	print_pict(the_picture,0,CONTROL_PANEL_TOP);
    Hull_NewLife();
//	show_and_get_next_screen();
     Show_it();	//splat control panel to 3rd buffer (or 1st again)
    LS_DisposeHandle(the_file_data_H);	//dispose control panel data

#endif	//ogl==1
}
//void Erase_backs(void) {
//extern	DSpContextReference		    mDisplayContext;
//extern Ptr screenaddr;
//extern int screenwidth;
//LSRAW *the_picture;
//FSSpec the_file;
//Handle the_file_data_H;
//
//extern FSSpec Zex_FSSpec;
//
//LS_CGrafPtr the_drawing_buffer;
//GDHandle	  saveGDH;
//
////	saveGDH = GetGDevice();
//	  SetGDevice(mZexGDH);
////load control panel into lower bit of screens
//	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"zcp1.RAW",&the_file);
//	  the_file_data_H=read_file(the_file);
//	  the_picture = (LSRAW*)*the_file_data_H;
//
//DSpContext_SwapBuffers(mDisplayContext, nil, nil);
//
//the_drawing_buffer=Get_back_buffer();
//SetPort((GrafPtr)the_drawing_buffer);
//RGBBackColor (&rgbBlack);
//RGBForeColor (&rgbWhite);
//EraseRect(&the_drawing_buffer->portRect);
//
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
//
//	   print_pict(the_picture,0,500);
//
//DSpContext_SwapBuffers(mDisplayContext, nil, nil);
//
//the_drawing_buffer=Get_back_buffer();
//SetPort((GrafPtr)the_drawing_buffer);
//RGBBackColor (&rgbBlack);
//RGBForeColor (&rgbWhite);
//EraseRect(&the_drawing_buffer->portRect);
//
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
//
//	  print_pict(the_picture,0,500);
//
//DSpContext_SwapBuffers(mDisplayContext, nil, nil);
//
//the_drawing_buffer=Get_back_buffer();
//SetPort((GrafPtr)the_drawing_buffer);
//RGBBackColor (&rgbBlack);
//RGBForeColor (&rgbWhite);
//EraseRect(&the_drawing_buffer->portRect);
//
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
//
//	  print_pict(the_picture,0,500);
//
//
//	   LS_DisposeHandle(the_file_data_H);
//
////	SetGDevice(saveGDH);
//
//}

