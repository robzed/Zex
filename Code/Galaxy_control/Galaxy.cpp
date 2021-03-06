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

// *************************************MODULE HEADER*********************************
// * FILENAME           : Galaxy.c
// * MODULE TITLE       :
// * PROJECT            : Zex
// * DATE STARTED       : 19:57:55 on 29th August 1999
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

//#include <stdlib.h>			// for srand
#include <time.h>
//oldmac//#include <QuickDraw.h>
//oldmac//#include <memory.h>
//oldmac//#include <timer.h>
//oldmac//#include <textutils.h>
#include "3d_structures_def.h"
#include "Galaxy.h"
#include	"zex_misc.h"
#include "monster_control_misc.h"
#include "game_defs.h"
#include "file_io.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"

#include "ship_control.h"
#include "input_control.h"
#include "game_pause.h"
#include "object_control_top.h"
#include "camera.h"
#include "dlp.h"
#include "CPanel.h"
#include "handle_level.h"
#include "Error_handler.h"
#include "inventory.h"
#include    "fpoly4.h"
#include "genpoly_int.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "Planet_Generator.h"
#include "instrum_int.h"
#include "map_control.h"
#include "sound.h"
#include "olg_text.h"
#include "OGL.h"

// ***********************************************************************************
// * CONSTANTS
// *
#define PLANET 1
#define MOTHERSHIP 2
#define WARPHOLE 3
#define SPECIAL 4
#define GALAXY_SCENARY 5
// ***********************************************************************************
// * TYPE DEFINITIONS
// *

typedef struct {
int which_index;
int what_type;
int screen_x;
int screen_y;
}item_under_mouse;

typedef struct {
int rating,id;
} dist_id;

// ***********************************************************************************
// * VARIABLE DECLARATIONS
// *
int zex_colour;
int zex_colour_delta=-1;

int mouse_was_down=0;
int selected_item_type;
int selected_item_index;
int selected_item_screen_x;
int selected_item_screen_y;

galactic_map *GM_ptr;	//ptr to the galactic map

extern const RGBColor	rgbBlack;
extern const RGBColor	rgbWhite;
extern const RGBColor	rgbRed;
extern const RGBColor	rgbOrange;
extern const RGBColor	rgbBlue		;
extern const RGBColor	rgbDarkBlue;
extern const RGBColor	rgbYellow;
extern const RGBColor	rgbPurple;
extern const RGBColor	rgbGreen;
extern const RGBColor	rgbLightBlue;
extern const RGBColor	rgbGrey;
extern const RGBColor	rgbDarkGrey;

extern kit_desc ship_items[ITEMS_COUNT];
extern int difficulty;
extern int OGL_active;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
void make_planets();
void setup_motherships();
void calc_supply(int pos_x, int pos_y, int quadrant,dist_id * return_data);
void find_object_under_mouse(int mouse_x, int mouse_y, item_under_mouse* fill_me_baby);
void ShowItemInfo(int which_quad, int selected_item_type, int selected_item_index, int x, int y);
void setup_warpholes();
void reset_unique_number();
int get_unique_number();
void setup_specials();
void setup_scenary();
void center_x_and_y(_2D* coords);

//make four quadrants; see 3d-structures_def.h for the definition of
//a galaxy; err, not the real definition obviously...
void make_new_galaxy()
{
//oldmac//extern QDGlobals qd;
//oldmac//show_text_in_loading_screen("Zex: make_new_galaxy");
//set the random seed for three different galaxies depending on difficulty
zex_colour=255;

reset_unique_number();

//if (difficulty==16)
//qd.randSeed=0x69656868;	//good seed afaik
//qd.randSeed=0x72626868;	//good seed afaik
//qd.randSeed=0x73624868;	//good seed afaik
//oldmac//qd.randSeed=0x23621C69;	//good seed afaik
internal_srand(0x23621C69);

//else
//if (difficulty==8)
//qd.randSeed=0x12345678;
//else
//qd.randSeed=0x87654321;

init_zex_random();

//int temp=RangedRdm(0,100);

selected_item_type=-1;	//nothin selected
init_gen_planet_name();	//make sure name generator is initialised
make_planets();
setup_motherships();
setup_warpholes();
setup_specials();
setup_scenary();	//galaxies etc
init_map_control();

//OK, now go to random mode for game
//oldmac//	GetDateTime((unsigned long *)&qd.randSeed);		// Randomize random seed.
internal_srand(time(NULL)+SDL_GetTicks());
init_zex_random();

}

//galaxy set up as
//quad 0     |  quad 1
//           |
//---------------------
//           |
//quad 2     |  quad 3

void setup_motherships()
{
int section_x,section_y,mship_counter;
int mship_index;
int quadrant;
int planet;
int section_start_x,section_start_y;
int quad_x,quad_y;
int random_multiplier,n;
dist_id supply_route_data;


random_multiplier=SECTION_SIZE/65535;	//our random routine is 16 bit so we mult it's return by this value

for (quadrant=0;quadrant<4;quadrant++)
      {
      //calc quad start x/y
      mship_index=0;

      if (quadrant==0)
          {
          quad_x=GALAXY_START_X;
          quad_y=GALAXY_START_Y;
          }
      else
      if (quadrant==1)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=GALAXY_START_Y;
          }
      else
       if (quadrant==2)
          {
          quad_x=GALAXY_START_X;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }
      else
       if (quadrant==3)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }


       for (section_y=0;section_y<NUMBER_OF_SECTIONS_IN_A_QUAD_Y;section_y++)
         {
            //calc section start coords
           section_start_y=quad_y - section_y*SECTION_SIZE;

           for (section_x=0;section_x<NUMBER_OF_SECTIONS_IN_A_QUAD_X;section_x++)
             {
                //calc section start coords
                section_start_x=section_x*SECTION_SIZE+quad_x;

                for (mship_counter=0;mship_counter<NUMBER_OF_MOTHERSHIPS_IN_A_SECTION;mship_counter++)
                  {
					float pos_x,pos_y;
					int supply_rating,max_power;	//0-100

                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].visited=0;
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].shield_value=25500;

                    max_power=RangedRdm(10,100);
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].max_power_rating=max_power;
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].power_rating=max_power/4;	//start at 25% of max

                    pos_x=section_start_x + ZexRandom()*random_multiplier;
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].real_coords.x=
                    pos_x;
                    pos_y=section_start_y - ZexRandom()*random_multiplier;	  //note the subtract
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].real_coords.y=
                    pos_y;
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].surrender_timer=0;
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].galactic_id=get_unique_number();

                    //first mothership is Earth, rest are alien
//                    if (quadrant==0 && section_y==0 && section_x==0 && mship_counter==0)
                    if (difficulty==16)	//more human motherships on easier levels
                    {
                    if (mship_counter==0 && RangedRdm(0,100)>30)
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance=HUMAN;
                    else
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance=ALIEN;
                    }
                    else
                    if (difficulty==8)
                    {
                    if (mship_counter==0 && RangedRdm(0,100)>60)
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance=HUMAN;
                    else
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance=ALIEN;
                    }
                    else
                    if (difficulty==1)
                    {
                    if (mship_counter==0 && RangedRdm(0,100)>80)
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance=HUMAN;
                    else
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance=ALIEN;
                    }

                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].like_zex=1;
                    calc_supply(static_cast<int>(pos_x),static_cast<int>(pos_y),quadrant, &supply_route_data);

                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].supply_rating=supply_route_data.rating;
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].supply_planet_index=supply_route_data.id;
                    //calc production rating as
                    //supply_rating*power_rating/100
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].production_rating=
                    (GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].power_rating
                    * supply_route_data.rating)/100;
                    //max ships in dock as production_rating/5
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].max_ships_in_dock=
                    static_cast<int>(GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].production_rating/5);
                    //minimum of 2
                    if (GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].max_ships_in_dock<2)
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].max_ships_in_dock=2;
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].ships_in_dock=0;
                    //set up dock queue
                    for (n=0;n<8;n++)
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].docked_queue[n].in_use=0;

                    //callsign
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].callsign=
                    make_callsign(GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance);

                    mship_index++;
                  }	//END OF mship_counter
             }	//end of section_x
         }	//end of section_y
      }	//end of quadrant

}


void setup_warpholes()
{
int section_x,section_y,mship_counter;
int mship_index;
int quadrant;
int toll;
int section_start_x,section_start_y;
int quad_x,quad_y,wx,wy;
int random_multiplier,n;
dist_id supply_route_data;


random_multiplier=SECTION_SIZE*NUMBER_OF_SECTIONS_IN_A_QUAD_X/65535;	//our random routine is 16 bit so we mult it's return by this value

for (quadrant=0;quadrant<4;quadrant++)
      {
      //calc quad start x/y
      mship_index=0;

      if (quadrant==0)
          {
          quad_x=GALAXY_START_X;
          quad_y=GALAXY_START_Y;
          toll=2000;
          }
      else
      if (quadrant==1)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=GALAXY_START_Y;
          toll=25000;
          }
      else
       if (quadrant==2)
          {
          quad_x=GALAXY_START_X;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          toll=50000;
          }
      else
       if (quadrant==3)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          toll=60000;
          }

       wx=RangedRdm(10000,55500);
       wx*=random_multiplier;
       wx+=quad_x;
       wy=RangedRdm(10000,55500);	//somwhere in middle
       wy*=random_multiplier;
       wy=quad_y-wy;
       GM_ptr->the_quadrant[quadrant].the_warphole.real_coords.x=wx;
       GM_ptr->the_quadrant[quadrant].the_warphole.real_coords.y=wy;
       GM_ptr->the_quadrant[quadrant].the_warphole.toll=toll;
       GM_ptr->the_quadrant[quadrant].the_warphole.galactic_id=get_unique_number();

      }	//end of quadrant

}

int get_current_warphole_toll()
{
extern int current_quad;
	return  GM_ptr->the_quadrant[current_quad].the_warphole.toll;
}


//returns zero to 100 depending on closeness to inhabited planet
void calc_supply(int pos_x, int pos_y, int quadrant,dist_id * return_data)
{
int closest_distance=100000000;
int planet_index;
int planet_x,planet_y;
int distance;
int return_val,closest_planet_index;


closest_planet_index=-1;


//within 1e6=100
//within 1e7=0
 for (planet_index=0;planet_index<NUMBER_OF_PLANETS_PER_QUAD;planet_index++)
    {
     if (GM_ptr->the_quadrant[quadrant].quadrant_planets[planet_index].inhabited==1)
      {
		planet_x=static_cast<int>(GM_ptr->the_quadrant[quadrant].quadrant_planets[planet_index].real_coords.x);
        planet_y=static_cast<int>(GM_ptr->the_quadrant[quadrant].quadrant_planets[planet_index].real_coords.y);
        distance=get_distance_to_from_point(planet_x,planet_y,pos_x,pos_y);
        if (distance<closest_distance)
         {
          closest_distance=distance;
          closest_planet_index=planet_index;
         }
      }
    }
 //now calc supply rating
 if (closest_distance<1000000)
  return_val=100;
 else
 if (closest_distance>10000000)
 {
 return_val=0;
 }
 else
 //somewhere between 1 and 10 million
 {
 return_val=100-(closest_distance/100000);
 }

 return_data->rating=return_val;
 return_data->id=closest_planet_index;
}



void make_planets()
{
int quadrant;
int planet;
int inhabited;
int quad_x,quad_y;
int random_multiplier;
float pos_x,pos_y;

random_multiplier=(SECTION_SIZE*NUMBER_OF_SECTIONS_IN_A_QUAD_X)/65535;	//our random routine is 16 bit so we mult it's return by this value

for (quadrant=0;quadrant<4;quadrant++)
      {
      //calc quad start x/y
      if (quadrant==0)
          {
          quad_x=GALAXY_START_X;
          quad_y=GALAXY_START_Y;
          }
      else
      if (quadrant==1)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=GALAXY_START_Y;
          }
      else
       if (quadrant==2)
          {
          quad_x=GALAXY_START_X;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }
      else
       if (quadrant==3)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }

       for (planet=0;planet<NUMBER_OF_PLANETS_PER_QUAD;planet++)
         {
         int gnp_rating;
         //less inhabited planets=easier
         if (difficulty==16)
         {
         inhabited=RangedRdm(0,100);
         if (inhabited>85) inhabited=1;
         else inhabited=0;
         }
         else
         if (difficulty==8)
         {
         inhabited=RangedRdm(0,100);
         if (inhabited>80) inhabited=1;
         else inhabited=0;
         }
         else
         {
         inhabited=RangedRdm(0,100);
         if (inhabited>75) inhabited=1;
         else inhabited=0;
         }

         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].seed=RangedRdm(0,65535);
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].inhabited=inhabited;
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].type=RangedRdm(1,10);	//1-5=agri, 6-10=tech

         if (inhabited==1)
         {
         gnp_rating=GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].type;
         if (gnp_rating>=6) gnp_rating-=5;	//range 1 to 5
         if (gnp_rating==1)
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].production_quota=RangedRdm(2,20);
         else
         if (gnp_rating==2)
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].production_quota=RangedRdm(21,40);
         else
         if (gnp_rating==3)
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].production_quota=RangedRdm(41,60);
         else
         if (gnp_rating==4)
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].production_quota=RangedRdm(61,80);
         else
         if (gnp_rating==5)
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].production_quota=RangedRdm(81,100);
         }
         else
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].production_quota=0;
         //x and y
                    pos_x=quad_x + ZexRandom()*random_multiplier;
                    GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].real_coords.x=
                    pos_x;
                    pos_y=quad_y - ZexRandom()*random_multiplier;	 //note the subtract
                    GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].real_coords.y=
                    pos_y;

                    gen_planet_name(GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].seed,
                    GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].name);
         GM_ptr->the_quadrant[quadrant].quadrant_planets[planet].galactic_id=get_unique_number();

         }	//end of planet
      }	//end of quadrant
}



void setup_scenary()	//galaxies etc
{
int quadrant;
int item;
int inhabited;
int quad_x,quad_y;
int random_multiplier;
float pos_x,pos_y;
_2D Gpos;

random_multiplier=(SECTION_SIZE*NUMBER_OF_SECTIONS_IN_A_QUAD_X)/65535;	//our random routine is 16 bit so we mult it's return by this value

for (quadrant=0;quadrant<4;quadrant++)
      {
      //calc quad start x/y
      if (quadrant==0)
          {
          quad_x=GALAXY_START_X;
          quad_y=GALAXY_START_Y;
          }
      else
      if (quadrant==1)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=GALAXY_START_Y;
          }
      else
       if (quadrant==2)
          {
          quad_x=GALAXY_START_X;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }
      else
       if (quadrant==3)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }

       for (item=0;item<NUMBER_OF_SCENARY_PER_QUAD;item++)
         {

         //x and y
                    pos_x=quad_x + ZexRandom()*random_multiplier;
                    pos_y=quad_y - ZexRandom()*random_multiplier;	 //note the subtract

                    Gpos.x=static_cast<int>(pos_x);
                    Gpos.y=static_cast<int>(pos_y);
                    center_x_and_y(&Gpos);	//move the item into the center of the section


                    GM_ptr->the_quadrant[quadrant].quadrant_scenary[item].real_coords.x=
                    Gpos.x;

                    GM_ptr->the_quadrant[quadrant].quadrant_scenary[item].real_coords.y=
                    Gpos.y;

                    GM_ptr->the_quadrant[quadrant].quadrant_scenary[item].galactic_id=get_unique_number();
                    GM_ptr->the_quadrant[quadrant].quadrant_scenary[item].seed=RangedRdm(0,65535);

         }	//end of planet
      }	//end of quadrant
}

void setup_specials()	//asteroids, space cows etc
{
int quadrant;
int item;
int inhabited;
int quad_x,quad_y;
int random_multiplier;
float pos_x,pos_y;
_2D Gpos;

random_multiplier=(SECTION_SIZE*NUMBER_OF_SECTIONS_IN_A_QUAD_X)/65535;	//our random routine is 16 bit so we mult it's return by this value

for (quadrant=0;quadrant<4;quadrant++)
      {
      //calc quad start x/y
      if (quadrant==0)
          {
          quad_x=GALAXY_START_X;
          quad_y=GALAXY_START_Y;
          }
      else
      if (quadrant==1)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=GALAXY_START_Y;
          }
      else
       if (quadrant==2)
          {
          quad_x=GALAXY_START_X;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }
      else
       if (quadrant==3)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }

       for (item=0;item<NUMBER_OF_SPECIALS_PER_QUAD;item++)
         {
         int the_code;
         the_code=RangedRdm(0,100);
         if (the_code>80) the_code=CORRAL_FACING_RIGHT;
		 else
         if (the_code>60) the_code=ASTEROID_FIELD1;
		 else
         if (the_code>40) the_code=ASTEROID_FIELD2;
		 else
         if (the_code>20) the_code=COWS1;
		 else
         the_code=COWS2;

         //x and y
                    pos_x=quad_x + ZexRandom()*random_multiplier;
                    pos_y=quad_y - ZexRandom()*random_multiplier;	 //note the subtract
                    Gpos.x=static_cast<int>(pos_x);
                    Gpos.y=static_cast<int>(pos_y);
                    center_x_and_y(&Gpos);	//move the item into the center of the section

                    GM_ptr->the_quadrant[quadrant].quadrant_specials[item].real_coords.x=
                    Gpos.x;
                    GM_ptr->the_quadrant[quadrant].quadrant_specials[item].real_coords.y=
                    Gpos.y;

                    GM_ptr->the_quadrant[quadrant].quadrant_specials[item].code=the_code;
                    GM_ptr->the_quadrant[quadrant].quadrant_specials[item].galactic_id=get_unique_number();

         }	//end of planet
      }	//end of quadrant
}

#define NEAR_EDGE 400000
void center_x_and_y(_2D* coords)
{
int temp_x,temp_y;
int itemp;
  temp_x=coords->x;
  temp_y=coords->y;
  itemp=temp_x/SECTION_SIZE;
  itemp=itemp*SECTION_SIZE;
  itemp=temp_x-itemp;	//temp_x%SECTION_SIZE

  if (itemp<NEAR_EDGE) temp_x+=NEAR_EDGE;
  if (itemp>SECTION_SIZE-NEAR_EDGE) temp_x-=NEAR_EDGE;
  coords->x=temp_x;

  itemp=temp_y/SECTION_SIZE;
  itemp=itemp*SECTION_SIZE;
  itemp=temp_y-itemp;	//temp_x%SECTION_SIZE

//  temp=temp_y%SECTION_SIZE;
  if (itemp<NEAR_EDGE) temp_y+=NEAR_EDGE;	//moving on up
  if (itemp>SECTION_SIZE-NEAR_EDGE) temp_y-=NEAR_EDGE;	//moving on down
  coords->y=temp_y;
}

extern int training_mission, tutorial;
extern	DynObjectsFixedSize *ocb_ptr;

int get_supply_rating_docked(int ocb_slot)
{

if (training_mission!=0 || tutorial!=0) return 50;	//training mission; not in galaxy
else
return static_cast<int>((*ocb_ptr).object_list[ocb_slot].mship_ptr->supply_rating);
}

//int get_supply_rating(int mothership_index)
//{
//
//
//show_text_in_loading_screen("Zex: get_supply_rating");
//
//if (in_ramparian_space==0) return 50;	//training mission; not in galaxy
//else
//if (mothership_index<0 || mothership_index>NUMBER_OF_MOTHERSHIPS_PER_QUAD*4) return 50;
//else
// {
// int quad;
// quad=mothership_index/NUMBER_OF_MOTHERSHIPS_PER_QUAD;
// return GM_ptr->the_quadrant[quad].quadrant_motherships[mothership_index].supply_rating;
// }
//}
//
//


static item_under_mouse last_item;	//the thing we clicked on last
static int warp_scale=1;
static float specials_colour=215;
static float structures_colour=215;

//info strings
//Str63 info_line_1;
//bit of a biggy here
void handle_galactic_map()
{
LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
//oldmac//extern GDHandle				mZexGDH;
extern const RGBColor	rgbDarkBlue;
extern	DynObjectsFixedSize *ocb_ptr;
RGBColor my_rgb;
int my_red,my_green,my_blue;	//colour conversion
             Rect arc_rect;
int quad,ship_x,ship_y;
unsigned char number_string[12];
int mship_index,planet_index;
            float mship_x,mship_y;
            int zex_x,zex_y;
            int disp_x,disp_y;
            int disp_x_div,disp_y_div;
Point mouse_xy;
extern LSRAW *map_cursor_picture;
extern Handle map_cursor_H;
item_under_mouse this_item;
int mouse_x,mouse_y,zex_display_x,zex_display_y;

extern LSRAW *small_flare_picture;

extern LSRAW *large_flare_picture;
extern float targ_x,targ_y;	//from TARG command in level file
extern int tutorial,current_quad;
extern int show_bg_gfx_on_map;

TextSize (9);
TextFont (21);
TextFace(0);	//stop it bing bold if we had been trading



	//calc current quadrant
	ship_x=static_cast<int>((*ocb_ptr).object_list[0].Oworldx);
	ship_y=static_cast<int>((*ocb_ptr).object_list[0].Oworldy);

    ZexGetMouse (&mouse_xy);
    mouse_x=mouse_xy.h; mouse_y=mouse_xy.v;

    //clamp mouse
    if (mouse_x<0) mouse_x=0;
    else
    if (mouse_x>640-16) mouse_x=640-16;

    if (mouse_y<0) mouse_y=0;
    else
    if (mouse_y>479-16) mouse_y=479-16;

//calc quadrant
//	  if (ship_x<GALAXY_WIDTH/2)
//	     {
//	     //quad 0 or 2
//	      if (ship_y<GALAXY_H/2)
//	        quad=2;
//	      else
//	        quad=0;
//	     }
//	  else
//	     {
//	     //quad 1 or 3
//	      if (ship_y<GALAXY_H/2)
//	        quad=3;
//	      else
//	        quad=1;
//	     }
quad=current_quad;

//get bg picture
//it may be possible we don't have the memory here, which is fine, and not a fatal error

//mouse clicks
if (training_mission==0 && tutorial==0)
{
if (LS_Left_Button()==0)
 {
 if (mouse_was_down==1)
    {
    //down up sequence complete
    //find out what item is under the mouse
    find_object_under_mouse(mouse_x, mouse_y,&this_item);
    if (this_item.what_type>-1)
         {
         //valid item selected
         selected_item_type=this_item.what_type;
         selected_item_index=this_item.which_index;
         selected_item_screen_x=this_item.screen_x;
         selected_item_screen_y=this_item.screen_y;
        //poke coords into Nav display
         if (selected_item_type==MOTHERSHIP)
          {
           play_zsound_always(select_mship, sound_high_pri, c1_chan1, sound_vol_7);

           targ_x=GM_ptr->the_quadrant[quad].quadrant_motherships[selected_item_index].real_coords.x;
           targ_y=GM_ptr->the_quadrant[quad].quadrant_motherships[selected_item_index].real_coords.y;
          }
         else
         if (selected_item_type==PLANET)
          {
                   play_zsound_always(select_planet, sound_high_pri, c1_chan1, sound_vol_7);

           targ_x=GM_ptr->the_quadrant[quad].quadrant_planets[selected_item_index].real_coords.x;
           targ_y=GM_ptr->the_quadrant[quad].quadrant_planets[selected_item_index].real_coords.y;
          }
         else
         if (selected_item_type==GALAXY_SCENARY)
          {
                   play_zsound_always(select_planet, sound_high_pri, c1_chan1, sound_vol_7);

           targ_x=GM_ptr->the_quadrant[quad].quadrant_planets[selected_item_index].real_coords.x;
           targ_y=GM_ptr->the_quadrant[quad].quadrant_planets[selected_item_index].real_coords.y;
          }

         else
         if (selected_item_type==SPECIAL)
          {
                   play_zsound_always(select_planet, sound_high_pri, c1_chan1, sound_vol_7);

           targ_x=GM_ptr->the_quadrant[quad].quadrant_specials[selected_item_index].real_coords.x;
           targ_y=GM_ptr->the_quadrant[quad].quadrant_specials[selected_item_index].real_coords.y;
          }
         else
         if (selected_item_type==WARPHOLE)
          {
                   play_zsound_always(select_planet, sound_high_pri, c1_chan1, sound_vol_7);

           targ_x=GM_ptr->the_quadrant[quad].the_warphole.real_coords.x;
           targ_y=GM_ptr->the_quadrant[quad].the_warphole.real_coords.y;
          }

         }
    else
         {
         play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

         selected_item_type=-1;
         }
    }
   mouse_was_down=0;	//registered
 }
else mouse_was_down=1;

}	//end of not training

//if (zex_colour>250) zex_colour=0;
if(zex_colour>255) zex_colour_delta=-zex_colour_delta;
if(zex_colour<247) zex_colour_delta=-zex_colour_delta;

zex_colour+=zex_colour_delta;


     //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
     the_drawing_buffer=Get_back_buffer();
// 	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);
//	RGBBackColor (&rgbBlue);
//    erase_zex_rect(640,480);	  //in pictures
//	EraseRect(&the_drawing_buffer->portRect);
#if OGL==1
    if (OGL_active==1)
    {
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);	//red
	glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    erase_zex_rect_colour(640,480,244);	//white

#else
	extern int gShowGalacticMap;
	if (gShowGalacticMap==1)
	{
		erase_zex_rect_colour(640,480,244);	//white
	}
#endif

if (training_mission!=0 || tutorial!=0)
{
ZMoveTo(180,200);
ZDrawString_c ("Galactic Map not available in training missions. Press G to return.");
return;
}




    disp_y_div=(GALAXY_WIDTH/2)/460;	//margin of 10 above and 10 below
    disp_x_div=(GALAXY_WIDTH/2)/620;	//amrgin of 10 left and 10 right
#define col 246
    SplatText_Transparent_Fade ("Galactic Map ",230,6,col);
    #define QPOS 334
    if (quad==0)
    SplatText_Transparent_Fade ("Alpha Quad",QPOS,6,col);
    else
    if (quad==1)
    SplatText_Transparent_Fade ("Beta Quad",QPOS,6,col);
    else
    if (quad==2)
    SplatText_Transparent_Fade ("Gamma Quad",QPOS,6,col);
    else
    if (quad==3)
    SplatText_Transparent_Fade ("Delta Quad",QPOS,6,col);


    draw_topbottom_lines();

//display planets
    for (mship_index=0;mship_index<NUMBER_OF_PLANETS_PER_QUAD;mship_index++)

            {
             int colour;

                     mship_x=GM_ptr->the_quadrant[quad].quadrant_planets[mship_index].real_coords.x;
                     mship_y=GM_ptr->the_quadrant[quad].quadrant_planets[mship_index].real_coords.y;
                     if (quad==1 || quad==3) mship_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) mship_y-=GALAXY_WIDTH/2;	//normalise y
                     mship_y=GALAXY_WIDTH/2-mship_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=static_cast<int>(mship_x/disp_x_div+10);
                     disp_y=static_cast<int>(mship_y/disp_y_div+10);
                     if (GM_ptr->the_quadrant[quad].quadrant_planets[mship_index].inhabited==1)
                     {
                     colour=66;
                     //RGBForeColor(&rgbPurple);	//for the text
                     dlp_draw_rounded_box(disp_y-2,disp_x-2,disp_y+2,disp_x+2,colour);

                     }
                     else
                     {
                     colour=204; // blue
                      draw_line(disp_x-1,disp_y,disp_x+2,disp_y,colour);
                      draw_line(disp_x,disp_y-1,disp_x,disp_y+2,colour);	//small cross for uninhabited planets

                     }

                     if (GM_ptr->the_quadrant[quad].quadrant_planets[mship_index].inhabited==1)
                       {
                       disp_x+=10;
                       disp_y+=4;
                       if (disp_x > 580) disp_x=disp_x-50;
 //                      if (disp_x<100) disp_x=120;
                       #if OGL==1
                       if (OGL_active==1)
                        {
                             OGL_move_and_set_colour(disp_x, disp_y,colour);
                             OGL_draw_helveticaP( GM_ptr->the_quadrant[quad].quadrant_planets[mship_index].name);	 //x,y,colour

                        }
                       else
                       {
                       MoveTo(disp_x,disp_y);
                       DrawString(GM_ptr->the_quadrant[quad].quadrant_planets[mship_index].name);
                       }
                       #else
                       MoveTo(disp_x,disp_y);
                       DrawString(GM_ptr->the_quadrant[quad].quadrant_planets[mship_index].name);
                       #endif
                       }
//                     MoveTo(disp_x,disp_y);
//                     LineTo(disp_x,disp_y);


            }

//display scenary (debug

if (0)
{
    for (mship_index=0;mship_index<NUMBER_OF_SCENARY_PER_QUAD;mship_index++)

            {
             int colour;

                     mship_x=GM_ptr->the_quadrant[quad].quadrant_scenary[mship_index].real_coords.x;
                     mship_y=GM_ptr->the_quadrant[quad].quadrant_scenary[mship_index].real_coords.y;
                     if (quad==1 || quad==3) mship_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) mship_y-=GALAXY_WIDTH/2;	//normalise y
                     mship_y=GALAXY_WIDTH/2-mship_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=static_cast<int>(mship_x/disp_x_div+10);
                     disp_y=static_cast<int>(mship_y/disp_y_div+10);
                     //colour=247;
                     //RGBForeColor(&rgbPurple);	//for the text
                     draw_line(disp_x,disp_y,disp_x+1,disp_y,247);
//                     dlp_draw_rounded_box(disp_y-2,disp_x-2,disp_y+2,disp_x+2,colour);


            }
}

//show warphole
    zex_x=static_cast<int>(GM_ptr->the_quadrant[quad].the_warphole.real_coords.x);
    zex_y=static_cast<int>(GM_ptr->the_quadrant[quad].the_warphole.real_coords.y);
    if (quad==1 || quad==3) zex_x-=GALAXY_WIDTH/2;	//normalise x
    if (quad==0 || quad==1) zex_y-=GALAXY_WIDTH/2;	//normalise y

	 zex_y=GALAXY_WIDTH/2-zex_y;	//normalise y some more :)
	 //now x is 0=left, 20e6 right
	 //y =0=top, 20e6=bottom
	 disp_x=zex_x/disp_x_div+10;
	 disp_y=zex_y/disp_y_div+10;

    print_pict_scaled(large_flare_picture, disp_x, disp_y, warp_scale, 1);
	 ZRGBForeColor(&rgbYellow);	//human
	 arc_rect.top=disp_y-2;
	 arc_rect.bottom=disp_y+2;
	 arc_rect.left=disp_x-2;
	 arc_rect.right=disp_x+2;
//                     PaintRect(&arc_rect);
	 dlp_draw_box(disp_y-2,disp_x-2,disp_y+2, disp_x+2,13);

	if (warp_scale<0) warp_scale=0;	//ummm.
	warp_scale+=1;
	if (warp_scale>12) warp_scale=1;

//a quadrant is about 20000000 units along a side, so we need to scale from this, centered on Zex
//display motherships



//display Zex - yellow
    zex_x=ship_x;
    zex_y=ship_y;
                     if (quad==1 || quad==3) zex_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) zex_y-=GALAXY_WIDTH/2;	//normalise y
                     zex_y=GALAXY_WIDTH/2-zex_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=zex_x/disp_x_div+10;
                     disp_y=zex_y/disp_y_div+10;
//					 add_to_dlp(DRAW_FILLED_BOX,disp_y,disp_x,disp_y+2,disp_x+2,zex_colour,0);
//                     dlp_plot(disp_x,disp_y,zex_colour);
//save zex screen coords
					 zex_display_x=disp_x;
					 zex_display_y=disp_y;

                     if (disp_x>0 && disp_x<640 && disp_y>0 && disp_y<479)
					 {
						dlp_draw_box(disp_y-2,disp_x-2,disp_y+2, disp_x+2,zex_colour);
                     }
//                    convert_to_rgb(zex_colour, &my_red, &my_green, &my_blue);
//                     my_rgb.red=(short) my_red<<8;
//                     my_rgb.green=(short) my_green<<8;
//                     my_rgb.blue=(short) my_blue<<8;
//                     RGBForeColor(&my_rgb);	//human
//                     arc_rect.top=disp_y-2;
//                     arc_rect.bottom=disp_y+2;
//                     arc_rect.left=disp_x-2;
//                     arc_rect.right=disp_x+2;
//                     PaintRect(&arc_rect);
                     //show fuel and shields
                     //calc correct x and y
                     if (disp_x<10) { disp_x=10; }	//to the left
                     else
                     if (disp_x>520)
                       { disp_x=520;}	//to the right


                     if (disp_y<10) disp_y=10;

                     if (disp_y>470) disp_y=470;

                     #if OGL
                     if (OGL_active==1)
                     {
                             OGL_move_and_set_colour(disp_x+6, disp_y+3,zex_colour);
                             OGL_draw_helveticaP("Fuel: ");	//x,y,colour
                             ZexNumToString(ship_items[KNORMAL_FUEL_TANK].value1,number_string);
                             OGL_draw_helveticaP(number_string);	//x,y,colour

                             OGL_move_and_set_colour(disp_x+50, disp_y+3,zex_colour);
                             OGL_draw_helveticaP("Shields: ");	//x,y,colour
                             ZexNumToString(ship_items[KSHIELDS].value1/10,number_string);
                             OGL_draw_helveticaP(number_string);	//x,y,colour

                     }
                     else
                     {
                     MoveTo(disp_x+6,disp_y+3);
                     DrawString("Fuel: ");
                     ZexNumToString(ship_items[KNORMAL_FUEL_TANK].value1,number_string);
                     DrawString (number_string);

                     MoveTo(disp_x+50,disp_y+3);
                     DrawString("Shields: ");
                     ZexNumToString(ship_items[KSHIELDS].value1/10,number_string);
                     DrawString (number_string);
                     }
                     #else
                     MoveTo(disp_x+6,disp_y+3);
                     DrawString_c("Fuel: ");
                     ZexNumToString(ship_items[KNORMAL_FUEL_TANK].value1,number_string);
                     DrawString (number_string);

                     MoveTo(disp_x+50,disp_y+3);
                     DrawString_c("Shields: ");
                     ZexNumToString(ship_items[KSHIELDS].value1/10,number_string);
                     DrawString (number_string);
                     #endif
//                     LineTo(disp_x,disp_y);

////display motherships
    for (mship_index=0;mship_index<NUMBER_OF_MOTHERSHIPS_PER_QUAD;mship_index++)

            {
            int colour;
                     mship_x=GM_ptr->the_quadrant[quad].quadrant_motherships[mship_index].real_coords.x;
                     mship_y=GM_ptr->the_quadrant[quad].quadrant_motherships[mship_index].real_coords.y;
                     if (quad==1 || quad==3) mship_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) mship_y-=GALAXY_WIDTH/2;	//normalise y
                     mship_y=GALAXY_WIDTH/2-mship_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=static_cast<int>(mship_x/disp_x_div+10);
                     disp_y=static_cast<int>(mship_y/disp_y_div+10);
                     if (GM_ptr->the_quadrant[quad].quadrant_motherships[mship_index].allegiance==ALIEN)
                     colour=215;
                     else
                     colour=225;	//green
//                     RGBForeColor(&rgbRed);	//alien
//                     else
//                     RGBForeColor(&rgbGreen);	//human
//                     arc_rect.top=disp_y-1;
//                     arc_rect.bottom=disp_y+1;
//                     arc_rect.left=disp_x-1;
//                     arc_rect.right=disp_x+1;
                     dlp_draw_box(disp_y-1,disp_x-1,disp_y+1,disp_x+1,colour);
//                     PaintRect(&arc_rect);

//                     MoveTo(disp_x,disp_y);
///                     LineTo(disp_x,disp_y);


            }

////display specials
                     specials_colour+=0.4;
                     structures_colour+=0.2;
                     if (specials_colour>255) specials_colour=0;
                     if (structures_colour>255) structures_colour=0;

    for (mship_index=0;mship_index<NUMBER_OF_SPECIALS_PER_QUAD;mship_index++)

            {
            int colour;
                     mship_x=GM_ptr->the_quadrant[quad].quadrant_specials[mship_index].real_coords.x;
                     mship_y=GM_ptr->the_quadrant[quad].quadrant_specials[mship_index].real_coords.y;
                     if (quad==1 || quad==3) mship_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) mship_y-=GALAXY_WIDTH/2;	//normalise y
                     mship_y=GALAXY_WIDTH/2-mship_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=static_cast<int>(mship_x/disp_x_div+10);
                     disp_y=static_cast<int>(mship_y/disp_y_div+10);
//                     colour=175;
                     if (GM_ptr->the_quadrant[quad].quadrant_specials[mship_index].code==CORRAL_FACING_RIGHT)
                     {
                      draw_line(disp_x-1,disp_y+1,disp_x+1,disp_y+1,static_cast<int>(structures_colour));
                      draw_line(disp_x-2,disp_y+2,disp_x+2,disp_y+2,static_cast<int>(structures_colour));
                      draw_line(disp_x-3,disp_y+3,disp_x+3,disp_y+3,static_cast<int>(structures_colour));

                     }
                     else
                     {
                      draw_line(disp_x,disp_y,disp_x+3,disp_y,static_cast<int>(specials_colour));
                      draw_line(disp_x,disp_y-2,disp_x,disp_y,static_cast<int>(specials_colour));
                      draw_line(disp_x+2,disp_y-2,disp_x+2,disp_y,static_cast<int>(specials_colour));
                     }
//
//                     dlp_draw_box(disp_y,disp_x-1,disp_y,disp_x+1,specials_colour);
//                     dlp_draw_box(disp_y,disp_x-1,disp_y,disp_x+1,specials_colour);

//                     if (specials_colour>224) specials_colour=215;
//                     PaintRect(&arc_rect);

//                     MoveTo(disp_x,disp_y);
///                     LineTo(disp_x,disp_y);


            }



//draw box around selected item
                     ZRGBForeColor(&rgbWhite);	//not

    if( selected_item_type!=-1)
     {
     //draw line from Zex to selected item
                     ZRGBForeColor(&rgbGreen);	//not
     ZMoveTo(zex_display_x,zex_display_y);
     ZLineTo(selected_item_screen_x,selected_item_screen_y);

                     ZRGBForeColor(&rgbWhite);	//not
      if(selected_item_type==PLANET)
       {
                     arc_rect.top=selected_item_screen_y-3;
                     arc_rect.bottom=selected_item_screen_y+3;
                     arc_rect.left=selected_item_screen_x-3;
                     arc_rect.right=selected_item_screen_x+3;
                     FrameRect(&arc_rect);

     //show info in top left of screen
     ShowItemInfo(quad, selected_item_type, selected_item_index, selected_item_screen_x,selected_item_screen_y);


       }
       else
       if(selected_item_type==MOTHERSHIP)
       {
                     arc_rect.top=selected_item_screen_y-3;
                     arc_rect.bottom=selected_item_screen_y+3;
                     arc_rect.left=selected_item_screen_x-3;
                     arc_rect.right=selected_item_screen_x+3;
                     FrameRect(&arc_rect);

     //show info in top left of screen
     ShowItemInfo(quad, selected_item_type, selected_item_index, selected_item_screen_x,selected_item_screen_y);

       }
       else
       if(selected_item_type==WARPHOLE)
       {
                     arc_rect.top=selected_item_screen_y-2;
                     arc_rect.bottom=selected_item_screen_y+4;
                     arc_rect.left=selected_item_screen_x-2;
                     arc_rect.right=selected_item_screen_x+4;
                     FrameRect(&arc_rect);

     //show info in top left of screen
     ShowItemInfo(quad, selected_item_type, selected_item_index, selected_item_screen_x,selected_item_screen_y);

       }
       else
       if(selected_item_type==SPECIAL)
       {
                     arc_rect.top=selected_item_screen_y-2;
                     arc_rect.bottom=selected_item_screen_y+4;
                     arc_rect.left=selected_item_screen_x-2;
                     arc_rect.right=selected_item_screen_x+4;
                     FrameRect(&arc_rect);

     //show info in top left of screen
     ShowItemInfo(quad, selected_item_type, selected_item_index, selected_item_screen_x,selected_item_screen_y);

       }

     }

    print_crunched_trans_pict(map_cursor_picture,mouse_x,mouse_y);

	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	 //oldmac//SetPort(savePort);




}

//typedef {
//int which_index;
//int what_type;
//}item_under_mouse;

//fills in which_index and what_type. Sets what_type=-1 if nothing there
void find_object_under_mouse(int mouse_x, int mouse_y, item_under_mouse* fill_me_baby)
{
int mgal_x,mgal_y;
int disp_x_mult,disp_y_mult;
int ship_x,ship_y;
int quad,quad_x,quad_y;
int object_x,object_y,object_index;
int disp_x,disp_y;
extern int current_quad;

	fill_me_baby->which_index=-1;
	fill_me_baby->what_type=-1;	//1 for planets

//translate x/y to galactic coords
//calc quad startx/y
		quad=current_quad;
      if (quad==0)
          {
          quad_x=GALAXY_START_X;
          quad_y=GALAXY_START_Y;
          }
      else
      if (quad==1)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=GALAXY_START_Y;
          }
      else
       if (quad==2)
          {
          quad_x=GALAXY_START_X;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }
      else
       if (quad==3)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }
//now calc multipliers
    disp_y_mult=(GALAXY_WIDTH/2)/460;
    disp_x_mult=(GALAXY_WIDTH/2)/620;
//now calc mouse galactic coords

    mgal_x=mouse_x+8;
    mgal_y=mouse_y+8;

//    mgal_x=mouse_x*disp_x_mult+quad_x;
//    mouse_y=480-mouse_y;	  //flip my
//    mgal_y=quad_y-(mouse_y*disp_y_mult);
//taadaah!
//now search...
//first planets


    for (object_index=0;object_index<NUMBER_OF_PLANETS_PER_QUAD;object_index++)
            {


              object_x=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_planets[object_index].real_coords.x);
              object_y=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_planets[object_index].real_coords.y);
              //translate to screen coords
                     if (quad==1 || quad==3) object_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) object_y-=GALAXY_WIDTH/2;	//normalise y
                     object_y=GALAXY_WIDTH/2-object_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=object_x/disp_x_mult+10;
                     disp_y=object_y/disp_y_mult+10;

            if (mgal_x>disp_x-4 && mgal_x<disp_x+4 &&
                mgal_y>disp_y-4 && mgal_y<disp_y+4)
                  {
                   fill_me_baby->which_index=object_index;
                   fill_me_baby->what_type=PLANET;	//1 for planets
                   fill_me_baby->screen_x=disp_x;
                   fill_me_baby->screen_y=disp_y;

                  }
            }
//motherships
    for (object_index=0;object_index<NUMBER_OF_MOTHERSHIPS_PER_QUAD;object_index++)
            {


              object_x=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_motherships[object_index].real_coords.x);
              object_y=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_motherships[object_index].real_coords.y);
              //translate to screen coords
                     if (quad==1 || quad==3) object_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) object_y-=GALAXY_WIDTH/2;	//normalise y
                     object_y=GALAXY_WIDTH/2-object_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=object_x/disp_x_mult+10;
                     disp_y=object_y/disp_y_mult+10;

            if (mgal_x>disp_x-4 && mgal_x<disp_x+4 &&
                mgal_y>disp_y-4 && mgal_y<disp_y+4)
                  {
                   fill_me_baby->which_index=object_index;
                   fill_me_baby->what_type=MOTHERSHIP;	//1 for planets
                   fill_me_baby->screen_x=disp_x;
                   fill_me_baby->screen_y=disp_y;

                  }
            }

//specials
    for (object_index=0;object_index<NUMBER_OF_SPECIALS_PER_QUAD;object_index++)
            {


              object_x=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_specials[object_index].real_coords.x);
              object_y=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_specials[object_index].real_coords.y);
              //translate to screen coords
                     if (quad==1 || quad==3) object_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) object_y-=GALAXY_WIDTH/2;	//normalise y
                     object_y=GALAXY_WIDTH/2-object_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=object_x/disp_x_mult+10;
                     disp_y=object_y/disp_y_mult+10;

            if (mgal_x>disp_x-4 && mgal_x<disp_x+4 &&
                mgal_y>disp_y-4 && mgal_y<disp_y+4)
                  {
                   fill_me_baby->which_index=object_index;
                   fill_me_baby->what_type=SPECIAL;	//1 for planets
                   fill_me_baby->screen_x=disp_x;
                   fill_me_baby->screen_y=disp_y;

                  }
            }
//scenary
 if (0)
 {
    for (object_index=0;object_index<NUMBER_OF_SCENARY_PER_QUAD;object_index++)
            {


              object_x=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_scenary[object_index].real_coords.x);
              object_y=static_cast<int>(GM_ptr->the_quadrant[quad].quadrant_scenary[object_index].real_coords.y);
              //translate to screen coords
                     if (quad==1 || quad==3) object_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) object_y-=GALAXY_WIDTH/2;	//normalise y
                     object_y=GALAXY_WIDTH/2-object_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=object_x/disp_x_mult+10;
                     disp_y=object_y/disp_y_mult+10;

            if (mgal_x>disp_x-4 && mgal_x<disp_x+4 &&
                mgal_y>disp_y-4 && mgal_y<disp_y+4)
                  {
                   fill_me_baby->which_index=object_index;
                   fill_me_baby->what_type=GALAXY_SCENARY;	//1 for planets
                   fill_me_baby->screen_x=disp_x;
                   fill_me_baby->screen_y=disp_y;

                  }
            }
 }

//warphole


              object_x=static_cast<int>(GM_ptr->the_quadrant[quad].the_warphole.real_coords.x);
              object_y=static_cast<int>(GM_ptr->the_quadrant[quad].the_warphole.real_coords.y);
              //translate to screen coords
                     if (quad==1 || quad==3) object_x-=GALAXY_WIDTH/2;	//normalise x
                     if (quad==0 || quad==1) object_y-=GALAXY_WIDTH/2;	//normalise y
                     object_y=GALAXY_WIDTH/2-object_y;	//normalise y some more :)
                     //now x is 0=left, 20e6 right
                     //y =0=top, 20e6=bottom
                     disp_x=object_x/disp_x_mult+10;
                     disp_y=object_y/disp_y_mult+10;

            if (mgal_x>disp_x-4 && mgal_x<disp_x+4 &&
                mgal_y>disp_y-4 && mgal_y<disp_y+4)
                  {
                   fill_me_baby->which_index=GM_ptr->the_quadrant[quad].the_warphole.toll;
                   fill_me_baby->what_type=WARPHOLE;	//1 for planets
                   fill_me_baby->screen_x=disp_x;
                   fill_me_baby->screen_y=disp_y;

                  }



}


void ShowItemInfo(int quad, int item_type, int item_index, int x, int y)
{
unsigned char name [255];
unsigned char number_string[12];
int supply_planet_index;
float item_x,item_y;
int distance,ship_x,ship_y;
Str63 cs_string;
extern int global_show_target_vector;

global_show_target_vector=1;

//OGL_set_pen_colour_from_index(1);	//white
ZRGBForeColor(&rgbWhite);
//calc correct x and y
if (x<120) x=x+20;	//to the left
else x=x-120;	//to the right


if (y<30) y=30;

if (y>350) y=350;
#define XOFF 60


	ship_x=static_cast<int>((*ocb_ptr).object_list[0].Oworldx);
	ship_y=static_cast<int>((*ocb_ptr).object_list[0].Oworldy);

if (item_type==PLANET)
  {
     item_x=GM_ptr->the_quadrant[quad].quadrant_planets[item_index].real_coords.x;
     item_y=GM_ptr->the_quadrant[quad].quadrant_planets[item_index].real_coords.y;
     ZMoveTo(x,y);
     ZDrawString_c ("Planet Name: ");
     ZMoveTo(x+XOFF,y);
     ZDrawString(GM_ptr->the_quadrant[quad].quadrant_planets[item_index].name);

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Inhabited: ");
     ZMoveTo(x+XOFF,y);
     if (GM_ptr->the_quadrant[quad].quadrant_planets[item_index].inhabited==1)
     ZDrawString_c ("Yes");
     else
     ZDrawString_c ("No");

     if (GM_ptr->the_quadrant[quad].quadrant_planets[item_index].inhabited==1)
     {
     int gnp_rating;

      y+=14;
      ZMoveTo(x,y);
      ZDrawString_c ("Type:");
      ZMoveTo(x+XOFF,y);
      if (GM_ptr->the_quadrant[quad].quadrant_planets[item_index].type<6)
      ZDrawString_c ("Agricultural");
      else
      ZDrawString_c ("Technological");
//show rating
      y+=14;
      ZMoveTo(x,y);
      ZDrawString_c ("Class:");
      ZMoveTo(x+XOFF,y);
      gnp_rating=GM_ptr->the_quadrant[quad].quadrant_planets[item_index].type;
      if (gnp_rating>=6) gnp_rating-=5;
      ZexNumToString(gnp_rating, number_string);
      ZDrawString (number_string);

      y+=14;
      ZMoveTo(x,y);
      ZDrawString_c ("GNP:");
      ZMoveTo(x+XOFF,y);
      ZexNumToString(GM_ptr->the_quadrant[quad].quadrant_planets[item_index].production_quota, number_string);
      ZDrawString (number_string);
      ZDrawString_c (" Million Tonnes");
     }
//now show distance from Zex
     item_x=GM_ptr->the_quadrant[quad].quadrant_planets[item_index].real_coords.x;
     item_y=GM_ptr->the_quadrant[quad].quadrant_planets[item_index].real_coords.y;
     distance=get_distance_to_from_point(ship_x,ship_y,static_cast<int>(item_x),static_cast<int>(item_y));

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Distance: ");
     ZMoveTo(x+XOFF,y);
      ZexNumToString(distance,number_string);
     ZDrawString (number_string);


  }
else
if (item_type==MOTHERSHIP)
  {
     item_x=GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].real_coords.x;
     item_y=GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].real_coords.y;

     ZMoveTo(x,y);
     if(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].visited!=0)
     ZDrawString_c ("DATA (Known) ");
     else
     ZDrawString_c ("DATA (Estimated) ");

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Mothership: ");
     ZMoveTo(x+XOFF,y);
     if(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].allegiance==ALIEN)
     ZDrawString_c ("R'mparian, ");
     else
     ZDrawString_c ("Earth, ");
     //callsign
     cs_string[0]=4;
	 safe_number_id_to_4_character_string(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].callsign, &cs_string[1]);
     ZDrawString (cs_string);




     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Power Rating: ");
     ZMoveTo(x+XOFF,y);
     if(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].visited!=0)
     {
      ZexNumToString(static_cast<long int>(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].power_rating),
                  number_string);
      ZDrawString (number_string);
     }
     else
      ZDrawString_c("Can't Estimate");
     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Prod. Rating: ");
     ZMoveTo(x+XOFF,y);
      ZexNumToString(static_cast<long int>(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].production_rating),
                  number_string);
      ZDrawString (number_string);

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Supply Rating: ");
     ZMoveTo(x+XOFF,y);
      ZexNumToString(static_cast<long int>(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].supply_rating),
                  number_string);
      ZDrawString (number_string);

     y+=14;	//name of planet suppplied from
     ZMoveTo(x,y);
     ZDrawString_c ("Supply Route: ");
     ZMoveTo(x+XOFF,y);
     if(GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].supply_rating<0)
      ZDrawString_c("None");
     else
     {
      supply_planet_index=GM_ptr->the_quadrant[quad].quadrant_motherships[item_index].supply_planet_index;
      ZDrawString(GM_ptr->the_quadrant[quad].quadrant_planets[supply_planet_index].name);
     }
//now show distance from Zex
   distance=get_distance_to_from_point(ship_x,ship_y,static_cast<int>(item_x),static_cast<int>(item_y));

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Distance: ");
     ZMoveTo(x+XOFF,y);
      ZexNumToString(distance,number_string);
     ZDrawString (number_string);

  }
else
if (item_type==WARPHOLE)
  {
     ZMoveTo(x,y);
     if (quad==0)
     ZDrawString_c ("Warphole to Beta Quad");
     else
     if (quad==1)
     ZDrawString_c ("Warphole to Gamma Quad");

     if (quad==2)
     ZDrawString_c ("Warphole to Delta Quad");

     if (quad==3)
     ZDrawString_c ("Warphole to Alpha Quad");

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Toll: ");
     ZMoveTo(x+XOFF,y);
      ZexNumToString(item_index,number_string);
     ZDrawString (number_string);
//now show distance from Zex
     item_x=GM_ptr->the_quadrant[quad].the_warphole.real_coords.x;
     item_y=GM_ptr->the_quadrant[quad].the_warphole.real_coords.y;
     distance=get_distance_to_from_point(ship_x,ship_y,static_cast<int>(item_x),static_cast<int>(item_y));

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Distance: ");
     ZMoveTo(x+XOFF,y);
      ZexNumToString(distance,number_string);
     ZDrawString (number_string);

  }

else
if (item_type==SPECIAL)
  {
     ZMoveTo(x,y);
     if (GM_ptr->the_quadrant[quad].quadrant_specials[item_index].code==CORRAL_FACING_RIGHT)
     ZDrawString_c ("Supply Depot");
     else
     ZDrawString_c ("Unknown Item(s)");
//now show distance from Zex
     item_x=GM_ptr->the_quadrant[quad].quadrant_specials[item_index].real_coords.x;
     item_y=GM_ptr->the_quadrant[quad].quadrant_specials[item_index].real_coords.y;
     distance=get_distance_to_from_point(ship_x,ship_y,static_cast<int>(item_x),static_cast<int>(item_y));

     y+=14;
     ZMoveTo(x,y);
     ZDrawString_c ("Distance: ");
     ZMoveTo(x+XOFF,y);
      ZexNumToString(distance,number_string);
     ZDrawString (number_string);

  }

}


void get_galactic_stats(galactic_stats* the_stats)
{
int section_x,section_y,mship_counter;
int mship_index;
int quadrant;
int planet;
int section_start_x,section_start_y;
int quad_x,quad_y;
int random_multiplier,n;
int return_val=0;

the_stats->number_of_earth_mships=0;
the_stats->number_of_alien_mships=0;

the_stats->av_earth_supply_rating=0;
the_stats->av_alien_supply_rating=0;

the_stats->total_earth_supply_rating=0;
the_stats->total_alien_supply_rating=0;

the_stats->av_earth_power_rating=0;
the_stats->av_alien_power_rating=0;

the_stats->total_earth_power_rating=0;
the_stats->total_alien_power_rating=0;

for (quadrant=0;quadrant<4;quadrant++)
      {
      //calc quad start x/y
      mship_index=0;

      if (quadrant==0)
          {
          quad_x=GALAXY_START_X;
          quad_y=GALAXY_START_Y;
          }
      else
      if (quadrant==1)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=GALAXY_START_Y;
          }
      else
       if (quadrant==2)
          {
          quad_x=GALAXY_START_X;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }
      else
       if (quadrant==3)
          {
          quad_x=NUMBER_OF_SECTIONS_IN_A_QUAD_X*SECTION_SIZE;
          quad_y=NUMBER_OF_SECTIONS_IN_A_QUAD_Y*SECTION_SIZE;
          }


       for (section_y=0;section_y<NUMBER_OF_SECTIONS_IN_A_QUAD_Y;section_y++)
         {
            //calc section start coords
           section_start_y=quad_y - section_y*SECTION_SIZE;

           for (section_x=0;section_x<NUMBER_OF_SECTIONS_IN_A_QUAD_X;section_x++)
             {
                //calc section start coords
                section_start_x=section_x*SECTION_SIZE+quad_x;

                for (mship_counter=0;mship_counter<NUMBER_OF_MOTHERSHIPS_IN_A_SECTION;mship_counter++)
                  {

                    if (GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance==HUMAN)
                    {
                    the_stats->number_of_earth_mships++;
                    the_stats->total_earth_supply_rating=static_cast<int>(the_stats->total_earth_supply_rating+GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].supply_rating);
                    the_stats->total_earth_power_rating=static_cast<int>(the_stats->total_earth_power_rating+GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].power_rating);
                    }
                    else
                    if (GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance==ALIEN)
                    {
                    the_stats->number_of_alien_mships++;
                    the_stats->total_alien_supply_rating=static_cast<int>(the_stats->total_alien_supply_rating+GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].supply_rating);
                    the_stats->total_alien_power_rating=static_cast<int>(the_stats->total_alien_power_rating+GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].power_rating);
                    }

                    mship_index++;
                  }	//END OF mship_counter
             }	//end of section_x
         }	//end of section_y
      }	//end of quadrant

the_stats->av_earth_supply_rating=the_stats->total_earth_supply_rating/the_stats->number_of_earth_mships;
the_stats->av_alien_supply_rating=the_stats->total_alien_supply_rating/the_stats->number_of_alien_mships;

the_stats->av_earth_power_rating=the_stats->total_earth_power_rating/the_stats->number_of_earth_mships;
the_stats->av_alien_power_rating=the_stats->total_alien_power_rating/the_stats->number_of_alien_mships;

}


//these routines used to gen galactic_id's
static int unique_number;

void reset_unique_number()
{
unique_number=1;
}

int get_unique_number()
{
return unique_number++;
}


//galaxy set up as
//quad 0     |  quad 1
//           |
//---------------------
//           |
//quad 2     |  quad 3

void set_all_motherships_to_alegiance(int allegiance, int quadrant)
{
int section_x,section_y,mship_counter;
int mship_index;
int quad_x,quad_y;
int n;

//Set all motherships in all miniblocks of this block to allegiance

      mship_index=0;

       for (section_y=0;section_y<NUMBER_OF_SECTIONS_IN_A_QUAD_Y;section_y++)
         {

           for (section_x=0;section_x<NUMBER_OF_SECTIONS_IN_A_QUAD_X;section_x++)
             {

                for (mship_counter=0;mship_counter<NUMBER_OF_MOTHERSHIPS_IN_A_SECTION;mship_counter++)
                  {

                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance=allegiance;

                    //callsign
                    GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].callsign=
                    make_callsign(GM_ptr->the_quadrant[quadrant].quadrant_motherships[mship_index].allegiance);

                    mship_index++;
                  }	//END OF mship_counter
             }	//end of section_x
         }	//end of section_y


}


