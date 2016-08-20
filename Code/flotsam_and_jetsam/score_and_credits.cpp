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

//score and credits

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <NumberFormatting.h>
#include "score_and_credits.h"
#include "3d_structures_def.h"
#include	"wingman.h"
#include	"zex_misc.h"
#include "dlp.h"
#include "cargo.h"

void add_to_score_and_credits(int object_just_destroyed)
{
extern int score,credits,showing_stats;
extern int difficulty,promotion_points,kills;
extern	DynObjectsFixedSize *ocb_ptr;

int id,wingman;
int earn;
int hit_by,owner,zex_shot;
unsigned long owner_id;	//who fired the shot?



if (showing_stats==1) return;	//no score when stats or docked

zex_shot=0;

hit_by=(*ocb_ptr).object_list[object_just_destroyed].hit_by_slot;
if (hit_by!=-1)
{
 owner=(*ocb_ptr).object_list[hit_by].who_spawned_this_object;
 if (owner!=-1)
 {
  owner_id=(*ocb_ptr).object_list[owner].Oid;
  if (owner_id=='ZEXS') zex_shot=1;
  else zex_shot=0;
 }
 else zex_shot=0;
}

if (zex_shot==0) return;
//Beta 5 - kills was incremented here

id=(*ocb_ptr).object_list[object_just_destroyed].Oid;
if (id=='MIS1' || id=='MIS2' || id=='MIS3' || id=='EX01') return; 	  //dont do for cannons being destroyed

															//or explosion
switch (id)
 {
  case 'MON1':	//not used in the game
                score+=RangedRdm(10,18);
                earn=RangedRdm(30,40);
                credits+=earn;
                show_earnings(earn);
				gen_cargo(object_just_destroyed,difficulty);	//possibly throw out a cargo container
				promotion_points+=5;
				kills++;	//Beta 5
                break;
  case 'WH01':
                score+=RangedRdm(20,25);
                break;

  case 'MON2':	//little one - M1
                score+=RangedRdm(20,40);
                earn=RangedRdm(40,50);
                credits+=earn;
                show_earnings(earn);
				promotion_points+=10;
				kills++;	//Beta 5
                break;
  
  case 'MON3':	//M3 - blue
                score+=RangedRdm(75,152);
                earn=RangedRdm(60,80);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
				}
				kills++;	//Beta 5
				promotion_points+=15;
				gen_cargo(object_just_destroyed,2+difficulty);	//possibly throw out a cargo container
                break;

  case 'GRN1':
                score+=RangedRdm(160,170);
                earn=RangedRdm(100,120);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
				}
				promotion_points+=20;
				kills++;	//Beta 5
				gen_cargo(object_just_destroyed,3+difficulty);	//possibly throw out a cargo container
                break;
				
  case 'ASH1':	//shuttle
                score+=RangedRdm(285,372);
                earn=RangedRdm(300,350);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
				}
                promotion_points+=40;
				gen_cargo(object_just_destroyed,90+difficulty);	//possibly throw out a cargo container
				gen_cargo(object_just_destroyed,90+difficulty);	//possibly throw out a cargo container
				kills++;	//Beta 5
                break;


  case 'MON4':
                score+=RangedRdm(80,152);
                earn=RangedRdm(150,160);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
                }
				gen_cargo(object_just_destroyed,10+difficulty);	//possibly throw out a cargo container
				promotion_points+=25;
				kills++;	//Beta 5
                break;

//phoenix
  case 'MN31': case 'MN32': case 'MN33': case 'MN34':
                score+=RangedRdm(80,200);
                earn=RangedRdm(160,180);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
				}
				gen_cargo(object_just_destroyed,6+difficulty);	//possibly throw out a cargo container
				promotion_points+=30;
				kills++;	//Beta 5
                break;

//warper
  case 'WARP':
                score+=RangedRdm(200,400);
                earn=RangedRdm(230,240);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
				}
				gen_cargo(object_just_destroyed,10+difficulty);	//possibly throw out a cargo container
				promotion_points+=40;	//Beta 5 - was 16
				kills++;	//Beta 5
                break;


  case 'MON6':	//python
                score+=RangedRdm(280,352);
                earn=RangedRdm(275,290);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
				}
				gen_cargo(object_just_destroyed,5+difficulty);	//possibly throw out a cargo container
               		promotion_points+=55;
				kills++; //Beta 5
                break;
                
  case 'EXC1':	//excalibur
                score+=RangedRdm(290,392);
                earn=RangedRdm(360,380);
                credits+=earn;
                show_earnings(earn);
               if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
				}
				gen_cargo(object_just_destroyed,10+difficulty);	//possibly throw out a cargo container
				promotion_points+=70;
				kills++;	//Beta 5
                break;
                

  case 'ATRA':	//alien transporter
                score+=RangedRdm(600,800);
                earn=RangedRdm(870,930);	//make this worth the players while
                credits+=earn;
                show_earnings(earn);
				{
					int i=RangedRdm(3,6);	//spill lots of cargo
					for (int n=0;n<i;n++)
					{
						extern int number_of_cargos;
						gen_specific_cargo(object_just_destroyed,RangedRdm(0,number_of_cargos));	//throw out a cargo container
					}
				}
				promotion_points+=200;	//Beta 5 - was 40
				kills++;
                break;
				               
  case 'SGN1':	//static gun
                score+=RangedRdm(35,62);
                earn=RangedRdm(45,85);
                credits+=earn;
                show_earnings(earn);
				promotion_points+=5;
				kills++;	//Beta 5
                break;
				
  case 'IOD1':	//ionic device
                score+=RangedRdm(150*difficulty,300*difficulty);
                earn=RangedRdm(350*difficulty,450*difficulty);
                credits+=earn;
                show_earnings(earn);
				promotion_points+=2;
				kills++;	//Beta 5
				break;
  
  case 'EC01':	//escape capsule
                score+=0;
                earn=-30;
                credits+=earn;
                if (credits<0) credits=0;
                show_earnings(earn);
                
				promotion_points-=35;
                if (promotion_points<0) promotion_points=0;
                break;


//static laser
  case 'SLS1': case 'SLS2': case 'SLS3':
                score+=RangedRdm(80,200);
                earn=RangedRdm(100,200);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
                wingman=find_a_wingman();
                wingman_diealienbyboss_text(wingman);	    //eh42: nice shot boss! etc
				}
				promotion_points+=30;
                break;

                

//boids
  case 'BID1': case 'BID2': case 'BID3': case 'BID4':
                score+=RangedRdm(500,1500);
                earn=RangedRdm(230,400);
                credits+=earn;
                show_earnings(earn);
                	promotion_points+=20;
                break;

  case 'AST1':
                score+=RangedRdm(100,500);
                earn=RangedRdm(130,150);	//we don't like asteroids
                credits+=earn;
                show_earnings(earn);
				promotion_points+=25;	//Beta 5 - was 2
                break;


  case 'ZZ1A': case 'ZZ1B': case 'ZZ1C':	//missile
                score+=RangedRdm(900,1700);
                break;
  
  default :
                break;

 }
}

char credits_string[]=" CREDITS";
char deduct_string[]="DESTROYING THE ESCAPE CAPSULE HAS JUST COST YOU 30 CREDITS.";
char deduct_all_string[]="DESTROYING THE ESCAPE CAPSULE HAS COST YOU ALL YOUR CREDITS.";

void show_earnings(int amount)
{
unsigned char no_string[24];
int end_of_number_string,index;
unsigned char the_char=13;
extern int credits;

 if (amount<0)
  {
  if (credits==0)
  add_to_text_display((char *) deduct_all_string,0);
  else
  add_to_text_display((char *) deduct_string,0);
  
  return;
  }
 ZexNumToString(amount,no_string);
//tag on credits string
 end_of_number_string=no_string[0]+1;	 //quick p to c hack
 no_string[0]=' ';
 index=0;
//copy from credits_string to no_string
 while(the_char!=0)
 {
 the_char=credits_string[index];
 no_string[end_of_number_string+index]=the_char;
 index++;
 }
add_to_text_display((char *) &no_string[1],0);
}


