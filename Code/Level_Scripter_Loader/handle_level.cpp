//handle level - dispatches to right level handler
//called once every frame
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


#include "3d_structures_def.h"
#include "objects_controls.h"	//contains monster defs
#include "load_objects.h"
#include "object_misc.h"
#include "compile_zmap.h"
#include "Error_handler.h"
#include "dlp.h"
#include "handle_level.h"
//level prototypes
void Level1();
void Level2();
void Level3();
void Level4();
void Level5();
void Level6();


void init_level1();
void init_level2();
void init_level3();
void init_level4();
void init_level5();
void init_level6();



void quicksave_level1();
void quicksave_level2();
void quicksave_level3();
void quicksave_level4();
void quicksave_level5();


void restore_quicksave_level1();
void restore_quicksave_level2();
void restore_quicksave_level3();
void restore_quicksave_level4();
void restore_quicksave_level5();


int can_level3_quicksave();

void reinit_level1();
void reinit_level2();
void reinit_level3();
void reinit_level4();
void reinit_level5();


//changed 9/2/99
//if level_complete==2 then we dont run the level controller anymore, but deal with
//it ourselves as the only thing the player can do is dock with mothership(s)


void handle_level()
{
extern int current_level,level_complete;
extern int launch_timer, training_complete, training_mission;
extern float dest_x,dest_y;
extern	DynObjectsFixedSize *ocb_ptr;

LSRect pick_up_box;

if (level_complete==3) return;	//docked

if (level_complete==2)	//we dont run the level from here on, we look for docked conditions
{
 if (launch_timer==0)	//we are not launching
  {
  pick_up_box.top=static_cast<int>(dest_y+1000);
  pick_up_box.bottom=static_cast<int>(dest_y-1000);
  pick_up_box.left=static_cast<int>(dest_x-3000);
  pick_up_box.right=static_cast<int>(dest_x+3000);

  //check whether we are in the end zone
  if (point_in_rect(static_cast<int>((*ocb_ptr).object_list[0].Oworldx),
                  static_cast<int>((*ocb_ptr).object_list[0].Oworldy),
                  pick_up_box) && (*ocb_ptr).object_list[0].request_dock==1) 
				  {
                                level_complete=3;	//Thank You, Thank You. Thank You..
								if (training_mission)
								{
									training_complete=1;	//mission 1 ends when we dock
								}
				  }
 }
}

else
{
 if (launch_timer==0)	//we are not launching
  {
  pick_up_box.top=static_cast<int>(dest_y+1000);
  pick_up_box.bottom=static_cast<int>(dest_y-1000);
  pick_up_box.left=static_cast<int>(dest_x-3000);
  pick_up_box.right=static_cast<int>(dest_x+3000);

  //check whether we are in the end zone
  if (point_in_rect(static_cast<int>((*ocb_ptr).object_list[0].Oworldx),
                  static_cast<int>((*ocb_ptr).object_list[0].Oworldy),
                  pick_up_box)  && (*ocb_ptr).object_list[0].request_dock==1) 
  {
                                level_complete=3;	//Thank You, Thank You. Thank You..
   return;
  }
 }
//not level 2 or in dock box so run the level

   switch (current_level)
   {
   
   case 1:
           Level1();
           break;
   case 2:
           Level2();
           break;
   case 3:
           Level3();
           break;
   case 4:
           Level4();
           break;
   case 5:
           Level5();
           break;
   case 6:	//the game
           Level6();
           break;

   }	//end of switch
}

}



void init_level()
{
extern int current_level;

   switch (current_level)
   {
   
   case 1:
           init_level1();
           break;
   case 2:
           init_level2();
           break;
   case 3:
           init_level3();
           break;
   case 4:
           init_level4();
           break;
   case 5:
           init_level5();
           break;

   case 6:
           init_level6();
           break;
          
   }	//end of switch

}


int level_can_quicksave()
{
extern int current_level;

   switch (current_level)
   {
   
   case 1:
           return 0;
           break;
   case 2:
	return 0;
           break;
   case 3:
           return can_level3_quicksave();
          
           break;
   case 4:
            return 0;
           break;
   case 5:
            return 0;
           break;

   case 6:	//the game
            return 0;
           break;
          
   }	//end of switch

   return 0;		// shouldn't get here.
}

void do_level_quicksave()
{
extern int current_level;

   switch (current_level)
   {
   
   case 1:
           quicksave_level1();

           break;
   case 2:
           quicksave_level2();
           break;
   case 3:
           quicksave_level3();
           break;
   case 4:
           quicksave_level4();
           break;
   case 5:
           quicksave_level5();
           break;
          
   }	//end of switch

}


void do_level_quicksave_restore()
{
extern int current_level;

   switch (current_level)
   {
   
   case 1:
           restore_quicksave_level1();

           break;
   case 2:
           restore_quicksave_level2();
           break;
   case 3:
           restore_quicksave_level3();
           break;
   case 4:
           restore_quicksave_level4();
           break;
   case 5:
           restore_quicksave_level5();
           break;
          
   }	//end of switch

}


void do_level_reinit()
{
extern int current_level;

   switch (current_level)
   {
   
   case 1:
           reinit_level1();

           break;
   case 2:
           reinit_level2();
           break;
   case 3:
           reinit_level3();
           break;
   case 4:
           reinit_level4();
           break;
   case 5:
           reinit_level5();
           break;
          
   }	//end of switch

}




