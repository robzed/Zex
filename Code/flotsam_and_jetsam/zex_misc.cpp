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

//misc functions

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <stdlib.h>
/* example function returns value between min and max */
//oldmac//#include <Windows.h>
//oldmac//#include <Quickdraw.h>
//oldmac//#include <resources.h>
#include "zex_misc.h"
#include "Error_handler.h"
#include "game_defs.h"
#include "splat_control_int.h"
#include "splat_control.h"

//internal protos
void show_vers();


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Oct 2005
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

#define INTERNAL_RAND_MAX 0x7fffffff

static unsigned long int next = 1;

int internal_rand()
{	
	/*
	 * Compute x = (7^5 * x) mod (2^31 - 1)
	 * wihout overflowing 31 bits:
	 *      (2^31 - 1) = 127773 * (7^5) + 2836
	 * From "Random number generators: good ones are hard to find",
	 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
	 * October 1988, p. 1195.
	 */
	
	long hi, lo, x;
	
	/* Can't be initialized with 0, so use another value. */
	if (next == 0)
		next = 123459876;
	hi = next / 127773;
	lo = next % 127773;
	x = 16807 * lo - 2836 * hi;
	if (x < 0)
		x += 0x7fffffff;
	return ((next = x) % ((unsigned long int)INTERNAL_RAND_MAX + 1));
	
}

void internal_srand(unsigned int seed)
{
	next = seed;
}





//Zex's random function simply pulls a number out of an array of random numbers
//generated at init time. Will wrap after max_random_index calls.

#define max_random_index 90000
unsigned short random_array[max_random_index+1];	//filled at init time
int random_array_pos=0;



unsigned short RangedRdm( unsigned short min, unsigned short max )
/* assume that min is less than max */
{
	unsigned short	qdRdm;	/* treat return value as 0-65536 */
	long	range, t;
extern unsigned short random_array[max_random_index+1];
extern int random_array_pos;

//	qdRdm = ZexRandom();
    random_array_pos++;
    if (random_array_pos>max_random_index) random_array_pos=0;
    qdRdm=random_array[random_array_pos];

	range = max - min;
	t = (qdRdm * range) / 65536; 	/* now 0 <= t <= range */
	return( t+min );
}


//returns a number between 0 and 65535
unsigned short ZexRandom()
{
random_array_pos++;
if (random_array_pos>max_random_index) random_array_pos=0;
return random_array[random_array_pos];
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 22 May 2005
// +
// | Replacement Mac OS function.
// |
// | Obtains a pseudorandom integer.
// | function result: A pseudorandom integer, uniformly distributed in the range -32767 to 32767.
// +----------------------------------------------------------------ROUTINE HEADER----

// Note: we assume RAND_MAX is greater or equal to 65534. Enforce this with a compile time check.
#if (INTERNAL_RAND_MAX < 65534)
	#error "RAND_MAX lower than 65534 - which means that Random() will not work correctly. "
#endif

short Random()
{
	// I think this probably breaks the uniform distribution - but it should
	// be good enough for our purposes.
	return (internal_rand() % 65535)-32767;
}


void init_zex_random()
{
extern unsigned short random_array[max_random_index+1];
extern int random_array_pos;
int i;

random_array_pos=0;
for (i=0;i<max_random_index;i++)
{ 
 random_array[i]=Random();
 
}
}

//standardlib functions
//oldmac//void srand(int seed)
//oldmac//{
//oldmac//extern QDGlobals qd;
//oldmac//qd.randSeed=seed;
//oldmac//}

short rand16()
{
return Random();
}

////¥¥This routine prints an 8 bit RAW file with the width and height in the header
////at x,y in screen addr
//void print_raw_resource(ResType the_resource, int theID, int x, int y)
//{
//extern Ptr screenaddr;
//extern int screenwidth;
//
//int raw_w,raw_h;
//Handle the_raw;
//Ptr the_raw_ptr;
//
//the_raw=Get1Resource(the_resource,theID);
//if (!the_raw) report_error("Print_raw_resource: Resource missing");
//the_raw_ptr=*the_raw;
//
//}


//wait for delay_ticks ticks
void user_delay(int delay_ticks)
{
//oldmac//int stop_ticks;
    if (delay_ticks==1) //push up small delays (looks like minimum was 17ms, looks like is now 10 (on 10.10))
    { delay_ticks=2;}
    else
    if (delay_ticks==2)
    { delay_ticks=4;}

//oldmac//stop_ticks=LS_TickCount60()+delay_ticks;
//oldmac//while (LS_TickCount60()<stop_ticks);
	while(delay_ticks >= 6)		// for long delays (e.g. longer than 100ms)
	{
		SDL_Delay(17*6);
		Show_it();						// ensure we see the mouse cursor move on long delays (all long user delays have been checked and this is safe)
		process_standard_event_functions();
		delay_ticks -= 6;
	}
	
	if(delay_ticks != 0) SDL_Delay(delay_ticks*17);			// give it back to the system
}

// no show it here
void user_delay_no_cursor_update_required(int delay_ticks)
{
    if (delay_ticks==1) //push up small delays (minimum was 17ms, looks like is now 10 (on 10.10))
    { delay_ticks=2;}
    else
    if (delay_ticks==2)
    { delay_ticks=4;}
    
	//oldmac//int stop_ticks;
	
	//oldmac//stop_ticks=LS_TickCount60()+delay_ticks;
	//oldmac//while (LS_TickCount60()<stop_ticks);
	while(delay_ticks >= 6)		// for long delays (e.g. longer than 100ms)
	{
		SDL_Delay(17*6);
		process_standard_event_functions();
		delay_ticks -= 6;
	}
	
	if(delay_ticks != 0) SDL_Delay(delay_ticks*17);			// give it back to the system
}


void num_to_string99(int value, Ptr string)
{
int tens,units;
tens=value/10;
units=value-(tens*10);
(*string)=tens+'0';
string++;
(*string)=units+'0';
string++;
(*string)=0;
}

void num_to_string999(int value, Ptr string)
{
int hundreds,tens,units;
hundreds=value/100;
value=value-hundreds*100;
tens=value/10;
value=value-tens*10;
units=value;

(*string)=hundreds+'0';
string++;
(*string)=tens+'0';
string++;
(*string)=units+'0';
string++;
(*string)=0;
}



//oldmac//WindowPtr our_window=0;

void show_loading_screen()
{
extern const RGBColor	rgbRed;
extern const RGBColor	rgbGreen;
extern const RGBColor	rgbWhite;
//oldmac//our_window=GetNewCWindow(128,0,(WindowPtr)-1);
//oldmac//SetPort(our_window);
RGBForeColor(&rgbGreen);
TextSize (9);
TextMode (0);
//MoveTo(260,200);
//DrawString_c("ZEXª ©LSG1999.");
//MoveTo(240,220);
//DrawString_c("Please wait. Loading...");
user_delay(10);

}

#define DRAWSTRING_ROB_TEST 0
#if DRAWSTRING_ROB_TEST
void rob_test(const char* drawstring_rob_test_chars)
{
	Rect my_rect={44,0,440,620};
	RGBBackColor(&rgbBlue);
	MoveTo(30,420);
	Str255 drawstring_rob_test_chars255;
	CToPascal(drawstring_rob_test_chars, drawstring_rob_test_chars255);
	DrawString(drawstring_rob_test_chars255);
	RGBBackColor(&rgbBlack);
	ScrollRect(&my_rect,0,-30,0);	
}
#endif

static void show_zex_loading_title()
{
	MoveTo(230,10);
	DrawString_c("ZEX Copyright LSG 1998-2015.");
	MoveTo(250,30);
	DrawString_c("Please wait. Loading...");	
}
void show_text_in_loading_screen(const char* the_text)
{
	Rect my_rect={44,0,440,620};
int i;
//oldmac//if (our_window==0) return;

//oldmac//SetPort(our_window);
RGBForeColor(&rgbWhite);
// for testing RGBBackColor(&rgbBlue);
RGBBackColor(&rgbBlack);

show_zex_loading_title();
//RGBForeColor(&rgbWhite);

#if DRAWSTRING_ROB_TEST
  TextMode (0);			// 0, 1, 4
  TextSize (24);		// 9, 12, 14, 24
  rob_test(" !\"#$%&'()*+,-./0123456789:;<=>?@A");
  rob_test("ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^");
  rob_test("Connecting... ^_`abcdefghijklmnopqrstuvwxyz{|}~");
  rob_test("A@?>=<;:9876543210/.-,+*)('&%$#\"! ");
  rob_test("^]\\[ZXYWVUTSRQPONMLKJIHGFEDCBA");
  rob_test("~}|{zyxwvutsrqonmlkjihgfedcba`_^");  
  Show_it();
  TextSize (14);		// 9, 12, 14, 24
  rob_test(" !\"#$%&'()*+,-./0123456789:;<=>?@A");
  rob_test("ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^");
  rob_test("Connecting... ^_`abcdefghijklmnopqrstuvwxyz{|}~");
  rob_test("A@?>=<;:9876543210/.-,+*)('&%$#\"! ");
  rob_test("^]\\[ZXYWVUTSRQPONMLKJIHGFEDCBA");
  rob_test("~}|{zyxwvutsrqonmlkjihgfedcba`_^");  
  Show_it();
  TextSize (12);		// 9, 12, 14, 24
  rob_test(" !\"#$%&'()*+,-./0123456789:;<=>?@A");
  rob_test("ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^");
  rob_test("Connecting... ^_`abcdefghijklmnopqrstuvwxyz{|}~");
  rob_test("A@?>=<;:9876543210/.-,+*)('&%$#\"! ");
  rob_test("^]\\[ZXYWVUTSRQPONMLKJIHGFEDCBA");
  rob_test("~}|{zyxwvutsrqonmlkjihgfedcba`_^");  
  Show_it();
  TextSize (9);		// 9, 12, 14, 24
  rob_test(" !\"#$%&'()*+,-./0123456789:;<=>?@A");
  rob_test("ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^");
  rob_test("Connecting... ^_`abcdefghijklmnopqrstuvwxyz{|}~");
  rob_test("A@?>=<;:9876543210/.-,+*)('&%$#\"! ");
  rob_test("^]\\[ZXYWVUTSRQPONMLKJIHGFEDCBA");
  rob_test("~}|{zyxwvutsrqonmlkjihgfedcba`_^");  
  Show_it();
  
#endif

MoveTo(30,420);
DrawString_c(the_text);
//RGBForeColor(&rgbWhite);
#if OGL==1
MoveTo(190,450);
#else
MoveTo(230,450);
#endif

show_vers();
//for (i=0;i<15;i++)
{
ScrollRect(&my_rect,0,-12,0);
//user_delay(1);
}
Show_it();
process_standard_event_functions();
user_delay(3);		// increased from 1 to be more impressive
}

void show_text_in_loading_screen_fcr(Str255 the_text)
{
Rect my_rect={44,0,440,620};
int i;
//oldmac//if (our_window==0) return;

//oldmac//SetPort(our_window);
RGBForeColor(&rgbWhite);


//MoveTo(260,20);
//DrawString("ZEXª ©LSG1999.");
//MoveTo(250,40);
//DrawStringq("Please wait. Loading...");

//RGBForeColor(&rgbWhite);

DrawString(the_text);

//for (i=0;i<15;i++)
{
ScrollRect(&my_rect,0,-12,0);
//user_delay(1);
}
Show_it();

user_delay(1);

}

void show_text_in_loading_screen_nocr(const char* the_text)
{
Rect my_rect={44,0,440,620};
int i;
//oldmac//if (our_window==0) return;

//oldmac//SetPort(our_window);
RGBForeColor(&rgbWhite);

show_zex_loading_title();

//RGBForeColor(&rgbWhite);

MoveTo(30,420);
DrawString_c(the_text);

Show_it();

//user_delay(1);
}

void append_text_to_loading_screen(Str255 the_text)
{
Rect my_rect={44,0,440,620};
int i;
//oldmac//if (our_window==0) return;

//oldmac//SetPort(our_window);
RGBForeColor(&rgbWhite);
DrawString(the_text);
//user_delay(1);
Show_it();

}

void show_error_in_loading_screen(Str255 the_text)
{
Rect my_rect={0,0,440,620};
int i;
//oldmac//if (our_window==0) return;

//oldmac//SetPort(our_window);
TextSize (9);
RGBForeColor(&rgbRed);
TextMode (0);
MoveTo(30,420);
DrawString(the_text);

RGBForeColor(&rgbWhite);
MoveTo(230,450);
show_vers();

for (i=0;i<12;i++)
{
ScrollRect(&my_rect,0,-1,0);
//user_delay(1);
}
Show_it();


}

void show_vers()
{
DrawString_c("Zex Exec v");
DrawString_c(VERS_STRING);
DrawString_c(DATE_STRING);
}


void kill_loading_screen()
{
//oldmac//DisposeWindow(our_window);
//oldmac//our_window=0;
}

//copy t to s
/*void LSstrcpy(char *s, char *t)
{
while((*s=*t) != '\0')
  {
  s++;
  t++;
  }
}*/

// C string length routine

/*int strlen (const char *str)
	{
		char *p;

		p = str;
		while (*p++) ;
		return((long)(--p - str));
	}
*/

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: PascalToC                  | AUTHOR(s): Rob Probin   | DATE STARTED: 18 May 2005
// +
// | convert a pascal string to C
// +----------------------------------------------------------------ROUTINE HEADER----

void PascalToC(const unsigned char *p_str, char *c_str)
{
	int len = *p_str;
	p_str++;
	
	for(int i=0; i<len; i++)
	{
		*c_str = *p_str;
		c_str++; p_str++;
	}
	
	*c_str = 0;		// zero terminate afterwards
}

//convert a C string to pascal
void CToPascal(const char *str, unsigned char *dest_str)
	{
	char *p,*q;
        char temp;
        unsigned char len;

//		len = strlen(str);
//		if (len > 255) len = 255;
                temp=1;
		len=0;
		while (temp!=0)
		{
		temp=str[len];
		dest_str[len+1]=temp;
		len++;
		}
	        dest_str[0]=len-1;
	}

unsigned int zex_ticks;

//called once per game frame to set up zex_ticks
void get_system_ticks()
{
zex_ticks=LS_TickCount60();
}

unsigned int  ZexTickCount()
{
return zex_ticks;
}





