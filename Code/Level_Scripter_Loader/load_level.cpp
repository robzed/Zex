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

//load level
//SB 18/12/98
//See 3D engine design for notes

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <stdlib.h>		// for atof()
#include <string.h>
//oldmac//#include <files.h>
//oldmac//#include <fp.h>
//oldmac//#include <resources.h>

#include "sound.h"
#include "3d_structures_def.h"
#include "Error_handler.h"
#include "MeshToZex.h"
#include "load_objects.h"
#include "load_level.h"
#include "file_io.h"
#include "objects_controls.h"
#include "compile_zmap.h"
#include "Z_init.h"
#include	"splat_control.h"
#include	"txtsplat.h"
#include "ship_control.h"
#include "input_control.h"
#include "zex_misc.h"
#include "refuel.h"
#include "level_control.h"
#include "pictures.h"
#include "motherships.h"

#include <limits>

#include "save_path_destination.h"

using namespace std;

int level_text_read_index;
char level_text[2048];	//level description goes here
int level_start_time_ticks,current_level;


extern int reset_brightness;
extern float targ_x,targ_y,dest_x,dest_y;	//the target of the level

void load_level (int the_level)
{
extern FSSpec Zex_FSSpec;
extern int difficulty;
extern int	static_object_index;	//how many static objects
//these following 5 are in level_control.c
extern int level_complete,can_do_next_level;
extern int music_volume;
int tens;
FSSpec the_file;
Handle the_file_data_H;
Ptr the_level_file;
char filename[]="xx.ZLV";

//oldmac//show_text_in_loading_screen("Zex: Load level");
     current_level=the_level;
     
    level_start_time_ticks=LS_TickCount60();	//make a note of the start time of the level

//set difficulty to default 1 in case user forgets
reset_brightness=25000;	//default daytime mission
level_complete=0;
can_do_next_level=0;
static_object_index=0;
reset_static_slots();	//clear out old static objects
reset_objects();	//clear out old dynamic objects
init_refuel_rects();	//reset refuelling bits

//convert level to 2 char number string
    tens=the_level/10;
    filename[0]=tens+'0';
    filename[1]=(the_level-(tens*10))+'0';
    make_data_fsspec("L",filename,&the_file);
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,filename,&the_file);
    run_level_music(&the_file);	//either madh128 from level or 130 from zex
//    music_set_vol(music_volume);

	the_file_data_H=read_file(the_file);
//	GetBGRAW(&the_file);	//see if theres a BGIM 128 resource and load it if so
    LS_HLock(the_file_data_H);
    the_level_file=*the_file_data_H;
    interpret_file(the_level_file);
	LS_DisposeHandle(the_file_data_H);
	load_motherships_into_nav();	//mothership vectors for radar display (200299)
}


int have_bpic;
Handle bpic;
LSRAW *bpic_ptr;

#if 0 //oldmac//  not used any more
void GetBGRAW(FSSpec* the_file)
{
extern Handle bpic;
short fid;

 have_bpic=0;
 return;
 
//remove old
 if (have_bpic==1) LS_DisposeHandle(bpic);
 have_bpic=0;
 bpic=0;
//open res file
 fid=FSpOpenResFile(the_file,fsRdPerm);
 
 bpic = (Handle) Get1Resource('BGIM',128);  //Get the Handle to the Resource 
 if (bpic==0 || ResError()!=0) have_bpic=0;
 else
 {
  have_bpic=1;
  LS_DetachResource (bpic);
  LS_HLock ((Handle) bpic);
  bpic_ptr = (LSRAW*)*bpic;
 }
//close res file
 CloseResFile(fid);
}
#endif

//oldmac//decimal my_dec;

void interpret_file(Ptr the_level)
{
extern	  DynObjectsFixedSize *ocb_ptr;
extern FSSpec Zex_FSSpec;
extern _3D camera_pos;
extern int monsters_can_fire;
extern int score_this_level,credits_this_level,time_this_level,monsters_killed_this_level;

Handle the_map_data_H;
FSSpec the_file;
char the_line[512];	//max line size in a mesh file
char the_word[512];	//max length of a word from a line
char *source;		//walk through the file with this
int line_len=0, line_pos=0, word_len=0,stop;		//loop control
//oldmac//short ix;
//oldmac//short vp;
float dectof_result;	//for the OS routine numtostring
unsigned long command;
load_dyn_object_pb the_params;
anim_pb anim;
int fname_length,i,line_number=1;
char filename[255];
int static_slot,frames_counter;
unsigned long id;
int colldetect;
unsigned long texture_id;
int dest_ocb,temp_int;
float bg_x,bg_y;
//float delta_y;
int frame_rate,anim_state,anim_control;
char character;
int line_index;
Rect fuel;
extern int score,credits,accumulated_game_time;
//copy score etc to this_level so we calc items this level
    score_this_level=score,
    credits_this_level=credits;
    time_this_level=accumulated_game_time;
    monsters_killed_this_level=0;
    reset_watchdog();	    //have to keep reseting this else we'll error

//stuff we always have to do - load in weapons 'n' things to our reserved slots (<20)
    load_object ("zex_game.z",zex,'ZEXS',1,'T009',IS_NOT_TRANSLUCENT);
    
    load_object ("star2.z",dust,'DST2',no_colldet,0,IS_NOT_TRANSLUCENT);	//earth dust
    load_object ("star3.z",dust2,'DST1',no_colldet,0,IS_NOT_TRANSLUCENT);	//rmparian dust
    load_object ("cannon1.z",player_bullet_not_so_weeny,'MIS2',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("cannon0.z",player_bullet_weeny,'MIS1',1,0,IS_NOT_TRANSLUCENT);
    load_object ("laser1.z",ship_laser1,'LAS1',no_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("ejectcap.z",ejection_cap,'EC01',yes_colldet,'TB00',IS_NOT_TRANSLUCENT);
    load_object ("SE_object.z",explosion,'EX01',no_colldet,0,IS_NOT_TRANSLUCENT);	//our exploding ball

    load_object ("miss1a.z",missile1,'ZZ1A',yes_colldet,0,IS_NOT_TRANSLUCENT);	//our first missile
    load_object ("miss1b.z",missile1b,'ZZ1B',yes_colldet,0,IS_NOT_TRANSLUCENT);	//which is an animated object
    load_object ("miss1c.z",missile1c,'ZZ1C',yes_colldet,0,IS_NOT_TRANSLUCENT);	//over three frames
    load_object ("lshield.z",large_shield,'SHL1',no_colldet,0,IS_TRANSLUCENT);	//
    load_object ("zshield.z",zex_shield,'SHLZ',no_colldet,0,IS_TRANSLUCENT);	      //16
	
    load_object ("carg1.z",17,'CAR1',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("carg2.z",18,'CAR2',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("carg3.z",19,'CAR3',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("carg4.z",20,'CAR4',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("miss2a.z",21,'ZZ2A',yes_colldet,0,IS_NOT_TRANSLUCENT);	//our first missile
    load_object ("miss2b.z",22,'ZZ2B',yes_colldet,0,IS_NOT_TRANSLUCENT);	//which is an animated object
    load_object ("miss2c.z",23,'ZZ2C',yes_colldet,0,IS_NOT_TRANSLUCENT);	//over three frames
    load_object ("miss3a.z",24,'ZZ3A',yes_colldet,0,IS_NOT_TRANSLUCENT);	//our first missile
    load_object ("miss3b.z",25,'ZZ3B',yes_colldet,0,IS_NOT_TRANSLUCENT);	//which is an animated object
    load_object ("miss3c.z",26,'ZZ3C',yes_colldet,0,IS_NOT_TRANSLUCENT);	//over three frames

    load_object ("zex_docked.z",27,'ZEXD',no_colldet,'CP02',0);
    load_object ("DB01.z",28,'BGTE',no_colldet,'DBE1',0);	//earth texture
    load_object ("DB01.z",29,'BGTA',no_colldet,'DBA1',0);	//alien texture
    load_object ("star2OGL.z",30,'DST2',no_colldet,0,IS_NOT_TRANSLUCENT);	//earth dust

	load_object ("shockwave.z",31 ,'SHWV',no_colldet,0,IS_NOT_TRANSLUCENT);	      //17


    reset_watchdog();	//have to keep reseting this else we'll error
       
//      goto fred;	 //debug!!!!!!!
//set up ship
	the_params.world_x=0;
	the_params.world_y=0;
	the_params.world_z=MONSTER_Z;
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=270;
	the_params.controller_ref=zex;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.current_velocity=0;
	the_params.control_use_7=0;
	the_params.wanted_velocity=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
     the_params.has_roll=0;
	the_params.mass=100;
		 	 the_params.dq=0;
	the_params.object_category=zex_object;
	 the_params.laser_cat=1;
	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=3;

	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(zex,0,&the_params,-1);	   //the object, the position (-1=next free)

fred: level_text_read_index=0;

//now process level file
source=the_level;	//source is incremented through the file

stop=0;	//loop term

while (stop==0)
{
 line_len=get_line(source,the_line);
 if (line_len!=-1)
    {
     word_len+=get_word(the_word,&the_line[word_len]);
     if (word_len!=-1) //-1=cr
     {
      command=(the_word[0]<<24) + (the_word[1]<<16) +
      (the_word[2]<<8) + the_word[3];
//ееееееееееееcommand processing, all cut 'n' pasted - no apologies.
      switch (command)
      {
       case '****': break;
       
//       case 'DIFF':	  //difficulty level
//          {
//            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
//            ix=0;
//            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
//            dectof_result=dec2f (&my_dec);	  //number of objects in a column
//            difficulty=dectof_result;
//            break;
//          }
 
       case 'LITE':	//brightness
          {
            reset_brightness=30000;
            break;
          }

       case 'NITE':	//brightness
          {
            reset_brightness=65500;
            break;
          }
         
       case 'FUEL':	//a refuel rectangle
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: FUEL: Not enough parameters.","Expected top member of rect",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.top= static_cast<short int>(dectof_result);

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: FUEL: Not enough parameters.","Expecting left member of rect",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.left= static_cast<short int>(dectof_result);

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: FUEL: Not enough parameters.","Expecting bottom member of rect",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.bottom= static_cast<short int>(dectof_result);

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: FUEL: Not enough parameters.","Expecting right member of rect",line_number);
 			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.right= static_cast<short int>(dectof_result);
            add_refuel_rect(fuel);
            break;
          }

       case 'LTXT': //level descriptor text
       character=13;
       line_index=0;
       while (character!=0 && level_text_read_index<1022)
       {
       character=the_line[word_len+line_index];	//get char from line
       if (character!=0)
       level_text[level_text_read_index]=character;
       else
       level_text[level_text_read_index]=13;
       line_index++;
       level_text_read_index++;
       }
            break;

       case 'SCRD':	//ship start coords
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
            (*ocb_ptr).object_list[zex].Oworldx=dectof_result;
            camera_pos.x=dectof_result;	//set camera too

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
 			dectof_result = atof(the_word);
            (*ocb_ptr).object_list[zex].Oworldy=dectof_result;
            camera_pos.y=dectof_result;	//set camera too
            break;
          }

       case 'MONF':	//monsters can fire - 1 they do, 0 they dont
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
            (*ocb_ptr).object_list[zex].Oworldx=dectof_result;
            monsters_can_fire= static_cast<int>(dectof_result);	//set camera too
             break;
          }

       case 'TARG':	//target of level
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
            targ_x=dectof_result;	//set camera too
			
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
            targ_y=dectof_result;	//set camera too
            break;
          }

       case 'DEST':	//destination once level is complete for nav
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
            dest_x=dectof_result;	//set camera too
			
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
            dest_y=dectof_result;	//set camera too
            break;
          }


       case 'RFIL':	//load an object
          {
            //get iflename as pascal string
            fname_length=get_word(the_word,&the_line[word_len]);
            word_len+=fname_length;
            fname_length--;	//account for param separator
            for (i=0;i<fname_length;i++)
             {
             filename[i]=the_word[i];
             }
             filename[i]=0;
              
            //get ID
            word_len+=get_word(the_word,&the_line[word_len]);
            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
            //get static_object slot
            word_len+=get_word(the_word,&the_line[word_len]);
			dectof_result = atof(the_word);
            static_slot= static_cast<int>(dectof_result);
			
            //get colldet
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
            colldetect= static_cast<int>(dectof_result);	//0-3
			
            //get texture ID
            the_word[0]=0;the_word[1]=0;the_word[2]=0;the_word[3]=0;
            fname_length=get_word(the_word,&the_line[word_len]);
            word_len+=fname_length;

            texture_id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
            if (the_word[0]==0 || the_word[1]==0 || the_word[2]==0 || the_word[3]==0 || the_word[4]!=0) 
            report_error_c ("load_level: RFIL: Bad texture ID in object.","(Must be 4 chars or 0000). Line number below.",line_number);
            if (texture_id=='0000') texture_id=0;
            if (static_slot<40) report_error_c("load_level: RFIL: Can't read object into slots below 40 (Reserved slots)","",100);
            load_object(filename,static_slot,id,colldetect,texture_id,IS_NOT_TRANSLUCENT);
            break;
          }
       case 'LMON':	//insert object to dyn ocbs
          {
          //read object id
            word_len+=get_word(the_word,&the_line[word_len]);
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting Object ID code",line_number);

            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
          //find id
             static_slot=get_static_object_slot(id);	//go fetch fido (the object)
             if (static_slot==-1) report_error_id ("load_level: Can't find object. ID is: ",id);
          //get world x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting start pos x",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_x=dectof_result;

          //get world y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting start pos y",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_y=dectof_result;
         
          //get world z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting start pos z",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_z=dectof_result;

          //get movement delta x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting delta x",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedx=dectof_result;

          //get movement delta y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting delta y",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedy=dectof_result;
			
          //get movement delta z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting delta z",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedz=dectof_result;

          //get delta rot x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting delta rot x",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_x= static_cast<int>(dectof_result);

          //get delta rot y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting delta rot y",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_y= static_cast<int>(dectof_result);

          //get delta rot z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting delta rot z",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_z= static_cast<int>(dectof_result);

          //get control_use_1
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting Control Use 1 Value",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_1= static_cast<int>(dectof_result);


          //get control_use_2
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting Control Use 2 Value",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_2= static_cast<int>(dectof_result);

          //get control_use_3
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting Control Use 3 value",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_3= static_cast<int>(dectof_result);

          //get controller ref
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting Controller reference number",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.controller_ref= static_cast<int>(dectof_result);
          
          //get shields
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting shield value",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.shield_value= static_cast<short int>(dectof_result);

          //get mass
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting Mass",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.mass= static_cast<short int>(dectof_result);
            if (dectof_result<0) report_error_c("load_level: LMON: Negative mass not allowed.","Line number below.",line_number);

          //get category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting object category",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.object_category=temp_int;

          //get trivial rejection
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting No trivial rejection",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.no_trivial_rejection=temp_int;

          //get number of missiles
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting number of missiles",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.number_of_missiles=temp_int;
          //get laser category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting laser category",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.laser_cat=temp_int;

          //get has roll
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting has roll",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.has_roll=temp_int;

          //get unique ID
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting unique ID",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	      //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.unique_id=temp_int;

          //get zrot
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting zrot",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	      //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.rot_z=temp_int;



	      the_params.rot_x=0;
	      the_params.rot_y=0;
	the_params.rot_z=0;
	      the_params.control_use_4=0;
	the_params.control_use_7=0;
	      the_params.current_velocity=0;
	      the_params.wanted_velocity=0;
	      	   	the_params.dq=0;
	      the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	      the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	      the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	      the_params.attack_mode =  0;	//attacking
	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	      
	      dest_ocb=find_vacant_dynamic_object_slot();	//get us a slot to put it in

      if (dest_ocb==-1) report_error_id ("load_level: Cant get dynamic slot for LMON",2);

     the_params.who_spawned_this_object=-1;
 	  load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//load it
          
           break;
          }  


       case 'LAMN':	//insert animated object to dyn ocbs
          {
			  frames_counter=0;
			  anim.object_id[0] = numeric_limits<unsigned int>::max(); //-1;	//we allow up to 6 frames
			  anim.object_id[1] = numeric_limits<unsigned int>::max(); //-1;
			  anim.object_id[2] = numeric_limits<unsigned int>::max(); //-1;
			  anim.object_id[3] = numeric_limits<unsigned int>::max(); //-1;
			  anim.object_id[4] = numeric_limits<unsigned int>::max(); //-1;
			  anim.object_id[5] = numeric_limits<unsigned int>::max(); //-1;
																	 //read object ids - '0000'=null
           for (i=0;i<6;i++)
           {
            word_len+=get_word(the_word,&the_line[word_len]);
            if (the_word[0]==0) report_error_c("load_level: LMON: Not enough parameters.","Expecting Object ID code",line_number);

            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
             if (id!='0000') 
             {
              frames_counter++;
              anim.object_id[i]=id;
          //find id
              static_slot=get_static_object_slot(id);	//go fetch fido (the object)
              if (static_slot==-1) report_error_id ("load_level: LAMN: Can't find object. ID is: ",id);
             }
             
           }

          //get frame rate
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting anim frame rate",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        frame_rate= static_cast<int>(dectof_result);

          //get animation control
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting anim control",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        anim_control= static_cast<int>(dectof_result);

          //get animation state
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting anim state",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        anim_state= static_cast<int>(dectof_result);
          
          //get world x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting start pos x",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_x=dectof_result;

          //get world y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting start pos y",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_y=dectof_result;
         
          //get world z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting start pos z",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_z=dectof_result;

          //get movement delta x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting delta x",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedx=dectof_result;

          //get movement delta y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting delta y",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedy=dectof_result;
          //get movement delta z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting delta z",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedz=dectof_result;

          //get delta rot x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting delta rot x",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_x= static_cast<int>(dectof_result);

          //get delta rot y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting delta rot y",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_y= static_cast<int>(dectof_result);

          //get delta rot z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting delta rot z",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_z= static_cast<int>(dectof_result);

          //get control_use_1
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting Control Use 1 Value",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_1= static_cast<int>(dectof_result);


          //get control_use_2
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting Control Use 2 Value",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_2= static_cast<int>(dectof_result);

          //get control_use_3
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting Control Use 3 value",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_3= static_cast<int>(dectof_result);

          //get controller ref
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting Controller reference number",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.controller_ref= static_cast<int>(dectof_result);
          
          //get shields
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting shield value",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.shield_value= static_cast<short int>(dectof_result);

          //get mass
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting Mass",line_number);

			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.mass= static_cast<short int>(dectof_result);
            if (dectof_result<0) report_error_c("load_level: LAMN: Negative mass not allowed.","Line number below.",line_number);

          //get category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting object category",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.object_category=temp_int;

          //get trivial rejection
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting control if not visible",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.no_trivial_rejection=temp_int;

          //get number of missiles
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting number of missiles",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.number_of_missiles=temp_int;
          //get laser category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting laser category",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.laser_cat=temp_int;
          //get has roll
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: LAMN: Not enough parameters.","Expecting has roll",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.has_roll=temp_int;

//     the_params.has_roll=0;

	      the_params.rot_x=0;
	      the_params.rot_y=0;
	      the_params.rot_z=270;
	      the_params.control_use_4=0;
	the_params.control_use_7=0;
	      the_params.current_velocity=0;
	      the_params.wanted_velocity=0;
	      the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	      the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	      the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	      the_params.attack_mode =  0;	//attacking

	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	
//	dest_ocb=find_vacant_dynamic_object_slot();	//get us a slot to put it in


     the_params.who_spawned_this_object=-1;
 	  load_animated_object(&anim,frames_counter,frame_rate,anim_control,anim_state,&the_params,-1);	//load it
          
           break;
          }  


      case 'BMAP':	 //load bg map
          {
            //get iflename as pascal string
            get_word(the_word,&the_line[word_len]);
            //calc length as it may be last item on line and get_word will return -1 in this case
            fname_length=0;
            for(;;)
             {
             if (the_word[fname_length]==0) break;
             fname_length++;
             }
            word_len+=fname_length;
//            fname_length--;	//account for param separator
            for (i=0;i<fname_length;i++)
             {
             filename[i]=the_word[i];
             }
             filename[i]=0;
//get object position x
            word_len+=1;	//past comma
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: BMAP: Not enough parameters.","Expecting object x position. Line number below.",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            bg_x=dectof_result;


//get object position y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_level: BMAP: Not enough parameters.","Expecting object y position. Line number below.",line_number);
			dectof_result = atof(the_word);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            bg_y=dectof_result;


             //load it
             make_data_fsspec("L",filename,&the_file);
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,filename,&the_file);
	         the_map_data_H=read_file(the_file);
             compile_zmap(*the_map_data_H);	//file compile_zmap.c
	         init_cmap(bg_x,bg_y);	//load in the bg objects
	         LS_DisposeHandle(the_map_data_H);
            break;
          }          
       }	//end of switch
//ееееееееееееend command processing
      word_len=0;
      source+=line_len+1;	//point to next line (accounting for CR)
      line_number++;
     }
	}
	else stop=1;
 }
level_text[level_text_read_index]=0;
}

char good_luck[]="GOOD LUCK CAPTAIN.";
char be_lucky[]="BE LUCKY CAPTAIN.";
char watch_ass[]="REMEMBER - WATCH YOUR BUTT!";
char counting[]="WE ARE COUNTING ON YOU.";

//еееееееееееееееееееееееее
//display the LTXT text
void show_level_text()
{
extern int level_text_read_index;
extern char level_text[2048];	//level description goes here

extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int gLaser;
extern int gCannon;


LS_CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

int level_text_pos=0;
int text_line_index;
int curs_y=400;
char text_line[1024];
char the_char;
int temp,temp_y;
typedef struct {
int x,y,c,delta;
}star_object;

	do_fixed_frame_rate(FRAME_RATE_REINIT);

	star_object stars[612];	//array of bg stars
    gLaser=0;
	user_delay_no_cursor_update_required(10);	// 10 tick delay but don't let show_it() happen

//fill the star array
    init_zex_random();
    for (temp=0;temp<612;temp++)
    {
     Ptr pix;
    
     stars[temp].x=RangedRdm(10,639);
     stars[temp].y=RangedRdm(10,479);
     stars[temp].delta=RangedRdm(1,6);	//movement frame rate
     stars[temp].c=RangedRdm(245,252);
//     stars[temp].c=255-(5-stars[temp].delta);	//slower=darker=further away
    }
//this is a vertical scrolly
while (game_fire_button()==0)
 {
    level_text_pos=0;
    if (curs_y>100) curs_y-=2;	//stop at line 100
	the_drawing_buffer=Get_back_buffer();
//	  SetPort((GrafPtr)the_drawing_buffer);
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
//lets do some stars as teh background image
    for (temp=0;temp<612;temp++)
    {
     int x,y,c;
     Ptr pix;
    
     x=stars[temp].x;
     y=stars[temp].y;
     c=stars[temp].c;
     x+=stars[temp].delta;
      if (x>629) x=0;
      stars[temp].x=x;
     pix=(char*)screenaddr+(y*screenwidth)+x;	 //screenaddr
     *pix=c;

    }
//    init_zex_random();

    temp_y=curs_y;    
    
    while(level_text_pos<level_text_read_index)
    {

    text_line_index=0;
    the_char=0;
     while (the_char!=13 && text_line_index<1023)
      {
      the_char=level_text[level_text_pos];
      text_line[text_line_index]=the_char;
      level_text_pos++;
      text_line_index++;
      }
      text_line[text_line_index-1]=0;	//make c string
    
     if (temp_y<460) SplatText_Transparent_Fade (text_line,120,temp_y,247);
    temp_y+=12;
    
     }
	Show_it();
	do_fixed_frame_rate(FRAME_RATE_DELAY);	

 }	//end of while
 
 
//now scroll text down and off
  while(curs_y<460)	
 {
    level_text_pos=0;
    if (curs_y<462) curs_y+=6;	//stop at line 100
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
//lets do some stars as teh background image
    for (temp=0;temp<612;temp++)
    {
     int x,y,c;
     Ptr pix;
    
     x=stars[temp].x;
     y=stars[temp].y;
     c=stars[temp].c;
     x+=stars[temp].delta;
      if (x>629) x=0;
      stars[temp].x=x;
     pix=(char*)screenaddr+(y*screenwidth)+x;	 //screenaddr
     *pix=c;

    }
//    init_zex_random();

    temp_y=curs_y;    
    
    while(level_text_pos<level_text_read_index)
    {

    text_line_index=0;
    the_char=0;
     while (the_char!=13 && text_line_index<1023)
      {
      the_char=level_text[level_text_pos];
      text_line[text_line_index]=the_char;
      level_text_pos++;
      text_line_index++;
      }
      text_line[text_line_index-1]=0;	//make c string
    
     if (temp_y<460) SplatText_Transparent_Fade (text_line,120,temp_y,247);
    temp_y+=12;
    
     }
	Show_it();
	do_fixed_frame_rate(FRAME_RATE_DELAY);	

 }	//end of while
 

    gCannon=0; 
    gLaser=0;
}

//
// Append on pascal string onto another
//
// Notice: No error checking!!!
void append_c_to_pascal_string(unsigned char *target, const char *source)
{
	int len_source = strlen(source);
	int len_original_target = *target;
	*target = len_source + len_original_target;			// update the target to reflect the new greater length
	
	target += len_original_target;	// we are now just after the last character in the current target - i.e. where we need to start appending to.
	
	for(int i=0; i<len_source; i++)
	{
		*(++target) = *(source++);			// copy each character
	}
	
}

//
// Return the path where we save our stuff
//
std::string get_saved_file_path()
{
	static Save_Path our_save_path("ZD3/P/save_file_path.txt");		// will be constructed on first function call
	
	return our_save_path.get();
}

//looks for a file in ZD3/folder name/
void make_data_fsspec(const char* the_folder_name, const char* the_filename, FSSpec* the_fsspec)	//make fsspec of file name in zd3 folder
{
	CToPascal(the_filename, the_fsspec->name);
	// we instead of emulating all the previous calls construct the directory path into the fspsec name field

	if(the_folder_name[0] == 'P' && the_folder_name[1] == 0)
	{
		// we have a prefs folder call so we want to write to the save directory
		CToPascal(get_saved_file_path().c_str(), the_fsspec->fullpathname);
		append_c_to_pascal_string(the_fsspec->fullpathname, "/");
		append_c_to_pascal_string(the_fsspec->fullpathname, the_filename);		
	}
	else		// we have a 'non-prefs' folder call
	{
		CToPascal("ZD3", the_fsspec->fullpathname);
		append_c_to_pascal_string(the_fsspec->fullpathname, "/");
		append_c_to_pascal_string(the_fsspec->fullpathname, the_folder_name);
		append_c_to_pascal_string(the_fsspec->fullpathname, "/");
		append_c_to_pascal_string(the_fsspec->fullpathname, the_filename);
	}
	
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
extern FSSpec Zex_FSSpec;
FSSpec data_folder_FSSpec;
CInfoPBRec pb;
DirInfo *dpb=(DirInfo*) &pb;
HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
int folder_DirID;
int return_value;
short get_file_err;

//get the objects' folder
	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"ZD3",&data_folder_FSSpec);
//get zd3's dirctory ID
     dpb->ioVRefNum = data_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = data_folder_FSSpec.parID;
     dpb->ioNamePtr = data_folder_FSSpec.name;
     dpb->ioFDirIndex=0;	//query
     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
     report_error ("load_level couldn't read a file:",the_filename,return_value);
     return return_value;
     }
     folder_DirID=dpb->ioDrDirID;

return_value=0;

//find sub folder
      fpb->ioVRefNum = data_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = the_folder_name;
     fpb->ioFDirIndex=0;	//query the file name

     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
     report_error ("load_level couldn't find a folder:",the_folder_name,return_value);
     return return_value;
     }
     folder_DirID=dpb->ioDrDirID;

//now load it
//find file
      fpb->ioVRefNum = data_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = the_filename;
     fpb->ioFDirIndex=0;	//query the file name
     get_file_err=PBGetCatInfo(&pb,0);
     if (get_file_err) report_error ("make_data_fsspec couldn't read a file:",the_filename,get_file_err);
     if(get_file_err) return return_value;

     FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,the_filename,the_fsspec);
#endif
}

//int make_data_fsspec(Str255 the_folder_name,Str255 the_filename, FSSpec* the_fsspec)	  //make fsspec of file name in zd3 folder
//{
//extern FSSpec Zex_FSSpec;
//FSSpec data_folder_FSSpec;
//CInfoPBRec pb;
//DirInfo *dpb=(DirInfo*) &pb;
//HFileInfo *fpb=(HFileInfo*) &pb;	  //ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
//int folder_DirID;
//int return_value;
//short get_file_err;
//
////get the objects' folder
//	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"ZD3",&data_folder_FSSpec);
////get the folders dirctory ID
//     dpb->ioVRefNum = data_folder_FSSpec.vRefNum;
//     dpb->ioDrDirID = data_folder_FSSpec.parID;
//     dpb->ioNamePtr = data_folder_FSSpec.name;
//     dpb->ioFDirIndex=0;	  //query
//     return_value=PBGetCatInfo(&pb,0);
//     if (return_value) 
//     {
//     report_error ("load_level couldn't read a file:",the_filename,return_value);
//     return return_value;
//     }
//     folder_DirID=dpb->ioDrDirID;
//
//return_value=0;
//
//
//      fpb->ioVRefNum = data_folder_FSSpec.vRefNum;
//     fpb->ioDirID = folder_DirID;
//     fpb->ioNamePtr = the_filename;
//     fpb->ioFDirIndex=0;	  //query the file name
//     get_file_err=PBGetCatInfo(&pb,0);
//     if (get_file_err) report_error ("make_data_fsspec couldn't read a file:",the_filename,get_file_err);
//     if(get_file_err) return return_value;
////
////now load it
//     FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,the_filename,the_fsspec);
//}
	

