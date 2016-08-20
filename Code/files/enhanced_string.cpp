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
 *  enhanced_string.cpp
 *  Zex
 *
 *  Created by Rob Probin on 28/03/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#include "enhanced_string.h"
#include <cctype>

using namespace std;

//
// Bunch of string stripping routines
//
bool enhanced_string::strip(std::string& strip_this)
{
	bool any = false;
	if(rstrip(strip_this)) any=true;
	if(lstrip(strip_this)) any=true;
	
	return any;
}

bool enhanced_string::strip(std::string& strip_this, char c)
{
	bool any = false;
	if(rstrip(strip_this, c)) any=true;
	if(lstrip(strip_this, c)) any=true;
	
	return any;
}


bool enhanced_string::rstrip(std::string& strip_this)
{
	bool any = false;
	if(rstrip1(strip_this)) any=true;
	
	while(rstrip1(strip_this))
	{ /* do nothing else */ }
	
	return any;
}


bool enhanced_string::lstrip(std::string& strip_this)
{
	bool any = false;
	if(lstrip1(strip_this)) any=true;
	   
	while(lstrip1(strip_this))
	{ /* do nothing else */ }
	   
	return any;	   
}


bool enhanced_string::rstrip(std::string& strip_this, char c)
{
	bool any = false;
	if(rstrip1(strip_this, c)) any=true;
	
	while(rstrip1(strip_this, c))
	{ /* do nothing else */ }

	return any;
}


bool enhanced_string::lstrip(std::string& strip_this, char c)
{
	bool any = false;
	if(lstrip1(strip_this, c)) any=true;
	   
	while(lstrip1(strip_this, c))
	{ /* do nothing else */ }
	   
	return any;	   
}


bool enhanced_string::rstrip1(std::string& strip_this)
{
	int len = strip_this.length();
	if(len > 0)	// must have a character there to delete it!
	{
		if(isspace(strip_this[len-1]))
		{
			// we have a character to strip
			strip_this.erase(len-1, 1);		// remove one character from end
			return true;
		}
	}
	return false;
}

bool enhanced_string::lstrip1(std::string& strip_this)
{
	int len = strip_this.length();
	if(len > 0)	// must have a character there to delete it!
	{
		if(isspace(strip_this[0]))
		{
			// we have a character to strip
			strip_this.erase(0, 1);		// remove one character from start
			return true;
		}
	}
	return false;
}


bool enhanced_string::rstrip1(std::string& strip_this, char c)
{
	int len = strip_this.length();
	if(len > 0)	// must have a character there to delete it!
	{
		if(strip_this[len-1] == c)
		{
			// we have a character to strip
			strip_this.erase(len-1, 1);		// remove one character from end
			return true;
		}
	}
	return false;
}

bool enhanced_string::lstrip1(std::string& strip_this, char c)
{
	int len = strip_this.length();
	if(len > 0)	// must have a character there to delete it!
	{
		if(strip_this[0] == c)
		{
			// we have a character to strip
			strip_this.erase(0, 1);		// remove one character from start
			return true;
		}
	}
	return false;
}

