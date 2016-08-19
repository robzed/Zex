//3d_top.h

#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif
void do_3d ();
void do_pers2(int width, int height);
void calc_normal(int the_object, int poly_select);
void do_culling2();
void do_zsort();
void calc_normal(int the_object, int poly_select);
void insert_to_z_buffer(_ZPoly *the_poly);
void reset_zsort_insert();
void do_gourad_polys();
_2D get_screen_center_coord(int object_slot);
_2D get_screen_front_coord(int object_slot);
_2D get_screen_rear_coord(int object_slot);
_2D point_to_screen(float x, float y, float z, int width, int h);
_2D get_any_screen_coord(int object_slot);

