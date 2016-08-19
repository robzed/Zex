//prototypes
Handle read_file (FSSpec the_file);
void get_zex_fsspec();
void read_prefs();
void write_prefs();

typedef struct {
int score;
char name[63];
} hiscore;

typedef struct {
int Change_res;
int Use_VBL;
int Use_SpeedLimit;
int Use_Mode320;
int Use_Interlace;
int sound_volume;
int music_volume;
int registered;
int version;

int camera_zoom;
int camera_mode;

int ticks_at_installation_time;
int random_number_at_installation_time;	//from ticks - show to user for registration
int key_from_random_number_at_installation_time;	//user sends this to get a registration ky
UInt32 registration_key;

hiscore the_scores[10];
} prefs;



void append_save_game_data(FILE* file_ptr);
int load_game();
int load_core_game_data(FILE* file_ptr);
OSErr WriteZexFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen );
OSErr ReadZexFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen );

// two version of does_file_exist. Static polymophism determines which to call.
bool does_file_exist(const char *filename, const char *mode);
bool does_file_exist(FSSpec* the_fsspec);

#include <string>
#include <fstream>
// lightsoft file version of getline (supports any line endings)
void lsf_getline(std::string& output_string, FILE *file);
void lsf_getline(std::string& output_string, std::ifstream& file);

int return_save_version();
