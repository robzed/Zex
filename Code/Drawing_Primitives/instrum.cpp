// Name:   		    Instrum.c
// Description:		These routines, cope with instruments  
//			
// Date Started:	4/1/99
// Author(s):		ID
//
// Copyright(c) Lightsoft Software   		

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <resources.h>
//oldmac//#include <ToolUtils.h>
#include "instrum.h"
#include "Error_handler.h"
#include "instrum_int.h"
#include "zex_misc.h"

#define NONE 0

#define CHAR_OFFSET 180  // in Pixels to the next character  
#define ROW_SIZE 36
#define CHAR_LINES 5





void _Draw_to_Double_Buffer_Instrum(char *char_ptr, int x_pos, int y_pos);

//main ship items
kit_desc ship_items[ITEMS_COUNT];
//hold items
hold_desc hold_items[MAX_HOLD_ITEMS];




Ptr double_buffer;
int screen_width;
Handle inst_pic;
int counter;
  

void InitInstruments()
 {
 inst_pic = (Handle) LS_GetResource('ZRAW',137);  //Get the Handle to the Resource 
 if (inst_pic==0) report_error_c("Resource missing: ZRAW 137","",4);
 LS_HLock(inst_pic);
 counter = 3;
 

 
 }


 
 void update_instruments()
 {
  
//  int empty_blocks;
//  int remainder;
  int _start_point;
  int i; 
//  int instrument;
  
  _start_point = 115;
  
       for (i = 0; i < 6;i++)
       
     {
          SplatInstrum(ship_items[i].status,_start_point+(i*40),406);
        
     }
  
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatInstrum(int instrument, int x_pos, int y_pos)
 {
  char *char_ptr;

	if (instrument<0) instrument=2;	//if lost then draw as U/S
	if (instrument>2) instrument=2;	//lets not draw garbage
	char_ptr =  (CHAR_OFFSET * (instrument)) + *inst_pic;
	_Draw_to_Double_Buffer_Instrum(char_ptr,x_pos,y_pos);
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void _Draw_to_Double_Buffer_Instrum(char *char_ptr, int x_pos, int y_pos)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;

double_buffer=screenaddr;
screen_width=screenwidth;



 db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)
  {
    for(a=0; a < ROW_SIZE; a++)
     {
      *db_ptr =  *char_ptr;
      char_ptr++;
      db_ptr++;
     }
  db_ptr-=ROW_SIZE;
  db_ptr+=screen_width; 
 }
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


