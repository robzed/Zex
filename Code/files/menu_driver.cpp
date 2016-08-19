/*
 *  menu_driver.cpp
 *  Z_dungeon
 *
 *  Created by Rob Probin on Sun May 04 2003.
 *  Copyright (c) 2003 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/08/13 11:39:00 $
 * $Revision: 1.8 $
 *
 * For log see end of file...
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "stdlib.h"

//#include "general_include.h"
#include "menu_driver.h"
//#include "graphic_utilities.h"
//#include "Game_Controller.h"
//#include "Surface_Controller.h"
//#include "Player_Interface.h"
//#include "system_interface.h"
#include "timing.h"
//#include "drawing_primitives.h"
//#include "sound_actions.h"
//#include "text_loader.h"
//#include "fonts.h"

#include "key_setup_display.h"
#include "mac_emulation.h"
#include "splat_control.h"
#include "Error_handler.h"
#include "sound.h"
#include "fpoly4.h"
#include "get_text_string.h"
#include "zdungeon_printing_emulation.h"

// ***********************************************************************************
// * CONSTANTS 
// *

const bool STARS_ON_MENU_BACKGROUND = true;		// select as true to make stars appear

const bool DEBUG_MENU_MOUSE_AREAS = false;		// print the mouse areas for all menus
const bool DEBUG_MENU_FRAME_RATE = false;		// print various menu frame rate data to console


// ***********************************************************************************
// * INTERNAL CLASSES
// *



// ***********************************************************************************
// * TYPE DEFS AND ENUMERATORS
// *

enum button_event_type {
	BUTTON_EVENT_MOVE = 0,
	BUTTON_EVENT_DOWN = 1,
	BUTTON_EVENT_UP = 2
};


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

// static bool menu_mode_active_flag=false;
static bool menu_mode_terminate_flag=false;

static fade_timer* fade;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

static void run_all_loop_callbacks(menu_table_type *this_menu);
static void init_callbacks(menu_table_type *this_menu);
static void display_menu(menu_table_type *this_menu);

static void load_menu_graphics(menu_table_type *this_menu);
//static int load_single_menu_graphic(menu_table_type *this_menu, const char *raw_filename, bool load_with_alpha);
static void reset_menu_variables(menu_table_type *this_menu);

static void handle_menu_input_events(menu_table_type *this_menu, bool (*key_event_function)(SDL_Event event));

static void menu_key_press(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key);
static void menu_key_release(menu_table_type *this_menu, SDLKey key);

static bool menu_direct_key_search(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key);
static bool menu_direct_unkey_search(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key);
static void menu_arrow_key(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key);
static void menu_arrow_unkey(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key);

static void menu_mouse_control(menu_table_type *this_menu, int x, int y, int button, button_event_type button_event);
void mouse_highlight(menu_table_type *this_menu, int x, int y);
void init_key_mouse_system(void);

// highlight items
void highlight_one_forward(menu_table_type *this_menu);
void highlight_one_backward(menu_table_type *this_menu);
void select_highlighted_option(menu_table_type *this_menu);

// find highlight items
menu_table_type *find_highlighted_item(menu_table_type *this_menu);
menu_table_type *search_for_highlightable_items(menu_table_type *this_menu);
menu_table_type *search_for_highlightable_items_from_end(menu_table_type *this_menu);
menu_table_type *search_for_highlightable_items_backward(menu_table_type *start, menu_table_type *this_menu);

// clicking items
void do_click_item(menu_table_type *this_menu, menu_driver_type actioning_driver);
void do_unclick_item(menu_table_type *this_menu, menu_driver_type actioning_driver);
void perform_action(menu_table_type *this_menu);

// debug
static void draw_mouse_areas(menu_table_type *this_menu);

// ***********************************************************************************
// * ACTUAL CODE
// *

//
//  P U B L I C   F U N C T I O N S
//



//menu::menu(menu_table_type *menu_to_link_with)
//: cls_flag(true), this_menu(menu_to_link_with)
//{
//}
//
//menu::cls_on_every_frame(bool enable_flag)
//{
//	cls_flag = enable_flag;
//}
//


// TITLE:        run_menu
// AUTHOR(s):    Rob Probin
// DATE STARTED: 6th May 2003
//
// DESCRIPTION:  Generic menu handler
//               
void run_menu(menu_table_type *this_menu, bool cls_on_every_frame)
{	
	//menu our_menu(this_menu);
	//our_menu.cls_on_every_frame(cls_on_every_frame);
	
	run_menu_advanced(this_menu, cls_on_every_frame, NULL);
}

// TITLE:        run_menu_advanced
// AUTHOR(s):    Rob Probin
// DATE STARTED: 27th April 2004
//
// DESCRIPTION:  Advanced Generic menu handler
//
void run_menu_advanced(menu_table_type *this_menu, bool cls_on_every_frame, bool (*key_event_function)(SDL_Event event))
{
	int old_cursor_state = Zex_ShowCursor(SDL_QUERY);
	Zex_ShowCursor(SDL_ENABLE);
	
	init_frame_time();	// added for Zex so timing.cpp is set up
	set_poly_clipping(0,639,0,479);
	
menu_mode_terminate_flag=false;		// don't quit yet

init_screen_stars(); 

//int graphics_position = get_graphics_loaded_position();

init_key_mouse_system();

int previous_unicode = SDL_EnableUNICODE(1); // turn on unicode translation
zex_polling_switch(false);

init_callbacks(this_menu);		// initialise variables that haven't been before
reset_menu_variables(this_menu);	// set all the control variables up - including the button state based on the value
load_menu_graphics(this_menu);		// load the graphics for the menu
cls();					// clear the screen
//timed_fade_screen(1.0,1000);		// fade in the screen

fade_timer my_fade_value;
fade = &my_fade_value;

while(/* no_game_requests() &&*/ !menu_mode_terminate_flag)
  {
  if(cls_on_every_frame) cls();		// clear the screen everytime around the main loop to avoid artifacts?

  if(STARS_ON_MENU_BACKGROUND) 
  {
	  show_screen_stars();
  }
  
//  screen_effects_service();		// do fades, and stuff
  display_menu(this_menu);		// display the menu
  
  handle_menu_input_events(this_menu, key_event_function);	// check for mouse and keys
  run_all_loop_callbacks(this_menu);	// call any external functions
  
  Show_it();
  //SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);	// update the WHOLE screen
  measure_frame_time();
  }

// fade out  
//timed_fade_screen(0.0,1000);

zex_polling_switch(true);
//unload_graphics_to_position(graphics_position);		// unload all the menu graphics from the surface controller
SDL_EnableUNICODE(previous_unicode);	// turn unicode off (if off previously)

/*
while(screen_effects_not_completed())
  {
  screen_effects_service();
  measure_frame_time();
  }
*/

destroy_screen_stars();

cls();

if(DEBUG_MENU_FRAME_RATE)
  {
  printf("Menu Frame Time Data\n");
  printf("Number of Frames = %lu\n",(unsigned long) total_game_frames());
  printf("Number of milliseconds = %lu\n",(unsigned long) total_game_ticks());
  printf("Average FPS = %f\n\n",(float)overall_average_FPS());
  }

menu_mode_terminate_flag=false;		// don't quit any nested menus

Zex_ShowCursor(old_cursor_state);
}


// TITLE:        
// AUTHOR(s):    Rob Probin
// DATE STARTED: 5th June 2003
//
// DESCRIPTION:  
// 

bool have_any_menu_values_changes(menu_table_type *this_menu)
{
bool this_is_last_item;

do	// do all of the menu entries
  {
  
  if((this_menu->save_callback_function != NULL) && 
           (this_menu->original_value != this_menu->value)) 
    { 
    return true;
    }
  
  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item

return false;
}

// TITLE:        
// AUTHOR(s):    Rob Probin
// DATE STARTED: 6th May 2003
//
// DESCRIPTION:  
//               

void passback_menu_values(menu_table_type *this_menu, int user_val)
{
bool this_is_last_item;

do	// do all of the menu entries
  {
  if(this_menu->save_callback_function != NULL)		// if there is a function
      { this_menu->save_callback_function(this_menu, user_val); }	// call any external function

  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item
}



// TITLE:        terminate_menu_mode
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  Finished with menu mode? Use this to stop it.
//               
void terminate_menu_mode(void)
{
menu_mode_terminate_flag=true;
}


//  ====================================
//
//  L O W   L E V E L  F U N C T I O N S
//
//  ====================================

// TITLE:        run_all_loop_callbacks
// AUTHOR(s):    Rob Probin
// DATE STARTED: 28th May 2003
//
// DESCRIPTION:  
//

static void run_all_loop_callbacks(menu_table_type *this_menu)
{
bool this_is_last_item;

do	// do all of the menu entries
  {
  if(this_menu->run_loop_callback_function != NULL)		// if there is a function
      { this_menu->run_loop_callback_function(this_menu); }	// call any external function

  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item
}


// TITLE:        init_callbacks
// AUTHOR(s):    Rob Probin
// DATE STARTED: 3rd June 2003
//
// DESCRIPTION:  
//

static void init_callbacks(menu_table_type *this_menu)
{
bool this_is_last_item;

do	// do all of the menu entries
  {
  if(this_menu->init_callback_function != NULL)		// if there is a function
      { this_menu->init_callback_function(this_menu); }	// call any external function

  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item
}




// TITLE:        display_menu
// AUTHOR(s):    Rob Probin
// DATE STARTED: 6th May 2003
//
// DESCRIPTION:  
//               

static void display_menu(menu_table_type *this_menu)
{
	bool this_is_last_item;
	SDL_Rect rect;
	int string_name;
	
	fade->single_step();
	
	do	// do all of the menu entries
	{
		rect.x = this_menu->print_location_x; rect.y = this_menu->print_location_y;
		
		if(this_menu->menu_control != non_display_item)
		{
			if(this_menu->draw_graphics)
			{
				if      (this_menu->current_state == CURRENT_STATE_STANDARD) { string_name = this_menu->standard_string_name; }
				else if (this_menu->current_state == CURRENT_STATE_CLICKED) { string_name = this_menu->clicked_string_name; }
				else if (this_menu->current_state == CURRENT_STATE_SELECTED) { string_name = this_menu->selected_string_name; }
				else { string_name = this_menu->standard_string_name; }
				
				// now overlay the hightlight graphic
				if(this_menu->highlighted && !(this_menu->current_state == CURRENT_STATE_CLICKED))
				{
					FadeForeColour(fade->get_fade_value());
				}
				
				if(string_name != empty_text)
				{
					print_formatted_string(rect.x,rect.y,string_name); // display the string as a formatted graphic
					//        display_graphic(string_id, screen, &rect);		// draw the main graphic!!
				}
				
				// now overlay the hightlight graphic
				if(this_menu->highlighted && !(this_menu->current_state == CURRENT_STATE_CLICKED))
				{
					string_name = this_menu->highlight_string_name;
					
					if(string_name != empty_text)
					{
						//display_graphic(gid, screen, &rect);		// draw the highlight graphic!!
						print_formatted_string(rect.x+this_menu->highlight_offset_x,rect.y,string_name); //x space for the highlight graphic
					}
				} // if highlighted
				
			} // if draw graphics
			
			if(this_menu->special_display_function != NULL)		// if there is a function
			{ this_menu->special_display_function(this_menu); }	// call any external function

			FadeForeColour(0);		// turn off fade (if any)

		} // if not non-display-item
		
		draw_mouse_areas(this_menu);
		
		this_is_last_item = this_menu->last_item;
		this_menu++;					// goto next item
	} while(this_is_last_item == false);		// abort if we reached last item
	
	
}

// TITLE:        draw_mouse_areas
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  Read the event queue for input events and process those
//               
static void draw_mouse_areas(menu_table_type *this_menu)
{
if(DEBUG_MENU_MOUSE_AREAS)
	{
	//SDL_Surface *screen = the_screen::get_instance()->get_screen_ref();

	//lsg_draw_box(screen,
	//this_menu->mouseable_area.x, 
	//this_menu->mouseable_area.y, 
	//this_menu->mouseable_area.x+this_menu->mouseable_area.w, 
	//this_menu->mouseable_area.y+this_menu->mouseable_area.h, 255, 0, 0);
	
	Rect r = { 
		this_menu->mouseable_area.y, 
		this_menu->mouseable_area.x, 
		this_menu->mouseable_area.y+this_menu->mouseable_area.h,
		this_menu->mouseable_area.x+this_menu->mouseable_area.w
	};

	RGBColor c = { 65535, 0, 0 };		// aka red

	RGBForeColor(&c);

	FrameRect(&r);		// draw a box
	}
}

// TITLE:        handle_menu_input_events
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  Read the event queue for input events and process those
//
//               Return type of embedded function is whether menu can process the key event (ignored if not a key event)
//
static void handle_menu_input_events(menu_table_type *this_menu, bool (*key_event_function)(SDL_Event event))
{
	SDL_Event event;
	
	while ( SDL_PollEvent(&event) ) {
		
		update_zex_main_event_state(event);	// keep Zex's state up to date...
											// (Zex is entirely state polled)
		// also process for the 
		switch (event.type) {
			
			case SDL_MOUSEMOTION:
				menu_mouse_control(this_menu, event.button.x,event.button.y,event.button.button, BUTTON_EVENT_MOVE);
				break;
				
			case SDL_MOUSEBUTTONUP:
				//printf("mouse button up, key2rel=%i keyAwaitRel=%i mousebut=%i\n",key_to_release, key_awaiting_release, mouse_button_currently_down);
				menu_mouse_control(this_menu, event.button.x,event.button.y,event.button.button, BUTTON_EVENT_UP);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				//printf("mouse button down, key2rel=%i keyAwaitRel=%i mousebut=%i\n",key_to_release, key_awaiting_release, mouse_button_currently_down);
				menu_mouse_control(this_menu, event.button.x,event.button.y,event.button.button, BUTTON_EVENT_DOWN);
				break;
				
			case SDL_KEYDOWN:
			{
				//printf("key down, key2rel=%i keyAwaitRel=%i mousebut=%i unicode=%i\n",key_to_release, key_awaiting_release, mouse_button_currently_down, event.key.keysym.unicode);
				bool menu_key_handler = true;

				if(key_event_function)
				{
					menu_key_handler = key_event_function(event);
				}
				if(menu_key_handler)
				{
					menu_key_press(this_menu, event.key.keysym.sym, event.key.keysym.unicode);
				}
				break;
			}
				
			case SDL_KEYUP:
			{
				//printf("key up, key2rel=%i keyAwaitRel=%i mousebut=%i unicode=%i\n",key_to_release, key_awaiting_release, mouse_button_currently_down, event.key.keysym.unicode);
				bool menu_key_handler = true;
				
				if(key_event_function)
				{
					menu_key_handler = key_event_function(event);
				}
				if(menu_key_handler)
				{
					menu_key_release(this_menu, event.key.keysym.sym);	// no unicode on key up
				}
				break;
			}

			case SDL_QUIT:
				// this is what the Zex event processing does - it cleans up SDL via atexit().
				exit(0);			// quit immediately
//				request_quit();
				break;
			default:
				break;
		} // switch(event.type)
		
	} // while(SDL_PollEvent())
	
}




//
// Key and mouse local data (1st June 2003)
// 
static bool key_awaiting_release;				// key routines record when a key is press, and we use it to lockout other selection routines
static SDLKey key_to_release;
static Uint16 waiting_unicode_release;				// unicode up is invalid, so we store it

static bool mouse_button_currently_down = false;		// assume button is up to start


// TITLE:        init_key_mouse_system
// AUTHOR(s):    Rob Probin
// DATE STARTED: 2nd June 2003
//
// DESCRIPTION:  init mouse and keyboard variables
//               
void init_key_mouse_system(void)
{
key_awaiting_release = false;
mouse_button_currently_down = false;
}


// TITLE:        menu_mouse_control
// AUTHOR(s):    Rob Probin
// DATE STARTED: 6th May 2003
//
// DESCRIPTION:  check for mouse stuff
//               

static void menu_mouse_control(menu_table_type *this_menu, int x, int y, int button, button_event_type button_event)
{
menu_table_type *highlighted_item;

if(key_awaiting_release) return;		// don't do anything if a key is pressed

if(!mouse_button_currently_down) mouse_highlight(this_menu,x,y); // mouse moves & button presses change highlight, but
                                                                // we don't update whilst the button is down
                                                                
// let's handle button presses
if((button_event == BUTTON_EVENT_DOWN && !mouse_button_currently_down) || (button_event == BUTTON_EVENT_UP && mouse_button_currently_down))	// we check mouse_button_currently_down just in case multiple buttons have been pressed
  {
  if(button_event == BUTTON_EVENT_DOWN) mouse_button_currently_down = true;	// track button up & down 
  if(button_event == BUTTON_EVENT_UP) mouse_button_currently_down = false;
  
  highlighted_item = find_highlighted_item(this_menu);
  if(highlighted_item != NULL)			// clicks only matter if something is highlighted to action
    {
    if(button_event == BUTTON_EVENT_DOWN)	// any button click
      {
      do_click_item(highlighted_item, DRIVER_MOUSE);
      }

    if(button_event == BUTTON_EVENT_UP)		// any button release
      {
      do_unclick_item(highlighted_item, DRIVER_MOUSE);
      }
      
    } // if an item highlighted
    
  } // end button events
  
} // end menu_mouse_control


// TITLE:        mouse_highlight
// AUTHOR(s):    Rob Probin
// DATE STARTED: 31st May 2003
//
// DESCRIPTION:  Unhighlights previous highlights, and highlights any that the mouse cursor is over...
//               Only called when something happens to the mouse...      
void mouse_highlight(menu_table_type *this_menu, int x, int y)
{
bool this_is_last_item;
bool found_item;
menu_table_type *highlighted_item;

//
// get rid old highlight
//
  highlighted_item = find_highlighted_item(this_menu);
  if(highlighted_item != NULL)			// clicks only matter if something is highlighted to action
    {
    highlighted_item->highlighted = false;
    }

//
// go around the loop looking for areas to highlight - but only highlight the first.
//
found_item=false;
do	// do all of the menu entries
  {
  if(this_menu->menu_control == toggle_item || this_menu->menu_control == event_item)	// only do for highlightable items...
    {
    if(this_menu->mouseable_area.w > 0 && this_menu->mouseable_area.h > 0)	// valid mouse area?
      {
      if( x >= (this_menu->mouseable_area.x) &&
          y >= (this_menu->mouseable_area.y) &&
          x < ((this_menu->mouseable_area.x) + (this_menu->mouseable_area.w)) &&
          y < ((this_menu->mouseable_area.y) + (this_menu->mouseable_area.h)) 
        )
        {
        found_item=true;
        break;
        }
      }
    }
  
  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item

if(found_item==true)
  {
  this_menu->highlighted = true;  
  }
}


// TITLE:        menu_key_press
// AUTHOR(s):    Rob Probin
// DATE STARTED: 6th May 2003
//
// DESCRIPTION:  check direct keys and arrow keys
//               


static void menu_key_press(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key)
{  
if(!key_awaiting_release && !mouse_button_currently_down)
  {
  bool key_processed = menu_direct_key_search(this_menu, key, unicode_key); // now let's check the menu's for direct keys

  // only process keys that weren't mentioned in the menu
  if(!key_processed) menu_arrow_key(this_menu, key, unicode_key);  // now let's handle the arrow keys (along with space, return and enter)
  }
}



// TITLE:        menu_key_release
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  check direct keys for release
//               
// Note: No unicode available on keyup, so we have to work around that...

static void menu_key_release(menu_table_type *this_menu, SDLKey key)
{
bool correct_key_release = false;
Uint16 last_unicode_key;			// what unicode key it was

if(key_awaiting_release && key == key_to_release)
  {
  key_awaiting_release = false;	// we aren't waiting for this key to release
  correct_key_release = true;	// but we need to know that it wasn't just ANY random key that was released
  last_unicode_key = waiting_unicode_release;		// restore the unicode valid of this key...
  }

if(correct_key_release && !key_awaiting_release && !mouse_button_currently_down)
  {
  bool key_processed = menu_direct_unkey_search(this_menu, key, last_unicode_key); // just check for direct key releases - restore the buttons...
  
  // only process keys that weren't mentioned in the menu
  if(!key_processed) menu_arrow_unkey(this_menu, key, last_unicode_key);
  }
}


// TITLE:        menu_direct_key_search
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  check direct keys
//               

static bool menu_direct_key_search(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key)
{
bool this_is_last_item;
bool key_processed = false;

do	// do all of the menu entries
  {
  if((this_menu->menu_control == toggle_item || this_menu->menu_control == event_item) && this_menu->direct_key_value != -1)
    {		// (NOTE: Remove case - and make menu structure neater at some point!)
    if((this_menu->direct_key_value == (signed)key && this_menu->unicode_key == false)
       || ( this_menu->direct_key_value == unicode_key && this_menu->unicode_key == true ))
      {
      do_click_item(this_menu, DRIVER_DIRECT_KEY);

      key_awaiting_release=true;
      key_to_release=key;
      waiting_unicode_release = unicode_key;
	  key_processed = true;
      }
    }

  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item
  

return key_processed;
}


// TITLE:        menu_direct_unkey_search
// AUTHOR(s):    Rob Probin
// DATE STARTED: 29th May 2003
//
// DESCRIPTION:  check direct keys
//               
static bool menu_direct_unkey_search(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key)
{
bool this_is_last_item;
bool key_processed = false;

do	// do all of the menu entries
  {
  if((this_menu->menu_control == toggle_item || this_menu->menu_control == event_item) && this_menu->direct_key_value != -1)
    {
    // if it is the key for this menu option		(NOTE: Remove case - and make menu structure neater at some point!)
    if((this_menu->direct_key_value == (signed)key && this_menu->unicode_key == false)
       || ( this_menu->direct_key_value == unicode_key && this_menu->unicode_key == true ))
      {
        do_unclick_item(this_menu, DRIVER_DIRECT_KEY);
		key_processed = true;
      }
    }

  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item


return key_processed;
}


// TITLE:        do_click_item
// AUTHOR(s):    Rob Probin
// DATE STARTED: 29th May 2003
//
// DESCRIPTION:  
//               
void do_click_item(menu_table_type *this_menu, menu_driver_type actioning_driver)
{
      this_menu->current_state = CURRENT_STATE_CLICKED;		  // override other states
      this_menu->internal_selection_driver_method = actioning_driver; // override other drivers

      this_menu->value = !this_menu->value;				// toggle the value for 
}

// TITLE:        do_unclick_item
// AUTHOR(s):    Rob Probin
// DATE STARTED: 29th May 2003
//
// DESCRIPTION:  
//               
void do_unclick_item(menu_table_type *this_menu, menu_driver_type actioning_driver)
{
if(this_menu->internal_selection_driver_method == actioning_driver)
  {
  
  if(this_menu->menu_control == toggle_item)
    {
    if(this_menu->value)
      {
      this_menu->current_state = CURRENT_STATE_SELECTED;
      }
    else
      {
      this_menu->current_state = CURRENT_STATE_STANDARD;
      }

    }
  else // not toggle type!
    {
    this_menu->current_state = CURRENT_STATE_STANDARD;		// reset (whatever it was doing)
    this_menu->internal_selection_driver_method = DRIVER_NONE;	// and remove the driver
    }
	
  perform_action(this_menu);		// actually perform action now we've done all the menu configuration

  }

}

// TITLE:        perform_action
// AUTHOR(s):    Rob Probin
// DATE STARTED: 29th May 2003
//
// DESCRIPTION:  
//        
void perform_action(menu_table_type *this_menu)
{
	play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);

//      sound_play_menu_beep();
      if(this_menu->click_callback_function != NULL)			// if there is a function
          { this_menu->click_callback_function(this_menu); }		// call any external function
}




// TITLE:        menu_arrow_key
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  check arrow keys
//               

static void menu_arrow_key(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key)
{
// if arrow right or down select one forward
if(key == SDLK_RIGHT || key == SDLK_DOWN)
  {
  highlight_one_forward(this_menu);
  key_awaiting_release=true;
  key_to_release=key;
  waiting_unicode_release = unicode_key;
  }
  
// if arrow left or up select one backward
if(key == SDLK_LEFT || key == SDLK_UP)
  {
  highlight_one_backward(this_menu);
  key_awaiting_release=true;
  key_to_release=key;
  waiting_unicode_release = unicode_key;
  }

// if enter, or keypad enter, or space bar and any selected, then select that option
if(key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_SPACE)
  {
  select_highlighted_option(this_menu);
  key_awaiting_release=true;
  key_to_release=key;
  waiting_unicode_release = unicode_key;
  }

}

// TITLE:        select_highlighted_option
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  
//               

void select_highlighted_option(menu_table_type *this_menu)
{
menu_table_type *highlighted_item;

highlighted_item = find_highlighted_item(this_menu);
if(highlighted_item != NULL)
  {
  do_click_item(highlighted_item, DRIVER_ARROW_KEY);
  }

}

// TITLE:        menu_arrow_unkey
// AUTHOR(s):    Rob Probin
// DATE STARTED: 29th May 2003
//
// DESCRIPTION:  check arrow keys for release
//               

static void menu_arrow_unkey(menu_table_type *this_menu, SDLKey key, Uint16 unicode_key)
{
menu_table_type *highlighted_item;

// if enter, or keypad enter, or space bar and any selected, then select that option
if(key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_SPACE)
  {
  
  highlighted_item = find_highlighted_item(this_menu);
  if(highlighted_item != NULL)
    {
    do_unclick_item(highlighted_item, DRIVER_ARROW_KEY);
    }
    
  }

}



// TITLE:        highlight_one_forward
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  
//               

void highlight_one_forward(menu_table_type *this_menu)
{
menu_table_type *highlighted_item;
menu_table_type *next_highlightable_item;

highlighted_item = find_highlighted_item(this_menu);
if(highlighted_item == NULL)			// no highlighted item exists
  {
  next_highlightable_item = search_for_highlightable_items(this_menu); // search for possible from start
  }
else if(highlighted_item->last_item == true)			// search from start if we are at end
 {
 next_highlightable_item = search_for_highlightable_items(this_menu);
 }
else		// search current position plus 1
 {
 next_highlightable_item = search_for_highlightable_items(highlighted_item+1);
 if(next_highlightable_item == NULL)	// wrap
   {
   next_highlightable_item = search_for_highlightable_items(this_menu);
   }
 }
 
if(highlighted_item != NULL)	// de-select old one
  {
  highlighted_item->highlighted = false;
  }

if(next_highlightable_item != NULL)	// if one actually exists we can highlight
  {
  next_highlightable_item->highlighted = true;
  }
}

// TITLE:        highlight_one_backward
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  
//               

void highlight_one_backward(menu_table_type *this_menu)
{
menu_table_type *highlighted_item;
menu_table_type *next_highlightable_item;

highlighted_item = find_highlighted_item(this_menu);
if(highlighted_item == NULL)			// no highlighted item exists
  {
  next_highlightable_item = search_for_highlightable_items_from_end(this_menu); // search for possible from start
  }
else if(highlighted_item == this_menu)			// search from end if we are at start
 {
 next_highlightable_item = search_for_highlightable_items_from_end(this_menu);
 }
else		// search current position plus 1
 {
 next_highlightable_item = search_for_highlightable_items_backward(this_menu, highlighted_item-1);
 if(next_highlightable_item == NULL)	// wrap
   {
   next_highlightable_item = search_for_highlightable_items_from_end(this_menu);
   }
 }
 
if(highlighted_item != NULL)	// de-select old one
  {
  highlighted_item->highlighted = false;
  }

if(next_highlightable_item != NULL)	// if one actually exists we can highlight
  {
  next_highlightable_item->highlighted = true;
  }
}





// TITLE:        find_highlighted_item
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  
//               

menu_table_type *find_highlighted_item(menu_table_type *this_menu)
{
bool this_is_last_item;

do	// do all of the menu entries
  {
  if(this_menu->menu_control == toggle_item || this_menu->menu_control == event_item)
    {
    if(this_menu->highlighted)
      {
      return this_menu;
      }
    }
  
  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item

return NULL;
}



// TITLE:        search_for_highlightable_items
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  
//               

menu_table_type *search_for_highlightable_items(menu_table_type *this_menu)
{
bool this_is_last_item;

do	// do all of the menu entries
  {
  if(this_menu->menu_control == toggle_item || this_menu->menu_control == event_item)
    {
    if(this_menu->mouseable_area.w > 0 && this_menu->mouseable_area.h > 0)
      {
      return this_menu;
      }
    }
  
  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item

return NULL;
}


// TITLE:        search_for_highlightable_items_from_end
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  
//               
menu_table_type *search_for_highlightable_items_from_end(menu_table_type *this_menu)
{
bool this_is_last_item;
menu_table_type *start;

start = this_menu;	// record start

//
// First find end of list
//
do	// do all of the menu entries
  {
  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item

  this_menu--;	// this_menu is now pointing at last item
  
// now use existing routine to find previous items
return search_for_highlightable_items_backward(start, this_menu);
}


// TITLE:        search_for_highlightable_items_backward
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  
//               
menu_table_type *search_for_highlightable_items_backward(menu_table_type *start, menu_table_type *this_menu)
{
bool this_is_first_item;

do	// do all of the menu entries
  {
  if(this_menu->menu_control == toggle_item || this_menu->menu_control == event_item)
    {
    if(this_menu->mouseable_area.w > 0 && this_menu->mouseable_area.h > 0)
      {
      return this_menu;
      }
    }
  
  this_is_first_item = (this_menu==start);
  this_menu--;					// goto previous item
  } while(this_is_first_item == false);		// abort if we reached last item

return NULL;
}





// TITLE:        reset_menu
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  Sets up the start conditions
//               

static void reset_menu_variables(menu_table_type *this_menu)
{
bool this_is_last_item;

do	// do all of the menu entries
  {
  this_menu->internal_selection_driver_method = DRIVER_NONE;
  this_menu->original_value = this_menu->value;			// make a copy for change tracking
  
  if((this_menu->value) && (this_menu->menu_control == toggle_item))	// set up the toggle state
      {
      this_menu->current_state = CURRENT_STATE_SELECTED;
      }
  else
      {
      this_menu->current_state = CURRENT_STATE_STANDARD;		// all other set up as standard
      }

  this_menu->highlighted = false;			// none highlighted to start with
  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item

}


// TITLE:        reset_menu
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  Unhighlights a specific menu entry

void unhighlight_menu_entry(menu_table_type *this_menu)
{
	this_menu->highlighted = false;
	
	if((this_menu->value) && (this_menu->menu_control == toggle_item))	// set up the toggle state
	{
		this_menu->current_state = CURRENT_STATE_SELECTED;
	}
	else
	{
		this_menu->current_state = CURRENT_STATE_STANDARD;		// all other set up as standard
	}
	
	this_menu->internal_selection_driver_method = DRIVER_NONE;
}


// TITLE:        load_menu_graphics
// AUTHOR(s):    Rob Probin
// DATE STARTED: 10th May 2003
//
// DESCRIPTION:  Called by menu mode to load all graphics
//               

static void load_menu_graphics(menu_table_type *this_menu)
{
bool this_is_last_item;


do	// do all of the menu entries
  {
  
  this_is_last_item = this_menu->last_item;
  this_menu++;					// goto next item
  } while(this_is_last_item == false);		// abort if we reached last item
  
}


// TITLE:        load_single_menu_graphic
// AUTHOR(s):    Rob Probin
// DATE STARTED: 11th May 2003
//
// DESCRIPTION:  Called by load_menu_graphics to load one graphic.
// Note used but left in (Stu)

//static int load_single_menu_graphic(menu_table_type *this_menu, const char *raw_filename, bool load_with_alpha)
//{
//char filename[FILENAME_LENGTH];
//int new_gid;
//int attrib_string;
//
//if(raw_filename==NULL || (*raw_filename)=='\0')		// no graphic present
//  {
//  return EMPTY_GID;
//  }
//
//attrib_string = this_menu->graphics_load_attributes;
//if(attrib_string<first_FLIP || attrib_string>last_FLIP)		// invalid case
//  {
//  attrib_string=NO_FLIP;
//  fprintf(OUTPUT_FP,"Invalid mode supplied to load_menu_graphic\n");
//  }
//
//if(!make_filename(filename, raw_filename, MENU_GRAPHICS_PATH, "Menu Graphics"))	// make path first
//    {
//    new_gid = EMPTY_GID;
//    }
//else
//    {
//    new_gid = load_graphic( filename, 
//                /*shrink:*/false, /*transparency:*/load_with_alpha, 
//                /*flip:*/attrib_string, /*center_x:*/ 0, /*center_y:*/0);
//    }
//
//return new_gid;
//}




void fade_value_class::step(unsigned int time_in_ms_since_last)
{
	// we want it to fade about twice a second
	// therefore 500ms must be full-brightness to dim to full-brightness
	// ignoring the direction gives 250ms for half a cycle.
	const unsigned int fade_max = 250;	// milliseconds

	// first we add the time we are given onto the accumulation register
	fade_time += time_in_ms_since_last;
	
	// if we have gone past the maximum then we need to cope with that
	if(fade_time > fade_max)
	{
		fade_time -= fade_max;			// we have got to the max

		// if the time in is greater than 250ms then just blink as fast as we can because it's not going to blink twice a second
		if(fade_time >= fade_max) fade_time = 0;		// if we are running really slow the best we can do is blink

		if(direction) { fade_percentage=100; direction = false; }
		else { fade_percentage=0; direction = true; }
	}
	else	// we are partially the way to fade_max then we can set the percent fade,
	{		// i.e. we have a small enough time period to allow proper fading
		fade_percentage = (fade_time * 100) / fade_max;
		
		if(! direction)
		{
			fade_percentage = 100 - fade_percentage;
		}
		
	}

}


fade_timer::fade_timer() 
{ 
	last_time = LS_GetTicks(); 
}

void fade_timer::single_step() 
{ 
	unsigned int new_time = LS_GetTicks(); 
	step(new_time - last_time); 
	last_time = new_time; 
}


/* CVS LOG
 *
 * $Log: menu_driver.cpp,v $
 * Revision 1.8  2006/08/13 11:39:00  rob
 * Zex2005: Alter menu driver so that return can be procesed by client only.
 *
 * Revision 1.7  2006/05/18 07:08:16  rob
 * Zex2005: Menu for registration code entry.
 *
 * Revision 1.6  2006/04/02 11:49:06  rob
 * Zex2005: Finally fixed the keys disappearing on the save screen bug. Remember, don't take keys from the event queue from two places ... even by accident via Show_It & get_mouse_x_and_y.
 *
 * Revision 1.5  2006/03/19 12:34:16  rob
 * Zex2005: Highlight rather than naff arrow for menus.
 *
 * Revision 1.4  2005/12/14 21:41:46  rob
 * Zex2005: Removed unused screen reference.
 *
 * Revision 1.3  2005/12/10 10:51:34  rob
 * Zex2005: Fixed full screen cursor flicker (hopefully) by implementing our own cursor.
 *
 * Revision 1.2  2005/11/16 21:12:24  rob
 * Zex2005: Update that was causing all sorts of pushed buttons to appear due to the menu system stealing events away from the state based main event loop.
 *
 * Revision 1.1  2005/11/15 22:14:23  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.2  2005/02/05 21:26:06  rob
 * ZD: (i) Changed get_average_frame_time to double return and frame time in seconds rather than ms - most places (now all) convert to this anyway, plus it's more accurate for when we need to track time between two machines. (ii) added a new function to get a long timebase FPS (500ms), (iii) added more comments and explanations, (iv) added a function to return the accumulated gametime.
 *
 * Revision 1.1  2005/01/01 14:11:58  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.23  2004/05/09 22:19:51  robp
 * ZD: Changed stars to class based version.
 *
 * Revision 1.22  2004/05/01 21:55:44  robp
 * ZD: (1) Delegated menu key handlers (in this case the save as description entry text line) now return a flag saying whether they want the menu system to handle a specific key press. In this specific case we return "yes please handle" for the return and escape keys. This has the effect as changing the Accept and Cancel "pseudo-icons" and audibly clicking the button - a much better UI feel. The only case this doesn't happen is for the numeric enter key. It is possible to change this over as well (using a dummy menu element entry and a line in the text entry event handler - but it was seen as "not a major problem and not worth it". (2) Also deleted commented out code that was the start of a hand-rolled key repeat system (see previous check-in regarding SDL repeat).
 *
 * Revision 1.21  2004/04/28 22:38:56  robp
 * ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
 *
 * Revision 1.20  2004/04/18 16:13:43  robp
 * ZD: Menu #define lists become enumerated constant lists.
 *
 * Revision 1.19  2004/04/17 11:58:31  robp
 * ZD: Changed some #define debug macros to constants and real time evaluated if()'s since we are not limited on execution speed (none of these are inside tight inner loops). These will be easier to change to interpreter words if necessary.
 *
 * Revision 1.18  2004/04/17 11:08:33  robp
 * ZD: Moved DEBUG_ enable statements out of debug_options.h and into their native c files.
 *
 * Revision 1.17  2004/04/16 22:08:22  robp
 * ZD: Fixed return key locking up menu - and consolidated menu actioning.
 *
 * Revision 1.16  2004/04/16 20:40:24  robp
 * ZD: Altered menu's to use menu highlight offset rather than hardwired offset of 20 in menu driver. Also added correct offsets for file selector. Also disabled event debugging code from menu driver.
 *
 * Revision 1.15  2004/04/15 22:22:18  robp
 * ZD: Added unhighlight function, Added mouse area draw debug facility, added debug code to log menu events and changed menu draw order to not print menu if menu terminated after sub-menu quit.
 *
 * Revision 1.14  2004/04/07 19:55:41  robp
 * ZD: File selector Menu change updates
 *
 * Revision 1.13  2004/01/11 03:41:45  stu_c
 * menus: now use the font routines
 *
 * Revision 1.12  2003/12/14 17:18:41  stu_c
 * Initial Issue of sound.
 *
 * Revision 1.11  2003/10/13 22:12:11  robp
 * Fixed nasty assignment in if() bug, and alos unicode key is not valid on release in SDL bug.
 *
 * Revision 1.10  2003/09/20 18:02:18  robp
 * *** NAME CHANGE OF ALL C FILES FROM .c TO .cpp *** 
Before this point you will need to rename the files back to .c (i.e. remove the pp from .cpp) to build older versions.
 *
 * Revision 1.9  2003/09/18 20:26:38  robp
 * Changes for C++ compiler - this variable replaced with this_menu and various types "cleaned up" or cast to avoid problems with both compilers.
 *
 * Revision 1.8  2003/07/06 20:15:42  robp
 * Got rid of unsigned/signed int/char compare warnings for gcc3.3, and added cvs header and footers to all files that didn't have them.
 *
 * Revision 1.7  2003/07/06 10:16:33  robp
 * 1. Added user definable keys
2. Added debug panes
3. Added loadable text strings
4. Added support for more fonts
5. Added more debug for graphic loading
6. Fixed problem with star initialisation
7. Fix bug with key action in menu latching icon
8. Fixed bug in unload of player interface & fonts
 *
 * Revision 1.6  2003/06/14 09:53:06  robp
 * Assignment before finishing defining global variables causes problems on Windows build.
 *
 * Revision 1.5  2003/06/14 09:06:00  robp
 * Added stars to menu background. This is only temporary until the theme gets sorted out, but allows me to test the Zex ported code in key_setup_display.
 *
 * Revision 1.4  2003/06/07 21:34:24  rob_c
 * Added note about change log at bottom of file in header.
 *
 * Revision 1.3  2003/06/07 21:27:45  rob_c
 * Stopped the menu driver unloading all our graphics in the game when using the menu system for the game options screen, and added a cls to clean up the screen afterwards.
 *
 *
 */
