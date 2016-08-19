/*
 *  game_script.h
 *  Zex
 *
 *  Created by Stuart Ball on 11/04/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
//This file must only be included into map control
//All times are frame counts

#define TIME_TILL_MESSAGE1_FROM_DELTA 1000	
#define TIME_TILL_MESSAGE2_FROM_DELTA 1000	
#define TIME_TILL_MESSAGE3_FROM_DELTA 1000	
#define TIME_TILL_SURRENDER 1000

//States
#define NO_ACTION 0		//this is used when we want to do nothing
#define DO_DELTA_MESSAGE1 1	//message from delta quadrant - this is first. We add some aliens to delta 
#define DO_DELTA_MESSAGE2 2	//message from delta quadrant - this is second. We add more aliens to delta
#define DO_DELTA_MESSAGE3 3	//message from delta quadrant - this is third. We make all aliens in delta and if player not in gamma we add some there too

#define SURRENDER_MESSAGE 20	//Aliens want to surrender message
#define BETA_MESSAGE 30	//if player doesn't accept surrender, some time later they invade the beta quad  
#define HIT_GAMMA_QUAD 40	//if surrendered (not accepted) then when player hits gamma quad next time we suck all his items via some secret weapon

//Strings
#define DELTA_MESSAGE1 0
#define DELTA_MESSAGE2 1
#define DELTA_MESSAGE3 2
#define BETA_MESSAGE1 3
#define BETA_MESSAGE2 4

extern unsigned char script_messages[];

extern int surrendered_flag;
extern int gone_around_flag;
//Scripting
extern unsigned long script_timer1;	//gp timer (frames)
extern unsigned long script_timer2;	//
extern unsigned long script_state1;	//
extern unsigned long script_state2;	//

extern void update_script(void);
extern void handle_surrender(void);

