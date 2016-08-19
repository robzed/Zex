// Planet_Generator.h
void gen_planet_name(unsigned int seed, unsigned char * output_string);

void planet_gen(int seed, int is_inhabited, unsigned char *scratch_address, unsigned char *output_address);
//void galaxy_gen(int seed, char *output_address);
void galaxy_gen(int seed, unsigned char *output_address, int squish_flag);
void init_gen_planet_name(void);


