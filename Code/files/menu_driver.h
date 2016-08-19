/*
 *  menu_driver.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Sun May 04 2003.
 *  Copyright (c) 2003 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/05/18 07:08:54 $
 * $Revision: 1.5 $
 *
 * For log see end of file...
 *
 */

#ifndef _MENU_DRIVER_H_
#define _MENU_DRIVER_H_

// ***********************************************************************************
// * TYPE DEFS AND ENUMERATORS
// *

enum menu_control_type
    {
    non_display_item,				// used for saving non-visible data
    display_item_only,				// things that don't change upon click or mousing
    toggle_item,					// things that toggle like a light switch
    event_item,						// push button type actions
    };

//
// Semi-Private types
//

enum menu_current_state_type {
	CURRENT_STATE_STANDARD = 0,
	CURRENT_STATE_CLICKED = 1,
	CURRENT_STATE_SELECTED = 2
};

enum menu_driver_type {
	DRIVER_NONE = 0,
	DRIVER_DIRECT_KEY = 1,
	DRIVER_ARROW_KEY = 2,
	DRIVER_MOUSE = 3
};



// Typical usage: pass in an array of these to the run_menu. Call save_menu_values after returns.
//
// All fields should be setup before calling any menu function.
//
typedef struct menu_table_element_type_tag
    {
    // basic configuration data
    bool last_item;					// true = last in array
    int user_defined_data;				// not changed by menu driver
    menu_control_type menu_control;			// see above - this is the controlling type.

    int print_location_x, print_location_y;		// where the graphic gets printed
    int highlight_offset_x;           //where the highlight is printed relative to start of menu string
    bool draw_graphics;					// should the menu routine draw the graphics (external graphics display will happen either way)
    bool load_graphics_with_alpha;			// should the graphics be loaded with alpha?
    bool load_highlight_graphics_with_alpha;		// should the highlight graphic with alpha?
    int direct_key_value;				// this is an SDL_key, or -1 for no key
    bool unicode_key;					// if this is true, the above key is a unicode key rather than an SDL key
    
    bool value;						// what value this object has - only useful for toggle and is the new state.
    int graphics_load_attributes;			// for example NO_FLIP - see Surface_Controller.h
	// graphics load attributes is not used anymore

	SDL_Rect mouseable_area;				// this is the area that is moused over and can be clicked (if applicable)
                                                        // if the width or height is 0, the area cannot be clicked or highlighted
  
    // graphics used
    int standard_string_name;	// normally printed (can be empty string for nothing)
    int highlight_string_name;	// when moused over this is printed (can be empty string for nothing)
    int clicked_string_name;	// when click this is printed - e.g. super bright version (can be empty string for nothing)
    int selected_string_name;	// when toggled on this is printed (can be empty string for nothing)
    
    // callback calls - this pointer will point to this specific entry, not to start of list
    void (*special_display_function)(struct menu_table_element_type_tag *this_menu);	// if we need to print something extra than the graphic above (e.g. a number). Can be NULL.    
    void (*click_callback_function)(struct menu_table_element_type_tag *this_menu);	// this is called when clicked. Can be NULL.
    void (*save_callback_function)(struct menu_table_element_type_tag *this_menu, int user_val);	// change the value into a interpreter string for saving. Can be NULL.
    void (*run_loop_callback_function)(struct menu_table_element_type_tag *this_menu); // called everytime around the run loop. Can be NULL.
    void (*init_callback_function)(struct menu_table_element_type_tag *this_menu); // called at menu start for value initialisation. Can be NULL - especially if the menu structure is set up with values before calling run_menu.

    // sound stuff - no currently used
//    char click_sound_filename[FILENAME_LENGTH];		// sound when click (can be empty string for nothing)
//    char hover_sound_filename[FILENAME_LENGTH];		// sound when hovered over (can be empty string for nothing)
    
    // these are set up by the initialisation routines of the menu
    bool highlighted;					// false=no, true=yes  (init doesn't matter)
    bool original_value;				// copy of the original menu item to check for changes
    int /*menu_current_state_type*/ current_state;					// 0=standard, 1=clicked, 2=selected  (init doesn't matter)
    menu_driver_type internal_selection_driver_method;		// 0=none, 1=direct key, 2=arrow keys, 3=mouse  (init doesn't matter)

// not currently used
//    int standard_graphic_gid;				// gid after graphic is loaded
//    int highlight_graphic_gid;				// gid after graphic is loaded
//    int clicked_graphic_gid;				// gid after graphic is loaded
//    int selected_graphic_gid;				// gid after graphic is loaded
    
    } menu_table_type;




//
// public functions
//

void run_menu(menu_table_type *this_menu, bool cls_on_every_frame);	// pass in start of menu list as this_menu
void passback_menu_values(menu_table_type *this_menu, int user_val);
void terminate_menu_mode(void);
bool have_any_menu_values_changes(menu_table_type *this_menu);
void run_menu_advanced(menu_table_type *this_menu, bool cls_on_every_frame, bool (*key_event_function)(SDL_Event event));	// pass in start of menu list as this_menu. Return type of embedded function is whether menu can process the key event (ignored if not a key event)

// active menu modifiers
void unhighlight_menu_entry(menu_table_type *this_menu);


//
// public classes
//

class menu_callback_interface_protocol {
public:
	virtual void special_display_function(struct menu_table_element_type_tag &this_menu) = 0;
	virtual void click_callback_function(struct menu_table_element_type_tag &this_menu) = 0;
	virtual void save_callback_function(struct menu_table_element_type_tag &this_menu) = 0;
	virtual void run_loop_callback_function(struct menu_table_element_type_tag &this_menu) = 0;
	virtual void init_callback_function(struct menu_table_element_type_tag &this_menu) = 0;
	virtual ~menu_callback_interface_protocol();
};

//
// Wierd fade value class used inside menu for highlighting
// Should really by via inherited classes, but still...
//
class fade_value_class {
public:
	fade_value_class() { reset_fade(); }
	void reset_fade() { direction = true; fade_percentage = 0; fade_time=0; }
	void step(unsigned int time_in_ms_since_last);
	unsigned char get_fade_value() { return fade_percentage; }
private:
	bool direction;
	unsigned char fade_percentage;
	unsigned int fade_time;
};

class fade_timer : public fade_value_class {
public:
	fade_timer();
	void single_step();
private:
	unsigned int last_time;
};

//
// Menu class
//
//class menu {
//	
//public:
//	menu(menu_table_type *menu_to_link_with);
//	cls_on_every_frame(bool enable_flag);
//	run(void) { run_advanced(NULL); }
//	run_advanced(bool (*key_event_function)(SDL_Event event));		// temporary hook to avoid changing two sets of interface
//private:
//	menu_table_type *this_menu;
//	bool cls_flag;		// normally true
//	
//	fade_value our_fade;	// added for highlighing  ... should really be inherited class version
//};
//


#endif // _MENU_DRIVER_H_


/* CVS LOG
 *
 * $Log: menu_driver.h,v $
 * Revision 1.5  2006/05/18 07:08:54  rob
 * Zex2005: Menu for registration code entry.
 *
 * Revision 1.4  2006/03/19 12:34:20  rob
 * Zex2005: Highlight rather than naff arrow for menus.
 *
 * Revision 1.3  2005/12/16 22:13:06  rob
 * Zex2005: Added virtual destructor - Minor warning removal after -Wall -W
 *
 * Revision 1.2  2005/12/14 21:41:53  rob
 * Zex2005: Removed unused screen reference.
 *
 * Revision 1.1  2005/11/15 22:14:21  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:12:00  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.11  2004/05/01 21:55:44  robp
 * ZD: (1) Delegated menu key handlers (in this case the save as description entry text line) now return a flag saying whether they want the menu system to handle a specific key press. In this specific case we return "yes please handle" for the return and escape keys. This has the effect as changing the Accept and Cancel "pseudo-icons" and audibly clicking the button - a much better UI feel. The only case this doesn't happen is for the numeric enter key. It is possible to change this over as well (using a dummy menu element entry and a line in the text entry event handler - but it was seen as "not a major problem and not worth it". (2) Also deleted commented out code that was the start of a hand-rolled key repeat system (see previous check-in regarding SDL repeat).
 *
 * Revision 1.10  2004/04/28 22:38:57  robp
 * ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
 *
 * Revision 1.9  2004/04/18 16:13:42  robp
 * ZD: Menu #define lists become enumerated constant lists.
 *
 * Revision 1.8  2004/04/15 22:22:18  robp
 * ZD: Added unhighlight function, Added mouse area draw debug facility, added debug code to log menu events and changed menu draw order to not print menu if menu terminated after sub-menu quit.
 *
 * Revision 1.7  2004/04/07 19:55:42  robp
 * ZD: File selector Menu change updates
 *
 * Revision 1.6  2004/02/01 12:01:24  robp
 * Commented out unused entried in menu data
 *
 * Revision 1.5  2004/01/11 03:41:53  stu_c
 * menus: now use the font routines
 *
 * Revision 1.4  2003/09/18 20:26:43  robp
 * Changes for C++ compiler - this variable replaced with this_menu and various types "cleaned up" or cast to avoid problems with both compilers.
 *
 * Revision 1.3  2003/06/07 21:53:19  rob_c
 * Added cvs keywords to some files.
 *
 *
 *
 */
