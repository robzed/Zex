#define GALAXY_WIDTH NUMBER_OF_SECTIONS_IN_A_QUAD_X * 2 * SECTION_SIZE
#define GALAXY_H NUMBER_OF_SECTIONS_IN_A_QUAD_Y * 2 * SECTION_SIZE

//galactic scenary etc.
#define ASTEROID_FIELD1 0X00060001	//6 ASTEROIDS
#define ASTEROID_FIELD2 0x000C0001	//12 asteroids
#define COWS1 0x00020002	//3 space cows
#define COWS2 0x00060002	//10 space cows
#define CORRAL_FACING_RIGHT 0x00000003	//right facing corral
 
typedef struct {
int number_of_earth_mships;
int total_earth_supply_rating;
int av_earth_supply_rating;
int total_earth_power_rating;
int av_earth_power_rating;

int number_of_alien_mships;
int total_alien_supply_rating;
int av_alien_supply_rating;
int total_alien_power_rating;
int av_alien_power_rating;
} galactic_stats;


void make_new_galaxy();
int get_supply_rating(int mothership_index);
void handle_galactic_map();
int get_supply_rating_docked(int ocb_slot);

void get_galactic_stats(galactic_stats* the_stats);
int get_current_warphole_toll();

void set_all_motherships_to_alegiance(int allegiance, int quadrant);
