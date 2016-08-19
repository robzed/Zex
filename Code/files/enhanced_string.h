/*
 *  enhanced_string.h
 *  Zex
 *
 *  Created by Rob Probin on 28/03/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#ifndef ENHANCED_STRING_H
#define ENHANCED_STRING_H

/* 
Advantages of being inside a class / no being functions
 - namespace preserved (although we could use namespace directive to seperate these if they were seperate functions)
 - string.operation_name() rather than operation_name(string) syntax that looks more like operation on object
 - ability to replace with virtual function (would we ever need to do this??)
 - more clear which thing is being stripped (assuming modification of string)
 - internal functions don't have to pass around the string reference - because it's what we are that's being modified

 Disadvantages of being inside a class / being seperate functions
 - can't use this class and another drived from string
 - These are really just functions - they don't have any OO features
 - (legal) pointer passed rather than reference passed  (disadvantage?)
 - no reason to virtually replace functions (i.e. why a class)
 - implementation function have to deal with a pointers not a references to main string
 
 
I've seen the Date problem solved by having a base class Date with some operations on it and the data protected, 
with utility functions provided by deriving a new class and adding the utility functions. You get really messy 
systems like that, and there's no reason for having the utility functions in derived classes. You want the 
utility functions to the side so you can combine them freely. How else do I get your utility functions and my 
utility functions also? The utility functions you wrote are independent from the ones I wrote, and so they 
should be independent in the code. If I derive from class Date, and you derive from class Date, a third 
person won't be able to easily use both of our utility functions, because we have built dependencies in that 
didn't need to be there. So you can overdo this class hierarchy stuff.

*/

#include <string>

namespace enhanced_string {
	// all these function return true if they stripped anything
	
	// strip whitespace
	bool strip(std::string& strip_this);		// all whitespace off both edges
	bool rstrip(std::string& strip_this);		// all whitespace off the trailing edge
	bool lstrip(std::string& strip_this);		// all whitespace off the leading edge
	bool rstrip1(std::string& strip_this);		// one whitespace character of the trailing edge
	bool lstrip1(std::string& strip_this);		// one whitespace character of the leading edge
	
	// strip a specific charactger
	bool strip(std::string& strip_this, char c);
	bool rstrip(std::string& strip_this, char c);
	bool lstrip(std::string& strip_this, char c);
	bool rstrip1(std::string& strip_this, char c);
	bool lstrip1(std::string& strip_this, char c);
	
	// strip these set of characters off
	// bool strip(std::string& strip_this, std::string& chars_to_strip);
}

//class string2 : public std::string {
//
//public:
//	// let's add some more classes to this baby...
//	
//	// strip whitespace
//	bool strip();		// all whitespace off both edges
//	bool rstrip();		// all whitespace off the trailing edge
//	bool lstrip();		// all whitespace off the leading edge
//	bool rstrip1();		// one whitespace character of the trailing edge
//	bool lstrip1();		// one whitespace character of the leading edge
//
//	// strip a specific charactger
//	bool strip(char c);
//	bool rstrip(char c);
//	bool lstrip(char c);
//	bool rstrip1(char c);
//	bool lstrip1(char c);
//	
//	// constructors
//	string2(const char* name) : std::string(name)  { }
//	string2() : std::string() { }
//	string2(std::string& mystring) : std::string(mystring) { }
//	string2(const string2& mystring) : std::string(mystring) { }
//};


#endif // ENHANCED_STRING_H


