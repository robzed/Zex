/*
 *  get_text_string.cpp
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "get_text_string.h"


// these strings need putting into a resource for localisation
//
//
// keep table in same order as enums in header...
//
const char *text_string_bank[] = {
	"",
	"",	// used to be -->
	"", // used to be ->
	
	//
	// These are the strings for the file selector dialog menu
	//
	"%YSave File Selector",
	"%YLoad File Selector",
	
	"Slot",
	"No game found",
	"Player 1 - Character Level ",
	"Player 2 - Character Level ",
	"Room Name: ",
	"Single Player",
	"(No description)",
	"Time and Date",
	"PowerPC",
	"x86",
	"Zex Saved Game",
	
	"%M%GP%Wrevious",
	"%M%OP%Rrevious",
	"%M%GN%Wext",
	"%M%ON%Rext",
	"%L%GC%Wancel",
	"%L%OC%Rancel",
	
	//
	// These are the "save as" menu strings
	//
	"%L%WGive this file a description...",
	"%L%WCancel %G[Esc]",
	"%L%RCancel %O[Esc]",
	"%L%WAccept %G[Return]",
	"%L%RAccept %O[Return]",
	"%M%WClear",
	"%M%RClear",
	"%M%WRecall Previous",
	"%M%RRecall Previous",

	"%L%WEnter Registration Details...",
	"%L%WName",
	"%L%WCode"

};

// TITLE:		 get_text
// AUTHOR(s):    Rob Probin
// DATE STARTED: 16th October 2005
//
// DESCRIPTION:  Emulate Z_dungeon get_text for Zex
//               
const char *get_text(int text_ref)
{
	return text_string_bank[text_ref];
}

