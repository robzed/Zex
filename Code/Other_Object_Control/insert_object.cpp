//insert object
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


#include <string.h>

#include "3d_structures_def.h"
#include "3d_top_int.h"
#include "3d_top.h"
#include "Error_handler.h"
#include "insert_object.h"
#include "texture_management.h"

//еееееееееееееееееееееееееееееееееееееееееееее

//Inserts an object into the static definitions
void 
insert_object(int object_serial,_3D *vertices,textured_connections *connections, int number_of_vertices, int number_of_connections, 
              int pos_x, unsigned long the_id,int is_crashable,float bounds_sphere,int texture_id, int translucent)
{
//set up DynObjectFixedSize
extern Zex_Colour pcol[9];
extern	StatObjectsFixedSize *ocb_Sptr;

extern int	static_object_index;	//how many static objects

extern _3D *points_list_ptr;
extern textured_connections *connection_list_ptr;
extern	_ZPoly *Spoly_list_ptr;
extern	textured_connections* connection_list_top;


_ZPoly *object_poly_list_ptr;
int object_number_of_points;
int object_number_of_polys;
int	ocb_place=0;
int i,material_count,points_count;
int j,k;
float minx,maxx,miny,maxy;

ZObject test;
memset(&test, 0, sizeof(ZObject));

#if CALLSIGN_DEBUG_MODE
test.callsign = 0x3333;
#endif

_ZPoly test_poly = {0,	//connections ptr
                    { 0,0,0},	//face normal
                    { 0,0,0},	//face normal translated
                    { 0x0,0x0,0x0} ,	//color
                    0,	//average z
                    0,	//light value
                    0,	//offset to points for renderer
                    0,	//texture ptr (0=none)
					0, //PID
					{0,0,0,0,0,0},	//Ptouching1
					{0,0,0,0,0,0},	//Ptouching2
					{0,0,0,0,0,0},	//Ptouching3
					0, //p_visible
					0, //draw_code
                    };
                    
textured_connections *oconnections;	//for touching polys test
textured_connections *tconnections;	//same
int touching_polys;
_ZPoly *the_polys;
_ZPoly *our_poly;
_ZPoly *touching_test_poly;

texture_slot* the_texture_slot;
Ptr texture_ptr0 = 0;
unsigned int texture_name=0;

if (texture_id!=0)
 {
 
 the_texture_slot=find_texture_ptr(texture_id);

 if (the_texture_slot==0) report_error_id ("insert_object: Texture not found!",texture_id);
 texture_ptr0=the_texture_slot->the_raw_data;
 texture_name=the_texture_slot->gl_text_name;
 }


object_number_of_polys=number_of_connections/3;	//1 for number of connections and 6 for connections
if (object_number_of_polys>MAX_NUMBER_POLYS_PER_OBJ) report_error_id_number ("Object has too many polys!",the_id, object_number_of_polys);
object_number_of_points=number_of_vertices;
                                
//copy connections to connection_list_ptr
BlockMoveData	(connections,connection_list_ptr,number_of_connections*sizeof(textured_connections));
//copy points to points_list_pt

BlockMoveData	(vertices,points_list_ptr,number_of_vertices*(sizeof(float)*3));
object_poly_list_ptr=Spoly_list_ptr;
//set up ptrs to connections
material_count=0;
points_count=0;

  if (Spoly_list_ptr<Spoly_list_ptr_reset) report_error_id ("insert_object: Static poly ptr too low!!!!",2112);
  if (connection_list_ptr<connection_list_ptr_reset) report_error_id ("insert_object: static connections ptr too low!!!!",2113);


for (i=0;i<object_number_of_polys;i++)
{
//set up first set of colours
//skip unused material blocks
  while (pcol[material_count].points_start==pcol[material_count+1].points_start) material_count++;

  if (pcol[material_count].points_start==points_count)
  {
    test_poly.Prgb=pcol[material_count].colour;
    material_count++;
  }	//set up colour for these polys
  test_poly.Ptexture = texture_ptr0;
  test_poly.Pid = texture_name;	//the OGL "name"
  test_poly.poly_connections_ptr = connection_list_ptr;
  test_poly.draw_code=translucent;	//not translucent
  connection_list_ptr+=3;	//6 ints make up a triag
  if (connection_list_ptr>connection_list_top) report_error_id ("insert_object: Too many static connections!!!!",2114);
  
  BlockMoveData	(&test_poly,Spoly_list_ptr,sizeof(test_poly));
  Spoly_list_ptr+=1;	//another poly in the static list
  if (Spoly_list_ptr>Spoly_list_top) report_error_id ("insert_object: Too many static polys!!!!",2115);
  points_count++;
}
//create the poly normals - this is calculated from three points on the poly


//test_polys[0].face_normal



//copy the polys to polylist_ptr
//(*poly_list_ptr).poly_list[0]=test_polys;
test.showing_shield=0;
test.request_dock=0;
test.eng_invisible=0;
test.Oid=the_id;
test.Orotx=0;
test.Oroty=0;
test.Orotz=0;
test.Orotdx=0;
test.Orotdy=0;
test.Orotdz=0;
test.Omovedx=test.Omovedy=test.Omovedz=0;
test.hit_by_slot=-1;
test.attack_what=-1;	//not attacking anything

//change 050299
//if colldet is not zero then we set crashable and colltype
if (is_crashable!=0)
{
test.crashable=1;
test.colltype=is_crashable;
}
else
{
test.crashable=0;
test.colltype=0;
}
//test.number_owned=0;
test.in_use=1;

test.Oworldx=pos_x;
test.Oworldy=0;
test.Oworldz=0;

//bounds sphere
test.radius=bounds_sphere;

test.controller=0;
test.control_use_1 = test.control_use_2 = test.control_use_3 = test.control_use_4 = test.current_velocity = test.wanted_velocity=0;
//test.Oreference=1;
//test.immediate_child=NULL;
test.number_of_polys=object_number_of_polys;
test.number_of_points=object_number_of_points;
test.points_ptr=points_list_ptr;
test.polys_ptr=object_poly_list_ptr;
//calc bounding box (x and y only)
minx=maxx=miny=maxy=0;

for (i=0;i<object_number_of_points;i++)
	{
	if ((*points_list_ptr).x<minx) minx=(*points_list_ptr).x;
	if ((*points_list_ptr).x>maxx) maxx=(*points_list_ptr).x;
	if ((*points_list_ptr).y<miny) miny=(*points_list_ptr).y;
	if ((*points_list_ptr).y>maxy) maxy=(*points_list_ptr).y;
	points_list_ptr++;
	}

//when creating the box we don't bother with z
if (the_id=='MIS1' || the_id=='MIS2' || the_id=='MIS3')
{
 //make box a little bigger for cannon shells
 minx*=2;
 miny*=2;
 maxx*=2;
 maxy*=2;
} 

test.box_min.x=minx;
test.box_min.y=miny;
test.box_min.z=0;

test.box_max.x=maxx;
test.box_max.y=maxy;
test.box_max.z=0;

//set up polys pointer
(*ocb_Sptr).object_list[object_serial]=test;	//copy into master static object array
//BlockMoveData	(&test,ocb_ptr,sizeof(Zobject);

//ocb_ptr=+dynamic_object_index;	//point to right place for header

//calculate polys normals
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
calc_normal(object_serial,i);	//object,poly

points_list_ptr+=object_number_of_points+10;		//ready for next object (that +10 was causing all kinds of bugs 13/1/99 SB)
//poly_list_ptr+=object_number_of_polys;

static_object_index++;

//d11 - gourad shading
//now calc touching polys for every poly [i] in this object - crunch crunch time.


//do point 1 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<MAX_TOUCHING_POLYS;j++)
 {
  (*our_poly).Ptouching1[j]=-1;
  (*our_poly).Ptouching2[j]=-1;
  (*our_poly).Ptouching3[j]=-1;
 }	//set all touching to not

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      
      
      
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching1[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69; from k	- we have found a point on this poly touching so get out
          }
        tconnections++;	//inc test poly point
       }	//end of k
       
       
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i


//do point 2 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      oconnections++;	//point 2
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching2[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69;	//have to get out of these loops
          }
        tconnections++;	//inc test poly point
       }	//end of k
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i



//do point 3 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      oconnections+=2;	//point 3
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching3[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69;	//have to get out of these loops
          }
        tconnections++;	//inc test poly point
       }	//end of k
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i



}	//end of new_insert_object



