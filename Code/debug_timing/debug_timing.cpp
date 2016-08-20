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

//debug_timing.c
//SB 6/2/99
//DO NOT RELEASE THIS CODE - IT ONLY WORKS WHEN LINKED WITH DRIVERSEERVICESLIB
//WHICH IS NOT AVAILABLE EVERYWHERE
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#error "This file is not in the Zex2005 build"

//oldmac//#include <DriverServices.h>
#include "debug_timing.h"


//#if !PCI_TIMING_DEBUG
//#error !!!! NEED TO SORT DUMMY PCI TIMING ROUTINES !!!!
//#endif


//this routine takes a pointer to an LS_UnsignedWide (thats a 64 bit to you and me) and
//starts a timer
//User should not touch the int32 whilst timer is in operation
#if PCI_TIMING_DEBUG
void start_timer(LS_UnsignedWide *user_time_var)
{
	(*user_time_var)=UpTime();

}

//this routine stops the appropriate timer
//it returns in the lower 32 bits of the input var, the diff between start
//and stop time
void stop_timer(LS_UnsignedWide *user_time_var)
{
LS_UnsignedWide t;
UInt32 stop_low;
	t=UpTime();

	(*user_time_var)=AbsoluteToNanoseconds(SubAbsoluteFromAbsolute(t,*user_time_var));

}

#endif


#define FRAMES_TO_RESET_AFTER 300	/* set to -1 to disable reset */
					// Note: setting too low gives false readings due to interrupts.


#define NUMLOC T_MAX-T_MIN+10		/* 10 for safety */
double timemin[NUMLOC];
double timemax[NUMLOC];
double timetotal[NUMLOC];
double timeav[NUMLOC];
double timepercent[NUMLOC];

int numframes=FRAMES_TO_RESET_AFTER+10;
int totalframes=0;				// never reset

LS_UnsignedWide timesofar;
LS_UnsignedWide *T_timesofar_ptr=&timesofar;



#if PCI_TIMING_DEBUG
void T(int routine_code)
{
double current_time;
int count;

#if FIND_PERCENTAGES_DEBUG

if(routine_code==0) totalframes++;

if(FRAMES_TO_RESET_AFTER>0 && numframes>FRAMES_TO_RESET_AFTER)
  {
  numframes=0;
  for(count=T_MIN; count<=T_MAX; count++)
    {
    timemin[count]=0;
    timemax[count]=0;
    timetotal[count]=0;
    timeav[count]=0;
    timepercent[count]=0;

    }
  }
else
  {
  stop_timer(T_timesofar_ptr);

  current_time=((int)T_timesofar_ptr->lo);


  // increment number of frames
  if(routine_code==0) numframes++;

  // TOTAL
  timetotal[routine_code]+=current_time;

  // *** MINIMUMS ***
  if(timemin[routine_code]==0) { timemin[routine_code]=999999999; }	// zero means non-initialised
  if(current_time==0) { timemin[routine_code]=1; }
  else { if(timemin[routine_code]>current_time) timemin[routine_code]=current_time; }
  // minimum CANNOT be zero - will use 1 instead

  // *** MAXIMUMS ***
  if(timemax[routine_code]<current_time) { timemax[routine_code]=current_time; }
  // BIG NOTE!! maximums cannot be trusted too much - interrupts (e.g. music) can make them look much worst. This
  // effects maximums much more than averages (which over long periods even out over the whole range).


  // *** MEAN AVERAGE ***
  timeav[routine_code]=timetotal[routine_code]/numframes;

  // *** PERCENTAGE ***

  current_time=0;

  for(count=T_MIN; count<=T_MAX; count++)
  {
    current_time+=timeav[count]; 		  	  // create sum of all averages
  }


  timepercent[routine_code]=100*(timeav[routine_code]/current_time); // 100 * fraction of total time

  }

#endif

// start the timer for the next go
start_timer(T_timesofar_ptr);
}


#endif


