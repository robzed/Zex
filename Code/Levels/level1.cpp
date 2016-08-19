//Level1.c

/*


 $Log:

*/

//level1
//to successfully end a level, so the extern int level_complete to !0
//level 1 is to destroy the ionic device and then fly to 230000,175000 where
//a ship is waiting to pick you up.
//modded 060699 SB - when completed large ship is destroyed

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <string.h>
//oldmac//#include <resources.h>
#include "3d_structures_def.h"
#include "objects_controls.h"	//contains monster defs
#include "load_objects.h"
#include "LS_Standard.h"
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
#include "input_control.h"
#include "ship_control.h"

//proto
void Level1();
void init_level1();
void quicksave_level1();
void restore_quicksave_level1();
void reinit_level1();
void get_new_dialog_string(int index);
void load_tut_object(int static_slot,int controller, int shields);
void load_tut_object1(int static_slot,int controller, int shields, int mass, int laser, int cannon);

extern int ship_z_ang;

extern kit_desc ship_items[ITEMS_COUNT];
int tutorial_phase;
static int old_tutorial_phase,bcolour,dialog_y;;                
static int has_been_up,drag_counter,done_load,hide_dialog,next_lockout;
extern int camera_mode;
extern int global_show_target_vector;
void init_level1()
{
  add_to_text_display("Welcome to Zex!",0);
  tutorial_phase=0;
  old_tutorial_phase=-1;
  LS_ShowCursor();
  bcolour=0;
  dialog_y=0;
  get_new_dialog_string(0);
  has_been_up=1;
  drag_counter=0;
  done_load=0;
  camera_mode=0;
  global_show_target_vector=0;	//disable nav vectors
  hide_dialog=0;
  next_lockout=0;
  
}

static char dialog_str[1000];

void Level1()
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
extern int thrust,global_show_status,thrust_lockout;

LSRect pick_up_box;
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot,ast_x,ast_y;
anim_pb anim;
int button_state;
Point mouse_xy;



ZexGetMouse (&mouse_xy);
button_state=LS_Left_Button();
LS_ShowCursor();	//always cursor on in tutorial

//draw box in top left of screen
if (hide_dialog==1)
{
add_to_dlp(DRAW_FILLED_BOX,10,10,20,20,bcolour,0);
bcolour++;
if (bcolour>255) bcolour=0;

    if (button_state==1 && has_been_up==1 &&mouse_xy.h>10 && mouse_xy.h<20 
       && mouse_xy.v>10 && mouse_xy.v<20) hide_dialog=0;	//show it

}

//check for mouse click over box

    if (hide_dialog==0 && button_state==1 && has_been_up==1 &&mouse_xy.h>260 && mouse_xy.h<330 
       && mouse_xy.v>90+dialog_y && mouse_xy.v<100+dialog_y)
    hide_dialog=1;
    
    
    if (hide_dialog==0 && button_state==1 && has_been_up==1 &&mouse_xy.h>330 && mouse_xy.h<390 
       && mouse_xy.v>90+dialog_y && mouse_xy.v<100+dialog_y && next_lockout==0)
    {
    tutorial_phase++;
    get_new_dialog_string(tutorial_phase);
    }
    else
        if (hide_dialog==0 && button_state==1 && has_been_up==1 &&mouse_xy.h>330 && mouse_xy.h<390 
       && mouse_xy.v>90+dialog_y && mouse_xy.v<100+dialog_y && next_lockout==1)
    {
    //drag lockout is true
          play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

    }
    else	//drag dialog?
    {
    if (button_state==1 && drag_counter>8)
    dialog_y=mouse_xy.v;
    if (dialog_y<0) dialog_y=0;
    if (dialog_y>290) dialog_y=290;
    }
    
if (old_tutorial_phase != tutorial_phase)
    {
     old_tutorial_phase=tutorial_phase;

    }	//end of phase inc

     switch (tutorial_phase)
     {
     case 0: case 1: case 2:
         {
          ship_items[KEJECTION].modifier=0;	//DO NOT allow ejection pods on tutorial
          ship_items[KEJECTION].status=0;	//no ejection pod in tutorial

 	 if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);

   	break;
         }
     case 3: //wait for left input
         {
	  if (hide_dialog==0)  add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if ((*ocb_ptr).object_list[0].Orotz!=0)
              {
                  hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);

              }
   	break;
         }

     case 4:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);

   	break;
         }

     case 5:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<5)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=295000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          
          thrust=0;
          done_load=6;
          hide_dialog=0;
          }
   	break;
         }
     case 6:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<6)	//our load in objects state var
          {
          
          thrust=0;
          done_load=7;
          hide_dialog=0;
          }
   	break;
         }
     case 7:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
   	break;
         }
         
     case 8:
         {
 	   if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
//          if (hide_dialog==0) 
//          {
//          add_to_dlp(DRAW_LINE_NO_CLIP,110,dialog_y+26,440,390,238,0);
//          }
          if (done_load<8)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=295000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          
          thrust=0;
          done_load=9;
          hide_dialog=0;
          }
   	break;
         }

     case 9:	//137 - 4 posts
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<10)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=504000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          
          thrust=0;
          done_load=10;
          hide_dialog=0;
          }
   	break;
         }


     case 10:	//cannon
         {
          camera_mode=0;
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<11)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=700000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          
          thrust=0;
          done_load=11;
          hide_dialog=0;
          }
   	break;
         }

     case 11:	//cannon
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
//          if (find_dynamic_object_slot('IODA')==-1)
//             {
//                  hide_dialog=0;
//                  tutorial_phase++;
//                  get_new_dialog_string(tutorial_phase);
//             }
   	break;
         }


     case 12:	//cannon
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<12)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=900000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          ship_items[KCANNON].modifier=2;
          thrust=0;
          done_load=12;
          hide_dialog=0;
          }
   	break;
         }

     case 13:	//laser - 141
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
   	break;
         }

     case 14:	//cannon
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<14)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=1100000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          ship_items[KCANNON].modifier=1;
          thrust=0;
          done_load=14;
          hide_dialog=0;
          }
               if (find_dynamic_object_slot('MON3')==-1)
             {
             int temp_slot;
                  hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  temp_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
                  load_tut_object(temp_slot,0,16000);
             }

   	break;
         }

     case 15:	//laser - indestructable m2 - 143
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<15)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=1100000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          thrust=0;
          done_load=15;
          hide_dialog=0;
          }


             if (find_dynamic_object_slot('MON3')==-1)
             {
             int temp_slot;
                  hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  temp_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
                  load_tut_object(temp_slot,0,670);
             }

   	break;
         }

     case 16:
         {
 	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (hide_dialog==0) 
//          {
//          add_to_dlp(DRAW_LINE_NO_CLIP,110,dialog_y+26,540,390,238,0);
//          }
          if (done_load<16)	//our load in objects state var
          {
             if (find_dynamic_object_slot('MON3')==-1)	//just in case it's been destroyed
             {
             int temp_slot;
                  hide_dialog=0;
                  get_new_dialog_string(tutorial_phase);
                  temp_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
                  load_tut_object(temp_slot,0,670);
             }
          thrust=0;
          done_load=16;
          hide_dialog=0;
          }
   	break;
         }

     case 17:	//prepare for live m2
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<17)	//our load in objects state var
          {
          int temp_slot;
          //kill any m2's
          temp_slot=find_dynamic_object_slot('MON3');
          if (temp_slot!=-1) kill(temp_slot);
          thrust=0;
          done_load=17;
          hide_dialog=0;
          }
   	break;
         }


     case 18:	//active m2
         {
          int temp_slot;
 	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<18)	//our load in objects state var
          {
          //kill any m2's
          temp_slot=find_dynamic_object_slot('MON3');
          if (temp_slot!=-1) kill(temp_slot);
         
          thrust=0;
          done_load=18;
          hide_dialog=0;
          }
 
          temp_slot=find_dynamic_object_slot('MON3');
          if (temp_slot==-1)
          {
//load in an m3 if not found
                  temp_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
                  load_tut_object1(temp_slot,22,670,100,1,2);
                  credits=250;
          }
          
   	break;
         }

     case 19:	//missiles - indestructable m2 - 143
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<19)	//our load in objects state var
          {
		  ship_items[KMISSILE_PYLON1].status=1;
		  ship_items[KMISSILE_PYLON1].modifier=1;
		  ship_items[KMISSILE_PYLON2].status=1;
		  ship_items[KMISSILE_PYLON2].modifier=1;
		  ship_items[KMISSILE_PYLON3].status=1;
		  ship_items[KMISSILE_PYLON3].modifier=1;
          (*ocb_ptr).object_list[0].Oworldx=-305000;
          (*ocb_ptr).object_list[0].Oworldy=0;
          ship_z_ang=0;
          thrust=0;
          done_load=19;
          hide_dialog=0;
                  credits=250;

          }


             if (find_dynamic_object_slot('MON3')==-1)
             {
             int temp_slot;
                  hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  temp_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
                  load_tut_object(temp_slot,0,670);
                  credits=250;
             }

   	break;
         }

     case 20:	//laser - 141
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (hide_dialog==0) 
//          {
//          add_to_dlp(DRAW_LINE_NO_CLIP,200,dialog_y+60,440,390,238,0);
//          }
   	break;
         }

     case 21:	//load new missile and show user
         {
 	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<21)	//our load in objects state var
          {
          thrust=0;
          done_load=21;
          hide_dialog=0;
          ship_items[KMISSILE_PYLON1].modifier=2;
                  credits=250;
          }
   	break;
         }
//navigation - fly towards mothership
     case 22:	//load new missile and show user
         {
 	 int dist,ms_slot;
 	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<22)	//our load in objects state var
          {
          thrust=0;
          done_load=22;
          hide_dialog=0;
          global_show_target_vector=1;	//disable nav vectors
          ship_items[KMISSILE_PYLON1].modifier=1;
          ship_items[KMISSILE_PYLON2].modifier=1;
          ship_items[KMISSILE_PYLON3].modifier=1;
          next_lockout=1;	//cant next
          }
         //wait for distance between zex and mothership is 1000
         ms_slot=find_dynamic_object_slot('MS02');
         if (ms_slot!=-1)
          {
          dist=get_distance_to_from(ms_slot,0);
          if (dist<19000) 
            {
                   hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  thrust=0;
                  thrust_lockout=1;	//user can't thrust
                  credits=250;

            }
          }
   	break;
         }

     case 23:	//
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if ((*ocb_ptr).object_list[0].request_dock==1)
            {
                   hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  thrust=0;
                  credits=250;

            }
          
    	 break;
         }
     
//nothing happening
     case 24:	//remove dock command, fly closer to mship
         {
 	 int dist,ms_slot;
 	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<24)	//our load in objects state var
          {
          thrust=0;
          done_load=24;
          hide_dialog=0;
          global_show_target_vector=1;	//disable nav vectors
          next_lockout=1;	//cant next
          thrust_lockout=0;
          }
         //wait for distance between zex and mothership is 1000
         ms_slot=find_dynamic_object_slot('MS02');
         if (ms_slot!=-1)
          {
          dist=get_distance_to_from(ms_slot,0);
          if (dist<13000) 
            {
                   hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  thrust=0;
                  thrust_lockout=1;	//user can't thrust
                  credits=250;

            }
          }
   	break;
         }

//dock again
     case 25:	//
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if ((*ocb_ptr).object_list[0].request_dock==1)
            {
                   hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  thrust=0;
                  thrust_lockout=0;	//user can't thrust
                  credits=250;

            }
          
    	 break;
         }

//bye
     case 26:	//
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
         
          if ((*ocb_ptr).object_list[0].request_dock==0)
            {
                  hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  thrust=0;
                  LS_ShowCursor();

            } 
    	 break;
         }
//hello again!
     case 27:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
	            next_lockout=0;	//can next

   	break;
         }

     case 28:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);

   	break;
         }


     case 29:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<29)	//our load in objects state var
          {
          extern int ship_z_ang;
          (*ocb_ptr).object_list[0].Oworldx=0;
          (*ocb_ptr).object_list[0].Oworldy=200000;
          ship_z_ang=0;
          thrust=0;

          done_load=29;
          hide_dialog=0;
          }

   	break;
         }

     case 30:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);

   	break;
         }

     case 31:
         {
         int temp_slot;
	if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          if (done_load<31)	//our load in objects state var
          {
          (*ocb_ptr).object_list[0].Oworldx=0;
          (*ocb_ptr).object_list[0].Oworldy=200000;
          ship_z_ang=0;
          thrust=0;
          ship_items[KASTEROID_ADDITION].modifier=1;
          ship_items[KASTEROID_ADDITION].status=1;
          
          done_load=31;
          hide_dialog=0;
                    next_lockout=1;	  //cant next

          }

          temp_slot=find_dynamic_object_slot('AST1');	//wait for all asteroids to be destroyed
          if (temp_slot==-1)
          {
                  hide_dialog=0;
                  tutorial_phase++;
                  get_new_dialog_string(tutorial_phase);
                  thrust=0;
                    next_lockout=0;	//can next

          }


   	break;
         }

     case 32:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);


   	break;
         }

     case 33:
         {
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);

   	break;
         }

     case 34:	//bye
         {
         extern int zex_alive, death_frame_counter;
	  if (hide_dialog==0) add_to_dlp(FLOATING_DIALOG,0,dialog_y,0,0,0,dialog_str);
          zex_alive=0;
          death_frame_counter=0;
          
   	break;
         }


     }

if (LS_Left_Button()==0) {
has_been_up=1;
drag_counter=0;
}
else 
{
has_been_up=0;
drag_counter++;;	//ycoord mouse button went down at
}
}	//end




void quicksave_level1()
{
}

void restore_quicksave_level1()
{
}

void reinit_level1()
{
}

//get the string into dialog_str;
void get_new_dialog_string(int index)
{
Handle hpic;
char * src_string;
 hpic = (Handle) LS_GetResource('TUTS',128+index);  //Get the Handle to the Resource 
 if (hpic==0 || ResError()!=0) report_error_c("get_new_dialog_string: Missing resource TUTS","",index+128);
 LS_DetachResource (hpic);
 LS_HLock (hpic);
// src_string=*hpic;
//copy to dialog_str
 strcpy (dialog_str,*hpic);
 LS_DisposeHandle(hpic);
}



void load_tut_object(int static_slot,int controller, int shields)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+7000;
	
	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy;
	the_params.world_z=-200;	//RangedRdm(2000,6000);
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	
	the_params.delta_rot_y=0;
	the_params.rot_y=0;
	the_params.shield_value=shields;
	the_params.mass=100;
	
	the_params.controller_ref=controller;
	the_params.object_category=ALIEN;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,it,&the_params,-1);	//the object, the position (-1=next free)


 }
}

void load_tut_object1(int static_slot,int controller, int shields, int mass, int laser, int cannon)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+7000;
	
	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy;
	the_params.world_z=-200;	//RangedRdm(2000,6000);
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	
	the_params.delta_rot_y=0;
	the_params.rot_y=0;
	the_params.shield_value=shields;
	the_params.mass=mass;
	the_params.laser_cat=laser;
	the_params.cannon_cat=cannon;
	
	the_params.controller_ref=controller;
	the_params.object_category=ALIEN;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,it,&the_params,-1);	//the object, the position (-1=next free)


 }
}



