/*
 *  timing.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Sun Jan 12 2003.
 *  Copyright (c) 2003 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2005/11/15 22:14:58 $
 * $Revision: 1.1 $
 *
 * For log see end of file...
 *
 */

#ifndef _TIMING_H_
#define _TIMING_H_

// get timing data
void init_frame_time(void);
void measure_frame_time(void);
Uint32 get_frame_time(void);			// in ms. Normally you use get_average_frame_time() since this is unstable
double get_average_frame_time(void);	// in seconds
Uint32 LS_GetTicks(void);				// counts in 1ms jumps

// stuff to get total game framerates
Uint32 total_game_frames(void);
double overall_average_FPS(void);		// very accurate game framerate over entire game
Uint32 total_game_ticks(void);
double get_current_FPS(void);			// Current Frames per second (averaged over 5 frames)
double get_long_average_FPS(void);		// over 500ms
double get_total_gametime_run(void);	// This returns the number of seconds that has existed in the game-world.

// 
// This timer abstracts away the details of maintaining a periodic time and instead provides a call that is used 
// every frame to update a timer. A flag is returned to say whether the timer has expired for this frame. It maintains 
// a rollover so on average (if the frame rate is high enough) it will get very close to the actual requested rate.
//
// If the frame rate is not high enough to maintain the required interval then the routine will attempt to get
// as close as it can. In some cases this might flag up every frame for a low frame rate.
//
// The routine uses average frame rate since we cannot predict the actual time this frame.
//
class MaximumRateTimer
{
public:
	MaximumRateTimer(Uint32 minimum_time_in_ms);  // construct a timer of a certain
	bool frame_update_and_flag();			// should be called once a frame, and the action code called if the returned result is true.
	void change_update_rate(Uint32 new_minimum_time_in_ms);   // used to change the time it occurs. May cause slight timing glitches when changed.
	
	// at some point in the future we should change this to: (a) call back automatically, (b) do a less processor intensive timing (e.g. a queue of to expire times).
	//MaximumRateTimer(Uint32 time, void (*called_function)());
	//MaximumRateTimer(Uint32 time, object to call through interface class);
	// also note: sdl has callback timers...
private:
	double time_period;
	double accumulator;
};

//
// This class provides a class that returns expired after a certain amount of time.
//
// Will break if run over 49 days...
//
// NOTES
// 
// 1. Accuracy should be counted as 10ms - since this is the minimum base tick on most machines.
// 2. Holding and releasing may cause a +/-10ms alteration in the end time (per call).
// 3. Might be able to fix 49 day roll-over break with subtract instead of compare?
//
// *** NOT FULLY TESTED - checkout source for exact routines not tested ***
//
class SingleShotTimer
{
public:
	SingleShotTimer();							// create a new single shot timer - intially disabled
	bool expired();								// returns TRUE when the timer has come to the end
	void start_timer(Uint32 minimum_time_in_ms);   // start (or restart) the timer with a specific target duration
	void hold_timer();  						// temporarily disabled the timer from incrementing
	void release_timer();						// release a previously held timer
	void disable_timer();						// stops it returing expired

private:
	Uint32 destination_time;
	bool enabled;
	bool expired_flag;
	bool held;
};



#endif // _TIMING_H_



/* CVS LOG
 *
 * $Log: timing.h,v $
 * Revision 1.1  2005/11/15 22:14:58  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.2  2005/02/05 21:25:51  rob
 * ZD: (i) Changed get_average_frame_time to double return and frame time in seconds rather than ms - most places (now all) convert to this anyway, plus it's more accurate for when we need to track time between two machines. (ii) added a new function to get a long timebase FPS (500ms), (iii) added more comments and explanations, (iv) added a function to return the accumulated gametime.
 *
 * Revision 1.1  2005/01/01 14:11:49  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.5  2004/06/10 21:38:52  robp
 * ZD: Amended notes about single timer type.
 *
 * Revision 1.4  2004/04/30 21:52:31  robp
 * ZD: Timer code that's not going to be used because I've found the repeat command in the SDL.
 *
 * Revision 1.3  2004/04/29 21:05:14  robp
 * ZD: Added a timer class - based on animation timer - then replaced animation timer with an instance. Then used new class to create a flashing cursor for the saved game description entry box.
 *
 * Revision 1.2  2003/06/14 08:51:40  robp
 * Added note about getting the average frame rate.
 *
 *
 *
 */
