// *************************************MODULE HEADER*********************************
// * FILENAME           : load_complex_object.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 00:29:42 on 26th September 1999
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

#include <stdlib.h>		// for atof()
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
#include "load_complex_object.h"

#include <limits>
using namespace std;

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
void interpret_object_file(Ptr the_file_data, int x, int y);




int load_complex_object(const char* filename, int x, int y)
{
FSSpec the_file;
Handle the_file_data_H;
Ptr the_object_file;

    make_data_fsspec("L",filename,&the_file);	//folder, name, fsspec here
   
	the_file_data_H=read_file(the_file);
    LS_HLock(the_file_data_H);
    the_object_file=*the_file_data_H;
    interpret_object_file(the_object_file,x,y);	//load the objects relative to x and y
	LS_DisposeHandle(the_file_data_H);
return 0;
}



void interpret_object_file(Ptr the_file_data, int x, int y)
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
//oldmac//decimal my_dec;
short ix;
short vp;
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



//now process level file
source=the_file_data;	//source is incremented through the file

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
//¥¥¥¥¥¥¥¥¥¥¥¥command processing, all cut 'n' pasted - no apologies.
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


         
       case 'FUEL':	//a refuel rectangle
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: FUEL: Not enough parameters.","Expected top member of rect",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.top= static_cast<short int>(dectof_result);

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: FUEL: Not enough parameters.","Expecting left member of rect",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.left= static_cast<short int>(dectof_result);

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: FUEL: Not enough parameters.","Expecting bottom member of rect",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.bottom= static_cast<short int>(dectof_result);

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: FUEL: Not enough parameters.","Expecting right member of rect",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            fuel.right= static_cast<short int>(dectof_result);
            add_refuel_rect(fuel);
            break;
          }


       case 'MONF':	//monsters can fire - 1 they do, 0 they dont
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			  dectof_result = atof(the_word);
			  //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
            (*ocb_ptr).object_list[zex].Oworldx=dectof_result;
            monsters_can_fire= static_cast<int>(dectof_result);	//set camera too
             break;
          }


//RFIL NOT ALLOWED IN AN OBJECT FILE BUT LEFT JUST IN CASE
       case 'RFIL':	//load an object int ostatic
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
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            static_slot= static_cast<int>(dectof_result);
            //get colldet
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            colldetect= static_cast<int>(dectof_result);	//0-3
            //get texture ID
            the_word[0]=0;the_word[1]=0;the_word[2]=0;the_word[3]=0;
            fname_length=get_word(the_word,&the_line[word_len]);
            word_len+=fname_length;

            texture_id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
            if (the_word[0]==0 || the_word[1]==0 || the_word[2]==0 || the_word[3]==0 || the_word[4]!=0) 
            report_error_c ("load_complex_object: RFIL: Bad texture ID in object.","(Must be 4 chars or 0000). Line number below.",line_number);
            if (texture_id=='0000') texture_id=0;
            if (static_slot<20) report_error_c("load_complex_object: RFIL: Can't read object into slots below 20 (Reserved slots)","",100);
            load_object(filename,static_slot,id,colldetect,texture_id,IS_NOT_TRANSLUCENT);
            break;
          }
          
          
       case 'LMON':	//insert object to dyn ocbs
          {
          //read object id
            word_len+=get_word(the_word,&the_line[word_len]);
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting Object ID code",line_number);

            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
          //find id
             static_slot=get_static_object_slot(id);	//go fetch fido (the object)
             if (static_slot==-1) report_error_id ("load_complex_object: Can't find object. ID is: ",id);
          //get world x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting start pos x",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_x=dectof_result+x;	//add on gmap coords

          //get world y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting start pos y",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_y=dectof_result+y;	//add on map y
         
          //get world z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting start pos z",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_z=dectof_result;

          //get movement delta x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting delta x",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedx=dectof_result;

          //get movement delta y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting delta y",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedy=dectof_result;
          //get movement delta z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting delta z",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedz=dectof_result;

          //get delta rot x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting delta rot x",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_x= static_cast<int>(dectof_result);

          //get delta rot y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting delta rot y",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_y= static_cast<int>(dectof_result);

          //get delta rot z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting delta rot z",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_z= static_cast<int>(dectof_result);

          //get control_use_1
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting Control Use 1 Value",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_1= static_cast<int>(dectof_result);


          //get control_use_2
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting Control Use 2 Value",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_2= static_cast<int>(dectof_result);

          //get control_use_3
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting Control Use 3 value",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_3= static_cast<int>(dectof_result);

          //get controller ref
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting Controller reference number",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.controller_ref= static_cast<int>(dectof_result);
          
          //get shields
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting shield value",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.shield_value= static_cast<short int>(dectof_result);

          //get mass
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting Mass",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.mass= static_cast<short int>(dectof_result);
            if (dectof_result<0) report_error_c("load_complex_object: LMON: Negative mass not allowed.","Line number below.",line_number);

          //get category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting object category",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.object_category=temp_int;

          //get trivial rejection
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting No trivial rejection",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.no_trivial_rejection=temp_int;

          //get number of missiles
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting number of missiles",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.number_of_missiles=temp_int;
          //get laser category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting laser category",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.laser_cat=temp_int;

          //get has roll
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting has roll",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.has_roll=temp_int;

          //get unique ID
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting unique ID",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	      //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.unique_id=temp_int;

          //get zrot
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting zrot",line_number);
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

      if (dest_ocb==-1) report_error_id ("load_complex_object: Cant get dynamic slot for LMON",2);

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
            if (the_word[0]==0) report_error_c("load_complex_object: LMON: Not enough parameters.","Expecting Object ID code",line_number);

            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
             if (id!='0000') 
             {
              frames_counter++;
              anim.object_id[i]=id;
          //find id
              static_slot=get_static_object_slot(id);	//go fetch fido (the object)
              if (static_slot==-1) report_error_id ("load_complex_object: LAMN: Can't find object. ID is: ",id);
             }
             
           }

          //get frame rate
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting anim frame rate",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        frame_rate= static_cast<int>(dectof_result);

          //get animation control
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting anim control",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        anim_control= static_cast<int>(dectof_result);

          //get animation state
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting anim state",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        anim_state= static_cast<int>(dectof_result);
          
          //get world x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting start pos x",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_x=dectof_result+x;

          //get world y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting start pos y",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_y=dectof_result+y;
         
          //get world z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting start pos z",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.world_z=dectof_result;

          //get movement delta x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting delta x",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedx=dectof_result;

          //get movement delta y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting delta y",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedy=dectof_result;
          //get movement delta z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting delta z",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.Omovedz=dectof_result;

          //get delta rot x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting delta rot x",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_x= static_cast<int>(dectof_result);

          //get delta rot y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting delta rot y",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_y= static_cast<int>(dectof_result);

          //get delta rot z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting delta rot z",line_number);
            //oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.delta_rot_z= static_cast<int>(dectof_result);

          //get control_use_1
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting Control Use 1 Value",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_1= static_cast<int>(dectof_result);


          //get control_use_2
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting Control Use 2 Value",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_2= static_cast<int>(dectof_result);

          //get control_use_3
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting Control Use 3 value",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.control_use_3= static_cast<int>(dectof_result);

          //get controller ref
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting Controller reference number",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.controller_ref= static_cast<int>(dectof_result);
          
          //get shields
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting shield value",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.shield_value= static_cast<short int>(dectof_result);

          //get mass
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting Mass",line_number);

			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
  	        the_params.mass= static_cast<short int>(dectof_result);
            if (dectof_result<0) report_error_c("load_complex_object: LAMN: Negative mass not allowed.","Line number below.",line_number);

          //get category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting object category",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.object_category=temp_int;

          //get trivial rejection
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting control if not visible",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.no_trivial_rejection=temp_int;

          //get number of missiles
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting number of missiles",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.number_of_missiles=temp_int;
          //get laser category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting laser category",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            temp_int= static_cast<int>(dectof_result);
  	        the_params.laser_cat=temp_int;
          //get has roll
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: LAMN: Not enough parameters.","Expecting has roll",line_number);
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
            if (the_word[0]==0) report_error_c("load_complex_object: BMAP: Not enough parameters.","Expecting object x position. Line number below.",line_number);
			dectof_result = atof(the_word);
			//oldmac//ix=0;
            //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            //oldmac//dectof_result=dec2f (&my_dec);
            bg_x=dectof_result;


//get object position y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error_c("load_complex_object: BMAP: Not enough parameters.","Expecting object y position. Line number below.",line_number);
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
//¥¥¥¥¥¥¥¥¥¥¥¥end command processing
      word_len=0;
      source+=line_len+1;	//point to next line (accounting for CR)
      line_number++;
     }
	}
	else stop=1;
 }

}




