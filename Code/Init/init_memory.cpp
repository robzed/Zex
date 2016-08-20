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

//init_ocbs

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//#include <stdlib.h>			// for malloc
#include <new>				// for new to replace malloc
using namespace std;

#include <string.h>
#include "object_control_top.h"

#include "3d_structures_def.h"
#include "3d_top_int.h"
#include "3d_top.h"
#include "Error_handler.h"
#include "init_memory.h"
#include "fcircle.h"

#include "memory_debug.h"

int init_ocbs()
{
//galactic_map_handle thegmHdl;
galactic_map_ptr thegmPtr;

//DynObjectsFixedSizeHandle theHdl;
DynObjectsFixedSizePtr thePtr;
//StatObjectsFixedSizeHandle theSHdl;
StatObjectsFixedSizePtr theSPtr;

//short count;
//extern	int *the_ocbs
//	Handle temp_h;
//oldmac//	void *temp_ptr;
	int return_val=0;
	//int temp_val;
	int i;
	extern	DynObjectsFixedSize *ocb_ptr;
	extern	StatObjectsFixedSize *ocb_Sptr;
	extern  galactic_map *GM_ptr;

	//extern	_3D *pointslist_ptr; //current index
	extern  _3D *points_list_ptr_reset;
	//extern	_3D *visible_pointslist_ptr; //current index

	extern	_3D *points_list_ptr_base;	//they start here
	//extern	textured_connections *connectionlist_ptr;
	//extern	_ZPoly *poly_list_ptr;
	extern	_ZPoly *Spoly_list_ptr;
	extern	_ZPoly *Spoly_list_ptr_reset;
	extern  _ZPoly * Spoly_list_top;	
	extern	_ZPoly_objs *dynamic_polys_ptr;

//	extern	Polygon **Polygon_handle;
	extern	Polygon *Polygon_ptr;

	extern	textured_connections* connection_list_top;
	extern  int *points_offsets_ptr; //array of ints specifying offset to points for object.


extern int points_list_top;
extern int view_points_list_top;
extern int world_points_list_top;
extern int dynamic_object_top;
//extern int poly_list_top;
//extern int Spoly_list_top;
extern int points_offsets_top;	
extern int dynamic_object_index;
extern int static_object_index;
extern textured_connections *connection_list_ptr;	//global connection list ptr
extern textured_connections *connection_list_ptr_reset;	//global connection list ptr
//int size_of_GM;

_ZPoly_objs *temp_dynamic_polys_ptr;


New_MemoryDebug* no_use_for_this_variable1 = new New_MemoryDebug(sizeof(StatObjectsFixedSize));

//memory for static objects
//	theSHdl = (StatObjectsFixedSizeHandle) NewHandleClear(sizeof(StatObjectsFixedSize));
//rob	theSPtr = malloc(sizeof(StatObjectsFixedSize));
	theSPtr = new(nothrow) StatObjectsFixedSize;

//	if (theSHdl!=0)
	if (theSPtr!=0)
		{ 
		memset(theSPtr, 0, sizeof(StatObjectsFixedSize));		// write zero to all bytes requested
		//LS_HLock((Handle)theSHdl);
		//theSPtr=*theSHdl;
		ocb_Sptr =theSPtr;
		dynamic_object_top =MAX_NUMBER_OF_STAT_OBJECTS;
		}
	else
		report_error_c("Not enough memory (init_ocbs: StatObjectFixedSize).","",3);

	
	New_MemoryDebug* no_use_for_this_variable2 = new New_MemoryDebug(sizeof(DynObjectsFixedSize));
	
//memory for dynamic objects
//	theHdl = (DynObjectsFixedSizeHandle) NewHandleClear(sizeof(DynObjectsFixedSize));
//rob	thePtr = malloc(sizeof(DynObjectsFixedSize));
	thePtr = new(nothrow) DynObjectsFixedSize;
//	if (theHdl!=0)
	if (thePtr!=0)
		{ 
		memset(thePtr, 0, sizeof(DynObjectsFixedSize));		// write zero to all bytes requested

//		LS_HLock((Handle)theHdl);
//		thePtr=*theHdl;
		ocb_ptr =thePtr;
		dynamic_object_top =MAX_NUMBER_OF_DYN_OBJECTS;
		for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
		{
		(*ocb_ptr).object_list[i].in_use=0;	//all not in use
		}
		}
	else
		report_error_c("Not enough memory (init_ocbs: DynObjectFixedSize).","",3);

//memory for galactic map
//	size_of_GM=sizeof(galactic_map);
//	thegmHdl = (galactic_map_handle) NewHandleClear(size_of_GM);
//rob	thegmPtr = malloc(size_of_GM);

	thegmPtr = new(nothrow) galactic_map;
//	if (thegmHdl!=0)
	if (thegmPtr!=0)
		{ 
		memset(thegmPtr, 0, sizeof(galactic_map));		// write zero to all bytes requested

//		LS_HLock((Handle)thegmHdl);
//		thegmPtr=*thegmHdl;
	        GM_ptr =thegmPtr;
		}
	else
		report_error_c("Not enough memory (init_memory: galactic_map).","",3);

//memory for cmap
//	temp_h=NewHandleClear(sizeof(cmap_objects));
//	if (temp_h!=0)
//		{ 
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
//		cmap_ptr_base =temp_ptr;		
//		}
//	else
	cmap_ptr_base = new(nothrow) cmap_objects;
	if(! cmap_ptr_base)
	{
		report_error_c("Not enough memory (init_ocbs: cmap).","",3);
	}
	else
	{
		memset(cmap_ptr_base, 0, sizeof(cmap_objects));		// write zero to all bytes requested
	}

	init_circle();			// do circle memory

//memory for points
//	temp_h=(Handle)NewHandleClear(300000);
//	if (temp_h!=0)
	points_list_top=25000;
	points_list_ptr = new(nothrow) _3D[points_list_top+1];		// +1 just in case
	if (points_list_ptr!=0)
		{ 
		memset(points_list_ptr, 0, sizeof(_3D[points_list_top+1]));		// write zero to all bytes requested
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
		//points_list_ptr =temp_ptr;
		points_list_ptr_reset=points_list_ptr;	//so we can reset it
//		points_list_top=300000/sizeof(_3D);
		}
	else
		{
		report_error_c("Not enough memory (init_ocbs: points_list).","",3);
		}

//memory for a polygon - for sys poly routine
//	temp_h=(Handle)NewHandleClear(100);
//	if (temp_h!=0)
//	temp_ptr=malloc(100);
	Polygon_ptr = new(nothrow) Polygon[8];		//	100 / 14 (size of Polygon) = 7.14 points. We probably use trimeshes only?
//		{ 
//		Polygon_handle=(Polygon**)temp_h;	// can't use this
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
//		Polygon_ptr =temp_ptr;
//		}
//	else
	if(! Polygon_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: Polygon).","",3);
	}
	else
	{
		memset(Polygon_ptr, 0, sizeof(Polygon[8]));		// write zero to all bytes requested
	}

//memory for VISIBLE points
//	temp_h=(Handle)NewHandleClear(300000);
//	if (temp_h!=0)
//	temp_ptr=malloc(300000);
//	if (temp_ptr!=0)
//		{ 
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
//		visible_points_list_ptr =temp_ptr;
//		visible_points_list_top=300000/sizeof(_3D);
//		}
//	else
	visible_points_list_top=25000;				// there was a bug in the original code - top calc vs. ptr type.
	visible_points_list_ptr = new(nothrow) visible_poly[visible_points_list_top+1];		// +1 just in case
	if(! visible_points_list_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: visible_points_list).","",3);
	}
	else
	{
		memset(visible_points_list_ptr, 0, sizeof(visible_poly[visible_points_list_top+1]));		// write zero to all bytes requested
	}


//memory for vieww points
//	temp_h=(Handle)NewHandleClear(300000);
//	if (temp_h!=0)
//	temp_ptr=malloc(300000);
//	if (temp_ptr!=0)
//		{ 
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
//		view_points_list_ptr =temp_ptr;
//		view_points_list_top=300000/sizeof(_2D);
//		}
//	else
	view_points_list_top=25000;				// there was a bug in the original code - top calc vs. ptr type.
	view_points_list_ptr = new(nothrow) _2DPoly[view_points_list_top+1];		// +1 just in case
	if(! view_points_list_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: view_points).","",3);
	}
	else
	{
		memset(view_points_list_ptr, 0, sizeof(_2DPoly[view_points_list_top+1]));		// write zero to all bytes requested
	}
	
//memory for points_offsets
//	temp_h=(Handle)NewHandleClear(300000);
//	if (temp_h!=0)
//	temp_ptr=malloc(300000);
//	if (temp_ptr!=0)
//		{ 
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
//		points_offsets_ptr =temp_ptr;
//		points_offsets_top=300000/sizeof(int);
//		}
//	else
	points_offsets_top=75000;
	points_offsets_ptr = new(nothrow) int[points_offsets_top+1];		// +1 just in case
	if(! points_offsets_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: points_offsets).","",3);
	}
	else
	{
		memset(points_offsets_ptr, 0, sizeof(int[points_offsets_top+1]));		// write zero to all bytes requested
	}
	
//memory for world points
//	temp_h=(Handle)NewHandleClear(300000);
//	if (temp_h!=0)
//	temp_ptr=malloc(300000);
//	if (temp_ptr!=0)
//		{ 
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
//		world_points_list_ptr =temp_ptr;
//		world_points_list_top=300000/sizeof(_3D);
//		}
//	else
	world_points_list_top=25000;
	world_points_list_ptr = new(nothrow) _3D[world_points_list_top+1];		// +1 just in case
	if(! world_points_list_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: world_points_list).","",3);
	}
	else
	{
		memset(world_points_list_ptr, 0, sizeof(_3D[world_points_list_top+1]));		// write zero to all bytes requested
	}
	

//memory for connections
//	temp_h=(Handle)NewHandleClear(300000);
//	if (temp_h!=0)
//	temp_ptr=malloc(300000);
//	if (temp_ptr!=0)
//		{ 
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
//		connection_list_ptr =temp_ptr;
//		connection_list_ptr_reset=connection_list_ptr;
//		connection_list_top=connection_list_ptr+(300000/sizeof(textured_connections));
//		}
//	else

	int connection_list_num=37500;
	connection_list_ptr = new(nothrow) textured_connections[connection_list_num+1];		// +1 just in case
	if(! connection_list_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: connection_list).","",3);
	}
	else
	{
		memset(connection_list_ptr, 0, sizeof(textured_connections[connection_list_num+1]));		// write zero to all bytes requested
	}
	connection_list_ptr_reset = connection_list_ptr;
	connection_list_top	= connection_list_ptr+connection_list_num;

//memory for static polys
//	temp_h=(Handle)NewHandleClear(sizeof(_ZPoly)*MAX_NUMBER_OF_POLYS);
//	if (temp_h!=0)
	//temp_ptr=malloc(sizeof(_ZPoly)*MAX_NUMBER_OF_POLYS);
	//if (temp_ptr!=0)
	//	{ 
	//	int temp;
		
//		LS_HLock(temp_h);
//		temp_ptr=*temp_h;
	//	Spoly_list_ptr =temp_ptr;
	//	Spoly_list_ptr_reset=Spoly_list_ptr;
		
	//	Spoly_list_top=Spoly_list_ptr+MAX_NUMBER_OF_POLYS;
		
	//	temp=Spoly_list_top-Spoly_list_ptr;
	//	}
	//else
	//	report_error_c("Not enough memory (init_ocbs: poly_list).","",3);

	Spoly_list_ptr = new(nothrow) _ZPoly[MAX_NUMBER_OF_POLYS+1];		// +1 just in case
	if(! Spoly_list_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: poly_list).","",3);
	}
	else
	{
		memset(Spoly_list_ptr, 0, sizeof(_ZPoly[MAX_NUMBER_OF_POLYS+1]));		// write zero to all bytes requested
	}
	Spoly_list_ptr_reset = Spoly_list_ptr;
	Spoly_list_top	= Spoly_list_ptr+MAX_NUMBER_OF_POLYS;
	
	//memory for dynamic polys
	//	temp_h=(Handle)NewHandleClear(sizeof(_ZPoly_objs));
	//	if (temp_h!=0)
	//temp_ptr=malloc(sizeof(_ZPoly_objs));
	//if (temp_ptr!=0)
	//{ 
	//		LS_HLock(temp_h);
	//		temp_ptr=*temp_h;
	//	dynamic_polys_ptr =temp_ptr;
	//set em all empty
	//	temp_dynamic_polys_ptr=dynamic_polys_ptr;
	
	//	for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
	//	{
	//	(*temp_dynamic_polys_ptr)[i].in_use=0;
	//	}
	//	}
	//else
	//	report_error_c("Not enough memory (init_ocbs: dynamic_polys).","",3);
	
	dynamic_polys_ptr = new(nothrow) _ZPoly_objs[MAX_NUMBER_OF_DYN_OBJECTS];
	if(! dynamic_polys_ptr)
	{
		report_error_c("Not enough memory (init_ocbs: dynamic_polys).","",3);
	}
	else
	{
		memset(dynamic_polys_ptr, 0, sizeof(_ZPoly_objs[MAX_NUMBER_OF_DYN_OBJECTS]));		// write zero to all bytes requested
	}
	temp_dynamic_polys_ptr = dynamic_polys_ptr;
	for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
	{
		//(*temp_dynamic_polys_ptr)[i].in_use=0;
		temp_dynamic_polys_ptr[i].in_use=0;
	}
	

points_list_ptr_base=points_list_ptr;
visible_points_list_base=visible_points_list_ptr;
dynamic_object_index=0;
static_object_index=0;

// fill the callsigns with known start data
#if CALLSIGN_DEBUG_MODE
robs_callsign_hack();
#endif

/*
insert_test_object(0,-90,-30,80,2,2,1);
insert_test_object(1,-30,30,120,1,1,2);
insert_test_object(2,30,-30,160,2,0,3);
insert_test_object(3,90,30,200,2,3,4);
insert_test_object(4,0,130,40,-1,-1,-1);
insert_test_object(5,0,-130,40,0,0,-4);

insert_test_object(6,-150,120,40,-1,-1,-1);
insert_test_object(7,150,-120,40,0,0,-4);

insert_test_object(8,150,-20,40,-3,-1,2);
insert_test_object(9,-150,20,40,0,5,0);

insert_test_object(10,150,120,40,-3,-1,2);
insert_test_object(11,-150,-120,40,0,5,0);

insert_test_object(12,100,180,140,0,0,-2);
insert_test_object(13,-100,-220,340,-1,-1,0);

insert_test_object(14,100,-220,240,4,1,-2);
insert_test_object(15,-150,220,240,3,3,1);
 */
return return_val;
}




