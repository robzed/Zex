//game_defs.h
#ifndef __zex_defs__
#define __zex_defs__
#endif

#define MAX_NUMBER_OF_OBJECTS 1000
#define MAX_FRAME_RATE 35

#define MAX_VELOCITY 360	//zex's maximum velocity
#if OGL==1
#define VERS_STRING "0.99.7 - Open GLª"
#else
#define VERS_STRING "0.99.7" 
#endif
#define DATE_STRING " 28 Sept 15"

#define ENS 0
#define LT 1
#define LTCR 2
#define CDR 3
#define CAPT 4
#define RADM	5
#define VADM 6
#define ADM 7
#define FADM 8

#define COMMODITIES_MULTIPLIER 12	//commodities base price multiplier
#define LASER_RANGE_MULTIPLIER 1.4	//All laser ranges multiplied by this (float)

