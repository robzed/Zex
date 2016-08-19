// object_control_top.h
void do_object_control();
void handle_weeny_bullet(int it);
void handle_dust(int it);
void handle_explode(int it);
void handle_monster1(int it);
void handle_not_so_weeny_bullet(int it);
void handle_ejection(int it);
void check_collision(int current_object);
void kill_kill_list();
void reset_kill_list();
void add_to_kill_list (int current_object);
void do_light_control();
void handle_laser_weeny (int it, int rear);
void handle_gun1(int it);
void handle_asteroid1(int it);
void handle_persistent_asteroid(int it);
void handle_burning_particle(int it);
void handle_missile1(int it);
void handle_space_cow(int it);
void handle_earth_mothership(int it);
void handle_planet_roty(int it);	//rotate slowly every control_1 frames
void handle_planet_rotz(int it);	//rotate slowly every control_1 frames
void do_mothership_hit(int object_slot, int mothership_slot);
void handle_static_laser1(int it);	//static laser cointroller


// callsign debug code
void write_callsigns();
void parse_ocb_for_bad_callsigns();
void robs_callsign_hack();



