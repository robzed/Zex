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

/*
 *  game_script.cpp
 *  Zex
 *
 *  Created by Stuart Ball on 11/04/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <mactypes.h>
//oldmac//#include <files.h>
//oldmac//#include <events.h>
//oldmac//#include <timer.h>

//#include <memory.h>
#include "game_defs.h"
#include "file_io.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"
#include "zex_misc.h"
#include "3d_structures_def.h"
#include "ship_control.h"
#include "input_control.h"
#include "game_pause.h"
#include "object_control_top.h"
#include "camera.h"
#include "dlp.h"
#include "CPanel.h"
#include "handle_level.h"
#include "Error_handler.h"
#include "main_sel_screen.h"
#include "Galaxy.h"
#include "game_script.h"

int surrendered_flag=0;
int gone_around_flag=0;
//Scripting
unsigned long script_timer1=0;	//gp timer (frames)
unsigned long script_timer2=0;	//
unsigned long script_state1=0;	//
unsigned long script_state2=0;	//


void update_script(void)
{
int quad;
extern int current_quad;
	if (script_timer1>0)
	{
		script_timer1--;
		if (script_timer1==0)
		{
			//do something
			if (script_state1==BETA_MESSAGE)
			{
				surrendered_flag=0;	//clear this so they may surrender again
				//flood quadrant 3 or 4 with aliens
				if (RangedRdm(0,100)>50) quad=3;
				else
				quad=4;
				if (current_quad==3 && quad==3)
				{
					quad=4;
				}
				else
				if (current_quad==4 && quad==4)
				{
					quad=3;
				}
				if (quad==3)
				{
					add_to_text_display("OBC> Incoming message from Gamma Quadrant follows...",215);
					add_to_text_display("H3W1: Gamma Quad - Huge R'Mparian fleet incoming!",215);
					add_to_text_display("H3W1: They're everywhere!!",215);
					//flood gamma quad
					set_all_motherships_to_alegiance(ALIEN, 2);
				}
				else
				if (quad==4)
				{
					add_to_text_display("OBC> Incoming message from Delta Quadrant follows...",215);
					add_to_text_display("H7Z2: Delta Quad - Major R'Mparian activity!",215);
					add_to_text_display("H7Z2: We can't hold out much longer...",215);
					//flood delta quad
					set_all_motherships_to_alegiance(ALIEN, 3);
				}

			}
		}
	}

	if (script_timer2>0)
	{
		script_timer2--;
		if (script_timer2==0)
		{
			//do something
		}
	}

}

void render_frame(int width, int height);
void handle_surrender(void)
{
int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
Handle button_data_H;
Handle button_down_data_H;
LSRAW *button_up;
LSRAW *button_down;
Handle picture_H;
LSRAW* the_picture;
extern _3D light_normal;
extern int freeze;
extern int Mode320, zex_alive,user_abort,death_frame_counter;
_3D light_normal_save;
int return_val, real_return_val, old_button_state;

    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up
	LS_HLock(button_data_H);
	button_up = (LSRAW*)*button_data_H;

    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

	picture_H=GetZexPicture ('ZRAW',150);	//surrender pict
	if (picture_H==0) report_error_c("Resource missing: ZRAW 150","",4);
	LS_HLock(picture_H);

    prefs_x=120; prefs_y=80;
//	the_drawing_buffer=Get_back_buffer();
    LS_ShowCursor();
    light_normal_save=light_normal;
	light_normal.z=1.3;	//dim the scene
	freeze=1;	//in 3d_top

    ZexGetMouse (&mouse_xy);


//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
	while(button_state==1)
	{
		button_state=game_fire_button();
	}
	old_button_state=button_state;
#define b1_y 153
#define b2_y 193
#define b_x 140
	return_val=real_return_val=0;
	while(real_return_val==0)
	{
		return_val=0;
		//Get_back_buffer();
		if (Mode320==1)
		render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
		else
		render_frame(640,390);	//in this file - calls all the needed 3d stuff

		run_dlp();
	//    run_dlp_text();
		do_control_panel_control();

		ZexGetMouse (&mouse_xy);
		the_picture = (LSRAW*)*picture_H;
		//erase_zex_rect(640,480);	//in pictures

		print_pict_scaled(the_picture,prefs_x,prefs_y,0,1);	//Print with transparency

		button_state=game_fire_button();
//first draw all buttons as off
		print_crunched_pict(button_up,prefs_x+b_x,prefs_y+b1_y);
		print_crunched_pict(button_up,prefs_x+b_x,prefs_y+b2_y);
//now draw the down one
		 if (mouse_xy.v>b1_y+prefs_y && mouse_xy.v<b1_y+18+prefs_y )
		 {
			print_crunched_trans_pict(button_down,prefs_x+b_x,prefs_y+b1_y);
			return_val=1;
		 }
		 if (mouse_xy.v>b2_y+prefs_y && mouse_xy.v<b2_y+18+prefs_y )
		 {
			print_crunched_trans_pict(button_down,prefs_x+b_x,prefs_y+b2_y);
			return_val=2;
		 }

		Show_it();
		if (button_state==1 && old_button_state==0)	//if up and was down
		real_return_val = return_val;
		else
		real_return_val=0;
		old_button_state=button_state;

	}
    LS_DisposeHandle(picture_H);
    LS_DisposeHandle(button_data_H);
    LS_DisposeHandle(button_down_data_H);
	LS_HideCursor();
    light_normal=light_normal_save;	//restore the lighting
	freeze=0;
	if (real_return_val==1)
	{
		//end game
        zex_alive=0;
        user_abort=0;
        death_frame_counter=1;

	}
}

