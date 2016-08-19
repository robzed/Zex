//object_control_misc_routines.h
int damage_object (int current_object, int shield_damage);
void do_light_control();
void GenZexShield();
void tumble_object(int the_object);
void set_quick_fade();
void clear_quick_fade();

int find_unique_id(int the_id);
void clear_pb(load_dyn_object_pb* the_pb);

void gen_shockwave_object(int distance, int damage, int damage_happens_distance);
