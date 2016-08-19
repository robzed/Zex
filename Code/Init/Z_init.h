//Z_init.h
//public defs for init

//protos
int init_mac(void);
int init_game(void);
int	reinit_game(int the_level);
void clear_out_dyn();
void reset_static_slots();
void reset_3D();
int	reinit_game_for_next_level(int the_level);
void load_misc_graphics();
void load_32bit_graphics();


