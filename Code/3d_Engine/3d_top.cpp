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

//3d_top.c

// ***********************************************************************************
// * INCLUDED FILES
// *

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
//oldmac//#include <MacTypes.h>
//oldmac//#include <MacMemory.h>
//oldmac//#include <fp.h>

#include <math.h>

#include "3d_top_int.h"
#include "matrix_primitives.h"
#include "3d_top.h"
#include "Error_handler.h"
#include "camera.h"
#include "insert_object.h"
#include "monster_control_misc.h"
#include "debug_timing.h"

//#ifndef __zex_defs__
//#include "Zex_defs.h"
//#endif


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

//3d_top.int
float global_matrix[4][4];
DynObjectsFixedSize *ocb_ptr;
StatObjectsFixedSize *ocb_Sptr;
cmap_objects *cmap_ptr_base;

_3D *points_list_ptr;
visible_poly *visible_points_list_ptr;
_3D *points_list_ptr_base;
_3D *points_list_ptr_reset;	//reset value for points_list_pointer
visible_poly *visible_points_list_base;
_3D *world_points_list_ptr;
_2DPoly *view_points_list_ptr;
_3D light_normal;	//the light vecterre
_3D camera_pos;	//+z inwards remember
_3D camera_pos_delta;	//change to enable auto camera movement - dont do it anymore!

int camera_rotation_z;	//obviously the camera rot
int camera_rotation_x;
int camera_rotation_y;
int camera_rotation_delta_z; //set this to !0 and caamera rots around z
textured_connections fixed_triag_connection [3] = {{ 0,-1,-1,0 },
	{ 1,-1,-1,0 },
	{ 2,-1,-1,0 }}; //used to create polys in explode
_3D fixed_triag_points[3] = {{ 0,50,0 },
	{ -50,-50,0 },
	{ 50,-50,0 } };

//Polygon **Polygon_handle;	- can't use this 
Polygon *Polygon_ptr;	//these two are temp for drawing with system poly routine

textured_connections *connection_list_ptr;	 //global connection list ptr
textured_connections *connection_list_ptr_reset;	//global connection list ptr
textured_connections* connection_list_top;

_ZPoly *Spoly_list_ptr;	//static objects holding area, compiled from Mesh input
_ZPoly *Spoly_list_ptr_reset;
_ZPoly * Spoly_list_top;

_ZPoly *poly_list_ptr;	//dynamic working area
_ZPoly_objs *dynamic_polys_ptr;
int *points_offsets_ptr; //array of ints specifying offset to points for object.

//index into various storages
int connection_list_index;
int points_list_index;
int world_points_list_index;
int dynamic_object_index;
int poly_list_index;
int freeze;	//if 1 3d does not add deltas
			//extent of various storages

int points_list_top;
int visible_points_list_top;
int view_points_list_top;
int world_points_list_top;
int dynamic_object_top;
int poly_list_top;
//int Spoly_list_top;

int points_offsets_top;
int	total_points_counter;
//ptrs to polys for z sort
_ZPolySort zsort_list[MAX_NUMBER_OF_VISIBLE_POLYS];	//max of MAX_NUMBER_OF_VISIBLE_POLYS polys in system at any on time
int zsort_index; //should not really be here
int zsort_start_index,zsort_count,zbuffer_next_free;
float last_z;

// ***********************************************************************************
// * EXTERNAL VARIABLES
// *

extern int Mode320,show_control_panel;

// ***********************************************************************************
// * CODE
// *

//this is the top level function of the transformation engine
//it takes a list of points
//do_3d (int number_of_points,float *points_list)
void do_3d ()
{
int	number_of_points;
//_3D world_point;
_3D dest_point;
_3D source_point;
_3D *the_points;
_3D object_position;
int object_rotation_x,object_rotation_y,object_rotation_z;
extern	int	total_points_counter;
extern int dynamic_object_index;
extern int dynamic_object_total;
extern int level_complete;

int points_offset=0;	//how many points we have transformed (ultimately added to connections)
int this_object_points_offset=0;
int object_counter;
int *offsets;


_3D *output_points;
_ZPoly *the_polys;

int i,number_of_polys,active_object_counter;

extern int *points_offsets_ptr; //array of ints specifying offset to points for object.
extern int camera_rotation_z, camera_rotation_x, camera_rotation_y;
extern _3D ship_normal;	//Start pointing out of screen
extern _3D ship_rotated_normal; //from engine
extern int objs_displayed;
extern int camera_mode;
//textured_connections *connection;	//for Gourad check
ZObject *source_object;
register ZObject * current_object_ptr;
//int debug_loop_count=0;
//end of declarations

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//object brick wall fov filter - SB 01/12/98
//trivial rejection
active_object_counter=0;
object_counter=0;
objs_displayed=0;

//these routines are the object trivial rejection phase. They must either set or clear the is_visible flag in the dynamic ocb
//if (camera_mode==1)
//{
//while (active_object_counter<dynamic_object_index)
// {
//    if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
//   {
//       //if any one point of the bounding box is within fov, the is_visible is set
//       
//       active_object_counter++;
//   } //end of if in_use
//    
//  object_counter++;
// }	  //end of while
//
//}
//
//else 
if (1)	//was camera_mode
{
while (active_object_counter<dynamic_object_index && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("triv_rejection looped");
    current_object_ptr=&ocb_ptr->object_list[object_counter];

  if (current_object_ptr->in_use==1) //if object is alive
   {

    if (current_object_ptr->no_trivial_rejection==0
    || current_object_ptr->no_trivial_rejection==2)	//if we can reject it OR we can reject it until it's visible
     {
//check if object is visible, and if so set is_visible else clear it
//capitalise on the fact thatt this is a scroller so we only consider absolute coords relative to camera
     int temp,vis,x_corr,y_corr;
     extern _3D camera_pos;
     vis=0;
     
//we have two levels for speed. Objects <1000 and objects >= 1000
     if (current_object_ptr->Oworldz>5000)
     {
     x_corr=static_cast<int>(current_object_ptr->Oworldz/3500);	     //was 3500
     y_corr=static_cast<int>(current_object_ptr->Oworldz);
     temp=static_cast<int>(-camera_pos.x-current_object_ptr->Oworldx);
     if (temp>camera_pos.z*x_corr && temp<-camera_pos.z*x_corr) vis+=1;	//max +-9000
    
     temp=static_cast<int>(-camera_pos.y-current_object_ptr->Oworldy);
      if (temp>camera_pos.z-(y_corr-1000) && temp<-camera_pos.z+(y_corr-1000)) vis+=1;	  //max +-5000
     
     }
     else	//object<1000
     {
     temp=static_cast<int>(-camera_pos.x-current_object_ptr->Oworldx);
     if (temp>camera_pos.z*2 && temp<-camera_pos.z*2) vis+=1;	 //max +-9000
    
     temp=static_cast<int>(-camera_pos.y-current_object_ptr->Oworldy);
      if (temp>camera_pos.z-2000 && temp<-camera_pos.z+2000) vis+=1;	//max +-5000

     }

      if (vis==2)	 //visible in x AND y or it's a star
      {
      current_object_ptr->is_visible=1;
        //if was don't move until visible, now make it no trivial rejection
        if ((*ocb_ptr).object_list[object_counter].no_trivial_rejection==2) 
        (*ocb_ptr).object_list[object_counter].no_trivial_rejection=1;

      objs_displayed++;
      }
      else
      current_object_ptr->is_visible=0;	//is not going to be processed by 3d engine
      														
      active_object_counter++;
     } //end of no_trivial_rejection
     else //object cant be rejected
     {
        objs_displayed++;
        current_object_ptr->is_visible=1;	//so we have to maake it visible
        active_object_counter++;
      }
   } //end of if in_use
 object_counter++;

 //е check next object visible
 }	//end of while

}	//end of camera mode==0



//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//Transformation/rotation on visible objects - SB 02/11/98
total_points_counter=0;	///for subsequent function - pers for example

offsets =points_offsets_ptr;	//point to offsets arrray

world_points_list_index=0;	//our transformed points go here
output_points =world_points_list_ptr;
active_object_counter=0;
object_counter=0;
//debug_loop_count=0;
while (active_object_counter<dynamic_object_index  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
{
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("3D main looped");
    current_object_ptr=&ocb_ptr->object_list[object_counter];

 if (current_object_ptr->in_use==1)
  {
   if (current_object_ptr->is_visible==1)
    {
//е object transform/rotate loop
     this_object_points_offset =points_offset;	//the offset applied to this objects connections

     source_object=&(*ocb_ptr).object_list[object_counter];

     number_of_points=(*source_object).number_of_points;

//  number_of_points=(*ocb_ptr).object_list[object_counter].number_of_points;
     number_of_polys=(*source_object).number_of_polys;

//  number_of_polys=(*ocb_ptr).object_list[object_counter].number_of_polys;
     the_polys=(*source_object).polys_ptr;

//  the_polys=(*ocb_ptr).object_list[object_counter].polys_ptr;
     the_points=(*source_object).points_ptr;

//  the_points=(*ocb_ptr).object_list[object_counter].points_ptr;
//store trasnformed points ptr
     (*source_object).world_points_ptr=output_points;

//  (*ocb_ptr).object_list[object_counter].world_points_ptr=output_points;
//do any movement inherent in object
     if (freeze==0)
     {
     (*source_object).Oworldx+=(*source_object).Omovedx;
     (*source_object).Oworldy+=(*source_object).Omovedy;
     (*source_object).Oworldz+=(*source_object).Omovedz;
     }
//(*ocb_ptr).object_list[object_counter].Oworldx+=(*ocb_ptr).object_list[object_counter].Omovedx;
//point to points
    
     object_position.x=(*source_object).Oworldx;
     object_position.y=(*source_object).Oworldy;
     object_position.z=(*source_object).Oworldz;


     if (freeze==0)
     {
//rotate object arround y
     object_rotation_y=(*source_object).Oroty;	//get current roty
     object_rotation_y+=source_object->Orotdy;	//add on delta rot
     if (object_rotation_y > 359) object_rotation_y = object_rotation_y - 360;	//check if > 255
     if (object_rotation_y < 0) object_rotation_y = object_rotation_y + 360;	//check if > 255

     source_object->Oroty=object_rotation_y; //store back in object new roty

//rotate object arround x
     object_rotation_x=source_object->Orotx;	//get current roty
     object_rotation_x+=source_object->Orotdx;	//add on delta rot
     if (object_rotation_x > 359) object_rotation_x = object_rotation_x - 360;	//check if > 255
     if (object_rotation_x < 0) object_rotation_x = object_rotation_x + 360;	//check if > 255
      source_object->Orotx=object_rotation_x; //store back in object new roty

//rotate object arround z
     object_rotation_z=source_object->Orotz;	//get current roty
     object_rotation_z+=source_object->Orotdz;	//add on delta rot
     if (object_rotation_z > 359) object_rotation_z = object_rotation_z - 360;	//check if > 255
     if (object_rotation_z < 0) object_rotation_z = object_rotation_z + 360;	//check if > 255
     source_object->Orotz=object_rotation_z; //store back in object new roty
     }
     else	//frozen
     {
     object_rotation_z=source_object->Orotz;	//get current roty
     object_rotation_x=source_object->Orotx;	//get current roty
     object_rotation_y=(*source_object).Oroty;	//get current roty
     }
//delete these next three
//object_rotation_x=(*ocb_ptr).object_list[object_number].Orotx;	//get current roty
//object_rotation_y=(*ocb_ptr).object_list[object_number].Oroty;	//get current roty
//object_rotation_z=(*ocb_ptr).object_list[object_number].Orotz;	//get current roty
//Send all points in this object through rot/tran engine
//step 2 - set global matrix to identity
     MAT_Identity(global_matrix);
//step 3 - scalingglobal matrix with scaling for object - not done.
//step 4 - rotate global matrix by angles of object
     TR_Rotate(global_matrix,object_rotation_x, object_rotation_y, object_rotation_z);
//step 5 - translate the global matrix with the position of the object
     TR_Translate(global_matrix,object_position.x,object_position.y,object_position.z);
//camera rotate and trasnlate goes here, but is at step 7 for now
//step 6 - multiply the local coords by the global_matrix to get world coords

//do camera stuff with same matrix as well
//step 8 - translate the global matrix with the !negative! position of the camera
     TR_Translate(global_matrix,camera_pos.x,camera_pos.y,-camera_pos.z);
//step 9 - rotate global matrix by angles of camera
     TR_Rotate(global_matrix,camera_rotation_x, camera_rotation_y, camera_rotation_z);

//Now transform/rotate the points.
//Lets eat some cycles...
//Zex spends it's days here...
    for (i=0; i<number_of_points; i++)
    {
//    source_point=(*the_points++);
//    VEC_MultMatrix(&source_point,global_matrix,&world_point);
//    (*output_points++)=world_point;
//    total_points_counter++;
//    points_offset++;	  //done another point

//    source_point=(*the_points++);
      VEC_MultMatrix(&(*the_points),global_matrix,&(*output_points));
      total_points_counter++;
	  the_points++;
	  output_points++;
      points_offset++;	//done another point

    }
//rottran bounding box points
//    VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].box_min,global_matrix,&(*ocb_ptr).object_list[object_counter].box_min_translated);
//    VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].box_max,global_matrix,&(*ocb_ptr).object_list[object_counter].box_max_translated);

//  for (i=0; i<2; i++)
//  {
//    source_point=(*ocb_ptr).object_list[object_counter].BBox[i];
//    VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].BBox[i],global_matrix,&(*ocb_ptr).object_list[object_counter].BBox_translated[i]);
//  }
//now rotate and transfrom collision boxes
    MAT_Identity(global_matrix);
//step 3 - scalingglobal matrix with scaling for object - not done.
//step 4 - rotate global matrix by angles of object
    TR_Rotate(global_matrix,object_rotation_x, object_rotation_y, object_rotation_z);
    TR_Translate(global_matrix,object_position.x,object_position.y,object_position.z);
//step 9 - rotate global matrix by angles of camera
//    TR_Rotate(global_matrix,camera_rotation_x, camera_rotation_y, camera_rotation_z);
    VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].box_min,global_matrix,&(*ocb_ptr).object_list[object_counter].box_min_translated);
    VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].box_max,global_matrix,&(*ocb_ptr).object_list[object_counter].box_max_translated);

//now  rotate the face_normal into face_normal_translated 
    MAT_Identity(global_matrix);
//step 3 - scalingglobal matrix with scaling for object - not done.
//step 4 - rotate global matrix by angles of object
    TR_Rotate(global_matrix,object_rotation_x, object_rotation_y, object_rotation_z);
//step 9 - rotate global matrix by angles of camera
//    TR_Rotate(global_matrix,camera_rotation_x, camera_rotation_y, camera_rotation_z);

      VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].object_normal,global_matrix,&(*ocb_ptr).object_list[object_counter].object_normal_translated);

    for (i=0; i<number_of_polys; i++)
     {
      VEC_MultMatrix(&(*the_polys).face_normal,global_matrix,&(*the_polys).face_normal_translated);      
      the_polys++;	//next poly

     }
   } //end of is_visible
  active_object_counter+=1;

 } //end of if in_use
object_counter++;

//е do next object
}	//end of while
//finally rotate ship normal - the matrix is still set up
    MAT_Identity(global_matrix);
    TR_Rotate(global_matrix,(*ocb_ptr).object_list[0].Orotx, (*ocb_ptr).object_list[0].Oroty, (*ocb_ptr).object_list[0].Orotz);
//    TR_Rotate(global_matrix,camera_rotation_x, camera_rotation_y, camera_rotation_z);

    source_point=ship_normal;
    dest_point=ship_rotated_normal;
    VEC_MultMatrix(&source_point,global_matrix,&dest_point);
    ship_rotated_normal=dest_point;

}	//end


//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//this routine looks at each "on" object.
//for each poly in the object, it determins if visible and if so
//copies visible points (and the poly def) to visible_points_list[] for further work by pers2
float zex_z;	//zex's z so we know ehn to draw the dlp

void do_culling2()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern	int number_of_polys_displayed;
extern visible_poly *visible_points_list_ptr;
extern visible_poly *visible_points_list_base;
//extern int zsort_index;

//_3D Camera_normal;	//we have to calc this on every frame :(
_ZPoly *the_polys;
//int	points_offset;	//offset to points for the polys in the object
//float view_angle;
int poly_count;
int	how_many_polys;
//_3D poly_plane;
int the_object=0;
//int frontface;
//_3D *world_points;
textured_connections *pconnections;
_3D p1,p2,p3;	//our three points and the z point for gourad
_3D *this_object_points;
//_3D *this_object_object_points;
int conn,D,object_count,active_object_count;
float average_z,light_value;
int vis,temp_x,temp_y;	//poly visible calcs
int poly_type;	//flat, gourad or texture mapped
int got_zex_z=0;	//where zex z is - first point of first object - flag=1 if so
//int touching_poly;
extern int zsort_start_index,zsort_count, light_amplitude;
register ZObject * current_object_ptr;

zsort_index=0;
reset_zsort_insert();

//dynamic_object_index=1;
number_of_polys_displayed=0;
visible_points_list_ptr=visible_points_list_base;	//we output visible points here

object_count=active_object_count=0;
while (active_object_count<dynamic_object_index  && object_count<MAX_NUMBER_OF_DYN_OBJECTS)
{
     current_object_ptr=&ocb_ptr->object_list[object_count];


  if (current_object_ptr->in_use==1)
  {
   if (current_object_ptr->is_visible==1 && current_object_ptr->eng_invisible==0)
    {
    
     if (current_object_ptr->has_roll==1) calc_roll(object_count);	//go calculate the roll in monster_misc
//point to rotated world points
  this_object_points=current_object_ptr->world_points_ptr;	//the points for this object
//  this_object_object_points=(*ocb_ptr).object_list[object_count].points_ptr;	  //the points for this object

  how_many_polys=current_object_ptr->number_of_polys;
  the_polys=current_object_ptr->polys_ptr;

  for (poly_count=0;poly_count<how_many_polys;poly_count++)
  {

    pconnections=(*the_polys).poly_connections_ptr;	
    poly_type=(*pconnections).type;	//flat, gourad or textured

    conn=(*pconnections).the_node;
    pconnections++;
    p1=(*(this_object_points+conn));
    conn=(*pconnections).the_node;
    pconnections++;
    p2=(*(this_object_points+conn));
    conn=(*pconnections).the_node;	//e.g. 1 to 2, 2 to 3
    pconnections++;
    p3=(*(this_object_points+conn));

//check if poly is in fov and get out of not
//this is a perspective transormation on all three points. We check if any are visible, if they are, we continue

 vis=0;
// temp_x=0;
// temp_y=0;
 

 if (Mode320==1)
 {
 temp_x=static_cast<int>(160+(p1.x*128/p1.z));
 temp_y=static_cast<int>(120-(p1.y*128/p1.z));

 if (temp_x>-80 && temp_x<400 && temp_y>-40 && temp_y<300) vis|=1;

 temp_x=static_cast<int>(160+(p2.x*128/p2.z));
 temp_y=static_cast<int>(120-(p2.y*128/p2.z));
 if (temp_x>-40 && temp_x<400 && temp_y>-40 && temp_y<300) vis|=1;

 temp_x=static_cast<int>(160+(p3.x*128/p3.z));
 temp_y=static_cast<int>(120-(p3.y*128/p3.z));
 if (temp_x>-40 && temp_x<400 && temp_y>-40 && temp_y<300) vis|=1;
 }
 else	//640*480
 {
 temp_x=static_cast<int>(320+(p1.x*256/p1.z));
 temp_y=static_cast<int>(240-(p1.y*256/p1.z));

 if (temp_x>-80 && temp_x<720 && temp_y>-80 && temp_y<560) vis|=1;

 temp_x=static_cast<int>(320+(p2.x*256/p2.z));
 temp_y=static_cast<int>(240-(p2.y*256/p2.z));
 if (temp_x>-40 && temp_x<720 && temp_y>-40 && temp_y<560) vis|=1;

 temp_x=static_cast<int>(320+(p3.x*256/p3.z));
 temp_y=static_cast<int>(240-(p3.y*256/p3.z));
 if (temp_x>-40 && temp_x<720 && temp_y>-40 && temp_y<560) vis|=1;
 }
 
 
// vis=1;
 if (vis!=0)
  {
//D=-x1*(y2*z3-y3*z2)-x2*(y3*z1-y1*z3)-x3*(y1*z2-y2*z1);
//calc poly lighting
//looks like a lot of maths, but is comparable to rotating the surface normal (3 muls less actually!) 
    D=static_cast<int>(-p1.x*(p2.y*p3.z-p3.y*p2.z) - p2.x*(p3.y*p1.z-p1.y*p3.z) - p3.x*(p1.y*p2.z-p2.y*p1.z));
//    frontface=D<0;
//   frontface=1;
/*
 poly_plane=(*the_polys).face_normal_translated;
//calc if vis
 view_angle=(Camera_normal.x*poly_plane.x)+(Camera_normal.y*poly_plane.y)+(Camera_normal.z*poly_plane.z);
 view_angle=acos(view_angle);
 view_angle=view_angle*(360/pi);
//if between 0 and -1 then visible
*/
     if (D<0)	//the poly is visible
     {
//first calculate lighting for this poly
//taken as the difference between poly_rotated_normal and light_normal
//d11 - check for Gourad or Flat shaded, we dont calc light for textured
//we have already calculated a list of touching polys at load time...
//first calc this planes light value

   if( ((current_object_ptr->Oid)&0xff) != '&' )
      {
    light_value=(light_normal.x*(*the_polys).face_normal_translated.x)+(light_normal.y*(*the_polys).face_normal_translated.y)+(light_normal.z*(*the_polys).face_normal_translated.z);
	light_value*=light_amplitude;
    (*the_polys).Plight_value=static_cast<int>(light_value);
      }
    else
      {
      (*the_polys).Plight_value=0;  // self lighting objects
      }
   
   
     average_z=p1.z+p2.z+p3.z;	//for debugging
       (*the_polys).average_z=average_z;	//needed fro z sort
       (*the_polys).PPoints_offset=number_of_polys_displayed; //points offset for renderer
//d7 - we dont z sort anymore, but insert into a linked list of z values
	if (got_zex_z==0)
	{
	 got_zex_z=1;
	 zex_z=average_z;
	}
	
	insert_to_z_buffer(the_polys);
//       zsort_list[zsort_index]=the_polys;	//store ptr to poly in zsort_list
//       zsort_index++;
       zsort_count++;
       if (zsort_count>(MAX_NUMBER_OF_VISIBLE_POLYS-1))
       		report_error_c("Too many visible triags (Do_Culling2).","",zsort_count);

//       (*the_polys).p_visible=true;
       (*visible_points_list_ptr).poly_ptr=the_polys;
       (*visible_points_list_ptr).point1=p1;
       (*visible_points_list_ptr).point2=p2;
       (*visible_points_list_ptr).point3=p3;
       visible_points_list_ptr++;	//ready for next visible poly
       (*the_polys).p_visible=1;	//for gourad if so
       number_of_polys_displayed+=1;
     }
     else
     {
       if ((*pconnections).type==GOURAD)
        {
        light_value=(light_normal.x*(*the_polys).face_normal_translated.x)+(light_normal.y*(*the_polys).face_normal_translated.y)+(light_normal.z*(*the_polys).face_normal_translated.z);
	light_value*=light_amplitude;
    (*the_polys).Plight_value=static_cast<int>(light_value);
        }
        
      (*the_polys).p_visible=0;
     }
    } //end of visible poly
   
    the_polys+=1;	//(*ocb_ptr).object_list[0].polys_ptr;


    }	 //end of poly loop
    }	//end of if vis
   active_object_count++;
   } //end of if on
   object_count++;
  }	//next object - end of while

//var_a=zsort_count;

do_gourad_polys();
}

//runs through the visible polys setting the light values for gouraded polys
void do_gourad_polys()
{
extern int dynamic_object_index;
int active_object_counter,object_counter;
int how_many_polys,poly_count;
textured_connections *pconnections;
_ZPoly *the_polys;
short poly_type;
float gourad_light;
int touching_count,i,touching_poly,light_value;
_ZPoly *temp_poly;
register ZObject * current_object_ptr;
_3D vertex_av;
extern int light_amplitude;

//int debug_loop_count=0;
//now if gourad, we calc the light value of the polys points via it's neighbours and this (averaging) 
//(some from last frame, some from this no doubt but it should be OK) 
active_object_counter=0;
object_counter=0;
while (active_object_counter<dynamic_object_index  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
{
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("do_gourad_polys looped");
     current_object_ptr=&ocb_ptr->object_list[object_counter];

 if (current_object_ptr->in_use==1)
  {
   if (current_object_ptr->is_visible==1)
    {

//run through the polys of this object
    how_many_polys=current_object_ptr->number_of_polys;
    the_polys=current_object_ptr->polys_ptr;

    for (poly_count=0;poly_count<how_many_polys;poly_count++)
    {
 
    pconnections=(*the_polys).poly_connections_ptr;	
    poly_type=(*pconnections).type;	//flat, gourad or textured

       if (poly_type==GOURAD && (*the_polys).p_visible==1)
       {
        pconnections=(*the_polys).poly_connections_ptr;	
//        light_value=(*the_polys).Plight_value;
//        light_value*=2;

//calc p1 "normal"
//because a point can't have a normal we take all connected planes normals and average them

//        gourad_light=light_value;	  //the plane light value
        touching_count=1;
	vertex_av.x=(*the_polys).face_normal_translated.x;
	vertex_av.y=(*the_polys).face_normal_translated.y;
	vertex_av.z=(*the_polys).face_normal_translated.z;
	
//	vertex_av.x=vertex_av.y=vertex_av.z=0;
        for (i=0;i<MAX_TOUCHING_POLYS;i++)
         {
           if((*the_polys).Ptouching1[i]!=-1)
            {
             touching_poly=(*the_polys).Ptouching1[i];
             temp_poly=current_object_ptr->polys_ptr;	     //point to polys for object
             temp_poly+=touching_poly;	//point to poly that is touching this point
//             if ((*temp_poly).p_visible==1)
//             {
             //gourad_light+=(*temp_poly).Plight_value;	//add it's light value
             vertex_av.x+=(*temp_poly).face_normal_translated.x;
             vertex_av.y+=(*temp_poly).face_normal_translated.y;
             vertex_av.z+=(*temp_poly).face_normal_translated.z;
             
             touching_count++;
//             }
           }
         }
   
//       gourad_light/=touching_count;	  //get average light at this point
	vertex_av.x/=touching_count;
	vertex_av.y/=touching_count;
	vertex_av.z/=touching_count;
//calc light
       gourad_light=(light_normal.x*vertex_av.x)+(light_normal.y*vertex_av.y)+(light_normal.z*vertex_av.z);
	gourad_light*=light_amplitude;

//       gourad_light-=32767;	//back to signed
       if (gourad_light<-32000) gourad_light=-32000;
       else
       if (gourad_light>32000) gourad_light=32000;
       (*pconnections).u=static_cast<int>(gourad_light);	//phew!
       pconnections++;	//next point
   
//calc p2
//       gourad_light=light_value;	  //starter for 10
       touching_count=1;
//	vertex_av.x=vertex_av.y=vertex_av.z=0;
	vertex_av.x=(*the_polys).face_normal_translated.x;
	vertex_av.y=(*the_polys).face_normal_translated.y;
	vertex_av.z=(*the_polys).face_normal_translated.z;
       for (i=0;i<MAX_TOUCHING_POLYS;i++)
         {
          if((*the_polys).Ptouching2[i]!=-1)
           {
            touching_poly=(*the_polys).Ptouching2[i];
            temp_poly=current_object_ptr->polys_ptr;	//point to polys for object
            temp_poly+=touching_poly;	//point to poly that is touching this point
//             if ((*temp_poly).p_visible==1)
//             {
//              gourad_light+=(*temp_poly).Plight_value;	//add it's light value
             vertex_av.x+=(*temp_poly).face_normal_translated.x;
             vertex_av.y+=(*temp_poly).face_normal_translated.y;
             vertex_av.z+=(*temp_poly).face_normal_translated.z;
              touching_count++;
//             }
           }
         }
    
//       gourad_light/=touching_count;
	vertex_av.x/=touching_count;
	vertex_av.y/=touching_count;
	vertex_av.z/=touching_count;
    gourad_light=(light_normal.x*vertex_av.x)+(light_normal.y*vertex_av.y)+(light_normal.z*vertex_av.z);
	gourad_light*=light_amplitude;

//       gourad_light-=32767;	//back to signed
       if (gourad_light<-32000) gourad_light=-32000;
       else
       if (gourad_light>32000) gourad_light=32000;
       (*pconnections).u=static_cast<int>(gourad_light);	//phew!
       pconnections++;	//next point
   
//calc p3
//       gourad_light=light_value;	  //starter for 10
       touching_count=1;
//	vertex_av.x=vertex_av.y=vertex_av.z=0;
	vertex_av.x=(*the_polys).face_normal_translated.x;
	vertex_av.y=(*the_polys).face_normal_translated.y;
	vertex_av.z=(*the_polys).face_normal_translated.z;
       for (i=0;i<MAX_TOUCHING_POLYS;i++)
       {
        if((*the_polys).Ptouching3[i]!=-1)
         {
          touching_poly=(*the_polys).Ptouching3[i];
          temp_poly=current_object_ptr->polys_ptr;	//point to polys for object
          temp_poly+=touching_poly;	//point to poly that is touching this point
//          if ((*temp_poly).p_visible==1)
//           {
//            gourad_light+=(*temp_poly).Plight_value;	      //add it's light value
             vertex_av.x+=(*temp_poly).face_normal_translated.x;
             vertex_av.y+=(*temp_poly).face_normal_translated.y;
             vertex_av.z+=(*temp_poly).face_normal_translated.z;
            touching_count++;
//           }
         }
        }
    
//       gourad_light/=touching_count;
	vertex_av.x/=touching_count;
	vertex_av.y/=touching_count;
	vertex_av.z/=touching_count;
    gourad_light=(light_normal.x*vertex_av.x)+(light_normal.y*vertex_av.y)+(light_normal.z*vertex_av.z);
	gourad_light*=light_amplitude;

//       gourad_light-=32767;	//back to signed
       if (gourad_light<-32000) gourad_light=-32000;
       else
       if (gourad_light>32000) gourad_light=32000;
   
       (*pconnections).u=static_cast<int>(gourad_light);	//phew!
       pconnections++;	//next point
      }	//end of gourad poly
      the_polys+=1;	//(*ocb_ptr).object_list[0].polys_ptr;
  
    }	//for loop next poly
    } //end of if visible
    active_object_counter++;
    
   } //end of if in_use
 object_counter++;

 //е check next object visible
 }	//end of while

}


//inserts the poly into a linked list. Position depends on average z - we want the furthest (greatest)
//at the top of the list and the furthest (least) at the  end
//SB 16/12/98
void insert_to_z_buffer(_ZPoly *the_poly)
{
extern _ZPolySort zsort_list[MAX_NUMBER_OF_VISIBLE_POLYS];	//max of MAX_NUMBER_OF_VISIBLE_POLYS polys
extern int zsort_count,zbuffer_next_free;
extern int zsort_start_index;	//holds index of start of list.
extern float last_z;
_ZPoly *current_poly;
register float the_z;
register int current_index,inserted,previous_slot,was_pointing_to;
register _ZPolySort * zbuff_next_free_ptr;


the_z=(*the_poly).average_z;
current_index=zsort_start_index;	//top of the list - always max z
inserted=0;
previous_slot=-1;
//find out where this z fits in the list
//first see if it's greater than the start value!



 current_poly=zsort_list[current_index].poly_data;
 if (current_poly!=0)	//prevent those annoying access exceptions..

 {
  
  if (the_z>zsort_list[current_index].z_value)
  {
  //insert poly
  
  zsort_list[zbuffer_next_free].previous_slot=-1;	//new head of list
  zsort_list[zbuffer_next_free].next_slot=zsort_start_index;	//point to next, which is old start
  zsort_list[zsort_start_index].previous_slot=zbuffer_next_free;	//patch old start to previous this new one

  zsort_list[zbuffer_next_free].poly_data=the_poly;
  zsort_list[zbuffer_next_free].z_value=the_z;	//store z
  zsort_start_index=zbuffer_next_free;	//new start
  zbuffer_next_free++;
  inserted=1;
  zsort_list[zbuffer_next_free].poly_data=0; //make end of list

  }
 }	//end of poly!=0;
 
//search loop
while(inserted==0)
 {
 //check if we have have reached the end of the line
 if (zsort_list[current_index].poly_data==0)
  {
  zsort_list[current_index].poly_data=the_poly;
  zsort_list[current_index].previous_slot=previous_slot;
  zsort_list[current_index].next_slot=zbuffer_next_free;
  zsort_list[current_index].z_value=the_z;	//store z
  zsort_list[zbuffer_next_free].poly_data=0;	//make end of list
  zbuffer_next_free++;
  inserted=1;
  }
 else //if not end of list

  {

  //see if our z is > than the current z 
  //if so this is where we need to insert
  if (the_z>zsort_list[current_index].z_value)
    {
    //insert our poly
    //patch previous to point to our new
    previous_slot=zsort_list[current_index].previous_slot;
    was_pointing_to=zsort_list[previous_slot].next_slot;
    zsort_list[previous_slot].next_slot=zbuffer_next_free;	//make previous point to our new one
    
    //insert new
    zsort_list[zbuffer_next_free].poly_data=the_poly;
    zsort_list[zbuffer_next_free].previous_slot=previous_slot;	//tie this one back to old
    zsort_list[zbuffer_next_free].next_slot=was_pointing_to;	//and our new one points to the old next
    zsort_list[zbuffer_next_free].z_value=the_z;	//store z
    zsort_list[was_pointing_to].previous_slot=zbuffer_next_free;	//follower points to new
    zbuffer_next_free++;
    zsort_list[zbuffer_next_free].poly_data=0;	//make end of list
    inserted=1;
   }
  else	//follow the chain
    {
    previous_slot=current_index;
    current_index=zsort_list[current_index].next_slot;
    }
  }

 } //end of while inserted==0
// last_z=the_z;	//save for next run in-case they're the same
}

//ееroutine resets the z buffer insert
void reset_zsort_insert()
{
extern int zsort_start_index,zsort_count,zbuffer_next_free;
extern _ZPolySort zsort_list[MAX_NUMBER_OF_VISIBLE_POLYS];	//max of MAX_NUMBER_OF_VISIBLE_POLYS polys
zbuffer_next_free=1;	//next free slot
zsort_start_index=0;	//used in insert_to_z_buffer - slot of the start of the list  - max z
zsort_count=0;	//counter of items inserted
//clear top of list
zsort_list[0].previous_slot=-1;	//indicate start of list
zsort_list[0].next_slot=-1;	//indicate end of list
zsort_list[0].poly_data=0;
}
//еееееееееееееееееееее
//sort poly pointers on average z - greatest first (+z is furthest away)
//void do_zsort()
//{
//extern _ZPolySort *zsort_list[MAX_NUMBER_OF_VISIBLE_POLYS];	  //max of MAX_NUMBER_OF_VISIBLE_POLYS polys
//extern int zsort_index;
//int zsort_list_position;
//int did_swap;
//float z1,z2;
//_ZPoly *the_poly;
//did_swap=1;	//if 1 we need to do it all again
//
//while (did_swap)
// {
// did_swap=0;
// zsort_list_position=0;
// 
// while (zsort_list_position+1<zsort_index)
//  {
////get z1 and z2
//    the_poly=*zsort_list[zsort_list_position].;
//    z1=(*the_poly).average_z;
//    the_poly=zsort_list[zsort_list_position+1];
//    z2=(*the_poly).average_z;
//    if (z2>z1)
//     {
//     //swap
//      zsort_list[zsort_list_position+1]=zsort_list[zsort_list_position]; //1 to 2
//      zsort_list[zsort_list_position]=the_poly; //2 to 1
//      did_swap=1;
//     }
//    zsort_list_position++;
//  }
//  
// }	//end of main while
// 
//}	//end off do_zsort
/*
//old routine
void do_culling()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern	int number_of_polys_displayed;

_3D Camera_normal;	//we have to calc this on every frame :(
_ZPoly *the_polys;
int	points_offset;	//offset to points for the polys in the object
float view_angle;
int poly_count;
int	how_many_polys;
_3D poly_plane;
int the_object=0;
int frontface;
_3D *world_points;
_Connection *pconnections;
_3D p1,p2,p3;	//our three points
_3D *this_object_points;
_3D *output_points;
int conn,D,object_count;
//for each object...
for (object_count=0;object_count<dynamic_object_index;object_count++)
{
//get_camera_normal(&Camera_normal);	//in camera_control.c
  output_points =world_points_list_ptr;
//point to rotated world points
 points_offset=(int)(*ocb_ptr).object_list[object_count].points_ptr - (int)points_list_ptr_base;	//the points for this object
//and we want offset as xy
 points_offset=points_offset/sizeof(_3D);

  this_object_points = output_points+points_offset;
  how_many_polys=(*ocb_ptr).object_list[object_count].number_of_polys;
  the_polys=(*ocb_ptr).object_list[object_count].polys_ptr;

  for (poly_count=0;poly_count<how_many_polys;poly_count++)
  {

    pconnections=(*the_polys).poly_connections_ptr;	
//    pconnections++;		  //skip connection count - we only need 3
    world_points=(*ocb_ptr).object_list[the_object].points_ptr;	//the points for this object
    conn=(*pconnections++);
    p1=(*(this_object_points+conn));
    conn=(*pconnections++);
    p2=(*(this_object_points+conn));
    conn=(*(pconnections));	//e.g. 1 to 2, 2 to 3
    p3=(*(this_object_points+conn));
//D=-x1*(y2*z3-y3*z2)-x2*(y3*z1-y1*z3)-x3*(y1*z2-y2*z1);
//looks like a lot of maths, but is comparable to rotating the surface normal (3 muls less actually!) 
    D=-p1.x*(p2.y*p3.z-p3.y*p2.z) - p2.x*(p3.y*p1.z-p1.y*p3.z) - p3.x*(p1.y*p2.z-p2.y*p1.z);
    frontface=D<0;


// poly_plane=(*the_polys).face_normal_translated;
//calc if vis
// view_angle=(Camera_normal.x*poly_plane.x)+(Camera_normal.y*poly_plane.y)+(Camera_normal.z*poly_plane.z);
// view_angle=acos(view_angle);
// view_angle=view_angle*(360/pi);
//if between 0 and -1 then visible


     if (frontface)
     {
       (*the_polys).p_visible=true;
       number_of_polys_displayed+=1;
     }
     else (*the_polys).p_visible=false;
 
     the_polys+=1;	//(*ocb_ptr).object_list[0].polys_ptr;


   }	//end of poly loop
  }	//next object

}
 */

//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//perspective transformation from world_points_list_ptr to view_list_ptr
//have tested both ways, I'm pretty sure it's ffaster just splatting all the points 
//through rather than seeing which points are disabled, specially as objects will
//probably share points? We'll have to see how it goes.

//modded 060699 SB to remove translated points behind camera

float depth_of_field=256;

void do_pers2(int width, int height)
//this is the rwritten version used in conjunction with do_culling2.
{
extern _2DPoly *view_points_list_ptr;
extern visible_poly *visible_points_list_base;
extern int OGL_active;

//extern	int	total_points_counter;

visible_poly *input_points;
_2DPoly *view_points;
float pz,pdiv,ooz;
int i;
int center_x,center_y;
int error_flag;
float fcenter_x, fcenter_y;

extern	int number_of_polys_displayed;

//if (OGL_active==1)
//{
//fcenter_x=0; fcenter_y=0;
//pdiv=depth_of_field;
//error_flag=0;
//for (i=0; i<number_of_polys_displayed; i++)	  //each poly made up of three points
// {
// 
// (*view_points)._2DPoly_ptr=(*input_points).poly_ptr;	//copy the poly ptr for rendering info
//
// pz=(*input_points).point1.z;
//#if Z_DEBUG
//if (pz<camera_pos.z) error_flag=1;	  //DebugStr(" Z behind camera - p1");
//#endif
// ooz=1.0/pz;
// (*view_points).x1=center_x+((*input_points).point1.x*pdiv*ooz);
// (*view_points).y1=center_y-((*input_points).point1.y*pdiv*ooz);
//
// pz=(*input_points).point2.z;
//#if Z_DEBUG
//if (pz<camera_pos.z) error_flag=1;	  //DebugStr(" Z behind camera - p2");
//#endif
// ooz=1.0/pz;
//
// (*view_points).x2=center_x+((*input_points).point2.x*pdiv*ooz);
// (*view_points).y2=center_y-((*input_points).point2.y*pdiv*ooz);
//
// pz=(*input_points).point3.z;
//#if Z_DEBUG
//if (pz<camera_pos.z) error_flag=1;	  //DebugStr(" Z behind camera - p3");
//#endif
// ooz=1.0/pz;
//
// (*view_points).x3=center_x+((*input_points).point3.x*pdiv*ooz);
// (*view_points).y3=center_y-((*input_points).point3.y*pdiv*ooz);
//
// if (error_flag==1) 
//  {
//  //set all points to zero
// // DebugStr("behind camera!");
//  (*view_points).x1=(*view_points).y1=0;
//  (*view_points).x2=(*view_points).y2=0;
//  (*view_points).x3=(*view_points).y3=0;
//  error_flag=0;
// }
// view_points++;
// 
// input_points++;
// }
//}
//else	  //ogl not active
//{
//еееееееееееееее
center_x=width/2;
center_y=height/2;

input_points = visible_points_list_base;
view_points=view_points_list_ptr;
//               Xt=Screen width/2+Trunc((X*256/Z));
//               Yt=screen height/2+Trunc((Y*256/Z));


//oldmac//#define Z_DEBUG 0
if (Mode320==1) pdiv=depth_of_field/2;
else pdiv=depth_of_field;

error_flag=0;

for (i=0; i<number_of_polys_displayed; i++)	//each poly made up of three points
 {
 
 (*view_points)._2DPoly_ptr=(*input_points).poly_ptr;	//copy the poly ptr for rendering info

 pz=(*input_points).point1.z;
#if Z_DEBUG
if (pz<camera_pos.z-1000) error_flag=1;	//DebugStr(" Z behind camera - p1");
#endif
 ooz=1.0/pz;
 (*view_points).x1=static_cast<int>(center_x+((*input_points).point1.x*pdiv*ooz));
 (*view_points).y1=static_cast<int>(center_y-((*input_points).point1.y*pdiv*ooz));

 pz=(*input_points).point2.z;
#if Z_DEBUG
if (pz<camera_pos.z-1000) error_flag=1;	//DebugStr(" Z behind camera - p2");
#endif
 ooz=1.0/pz;

 (*view_points).x2=static_cast<int>(center_x+((*input_points).point2.x*pdiv*ooz));
 (*view_points).y2=static_cast<int>(center_y-((*input_points).point2.y*pdiv*ooz));

 pz=(*input_points).point3.z;
#if Z_DEBUG
if (pz<camera_pos.z-1000) error_flag=1;	//DebugStr(" Z behind camera - p3");
#endif
 ooz=1.0/pz;

 (*view_points).x3=static_cast<int>(center_x+((*input_points).point3.x*pdiv*ooz));
 (*view_points).y3=static_cast<int>(center_y-((*input_points).point3.y*pdiv*ooz));

 if (error_flag==1) 
 {
 //set all points to zero
// DebugStr("behind camera!");
 (*view_points).x1=(*view_points).y1=0;
 (*view_points).x2=(*view_points).y2=0;
 (*view_points).x3=(*view_points).y3=0;
 error_flag=0;
 }
 view_points++;
 
 input_points++;
 }
//}	//ogl not active
}

/*
//еееееееVERY OLD ROUTINE
void do_pers() 
{
extern _2DPoly *view_points_list_ptr;
extern _3D *world_points_list_ptr;
extern	int	total_points_counter;

_3D *input_points;
_2D *view_points;
float px,py,pz;
int i=0;
int Xt,Yt;

//еееееееееееееее

input_points = world_points_list_ptr;
view_points=view_points_list_ptr;
//               Xt=Screen width/2+Trunc((X*256/Z));
//               Yt=screen height/2+Trunc((Y*256/Z));

for (i=0; i<total_points_counter; i++)
 {
 px=(*input_points).x;
 py=(*input_points).y;
 pz=(*input_points++).z;

 Xt=320+(px*256/pz);
 Yt=240-(py*256/pz);
 (*view_points).x=Xt;
 (*view_points++).y=Yt;
 }

}
 */

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//calculate face normal as
//               Xn:=(Z2-Z1)*(Y3-Y1)-(Y2-Y1)*(Z3-Z1);
//               Yn:=(Z2-Z1)*(X3-X1)-(X2-X1)*(Z3-Z1);
//               Zn:=(Y2-Y1)*(X3-X1)-(X2-X1)*(Y3-Y1);
//               { Now Normalize the normals }
//               Module:=Sqrt(Sqr(Xn)+Sqr(Yn)+Sqr(Zn));
//               Xn:=Xn/Module;
//               Yn:=Yn/Module;
//               Zn:=Zn/Module;
	//calculate a poly normal in static objects
	

void calc_normal(int the_object, int poly_select)
{
extern	StatObjectsFixedSize *ocb_Sptr;
textured_connections *pconnections;
int	conn;	//offset to points for the polys in the object
_3D p1,p2,p3;	//our three points
_3D *world_points;
_3D point_normal;
_ZPoly *the_polys;
_ZPoly *the_poly;
float Module;	//normalising normals
//float test_point;	//used for calcing poly point normals for Gourad.

the_polys=(*ocb_Sptr).object_list[the_object].polys_ptr;	//pointing to static definition
the_poly=the_polys+poly_select;

pconnections=(*the_poly).poly_connections_ptr;	

world_points=(*ocb_Sptr).object_list[the_object].points_ptr;	//the points for this object
conn=(*pconnections).the_node;
if ((*pconnections).type==1) (*pconnections).u=-32767;	//frig for gourad test - remove
    pconnections++;

  p1=(*(world_points+conn));
conn=(*pconnections).the_node;
if ((*pconnections).type==1) (*pconnections).u=0;	//frig for gourad test - remove
    pconnections++;
  p2=(*(world_points+conn));
conn=(*pconnections).the_node;	//e.g. 1 to 2, 2 to 3
if ((*pconnections).type==1) (*pconnections).u=32767;
    pconnections++;
  p3=(*(world_points+conn));
  
//get the normals
point_normal.x=(p2.z-p1.z)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.z-p1.z);
point_normal.y=(p2.z-p1.z)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.z-p1.z);
point_normal.z=(p2.y-p1.y)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.y-p1.y);

//point_normal.x=(p3.z-p1.z)*(p2.y-p1.y)-(p3.y-p1.y)*(p2.z-p1.z);
//point_normal.y=(p3.z-p1.z)*(p2.x-p1.x)-(p3.x-p1.x)*(p2.z-p1.z);
//point_normal.z=(p3.y-p1.y)*(p2.x-p1.x)-(p3.x-p1.x)*(p2.y-p1.y);

//normalize the normals
Module=sqrt(point_normal.x*point_normal.x+point_normal.y*point_normal.y+point_normal.z*point_normal.z);
//store normal vector (poly 0,0,0 to x,y,z where 1.0=max)
(*the_poly).face_normal.x=point_normal.x/Module;	//this is working in the static def
(*the_poly).face_normal.y=-point_normal.y/Module;
(*the_poly).face_normal.z=point_normal.z/Module;	//TaaDaah!


}



// get a random point from an object, simply as a place holder
_2D get_any_screen_coord(int object_slot)
{
_3D *this_object_points;
float x,y,z;

     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the points for this object
	 if (this_object_points==0)
	 {
	 _2D error_coord={0,0};
		return error_coord;
	 }
     x=(*this_object_points).x;
     y=(*this_object_points).y;
     z=(*this_object_points).z;
     
   if (show_control_panel==0)
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,CONTROL_PANEL_TOP/2);
     else
     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
   }
   else
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,240);
     else
     return point_to_screen(x,y,z,640,480);
   }

	
}




//only call during game as it assumes a fixed window size
//returns the 2d point of the center of the object
_2D get_screen_center_coord(int object_slot)
{
extern int Mode320;

//_ZPoly *the_polys;
//textured_connections *pconnections;
_3D *this_object_points;
int i,number_of_points;
float x,y,z;

//find the center of the object
     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the points for this object
     x=y=z=0;
     number_of_points=(*ocb_ptr).object_list[object_slot].number_of_points;
     for (i=0;i<number_of_points;i++)
     {
       x+=(*this_object_points).x;
       y+=(*this_object_points).y;
       z+=(*this_object_points).z;
       this_object_points++;
     }
     x/=number_of_points;
     y/=number_of_points;
     z/=number_of_points;
     
   if (show_control_panel==0)
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,CONTROL_PANEL_TOP/2);
     else
     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
   }
   else
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,240);
     else
     return point_to_screen(x,y,z,640,480);
   }

}


//only call during game as it assumes a fixed window size
//returns the 2d point of the FRONT of the object
_2D get_screen_front_coord(int object_slot)
{
//_ZPoly *the_polys;
//textured_connections *pconnections;
_3D *this_object_points;
_3D *this_object_original_points;
int point_index=0;	//which point has greatest x extent
int i,number_of_points;
float x,y,z;

//find the front of the object
///find the rightmost point in the non-rotated points, making a note of it;s point number
//and then persing that point out of the world points

     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the world points for this object
     this_object_original_points=(*ocb_ptr).object_list[object_slot].points_ptr;	//the original points
     x=-500000;
     number_of_points=(*ocb_ptr).object_list[object_slot].number_of_points;
//find rightmost point
     for (i=0;i<number_of_points;i++)
     {
       if ((*this_object_original_points).x>x)
       {
        x=(*this_object_original_points).x; 
        point_index=i;
       }
       this_object_original_points++;
     }
     
     
     x=this_object_points[point_index].x;
     y=this_object_points[point_index].y;
     z=this_object_points[point_index].z;
   if (show_control_panel==0)
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,CONTROL_PANEL_TOP/2);
     else
     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
   }
   else
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,240);
     else
     return point_to_screen(x,y,z,640,480);
   }

   
//     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
}

//only call during game as it assumes a fixed window size
//returns the 2d point of the FRONT of the object
_2D get_screen_rear_coord(int object_slot)
{
//_ZPoly *the_polys;
//textured_connections *pconnections;
_3D *this_object_points;
_3D *this_object_original_points;
int point_index=0;	//which point has greatest x extent
int i,number_of_points;
float x,y,z;

//find the front of the object
///find the rightmost point in the non-rotated points, making a note of it;s point number
//and then persing that point out of the world points

     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the world points for this object
     this_object_original_points=(*ocb_ptr).object_list[object_slot].points_ptr;	//the original points
     x=500000;
     number_of_points=(*ocb_ptr).object_list[object_slot].number_of_points;
//find rightmost point
     for (i=0;i<number_of_points;i++)
     {
       if ((*this_object_original_points).x<x)
       {
        x=(*this_object_original_points).x; 
        point_index=i;
       }
       this_object_original_points++;
     }
     
     
     x=this_object_points[point_index].x;
     y=this_object_points[point_index].y;
     z=this_object_points[point_index].z;
 
   if (show_control_panel==0)
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,CONTROL_PANEL_TOP/2);
     else
     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
   }
   else
   {
     if (Mode320==1)
     return point_to_screen(x,y,z,320,240);
     else
     return point_to_screen(x,y,z,640,480);
   }
   
//     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
}


//convert a 3d point to screen coords given screen width and height
_2D point_to_screen(float x, float y, float z, int width, int height)
{
_2D return_val;
float pdiv;
int center_x,center_y;


if (z<camera_pos.z)
{
return_val.x=-1; return_val.y=-1;
return return_val;
}
else
{
 center_x=width>>1;
 center_y=height>>1;
 
 if (Mode320==1) pdiv=128;
 else pdiv=256;
 //{
  return_val.x=static_cast<int>(center_x+(x*pdiv/z));
  return_val.y=static_cast<int>(center_y-(y*pdiv/z));
 //}
 //else
 //{
 // return_val.x=center_x+(x*256/z);
 // return_val.y=center_y-(y*256/z);
 //}
 return return_val;
}
}




