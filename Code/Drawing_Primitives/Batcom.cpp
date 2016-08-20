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

// Batcom.cpp

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


#include "monster1.h"
#include "3d_structures_def.h"
#include	"objects_controls.h"	//controller definitions
#include "angular_stuff.h"
#include "monster_control_misc.h"
#include "Batcom.h"
#include "txtsplat.h"
#include "zex_misc.h"
//oldmac//#include <NumberFormatting.h>

 void Select_Ship(int object_num);
 void Merge_String(void);
 void nothing_targetted();
void clear_string(unsigned char*no_string);
 
 extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
 extern _3D ship_rotated_normal; //from engine
 extern int dynamic_object_index;
 extern int ship_z_ang;

 static unsigned char no_string[16];
 unsigned char main_string[16] = "SHIELDS        ";
 
 static char bc_str1[32];	//message strings override normal operation
 static char bc_str2[32];
 
 static int have_batcom_message=0;
 static int message_splat_count;
 
void init_batcom()
{
have_batcom_message=0;
}

//this code can be replaced with
// thing we are aiming at = (*ocb_ptr).object_list[zex].attack_what

void update_battlecom(void)
{	 
	 //Added 100699 SB
	int zex_aiming_at;

	if (have_batcom_message==1)
	{
	  if (message_splat_count<2)
	  {
			message_splat_count++;
			SplatText_Small(bc_str1, 553,460,0,255);
		SplatText_Small(bc_str2,553,468,0,255);	
	  }
	 return;	//displaying a message
	}
	 
	 zex_aiming_at=(*ocb_ptr).object_list[zex].attack_what;
	 if (zex_aiming_at!=-1 && (*ocb_ptr).object_list[zex_aiming_at].in_use!=-1)
	 {
		Select_Ship(zex_aiming_at);
	 }
	 else
		nothing_targetted(); 
 } //end update battle com
 
 
 
 
 void Select_Ship(int object_num)
 {
 unsigned long the_id;
 
 the_id=(*ocb_ptr).object_list[object_num].Oid;
 
 SplatText_Small((char *)"     ",553,468,0,255);	//blank out previous	

 switch (the_id)
  {
   case 'MON3':
   {
    SplatText_Small("HEAVY DEFENCE   ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'ATRA':
   {
    SplatText_Small("TRANSPORTER    ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'GRN1':
   {
    SplatText_Small("HEAVY DEFENCE T2",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'ASH1':
   {
    SplatText_Small("ALIEN SHUTTLE   ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'MON2':
   {
    SplatText_Small("M2 LIGHT DEFENCE ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }
  
   case 'SGN1': case 'SGN2': case 'SGN3':
   {
    SplatText_Small("GUN EMPLACEMENT ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'MN31': case 'MN32': case 'MN33': case 'MN34':
   {
    SplatText_Small("PHOENIX      ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'SLS1': case 'SLS2': case 'SLS3':
   {
    SplatText_Small("LASER       ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'MS01': case 'MS02':	//Human mothership
   {
    SplatText_Small("HUMAN MOTHERSHIP",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }
  
   case 'MS10': case 'MS11': case 'MS12': case 'MS13':	//Alien mothership
   {
    SplatText_Small("ALIEN MOTHERSHIP",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'WARP':
   {
    SplatText_Small("WARPER          ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'EXC1':
   {
    SplatText_Small("EXCALIBUR      ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);
    break;
   }

   case 'MON6':
   {
    SplatText_Small("PYTHON         ",553,460,0,255);
    ZexNumToString((*ocb_ptr).object_list[object_num].shield_value,no_string);
    clear_string(no_string);

    break;
   }
  

  default:
  nothing_targetted(); 
  break;
 }
}

void clear_string(unsigned char*no_string)
{
    no_string[no_string[0]+1]=' ';
    no_string[no_string[0]+2]=' ';
    no_string[no_string[0]+3]=' ';
    no_string[no_string[0]+4]=' ';
    no_string[no_string[0]+5]=0;	//quick p to c hack
    no_string[0]=' ';
	Merge_String();
    SplatText_Small((char *)main_string,553,468,0,255);	

}
//inserts a cstr message that overrides the normal operation
void insert_batcom_message(const char * str1, const char * str2)
{
int count;
char the_char;

have_batcom_message=1;

the_char=255;
count=0;
  while(the_char!=0 && count<255)
  {
    the_char=str1[count];
    bc_str1[count]=the_char;
    count++;
  }
  
the_char=255;
count=0;
  while(the_char!=0 && count<255)
  {
    the_char=str2[count];
    bc_str2[count]=the_char;
    count++;
  }
message_splat_count=0;

}

void remove_batcom_message()
{
have_batcom_message=0;
}

void nothing_targetted()
{
extern int unlim_thrust,gvel_limiter,directional_control;


//ORCS OFF
if (directional_control==0)
 {
  int sign,temp_thrust;
	//show current velocity
    SplatText_Small("VELOCITY:       ",553,460,0,255);
    sign=0;
    temp_thrust=(*ocb_ptr).object_list[0].current_velocity;
    if (unlim_thrust<0) 
    {
    temp_thrust=-temp_thrust;    
    }
    ZexNumToString(temp_thrust,no_string);
//    no_string[no_string[0]+1]=0;	  //quick p to c hack
    no_string[no_string[0]+1]=' ';
    no_string[no_string[0]+2]=' ';
    no_string[no_string[0]+3]=' ';
    no_string[no_string[0]+4]=' ';
    no_string[no_string[0]+5]=' ';
    no_string[no_string[0]+6]=' ';
    no_string[no_string[0]+7]=' ';
    no_string[no_string[0]+8]=' ';
    no_string[no_string[0]+9]=' ';
    no_string[no_string[0]+10]=' ';
    no_string[no_string[0]+11]=0;	//quick p to c hack

    no_string[0]='P';
//	Merge_String();
    SplatText_Small((char *)no_string,553,468,0,255);	
  return;	
 }



if (gvel_limiter==1)
 {
  int sign,temp_thrust;
	//show current velocity
    SplatText_Small("VELOCITY:       ",553,460,0,255);
    sign=0;
    temp_thrust=unlim_thrust;
    if (unlim_thrust<0) 
    {
    sign=1;
    temp_thrust=-temp_thrust;
    
    }
    ZexNumToString(temp_thrust,no_string);
//    no_string[no_string[0]+1]=0;	  //quick p to c hack
    no_string[no_string[0]+1]=' ';
    no_string[no_string[0]+2]=' ';
    no_string[no_string[0]+3]=' ';
    no_string[no_string[0]+4]=' ';
    no_string[no_string[0]+5]=' ';
    no_string[no_string[0]+6]=' ';
    no_string[no_string[0]+7]=' ';
    no_string[no_string[0]+8]=' ';
    no_string[no_string[0]+9]=' ';
    no_string[no_string[0]+10]=' ';
    no_string[no_string[0]+11]=0;	//quick p to c hack
    if (sign==1)
    no_string[0]='M';
    else
    no_string[0]='P';
//	Merge_String();
    SplatText_Small((char *)no_string,553,468,0,255);	
  return;	
 }


        SplatText_Small("NO DATA         ",553,460,0,255);
	SplatText_Small("AVAILABLE       ",553,468,0,255);	
 
}


void Merge_String(void)
 {
  main_string[8] = no_string[0];
  main_string[9] = no_string[1];
  main_string[10] = no_string[2];
  main_string[11] = no_string[3];
  main_string[12] = no_string[4];
  main_string[13] = no_string[5];
 }



