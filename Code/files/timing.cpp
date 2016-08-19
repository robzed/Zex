/*
 *  timing.cpp
 *  Z_dungeon
 *
 *  Created by Rob Probin on Sun Jan 12 2003.
 *  Copyright (c) 2003 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2005/11/15 22:15:00 $
 * $Revision: 1.1 $
 *
 * For log see end of file...
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//#include "general_include.h"

#include "timing.h"

// +--------------------------------+-------------------------+-----------------------
// | TITLE: LS_GetTicks             | AUTHOR(s): Rob Probin   | DATE STARTED: 30 Dec 02
// +
// | DESCRIPTION: Very like SDL_GetTicks apart from it checks for time going backwards
// | and avoids it. Returns number of milliseconds.
// |
// | Suggested in SDL digest, Vol 1 #621 
// |
// | Freezes after 49 days.
// +----------------------------------------------------------------ROUTINE HEADER----
// NOTES about 49 day limit. We could check if previous_ticks < 1 minute
// and new_ticks > 48 days, then correct that.
// 
Uint32 LS_GetTicks(void)
{
	static Uint32 previous_ticks=0;
	Uint32 new_ticks; 
	
	new_ticks = SDL_GetTicks();
	if (new_ticks < previous_ticks)
	{
		// oops - time went backwards... (time does click backwards on some machines, otherwise
		// after 49 days it wraps. We can't really handle wraps so that would break us.)
		return previous_ticks; // pretend it didn't happen
	}
	previous_ticks = new_ticks;
	return new_ticks;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Jan 03
// +
// | DESCRIPTION: 
// |
// +----------------------------------------------------------------ROUTINE HEADER----
const int NUMBER_OF_AVERAGE_FRAMES = 5;
const int NUMBER_OF_STORED_FRAMES = 20;

// used for calculating the average frame time for the last 5 frames - this is used as the world timing
static Uint32 last_frame_ticks;
static Uint32 previous_frame_ticks[NUMBER_OF_AVERAGE_FRAMES];
static double average_frame_time;					// the average frame time of the last 5 frames

// overall game monitoring data
static Uint32 beginning_tick_count;					// used to calculate the FPS for the entire game
static Uint32 number_of_frames;						// number of frames for the entire game

static Uint32 long_term_num_of_frames;				// used for the slow FPS
static Uint32 long_term_next_frame_ticks;
static double long_term_average_frame_rate;
const int LONG_TERM_FRAME_RATE_TICKS_TOTAL = 500;			// 0.5 second - relative long period

static double total_frame_time_floating_point;				// keep a check of how long we ran things - for comparison with total ticks.


void init_frame_time(void)
{
	int count;
	
	last_frame_ticks = LS_GetTicks();
	
	for(count=0; count < NUMBER_OF_AVERAGE_FRAMES; count++)
	{
		// fill these in reverse order... last is newest therefore should have closest time...
		previous_frame_ticks[NUMBER_OF_AVERAGE_FRAMES - count] = last_frame_ticks-((count+1)*30);	// assume 33 fps to start with
	}
	
	// frame measurement setup
	beginning_tick_count = SDL_GetTicks();
	number_of_frames=0;
	
	// long term frame rate setup
	long_term_next_frame_ticks = LS_GetTicks() + LONG_TERM_FRAME_RATE_TICKS_TOTAL;
	long_term_num_of_frames = 0;
	long_term_average_frame_rate = 0;		// default
	
	// data for tracking accuracy of gametime against realtime.
	total_frame_time_floating_point = 0;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Jan 03
// +
// | DESCRIPTION: Time between ticks for this frame
// |
// +----------------------------------------------------------------ROUTINE HEADER----

#define FRAME_TIME_RECORD_FOR_DEBUG 0

#if FRAME_TIME_RECORD_FOR_DEBUG
Uint32 frames_tick[200];
int frame_index = 0;
#endif

void measure_frame_time(void)
{
	Uint32 new_frame_ticks;
	int count;
	
	new_frame_ticks = LS_GetTicks();
	
#if FRAME_TIME_RECORD_FOR_DEBUG
	frames_tick[frame_index++]=new_frame_ticks-last_frame_ticks;
	if(frame_index >= 200) frame_index = 0;
#endif
	
	// move previous frame ticks down by 1 slot
	for(count=0; count < (NUMBER_OF_AVERAGE_FRAMES-1); count++)
	{
		previous_frame_ticks[count] = previous_frame_ticks[count+1];
	}
	
	// record current frame tick count (ms time)
	previous_frame_ticks[NUMBER_OF_AVERAGE_FRAMES-1] = last_frame_ticks;
	
	// store current ticks for next time
	last_frame_ticks = new_frame_ticks;
	
	// 
	// calculate average frame time
	// Do this once per frame - since we are going to read this quite a few times - no need to do the calculation more than once...
	// This used to add up the previous 5 frame times and divide by 5. This way does the same and is quicker.
	//
	average_frame_time = (static_cast<double>(last_frame_ticks - previous_frame_ticks[0])) / NUMBER_OF_AVERAGE_FRAMES;
	average_frame_time /= 1000.0;
	// Accuracy... each time difference appears in the average 5 times - therefore appart from floating point inaccuracies (pretty minimal
	// with doubles) we should have a nice accurate tracking of real time with game time. If we were using just ints for average_frame_time
	// then we would have a round error since we would round down each time. There is another advantage of using doubles - we get a 
	// better resolution with higher frame rates and lower accuracy of the tick return - which is usually every 10ms on some machines.
	//
	total_frame_time_floating_point += average_frame_time;		// keep a check of how long we ran things - for comparison with total ticks.
	
	// update total frames - so frame measurement can be updated
	number_of_frames++;
	
	// long term calculations
	long_term_num_of_frames++;
	if(new_frame_ticks > long_term_next_frame_ticks)
	{
		long_term_average_frame_rate  =  (1000.0 * long_term_num_of_frames) / (new_frame_ticks + LONG_TERM_FRAME_RATE_TICKS_TOTAL - long_term_next_frame_ticks);
		long_term_num_of_frames = 0;
		long_term_next_frame_ticks = new_frame_ticks + LONG_TERM_FRAME_RATE_TICKS_TOTAL;
		
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Jan 03
// +
// | DESCRIPTION: Time between ticks for this frame
// +----------------------------------------------------------------ROUTINE HEADER----
Uint32 get_frame_time(void)
{
	return last_frame_ticks - previous_frame_ticks[NUMBER_OF_AVERAGE_FRAMES-1];
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Jan 03
// +
// | DESCRIPTION: Time average between ticks for this frame.
// |
// | This routine needs to be as accurate as possible without losing time to either
// | inaccuracy (originally it was a Uint32) or due to the averaging process losing 
// | milliseconds - this is because we are trying to keep two remote machines in check.
// +----------------------------------------------------------------ROUTINE HEADER----

double get_average_frame_time(void)
{
	return average_frame_time;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 5 Feb 05
// +
// | DESCRIPTION: Current Frames per second (averaged over 5 frames)
// +----------------------------------------------------------------ROUTINE HEADER----
double get_current_FPS(void)
{
	return 1/average_frame_time;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 5 Feb 05
// +
// | DESCRIPTION: Current Frames per second (averaged over 250ms)
// +----------------------------------------------------------------ROUTINE HEADER----
double get_long_average_FPS(void)
{
	return long_term_average_frame_rate;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Jan 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

Uint32 total_game_ticks(void)
{
	return SDL_GetTicks() - beginning_tick_count;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: get_total_gametime_run  | AUTHOR(s): Rob Probin   | DATE STARTED: 5 Feb 05
// +
// | DESCRIPTION: This returns the number of seconds that has existed in the game-world.
// +----------------------------------------------------------------ROUTINE HEADER----

double get_total_gametime_run(void)
{
	return total_frame_time_floating_point;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Jan 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

double overall_average_FPS(void)
{
	double FPS;
	
	FPS = (double)number_of_frames/(double)total_game_ticks();
	FPS *= 1000;
	
	return FPS;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Jan 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

Uint32 total_game_frames(void)
{
	return number_of_frames;
}



// +-------------------------------------+-------------------------+-----------------------
// | TITLE: MaximumRateTimer constructor | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

MaximumRateTimer::MaximumRateTimer(Uint32 minimum_time_in_ms)
{
	time_period = minimum_time_in_ms/1000.0;
	accumulator = 0;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: frame_update_and_flag   | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool MaximumRateTimer::frame_update_and_flag()
{
	bool flag = false;
	
	accumulator += get_average_frame_time();
	
	if(accumulator>time_period)	// if certain time has passed, it's time to run the animations
	{
		accumulator -= time_period;	// make a time period it less
		if(accumulator>time_period)	// if still more than time period we can't keep up so reset back to zero
		{
			accumulator=0;	// i.e. average frame time > time_period  (frame rate < 1/time_period), therefore stuff will run slow
		}
		
		flag = true;
	}
	
	return flag;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE: change_update_rate      | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void MaximumRateTimer::change_update_rate(Uint32 new_minimum_time_in_ms)
{
	time_period = new_minimum_time_in_ms;
}



// +------------------------------------+-------------------------+-----------------------
// | TITLE: SingleShotTimer constructor | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: create a new single shot timer - intially disabled
// +----------------------------------------------------------------ROUTINE HEADER----

SingleShotTimer::SingleShotTimer()
{
	enabled = false;			// timer starts off disabled
	expired_flag = false;		// not expired to start with
	held = false;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: expired                 | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: returns true when the timer has come to the end
// +----------------------------------------------------------------ROUTINE HEADER----

bool SingleShotTimer::expired()
{
	bool local_expired_flag = false;
	
	if(enabled) // always false if not enabled.
	{
		if(LS_GetTicks() > destination_time)
		{
			expired_flag = true;
		}
		
		local_expired_flag = expired_flag;
	}
	
	return local_expired_flag;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: start_timer            | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: start (or restart) the timer with a specific target duration
// +----------------------------------------------------------------ROUTINE HEADER----

void SingleShotTimer::start_timer(Uint32 minimum_time_in_ms)
{
	enabled = true;
	expired_flag = false;		// not expired to start with
	held = false;
	
	destination_time = LS_GetTicks() + minimum_time_in_ms;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: hold_timer              | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: temporarily disabled the timer from incrementing
// +----------------------------------------------------------------ROUTINE HEADER----

// *** NOT TESTED ***
void SingleShotTimer::hold_timer()
{
	if(!held && enabled)
	{
		enabled = false;
		held = true;
		
		destination_time = destination_time - LS_GetTicks();		// convert the destination time into time left
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: release_timer           | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: release a previously held timer
// +----------------------------------------------------------------ROUTINE HEADER----

// *** NOT TESTED ***
void SingleShotTimer::release_timer()
{
	if(held && !enabled)
	{
		destination_time = LS_GetTicks() + destination_time;	// convert the time left back into a destination time
	}
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: disable_timer           | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Apr 04
// +
// | DESCRIPTION: stops the timer returing expired ever
// +----------------------------------------------------------------ROUTINE HEADER----

// *** NOT TESTED ***
void SingleShotTimer::disable_timer()
{
	enabled = false;
	held = false;
}




/* CVS LOG
*
* $Log: timing.cpp,v $
* Revision 1.1  2005/11/15 22:15:00  rob
* Zex2005: Added new files for game file selector in Zex.
*
* Revision 1.3  2005/02/05 21:30:07  rob
* ZD: Re-formatted timing.cpp file so that it's all consistent. Also made a variable static.
*
* Revision 1.2  2005/02/05 21:25:45  rob
* ZD: (i) Changed get_average_frame_time to double return and frame time in seconds rather than ms - most places (now all) convert to this anyway, plus it's more accurate for when we need to track time between two machines. (ii) added a new function to get a long timebase FPS (500ms), (iii) added more comments and explanations, (iv) added a function to return the accumulated gametime.
*
* Revision 1.1  2005/01/01 14:11:49  robp
* ZD: Moved files from individual directories under root directory to a source directory.
*
* Revision 1.8  2004/06/10 21:37:24  robp
* ZD: Three routines tested - removed not tested comments.
*
* Revision 1.7  2004/04/30 21:52:30  robp
* ZD: Timer code that's not going to be used because I've found the repeat command in the SDL.
*
* Revision 1.6  2004/04/29 21:05:14  robp
* ZD: Added a timer class - based on animation timer - then replaced animation timer with an instance. Then used new class to create a flashing cursor for the saved game description entry box.
*
* Revision 1.5  2003/09/20 18:02:12  robp
* *** NAME CHANGE OF ALL C FILES FROM .c TO .cpp *** 
Before this point you will need to rename the files back to .c (i.e. remove the pp from .cpp) to build older versions.
*
* Revision 1.4  2003/06/14 09:15:43  robp
* Added header and footer
*
*
*
*/

