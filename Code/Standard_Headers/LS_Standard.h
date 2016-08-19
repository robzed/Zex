//LS_Standard.h

#ifndef _LS_STANDARD_H_
#define _LS_STANDARD_H_

#define PI 	((float)3.1415926535898)
#define ABS(x)	((x)>0?(x):-(x))
#define no 0
#define yes 1

//Below this line aa project specific things that should really go in another file
#define no_colldet no
#define yes_colldet yes	//sphere, fast and inaccurate
#define BOX_COLLDETECT 2	//box - slow
#define BS_COLLDETECT 3	//box and sphere - fast and accurate
#define no_texture 0
#define fpoly	//use fpoly if defined else use system poly

#define SHIP_MAX_Y 4500
#define SHIP_MIN_Y -4500

#define CONTROL_PANEL_TOP 390

#define WIND_LEFT 0
#define WIND_TOP 0
#define WIND_BOTTOM 479
#define WIND_RIGHT 639
#define WIND_HEIGHT ((WIND_BOTTOM-WIND_TOP)+1)
#define WIND_WIDTH ((WIND_RIGHT-WIND_LEFT)+1)


#define MONSTER_Z -200	//all non bg objects are at this level
//#define SHOW_CANNONS    //do we show gunshots on the radar

//texture mapping constants
#define BITMAP_SIZE 128

typedef struct
{
int top,left,bottom,right;
} LSRect;

typedef struct
{
float top,left,bottom,right;
} LSFloatRect;


//
// Include all the emulated mac types and functions
//
#include "mac_emulation.h"


//
// These are evil macros
//
#define abs_nobranch(x) abs(x)

//Endian macros
#define Endian16_Swap(value)                 \
        (((((UInt16)value)<<8) & 0xFF00)   | \
         ((((UInt16)value)>>8) & 0x00FF))

#define Endian32_Swap(value)                     \
        (((((UInt32)value)<<24) & 0xFF000000)  | \
         ((((UInt32)value)<< 8) & 0x00FF0000)  | \
         ((((UInt32)value)>> 8) & 0x0000FF00)  | \
         ((((UInt32)value)>>24) & 0x000000FF))

#define ARCH_LITTLE_ENDIAN (defined(SDL_BYTEORDER) && defined(SDL_LIL_ENDIAN) && (SDL_BYTEORDER == SDL_LIL_ENDIAN))
#define ARCH_BIG_ENDIAN (defined(SDL_BYTEORDER) && defined(SDL_BIG_ENDIAN) && (SDL_BYTEORDER == SDL_BIG_ENDIAN))

#endif // _LS_STANDARD_H_


