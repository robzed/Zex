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

// *************************************MODULE HEADER*********************************
// * FILENAME           : Planet_Generator.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 19:37:44 on 29th August 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


#include "3d_structures_def.h"
#include "Galaxy.h"
#include	"zex_misc.h"

#include "Planet_Generator.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
unsigned char get_consonant(int seed);
unsigned char get_vowel(int seed);
int is_vowel(unsigned char test);
void gen_char(int char_pos, int seed, char * where);
unsigned char random_vowel();
int is_cons(unsigned char test);
unsigned char get_capital_consonant(int seed);

//we get a seed and must return a pascal string
//derived from the see

int seed_add=3;

void init_gen_planet_name(void)
{
	seed_add=3;
}


void gen_planet_name(unsigned int seed, unsigned char * output_string)
{
int char_count,consonant;
unsigned int temp;
unsigned char temp_char;
int number_of_chars;
char gened_char [3];	//pascal string of new char
int i;

if (seed_add>17) seed_add=3;	//reset odd add

//clac number of chars
number_of_chars=seed%10;

temp=RangedRdm(0,100);


if (number_of_chars<4) 
{
 if (temp>60)
 number_of_chars=4;
 else if (temp>30)
 number_of_chars=5;
 else
 number_of_chars=3;
}

char_count=0;

//for (i=0;i<number_of_chars;i++)
// {
//  gen_char(i,seed/(i+1), gened_char);
//  output_string[i+1]=gened_char[1];
//  char_count++;
// }
// 
//first part can be any char
temp=seed%25+'A';

if (temp=='X')	//dont want X
  temp=(seed/2)%25+'A';
  
if (temp=='X') temp=get_capital_consonant(seed);	//last ditch effort

output_string[1]=temp;;
char_count=1;
//if it was a vowel, the second must be a consonant

//2nd char
if (temp=='Q')
{
output_string[2]='u';
temp_char = 'u';
}
else
{
if (is_vowel(temp)==1) consonant=1;
else
consonant=0;

//second char depends on consonant, and must be an even letter...
if (consonant==1) temp_char=get_consonant(seed/3);
else temp_char=get_vowel(seed/3);

output_string[2]=temp_char;
}
char_count++;
if (number_of_chars==2) goto done;

//3rd char
if (temp_char=='q')
output_string[3]='u';
else
{
temp=(seed/3)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[3]=temp;;
}
char_count++;
if (number_of_chars==3) goto done;

//4th char
if (temp=='q')
output_string[4]='u';
else
{
temp=(seed/6)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;


output_string[4]=temp;;
}
char_count++;
if (number_of_chars==4) goto done;

//5th char
if (temp=='q')
output_string[5]='u';
else
{
temp=(seed/5)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[5]=temp;;
}
char_count++;
if (number_of_chars==5) goto done;

//char 6
if (temp=='q')
{
output_string[6]='u';
temp_char = 'u';
}
else
{
if (is_vowel(temp)==1) consonant=1;
else
consonant=0;

//second char depends on consonant, and must be an even letter...
if (consonant==1) temp_char=get_consonant(seed);
else temp_char=get_vowel(seed/4);
}
//
// BIG NOTE: some might say that this should be temp_char ... however we like it as...  (Rob&Stu 20-Dec-2005)
//
output_string[6]=temp;;

char_count++;
if (number_of_chars==6) goto done;


//7th char
if (temp=='q')
output_string[7]='u';
else
{
temp=(seed/5)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[7]=temp;;
}
char_count++;
if (number_of_chars==7) goto done;

//8th char
if (temp=='q')
output_string[8]='u';
else
{
temp=(seed/9)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[8]=temp;;
}
char_count++;
if (number_of_chars==8) goto done;

done: 
output_string[0]=char_count;


//now check the string for chars that can't be next to each other
//e.g. m followed by g etc

for (i=1;i<char_count;i++)
 {
 char next_char;
 char this_char;
  
 next_char=output_string[i+1];
 this_char=output_string[i];
 if (this_char>='A' && this_char<='Z') this_char+=32;	//to lower case 
//this switch determines the rules of the word composition and is tweaked
//so bad language can't come out.
//faves seen to date include:
//Qunt and God. 
  	  	  switch(this_char) {
			case 'a':
			  if(next_char=='a') output_string[i+1]='b';
			  if(next_char=='o') output_string[i+1]='p';
				break;
				
            case 'b':
			  if(next_char=='c') output_string[i+1]='d';
			  if(next_char=='f') output_string[i+1]='b';
			  if(next_char=='g') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='a';
			  if(next_char=='n') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='q') output_string[i+1]='i';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='i';
			  if(next_char=='x') output_string[i+1]='b';
			  if(next_char=='y') output_string[i+1]='a';
			  if(next_char=='z') output_string[i+1]='e';
			  if(next_char=='d') output_string[i+1]=random_vowel();;
                break;
				
            case 'c':
			  if(next_char=='c') output_string[i+1]='i';	//dont allow double c's
			  if(next_char=='d') output_string[i+1]='c';
			  if(next_char=='f') output_string[i+1]='a';
			  if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='m') output_string[i+1]='k';
			  if(next_char=='n') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='q') output_string[i+1]='l';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='l';
			  if(next_char=='x') output_string[i+1]='e';
			  if(next_char=='y') output_string[i+1]='a';
			  if(next_char=='z') output_string[i+1]='a';
			  if(next_char=='t') output_string[i+1]='o';

                break;
				
            case 'd':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='e';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='g') output_string[i+1]='o';
			  if(next_char=='h') output_string[i+1]='u';
			  if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='k') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='d';
			  if(next_char=='n') output_string[i+1]='d';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='q') output_string[i+1]='a';

                break;
				
            case 'e':
                break;
				
            case 'f':
			  if(next_char=='b') output_string[i+1]='f';
			  if(next_char=='c') output_string[i+1]='a';
			  if(next_char=='d') output_string[i+1]='e';
			  if(next_char=='g') output_string[i+1]='o';
			  if(next_char=='h') output_string[i+1]='a';
			  if(next_char=='j') output_string[i+1]='e';
			  if(next_char=='k') output_string[i+1]='i';
			  if(next_char=='m') output_string[i+1]='l';
			  if(next_char=='n') output_string[i+1]='f';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='s') output_string[i+1]='i';
			  if(next_char=='t') output_string[i+1]='o';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='e';
			  if(next_char=='x') output_string[i+1]='f';
			  if(next_char=='z') output_string[i+1]='f';
			  if(next_char=='u') output_string[i+1]='i';

                break;
				
            case 'g':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='g';
			  if(next_char=='d') output_string[i+1]='e';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='j') output_string[i+1]='o';
			  if(next_char=='k') output_string[i+1]='h';
			  if(next_char=='l') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='a';
			  if(next_char=='n') output_string[i+1]='u';
			  if(next_char=='p') output_string[i+1]='g';
			  if(next_char=='t') output_string[i+1]='h';
			  if(next_char=='v') output_string[i+1]='i';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='o';

                break;
				
            case 'h':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='o';
			  if(next_char=='d') output_string[i+1]='e';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='g') output_string[i+1]='u';
			  if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='l') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='o';
			  if(next_char=='n') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='a';
			  if(next_char=='q') output_string[i+1]='i';
			  if(next_char=='r') output_string[i+1]='a';
			  if(next_char=='s') output_string[i+1]='u';
			  if(next_char=='t') output_string[i+1]='o';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='y') output_string[i+1]='o';
			  if(next_char=='z') output_string[i+1]='u';

                break;
				
            case 'i':
			  if(next_char=='i') output_string[i+1]='n';

                break;
				
            case 'j':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]=random_vowel();
			  if(next_char=='d') output_string[i+1]='u';
			  if(next_char=='f') output_string[i+1]='s';
			  if(next_char=='g') output_string[i+1]='u';
			  if(next_char=='k') output_string[i+1]='i';
			  if(next_char=='l') output_string[i+1]='o';
			  if(next_char=='p') output_string[i+1]='s';
			  if(next_char=='q') output_string[i+1]='e';
			  if(next_char=='r') output_string[i+1]='o';
			  if(next_char=='s') output_string[i+1]=random_vowel();
			  if(next_char=='t') output_string[i+1]='i';
			  if(next_char=='v') output_string[i+1]='s';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='u';

                break;
				
            case 'k':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='s';
			  if(next_char=='d') output_string[i+1]='a';
			  if(next_char=='g') output_string[i+1]='e';
			  if(next_char=='j') output_string[i+1]='o';
              if(next_char=='n') output_string[i+1]='o';
              if(next_char=='m') output_string[i+1]='o';
              if(next_char=='r') output_string[i+1]='a';
              if(next_char=='f') output_string[i+1]='o';
                break;
				
            case 'l':
			  if(next_char=='b') output_string[i+1]='l';
              if(next_char=='j') output_string[i+1]='a';
              if(next_char=='c') output_string[i+1]='u';
              if(next_char=='d') output_string[i+1]='a';
              if(next_char=='n') output_string[i+1]='o';
              if(next_char=='m') output_string[i+1]='o';
              if(next_char=='r') output_string[i+1]='a';
              if(next_char=='f') output_string[i+1]='o';
              if(next_char=='h') output_string[i+1]='u';
              if(next_char=='v') output_string[i+1]='t';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
                break;
				
            case 'm':
			  if(next_char=='b') output_string[i+1]='a';
              if(next_char=='j') output_string[i+1]='e';
			  if(next_char=='g') output_string[i+1]='o';
			  if(next_char=='c') output_string[i+1]='i';
                break;
				
            case 'n':
              if(next_char=='b') output_string[i+1]='o';
              if(next_char=='j') output_string[i+1]='n';
			  if(next_char=='v') output_string[i+1]='s';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='u';

                break;
				
            case 'o':
              if(next_char=='e') output_string[i+1]='n';

                break;
				
            case 'p':
              if(next_char=='b') output_string[i+1]='a';
              if(next_char=='j') output_string[i+1]='p';
			  if(next_char=='v') output_string[i+1]='p';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='u';
                break;
				
            case 'q':
                break;
				
            case 'r':
              if(next_char=='b') output_string[i+1]='s';
              if(next_char=='j') output_string[i+1]='o';
			  if(next_char=='r') output_string[i+1]='a';
              if(next_char=='c') output_string[i+1]='t';
              if(next_char=='d') output_string[i+1]='o';
              if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='t';
			  if(next_char=='n') output_string[i+1]='o';
			  if(next_char=='m') output_string[i+1]='u';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='a';


                break;
				
            case 's':
              if(next_char=='b') output_string[i+1]='s';
              if(next_char=='j') output_string[i+1]='s';

                break;
				
            case 't':
              if(next_char=='b') output_string[i+1]='t';
              if(next_char=='c') output_string[i+1]='t';
              if(next_char=='d') output_string[i+1]='o';
              if(next_char=='f') output_string[i+1]= random_vowel();
              if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='t';
			  if(next_char=='n') output_string[i+1]='o';
			  if(next_char=='m') output_string[i+1]='u';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='a';

                break;
				
            case 'u':
              if(next_char=='u') output_string[i+1]='s';
              if(next_char=='n') output_string[i+1]='a';

                break;
				
            case 'v':
              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='j') output_string[i+1]='a';
              if(next_char=='k') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='e';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='s') output_string[i+1]='u';
			  if(next_char=='c') output_string[i+1]='o';
			  if(next_char=='z') output_string[i+1]='i';

                break;
				
            case 'w':
              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='c') output_string[i+1]='a';
              if(next_char=='d') output_string[i+1]='i';
              if(next_char=='f') output_string[i+1]='u';
              if(next_char=='g') output_string[i+1]='o';
              if(next_char=='k') output_string[i+1]='i';
              if(next_char=='l') output_string[i+1]='u';
              if(next_char=='m') output_string[i+1]='o';
              if(next_char=='n') output_string[i+1]='e';
              if(next_char=='p') output_string[i+1]='o';
              if(next_char=='q') output_string[i+1]='a';
              if(next_char=='s') output_string[i+1]='i';
              if(next_char=='t') output_string[i+1]='e';

              if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='e';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='z') output_string[i+1]='i';

                break;
				
            case 'x':
              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='c') output_string[i+1]='a';
              if(next_char=='d') output_string[i+1]='i';
              if(next_char=='f') output_string[i+1]='u';
              if(next_char=='g') output_string[i+1]='o';
              if(next_char=='k') output_string[i+1]='i';
              if(next_char=='l') output_string[i+1]='u';
			  if(next_char=='v') output_string[i+1]='e';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='z') output_string[i+1]='i';
			  if(next_char=='n') output_string[i+1]='a';
			  if(next_char=='m') output_string[i+1]='i';
              if(next_char=='p') output_string[i+1]='o';
              if(next_char=='q') output_string[i+1]='a';
              if(next_char=='s') output_string[i+1]='i';
              if(next_char=='t') output_string[i+1]='e';

              if(next_char=='j') output_string[i+1]='a';

                break;
				
            case 'y':
              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='c') output_string[i+1]='a';
              if(next_char=='d') output_string[i+1]='i';
              if(next_char=='f') output_string[i+1]='u';
              if(next_char=='g') output_string[i+1]='o';
              if(next_char=='k') output_string[i+1]='i';
              if(next_char=='l') output_string[i+1]='u';
			  if(next_char=='e') output_string[i+1]='s';
			  if(next_char=='v') output_string[i+1]='m';
			  if(next_char=='w') output_string[i+1]='n';
			  if(next_char=='x') output_string[i+1]='p';
			  if(next_char=='z') output_string[i+1]='d';
			  if(next_char=='y') output_string[i+1]='e';
                break;
				
            case 'z':
			  if(next_char=='v') output_string[i+1]='e';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='z') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='i';
			  if(next_char=='m') output_string[i+1]='o';
			  if(next_char=='n') output_string[i+1]='a';
			  if(next_char=='t') output_string[i+1]='i';
			  if(next_char=='c') output_string[i+1]='o';
			  if(next_char=='d') output_string[i+1]='a';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='g') output_string[i+1]='o';
                break;
   }
 }

//now what we're gping to do, is run through the string looking
//for runs of three characters the same
if (char_count>3)
 {
  for (i=1;i<char_count-1;i++)
  {
  char next_char;
  char next_char2;
  char this_char;
  next_char2=output_string[i+2];  
  next_char=output_string[i+1];
  this_char=output_string[i];
  if (this_char==next_char && next_char==next_char2)
      {
       output_string[i+1]=random_vowel();	//replace middle with a
      }
  }
 }

//now run through the string looking
//for runs of three consonants and replace the middle witha vowel
if (char_count>3)
 {
  for (i=1;i<char_count-1;i++)
  {
  char next_char;
  char next_char2;
  char this_char;
  next_char2=output_string[i+2];  
  next_char=output_string[i+1];
  this_char=output_string[i];
  if (is_cons(this_char)==1 && is_cons(next_char)==1 && is_cons(next_char2)==1)
      {
       output_string[i+1]=random_vowel();	//replace middle with a
      }
  }
 }

//now run through the string looking
//for runs of three vowels and replace the middle witha cons
if (char_count>3)
 {
  for (i=1;i<char_count-1;i++)
  {
  char next_char;
  char next_char2;
  char this_char;
  next_char2=output_string[i+2];  
  next_char=output_string[i+1];
  this_char=output_string[i];
  if (is_vowel(this_char)==1 && is_vowel(next_char)==1 && is_vowel(next_char2)==1)
      {
       output_string[i+1]=get_consonant(seed/RangedRdm(1,5));	//replace middle with a
      }
  }
 }


}

//is it a consonant (opposite of vowel)
int is_cons(unsigned char test)
{
if (test=='A' || test=='E' || test=='I' || test=='O' || test=='U' ||
    test=='a' || test=='e' || test=='i' || test=='o' || test=='u'
    ) return 0;
else
return 1;
}

int is_vowel(unsigned char test)
{
if (test=='A' || test=='E' || test=='I' || test=='O' || test=='U' ||
    test=='a' || test=='e' || test=='i' || test=='o' || test=='u'
    ) return 1;
else
return 0;
}

unsigned char random_vowel()
{
int temp;
 temp=RangedRdm(0,4);
 if (temp==0) return 'a';
 if (temp==1) return 'e';
 if (temp==2) return 'i';
 if (temp==3) return 'o';
 if (temp==4) return 'u';
 
 return 'a';		// if something goes wrong
}

unsigned char get_consonant(int seed)
{
unsigned char temp_char;
temp_char=seed%25+'a';
if (is_vowel(temp_char)) temp_char++;
if (temp_char>'z') temp_char='z';
return temp_char;
}


unsigned char get_capital_consonant(int seed)
{
unsigned char temp_char;
temp_char=seed%25+'A';
if (is_vowel(temp_char)) temp_char++;
if (temp_char>'Z') temp_char='Z';
return temp_char;
}

unsigned char get_vowel(int seed)
{
int temp;
temp=seed%5;

if (temp==0) return 'a';
if (temp==1) return 'e';
if (temp==2) return 'i';
if (temp==3) return 'o';
if (temp==4) return 'u';

return 'a';		// if something goes wrong
}

//this routine generates either one or two characters, depending on the seed.
//it also sets the global variables vowel and consonant accordingly, 
//and stores the last character generated in last_char
//and takes a flag specifying whether the first character generated should be
//a vowel or consonant 
//void gen_char(int vc_flag, int seed, char * where)
int last_char,last2_char;
void gen_char(int char_pos, int seed, char * where)
{
int temp,seeda;

//if last was a q then return u
if (last_char=='q')
    {
	temp='u';
	goto end;
    }

///if this is char 2 then we must gen the alternate
if (char_pos==1)
 {
  if (is_vowel(last_char)==1)
     {
     temp=get_consonant(seed);
     goto end;
     }
  else
     {
     temp=get_vowel(seed);
     goto end;
     }
 }   
//gen char
temp=seed%26;
if (temp>25) temp=25;;
temp+='a';

//check for double vowels
if (is_vowel(last_char)==1)
 {
  if (is_vowel(temp)==1)
    {
    if(last_char==temp)
         {
         if (temp=='a' || temp=='i' || temp=='u')	//dont allow double aa ii uu
             {
              temp=get_consonant(seed);
             }
         }
    }
 }
//check for triple letters
seeda=seed;
while (temp & last_char & last2_char==temp)	//while they're all the same
    {
              seeda++;	//prevent posssible loop
              temp=get_consonant(seeda);
    }
    

end: last2_char=last_char;
last_char=temp;
where[0]=1;
where[1]=temp;

}

// *********************************GRAPHICS ARE IN planet_gen_main.c




