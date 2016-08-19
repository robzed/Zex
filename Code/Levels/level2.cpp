//Level2.c

/*


 $Log:

*/

//level2
//to successfully end a level, so the extern int level_complete to !0
//level 2 is to destroy the ionic device and then fly to 230000,175000 where
//a ship is waiting to pick you up.
//modded 060699 SB - when completed large ship is destroyed
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "3d_structures_def.h"
#include "objects_controls.h"	//contains monster defs
#include "load_objects.h"
#include "object_misc.h"
#include "compile_zmap.h"
#include "Error_handler.h"
#include "zex_misc.h"
#include "dlp.h"
#include "instrum_int.h"
#include "explode.h"
#include "object_control_top.h"
#include "sound.h"
#include "Batcom.h"
#include "monster_control_misc.h"
#include "obj_control_misc_routines.h"

//proto
void Level2();
void init_level2();
void quicksave_level2();
void restore_quicksave_level2();
void reinit_level2();

static int find_dynamic_object_slot_spesh(unsigned long the_id);


int done_monsters;	//used to control when we set off monsters
int LFrame_counter,total_frame_counter;	//timer
int next_ast_time;
int interval;	//how long before we gen a monster
int doing_destroy_ship;
static int destroy_ship_delay,destroy_item_count;

static unsigned long bg_objs[]= {'B001','B002','B003','B004','B005',
                          'B006','B007','WH01','TS01','TS02',
                          'H001'};
                
void init_level2()
{
  add_to_text_display("FOLLOW GREEN RADAR VECTOR TO TARGET.",0);
  done_monsters=0;
  LFrame_counter=0;
  total_frame_counter=0;
  next_ast_time=7000;
  interval=RangedRdm(600,800);	//how long before we gen a monster
  doing_destroy_ship=0;
  destroy_ship_delay=destroy_item_count=0;
}


void Level2()
{
extern int level_complete,can_do_next_level,player_input_counter; 
extern int level_start_time_ticks;	//tick count at start of level
extern float dest_x,dest_y;
extern	DynObjectsFixedSize *ocb_ptr;
extern int launch_timer;
extern int score,credits;
extern kit_desc ship_items[ITEMS_COUNT];	//ships systems
extern int score;
extern _3D ship_rotated_normal; //from engine

LSRect pick_up_box;
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot,ast_x,ast_y;
anim_pb anim;




//two checks - first is the ionic device destroyed - these have an id of
//IOD1.2.3 as these are made up of three objects - base, middle, top
//second is the ship in a given rectangle
//level_complete=1;

//uncomment next 3 lines to set end of level
//  level_complete=1;	  //IOD1 has been destroyed
//  can_do_next_level=1;
//return;

//is IOD1 in_use
if(level_complete==0)
{
 done_monsters=0;	//reset
// if (find_dynamic_object_slot('IOD1')==-1 && find_dynamic_object_slot('SGN1')==-1)
 if (find_dynamic_object_slot('SGN1')==-1)
 { 
 if (find_dynamic_object_slot('IODA')==-1)
  {
   level_complete=1;	   //IOD1 has been destroyed
 //keep player on his toes :)
    GenRandomMonster();
    GenRandomMonster();
    GenRandomMonster();
    GenRandomMonster();
    GenRandomMonster();
    GenRandomMonster();
    GenRandomMonster();
//    GenRandomNastyMonster();
//    GenRandomNastyMonster();
    GenRandomMonster();
    GenRandomMonster();
   can_do_next_level=1;
  }
 }
}
else	//must be either 1 or 2

if (level_complete==1)	//radar is now indicating vector to pick up zone
{
 if (done_monsters==0)
 {
//send out two phoenix and one blue thing
//phoenix's
  done_monsters=1;
  score=score+300;
  credits=credits+100;
//  add_to_text_display("OBJECTIVE ACHIEVED! DOCK WITH MOTHERSHIP.",0);
//    anim.object_id[0]='MN31';
//    anim.object_id[1]='MN32';
//    anim.object_id[2]='MN33';
//    anim.object_id[3]='MN34';
//	    the_params.world_z=MONSTER_Z;
//	    the_params.Omovedx=0;
//	    the_params.Omovedy=0;
//	    the_params.Omovedz=0;
//	    the_params.delta_rot_x=0;
//	    the_params.delta_rot_y=0;
//	    the_params.delta_rot_z=0;
//	    the_params.rot_x=0;
//	    the_params.rot_y=0;
//	    the_params.rot_z=0;
//	    the_params.controller_ref=monster4;
//	    the_params.control_use_1=0;
//	    the_params.control_use_2=0;
//	    the_params.control_use_3=0;
//	    the_params.control_use_4=0;
//	    the_params.current_velocity=0;
//	    the_params.wanted_velocity=0;
//	   the_params.attack_mode=0;
//	    the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	    the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	    the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	    the_params.shield_value=470;
//	    the_params.mass=100;
//	    the_params.no_trivial_rejection=1;
//	    the_params.object_category=baddie_object;
//
//	    the_params.cannon_cat=1;
//	    the_params.missile_cat=1;
//      the_params.laser_cat=1;
//      the_params.number_of_missiles=1;
//    
//   for (i=0;i<1;i++)	  //gen twoo of them
//   {
//   
//  	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+6000+(i*2000); 
//    	  the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+6000+(i*2000);
//     load_animated_object (&anim,4,3,1,1,&the_params);
//   }	  //end of for
//blue thingy

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
        clear_pb(&the_params);
        
	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx-16000;
	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy-16000;	
	the_params.world_z=MONSTER_Z;
	the_params.controller_ref=monster2;
	the_params.shield_value=370;
	the_params.mass=100;
	the_params.no_trivial_rejection=1;
	 the_params.object_category=baddie_object;
     static_slot=get_static_object_slot('MON2');	//go fetch fido (the object)
	 if (static_slot==-1) 
	     report_error_c("Level1: get_static_object_slot failed.","",-3);
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
    }
    doing_destroy_ship=1;
    add_to_text_display("FILMING DESTRUCTION OF SHIP!",0);

   }	   //end if if level_complete=1

}	//end of else not level_complete=0



//destroy the large bg ship bit by bit...
if (doing_destroy_ship==1)
   {
    int object,slot_id;
    
    score=score+3;
    credits=credits+1;
    
    destroy_ship_delay--;
    if (destroy_ship_delay<0)
    {
     destroy_ship_delay=5; //RangedRdm(2,);

     slot_id=bg_objs[RangedRdm(0,10)];	//random object ID out of array of bg id's

     object=find_dynamic_object_slot_spesh(slot_id);

     if (object!=-1)
      {
          insert_batcom_message("FILMING...      ", "                ");
//       add_to_text_display("FILMING...",0);

       destroy_item_count++;
       if (destroy_item_count>90) 
       {
       
       doing_destroy_ship=0;	//we wont be called again
       level_complete=2;
       insert_batcom_message("FILMING COMPLETE","MISSION COMPLETE");
       add_to_text_display("FOLLOW GREEN RADAR VECTOR TO MOTHERSHIP.",1);
      add_to_text_display("DOCK WITH MOTHERSHIP TO END MISSION.",1);

       }
       if (RangedRdm(0,100)>6)
       {
       add_to_explode_list(object,5);
       add_to_kill_list(object);
       }
       else	//change 070699 - we make them fall away
       {
       (*ocb_ptr).object_list[object].Omovedz=100;
       (*ocb_ptr).object_list[object].Orotdz=RangedRdm(0,10)-5;
       (*ocb_ptr).object_list[object].control_use_1=1;

       }
//      kill (object);
       if (RangedRdm(0,10)>5)
       play_zsound_relative_to_zex(object, short_explosion);
       else
       play_zsound_relative_to_zex(object,missile_explode);

      }
      
    }
   }

//we also maintain a timer that gens a new monster every n frames
//if player has more than 25% shields
//and asteroids every n frames
LFrame_counter++;
total_frame_counter++;

if (total_frame_counter>next_ast_time+5)
{
 total_frame_counter=0;
 if (ship_items[KNAV_COMPUTER].status==1)
 {
  if (RangedRdm(1,10)>5)
  add_to_text_display("WARNING: MAGNETIC ANOMALIES DETECTED",1);
  else
  add_to_text_display("WARNING: ABNORMAL NATURAL MAGNETIC FLUX VARIATIONS",1);
 }
 
 next_ast_time=RangedRdm(8000,25000);	//number of frames to next asteroid set
}

if (total_frame_counter>next_ast_time && total_frame_counter<next_ast_time+4)
{

//gen an asteroid
  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
           clear_pb(&the_params);

   	ast_x=static_cast<int>((*ocb_ptr).object_list[0].Oworldx+40000);
//	ast_x=ast_x+(40000*ship_rotated_normal.x);
	   ast_y=static_cast<int>((*ocb_ptr).object_list[0].Oworldy);
	   ast_y=static_cast<int>(ast_y+(40000*ship_rotated_normal.y));

	the_params.world_x=ast_x+RangedRdm(0,20000)-10000;
	the_params.world_y=ast_y+(RangedRdm(0,20000)-10000);	
	the_params.world_z=MONSTER_Z;
	the_params.Omovedx=RangedRdm(0,120)-140;
	the_params.delta_rot_y=RangedRdm(1,10)-5;
	the_params.delta_rot_z=RangedRdm(1,10)-5;
	the_params.controller_ref=asteroid1;
	the_params.shield_value=31170;
	the_params.mass=11000;
		 	 the_params.dq=0;
	the_params.no_trivial_rejection=1;
	 the_params.object_category=scenary_object;
     static_slot=get_static_object_slot('AST1');	//go fetch fido (the object)
	 if (static_slot==-1) 
	     report_error_c("Level1: get_static_object_slot failed.","",-3);
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
    }
}

//make it easier
//if (player_input_counter==5500)
//{
//   GenRandomMonster();
//   GenRandomMonster();
//   GenRandomMonster();
//   GenRandomMonster();
//   GenRandomMonster();
//   GenRandomMonster();
//   GenRandomMonster();
////   GenRandomNastyMonster();
////   GenRandomNastyMonster();
//   GenRandomMonster();
//   GenRandomMonster();
//}

//gen a monster at random
if (LFrame_counter>interval)
{
  LFrame_counter=0;
  if (ship_items[KSHIELDS].value1>250)
  {
   GenRandomMonster();
   interval=400+RangedRdm(100,200);
   
   if (score>3000 && RangedRdm(10,100)>40) 
   {
   interval+=200;
   GenRandomMonster();
   }
   if (score>50000 && RangedRdm(10,100)>20)
   {
   interval+=200;
   GenRandomMonster();
   }
   if (score>150000)
   {
   interval+=200;
   GenRandomMonster();
   }
   if (score>200000)
   {
   interval+=100;
   GenRandomMonster();
   }


  }
    
 }	//end of LFrame_counter
 
 
}	//end


int find_dynamic_object_slot_spesh(unsigned long the_id)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int	dynamic_object_index;	//how many dynamic objects
int return_val;

int match=0,active_object_count, object_count;
return_val=-1;

object_count=active_object_count=0;
while (active_object_count<dynamic_object_index  && object_count<MAX_NUMBER_OF_DYN_OBJECTS)
{
  if ((*ocb_ptr).object_list[object_count].in_use==1 && 
      (*ocb_ptr).object_list[object_count].control_use_1!=1)
  {
   if ((*ocb_ptr).object_list[object_count].Oid==the_id)
   {
     match=1;
     return_val=object_count;
     break;
   }
   active_object_count++;
 }
 object_count++;
 if (object_count>=MAX_NUMBER_OF_DYN_OBJECTS) break;

}
if (match==1) return return_val;	//slot of static definition of bg object
else return return_val;

}


void quicksave_level2()
{
}

void restore_quicksave_level2()
{
}

void reinit_level2()
{
}



