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

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <QuickDraw.h>
#include <memory.h>
//oldmac//#include <timer.h>
//oldmac//#include <textutils.h>
//oldmac//#include <files.h>
#include "sound.h"
#include "pictures.h"
#include "splat_control.h"
#include "Error_handler.h"
#include "zex_misc.h"
#include	"3d_structures_def.h"
#include "inventory.h"
#include "instrum_int.h"
#include "input_control.h"
#include	"txtsplat.h"
#include "cargo.h"
#include "buy_int.h"
#include "real_time.h"
#include "game_defs.h"
#include "OGL.h"
#include "olg_text.h"
#include "fpoly4.h"

extern const RGBColor	rgbBlack	= { 0x0000, 0x0000, 0x0000 };
extern const RGBColor	rgbWhite	= { 0xFFFF, 0xFFFF, 0xFFFF };
extern const RGBColor	rgbRed		= { 0xFFFF, 0x0000, 0x0000 };
extern const RGBColor	rgbOrange	= { 0xFFFF, 0x7777, 0x7777 };
extern const RGBColor	rgbBlue		= { 0x0000, 0x0000, 0xFFFF };
extern const RGBColor	rgbDarkBlue		= { 0x0000, 0x0000, 0x3333 };
extern const RGBColor	rgbYellow	= { 0xFFFF, 0xFFFF, 0x0000 };
extern const RGBColor	rgbPurple	= { 0xFFFF, 0x0000, 0xFFFF };
extern const RGBColor	rgbGreen	= { 0x0000, 0xFFFF, 0x0000 };
extern const RGBColor	rgbLightBlue	= { 0x0000, 0xFFFF, 0xFFFF };
extern const RGBColor	rgbGrey	= { 0x7777, 0x7777, 0x7777 };
extern const RGBColor	rgbDarkGrey	= { 0x4444, 0x4444, 0x4444 };

void draw_items();
void draw_hold_items(LSRAW * up_pict, LSRAW * down_pict, LSRAW * armed, bool with_jettision);
int draw_sell_items(int what_class);

Str255 ship_items_strings[35];	//read from str resource 130

void do_inventory()
{
//oldmac//extern GDHandle				mZexGDH;
extern LSRAW *inv_picture;
extern LSRAW *inv_picture_rgba;
//Handle inv_H;
LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;


while (LS_Left_Button()==1)  { SDL_Delay(10); Show_it(); } // give computer back time and allow mouse to move (if on screen)

//    inv_H=GetZexPicture ('RCZ ',4000);	//inventory pict   
//	  LS_HLock(inv_H);
//	  inv_picture = (LSRAW*)*inv_H;

     //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices

     the_drawing_buffer=Get_back_buffer();
 	 //oldmac//SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);
	 RGBBackColor (&rgbDarkBlue);
	 cls();
	 //oldmac//EraseRect(&the_drawing_buffer->portRect);
	 print_crunched_trans_pict(inv_picture,100,100);
	 draw_items();
	 //oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	 //oldmac//SetPort(savePort);
     Show_it();

//LS_DisposeHandle(inv_H);

	 while (LS_Left_Button()==0) { SDL_Delay(10); Show_it(); } // give computer back time and allow mouse to move (if on screen)

}

extern int OGL_active;

extern int global_show_status;
void do_inventory_single_frame(bool docked)	//draws picture and returns - speed is not an issue.
{
extern LSRAW *inv_picture;
extern Handle inv_H;
extern LSRAW *up_picture;
extern Handle up_H;
extern LSRAW *down_picture;
extern Handle down_H;
extern LSRAW *armed_picture;
extern Handle armed_H;

extern LSRAW *inv_cursor_picture;
extern Handle inv_cursor_H;



LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;

     //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
     the_drawing_buffer=Get_back_buffer();
// 	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);
//	RGBBackColor (&rgbDarkBlue);
//picture will be loaded from main loop
#if OGL==1
    if (OGL_active==1)
    {
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);	//dk blue
	glClear(GL_COLOR_BUFFER_BIT);
//	print_crunched32_trans_pict(inv_picture_rgba,100,100);
    }
    else
    erase_zex_rect_colour(640,480,244);	//dk blue
    
#else
    erase_zex_rect_colour(640,480,244);	//dk blue
//        erase_zex_rect_colour(640,480,242);	  //in pictures
//	EraseRect(&the_drawing_buffer->portRect);
	print_crunched_trans_pict(inv_picture,100,100);
#endif
	 if (global_show_status==1)
	 {
	 draw_items();
         ogl_hide_mouse_cursor();
  	 }
  	 else
	 {
         ogl_show_mouse_cursor();
		if (docked)
		{
			draw_hold_items(up_picture, down_picture, armed_picture,false);	    //without jettison buttons
		}
		else
		{
			draw_hold_items(up_picture, down_picture, armed_picture,true);	    //with jettison buttons
	    }
	 }
	 //oldmac//SetGDevice(saveGDH);	 //these are here so I can SEE them!
	 //oldmac//SetPort(savePort);

//LS_DisposeHandle(inv_H);
//LS_DisposeHandle(up_H);
//LS_DisposeHandle (down_H);
//LS_DisposeHandle (cursor_H);
//LS_DisposeHandle (armed_H);
}

int do_sell_kit(int obj_class)	//draws picture and returns - speed is not an issue.
{
extern LSRAW *inv_picture;
//Handle inv_H;
extern LSRAW *up_picture;
//Handle up_H;
extern LSRAW *down_picture;
//Handle down_H;
extern LSRAW *armed_picture;
//Handle armed_H;

extern LSRAW *inv_cursor_picture;
//Handle cursor_H;

LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
int done;
int return_val=-1;
int show_cant_sell_trading_computer=0;
int show_cant_sell_destroyed=0;
extern int mothership_cat;
extern kit_desc ship_items[ITEMS_COUNT];

done=0;
LS_ShowCursor();
while(done==0)
     {
     int which_item;
	 do_fixed_frame_rate(FRAME_RATE_DELAY);
     the_drawing_buffer=Get_back_buffer();
	 RGBBackColor (&rgbDarkBlue);
	 cls();
	 print_crunched_trans_pict(inv_picture,100,100);
	 if (show_cant_sell_trading_computer>0)
	 {
		show_cant_sell_trading_computer-=10;
		if (show_cant_sell_trading_computer>100)
		{
			SplatText_Transparent_Fade ("CANT SELL TRADING COMPUTER ON ALIEN MOTHERSHIP",150,100,246);
		}
		else
		{
			SplatText_Transparent_Fade ("CANT SELL TRADING COMPUTER ON ALIEN MOTHERSHIP",150,show_cant_sell_trading_computer,246);
		}
	 }

	 if (show_cant_sell_destroyed>0)
	 {
		show_cant_sell_destroyed-=10;
		if (show_cant_sell_destroyed>100)
		{
			SplatText_Transparent_Fade ("CANT SELL ITEM - IT HAS BEEN DESTROYED",150,100,246);
		}
		else
		{
			SplatText_Transparent_Fade ("CANT SELL ITEM - IT HAS BEEN DESTROYED",150,show_cant_sell_destroyed,246);
		}
	 }
	 
	 
	 which_item=draw_sell_items(obj_class);
     
         Show_it();
         if (LS_Left_Button()==1)
         {
         if (which_item==-1) 
          { 
			done=1;
            return_val=-1;
          }
         else
         //sell the item
          {
			//check if not destroyed
			if (ship_items[which_item].status!=-1)	//if not destroyed, then we can select it
			{
			  
				//Check if its a trading computer and we're on an alien mothership
				if (which_item==KTRADING_COMPUTER && (mothership_cat!=zex_object))
				{
					play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
					show_cant_sell_trading_computer=1000;	//display cant sell trade computer for 100 frames (step 10)
				}
				else
				{
					done=1;
					return_val=which_item;
				}
			}
			else	//item has been destroyed
			{
				play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
				show_cant_sell_destroyed=1000;	//display cant sell item as it has been destroyed
			}
		  }
         }
         
      }
         
return return_val;
}





spoint string_pos [] = 
{
{ 10,360,125,360 },	//radar
{ 10,390,127,362 },	//lr radar
{ 10,430,137,362 },	//cannon
{ 10,325,160,350 },	//laser
{ 150,430,270,300 },	//missile controller
{ 330,430,290,280 },	//repair systems
{ 450,430,310,260 },	//flight computer
{ 530,430,334,250 },	//engine
{ 10,290,165,330 },		//pylon 1
{ 10,230,220,230 },		//pylon 2
{ 10,190,255,180 },		//pylon 3
{ 10,135,260,160 },		//shields
{ 10,106,280,140 },		//fuel tank
{ 420,160,360,164 },	//rear_laser
{ 10,75,295,140 },		//360
{ 10,40,300,135 },		//shockwave
{ 150,40,310,145 },		//ecm
{ 330,40,320,155 },		//missile factory
{ 450,40,330,165 },		//fastdock
{ 530,40,340,160 },		//flatpacks
{ 530,380,350,240 },	//fuel maximiser
{ 530,335,370,230 },	//retro
{ 530,290,380,220 },	//hyperspace
{ 530,255,385,210 },	//nav computer
{ 530,210,365,200 },	//ejection
{ 530,165,325,180 },	//trading computer
{ 530,120,325,170 },	//hull auto repair
{ 150,390,245,300 },		//boid addition
{ 530,80,345,165 },	//asteroid addition
};



void draw_items()
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;
extern int zex_laser_experience;
extern int score,credits,start_time,accumulated_game_time;
int i;
unsigned char number_string[12];
int alive_time,hours,mins,secs,stop_time;
char number_string1[12];

TextFont(21);
//TextSize(12);
//   ZMoveTo(10,20);
//   RGBForeColor(&rgbWhite);

//SplatText_Transparent_Fade ("EQUIPMENT",280,6,235);

draw_topbottom_lines();
//#define COLOUR 225
//
//SplatText_Transparent_Fade ("SCORE",10,6,COLOUR);
//ZexNumToString(score,number_string);
//number_string[number_string[0]+1]=0;	  //quick p to c hack
//number_string[0]=' ';
//SplatText_Transparent_Fade ((char *)number_string,42,6,COLOUR);
//      
//SplatText_Transparent_Fade ("CREDITS",500,6,COLOUR);
//ZexNumToString(credits,number_string);
//number_string[number_string[0]+1]=0;	  //quick p to c hack
//number_string[0]=' ';
//SplatText_Transparent_Fade ((char *)number_string,560,6,COLOUR);
//
//   RGBForeColor(&rgbRed);
//ZMoveTo(0,20);
//ZLineTo(640,20);
//      
//
//#define TIMEX 100
//#define TIMEY 460
//SplatText_Transparent_Fade ("REAL TIME",10,TIMEY,COLOUR);
//stop_time=LS_TickCount60();
//alive_time=(stop_time-start_time);
//
//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);
//
//
//#define TIMEX 560
//#define TIMEY 460
//SplatText_Transparent_Fade ("TOTAL TIME",480,TIMEY,COLOUR);
//stop_time=LS_TickCount60();
//alive_time=(stop_time-start_time)+accumulated_game_time;
//
//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);
//
//
////   RGBForeColor(&rgbRed);
//ZMoveTo(0,456);
//ZLineTo(640,456);


TextSize(9);
TextFace(normal);

for (i=0;i<KLAST;i++)
   {
   ZMoveTo(string_pos[i].sx,string_pos[i].sy);
   ZRGBForeColor(&rgbWhite);

   if (ship_items[i].status==0) 
     {
      ZRGBForeColor(&rgbDarkGrey);
      ZDrawString_c("Empty slot");
     }
     else
     {
      ZDrawString(ship_items_strings[i]);
//      if (ship_items[i].status==-1) 
//       {
//       RGBForeColor(&rgbRed);
//       ZDrawString(" Lost!");
//       }
      }
      
   ZRGBForeColor(&rgbDarkGrey);

   if (string_pos[i].sx<320)
   ZLineTo(string_pos[i].lex,string_pos[i].ley);
   else
   {
   ZMoveTo(string_pos[i].sx,string_pos[i].sy);
   ZLineTo(string_pos[i].lex,string_pos[i].ley);   
   }
//now show status
//   RGBForeColor(&rgbGrey);

   ZMoveTo(string_pos[i].sx,string_pos[i].sy+10);
   if (ship_items[i].status==0) 
   {
   ZRGBForeColor(&rgbRed);
//   ZDrawString ("Not Fitted");
//   RGBForeColor(&rgbGrey);
   }
   else //fitted
   {
 
   if (ship_items[i].status==-1) 
   {
    ZRGBForeColor(&rgbRed);
    ZDrawString_c ("Destroyed.");
   }
   else
   if (ship_items[i].status==1)
   {
    ZRGBForeColor(&rgbGreen);
    ZDrawString_c ("Serviceable. ");
//    RGBForeColor(&rgbGrey);
   }
  else
   {
//      if (ship_items[i].status==-1) 
//       {
//       RGBForeColor(&rgbRed);
//       ZDrawString(" Lost!");
 
    ZRGBForeColor(&rgbOrange);
    ZDrawString_c ("Damaged. ");
//    RGBForeColor(&rgbGrey);
   }
//now show extra info depending on type
   if (i==KSHIELDS)
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("Type 1. ");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("Type 2. ");
       else
       if(ship_items[i].modifier==3) ZDrawString_c ("Type 3. ");
       else
       if(ship_items[i].modifier==4) ZDrawString_c ("Type 4. ");
       else
       if(ship_items[i].modifier==5) ZDrawString_c ("Type 5. ");

       ZexNumToString(ship_items[KSHIELDS].value1/10,number_string);
       ZDrawString (number_string);
       ZDrawString_c ("%");
       
       
     }
   else

   if (i==KCANNON)
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("65mm rounds.");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("120mm rounds.");
       else
       if(ship_items[i].modifier==3) ZDrawString_c ("200mm rounds.");
     }
   else
   if (i==KEJECTION)
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("Standard.");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("Hardened.");
     }
   else
   if (i==KREPAIR_SYSTEMS)	//bug 123
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("Level1.");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("Level2.");
       else
       if(ship_items[i].modifier==3) ZDrawString_c ("Level3.");
     }
   else
   if (i==KTRADING_COMPUTER)
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("Type S1.");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("Type S2.");
       else
       if(ship_items[i].modifier==3) ZDrawString_c ("Type S3.");
     }

   else

   if (i==KFLATPACK)
     {
       if(ship_items[i].value1==0) ZDrawString_c ("None.");
       else
       if(ship_items[i].value1==1) ZDrawString_c ("1.");
       else
       if(ship_items[i].value1==2) ZDrawString_c ("2.");
       else
       if(ship_items[i].value1==3) ZDrawString_c ("3.");
     }
   else

   if (i==KLASER)
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("40 Megawatts.");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("130 Megawatts.");
       else
       if(ship_items[i].modifier==3) ZDrawString_c ("350 Megawatts.");
        else	//tasers
        if(ship_items[i].modifier==11) ZDrawString_c ("40 Megawatts x 2.");
       else
       if(ship_items[i].modifier==12) ZDrawString_c ("130 Megawatts x 2.");
       else
       if(ship_items[i].modifier==13) ZDrawString_c ("350 Megawatts x 2.");
    
     }
    
   else

   if (i==KNORMAL_FUEL_TANK)
     {
       ZexNumToString(ship_items[KNORMAL_FUEL_TANK].value1,number_string);
       ZDrawString (number_string);
       ZDrawString_c (" tonnes.");
     }

   else
   if (i==KREAR_LASER)
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("60 Megawatts.");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("140 Megawatts.");
       else
       if(ship_items[i].modifier==3) ZDrawString_c ("270 Megawatts.");
     }

  else
   if (i==KFLIGHT_COMPUTER || i==KSHOCKWAVE || i==KNAV_COMPUTER)
     {
       if(ship_items[i].modifier==1) ZDrawString_c (" MK I");
       else
       if(ship_items[i].modifier==2) ZDrawString_c (" MK II");
     }

  else
   if (i==KMISSILE_PYLON1 || i==KMISSILE_PYLON2 || i==KMISSILE_PYLON3)
     {
       if(ship_items[i].modifier==1) ZDrawString_c ("T1 20 Megatonnes.");
       else
       if(ship_items[i].modifier==2) ZDrawString_c ("T2 60 Megatonnes.");
       else
       if(ship_items[i].modifier==3) ZDrawString_c ("T3 150 Megatonnes.");


     }

   }	//end of fitted
   
   }	//end of for loop
//finally show hull, which is a special case
   ZMoveTo(10,165);
   ZRGBForeColor(&rgbWhite);
   ZDrawString_c("Hull");
   ZRGBForeColor(&rgbDarkGrey);
   ZLineTo(260,160);

   ZMoveTo(10,175);
   ZRGBForeColor(&rgbGreen);

   ZexNumToString(hull_strength<<1,number_string);
   ZDrawString (number_string);
   ZDrawString_c ("%");

}




int draw_sell_items(int what_class)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;
extern int zex_laser_experience;
extern int score,credits,start_time,accumulated_game_time;
int i,mouse_item;
unsigned char number_string[12];
int alive_time,hours,mins,secs,stop_time;
char number_string1[12];
Point mouse_xy;


mouse_item=-1;
TextFont(21);
//TextSize(12);
//   ZMoveTo(10,20);
//   RGBForeColor(&rgbWhite);
if (what_class==EQUIPMENT)
SplatText_Transparent_Fade ("SELL EQUIPMENT - CLICK AN ITEM TO SELL",150,6,247);
else
if (what_class==WEAPONS)
SplatText_Transparent_Fade ("SELL WEAPONS - CLICK AN ITEM TO SELL",150,6,247);


#define COLOUR 225

SplatText_Transparent_Fade ("SCORE",10,6,COLOUR);
ZexNumToString(score,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent_Fade ((char *)number_string,42,6,COLOUR);
      
SplatText_Transparent_Fade ("CREDITS",500,6,COLOUR);
ZexNumToString(credits,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent_Fade ((char *)number_string,560,6,COLOUR);

   ZRGBForeColor(&rgbRed);
ZMoveTo(0,20);
ZLineTo(640,20);
      

#define TIMEX 100
#define TIMEY 460
SplatText_Transparent_Fade ("REAL TIME",10,TIMEY,COLOUR);
stop_time=LS_TickCount60();
alive_time=(stop_time-start_time);

hours=alive_time/(60*60*60);
alive_time-=hours*(60*60*60);

mins=alive_time/(60*60);
alive_time-=mins*(60*60);

secs=alive_time/60;
num_to_string99(hours,number_string1);	//hours
SplatText_Transparent_Fade ((char *)number_string1,TIMEX,TIMEY,COLOUR);
//
	SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
//	  
num_to_string99(mins,number_string1);	//mins
SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);

	SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
	
num_to_string99(secs,number_string1);	//secs
SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);

#undef TIMEX
#undef TIMEY

#define TIMEX 560
#define TIMEY 460
SplatText_Transparent_Fade ("TOTAL TIME",480,TIMEY,COLOUR);
stop_time=LS_TickCount60();
alive_time=(stop_time-start_time)+accumulated_game_time;

hours=alive_time/(60*60*60);
alive_time-=hours*(60*60*60);

mins=alive_time/(60*60);
alive_time-=mins*(60*60);

secs=alive_time/60;
num_to_string99(hours,number_string1);	//hours
SplatText_Transparent_Fade ((char *)number_string1,TIMEX,TIMEY,COLOUR);
//
	SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
//	  
num_to_string99(mins,number_string1);	//mins
SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);

	SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
	
num_to_string99(secs,number_string1);	//secs
SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);


//   RGBForeColor(&rgbRed);
ZMoveTo(0,456);
ZLineTo(640,456);


TextSize(9);
TextFace(normal);

for (i=0;i<KLAST;i++)
   {
   int obj_class;
   if (i==KRADAR) i++;
   if (i==KLONG_RANGE_RADAR) i++;
   if (i==KNORMAL_FUEL_TANK) i++;
   if (i==KMISSILES) i++;
   if (i==KENGINE) i++;
   obj_class=ship_items[i].obj_class;
   if (obj_class==REPAIRS) obj_class=EQUIPMENT;	//show repairs stuff under equipment
   if (obj_class==what_class)
   {
	   ZMoveTo(string_pos[i].sx,string_pos[i].sy);
	   ZRGBForeColor(&rgbWhite);

	   if (ship_items[i].status==0) 
		 {
	//      RGBForeColor(&rgbDarkGrey);
	//      ZDrawString("Empty slot");
		 }
		 else
		 {
		 int strlen;
		 
		  //if mouse over item, highlight it, else just draw it
		  ZexGetMouse (&mouse_xy);
		  strlen=StringWidth(ship_items_strings[i]);
		 
		  if (
			  (mouse_xy.v>string_pos[i].sy-10) && 
			  (mouse_xy.v<string_pos[i].sy+10) &&
			  mouse_xy.h>string_pos[i].sx    &&
			  (mouse_xy.h<string_pos[i].sx+strlen)
			  )
		  {
					TextMode(4);
					mouse_item=i;
		  }
		  else
		  {
			  TextMode(1);

		  }
			  
		  ZDrawString(ship_items_strings[i]);

		  ZRGBForeColor(&rgbDarkGrey);

		  if (string_pos[i].sx<320)
		  ZLineTo(string_pos[i].lex,string_pos[i].ley);
		  else
		  {
		  ZMoveTo(string_pos[i].sx,string_pos[i].sy);
		  ZLineTo(string_pos[i].lex,string_pos[i].ley);   
		  }

		  }
		  
	//now show status
	//   RGBForeColor(&rgbGrey);

	   ZMoveTo(string_pos[i].sx,string_pos[i].sy+10);
	   if (ship_items[i].status==0) 
	   {
	   ZRGBForeColor(&rgbRed);
	//   ZDrawString ("Not Fitted");
	//   RGBForeColor(&rgbGrey);
	   }
	   else //fitted
	   {
	 
	   if (ship_items[i].status==-1) 
	   {
		ZRGBForeColor(&rgbRed);
		ZDrawString_c ("Destroyed.");
	   }
	   else
	   if (ship_items[i].status==1)
	   {
		ZRGBForeColor(&rgbGreen);
		ZDrawString_c ("Serviceable. ");
	//    RGBForeColor(&rgbGrey);
	   }
	  else
	   {
	//      if (ship_items[i].status==-1) 
	//       {
	//       RGBForeColor(&rgbRed);
	//       ZDrawString(" Lost!");
	 
		ZRGBForeColor(&rgbOrange);
		ZDrawString_c ("Damaged. ");
	//    RGBForeColor(&rgbGrey);
	   }
	//now show extra info depending on type
	   if (i==KSHIELDS)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("Type 1. ");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("Type 2. ");
		   else
		   if(ship_items[i].modifier==3) ZDrawString_c ("Type 3. ");
		   else
		   if(ship_items[i].modifier==4) ZDrawString_c ("Type 4. ");
		   else
		   if(ship_items[i].modifier==5) ZDrawString_c ("Type 5. ");

		   ZexNumToString(ship_items[KSHIELDS].value1/10,number_string);
		   ZDrawString (number_string);
		   ZDrawString_c ("%");
		   
		   
		 }
	   else

	   if (i==KCANNON)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("65mm rounds.");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("120mm rounds.");
		   else
		   if(ship_items[i].modifier==3) ZDrawString_c ("200mm rounds.");
		 }
	   else
	   if (i==KEJECTION)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("Standard.");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("Hardened.");
		 }

	   else
	   if (i==KREPAIR_SYSTEMS)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("Class 1.");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("Class 2.");
		 }

	   else

	   if (i==KTRADING_COMPUTER)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("Type S1.");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("Type S2.");
		   else
		   if(ship_items[i].modifier==3) ZDrawString_c ("Type S3.");
		 }

	   else

	   if (i==KFLATPACK)
		 {
		   if(ship_items[i].value1==0) ZDrawString_c ("None.");
		   else
		   if(ship_items[i].value1==1) ZDrawString_c ("1.");
		   else
		   if(ship_items[i].value1==2) ZDrawString_c ("2.");
		   else
		   if(ship_items[i].value1==3) ZDrawString_c ("3.");
		 }
	   else

	   if (i==KLASER)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("40 Megawatts.");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("130 Megawatts.");
		   else
		   if(ship_items[i].modifier==3) ZDrawString_c ("350 Megawatts.");
			else	//tasers
			if(ship_items[i].modifier==11) ZDrawString_c ("40 Megawatts x 2.");
		   else
		   if(ship_items[i].modifier==12) ZDrawString_c ("130 Megawatts x 2.");
		   else
		   if(ship_items[i].modifier==13) ZDrawString_c ("350 Megawatts x 2.");
		
		 }
		
	   else

	   if (i==KNORMAL_FUEL_TANK)
		 {
		   ZexNumToString(ship_items[KNORMAL_FUEL_TANK].value1,number_string);
		   ZDrawString (number_string);
		   ZDrawString_c (" tonnes.");
		 }

	   else
	   if (i==KREAR_LASER)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("60 Megawatts.");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("140 Megawatts.");
		   else
		   if(ship_items[i].modifier==3) ZDrawString_c ("270 Megawatts.");
		 }

	  else
	   if (i==KFLIGHT_COMPUTER || i==KSHOCKWAVE || i==KNAV_COMPUTER)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c (" MK I");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c (" MK II");
		 }

	  else
	   if (i==KMISSILE_PYLON1 || i==KMISSILE_PYLON2 || i==KMISSILE_PYLON3)
		 {
		   if(ship_items[i].modifier==1) ZDrawString_c ("T1 20 Megatonnes.");
		   else
		   if(ship_items[i].modifier==2) ZDrawString_c ("T2 60 Megatonnes.");
		   else
		   if(ship_items[i].modifier==3) ZDrawString_c ("T3 150 Megatonnes.");


		 }

	   }	//end of fitted
		TextMode(1);
   }	//end of item obj_class match
   
   }	//end of for loop
return mouse_item;
}




spoint cargo_pos [] = 
{
	{ 40,100,270,200 },	//slot 0
	{ 40,140,320,250 },	//slot 1
	{ 40,180,250,230 },	//slot 2
	{ 40,220,257,270 },	//slot 3
};

extern int score,credits,start_time,accumulated_game_time;

//draw items in hold
void draw_hold_items(LSRAW * up_pict, LSRAW * down_pict, LSRAW *armed_pict, bool with_jettision)
{
extern hold_desc hold_items[MAX_HOLD_ITEMS];
extern int jettison_timer;

int i;
unsigned char number_string[12];
int alive_time,hours,mins,secs,stop_time;
char number_string1[12];
Point mouse_xy;

TextFont(21);
//TextSize(12);
//   ZMoveTo(10,20);
//   RGBForeColor(&rgbWhite);
#define COLOUR 225
SplatText_Transparent_Fade ("INVENTORY",280,6,235);
draw_topbottom_lines();




TextSize(9);
TextFace(normal);




for (i=0;i<MAX_HOLD_ITEMS;i++)
   {
   extern hold_desc cargos_array[NUMBER_OF_COMMODITIES];
   ZMoveTo(cargo_pos[i].sx,cargo_pos[i].sy);
   ZRGBForeColor(&rgbWhite);

   if (hold_items[i].id==-1) 
     {
      ZRGBForeColor(&rgbDarkGrey);
      ZDrawString_c("Empty cargo slot");
     }
     else
     {
      int slot;
      unsigned char temp_string[300];

      slot=hold_items[i].id;
      CToPascal(cargos_array[slot].string,temp_string);
      
      ZDrawString((unsigned char *) temp_string);
//      if (ship_items[i].status==-1) 
//       {
//       RGBForeColor(&rgbRed);
//       ZDrawString(" Lost!");
//       }
      }
      
   ZRGBForeColor(&rgbDarkGrey);

   if (cargo_pos[i].sx<320)
   ZLineTo(cargo_pos[i].lex,cargo_pos[i].ley);
   else
   {
   ZMoveTo(cargo_pos[i].sx,cargo_pos[i].sy);
   ZLineTo(cargo_pos[i].lex,cargo_pos[i].ley);   
   }
}

	if (with_jettision)
	{
//now draw jettison up buttons
   ZRGBForeColor(&rgbRed);

   ZMoveTo(20,100);
   ZLineTo(20,300);   
   ZMoveTo(20,300);
   ZLineTo(40,300);   
   ZMoveTo (42,304);
   ZDrawString_c ("Jettison Cargo");

  for (i=0;i<4;i++)
   {
    print_crunched_trans_pict(up_pict,4,cargo_pos[i].sy-18);
   }
//draw the mouse
    ZexGetMouse (&mouse_xy);

       for (i=0;i<4;i++)
           {
           if (mouse_xy.h>4 && mouse_xy.h<36
               && mouse_xy.v>cargo_pos[i].sy-18 && mouse_xy.v<cargo_pos[i].sy+14
               && hold_items[i].id!=-1)
                {
                   print_crunched_trans_pict(armed_pict,4,cargo_pos[i].sy-18);
                } 
           }
      
      

    if (LS_Left_Button()==1)
      {

       for (i=0;i<4;i++)
           {
           if (mouse_xy.h>4 && mouse_xy.h<36
               && mouse_xy.v>cargo_pos[i].sy-18 && mouse_xy.v<cargo_pos[i].sy+14
               && hold_items[i].id!=-1)
                {
                   print_crunched_trans_pict(down_pict,4,cargo_pos[i].sy-18);
                   //eject it
                   gen_specific_cargo(0,hold_items[i].id);
                   hold_items[i].id=-1;
                   jettison_timer=20;	//so we don't pick it back up immediately
                   play_zsound(jettison, sound_high_pri, c1_chan1, sound_vol_7);

                } 
           }
      }
    }	//end of with_jettison
//draw mouse cursor
    if (OGL_active==1)
    {
     ogl_update_mouse_cursor();
    }
    //else
    //print_crunched_trans_pict(cursor,mouse_xy.h,mouse_xy.v);

}



void draw_topbottom_lines()
{
extern int gRank,promotion_points;
unsigned char number_string[12];
int alive_time,hours,mins,secs,stop_time;
char number_string1[16];



#undef COLOUR
#define COLOUR 246

SplatText_Transparent_Fade ("Score",10,6,COLOUR);
ZexNumToString(score,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent_Fade ((char *)number_string,42,6,COLOUR);
      
SplatText_Transparent_Fade ("Credits",510,6,COLOUR);
ZexNumToString(credits,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent_Fade ((char *)number_string,560,6,COLOUR);

//   RGBForeColor(&rgbRed);
//ZMoveTo(0,20);
//ZLineTo(640,20);
      
#undef TIMEX
#define TIMEX 100
#define TIMEY 460
SplatText_Transparent_Fade ("Real time",10,TIMEY,COLOUR);
stop_time=LS_TickCount60();
alive_time=(stop_time-start_time);


draw_real_time(84,460,COLOUR);

#define textcol COLOUR
//rank - ties up with run_map_control
             SplatText_Transparent_Fade ("Rank",250,460,textcol);

			 SplatText_Transparent_Fade(rank_to_string(gRank),280,460,textcol);

             SplatText_Transparent_Fade ("Promotion Points",430,460,textcol);
             
             ZexNumToString(promotion_points,number_string);
             number_string[number_string[0]+1]='/';	//quick p to c hack
             
             if (gRank==ENS)
             {
             number_string[number_string[0]+2]='3';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }
             else
             if (gRank==LT)
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='5';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]=0;	//quick p to c hack
             }
            
             else
             if (gRank==LTCR)
             {
             number_string[number_string[0]+2]='6';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]=0;	//quick p to c hack
             }
            

             else
             if (gRank==CDR)
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='8';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==CAPT)
             {
             number_string[number_string[0]+2]='3';	//quick p to c hack
             number_string[number_string[0]+3]='5';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==RADM)
             {
             number_string[number_string[0]+2]='6';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==VADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='8';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==ADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]='0';	//quick p to c hack
             number_string[number_string[0]+8]=0;	//quick p to c hack
             }
             else
             if (gRank==FADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='N';	//quick p to c hack
             number_string[number_string[0]+3]='/';	//quick p to c hack
             number_string[number_string[0]+4]='A';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }




             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,544,460,textcol);

//promotion points


//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);


//#define TIMEX 560
//#define TIMEY 460
//SplatText_Transparent_Fade ("TOTAL TIME",480,TIMEY,COLOUR);
//stop_time=LS_TickCount60();
//alive_time=(stop_time-start_time)+accumulated_game_time;
//
//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);
//

//   RGBForeColor(&rgbRed);
//ZMoveTo(0,456);
//ZLineTo(640,456);

}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: rank_to_string          | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Feb 06
// +
// | DESCRIPTION: Produces a pointer to a constant string with a space at the front.
// +----------------------------------------------------------------ROUTINE HEADER----

const char* rank_to_string(int rank)
{
	if (rank==ENS) return " Ensign";
	else
		if (rank==LT) return " Lieutenant";
	else
		if (rank==LTCR) return " LT Commander";
	else
		if (rank==CDR) return " Commander";
	else
		if (rank==CAPT) return " Captain";
	else
		if (rank==RADM) return " Rear Admiral";
	else
		if (rank==VADM) return " Vice Admiral";
	else
		if (rank==ADM) return " Admiral";
	else
		if (rank==FADM) return " Fleet Admiral";

	
	return " Unknown";		// should never happen
}

const char* rank_to_string_no_space(int rank)
{
	if (rank==ENS) return "Ensign";
	else
		if (rank==LT) return "Lieutenant";
	else
		if (rank==LTCR) return "LT Commander";
	else
		if (rank==CDR) return "Commander";
	else
		if (rank==CAPT) return "Captain";
	else
		if (rank==RADM) return "Rear Admiral";
	else
		if (rank==VADM) return "Vice Admiral";
	else
		if (rank==ADM) return "Admiral";
	else
		if (rank==FADM) return "Fleet Admiral";

	
	return " Unknown";		// should never happen
}


