#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif

//3d_top.int
extern float global_matrix[4][4];
extern DynObjectsFixedSize *ocb_ptr;
extern StatObjectsFixedSize *ocb_Sptr;
extern cmap_objects *cmap_ptr_base;

extern _3D *points_list_ptr;
extern visible_poly *visible_points_list_ptr;
extern _3D *points_list_ptr_base;
extern _3D *points_list_ptr_reset;	//reset value for points_list_pointer
extern visible_poly *visible_points_list_base;
extern _3D *world_points_list_ptr;
extern _2DPoly *view_points_list_ptr;
extern _3D light_normal;	//the light vecterre
extern _3D camera_pos;	//+z inwards remember
extern _3D camera_pos_delta;	//change to enable auto camera movement - dont do it anymore!

extern int camera_rotation_z;	//obviously the camera rot
extern int camera_rotation_x;
extern int camera_rotation_y;
extern int camera_rotation_delta_z; //set this to !0 and caamera rots around z
extern textured_connections fixed_triag_connection [3]; //used to create polys in explode
extern _3D fixed_triag_points[3];

//Polygon **Polygon_handle;	- can't use this 
extern Polygon *Polygon_ptr;	//these two are temp for drawing with system poly routine

extern textured_connections *connection_list_ptr;	 //global connection list ptr
extern textured_connections *connection_list_ptr_reset;	//global connection list ptr
extern textured_connections* connection_list_top;

extern _ZPoly *Spoly_list_ptr;	//static objects holding area, compiled from Mesh input
extern _ZPoly *Spoly_list_ptr_reset;
extern _ZPoly * Spoly_list_top;

extern _ZPoly *poly_list_ptr;	//dynamic working area
extern _ZPoly_objs *dynamic_polys_ptr;
extern int *points_offsets_ptr; //array of ints specifying offset to points for object.

//index into various storages
extern int connection_list_index;
extern int points_list_index;
extern int world_points_list_index;
extern int dynamic_object_index;
extern int poly_list_index;
extern int freeze;	//if 1 3d does not add deltas
//extent of various storages

extern int points_list_top;
extern int visible_points_list_top;
extern int view_points_list_top;
extern int world_points_list_top;
extern int dynamic_object_top;
extern int poly_list_top;
//int Spoly_list_top;

extern int points_offsets_top;
extern int	total_points_counter;
#define MAX_NUMBER_OF_VISIBLE_POLYS 3000	//on test run logged max of 688
//ptrs to polys for z sort
extern _ZPolySort zsort_list[MAX_NUMBER_OF_VISIBLE_POLYS];	//max of MAX_NUMBER_OF_VISIBLE_POLYS polys in system at any on time
extern int zsort_index; //should not really be here
extern int zsort_start_index,zsort_count,zbuffer_next_free;
extern float last_z;


