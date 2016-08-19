// Name:   		Damage.c
// Description:		This routine, modifies the damage
//			bar for the hull
// Date Started:	10/12/98
// Author(s):		ID
//
// Copyright(c) Lightsoft Software

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
#include "Damage.h"

//oldmac//#include <ToolUtils.h>
//oldmac//#include <resources.h>
//#include <stdio.h>
#include "dlp.h"
#include "zex_misc.h"
#include "sound.h"

#include "Error_handler.h"
#include "instrum_int.h"

#define NONE 0

#define CHAR_OFFSET 60  // in Pixels to the next character
#define ROW_SIZE 5
#define CHAR_LINES 12

//defs for shield
#define SHIELD_TOTAL 1000
#define SHIELD_OFFSET 60
#define ROW_WIDTH 5
#define HEIGHT 12


void _Draw_to_Double_Buffer_Hull(char *char_ptr, int x_pos, int y_pos);
void _Draw_to_Double_Buffer_Shield(char *char_ptr, int x_pos, int y_pos);
void _Draw_to_Double_Buffer_Box(int x_pos, int y_pos, int width);


extern int current_level;
static Ptr double_buffer;
static int screen_width;
Handle dam_pic;
Handle shd_pic;
//oldmac//int dummy;


int hull_strength,hull_damage;
static int shield_strength;
static int my_damage;
static int counter,counter2;
static int dummy;

void InitDamageBar()
 {
 dam_pic = (Handle) LS_GetResource('ZRAW',130);  //Get the Handle to the Resource
 if (dam_pic==0) report_error_c("Resource missing: ZRAW 130","",4);
 LS_HLock (dam_pic);

 shd_pic = (Handle) LS_GetResource('ZRAW',145);  //Get the Handle to the Resource
 if (shd_pic==0) report_error_c("Resource missing: ZRAW 145","",4);
 LS_HLock(shd_pic);

 counter2  = 3;
 dummy = 120;

 }


void Hull_NewLife(void)
 {
  //This is for setting up the hull bar when the ship first starts or when
  //The player has died and starts a new life.
  int i;
  hull_strength = 50;
  shield_strength = 1000;

  for (i = 0; i < 10;i++)
   {
    SplatBar(5,43+(i*7),460);
   }

   for (i = 0 ; i < 20; i++)
    {
     SplatShield(5,168+(i*7),460);
    }

  counter=3;
 }

//call if hull_Strength previously set 9from like load-game)
void Show_Hull_NewLife(void)
 {
  //This is for setting up the hull bar when the ship first starts or when
  //The player has died and starts a new life.
  int i;
//  hull_strength = 50;
//  shield_strength = 1000;

  for (i = 0; i < 10;i++)
   {
    SplatBar(5,43+(i*7),460);
   }


  counter=3;
 }


void damage_something()
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int gRadarRange;
//extern int fuel_val;
int damaged_system;
#if ZEX_LIVES_FOR_EVER
return;
#endif

//see if we can damage a non critical system
if (RangedRdm(0,100)>90)	//lose a system?
{
	 damaged_system=RangedRdm(KREAR_LASER,KAUTO_HULL_REPAIR);
 //        damaged_system=KNAV_COMPUTER;
		if (ship_items[damaged_system].status>0)	//was !=0 which is bad as when something is lost we set status to -1
		{											//so it could appear as if things were lost more than once
			switch (damaged_system)
			{
				case KREAR_LASER:
						  add_to_text_display("REAR LASER INCINERATED",1);
						  ship_items[KREAR_LASER].status=-1;	//lose the rear laser
					   break;

				case K360:
						  add_to_text_display("360 DEGREE TARGETING LOST",1);
						  ship_items[K360].status=-1;	//lose the 360
					   break;

				case KSHOCKWAVE:
						  add_to_text_display("SHOCKWAVE LOST",1);
						  ship_items[KSHOCKWAVE].status=-1;	//lose the shockwave
					   break;

				case KECM:
						  add_to_text_display("E.C.M. DESTROYED",1);
						  ship_items[KECM].status=-1;	//lose the ECM
					   break;

				case KMISSILE_FACTORY:
						  add_to_text_display("MISSILE FACTORY DESTROYED",1);
						  ship_items[KMISSILE_FACTORY].status=-1;	//lose the MF
					   break;

				case KFAST_DOCK:
						  add_to_text_display("FAST DOCK DESTROYED",1);
						  ship_items[KFAST_DOCK].status=-1;	//lose the FASTDOCK
					   break;


				case KFUEL_MAXIMISER:
						  add_to_text_display("FUEL MAXIMISER LOST",1);
						  ship_items[KFUEL_MAXIMISER].status=-1;	//lose NAV
	//                      if (ship_items[KNORMAL_FUEL_TANK].value1>180) ship_items[KNORMAL_FUEL_TANK].value1=180;	//lose the fuel
					   break;

				case KRETRO_ROCKETS:
						  add_to_text_display("RETRO THRUST LOST",1);
						  ship_items[KRETRO_ROCKETS].status=-1;	//lose RETRO
					   break;

				case KHYPERSPACE:
						  add_to_text_display("HYPERJUMP LOST",1);
						  ship_items[KHYPERSPACE].status=-1;	//lose warp jump
					   break;

				case KNAV_COMPUTER:
						 if(current_level>2)
						 {
						  add_to_text_display("NAVIGATION COMPUTER DESTROYED",1);
						  ship_items[KNAV_COMPUTER].status=-1;	//lose NAV
						 }
					   break;

				case KEJECTION:
						  add_to_text_display("EJECTION POD DESTROYED",1);
						  ship_items[KEJECTION].status=-1;	//lose EJECTION POD
					   break;

				case KTRADING_COMPUTER:
						  add_to_text_display("TRADING COMPUTER DESTROYED",1);
						  ship_items[KTRADING_COMPUTER].status=-1;	//lose TC
					   break;

				case KAUTO_HULL_REPAIR:
						  add_to_text_display("HULL REPAIR SYSTEM DESTROYED",1);
						  ship_items[KAUTO_HULL_REPAIR].status=-1;	//lose HRS
					   break;


				default :
					   break;
				}
		}
//         ship_items[damaged_system].status=0;	//inc damage to system (1=S, >1=U/S, 0=lost)

}
else	//item not lost but hit
if (RangedRdm(0,10)>8)	//damage a system?
{
		damaged_system=RangedRdm(KRADAR,KENGINE);	//pick something
        if (ship_items[damaged_system].status<1)	//lost or not fitted, so don't inc it into a serviceable condition Stu you twit!
		{
			return;
		}
         ship_items[damaged_system].status++;	//inc damage to system (1=S, >1=U/S, 0=not fitted, -1 lost)

		  if (ship_items[damaged_system].status==2)
         {
			switch (damaged_system)
			{
            case KRADAR:
                      add_to_text_display("RADAR DAMAGED",1);
                   break;

            case KLONG_RANGE_RADAR:
                      add_to_text_display("LONG RANGE RADAR DAMAGED",1);
                      gRadarRange=0;	//if damaged then drop down
                   break;
            case KCANNON:
                      add_to_text_display("CANNON DAMAGED",1);
                   break;
            case KLASER:
                      add_to_text_display("LASER DAMAGED",1);
                   break;
            case KMISSILES:
                      add_to_text_display("MISSILE CONTROLLER DAMAGED",1);
                   break;
            case KREPAIR_SYSTEMS:
                      add_to_text_display("REPAIR SYSTEMS DAMAGED",1);
                   break;
            case KFLIGHT_COMPUTER:
                      add_to_text_display("ORCS DAMAGED",1);
                   break;
            case KENGINE:
                      add_to_text_display("ENGINE HIT!",1);
                   break;
            default :
                   break;
			}
         }	//end of damage==2
        else	//do nothing if status is 3, lose it at 4
		if (ship_items[damaged_system].status==4)	//have we lost this system
		{
		switch (damaged_system)
	     {
            case KRADAR:
                      add_to_text_display("RADAR LOST",1);
                      ship_items[damaged_system].status=-1;	//lost it
                   break;

            case KLONG_RANGE_RADAR:
                      add_to_text_display("LONG RANGE RADAR DESTROYED",1);
                      ship_items[damaged_system].status=-1;	//lost it
                   break;
            case KCANNON:
                      add_to_text_display("CANNON LOST",1);
                      ship_items[damaged_system].status=-1;	//lost it
                   break;
            case KLASER:
                      add_to_text_display("LASER DESTROYED",1);
                      ship_items[damaged_system].status=-1;	//lost it
                   break;
            case KMISSILES:
                      add_to_text_display("MISSILE TARGETING LOST",1);
                      ship_items[damaged_system].status=-1;	//lost it
                   break;
            case KREPAIR_SYSTEMS:
                      add_to_text_display("REPAIR SYSTEMS DESTROYED",1);
                      ship_items[damaged_system].status=-1;	//lost it
                   break;
            case KFLIGHT_COMPUTER:
                      add_to_text_display("ORCS LOST",1);
                      ship_items[damaged_system].status=-1;	//lost it
                   break;
            default :
                   break;
         }
        }	//end of damage==4

}	//end of damage a system

}


void damage_me(int damage)
 {
 extern int tutorial;
 extern kit_desc ship_items[ITEMS_COUNT];
	#if ZEX_LIVES_FOR_EVER
	return;
	#endif

	play_zsound(shield_scratch, sound_low_pri, c1_chan1, sound_vol_7);
   if (tutorial==1) return;
   damage=damage/32;
   if (damage<1) damage=1;	//Round up to 1 so some damage is always done

   if (hull_strength==HULL_TOTAL && ship_items[KAUTO_HULL_REPAIR].status==1)
   {
		add_to_text_display("HULL AUTO REPAIR SYSTEM ACTIVE",0);
   }

   hull_strength -= damage;
   if (hull_strength < 0) {hull_strength = 0;}
   if (hull_strength >= 0)
     {
      hull_damage = HULL_TOTAL - hull_strength;
      if (hull_damage >50) { hull_damage = 50;}
      counter = 3;
     }
 }


//same as damage_me but no screetch sound
void damage_me_no_sound(int damage)
 {
   damage=damage/4;
   if (damage<1) damage=1;
   hull_strength -= damage;
   if (hull_strength < 0) {hull_strength = 0;}
   if (hull_strength >= 0)
     {
      hull_damage = HULL_TOTAL - hull_strength;
      if (hull_damage >50) { hull_damage = 50;}
      counter = 3;
     }
 }

void update_hull_damage()
{

  int empty_blocks;
  int remainder;
  int _start_point;
  int i;


	if (counter >0 || hull_damage>0)	//we must update if we are damaged so auto hull repair is displayed
	{

		for (i = 0; i < 10;i++)	//fill the bar
		{
			SplatBar(5,43+(i*7),460);
		}

		_start_point = 43;	//Now erase the damaged portion
		empty_blocks = hull_damage / 5;

		_start_point += (10 - empty_blocks)*7;

		for (i = 0; i < empty_blocks;i++)	//erase all full blocks
		{

			 SplatBar(0,_start_point+(i*7),460);

		}

		_start_point -= 7;

		remainder = hull_damage - (empty_blocks * 5);	//erase remainder

		if (remainder !=0)
		{
			 switch(remainder)
			 {
			 case 1:
					SplatBar(4,_start_point,460);
					break;
			 case 2:
					SplatBar(3,_start_point,460);
					break;
			 case 3:
					SplatBar(2,_start_point,460);
					break;
			 case 4:
					SplatBar(1,_start_point,460);
					break;
			 default :

					break;
			 }
		}

		counter --;

	}

}

 void update_shield_damage(int shield_value)
  {

  int empty_blocks;
  int full_blocks;

  int remainder;
  int start_point;
  int i;


/*
  if (shield_value <= 300)
   {
    	shd_pic = (Handle) LS_GetResource('ZRAW',129);  //Get the Handle to the Resource
 	if (shd_pic==0) report_error("Resource missing: ZRAW 129","",4);
 	LS_HLock(shd_pic);
   }
  else if (shield_value <= 600)
   {
    	shd_pic = (Handle) LS_GetResource('ZRAW',144);  //Get the Handle to the Resource
 	if (shd_pic==0) report_error("Resource missing: ZRAW 129","",4);
 	LS_HLock(shd_pic);

   }
  else
   {
    	shd_pic = (Handle) LS_GetResource('ZRAW',145);  //Get the Handle to the Resource
 	if (shd_pic==0) report_error("Resource missing: ZRAW 129","",4);
 	LS_HLock(shd_pic);

   }

 */

  if (shield_value < 0) {shield_value = 0;}
   full_blocks = shield_value / 50;

   if (full_blocks < 7)
   {
  	shd_pic = (Handle) LS_GetResource('ZRAW',129);  //Get the Handle to the Resource
 	if (shd_pic==0) report_error_c("Resource missing: ZRAW 129","",4);
 	LS_HLock(shd_pic);

   }
   else if (full_blocks < 14)
   {
    	shd_pic = (Handle) LS_GetResource('ZRAW',144);  //Get the Handle to the Resource
 	if (shd_pic==0) report_error_c("Resource missing: ZRAW 129","",4);
 	LS_HLock(shd_pic);

   }
   else
   {
    	shd_pic = (Handle) LS_GetResource('ZRAW',145);  //Get the Handle to the Resource
 	if (shd_pic==0) report_error_c("Resource missing: ZRAW 129","",4);
 	LS_HLock(shd_pic);

   }

  empty_blocks = ((1000 - shield_value) / 50);
  remainder = shield_value - (full_blocks * 50);
  start_point = 168;

  if (full_blocks > 0)
        {
         for (i = 0; i < full_blocks;i++)
          {
           SplatShield(5,start_point +(i * 7),460);
          }
        }

      if (empty_blocks > 0)
        {
        start_point = start_point + 133;
         for (i = 0; i < empty_blocks;i++)
          {
           SplatShield(0,start_point - (i * 7),460);
          }
        }


   start_point = 168 + (full_blocks * 7);

   if ((remainder >= 1) && (remainder <= 12))
        SplatShield(1,start_point,460);
   else if ((remainder >= 13) && (remainder <= 25))
   	SplatShield(2,start_point,460);
   else if ((remainder >= 26) && (remainder <= 38))
   	SplatShield(3,start_point,460);
   else if ((remainder >= 39) && (remainder < 50))
   	SplatShield(4,start_point,460);

  }

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatBar(int damage, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (damage)) + *dam_pic;
  _Draw_to_Double_Buffer_Hull(char_ptr,x_pos,y_pos);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void SplatShield(int damage, int x_pos, int y_pos)
 {
  char *shield_ptr;

  shield_ptr =  (SHIELD_OFFSET * (damage)) + *shd_pic;
  _Draw_to_Double_Buffer_Shield(shield_ptr,x_pos,y_pos);
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void SplatBox(int x_pos, int y_pos, int width)
 {
  _Draw_to_Double_Buffer_Box(x_pos,y_pos,width);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void _Draw_to_Double_Buffer_Hull(char *char_ptr, int x_pos, int y_pos)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;

double_buffer=screenaddr;
screen_width=screenwidth;



 db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)
  {
    for(a=0; a < ROW_SIZE; a++)
     {
      *db_ptr =  *char_ptr;
      char_ptr++;
      db_ptr++;
     }
  db_ptr-=ROW_SIZE;
  db_ptr+=screen_width;
 }
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void _Draw_to_Double_Buffer_Shield(char *char_ptr, int x_pos, int y_pos)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;

double_buffer=screenaddr;
screen_width=screenwidth;



 db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < HEIGHT; b++)
  {
    for(a=0; a < ROW_WIDTH; a++)
     {
      *db_ptr =  *char_ptr;
      char_ptr++;
      db_ptr++;
     }
  db_ptr-=ROW_WIDTH;
  db_ptr+=screen_width;
 }
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void _Draw_to_Double_Buffer_Box(int x_pos, int y_pos, int width)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;

double_buffer=screenaddr;
screen_width=screenwidth;



 db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < 10; b++)
  {
    for(a=0; a < width; a++)
     {
      *db_ptr =  0xFF;
      db_ptr++;
     }
  db_ptr-=width;
  db_ptr+=screen_width;
 }
}


