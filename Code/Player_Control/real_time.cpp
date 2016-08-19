// *************************************MODULE HEADER*********************************
// * FILENAME           : real_time.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 03:02:30 on 9th September 1999
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

//oldmac//#include <Windows.h>
//oldmac//#include <Quickdraw.h>
#include "real_time.h"
#include	"txtsplat.h"
#include "zex_misc.h"
#include "dlp.h"
#include "game_defs.h"
//oldmac//#include <NumberFormatting.h>
#include "sound.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

extern int accumulated_game_time,start_time;
int real_game_time;	//in ticks
int sleep_timer;	//gets loaded with number of frames till we need to sleep
int pilot_asleep;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




void get_real_time_struct(real_time_struct* time_struct)
{
int current_t, divisor;
current_t=real_game_time;


divisor=60*60*60*24;
time_struct->days=current_t/divisor;
current_t-=time_struct->days*divisor;

divisor=60*60*60;
time_struct->hours=current_t/divisor;
current_t-=time_struct->hours*divisor;

divisor=60*60;
time_struct->minutes=current_t/divisor;
current_t-=time_struct->minutes*divisor;

divisor=60;
time_struct->seconds=current_t/divisor;
current_t-=time_struct->seconds*divisor;



}

int time_to_check_salary;
int done_salary;

void init_game_time()
{
accumulated_game_time=0;	//8 am
sleep_timer=MAX_AWAKE;	//-38*60*60*8;	//14 hours at 38 fps
//sleep_timer=38*60*13+60;	//14 hours at 38 fps
pilot_asleep=0;
time_to_check_salary=1000;
done_salary=0;
}


//called every frame
void check_sleep()
{
extern UInt32 gCannon;
extern int gRank,difficulty,credits;
char salary_note[] = "OBC> Message from Command: Your daily salary of ";
char credits_string[] = " credits has been paid.";
char salary_string [100];
unsigned char number_string[12];
int i,o,m,n;
char the_char;
real_time_struct time_struct;
float salary=40,smult=1;
//check time to do salary
if (time_to_check_salary<0)
 {
 time_to_check_salary=2000;
 update_real_time();
 get_real_time_struct(&time_struct);

 if (time_struct.hours==8) 
 {
  if (done_salary==0)
  {
   //salary based on rank
   if (difficulty==16) smult=1;
   else
   if (difficulty==8) smult=0.75;
   else
   if (difficulty==1) smult=0.5;
  
   if (gRank==ENS) salary=40;
   else
   if (gRank==LT) salary=80;
   else
   if (gRank==LTCR) salary=130;
   else
   if (gRank==CDR) salary=200;
   else
   if (gRank==CAPT) salary=280;
   else
   if (gRank==RADM) salary=320;
   else
   if (gRank==VADM) salary=360;
   else
   if (gRank==ADM) salary=450;
   else
   if (gRank==FADM) salary=600;
   
   salary*=smult;
  //add and display salary
   credits+= static_cast<int>(salary);
  //copy salary_note to salary_string
  i=0;
  o=0;
  
  copy_note: the_char=salary_note[i];
   if (the_char!=0) 
    {
    salary_string[o]=the_char;
    i++;o++;
    goto copy_note;
    }
  //now get the number of credits as a string
    ZexNumToString( static_cast<int>(salary),number_string);
  //copy to salary_string
   n=number_string[0];
   m=0;
   while (m<n)
    {
    salary_string[o]=number_string[m+1];
    o++; m++;
    }
    i=0;
  copy_creds: the_char=credits_string[i];
   if (the_char!=0) 
    {
    salary_string[o]=the_char;
    i++;o++;
    goto copy_creds;
    }
  salary_string[o]=0;
  
   add_to_text_display(salary_string,2);
      play_zsound_always(message_from_command, sound_high_pri, c1_chan1, sound_vol_7);
  
    done_salary=1;
    }	//end of done salary==0
   }	//end of 8 oclock
 else done_salary=0;	//not 800 to 859
 
 }
 else
 time_to_check_salary--;
 
if (pilot_asleep==0) sleep_timer--;

  if (sleep_timer>38*60*120+1 && pilot_asleep==0) return;
 
 if (sleep_timer==38*60*120) 	//2 hours
 add_to_text_display("OBC> Might be time to take some rest. You seem to be getting a little tired.",0);
 else
 if (sleep_timer==38*60*60) 	//1 hour
 add_to_text_display("OBC> I think you are getting tired boss. Best take a break.",0);
 else
 if (sleep_timer==38*60*30) 
 add_to_text_display("OBC> you are very tired boss. Better take a break.",0);
 else
 if (sleep_timer==38*60*15) 
 add_to_text_display("OBC> If you don't get some sleep now we are in big trouble!",0);
 else
 if (sleep_timer==38*60*14) 
 add_to_text_display("OBC> You really must sleep now dude! Find a mothership quick.",0);
 else
 if (sleep_timer==38*60*13) 
 {
 add_to_text_display("OBC> If you don't get some rest soon,you'll fall asleep at the controls!",0);
 }

 if (sleep_timer==38*60*12)
 add_to_text_display("YOU HAVE FALLEN ASLEEP AND LOST CONTROL.",0);
 
 if (sleep_timer<38*60*12 && pilot_asleep==0)
  {
  pilot_asleep=1;
  }


if (pilot_asleep==1)
  {
//  add_to_dlp(SLEEP_DIALOG,0,50,0,0,0,"");
  sleep_timer++;
  if (sleep_timer==38*60*60*13) pilot_asleep=0;	//14 hours at 38 fps

  if (LS_Left_Button()==1) pilot_asleep=0;
  add_to_dlp(DRAW_TEXT,150,100,215,0,0,"You have fallen asleep. Click mouse button to wake.");

  }

}


void update_real_time()
{
real_game_time=(LS_TickCount60()-start_time)+accumulated_game_time;
real_game_time+=8*(60*60*60);
}


void add_a_minute_to_real_time()
{
accumulated_game_time+=(60*60);
}


void add_an_hour_to_real_time()
{
accumulated_game_time+=(60*60*60);
}

//returns the hour
int get_hours()
{
real_time_struct time_struct;
 get_real_time_struct(&time_struct);
return time_struct.hours;
}

void draw_real_time(int x, int y, int colour)
{
real_time_struct time_struct;
char number_string1[12];

 update_real_time();
 get_real_time_struct(&time_struct);


 num_to_string999(time_struct.days,number_string1);	//hours
 SplatText_Transparent_Fade ((char *)number_string1,x,y,colour);
    x+=8;
	SplatText_Transparent_Fade (":",x+14,y-1,colour);
	
 num_to_string99(time_struct.hours,number_string1);	//hours
 SplatText_Transparent_Fade ((char *)number_string1,x+20,y,colour);
//
	SplatText_Transparent_Fade (":",x+34,y-1,colour);
//	  
 num_to_string99(time_struct.minutes,number_string1);	//mins
 SplatText_Transparent_Fade ((char *)number_string1,x+40,y,colour);

	SplatText_Transparent_Fade (":",x+55,y-1,colour);
	
 num_to_string99(time_struct.seconds,number_string1);	//secs
 SplatText_Transparent_Fade ((char *)number_string1,x+61,y,colour);


}

