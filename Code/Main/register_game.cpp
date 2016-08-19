/*
 *  register_game.cpp
 *  Zex
 *
 *  Created by Rob Probin on 17/05/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "register_game.h"
#include "pictures.h"
#include	"splat_control.h"
#include "input_control.h"
#include "Error_handler.h"

#include "game_registration_menu.h"
#include "file_io.h"
#include <string>
#include <sstream>
#include <iostream>

extern prefs gzex_prefs;
using std::string;

//
// Local functions
//
static void checkRegister(string name, string code);
static int calculate_code(string name_str);
static void show_register(bool success);
static bool blacklist(unsigned int code_to_check);

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: handle_register_game       | AUTHOR(s): Rob Probin   | DATE STARTED: 17 May 2006
// +
// | DESCRIPTION: Register the game
// +----------------------------------------------------------------ROUTINE HEADER----

void handle_register_game()
{
	GameRegistrationMenu our_game_reg;
	if(our_game_reg.user_accepted())
	{
		// the user entered valid registration details
		string name = our_game_reg.get_reg_name();
		string code = our_game_reg.get_reg_code();
		
		if(name != "" && code != "")		// don't process on blank strings ... user might have pressed return by mistake
		{
			// process registration here
			checkRegister(name,code);
		}
	}
}


//General purpose, type safe string to num convert
template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

//
//Check if codes match
//
static void checkRegister(string name, string code)
{
	unsigned int calc_code=calculate_code(name);
	unsigned int string_code;
	if (from_string<unsigned int>(string_code, code, std::dec))	//only if we converted the code string OK do we go on to check if registered
	{
		if (calc_code==string_code && !blacklist(calc_code))
		{
			//Register OK
			gzex_prefs.registered=1;	//this is all we need to do
			gzex_prefs.registration_key=string_code;	//Save also the reg code so we can blacklist it in future
			write_prefs();
			show_register(true);
		}
		else
		{
			//Register failed
			show_register(false);
		}
	}
	else
	{
		//Register failed - invalid numeric code
		show_register(false);
	}
	
	
}

//
// This function removes registrations from already registered games that have been blacklisted.
// Obviously this will only work on upgrades - but that's probably good enough to discourage real players...
//
void check_game_parameters(void)
{
	if(gzex_prefs.registered==1)		// only perform check on a registered game
	{
		if (blacklist(gzex_prefs.registration_key))
		{
			//Registered code has been blacklisted...
			gzex_prefs.registered=0;	// de-register game number...
			write_prefs();
		}
	}
}

//
// Check for a blacklisted registration in the background whilst playing the game...
//
void check_game_param_slowly(void)
{
	static int counter = 0;
	counter++;
	
	if(counter > 120)
	{
		counter = 0;
		check_game_parameters();
	}
	
}

//
// Check if a code has been blacklisted
//
static bool blacklist(unsigned int code_to_check)
{
	const unsigned int encrypting = 0xad931ab4;
	unsigned int blacklist_table[] = {
		0 ^ encrypting,
		0xffffffff ^ encrypting,
		165293979 ^ encrypting		// Rob's code
	};
	
	int table_size = sizeof(blacklist_table) / sizeof(unsigned int);
	for(int i=0; i<table_size; i++)
	{
		if((code_to_check ^ encrypting) == blacklist_table[i])
		{
			return true;
		}
	}
	
	return false;
}

//
// Show registered or not
//
static void show_register(bool success)
{
int prefs_x,prefs_y;
int button_state;
Point mouse_xy;
Handle picture_H;
LSRAW* the_picture;
int return_val, real_return_val, old_button_state;

	if (success)
	{
		picture_H=GetZexPicture ('ZRAW',160);	//reg ok pict   
		if (picture_H==0) report_error_c("Resource missing: ZRAW 160","",4);
	}
	else
	{
		picture_H=GetZexPicture ('ZRAW',161);	//reg failed pict   
		if (picture_H==0) report_error_c("Resource missing: ZRAW 161","",4);
	}
	LS_HLock(picture_H);

    prefs_x=120; prefs_y=80;
    LS_ShowCursor();
    ZexGetMouse (&mouse_xy);
	button_state=game_fire_button();
	while(button_state==1)
	{
		button_state=game_fire_button();
	}
	old_button_state=button_state;
	return_val=real_return_val=0;
	while(real_return_val==0)
	{
		return_val=0;
		erase_zex_rect(639,479);	//in pictures

		ZexGetMouse (&mouse_xy);
		the_picture = (LSRAW*)*picture_H;
		//erase_zex_rect(640,480);	//in pictures
 
		print_pict_scaled(the_picture,prefs_x,prefs_y,0,1);	//Print with transparency

		button_state=game_fire_button();
		
		Show_it();
		if (button_state==1 && old_button_state==0)	//if up and was down
		real_return_val = 1;
		else
		real_return_val=0;
		old_button_state=button_state;

	}
    LS_DisposeHandle(picture_H);
	LS_HideCursor();

}
 // ------------------------From Lightsoft_Register.app
// +----------------------------------------+-----------------------------------------
// | TITLE: calculate_code                  | :stu:20030302 
// +
// | DESCRIPTION:
// | This standard routine takes a string and converts it to an unsigned 32 bit
// | registration code which is returned to the caller
// +----------------------------------------------------------------ROUTINE HEADER----
// These arrays obfuscate the normal sequence of alpha characters.
// ***Note that non - western encoded alphabets will not be obfuscated
// and hence the code generated may not be as secure for these encodings***
 
static char obsfL_array[]="diegsxhlzkbpfyumorvncwtaqj";
static char obsfU_array[]="CAFQZDSTBJEKMXPONILUHVRWYG";

// ------------------------------------------------------------
//HWC code
static int calculate_code(string name_str)
{
 int name_length, n, m;
 char the_char;
 unsigned int xor_val=0xA7D33984;	//Product specific, must match LSReg
 unsigned int code=0x78777c52; //Product specific, must match LSReg

 name_length=name_str.length(); 
 //calc code
 for (n=0;n<name_length;n++)	//C++ change
 {
   the_char=name_str[n];	//C++ change
   //Obfuscate the character to increase the difference between successive characters
   if ((the_char>='a') && (the_char<='z')) { the_char=obsfL_array[the_char-'a']; }
   else
   if ((the_char>='A') && (the_char<='Z')) { the_char=obsfU_array[the_char-'A']; }
   
   //now modify the code by this character      
   code+=(the_char<<n)^xor_val;
   for (m=0;m<the_char;m++)
   {
     xor_val+=xor_val/256;
     xor_val+=xor_val-2112;
     //Have a go at all 32 bits
     code^=the_char<<24;
     code^=xor_val<<16;
     code^=the_char<<8;
     code^=xor_val;
	 code+=m;
	 code+=m*13;
	 code-=m*35;
	 code+=7;
   }
 }  //end of calc code loop
 
 return code;
}


/* CVS LOG
*
* $Log: register_game.cpp,v $
* Revision 1.18  2006/06/25 11:16:08  rob
* Zex2005: Game has now got a blacklist list which can be used to remove rogue serial numbers flying around the internet when we update the application.
*
* Revision 1.17  2006/05/28 21:47:06  rob
* Zex2005: Removed console printing.
*
* Revision 1.16  2006/05/27 23:39:49  stu
* Zex2005: Code: We now save the registration key in the prefs. This fixes bug #195
*
* Revision 1.15  2006/05/20 12:01:29  stu
* Zex2005: Code: Registration status should now be persistent. Once registered the only way to unregister is to delete the preferences.
*
* Revision 1.14  2006/05/20 11:43:35  stu
* Zex2005: Code: Now show whether registration succeeded or not.
*
* Revision 1.11  2006/05/18 07:08:10  rob
* Zex2005: Menu for registration code entry.
*
*
*/
