//display list processor
//the dlp is a 2d drawing engine that draws after the 3d engine has done its drawing
//SB 180199
//the display listprocessor can cope with up to 30 commands per frame

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <mactypes.h>
#include "dlp.h"
#include "txtsplat.h"
#include "LS_Standard.h"
#include "zex_misc.h"
#include "dlp_explosions.h"
#include "fcircle.h"
#include "pictures.h"
#include "OGL.h"
#include "olg_text.h"
#include "fpoly4.h"
#include "genpoly_int.h"
#include "fdistort.h"
#include "fgouraud.h"

#define MAX_DLP_SIZE 30


//sub funcs of dlp
void draw_line(int start_x, int start_y, int end_x, int end_y, int colour);
void draw_weeny_laser(int start_x, int start_y, int end_x, int end_y);
void draw_nsweeny_laser(int x1, int y1, int x3, int y3);
void draw_laser(int x1, int y1, int x3, int y3);
void draw_tractor(int x1, int y1, int x3, int y3, int tractor_type);

void draw_line_no_clip(int x1, int y1, int x3, int y3, int colour);

void dlp_draw_box(int top, int left, int bottom, int right, int colour);
void dlp_floating_dialog(int x, int y, const char * the_string);
void dlp_sleep_dialog(int x, int y, const char * the_string);

int dlp_plot(int x, int y, int colour); //return 1 if clip else 0
void do_dialog_text(int x, int y, const char * the_string);




typedef struct {
int the_command;
int param1,param2,param3,param4,param5;
const char* the_ptr1;
} dlp_command;

extern int Mode320;
extern int show_control_panel;

//the list of commands
dlp_command display_list[MAX_DLP_SIZE];

int dlp_add_index;

typedef struct {
char the_text[256];
int the_timer;
int the_colour;
int the_x;
} dlp_text_buffer_type;

#define NUMBER_OF_TEXT_LINES 7
dlp_text_buffer_type dlp_text_buffer[NUMBER_OF_TEXT_LINES];	//we cope with 6 lines
int dlp_text_buffer_insert_point;	//which of the 6 is the current start (so we dont have to copy stuff about)
 

char the_old_text[256];	//for compare so we don't get same message twice
int text_display_timer1,text_display_timer2,text_display_timer3;
char the_text[256];
char the_text2[256];
char the_text3[256];
Rect dlp_clip_rect;

void init_dlp()
{
int i;

the_old_text[0]=0;	//clear string=last string check string
text_display_timer1=text_display_timer2=text_display_timer3;
the_text[0]=0;
the_text2[0]=0;
the_text3[0]=0;
//init all the text buffers
for (i=0;i<NUMBER_OF_TEXT_LINES;i++) 
   {
   dlp_text_buffer[i].the_text[0]=0;
   dlp_text_buffer[i].the_timer=0;
   }
dlp_text_buffer_insert_point=0;

}

//add a command to the display list
void add_to_dlp(int command,int param1,int param2, int param3, int param4, int param5, const char* param6)
{
extern dlp_command display_list[30];
extern int dlp_add_index;

if (dlp_add_index<MAX_DLP_SIZE)
 {
  display_list[dlp_add_index].the_command=command;
  display_list[dlp_add_index].param1=param1;
  display_list[dlp_add_index].param2=param2;
  display_list[dlp_add_index].param3=param3;
  display_list[dlp_add_index].param4=param4;
  display_list[dlp_add_index].param5=param5;
  display_list[dlp_add_index].the_ptr1=param6;	//Note, we DO NOT copy the data (text)!
  
  dlp_add_index++;
 }


}

//clear the display_list - called at end of run_dlp
void clear_dlp()
{
extern int dlp_add_index;
dlp_add_index=0;	//clear list index
}

static Rect normal_CP ={0,0,CONTROL_PANEL_TOP-1,639};
static Rect normal_no_CP ={0,0,479,639};
static Rect double_CP ={0,0,CONTROL_PANEL_TOP/2-1,319};
static Rect double_no_CP ={0,0,239,319};

//process the display list
void run_dlp()
{
extern int dlp_add_index,showing_stats,freeze;
int dlp_index=0;
int command,p1,p2,p3,p4,p5;
const char* the_ptr;

//set up the clipping rect
if(show_control_panel==0)
 {
  if (Mode320==1)
    set_dlp_clip_bounds(double_CP);
  else
    set_dlp_clip_bounds(normal_CP);
 }
 else
 {
  if (Mode320==1)
    set_dlp_clip_bounds(double_no_CP);
  else
    set_dlp_clip_bounds(normal_no_CP);

 }
 
//if (showing_stats==1 || freeze==1) return;

//if (showing_stats==1) return;

//draw_line(1000,1000,-1480,-1630,32);


while (dlp_index<dlp_add_index)
  {
  command=display_list[dlp_index].the_command;
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  p5=display_list[dlp_index].param5;
  the_ptr=display_list[dlp_index].the_ptr1;  
  dlp_index++;
  


  if (command==DRAW_LINE_NO_CLIP)	//draw a line from p1,p2 to p3,p4
  {
  draw_line_no_clip(p1,p2,p3,p4,p5);
  }

//  else
//  if (command==DRAW_TEXT)
//  {
//  SplatText_Transparent (the_ptr,p1,p2,p3);
//  }
  else
  if (command==DRAW_LASER1)	//draw a line from p1,p2 to p3,p4
  {
  draw_weeny_laser(p1,p2,p3,p4);
  }
  else
  if (command==DRAW_LASER2)	//draw a line from p1,p2 to p3,p4
  {
  draw_nsweeny_laser(p1,p2,p3,p4);
  }
  else
  if (command==DRAW_LASER3)	//draw a line from p1,p2 to p3,p4
  {
  draw_laser(p1,p2,p3,p4);
  }


  else
  if (command==DRAW_LINE)	//draw a line from p1,p2 to p3,p4
  {
  draw_line(p1,p2,p3,p4,p5);
  }

  else
  if (command==DRAW_TRACTOR)	//draw a line from p1,p2 to p3,p4
  {
  draw_tractor(p1,p2,p3,p4,p5);
  }

  else
  if (command==DRAW_FILLED_BOX)	//draw a line from p1,p2 to p3,p4
  {
  dlp_draw_box(p1,p2,p3,p4,p5);
  }


 }	//end of while

//finally clear out list
//clear_dlp();	
}


//process the display list text items and clear
//run AFTER render_frame
void run_dlp_text()
{
extern int dlp_add_index;
extern int OGL_active;

int dlp_index=0;
int command,p1,p2,p3,p4,p5;
const char* the_ptr;

//draw_line(1000,1000,-1480,-1630,32);


while (dlp_index<dlp_add_index)
  {
  command=display_list[dlp_index].the_command;
  
  if (command==DRAW_TEXT)
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  the_ptr=display_list[dlp_index].the_ptr1;  
  #if OGL==1
    if (OGL_active==1)
     {

     OGL_set_pen_colour_from_index(p3);
     OGL_moveto(p1,p2);
     OGL_draw_monaco_bold(the_ptr);	//x,y,colour
//     OGL_draw_helvetica("Test",p1,p2,p3);	  //x,y,colour

     }
    else
    {
     if (Mode320==1)
     SplatText_Small_Transparent (the_ptr,p1,p2,p3);
     else
     SplatText_Transparent (the_ptr,p1,p2,p3);
    }
  #else
  if (Mode320==1)
  SplatText_Small_Transparent (the_ptr,p1,p2,p3);
  else
  SplatText_Transparent (the_ptr,p1,p2,p3);
  #endif
  }
  
  else
  if (command==DRAW_OUTER_FLARE)
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  create_outer_flare(p1,p2,p3,p4);
  }
  else
  if (command==DRAW_RADAR_LINE)	//draw a line from p1,p2 to p3,p4
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  p5=display_list[dlp_index].param5;

  draw_line_no_clip(p1,p2,p3,p4,p5);
  }
 
  else
  if (command==DRAW_DLP_EXPLOSION)
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  p5=display_list[dlp_index].param5;
  draw_dlp_explosion(p1,p2,p3,p4,p5);
  }
  else
  if (command==FLOATING_DIALOG)	//draw a line from p1,p2 to p3,p4
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  the_ptr=display_list[dlp_index].the_ptr1;  
    dlp_floating_dialog(p1,p2,the_ptr);
  }

  else
  if (command==SLEEP_DIALOG)	//draw a line from p1,p2 to p3,p4
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  the_ptr=display_list[dlp_index].the_ptr1;  
    dlp_sleep_dialog(p1,p2,the_ptr);
  }
  

  dlp_index++;

 }	//end of while

//finally clear out list
clear_dlp();	
}



//the purpose of this code is to draw what looks like the top half of a tube.
//we do this by defining 10 items - each contains a colour
//and a length addition  (in 100ths) which is added onto each of the lines we draw
typedef struct
{
int colour;
int length;
} earth_tractor_cl;

//do NOT put zeros in the length field for cross platform purposes
earth_tractor_cl earth_tractor_def [10] =
     {
		{ 240,7 },
		{ 239,6 },
		{ 238,4 },
		{ 237,2 },
		{ 236,1 },
		{ 235,1 },
		{ 236,2 },
		{ 237,4 },
		{ 238,6 },
		{ 239,7 },
     };

earth_tractor_cl alien_tractor_def [10] =
     {
		{ 230,7 },
		{ 229,6 },
		{ 228,4 },
		{ 227,2 },
		{ 226,1 },
		{ 225,1 },
		{ 226,2 },
		{ 227,4 },
		{ 228,6 },
		{ 229,7 },
     };


static int external_count=0;

void draw_tractor(int x1, int y1, int x3, int y3, int tractor_type)
{
int x_temp,y_temp,i,colour;

  external_count++;
  if (external_count>21) external_count=0;

if (tractor_type==0)
{
 for(i=0;i<10;i++)
 {
 x_temp=x3-x1;	//change in x
 x_temp*=100;
 x_temp/=earth_tractor_def[i].length;
 x_temp/=1000;
 x_temp=x_temp>>3;
 x_temp+=x3+(i);
 
 y_temp=y3-y1;	//change in y
 y_temp*=100;
 y_temp/=earth_tractor_def[i].length;
 y_temp/=1000;
 y_temp=y_temp>>4;
 y_temp+=y3+(i);
 
// if (RangedRdm(1,100)>96) colour=RangedRdm(215,222);	  //little random red fleck
 if (i==external_count) 
 {
  colour=RangedRdm(215,222);
 }
	else
 colour=earth_tractor_def[i].colour;
 
 draw_line(x1,y1,x_temp,y_temp,colour);
 }
}
else	//alien tractor beam
{
 for(i=0;i<10;i++)
 {
 x_temp=x3-x1;	//change in x
 x_temp*=100;
 x_temp/=alien_tractor_def[i].length;
 x_temp/=1000;
 x_temp=x_temp>>3;
 x_temp+=x3+(i);
 
 y_temp=y3-y1;	//change in y
 y_temp*=100;
 y_temp/=alien_tractor_def[i].length;
 y_temp/=1000;
 y_temp=y_temp>>4;
 y_temp+=y3+(i);
 
// if (RangedRdm(1,100)>96) colour=RangedRdm(215,222);	  //little random red fleck
 if (i==external_count) 
 {
  colour=RangedRdm(235,242);
 }
	else
 colour=alien_tractor_def[i].colour;
 
 draw_line(x1,y1,x_temp,y_temp,colour);
 }
}

}



void draw_weeny_laser(int x1, int y1, int x3, int y3)
{
///debug
//if (x1<-2000 || x1>2000) Debugger();
//if (x3<-2000 || x3>2000) Debugger();
//if (y1<-2000 || y1>2000) Debugger();
//if (y3<-2000 || y3>2000) Debugger();
x1--; y1--;
draw_line(x1,y1,x3,y3,228);	//green
x1++; y1++;
draw_line(x1,y1,x3,y3,245);	//white
x1++; y1++;
draw_line(x1,y1,x3,y3,228);

}


void draw_nsweeny_laser(int x1, int y1, int x3, int y3)
{
///debug
//if (x1<-2000 || x1>2000) Debugger();
//if (x3<-2000 || x3>2000) Debugger();
//if (y1<-2000 || y1>2000) Debugger();
//if (y3<-2000 || y3>2000) Debugger();
x1--; y1--;
draw_line(x1,y1,x3,y3,235);	//blue
x1++; y1++;
draw_line(x1,y1,x3,y3,245);	//white
x1++; y1++;
draw_line(x1,y1,x3,y3,235);

}

void draw_laser(int x1, int y1, int x3, int y3)
{
///debug
//if (x1<-2000 || x1>2000) Debugger();
//if (x3<-2000 || x3>2000) Debugger();
//if (y1<-2000 || y1>2000) Debugger();
//if (y3<-2000 || y3>2000) Debugger();
x1--; y1--;
draw_line(x1,y1,x3,y3,215);	//red
x1++; y1++;
draw_line(x1,y1,x3,y3,245);	//white
x1++; y1++;
draw_line(x1,y1,x3,y3,215);

}


//set up dlp clipping bounds - public function
//should be called at init and whenever the bounds change, such as when the control panel is invisible
void set_dlp_clip_bounds(Rect the_bounds)
{
dlp_clip_rect=the_bounds;
}

//sub funcs of dlp
//draw a filled box - does no clipping so can crash if called with bad params
void dlp_draw_box(int top, int left, int bottom, int right, int colour)
{
int line_loop_ctr;

for (line_loop_ctr=top;line_loop_ctr<bottom;line_loop_ctr++)
 {
 draw_line_no_clip(left, line_loop_ctr, right, line_loop_ctr, colour);
 }
 
}

//draw a filled rounded box - does no clipping so can crash if called with bad params

void dlp_draw_rounded_box(int top, int left, int bottom, int right, int colour)
{
int line_loop_ctr;

draw_line_no_clip(left+1, top, right-1, top, colour);

for (line_loop_ctr=top+1;line_loop_ctr<bottom;line_loop_ctr++)
 {
 draw_line_no_clip(left, line_loop_ctr, right, line_loop_ctr, colour);
 }

draw_line_no_clip(left+1, bottom, right-1, bottom, colour);
 
}


void draw_line(int x1, int y1, int x3, int y3, int colour)
{
int dx,dy,x2,y2,xstep,ystep,e,clip;
int right,bottom;
//
//if(show_control_panel==0)	  //visible
//{
// if (Mode320==1) 
// { 
//  right=319;
//  bottom=CONTROL_PANEL_TOP>>1;
// } 
// else 
// {
//  right=639;
//  bottom=CONTROL_PANEL_TOP;
// }
// }
// else	//cp hidden
// {
// if (Mode320==1) 
// { 
//  right=319;
//  bottom=240;
// } 
// else 
// {
//  right=639;
//  bottom=480;
// }
//}
//

if (single_line_clip(&x1,&y1,&x3,&y3)==0) return;
#if OGL==1
OGL_line( x1, y1, x3, y3, colour);
#else

////check if all points in view and call non-clip version if so
//if ((x1>dlp_clip_rect.left && x1<dlp_clip_rect.right)    &&
//    (x3>dlp_clip_rect.left && x3<dlp_clip_rect.right)    &&
//    (y1>dlp_clip_rect.top && y1<dlp_clip_rect.bottom) &&
//    (y3>dlp_clip_rect.top && y3<dlp_clip_rect.bottom))
//    {
    draw_line_no_clip(x1,y1,x3,y3,colour);
//    return;
//    }
//    else
//{
//Standard Bresenhnam (If I can remember it - bin a while).
//this is a pretty slow drawline as clipping is carried out as we draw rather
//than modifying the input points with a CS clip
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
//  clip=dlp_plot(x2,y2,colour);
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
// }	  //end of while
////}
#endif
}


void draw_line_no_clip(int x1, int y1, int x3, int y3, int colour)
{
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
Ptr pix;
int dx,dy,x2,y2,xstep,ystep,e;

#if OGL==1
OGL_line( x1, y1, x3, y3, colour);
#else

//Standard Breshnam
//this is a pretty slow drawline as clipping is carried out as we draw rather
//than modifying the input points
dx=x3-x1; dy=y3-y1;
x2=x1; y2=y1;

if (dx<0)
{
 dx=-dx;
 xstep=-1;
}
else
xstep=1;

if (dy<0)
{
 dy=-dy;
 ystep=-1;
}
else
ystep=1;

if (dy==0) e=-1;
   else e=0;

while (x2!=x3 || y2!=y3)
 {
  pix=(char*)screenaddr+(y2*screenwidth)+x2;	//screenaddr
  *pix=colour;	//set the pixel
  
  if (e>=0)
  {
   y2=y2+ystep;
   e=e-dx;
  }
  else
  {
   x2=x2+xstep;
   e=e+dy;
  }
 }	//end of while
#endif
}


int dlp_plot(int x, int y, int colour)
{
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
Ptr pix;

//if (Mode320==1)
//{
    if (x<dlp_clip_rect.left) return 1;
    if (x>dlp_clip_rect.right) return 1;
    if (y<dlp_clip_rect.top) return 1;
    if (y>dlp_clip_rect.bottom) return 1;
//}
//
//else
//{
//    if (x<0) return 1;
//    if (x>639) return 1;
//    if (y<0) return 1;
//    if (y>CONTROL_PANEL_TOP-1) return 1;
//}
    pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
    *pix=colour;
return 0;
}


//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
int text_x,text_x2,text_x3;
int colour1,colour2,colour3;


void add_to_text_display(const_Ptr text_in,int colour)
{
char the_char=13;
char temp_char,temp_char1;
int count=0;
int match=0;
int i,screen_center;	//good old i
extern int OGL_active;

if (text_in[0]==0)
{
	int a=5;
}

if (Mode320==1) screen_center=160;
 else screen_center=320;

//compare to old text and get out if =
//this is a different compare to what I normally write...
#if 0
for (i=0;i<255;i++)
{
  temp_char1=the_old_text[i];
  temp_char=text_in[i];
  if (temp_char1==0) break;	//eos
  if (temp_char!=temp_char1) goto strings_no_match;
}
  if(temp_char1==0 && temp_char==0) return;
#endif
strings_no_match:
//copy to old text
//cant use a while cause of length protection...
for (i=0;i<255;i++)
{
  the_old_text[i]=temp_char=text_in[i];
  if(temp_char==0) break;
}


//BIG CHANGE
//now we first increment the insert index
//we draw backwards up the chain to get a nice scrolling list. CHEAT! <G>
//insert the text

  if (Mode320==1)	//low res font doesn't have lower case letters!
  {
  while(the_char!=0 && count<255)
  {
    the_char=text_in[count];
    if (the_char>=0x61) the_char-=0x20;
    dlp_text_buffer[dlp_text_buffer_insert_point].the_text[count]=the_char;
    count++;
  }
  }
  else
  {
  while(the_char!=0 && count<255)
  {
    if (OGL_active==1)
    {
    the_char=text_in[count];	
//    if (the_char!=0) the_char++; //our font is off by 1
    }
    else
    the_char=text_in[count];
    
    dlp_text_buffer[dlp_text_buffer_insert_point].the_text[count]=the_char;
    count++;
  }
  }
  
//calc text x - center the string
//the font is 8 pixels wide
#if MESSAGES_CENTER
  text_x=(count>>1)*8;
  text_x=screen_center-text_x;
  if (text_x<0) text_x=0;
#else
  text_x=1;
#endif
dlp_text_buffer[dlp_text_buffer_insert_point].the_colour=colour;
dlp_text_buffer[dlp_text_buffer_insert_point].the_x=text_x;
dlp_text_buffer[dlp_text_buffer_insert_point].the_timer=TEXT_DISPLAY_TIME;

dlp_text_buffer_insert_point++;	//drawing takes place
if (dlp_text_buffer_insert_point>NUMBER_OF_TEXT_LINES-1) dlp_text_buffer_insert_point=0; 


////try for display 1 first
//if (text_display_timer1==0)
//{
//  text_display_timer1=TEXT_DISPLAY_TIME;
//  //copy string
//  while(the_char!=0 && count<255)
//  {
//    the_char=text_in[count];
//    the_text[count]=the_char;
//    count++;
//  }
////calc text x - center the string
////the font is 8 pixels wide
//#if MESSAGES_CENTER
//  text_x=(count>>1)*8;
//  text_x=screen_center-text_x;
//  if (text_x<0) text_x=0;
//#else
//  text_x=1;
//#endif
//  colour1=colour;
//}
//else
////try for display 2...
//if (text_display_timer2==0)
//{
//  text_display_timer2=TEXT_DISPLAY_TIME;
//  //copy string
//  while(the_char!=0 && count<255)
//  {
//    the_char=text_in[count];
//    the_text2[count]=the_char;
//    count++;
//  }
////calc text x - center the string
////the font is 8 pixels wide
//#if MESSAGES_CENTER
//  text_x2=(count>>1)*8;
//  text_x2=screen_center-text_x2;
//  if (text_x2<0) text_x2=0;
//#else
//  text_x=1;
//#endif
//
//  colour2=colour;
//}
//else	  //go for lowest
////try for display 3...
//if (text_display_timer3==0)
//{
//  text_display_timer3=TEXT_DISPLAY_TIME;
//  //copy string
//  while(the_char!=0 && count<255)
//  {
//    the_char=text_in[count];
//    the_text3[count]=the_char;
//    count++;
//  }
////calc text x - center the string
////the font is 8 pixels wide
//#if MESSAGES_CENTER
//  text_x3=(count>>1)*8;
//  text_x3=screen_center-text_x3;
//  if (text_x3<0) text_x3=0;
//#else
//  text_x=1;
//#endif
//
//  colour3=colour;
//}
//else	  //go for lowest
//
//{
//  if (text_display_timer1<text_display_timer2)
//  {
//   text_display_timer1=TEXT_DISPLAY_TIME;
//  //copy string
//   while(the_char!=0 && count<255)
//   {
//    the_char=text_in[count];
//    the_text[count]=the_char;
//    count++;
//   }
////calc text x - center the string
////the font is 8 pixels wide
//#if MESSAGES_CENTER
//   text_x=(count>>1)*8;
//   text_x=screen_center-text_x;
//   if (text_x<0) text_x=0;
//#else
//   text_x=1;
//#endif
//   colour1=colour;
//  }
//  else
//  {
//   text_display_timer2=TEXT_DISPLAY_TIME;
//   //copy string
//   while(the_char!=0 && count<255)
//   {
//    the_char=text_in[count];
//    the_text2[count]=the_char;
//    count++;
//   }
////calc text x - center the string
////the font is 8 pixels wide
//#if MESSAGES_CENTER
//   text_x2=(count>>1)*8;
//   text_x2=screen_center-text_x2;
//   if (text_x2<0) text_x2=0;
//#else
//  text_x=1;
//#endif
//  colour2=colour;
//  }
//}

}

void reset_dlp_text_display()
{
int i;
for (i=0;i<NUMBER_OF_TEXT_LINES;i++) dlp_text_buffer[i].the_timer=0;

//text_display_timer1=0;
//text_display_timer2=0;
//text_display_timer3=0;

}


void display_text_display()
{

int colour;	//max bright
int i;
int index;
int y_pos;
int text_display_timer;
//this is a clever bit
//we want a scrolling list of text in chronological order, fading out as it expires
//the text has been inserted in a circular list
//to get the list printed chornologically (oldest at top), we print the list backwards
//taadaah.
//so if dlp_text_buffer_insert_point==0, we print 5,4,3,2,1,0
//if it is 3, we print 2,1,0,5,4,3
//etc

//if(show_control_panel==0)
//{
//if (Mode320==0)
//y_pos=CONTROL_PANEL_TOP-12;
//else
//y_pos=CONTROL_PANEL_TOP/2-12;
//}
//else	  //no control panel
//{
//if (Mode320==0)
//y_pos=470-12;
//else
//y_pos=240-12;
//}

//OGL_draw_helvetica("Test",0,300,10);


y_pos=dlp_clip_rect.bottom-19;

index=dlp_text_buffer_insert_point;

for (i=0;i<NUMBER_OF_TEXT_LINES;i++)
{
//dec index
 index--;
 if (index<0) 
 {
	 index = (NUMBER_OF_TEXT_LINES-1);
 }
 //print the line if timer > 0
 text_display_timer=dlp_text_buffer[index].the_timer;
 if (text_display_timer!=0)
     {
     //calc correct colour    
     if (dlp_text_buffer[index].the_colour==0) colour=225;	//green
     else
     if (dlp_text_buffer[index].the_colour==1) colour=215;
     else colour=245;	    //red
     //calc colour fade according to time
     if (text_display_timer<128)
     colour+=((9-text_display_timer/16));
     //finally issue the command to draw the text
     add_to_dlp(DRAW_TEXT,dlp_text_buffer[index].the_x,
                y_pos,colour,0,0,dlp_text_buffer[index].the_text);
     
     if (Mode320==0)
     y_pos-=15;
     else
     y_pos-=10;
     
     dlp_text_buffer[index].the_timer--;	//ready for next frame
 
     }
}

return;

//if (text_display_timer1>0)
// {
// if (colour1==0) colour=225;	  //green
// else
// if (colour1==1) colour=215;
// else colour=245;	  //red
// 
// text_display_timer1--;
// if (text_display_timer1<16)
// colour+=((9-text_display_timer1/2));
// if (Mode320==1)
// add_to_dlp(DRAW_TEXT,text_x,CONTROL_PANEL_TOP/2-30,colour,0,0,the_text);
// else
// add_to_dlp(DRAW_TEXT,text_x,CONTROL_PANEL_TOP-50,colour,0,0,the_text);
// 
// }
//
//if (text_display_timer2>0)
// {
// text_display_timer2--;
// if (colour2==0) colour=225;	  //green
// else if (colour2==1) colour=215;
// else colour=245;	  //red
// if (text_display_timer2<16)
// colour+=((9-text_display_timer2/2));
//// colour+=((130-text_display_timer2)/14);
// if (Mode320==1)
// add_to_dlp(DRAW_TEXT,text_x2,CONTROL_PANEL_TOP/2-20,colour,0,0,the_text2);
// else
// add_to_dlp(DRAW_TEXT,text_x2,CONTROL_PANEL_TOP-30,colour,0,0,the_text2);
// 
// }
//
//if (text_display_timer3>0)
// {
// text_display_timer3--;
// if (colour3==0) colour=225;	  //green
// else if (colour3==1) colour=215;	  //red
// else colour=245;	  //blue
// if (text_display_timer3<16)
// colour+=((9-text_display_timer3/2));
//// colour+=((130-text_display_timer2)/14);
// if (Mode320==1)
// add_to_dlp(DRAW_TEXT,text_x3,CONTROL_PANEL_TOP/2-10,colour,0,0,the_text3);
// else
// add_to_dlp(DRAW_TEXT,text_x3,CONTROL_PANEL_TOP-10,colour,0,0,the_text3);
// 
// }
 
}

void dlp_floating_dialog(int x, int y, const char * the_string)
{
extern LSRAW *float_dialog;

	print_crunched_trans_pict(float_dialog,x,y);
	do_dialog_text(x, y, the_string);
}

void dlp_sleep_dialog(int x, int y, const char * the_string)
{
extern LSRAW *sleep_dialog;

	print_crunched_trans_pict(sleep_dialog,x,y);
	do_dialog_text(x, y, the_string);
}

void do_dialog_text(int x, int y, const char * the_string)
{
int print_x,print_y,src_pos,dest_pos;
int terminate=0;
char the_line [300];
char the_char;
//draw the text
//get each line and print it, terminate on zero
print_y=20+y;
src_pos=0;
while(terminate==0)
    {
		print_x=16+x;	//newline
		dest_pos=0;
		the_char=0;
		while(dest_pos<299)
		{
			//get line
			the_char=the_string[src_pos];
			the_line[dest_pos]=the_char;
			src_pos++;
			dest_pos++;
			if ((the_char=='\n') || (the_char=='\r')) break;
			if (the_char==0) { terminate=1; break; }
		
		}
	//we have the line
		the_line[dest_pos-1]=0;
		SplatText_Transparent_Fade (the_line,print_x,print_y,246);
		print_y+=16;
    }
}
