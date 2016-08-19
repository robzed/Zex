// *************************************MODULE HEADER*********************************
// * FILENAME           : load_objects.c
// * MODULE TITLE       : load_objects.c
// * PROJECT            : Zex
// * DATE STARTED       : 22:20:32 on 22nd June 1999
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

#include <string.h>		// included for memcpy
//oldmac//#include <files.h>
#include "file_io.h"
#include "MeshToZex.h"
#include "3d_structures_def.h"
#include "load_objects.h"
#include "zex_misc.h"
#include "objects_controls.h"
#include "compile_zmap.h"
#include "LS_Standard.h"
#include "Z_init.h"
#include "pictures.h"
#include "texture_management.h"
#include "Error_handler.h"
#include "load_level.h"
#include "light_control.h"
#include "monster_control_misc.h"
#include "motherships.h"
#include "dlp.h"
#include "obj_control_misc_routines.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
int	static_object_index;	//how many static objects

char *taddr;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *





//Zex Load objects
//SB 14/1198
/*
 * $Log: load_objects.cpp,v $
 * Revision 1.16  2006/01/08 03:06:32  stu
 * Zex2005: Code: Removed all remaining warnings. There should now be no compile warnings
 *
 * Revision 1.15  2005/12/30 16:21:49  stu
 * Zex2005: Code: Adds:- Missile Factory. Shockwave, ECM amd Warp jump.
 *
 * Revision 1.14  2005/12/17 10:48:07  rob
 * Zex2005: Added full nested braces for nested structure/array initialisations. Not strictly necessary, but it only took me a few minutes and I did find one minor error in the movie data.
 *
 * Revision 1.13  2005/12/16 22:15:55  rob
 * Zex2005: Dust initialiser missing comma - Minor warning removal after -Wall -W
 *
 * Revision 1.12  2005/12/13 22:53:06  rob
 * Zex2005: Got rid of non-void return warning, plus id being -1 warning (by making id signed rather than unsigned).
 *
 * Revision 1.11  2005/11/08 22:45:25  stu
 * Zex2005: Code: Various bug fixes as per the TODO
 *
 * Revision 1.10  2005/10/10 21:28:59  rob
 * Zex2005: Added newline at end to stop compiler whinging.
 *
 * Revision 1.9  2005/10/09 16:30:00  rob
 * Zex2005: More float to int conversion casts.
 *
 * Revision 1.8  2005/10/08 13:13:56  rob
 * Zex2005: "/p" pascal string size marker is not supported in normal gcc - it's a apple specific extension turned on by the option -fpascal_strings. This change removes that string so that the code will run both on Mac and Windows. Lots of files have been changed - but nearly all the changes are trivial. By the fun-jobs-are-us dept.
 *
 * Revision 1.7  2005/09/29 00:49:30  stu
 * Zex2005: Code: Basic x86 port
 *
 * Revision 1.6  2005/05/17 16:22:17  rob
 * Zex2005: Even more work sorting out build problems on gcc4.0/MacOSX/SDL conversion.
 *
 * Revision 1.5  2005/05/17 13:11:26  rob
 * Zex2005: Various assorted conversion fixes.
 *
 * Revision 1.4  2005/05/15 21:40:08  rob
 * Zex2005: gcc4 fixes - Command and control bug and init memory.
 *
 * Revision 1.3  2005/05/02 10:30:08  rob
 * Zex2005: More Updates (code)
 *
 * Revision 1.2  2005/04/23 20:11:23  rob
 * ZEX: First set of changes for SDL build. (remainder of files)
 *
 * Revision 1.29  1999/11/07 17:33:29  stu
 * *** empty log message ***
 *
 * Revision 1.28  1999/11/07 04:04:16  stu
 * 0.94.3
071199 SB
 *
 * Revision 1.27  1999/10/17 20:42:40  stu
 * 17 Oct 99 SB
 *
 * Revision 1.26  1999/10/16 04:22:53  stu
 * 151099
 *
 * Revision 1.25  1999/10/01 04:11:44  stu
 * 0.94.2 011099 SB
 *
 * Revision 1.24  1999/09/27 14:49:28  stu
 * Fixes NPC's bust on 260999
 *
 * Revision 1.23  1999/09/26 18:34:42  stu
 * 260999
 *
 * Revision 1.22  1999/09/20 19:03:30  stu
 * 0.93.3
 *
 * Revision 1.21  1999/09/19 05:38:53  stu
 * 0.93 as posted
 *
 * Revision 1.20  1999/09/14 15:07:47  stu
 * 14/9/99
 *
 * Revision 1.19  1999/09/12 00:56:13  stu
 * 120999
 *
 * Revision 1.18  1999/08/30 22:23:00  stu
 * 300899
 *
 * Revision 1.17  1999/08/20 09:57:29  stu
 * 20 Aug 99
 *
 * Revision 1.16  1999/08/15 22:17:00  stu
 * Cleanup 150899
 *
 * Revision 1.15  1999/07/26 19:08:43  stu
 * Pre 0.86 test 27 July 99
 *
 * Revision 1.14  1999/07/19 17:42:21  stu
 * 200799
 *
 * Revision 1.13  1999/07/17 18:35:52  stu
 * 180799 - level 4 changes
 *
 * Revision 1.12  1999/07/13 04:07:59  stu
 * Clean up 130799
 *
 * Revision 1.11  1999/07/04 18:37:17  stu
 * Clean up, Sunday 4th july
 *
 * Revision 1.10  1999/07/04 00:21:29  stu
 * Major changes and development.
Colldet changes.
Level 2,3,4
Version 0.86
 *
 * Revision 1.9  1999/06/25 01:14:36  stu
 * Lurkers clewan up
 *
 * Revision 1.8  1999/06/23 00:51:50  stu
 * Changes for new lighting control
 *
 * Revision 1.7  1999/06/22 10:57:31  stu
 * Lurkers clean up
 *
 * Revision 1.6  1999/05/25 04:49:34  stu
 * Missiles not working fix
 *
 * Revision 1.5  1999/05/21 03:31:23  stu
 * 1. New screen driver.
2. VBL Driven music fade and play
 *
 * Revision 1.4  1999/05/12 15:20:12  stu
 * Added $Log to top of file
 *
*/

extern int number_of_stars;



//load objects is no longer called - the level loader now does this job.
//code left for paranoia reasons only
int load_objects()	//loads objects needed into the static arrays
{
FSSpec the_file;
Handle the_file_data_H;
load_dyn_object_pb the_params;
int Err,i;
_3D dustPos [19] = {
{ 000,1000,800 },
{ 2000,950,400 },
{ 2500,-850,600 },
{ 1000,-1000,300 },
{ 0,-1500,250 },
{ -300,-2000,600 },
{ -500,-2500,270 },
{ -10000,00,330 },
{ -5000,1500,200 },
{ 3000,-2000,100 },
{ -8000,-4000,700 },
{ -7000,3000,600 },
{ -7546,456,567 },
{ -230,5000,214 },
{ -1000,-3,200 },
{ -4000,1234,700 },
{ -4321,321,200 },
{ -3000,0,250 },
{ -4000,-1000,400 }
};


extern FSSpec Zex_FSSpec;
anim_pb anim;
int dest_ocb;

//test data
Handle T000_H;
LSRAW *T000_P;

//end test data
    static_object_index=0;


////testing - read in texture map
//	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"T000.RAW",&the_file);
//	  T000_H=read_file(the_file);
//	  LS_HLock(T000_H);
//	  T000_P = (LSRAW*)* T000_H;
//    taddr=(char *)&T000_P->raw_data;	  //ptr to raw data
    reset_objects();
    
//read an object
    load_object ("zex_game.z",0,'ZEXS',yes_colldet,0,IS_NOT_TRANSLUCENT);
    
    load_object ("cannon0.z",1,'MIS1',yes_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("cannon1.z",2,'MIS2',yes_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("star.z",3,'DST1',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("star2.z",4,'DST2',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("monst2.z",5,'MON1',yes_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("ejectcap.z",6,'EC01',yes_colldet,'TB00',IS_NOT_TRANSLUCENT);

    load_object ("laser1.z",7,'LAS1',no_colldet,0,IS_NOT_TRANSLUCENT);


//background objects go in at static_bg_slot_start
    load_object ("B001.z",8,'B001',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("B002.z",9,'B002',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("B000.z",10,'B000',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("B003.z",11,'B003',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("B004.z",12,'B004',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("B005.z",13,'B005',no_colldet,0,IS_NOT_TRANSLUCENT);


    load_object ("B006.z",14,'B006',no_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("B007.z",15,'B007',no_colldet,'TB07',IS_NOT_TRANSLUCENT);

    load_object ("H001.z",16,'H001',yes_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("H002.z",17,'H002',yes_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("monst1.z",18,'MON2',yes_colldet,'T003',IS_NOT_TRANSLUCENT);


    load_object ("static_gun1A.z",19,'SGN1',yes_colldet,'TB08',IS_NOT_TRANSLUCENT);
    load_object ("static_gun2A.z",20,'SGN2',yes_colldet,'TB08',IS_NOT_TRANSLUCENT);
    load_object ("static_gun3A.z",21,'SGN3',yes_colldet,'TB08',IS_NOT_TRANSLUCENT);

    load_object ("monst3a.z",22,'MN31',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("monst3b.z",23,'MN32',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("monst3c.z",24,'MN33',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("monst3d.z",25,'MN34',yes_colldet,0,IS_NOT_TRANSLUCENT);

    load_object ("ast1.z",26,'AST1',yes_colldet,0,IS_NOT_TRANSLUCENT);


//set up ship
	the_params.world_x=4000;
	the_params.world_y=-2000;
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
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
     the_params.has_roll=0;
	the_params.mass=100;
		 	 the_params.dq=0;
	the_params.object_category=zex_object;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.number_of_missiles=0;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(zex,0,&the_params,-1);	   //the object, the position (-1=next free)




//load in a monster
	the_params.world_x=17000;
	the_params.world_y=-1000;
	the_params.world_z=-2000;
	the_params.Omovedx=-40;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=2;
	the_params.delta_rot_z=1;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=180;
	the_params.controller_ref=asteroid1;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_7=0;
	the_params.control_use_4=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	the_params.shield_value=10170;
	the_params.mass=1000;
		 	 the_params.dq=0;
	the_params.no_trivial_rejection=1;
	    the_params.object_category=scenary_object;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(26,1,&the_params,-1);	   //the object, the position (-1=next free)
//
//
//	  the_params.world_x=0;
//	  the_params.world_y=0;
//	  the_params.world_z=MONSTER_Z;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=180;
//	  the_params.controller_ref=monster1;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=170;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=1;
//	      the_params.object_category=baddie_object;
//	  
//	  load_dyn_object(5,2,&the_params);	//the object, the position (-1=next free)
//
//	  the_params.world_x=0;
//	  the_params.world_y=0;
//	  the_params.world_z=MONSTER_Z;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=180;
//	  the_params.controller_ref=monster2;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=170;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=1;
//	      the_params.object_category=baddie_object;
//
//	  load_dyn_object(5,3,&the_params);	//the object, the position (-1=next free)
//
//	  the_params.world_x=33000;
//	  the_params.world_y=-3000;
//	  the_params.world_z=MONSTER_Z;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=180;
//	  the_params.controller_ref=monster2;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=170;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=1;
//	  the_params.object_category=baddie_object;
//
//	  load_dyn_object(5,4,&the_params);	//the object, the position (-1=next free)
//
//	  the_params.world_x=60000;
//	  the_params.world_y=9000;
//	  the_params.world_z=MONSTER_Z;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=180;
//	  the_params.controller_ref=monster3;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=470;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=1;
//	      the_params.object_category=baddie_object;
//
//	  load_dyn_object(18,5,&the_params);	//the object, the position (-1=next free)
//
//
//	  the_params.world_x=20000;
//	  the_params.world_y=-10000;
//	  the_params.world_z=MONSTER_Z;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=180;
//	  the_params.controller_ref=monster3;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=470;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=1;
//	      the_params.object_category=baddie_object;
//
//	  load_dyn_object(18,1,&the_params);	//the object, the position (-1=next free)
//
//
//	  the_params.world_x=4000*4+1100;
//	  the_params.world_y=4900;
//	  the_params.world_z=100;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=0;
//	  the_params.controller_ref=gun1;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=370;
//	  the_params.mass=1000;
//	  the_params.no_trivial_rejection=1;
//	      the_params.object_category=baddie_object;
//
//	  load_dyn_object(19,2,&the_params);	//the object, the position (-1=next free)
//
//	  the_params.world_x=5000;
//	  the_params.world_y=-4500;
//	  the_params.world_z=100;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=0;
//	  the_params.controller_ref=gun1;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=370;
//	  the_params.mass=1000;
//	  the_params.no_trivial_rejection=1;
//	      the_params.object_category=baddie_object;
//
//	  load_dyn_object(19,1,&the_params);	//the object, the position (-1=next free)


		
//load in some stars
//	  for (i=0;i<10;i++)
//	  {
//	   dest_ocb=find_vacant_dynamic_object_slot();	  //get us a slot to put it in
//
//	  the_params.world_x=RangedRdm(0,12000);	//dustPos[i].x;
//	  the_params.world_y=RangedRdm(0,10000)-5000;	//dustPos[i].y;
//	  the_params.world_z=RangedRdm(0,3000)-2000;	//dustPos[i].z;
//	  the_params.Omovedx=-RangedRdm(10,42);	//vicious
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=RangedRdm(0,5);
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=RangedRdm(0,359);
//	  the_params.controller_ref=dust;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//    number_of_stars++;
//	  
//	  if(RangedRdm(0,2)>1)
//	  load_dyn_object(dust2,dest_ocb,&the_params);	  //the object, the position (-1=next free)
//	  else load_dyn_object(dust,dest_ocb,&the_params);
// 	}
	


//load two guns
    anim.object_id[0]='SGN1';
    anim.object_id[1]='SGN2';
    anim.object_id[2]='SGN3';
    
  	  the_params.world_x=4000*4+1100;; 
   	the_params.world_y=4900;
	  the_params.world_z=MONSTER_Z;
	  the_params.Omovedx=0;
	  the_params.Omovedy=0;
	  the_params.Omovedz=0;
	  the_params.delta_rot_x=0;
	  the_params.delta_rot_y=0;
	  the_params.delta_rot_z=0;
	  the_params.rot_x=0;
	  the_params.rot_y=0;
	  the_params.rot_z=0;
	  the_params.controller_ref=gun1;
	  the_params.control_use_1=0;
	  the_params.control_use_2=0;
	  the_params.control_use_3=0;
	  the_params.control_use_4=0;
	the_params.control_use_7=0;
	  the_params.current_velocity=0;
	  the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	  the_params.shield_value=470;
	  the_params.mass=1000;
	  the_params.dq=0;
	  the_params.no_trivial_rejection=1;
	  the_params.object_category=baddie_object;

     the_params.who_spawned_this_object=-1;
    load_animated_object (&anim,3,1,0,0,&the_params,-1);

  	  the_params.world_x=6500; 
   	the_params.world_y=-4500;
	  the_params.world_z=MONSTER_Z;
	  the_params.Omovedx=0;
	  the_params.Omovedy=0;
	  the_params.Omovedz=0;
	  the_params.delta_rot_x=0;
	  the_params.delta_rot_y=0;
	  the_params.delta_rot_z=0;
	  the_params.rot_x=0;
	  the_params.rot_y=0;
	  the_params.rot_z=0;
	  the_params.controller_ref=gun1;
	  the_params.control_use_1=0;
	  the_params.control_use_2=0;
	  the_params.control_use_3=0;
	  the_params.control_use_4=0;
	  the_params.current_velocity=0;
	the_params.control_use_7=0;
	  the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	  the_params.shield_value=470;
	  the_params.mass=1000;
	  the_params.dq=0;
	the_params.no_trivial_rejection=1;
	  the_params.object_category=baddie_object;

     the_params.who_spawned_this_object=-1;
    load_animated_object (&anim,3,1,0,0,&the_params,-1);

//load in an animated monster 2
    anim.object_id[0]='MN31';
    anim.object_id[1]='MN32';
    anim.object_id[2]='MN33';
    anim.object_id[3]='MN34';
   
  	  the_params.world_x=79000; 	//quite a way away
   	the_params.world_y=40000;
	  the_params.world_z=MONSTER_Z;
	  the_params.Omovedx=0;
	  the_params.Omovedy=0;
	  the_params.Omovedz=0;
	  the_params.delta_rot_x=0;
	  the_params.delta_rot_y=0;
	  the_params.delta_rot_z=0;
	  the_params.rot_x=0;
	  the_params.rot_y=0;
	  the_params.rot_z=0;
	  the_params.controller_ref=monster4;
	  the_params.control_use_1=0;
	  the_params.control_use_2=0;
	  the_params.control_use_3=0;
	  the_params.control_use_4=0;
	the_params.control_use_7=0;
	  the_params.current_velocity=0;
	  the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
     the_params.has_roll=0;
	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	  the_params.shield_value=670;
	  the_params.mass=100;
	  the_params.dq=0;
	the_params.no_trivial_rejection=1;
     the_params.who_spawned_this_object=-1;
	  the_params.object_category=baddie_object;

    load_animated_object (&anim,4,2,1,1,&the_params,-1);

//еееееееееееееееееееееееееееееееееееееееееее
//Now read and compile the BG map
//	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"L1.zmp",&the_file);
//	  the_file_data_H=read_file(the_file);
//    compile_zmap(*the_file_data_H);	//file compile_zmap.c
//	  init_cmap();	  //load in the initial objects we reckon are visible
//	  LS_DisposeHandle(the_file_data_H);

return 0;
}

int find_vacant_dyn_poly_slot();

extern	  DynObjectsFixedSize *ocb_ptr;

//loads up animation objects from the_objects
void load_animated_object(anim_pb* the_objects, int number_of_objects, int frame_rate, int control,
     int state, load_dyn_object_pb *the_pb, short galactic_id)
{
extern _ZPoly_objs *dynamic_polys_ptr;
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	dynamic_object_index;	//how many dynamic objs
int poly_slot=0;
int size_of_polys;
_ZPoly *new_polys_address=0;
_ZPoly *old_polys_address;
int i,static_slot=0,dest_ocb;
anim_obj_bits temp_anim_ptrs[6];
register ZObject * current_object_ptr;


     dest_ocb=find_vacant_dynamic_object_slot();	//get us a slot to put it in
     if (dest_ocb!=-1)
      {

      for (i=number_of_objects-1;i>-1;i--)
      {
		  static_slot=get_static_object_slot((*the_objects).object_id[i]);	//go fetch fido (the object)
		  if (static_slot==-1) report_error_id ("load_objects : load_animated_object couldn't find the following static object:",(*the_objects).object_id[i]);
		  else
		  {

	//now copy polys out of static into vacant dynamic
			poly_slot=find_vacant_dyn_poly_slot();
			if	(poly_slot!=-1)
			{
			 dynamic_polys_ptr[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
		//copy the polys across
			 new_polys_address=&dynamic_polys_ptr[poly_slot].the_obj_polys[0];	//so we can poke the new object
			 old_polys_address=(*ocb_Sptr).object_list[static_slot].polys_ptr;
		
			 size_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
			 size_of_polys=size_of_polys*sizeof(_ZPoly);
		
			 BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
		//
			temp_anim_ptrs[i].polys_ptr=new_polys_address;
			temp_anim_ptrs[i].polys_slot=poly_slot;
			temp_anim_ptrs[i].points_ptr=(*ocb_Sptr).object_list[static_slot].points_ptr;
			}	//end of load polys
				 else	//oops, couldn't get any polys! (Found on intel port)
			{
				add_to_text_display("Zex Exec> WARNING: load_animated_object failed to find poly_slot",1);;
				return; // -1;
			}
		  }	//end of got static
     }	//end of i loop

current_object_ptr=&ocb_ptr->object_list[dest_ocb];

//now load in the object
	(*ocb_ptr).object_list[dest_ocb]=(*ocb_Sptr).object_list[static_slot];	//copy the object

	current_object_ptr->in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	current_object_ptr->Oworldx=the_pb->world_x;
	current_object_ptr->Oworldy=the_pb->world_y;
	current_object_ptr->Oworldz=the_pb->world_z;
	current_object_ptr->Ostartx=the_pb->world_x;
	current_object_ptr->Ostarty=the_pb->world_y;

	current_object_ptr->Orotdx=the_pb->delta_rot_x;
	current_object_ptr->Orotdy=the_pb->delta_rot_y;
	current_object_ptr->Orotdz=the_pb->delta_rot_z;
	current_object_ptr->Orotx=the_pb->rot_x;
	current_object_ptr->Oroty=the_pb->rot_y;
	current_object_ptr->Orotz=the_pb->rot_z;
    current_object_ptr->invincible_count=0;
	current_object_ptr->Omovedx=the_pb->Omovedx;
	current_object_ptr->Omovedy=the_pb->Omovedy;
	current_object_ptr->Omovedz=the_pb->Omovedz;
	current_object_ptr->controller=the_pb->controller_ref;
	current_object_ptr->control_use_1=the_pb->control_use_1;
	current_object_ptr->control_use_2=the_pb->control_use_2;
	current_object_ptr->control_use_3=the_pb->control_use_3;
	current_object_ptr->control_use_4=the_pb->control_use_4;
	current_object_ptr->current_velocity=the_pb->current_velocity;
	current_object_ptr->wanted_velocity=the_pb->wanted_velocity;
	current_object_ptr->control_use_7=0;
	current_object_ptr->control_use_8=0;
	current_object_ptr->control_use_9=0;
	current_object_ptr->control_use_10=0;
	current_object_ptr->swerve_direction=0;
	current_object_ptr->attack_mode=the_pb->attack_mode;
	current_object_ptr->callsign=0;
	current_object_ptr->object_normal=the_pb->normal;
	current_object_ptr->shield_value=the_pb->shield_value;
	current_object_ptr->unique_id=0;
//stuff for NPC_Control
	current_object_ptr->going_to=the_pb->going_to;
	current_object_ptr->aggressive=the_pb->aggressive;
	current_object_ptr->obj_class=the_pb->obj_class;
	current_object_ptr->sub_controller=the_pb->sub_controller;
	current_object_ptr->shields_class=the_pb->shields_class;
	current_object_ptr->max_vel=the_pb->max_vel;
	current_object_ptr->min_vel=the_pb->min_vel;
	current_object_ptr->go_home_time=the_pb->go_home_time;
	current_object_ptr->dynamic_tex_ptr=0;
	current_object_ptr->time_out=1;
	current_object_ptr->rank=0;

//
//mothership specifc
	current_object_ptr->mship_ptr=0;	//very important!
	current_object_ptr->docking_what=-1;	//very important!
	current_object_ptr->galactic_id=galactic_id;

//next 4 130499
	current_object_ptr->original_shield_value=the_pb->shield_value;
//    current_object_ptr->tractor=0;
    current_object_ptr->docked_object_slot=-1;
	current_object_ptr->has_roll=the_pb->has_roll;
	current_object_ptr->last_heading=the_pb->rot_z;
	current_object_ptr->wanted_roll=0;	//calc'd from current and last

	current_object_ptr->mass=the_pb->mass;
	current_object_ptr->dq=the_pb->dq;
//	current_object_ptr->thrust_x=the_pb->Omovedx;
//	current_object_ptr->thrust_y=the_pb->Omovedy;
	current_object_ptr->extern_movedx=the_pb->Omovedx;
	current_object_ptr->extern_movedy=the_pb->Omovedy;
	current_object_ptr->extern_rotdz=0;
	current_object_ptr->hit_by_slot=-1;

	current_object_ptr->just_hit=0;
	current_object_ptr->no_trivial_rejection=the_pb->no_trivial_rejection;
	current_object_ptr->object_category=the_pb->object_category;

	current_object_ptr->laser_category=the_pb->laser_cat;
	current_object_ptr->cannon_category=the_pb->cannon_cat;
	current_object_ptr->missile_category=the_pb->missile_cat;
	current_object_ptr->number_of_missiles=the_pb->number_of_missiles;
	current_object_ptr->is_armed=1;
	current_object_ptr->who_spawned_this_object=the_pb->who_spawned_this_object;

	current_object_ptr->polys_ptr=new_polys_address;
	current_object_ptr->active_polys_slot=poly_slot;	//so we can quickly kill these polys
//        current_object_ptr->controller_disabled_count = 0;	   //set in coll_detect on coll_detect as it reverses thurst vector

	current_object_ptr->anim_frame_rate=frame_rate;
	current_object_ptr->anim_control=control;
	current_object_ptr->anim_state=state;

	current_object_ptr->anim_counter=0;
	current_object_ptr->anim_frame=0;
	current_object_ptr->anim_max_object=number_of_objects;
	current_object_ptr->the_anim_ptrs[0]=temp_anim_ptrs[0];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[1]=temp_anim_ptrs[1];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[2]=temp_anim_ptrs[2];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[3]=temp_anim_ptrs[3];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[4]=temp_anim_ptrs[4];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[5]=temp_anim_ptrs[5];	//store the new ptrs in the anim block


	dynamic_object_index++;	//we've added an object that needs to be processed
	
	}	


}


//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//this is called to load an object into the dynamic arrays
void load_dyn_object(int static_slot,int dest_ocb,load_dyn_object_pb *the_pb, int mothership_galactic_slot)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index;	//how many static objects
int poly_slot,size_of_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
register ZObject * current_object_ptr;

if (dest_ocb!=-1)
	{
     current_object_ptr=&ocb_ptr->object_list[dest_ocb];

	(*ocb_ptr).object_list[dest_ocb]=(*ocb_Sptr).object_list[static_slot];	//copy the object
	//fill in callsign
	current_object_ptr->in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	current_object_ptr->Oworldx=the_pb->world_x;
	current_object_ptr->Oworldy=the_pb->world_y;
	current_object_ptr->Oworldz=the_pb->world_z;
	current_object_ptr->Ostartx=the_pb->world_x;
	current_object_ptr->Ostarty=the_pb->world_y;

	current_object_ptr->Orotdx=the_pb->delta_rot_x;
	current_object_ptr->Orotdy=the_pb->delta_rot_y;
	current_object_ptr->Orotdz=the_pb->delta_rot_z;
	current_object_ptr->Orotx=the_pb->rot_x;
	current_object_ptr->Oroty=the_pb->rot_y;
	current_object_ptr->Orotz=the_pb->rot_z;
	current_object_ptr->Omovedx=the_pb->Omovedx;
	current_object_ptr->Omovedy=the_pb->Omovedy;
	current_object_ptr->Omovedz=the_pb->Omovedz;
	current_object_ptr->controller=the_pb->controller_ref;
	current_object_ptr->control_use_1=the_pb->control_use_1;
	current_object_ptr->control_use_2=the_pb->control_use_2;
	current_object_ptr->control_use_3=the_pb->control_use_3;
	current_object_ptr->control_use_4=the_pb->control_use_4;
	current_object_ptr->current_velocity=the_pb->current_velocity;
	current_object_ptr->wanted_velocity=the_pb->wanted_velocity;
	current_object_ptr->control_use_7=the_pb->control_use_7;
	current_object_ptr->control_use_8=0;
	current_object_ptr->control_use_9=0;
	current_object_ptr->control_use_10=0;
	current_object_ptr->cu11=-1;
	current_object_ptr->cu12=-1;
	current_object_ptr->cu13=-1;

//stuff for NPC_Control
	current_object_ptr->going_to=the_pb->going_to;
	current_object_ptr->aggressive=the_pb->aggressive;
	current_object_ptr->obj_class=the_pb->obj_class;
	current_object_ptr->sub_controller=the_pb->sub_controller;
	current_object_ptr->shields_class=the_pb->shields_class;
	current_object_ptr->max_vel=the_pb->max_vel;
	current_object_ptr->min_vel=the_pb->min_vel;
	current_object_ptr->go_home_time=the_pb->go_home_time;
	current_object_ptr->dynamic_tex_ptr=0;
	current_object_ptr->time_out=1;
	current_object_ptr->rank=0;

	current_object_ptr->command=0x8000;
	current_object_ptr->current_mode=1;
	
//mothership specifc
	current_object_ptr->mship_ptr=0;	//very important!
	current_object_ptr->galactic_id=mothership_galactic_slot;
	current_object_ptr->docking_what=-1;	//very important!

	
	current_object_ptr->swerve_direction=0;
	current_object_ptr->invincible_count=0;


	current_object_ptr->attack_mode=the_pb->attack_mode;

//	(*ocb_ptr).object_list[dest_ocb].object_normal=the_pb->normal;
	current_object_ptr->object_normal.x=1;
	current_object_ptr->object_normal.y=0;
	current_object_ptr->object_normal.z=0;

	current_object_ptr->shield_value=the_pb->shield_value;
//next 4  130499
	current_object_ptr->original_shield_value=the_pb->shield_value;
//    current_object_ptr->tractor=0;
    current_object_ptr->docked_object_slot=-1;	   //if -1, not docked
	current_object_ptr->has_roll=the_pb->has_roll;
	current_object_ptr->last_heading=the_pb->rot_z;
	current_object_ptr->wanted_roll=0;	//calc'd from current and last
	  
	current_object_ptr->mass=the_pb->mass;
	current_object_ptr->dq=the_pb->dq;
//	(*ocb_ptr).object_list[dest_ocb].thrust_x=the_pb->Omovedx;
//	(*ocb_ptr).object_list[dest_ocb].thrust_y=the_pb->Omovedy;
	current_object_ptr->extern_movedx=the_pb->Omovedx;
	current_object_ptr->extern_movedy=the_pb->Omovedy;
	current_object_ptr->extern_rotdz=0;

	current_object_ptr->who_spawned_this_object=the_pb->who_spawned_this_object;
	current_object_ptr->hit_by_slot=-1;

	current_object_ptr->just_hit=0;
	current_object_ptr->no_trivial_rejection=the_pb->no_trivial_rejection;
	current_object_ptr->object_category=the_pb->object_category;
	current_object_ptr->laser_category=the_pb->laser_cat;
	current_object_ptr->cannon_category=the_pb->cannon_cat;
	current_object_ptr->missile_category=the_pb->missile_cat;
	current_object_ptr->number_of_missiles=the_pb->number_of_missiles;
	current_object_ptr->unique_id=the_pb->unique_id;
	current_object_ptr->is_armed=1;

	current_object_ptr->anim_frame_rate=0;
	current_object_ptr->anim_counter=0;
	current_object_ptr->anim_frame=0;
	current_object_ptr->anim_max_object=1;	   //not animated

//	gen_callsign(dest_ocb);

//        (*ocb_ptr).object_list[dest_ocb].controller_disabled_count = 0;	//set in coll_detect on coll_detect as it reverses thurst vector

//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
	{
	dynamic_polys_ptr[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	//copy the polys across
	new_polys_address=&dynamic_polys_ptr[poly_slot].the_obj_polys[0];	//so we can poke the new object
	old_polys_address=(*ocb_ptr).object_list[dest_ocb].polys_ptr;
	
	size_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
	size_of_polys=size_of_polys*sizeof(_ZPoly);
	
	BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	//poke the address of the polys into the new object
	(*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	(*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
	
	}	
	else	//oops, couldn't get any polys! (Found on intel port)
	 {
		add_to_text_display("Zex Exec> WARNING: load_dyn_object failed to find poly_slot",1);;
		(*ocb_ptr).object_list[dest_ocb].in_use=0;
		return; // -1;
	 }
	dynamic_object_index++;	//we've added an object that needs to be processed
	}
}




//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//this is called to replace an object into the dynamic arrays
//dont use on motherships!!!!!!!!
//SB 180799

void replace_dyn_object(int static_slot,int dest_ocb)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index;	//how many static objects
int poly_slot,size_of_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
load_dyn_object_pb the_pb;

if (dest_ocb!=-1)
	{

//copy old attribs
	the_pb.world_x=(*ocb_ptr).object_list[dest_ocb].Oworldx;
	the_pb.world_y=(*ocb_ptr).object_list[dest_ocb].Oworldy;
	the_pb.world_z=(*ocb_ptr).object_list[dest_ocb].Oworldz;
	the_pb.delta_rot_x=(*ocb_ptr).object_list[dest_ocb].Orotdx;
	the_pb.delta_rot_y=(*ocb_ptr).object_list[dest_ocb].Orotdy;
	the_pb.delta_rot_z=(*ocb_ptr).object_list[dest_ocb].Orotdz;
	the_pb.rot_x=(*ocb_ptr).object_list[dest_ocb].Orotx;
	the_pb.rot_y=(*ocb_ptr).object_list[dest_ocb].Oroty;
	the_pb.rot_z=(*ocb_ptr).object_list[dest_ocb].Orotz;
	the_pb.Omovedx=(*ocb_ptr).object_list[dest_ocb].Omovedx;
	the_pb.Omovedy=(*ocb_ptr).object_list[dest_ocb].Omovedy;
	the_pb.Omovedz=(*ocb_ptr).object_list[dest_ocb].Omovedz;
	the_pb.controller_ref=(*ocb_ptr).object_list[dest_ocb].controller;
	the_pb.control_use_1=(*ocb_ptr).object_list[dest_ocb].control_use_1;
	the_pb.control_use_2=(*ocb_ptr).object_list[dest_ocb].control_use_2;
	the_pb.control_use_3=(*ocb_ptr).object_list[dest_ocb].control_use_3;
	the_pb.control_use_4=(*ocb_ptr).object_list[dest_ocb].control_use_4;
	the_pb.current_velocity=(*ocb_ptr).object_list[dest_ocb].current_velocity;
	the_pb.wanted_velocity=(*ocb_ptr).object_list[dest_ocb].wanted_velocity;
	the_pb.control_use_7=(*ocb_ptr).object_list[dest_ocb].control_use_7;
	the_pb.attack_mode=(*ocb_ptr).object_list[dest_ocb].attack_mode;

	the_pb.shield_value=(*ocb_ptr).object_list[dest_ocb].shield_value;
//next 4  130499
	the_pb.shield_value=(*ocb_ptr).object_list[dest_ocb].original_shield_value;
	the_pb.has_roll=(*ocb_ptr).object_list[dest_ocb].has_roll;
//	the_pb.rot_z=(*ocb_ptr).object_list[dest_ocb].last_heading;
	  
	the_pb.mass=(*ocb_ptr).object_list[dest_ocb].mass;
	the_pb.dq=(*ocb_ptr).object_list[dest_ocb].dq;
	the_pb.Omovedx=(*ocb_ptr).object_list[dest_ocb].extern_movedx;
	the_pb.Omovedy=(*ocb_ptr).object_list[dest_ocb].extern_movedy;

	the_pb.who_spawned_this_object=(*ocb_ptr).object_list[dest_ocb].who_spawned_this_object;

	the_pb.no_trivial_rejection=(*ocb_ptr).object_list[dest_ocb].no_trivial_rejection;
	the_pb.object_category=(*ocb_ptr).object_list[dest_ocb].object_category;
	the_pb.laser_cat=(*ocb_ptr).object_list[dest_ocb].laser_category;
	the_pb.cannon_cat=(*ocb_ptr).object_list[dest_ocb].cannon_category;
	the_pb.missile_cat=(*ocb_ptr).object_list[dest_ocb].missile_category;
	the_pb.number_of_missiles=(*ocb_ptr).object_list[dest_ocb].number_of_missiles;
	the_pb.unique_id=(*ocb_ptr).object_list[dest_ocb].unique_id;
	
	kill(dest_ocb);
//insert new blank object
	(*ocb_ptr).object_list[dest_ocb]=(*ocb_Sptr).object_list[static_slot];	//copy the object
	(*ocb_ptr).object_list[dest_ocb].in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].is_armed=1;

	(*ocb_ptr).object_list[dest_ocb].anim_frame_rate=0;
	(*ocb_ptr).object_list[dest_ocb].anim_counter=0;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=1;	//not animated
	(*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;
	(*ocb_ptr).object_list[dest_ocb].just_hit=0;
	(*ocb_ptr).object_list[dest_ocb].extern_rotdz=0;
	(*ocb_ptr).object_list[dest_ocb].wanted_roll=0;	//calc'd from current and last
//    (*ocb_ptr).object_list[dest_ocb].tractor=0;
    (*ocb_ptr).object_list[dest_ocb].docked_object_slot=-1;	//if -1, not docked
	(*ocb_ptr).object_list[dest_ocb].object_normal.x=1;
	(*ocb_ptr).object_list[dest_ocb].object_normal.y=0;
	(*ocb_ptr).object_list[dest_ocb].object_normal.z=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_8=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_9=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_10=0;
	(*ocb_ptr).object_list[dest_ocb].swerve_direction=0;
	(*ocb_ptr).object_list[dest_ocb].invincible_count=0;
	(*ocb_ptr).object_list[dest_ocb].callsign=0;
//mothership specific - don't use this routine to replace a mothership!
	(*ocb_ptr).object_list[dest_ocb].mship_ptr=0;



//set new attribs
	(*ocb_ptr).object_list[dest_ocb].Oworldx=the_pb.world_x;
	(*ocb_ptr).object_list[dest_ocb].Oworldy=the_pb.world_y;
	(*ocb_ptr).object_list[dest_ocb].Oworldz=the_pb.world_z;
	(*ocb_ptr).object_list[dest_ocb].Orotdx=the_pb.delta_rot_x;
	(*ocb_ptr).object_list[dest_ocb].Orotdy=the_pb.delta_rot_y;
	(*ocb_ptr).object_list[dest_ocb].Orotdz=the_pb.delta_rot_z;
	(*ocb_ptr).object_list[dest_ocb].Orotx=the_pb.rot_x;
	(*ocb_ptr).object_list[dest_ocb].Oroty=the_pb.rot_y;
	(*ocb_ptr).object_list[dest_ocb].Orotz=the_pb.rot_z;
	(*ocb_ptr).object_list[dest_ocb].Omovedx=the_pb.Omovedx;
	(*ocb_ptr).object_list[dest_ocb].Omovedy=the_pb.Omovedy;
	(*ocb_ptr).object_list[dest_ocb].Omovedz=the_pb.Omovedz;
	(*ocb_ptr).object_list[dest_ocb].controller=the_pb.controller_ref;
	(*ocb_ptr).object_list[dest_ocb].control_use_1=the_pb.control_use_1;
	(*ocb_ptr).object_list[dest_ocb].control_use_2=the_pb.control_use_2;
	(*ocb_ptr).object_list[dest_ocb].control_use_3=the_pb.control_use_3;
	(*ocb_ptr).object_list[dest_ocb].control_use_4=the_pb.control_use_4;
	(*ocb_ptr).object_list[dest_ocb].current_velocity=the_pb.current_velocity;
	(*ocb_ptr).object_list[dest_ocb].wanted_velocity=the_pb.wanted_velocity;
	(*ocb_ptr).object_list[dest_ocb].control_use_7=the_pb.control_use_7;
	(*ocb_ptr).object_list[dest_ocb].control_use_8=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_9=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_10=0;
	(*ocb_ptr).object_list[dest_ocb].swerve_direction=0;
	(*ocb_ptr).object_list[dest_ocb].invincible_count=0;
	(*ocb_ptr).object_list[dest_ocb].callsign=0;
	(*ocb_ptr).object_list[dest_ocb].attack_mode=the_pb.attack_mode;

//	(*ocb_ptr).object_list[dest_ocb].object_normal=the_pb->normal;
	(*ocb_ptr).object_list[dest_ocb].object_normal.x=1;
	(*ocb_ptr).object_list[dest_ocb].object_normal.y=0;
	(*ocb_ptr).object_list[dest_ocb].object_normal.z=0;

	(*ocb_ptr).object_list[dest_ocb].shield_value=the_pb.shield_value;
//next 4  130499
	(*ocb_ptr).object_list[dest_ocb].original_shield_value=the_pb.shield_value;
//    (*ocb_ptr).object_list[dest_ocb].tractor=0;
    (*ocb_ptr).object_list[dest_ocb].docked_object_slot=-1;	//if -1, not docked
	(*ocb_ptr).object_list[dest_ocb].has_roll=the_pb.has_roll;
	(*ocb_ptr).object_list[dest_ocb].last_heading=the_pb.rot_z;
	(*ocb_ptr).object_list[dest_ocb].wanted_roll=0;	//calc'd from current and last
	  
	(*ocb_ptr).object_list[dest_ocb].mass=the_pb.mass;
	(*ocb_ptr).object_list[dest_ocb].dq=the_pb.dq;
	(*ocb_ptr).object_list[dest_ocb].extern_movedx=the_pb.Omovedx;
	(*ocb_ptr).object_list[dest_ocb].extern_movedy=the_pb.Omovedy;
	(*ocb_ptr).object_list[dest_ocb].extern_rotdz=0;

	(*ocb_ptr).object_list[dest_ocb].who_spawned_this_object=the_pb.who_spawned_this_object;
	(*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;

	(*ocb_ptr).object_list[dest_ocb].just_hit=0;
	(*ocb_ptr).object_list[dest_ocb].no_trivial_rejection=the_pb.no_trivial_rejection;
	(*ocb_ptr).object_list[dest_ocb].object_category=the_pb.object_category;
	(*ocb_ptr).object_list[dest_ocb].laser_category=the_pb.laser_cat;
	(*ocb_ptr).object_list[dest_ocb].cannon_category=the_pb.cannon_cat;
	(*ocb_ptr).object_list[dest_ocb].missile_category=the_pb.missile_cat;
	(*ocb_ptr).object_list[dest_ocb].number_of_missiles=the_pb.number_of_missiles;
	(*ocb_ptr).object_list[dest_ocb].unique_id=the_pb.unique_id;
	(*ocb_ptr).object_list[dest_ocb].is_armed=1;

	(*ocb_ptr).object_list[dest_ocb].anim_frame_rate=0;
	(*ocb_ptr).object_list[dest_ocb].anim_counter=0;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=1;	//not animated


//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
	{
	dynamic_polys_ptr[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	//copy the polys across
	new_polys_address=&dynamic_polys_ptr[poly_slot].the_obj_polys[0];	//so we can poke the new object
	old_polys_address=(*ocb_ptr).object_list[dest_ocb].polys_ptr;
	
	size_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
	size_of_polys=size_of_polys*sizeof(_ZPoly);
	
	BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	//poke the address of the polys into the new object
	(*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	(*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
	
	}	
	else	//oops, couldn't get any polys! (Found on intel port)
	 {
		add_to_text_display("Zex Exec> WARNING: replace_dyn_object failed to find poly_slot",1);
		(*ocb_ptr).object_list[dest_ocb].in_use=0;
		return; // -1;
	 }
	dynamic_object_index++;	//we've added an object that needs to be processed
	}
}



void kill(int the_object)	//this should be called kill_obj, but this way I can say "kill(it)"
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index,training_mission,tutorial;
register ZObject * current_object_ptr;

int polys_slot,i;

//mod 210599 - SB
//we check if the object is alive before killing it to prevent dynamic_object-index being erroneously
//decremented.
//if ((*ocb_ptr).object_list[the_object].in_use!=0)
// {
	
	current_object_ptr=&ocb_ptr->object_list[the_object];
	//if (current_object_ptr->in_use==0) return;	//lets not 'Oops'
	
	current_object_ptr->in_use=0;	//deaded
	polys_slot=current_object_ptr->active_polys_slot;	 //point to the polys
	dynamic_polys_ptr[polys_slot].in_use=0;	//this poly slot is not in us
//if dynamic texture, release the memory
    if (current_object_ptr->dynamic_tex_ptr!=0)
       kill_dynamic_texture(current_object_ptr->dynamic_tex_ptr);

//if animatd then kill all the poly slots

    if (current_object_ptr->anim_max_object>1)
    {
    for (i=0;i<current_object_ptr->anim_max_object;i++)
     {
     dynamic_polys_ptr[current_object_ptr->the_anim_ptrs[i].polys_slot].in_use=0;
     }
    }
//if mothership ptr is set then dealoc the mem (training only!)
        if (current_object_ptr->mship_ptr!=0 && (training_mission!=0 || tutorial!=0)) 
             {

              mship_release_mem(current_object_ptr->mship_ptr);	//if sim, release the mem else do nothing

//              if (mship_release_mem(current_object_ptr->mship_ptr)!=0)	//if sim, release the mem else do nothing
//                    report_error ("Kill: Couldn't release mothership memory frag.","",-4);
             }
	if (dynamic_object_index>0)
	dynamic_object_index--;
// }
}

int find_vacant_dyn_poly_slot()	//returns an index into the dynamic_polys_array or -1 if we have none
{
extern _ZPoly_objs *dynamic_polys_ptr;
int i;

for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
	if (dynamic_polys_ptr[i].in_use==0) 
	{

			return i;
	}
return -1;
}

int find_vacant_dynamic_object_slot()	//returns an index into the dynamic_objects_array
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
int i;

for (i=1;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
	if ((*ocb_ptr).object_list[i].in_use==0) 
	{
		if (i==386)
		{
			int a=5;	//debugging
		}
		return i;
	}
add_to_text_display("Zex Exec> WARNING: find_vacant_dynamic_object_slot failed!",1);	
return -1;
}

void reset_objects()
{
extern int connection_list_index, points_list_index, world_points_list_index;
extern int dynamic_object_index, poly_list_index;
extern _3D light_normal;

connection_list_index=points_list_index=world_points_list_index=0;
dynamic_object_index=poly_list_index=0;
init_light();
clear_out_dyn();	//reset all objects to not in use
}

int load_object(const char* filename, int static_slot, unsigned long id,int colldetect, unsigned long texture_id, int translucent)
{
FSSpec the_file;
Handle the_file_data_H;
int Err,get_file_err;
extern FSSpec Zex_FSSpec;
FSSpec objects_folder_FSSpec;
//FSSpec the_object_FSSpec;

//CInfoPBRec pb;
//DirInfo *dpb=(DirInfo*) &pb;
//HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
int folder_DirID,return_value;
Str255 the_file_name;


////get the objects' folder
//	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"Objects",&objects_folder_FSSpec);
////get the folders dirctory ID
//     dpb->ioVRefNum = objects_folder_FSSpec.vRefNum;
//     dpb->ioDrDirID = objects_folder_FSSpec.parID;
//     dpb->ioNamePtr = objects_folder_FSSpec.name;
//     dpb->ioFDirIndex=0;	  //query
//     return_value=PBGetCatInfo(&pb,0);
//     if (return_value) 
//     {
//     report_error ("load_object couldn't read a file:",filename,return_value);
//     return return_value;
//     }
//     folder_DirID=dpb->ioDrDirID;
////now get all the file from the folder
//return_value=0;
//
//
//      fpb->ioVRefNum = objects_folder_FSSpec.vRefNum;
//     fpb->ioDirID = folder_DirID;
//     fpb->ioNamePtr = filename;
//     fpb->ioFDirIndex=0;	  //query the file name
//     get_file_err=PBGetCatInfo(&pb,0);
//     if (get_file_err) report_error ("load_object couldn't read a file:",filename,get_file_err);
//     if(get_file_err) return return_value;
     make_data_fsspec("O",filename,&the_file);

//now load it
//     FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,filename,&the_file);

//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,filename,&the_file);
	the_file_data_H=read_file(the_file);
	Err=MeshToZexDyn(*the_file_data_H,static_slot,id,colldetect,texture_id, translucent);	//stat slots 0
	LS_DisposeHandle(the_file_data_H);
return Err;
}

int can_gen_dust=1;
int in_ramparian_space;

void gen_dust()	//gen a star in front of zex
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z,camera_distance,distance_from_cam;
float screen_x,screen_y,world_x,world_y,z;
load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
if (global_show_status!=0) return;
if (can_gen_dust==0) return;

clear_pb(&the_params);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in star
    number_of_stars++;

	camera_z=static_cast<int>(camera_pos.z);
//	the_params.world_z=RangedRdm(0,8000)-3300;	//dustPos[i].z;
        distance_from_cam=RangedRdm(3500,16000);
        the_params.world_z=distance_from_cam+camera_z;	//dustPos[i].z;
      
//	the_params.world_z=RangedRdm(0,ABS(camera_z))+camera_z+4000;	//dustPos[i].z;
	camera_distance=distance_from_cam;
	

	the_params.Omovedx=RangedRdm(0,2)-2;	//vicious

#define COMPLEX_DUST 6000
if (camera_distance<COMPLEX_DUST)
{
	the_params.delta_rot_y=RangedRdm(0,20)-10;
	the_params.delta_rot_z=RangedRdm(0,20)-10;
}


	the_params.rot_z=RangedRdm(0,359);
	
	the_params.controller_ref=dust;
	the_params.object_category=scenary_object;
		
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
     the_params.who_spawned_this_object=-1;
	 the_params.unique_id=0;
	the_params.number_of_missiles=0;
//	if(RangedRdm(0,4)>2)
        if (camera_distance<COMPLEX_DUST)	//multiple level of detail (wow!)
        {
         load_dyn_object(30,it,&the_params,-1);	//dustOGL = the object, the position (-1=next free)       
        }
        else
        {
        if (in_ramparian_space==0)
	load_dyn_object(dust,it,&the_params,-1);	//the object, the position (-1=next free)
	else
	load_dyn_object(dust2,it,&the_params,-1);	//rmparian
	}
//	else load_dyn_object(dust,it,&the_params);
//calculate the viewable line at this z - i.e.we want world x and y given screen x and y
  screen_x=(*ocb_ptr).object_list[zex].object_normal_translated.x*640;	//+-320  
  screen_y=(*ocb_ptr).object_list[zex].object_normal_translated.y*320;	//+-240

  //offset in y by random
  screen_x+=(*ocb_ptr).object_list[zex].object_normal_translated.y*(RangedRdm(0,640)-320);
  screen_y+=(*ocb_ptr).object_list[zex].object_normal_translated.x*(RangedRdm(0,480)-240);
  
  //camera_z is always -ve, get abs distance between camera and dust z
  if (the_params.world_z>0)
  {
   z=the_params.world_z-camera_z;
  }
  else
  z=-camera_z+the_params.world_z;
  
//screen_x=world_x/z
//world_x=screen_x*z
  world_x=screen_x*(z/200);
  world_y=screen_y*(z/200);
  

//now set up position
  if (thrust>=0)
  {
//move dust to in-front of us
  (*ocb_ptr).object_list[it].Oworldx=(*ocb_ptr).object_list[0].Oworldx+world_x;
  (*ocb_ptr).object_list[it].Oworldy=(*ocb_ptr).object_list[0].Oworldy+world_y;
  }
  else	//thrust<0
  {
  (*ocb_ptr).object_list[it].Oworldx=(*ocb_ptr).object_list[0].Oworldx-world_x;
  (*ocb_ptr).object_list[it].Oworldy=(*ocb_ptr).object_list[0].Oworldy-world_y;
  }

 }
}


void kill_all_dust()
{
int i;

for (i=1;i<MAX_NUMBER_OF_DYN_OBJECTS-1;i++)
   {
    if ((*ocb_ptr).object_list[i].in_use==1 && (*ocb_ptr).object_list[i].controller==dust) 
     {
     kill(i);
     }    
   }
number_of_stars=0;

}

void gen_dust_for_end()	//gen a star in front of camera
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status!=0) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in star
	the_params.world_x=(-camera_pos.x)+(RangedRdm(0,12000)-6000);	//dustPos[i].x;
	the_params.world_y=(-camera_pos.y)+(RangedRdm(0,12000)-6000);	//dustPos[i].y;
//	the_params.world_z=RangedRdm(0,8000)-3300;	//dustPos[i].z;
	the_params.world_z= camera_pos.z+400;	//dustPos[i].z;
//	the_params.Omovedx=-RangedRdm(1,22);	//vicious
	the_params.Omovedx=0;	//vicious
	the_params.Omovedy=0;
	the_params.Omovedz=RangedRdm(5,50);
	the_params.delta_rot_x=RangedRdm(5,20);
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=RangedRdm(5,20);
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=RangedRdm(0,359);
	
	the_params.controller_ref=dust_for_end;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
     the_params.has_roll=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	the_params.attack_mode=0;

	the_params.shield_value=0;
	the_params.mass=0;
		 	 the_params.dq=0;
	the_params.object_category=scenary_object;
		
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	 the_params.number_of_missiles=0;
//	if(RangedRdm(0,4)>2)
	load_dyn_object(dust,it,&the_params,-1);	//the object, the position (-1=next free)
//	else load_dyn_object(dust,it,&the_params);


 }
}



void gen_dust_for_warphole()	//gen a star in front of camera
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status!=0) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in star
	the_params.world_x=(-camera_pos.x)+(RangedRdm(0,12000)-6000);	//dustPos[i].x;
	the_params.world_y=(-camera_pos.y)+(RangedRdm(0,12000)-6000);	//dustPos[i].y;
//	the_params.world_z=RangedRdm(0,8000)-3300;	//dustPos[i].z;
	the_params.world_z= camera_pos.z+400;	//dustPos[i].z;
//	the_params.Omovedx=-RangedRdm(1,22);	//vicious
	the_params.Omovedx=0;	//vicious
	the_params.Omovedy=0;
	the_params.Omovedz=RangedRdm(25,90);
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=RangedRdm(0,359);
	
	the_params.controller_ref=DUST_FOR_HOLE;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
     the_params.has_roll=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	the_params.attack_mode=0;

	the_params.shield_value=0;
	the_params.mass=0;
		 	 the_params.dq=0;
	the_params.object_category=scenary_object;
		
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	 the_params.number_of_missiles=0;
//	if(RangedRdm(0,4)>2)
	load_dyn_object(dust,it,&the_params,-1);	//the object, the position (-1=next free)
//	else load_dyn_object(dust,it,&the_params);


 }
}


void populate_dust_for_start()
{
int i;
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z,camera_distance,distance_from_cam;
float screen_x,screen_y,world_x,world_y,z;
load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;



for (i=0;i<70;i++)
     {
     
clear_pb(&the_params);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in star
    number_of_stars++;

	camera_z=static_cast<int>(camera_pos.z);
//	the_params.world_z=RangedRdm(0,8000)-3300;	//dustPos[i].z;
        distance_from_cam=RangedRdm(3500,16000);
        the_params.world_z=distance_from_cam+camera_z;	//dustPos[i].z;
      
//	the_params.world_z=RangedRdm(0,ABS(camera_z))+camera_z+4000;	//dustPos[i].z;
	camera_distance=distance_from_cam;
	

	the_params.Omovedx=RangedRdm(0,2)-2;	//vicious

#define COMPLEX_DUST 6000
if (camera_distance<COMPLEX_DUST)
{
	the_params.delta_rot_y=RangedRdm(0,20)-10;
	the_params.delta_rot_z=RangedRdm(0,20)-10;
}


	the_params.rot_z=RangedRdm(0,359);
	
	the_params.controller_ref=dust;
	the_params.object_category=scenary_object;
		
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
     the_params.who_spawned_this_object=-1;
	 the_params.unique_id=0;
	the_params.number_of_missiles=0;
//	if(RangedRdm(0,4)>2)
        if (camera_distance<COMPLEX_DUST)	//multiple level of detail (wow!)
        {
         load_dyn_object(30,it,&the_params,-1);	//dustOGL = the object, the position (-1=next free)       
        }
        else
        {
        if (in_ramparian_space==0)
	load_dyn_object(dust,it,&the_params,-1);	//the object, the position (-1=next free)
	else
	load_dyn_object(dust2,it,&the_params,-1);	//rmparian
	}

  
  //camera_z is always -ve, get abs distance between camera and dust z
  if (the_params.world_z>0)
  {
   z=the_params.world_z-camera_z;
  }
  else
  z=-camera_z+the_params.world_z;
  

  

//now set up position

  (*ocb_ptr).object_list[it].Oworldx=(*ocb_ptr).object_list[0].Oworldx+RangedRdm(0,30000)-15000;
  (*ocb_ptr).object_list[it].Oworldy=(*ocb_ptr).object_list[0].Oworldy+RangedRdm(0,30000)-15000;

 }

     
     }	//end of for i
}

