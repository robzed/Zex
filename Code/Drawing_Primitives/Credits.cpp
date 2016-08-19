// Name:   		    Credits.c
// Description:		This routine, reads in an string (Credits Value) 
//					and then spats at the location specified  
//
// Date Started:	02/02/99
// Author(s):		ID
//
// Copyright(c) Lightsoft Software   		

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <ToolUtils.h>
//oldmac//#include <resources.h>
//#include <stdio.h>	
#include "credits.h"
#include "Error_handler.h"
#include "zex_misc.h"


#define CHAR_OFFSET 96  // in Pixels to the next character  
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 12



void Draw_to_Double_Buffer_Score(char *char_ptr, int x_pos, int y_pos);

static Ptr double_buffer;
static int screen_width;
Handle cred_ptr;


void InitCredits()
 {
 cred_ptr = (Handle) LS_GetResource('ZRAW',136);  //Get the Handle to the Resource 
 if (cred_ptr==0) report_error_c("Resource missing: ZRAW 136","",4);
 LS_HLock(cred_ptr);
 }


//-----------------------------------------------------------------
//-----------------------------------------------------------------


//void Draw_to_Double_Buffer_Score(char *char_ptr, int x_pos, int y_pos)
//  {
//extern Ptr screenaddr;
//extern int screenwidth;
//char *db_ptr;
//int a,b;
//
//
//double_buffer=screenaddr;
//screen_width=screenwidth;
//
//db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));
//
//  for(b=0; b < CHAR_HEIGHT; b++)
//  {
//    for(a=0; a < CHAR_WIDTH; a++)
//     {
//     *db_ptr = *char_ptr;
//     char_ptr++;
//     db_ptr++;
//     }
//  char_ptr-=CHAR_WIDTH;
//  db_ptr-=CHAR_WIDTH;
//  char_ptr+=CHAR_WIDTH;
//  db_ptr+=screen_width;
//  }
//
//  }

void SplatCredits(char splat_str[30], int x_pos, int y_pos)
 {
  int i,my_char,counter;
  char *char_ptr;
//  char temp_str[20];
  char new_str[20];
  
  int char_pos;
//  Rect my_rect;

  
  char_pos=x_pos;
  i=0;
  counter = 0;
  
  my_char = splat_str[i];
  

  while(my_char)
   {
    if ((my_char >= '0') && (my_char <='9'))
    {
     counter++;
    }
     i++;
     my_char = splat_str[i];
   }

  
 counter  = 9 - counter;
   
  for (i= 0; i < counter; i++)
    {
     new_str[i] = '0';
    }

  
  for (i= counter; i < 9; i++)
    {
     new_str[i] = splat_str[i- (counter-1)];
     }
     
  new_str[i] = '\0';
  
 
  i = 0;
  my_char = new_str[i];
  

  while(my_char)
   {
    
    if ((my_char >= '0') && (my_char <='9'))
    {
     char_ptr =   (CHAR_OFFSET * (my_char - 48)) + *cred_ptr;
     Draw_to_Double_Buffer_Score(char_ptr,char_pos,y_pos);	
    }
   
   

   char_pos+= CHAR_WIDTH;
   i++;
   my_char=new_str[i];  
   } 

	
 }


