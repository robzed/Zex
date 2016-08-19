// *************************************MODULE HEADER*********************************
// * FILENAME           : Untitled 1
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 03:45:11 on 11th August 1999
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        
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
#include "game_defs.h"

#include <string.h>			// for memcpy

#include "3d_structures_def.h"
#include	"zex_misc.h"
#include "dlp.h"
#include "cargo.h"
#include "instrum_int.h"
#include	"load_objects.h"
#include	"object_control_top.h"
#include "compile_zmap.h"
#include "obj_control_misc_routines.h"
#include	"objects_controls.h"	//controller definitions
#include "buy_int.h"
#include "sound.h"
//oldmac//#include <resources.h>
#include "Error_handler.h"
// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int can_gen_cargo;	//if 0 no
int number_of_cargos;	//filled from read_in_cargos (bearing in mind our array starts at index 0)
int jettison_timer;	//if > 0 we can't pick up items

hold_desc cargos_array[NUMBER_OF_COMMODITIES];	//we read our resource to here (with some casting of course)


extern	DynObjectsFixedSize *ocb_ptr;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




//cargo.c

//gen a random cargo pod
//lieklyhood is the chance the object destroyed was carrying cargo - 
//a shuttle might be 80 whilst a fighter would be say 20
void gen_cargo(int object,int likelyhood)	
{
load_dyn_object_pb the_params;


if (RangedRdm(0,100)<likelyhood)
    gen_specific_cargo(object, RangedRdm(0,number_of_cargos));	//gen a random item
}


//use to gen a cargo pod with a specific cargo - called from
//kill_zex for example
void gen_specific_cargo(int object,int the_cargo)
{
load_dyn_object_pb the_params;
int static_slot;
int dest_ocb;
int test,cargs_div_4;
//load in a cargo container
        clear_pb(&the_params);
  
	the_params.world_x=(*ocb_ptr).object_list[object].Oworldx;
	the_params.world_y=(*ocb_ptr).object_list[object].Oworldy;
    the_params.delta_rot_z=5;
    the_params.delta_rot_x=3;
    the_params.Omovedx=RangedRdm(20,300)-150;
    the_params.Omovedy=RangedRdm(20,300)-150;
    the_params.control_use_2=the_cargo;	//store the item
    
	the_params.world_z=MONSTER_Z;

//pick colour depending on cargo
//we have about 80 cargos so...
    cargs_div_4=number_of_cargos/4;
    
    if (the_cargo<cargs_div_4)
      static_slot=get_static_object_slot('CAR1');	//go fetch fido (the object)
    else
    if (the_cargo<cargs_div_4*2)
      static_slot=get_static_object_slot('CAR2');	//go fetch fido (the object)
    else
    if (the_cargo<cargs_div_4*3)
      static_slot=get_static_object_slot('CAR3');	//go fetch fido (the object)
    else
      static_slot=get_static_object_slot('CAR4');	//go fetch fido (the object)
    
    
      if (static_slot==-1) 
      return;	//oops

      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
      if (dest_ocb!=-1)
        {
	     the_params.controller_ref=CARGO_CONTAINER;
         the_params.has_roll=1;
    	
	     the_params.shield_value=470;
	     the_params.mass=100;
	     the_params.no_trivial_rejection=1;
	     the_params.object_category=(*ocb_ptr).object_list[object].object_category;

         the_params.who_spawned_this_object=-1;
	     load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
       }
}

//returns 1 if picked up
int handle_pickup(int what_object)
{
extern hold_desc hold_items[MAX_HOLD_ITEMS];
int hold_slot,i,j;
char  cargo_descript_string[300];	//string for cargo descript.


if (jettison_timer>0) return 0;

hold_slot=-1;
//can we find an empty slot in the hold?
for (i=0;i<MAX_HOLD_ITEMS;i++)
    {
    if (hold_items[i].id==-1) 
     {
      hold_slot=i;
      break;
     }
    }
if (hold_slot==-1) 
 {
 add_to_text_display("OBC> No room in hold for cargo container!",1);

 return 0;	  //all slots full
 }
//ok, we can take it

i=(*ocb_ptr).object_list[what_object].control_use_2;;	//get the item
add_to_kill_list(what_object);


hold_items[hold_slot]=cargos_array[i];	//copy all the data for this item to the hold

gen_cargo_description_string(cargo_descript_string,i);

 add_to_text_display(cargo_descript_string,0);
     play_zsound(pickup, sound_low_pri, c1_chan1, sound_vol_7);

return 1;

}

void gen_cargo_description_string(char * where_string, int which_id)
{
if (cargos_array[which_id].dofq==1)
{
strcpy (where_string,"One Tonne of ");
where_string+=13;
}
else
if (cargos_array[which_id].dofq==4)
{
strcpy (where_string,"1Kg of ");
where_string+=7;
}
else
if (cargos_array[which_id].dofq==2)
{
strcpy (where_string,"1 Barrel of ");
where_string+=12;
}
else
if (cargos_array[which_id].dofq==3)
{
strcpy (where_string,"1 case of ");
where_string+=10;
}


strcpy (where_string,cargos_array[which_id].string);
}

//This is called once from z_init. We copy from here when docked.
void read_in_cargo()
{
Handle mitm_res_h;
short * mitm_ptr;
hold_desc * mitm_ptr1;
int i;

 mitm_res_h = (Handle) LS_GetResource('CITM',128);  //Get the Handle to the Resource 
 if (mitm_res_h==0) report_error_c("Resource missing: CITM 128","",4);
 LS_HLock (mitm_res_h);
 mitm_ptr=(short *) *mitm_res_h;	//Oh wonderful god of casting
 
 #if ARCH_LITTLE_ENDIAN
 number_of_cargos=Endian16_Swap(*mitm_ptr);
 #elif ARCH_BIG_ENDIAN
 number_of_cargos=(*mitm_ptr);
 #else
 #error "Endian defines missing"
 #endif
 
 mitm_ptr++;
 mitm_ptr1=(hold_desc*) mitm_ptr;	//skip number of entries
 
//now blockmove the rest
 BlockMoveData(mitm_ptr1, cargos_array,sizeof(hold_desc)*number_of_cargos);

 #if ARCH_LITTLE_ENDIAN
/*
 typedef struct
{
char string [0x24];
short dofq;	  //1=tonnes, 4=grammes	
short obj_class;	  //10-ores, 11-food, 12-tech, 13-luxury/textiles, 14-firearms
short base_price_human;
short base_price_alien;
short transport_risk,pirate_risk;
short MFR; //manufacturer: 1=human, 2=alien
short markup,stock,buy_price,sell_price;
short id;	//serial number (index - 0 based)
} hold_desc;
*/
int n;
for (n=0;n<number_of_cargos;n++)
{
	cargos_array[n].dofq=Endian16_Swap(cargos_array[n].dofq);
	cargos_array[n].obj_class=Endian16_Swap(cargos_array[n].obj_class);
	cargos_array[n].base_price_human=Endian16_Swap(cargos_array[n].base_price_human);
	cargos_array[n].base_price_alien=Endian16_Swap(cargos_array[n].base_price_alien);
	cargos_array[n].transport_risk=Endian16_Swap(cargos_array[n].transport_risk);
	cargos_array[n].pirate_risk=Endian16_Swap(cargos_array[n].pirate_risk);
	cargos_array[n].MFR=Endian16_Swap(cargos_array[n].MFR);
	cargos_array[n].markup=Endian16_Swap(cargos_array[n].markup);
	cargos_array[n].stock=Endian16_Swap(cargos_array[n].stock);
	cargos_array[n].buy_price=Endian16_Swap(cargos_array[n].buy_price);
	cargos_array[n].sell_price=Endian16_Swap(cargos_array[n].sell_price);
	cargos_array[n].sell_price=Endian16_Swap(cargos_array[n].sell_price);
}
#elif !ARCH_BIG_ENDIAN
#error "Endian defines missing"
#endif
 
//set up ID's (the private field of the resedit template
for (i=0;i<NUMBER_OF_COMMODITIES;i++)
{
 cargos_array[i].id=i;
}
//As of 050806 all prices are multiplied by COMMODITIES_MULTIPLIER
for (i=0;i<number_of_cargos;i++)
{
	cargos_array[i].base_price_human*=COMMODITIES_MULTIPLIER;
	cargos_array[i].base_price_alien*=COMMODITIES_MULTIPLIER;
}

}

