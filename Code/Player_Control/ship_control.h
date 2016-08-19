// ship_control.h
void apply_control_inputs();
//oldmac//short check_key(unsigned short k);
void ship_init();
void do_player_fire();
void reset_ship();
void reset_ship_with_carry_over();
//oldmac//short fire_button();
void do_player_laser();
void do_player_missile();
void kill_zex();


int get_pylon();
void select_next_pylon();
void reset_screen_from_status();
void clear_down_ship_on_eject();
void do_ejection();
void do_player_rear_laser();

void end_game_right_here_right_now(int EOLCode, const char * the_reason_string);

void do_shockwave(int modifier);
void do_warp_jump(int modifier);
void handle_ecm(unsigned int for_whom);
void run_missile_factory(void);

void damage_object_due_to_shockwave(int distance, int damage);
