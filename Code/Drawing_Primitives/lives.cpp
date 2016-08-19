// lives.cpp

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <ToolUtils.h>
//oldmac//#include <resources.h>
//#include <stdio.h>	
#include "lives.h"
#include "Error_handler.h"
#include "zex_misc.h"

#define CHAR_OFFSET 45  // in Pixels to the next character  
#define CHAR_COLUMN_SIZE 5
#define CHAR_LINES 9

void Draw_to_Double_Buffer_lives(char *char_ptr, int x_pos, int y_pos);

static Ptr double_buffer;
static int screen_width;
Handle liv_ptr;


void InitLives()
{
 liv_ptr = (Handle) LS_GetResource('ZRAW',140);  //Get the Handle to the Resource 
 if (liv_ptr==0) report_error_c("Resource missing: ZRAW 140","",4);
 LS_HLock(liv_ptr);
 
}



void Draw_to_Double_Buffer_lives(char *char_ptr, int x_pos, int y_pos)
{
 extern Ptr screenaddr;
 extern int screenwidth;
 char *db_ptr;
 char pixel;	//sb 23/12/98
 int a,b;


 double_buffer=screenaddr;
 screen_width=screenwidth;

 db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)
  {
    for(a=0; a < CHAR_COLUMN_SIZE; a++)
     {
     pixel=(*char_ptr);
     if (pixel==0) pixel=0xff;	//if 0 then 255
     *db_ptr = pixel;
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=CHAR_COLUMN_SIZE;
  db_ptr-=CHAR_COLUMN_SIZE;
  char_ptr+=CHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }


void SplatLives(char splat_str[4], int x_pos, int y_pos)
 {
  int i,my_char;
  char *char_ptr;
  
  int char_pos;
  Rect my_rect;
  
  
  char_pos=x_pos;
  i=0;
  
  my_char=splat_str[i];
  
  Draw_to_Double_Buffer_lives(*liv_ptr,(x_pos+5),y_pos);
  Draw_to_Double_Buffer_lives(*liv_ptr,(x_pos+10),y_pos);
 
  while(my_char)
   {
    
   if ((my_char >= '0') && (my_char <='9'))
    {
     char_ptr =   (CHAR_OFFSET * (my_char - 47)) + *liv_ptr;	
     Draw_to_Double_Buffer_lives(char_ptr,char_pos,y_pos);
    }

   

   char_pos+= CHAR_COLUMN_SIZE;
   i++;
   my_char=splat_str[i];  
   } 

	
 }


