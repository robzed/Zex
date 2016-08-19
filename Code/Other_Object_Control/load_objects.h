//load_objects.h
int load_objects();
void load_dyn_object(int static_slot,int dest_ocb,load_dyn_object_pb *the_pb, int mothership_galactic_slot);
void replace_dyn_object(int static_slot,int dest_ocb);
int find_vacant_dynamic_object_slot();	//returns an index into the dynamic_objects_array
int find_vacant_dyn_poly_slot();	//returns an index into the dynamic_polys_array
void kill(int the_object);
void reset_objects();
int load_object(const char* filename, int static_slot, unsigned long id,int colldetect, unsigned long texture_id, int translucent);
void load_animated_object(anim_pb* the_objects, int number_of_objects, int frame_rate, int control,
     int state, load_dyn_object_pb *the_pb, short galactic_id);     
void gen_dust();

void gen_dust_for_end();	//gen a star in front of camera
void gen_dust_for_warphole();
void populate_dust_for_start();
void kill_all_dust();



