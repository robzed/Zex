//Zex file io
//SB 14/11/98

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <new>
using namespace std;

#include <string.h>		// for memcpy (used in BlockMoveData thingy)
//oldmac//#include <standardfile.h>
//oldmac//#include <files.h>
//oldmac//#include <processes.h>
//oldmac//#include <Resources.h>
#include "file_io.h"
#include "file_io_int.h"
#include "Error_handler.h"
#include "PrefsFile.h"
#include "3d_structures_def.h"
#include	"splat_control.h"
#include "instrum_int.h"
#include "buy_int.h"
#include "camera.h"
#include "docked.h"
#include "zex_misc.h"
#include "map_control.h"

#include "load_level.h"			// for make_data_fsspec()

#include "game_save.h"

#include "Damage.h"


// +-------------------------------+-------------------------+-----------------------
// | TITLE: does_file_exist        | AUTHOR(s): Rob Probin   | DATE STARTED: 11 April 2004
// +
// | DESCRIPTION: Check whether a file exists and can be opened for "mode"
// +----------------------------------------------------------------ROUTINE HEADER----
bool does_file_exist(const char *filename, const char *mode)
{
	FILE *input_f;
	bool file_exists = false;

	input_f = fopen(filename,mode);
	if(input_f==NULL)
	{
		file_exists = false;  // there was a problem opening it - return false.
	}
	else
	{
		fclose(input_f);		// close the file that we opened
		file_exists = true;   // looks ok superficially
	}

	return file_exists;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: does_file_exist            | AUTHOR(s): Rob Probin   | DATE STARTED: 18 May 2005
// +
// | Does the file exist on the drive?
// +----------------------------------------------------------------ROUTINE HEADER----
bool does_file_exist(FSSpec* the_fsspec)
{
	char c_name[256];			// can't be longer than 255 long... it's a Pascal string

	PascalToC(the_fsspec->fullpathname, c_name);

	FILE *f = fopen(c_name, "rb");

	if(!f)
	{
		return false;
	}

	// if we can open it for reading, it must exist (?)
	fclose(f);
	return true;
}

//read file
//needs an fsspec
//returns a handle to the file data

prefs gzex_prefs;
int just_installed=0;	//set to 1 if prefs created

extern int Use_Mode320;
extern int Use_SpeedLimit, Use_VBL, Change_res;
extern int camera_mode;
extern float camera_zoom;
extern int missile_flip;
extern int stop_time,start_time,accumulated_game_time;
extern unsigned long script_timer1;
extern unsigned long script_timer2;
extern unsigned long script_state1;
extern unsigned long script_state2;

#define PREFS_VERSION 23

const char prefs_name[]="ZEX Prefs";

void read_prefs()
{
extern int sound_volume;
extern int music_volume;

#define dCHG_RES 1
#define dUSE_VBL 1
#define dSPD_LIM 1
#define dMOD320 0
#define dSND_VOL 40
#define dMUS_VOL 40

#if FORCE_REGISTRATION
#define dREGISTERED 1
#define dREG_KEY 0x12345678
#else
#define dREGISTERED 0
#define dREG_KEY 0
#endif

prefs reset_prefs = {
    //change res, vbl, speedlimiter, lores,
    dCHG_RES,dUSE_VBL,dSPD_LIM, dMOD320,0,
    // sound volume, music volume, registered, version
    dSND_VOL,dMUS_VOL,dREGISTERED,PREFS_VERSION,
    DEFAULT_CAMERA_ZOOM,0,
    //ticks,random,key from random,regi key
    0,0,0,dREG_KEY,
    { { 1000000,"ANVIL" },
    { 750000,"CLAIRE" },	//HISCORES
    { 500000,"PATCH" },
    { 200000,"FLUFFY" },
    { 100000,"STU" },
    { 50000,"JESS" },
    { 30000,"ROB" },
    { 15000,"CATH" },
    { 5000,"JULIE" },
    { 1000,"IAN" } }
                   };

FSSpec prefs_fsspec;
	short foundVRefNum;
	long foundDirID;
	OSErr err;

	// find the Preferences folder

	//oldmac//err = FindFolder(	kOnSystemDisk,
	//oldmac//			kPreferencesFolderType,
	//oldmac//			kCreateFolder,
	//oldmac//			&foundVRefNum,&foundDirID);
	//oldmac//if (!err)

	// make it into a FSSpec (given the file name)
	//oldmac//err =	FSMakeFSSpec(foundVRefNum, foundDirID, prefs_name, &prefs_fsspec);

	gzex_prefs = reset_prefs;			// make copy into the target structure just in case the read and write all fail

	make_data_fsspec("P", prefs_name, &prefs_fsspec);		// we will need to replace this with some other code at some point.... (after conversion)
	err = !does_file_exist(&prefs_fsspec);
    if (err)
    {
    err=WritePrefsFile(prefs_fsspec,(char*)&reset_prefs,sizeof(gzex_prefs));
    just_installed=1;
    }
    if (!err) err = ReadPrefsFile(prefs_fsspec,(char*)&gzex_prefs,sizeof(gzex_prefs));

//check prefs version and rewrite if wrong version
    if (gzex_prefs.version!=PREFS_VERSION)
    {
     just_installed=1;
     WritePrefsFile(prefs_fsspec,(char*)&reset_prefs,sizeof(gzex_prefs));
     ReadPrefsFile(prefs_fsspec,(char*)&gzex_prefs,sizeof(gzex_prefs));
    }
    sound_volume=gzex_prefs.sound_volume;
    music_volume=gzex_prefs.music_volume;
    Use_Mode320=gzex_prefs.Use_Mode320;
    Use_SpeedLimit=gzex_prefs.Use_SpeedLimit;
    Use_VBL=gzex_prefs.Use_VBL;
    #if OGL==1
    Change_res=1;
    #else
    Change_res=gzex_prefs.Change_res;
    #endif
    camera_zoom=gzex_prefs.camera_zoom;
    camera_mode=gzex_prefs.camera_mode;

}


void write_prefs()
{
FSSpec prefs_fsspec;
	short foundVRefNum;
	long foundDirID;
	OSErr err;
	gzex_prefs.Use_Mode320=Use_Mode320;
	gzex_prefs.Use_SpeedLimit=Use_SpeedLimit;
	gzex_prefs.Use_VBL=Use_VBL;
	gzex_prefs.Change_res=Change_res;
	gzex_prefs.camera_zoom=static_cast<int>(camera_zoom);
	gzex_prefs.camera_mode=camera_mode;
	// find the Preferences folder
	//oldmac//err = FindFolder(	kOnSystemDisk,
	//oldmac//					kPreferencesFolderType,
	//oldmac//					kCreateFolder,
	//oldmac//					&foundVRefNum,&foundDirID);
	//oldmac//if (!err)

	// make it into a FSSpec (given the file name)
	//oldmac//err =	FSMakeFSSpec(foundVRefNum, foundDirID, prefs_name, &prefs_fsspec);

	make_data_fsspec("P", prefs_name, &prefs_fsspec);		// we will need to replace this with some other code at some point.... (after conversion)

    WritePrefsFile(prefs_fsspec,(char*)&gzex_prefs,sizeof(gzex_prefs));
}

Handle read_file (FSSpec the_file)
{
	short refnum;
	//oldmac//char permission=fsRdPerm;
	//OSErr Err;
	//oldmac//long curEOF;
	Handle hData;	//we return this

	char c_name[256];			// can't be longer than 255 long... it's a Pascal string

	PascalToC(the_file.fullpathname, c_name);

	//oldmac//Err=FSpOpenDF(&the_file,permission,&refnum);
	//oldmac//	if (Err)
	//oldmac//		report_error("read_file(FSSpec) had an error on Open.",the_file.name,Err);
	FILE *f = fopen(c_name, "rb");			// read as binary (i.e. no line transation)

	if(!f)
		report_error("read_file(FSSpec) had an error on Open.",the_file.name,0);

	//oldmac//Err=GetEOF(refnum,&curEOF);	//get eof
	//oldmac//	if (Err)
	//oldmac//		report_error("read_file(FSSpec) had an error on GetEOF.",the_file.name,Err);
	unsigned long file_length = ls_get_file_size(f);
	if(file_length == 0)
		report_error("read_file(FSSpec) had an error on get file length.",the_file.name,0);

	hData=NewHandleClear(file_length+4);	//ensure zero terminated file!
	if (hData==0)
		report_error("read_file(FSSpec) had an error: No Memory.",the_file.name,3);

	LS_HLock(hData);

	//oldmac//Err=FSRead(refnum,&curEOF,*hData);
	//oldmac//if (Err)
	//oldmac//	report_error("read_file(FSSpec) had an error on Read.",the_file.name,Err);
	size_t read_size = fread(*hData, sizeof(char), file_length, f);
	if(read_size != file_length)
	{
		report_error("read_file(FSSpec) had an error on Read.",the_file.name,0);
	}

	// check there wasn't an error of some other sort....
	if(ferror(f))
	{
		report_error("read_file(FSSpec) - error returned", the_file.name, 0);
	}

	//oldmac//Err=FSClose(refnum);
	//oldmac//if (Err)
	//oldmac//	report_error("read_file(FSSpec) had an error on Close.",the_file.name,Err);
	if(fclose(f))
	{
		report_error("Error closing resource", the_file.name, 0);
	}

	return hData;
}


//oldmac//short zex_res_file;
//fills Zex_fsspec with the fsspec of Zex
void get_zex_fsspec()
{
//oldmac//ProcessSerialNumber zex_psn;
//oldmac//OSErr Err;
//oldmac//ProcessInfoRec proc_info_rec;

//oldmac//zex_res_file=CurResFile();

//get zex psn
//oldmac//Err=GetFrontProcess (&zex_psn);
//oldmac//proc_info_rec.processAppSpec=&Zex_FSSpec;
//oldmac//proc_info_rec.processName=NULL;
//oldmac//proc_info_rec.processInfoLength = sizeof(ProcessInfoRec);

//oldmac//Err=GetProcessInformation(&zex_psn, &proc_info_rec);	//get zex's fsspec

}

//еееееееееееееееееееееееееееее
OSErr WriteZexFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen )
{
	char c_name[256];			// can't be longer than 255 long... it's a Pascal string
	PascalToC(fileSpec.fullpathname, c_name);
	int error=0;

	FILE *f = fopen(c_name, "wb");			// write as binary (i.e. no line transation)

	if(!f) return 1;

	size_t write_size = fwrite(buffer, sizeof(char), bufferLen, f);
	if(write_size != bufferLen)
	{
		error = 2; //report_error("WriteZexFile() had an error on Write.",fileSpec.name,0);
	}

	// check there wasn't an error of some other sort....
	if(!error && ferror(f)) error = 3;

	if(fclose(f)) error = 4;

	return error;

#if 0 // old stuff
	short fileRefNum;
	OSErr err;
	extern OSType gPrefsCreator;

	// create our prefs file
	err = FSpCreate( &fileSpec, gPrefsCreator, 'GaMe', smSystemScript );
	if (err && err != -48) return err;

	// open it for writing
	err = FSpOpenDF( &fileSpec, fsWrPerm, &fileRefNum);
	if (err) return err;

	// write the data
	err = FSWrite( fileRefNum, &bufferLen, buffer );
	FSClose( fileRefNum );

	return err;
#endif
}


//еееееееееееееееееееееееееееее
OSErr ReadZexFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen )
{
	char c_name[256];			// can't be longer than 255 long... it's a Pascal string
	PascalToC(fileSpec.fullpathname, c_name);
	int error=0;

	FILE *f = fopen(c_name, "rb");			// read as binary (i.e. no line transation)

	if(!f) return 1;
	size_t read_size = fread(buffer, sizeof(char), bufferLen, f);
	if(read_size > bufferLen)	// unlikely
	{
		error = 2; //report_error("ReadZexFile() had an error on Write.",fileSpec.name,0);
	}

	// check there wasn't an error of some other sort....
	if(!error && ferror(f)) error = 3;

	if(fclose(f)) error = 4;

	return error;



#if 0		// old stuff
	short fileRefNum;
	OSErr err;
	extern OSType gPrefsCreator;

	// open it for writing
	err = FSpOpenDF( &fileSpec, fsRdPerm, &fileRefNum);
	if (err) return err;

	// write the data
	err = FSRead( fileRefNum, &bufferLen, buffer );
	FSClose( fileRefNum );

	return err;
#endif
}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//ZEX save/load game
extern int gCheckWatchDog;
extern kit_desc ship_items[ITEMS_COUNT];
extern int score,credits,docked_at;
extern	DynObjectsFixedSize *ocb_ptr;
extern int selected_item_type;
extern int selected_item_index;
extern int selected_item_screen_x;
extern int selected_item_screen_y;
#define SAVE_VERS 26
		//Updated to 26 110406 - added surrender falg and gone_around flag.
		//Updated to 25 080105 - slightly harder, changes in the Galactic map mean new games are probably advisable.
		//Updated to 24 251205 - ship items not initted properly, thus all previous saved games contain incorrect data.


int return_save_version()
{
	return SAVE_VERS;
}

//-----Save Game Structure--------//
typedef struct {
UInt32 platform;	//'MAC ', 'PC  ' etc
UInt32 vers;
UInt32 cstring0; // йLightsoft-Stuart Ball-Robert Probin 1999.";
UInt32 cstring1; // йLightsoft-Stuart Ball-Robert Probin 1999.";
UInt32 cstring2; // йLightsoft-Stuart Ball-Robert Probin 1999.";
//galactic map selection
int selected_item_type;
int selected_item_index;
int selected_item_screen_x;
int selected_item_screen_y;
//general stuff
int score,credits,kills,rank,promotion_points,selected_pylon;
int level;
int hull;
int accumulated_time;
int difficulty;
unsigned long docked_at_mothership_id;
int quad,section;
float target_x,target_y;
int sleep_timer,done_salary;

//experience etc.
int experience, laser_experience, hit_power,player_skill,luck;
int player_rank,total_level;
unsigned long wingman_id_0;
unsigned long wingman_id_1;
unsigned long wingman_id_2;
unsigned long wingman_id_3;
unsigned long wingman_id_4;
unsigned long wingman_id_5;
//Scripting
unsigned long script_timer1;	//gp timer (frames)
unsigned long script_timer2;	//
unsigned long script_state1;	//
unsigned long script_state2;	//

kit_desc save_items[ITEMS_COUNT];

hold_desc hold_items[4];	//max of 4 items in hold

int in_level_save;
int surrendered_flag;	//have RMparians surrendered?
int gone_around;		//has player been right around all four quadrants
char for_level_use	[4000];	//level overlays a structure onto this
UInt32 pad [200];
} ZEX_save_file;

typedef struct
{
ZEX_save_file my_save_file;
galactic_map the_gmap;
} saved_file_structure;

//------------------------------//

extern int surrendered_flag, gone_around_flag;
extern int gRank, kills, promotion_points,sleep_timer,done_salary;
extern float targ_x,targ_y;
extern hold_desc hold_items[MAX_HOLD_ITEMS];
extern galactic_map *GM_ptr;	//ptr to the galactic map

void append_save_game_data(FILE* file_ptr)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength,current_level,difficulty;
extern int docked_at,current_quad;
//ZEX_save_file my_save_file;
saved_file_structure * saved_file_data_ptr;

//Str255 my_prompt="Save game as...";
char my_name[64];	//make save file name here
unsigned char my_pname[64];	//as pascal
//oldmac//StandardFileReply my_reply;
	int i;
	//oldmac//short fileRefNum;


   gCheckWatchDog=0;	//disable the watchdog

GraphicsPaused();
LS_FlushEvents(-1,0);	//flush all
//get memory for saved game. bout 200K at last check
//oldmac//saved_file_data_ptr=(saved_file_structure*) NewPtr(sizeof(saved_file_structure));
saved_file_data_ptr= new(nothrow) saved_file_structure;

if (saved_file_data_ptr==0)
	report_error_id_non_fatal("Unable to save game - not enough memory.", 0); //oldmac// MemError());
else
	{
//we save by simply writing out the structure
saved_file_data_ptr->my_save_file.docked_at_mothership_id=(*ocb_ptr).object_list[docked_at].mship_ptr->callsign;
saved_file_data_ptr->my_save_file.quad=current_quad;
saved_file_data_ptr->my_save_file.section=get_section(current_quad, static_cast<int>((*ocb_ptr).object_list[docked_at].Oworldx), static_cast<int>((*ocb_ptr).object_list[docked_at].Oworldy));
//saved_file_data_ptr->my_save_file.quad
saved_file_data_ptr->my_save_file.sleep_timer=sleep_timer;

saved_file_data_ptr->my_save_file.platform='MAC ';
saved_file_data_ptr->my_save_file.vers=SAVE_VERS;
saved_file_data_ptr->my_save_file.cstring0='Zex ';
saved_file_data_ptr->my_save_file.cstring1='_LSG';
saved_file_data_ptr->my_save_file.cstring2='2005';

saved_file_data_ptr->my_save_file.selected_item_type=selected_item_type;
saved_file_data_ptr->my_save_file.selected_item_index=selected_item_index;
saved_file_data_ptr->my_save_file.selected_item_screen_x=selected_item_screen_x;
saved_file_data_ptr->my_save_file.selected_item_screen_y=selected_item_screen_y;

saved_file_data_ptr->my_save_file.rank=gRank;
saved_file_data_ptr->my_save_file.kills=kills;
saved_file_data_ptr->my_save_file.promotion_points=promotion_points;
saved_file_data_ptr->my_save_file.done_salary=done_salary;	//have we credited today?

saved_file_data_ptr->my_save_file.level=current_level;
saved_file_data_ptr->my_save_file.hull=hull_strength;
saved_file_data_ptr->my_save_file.score=score;
saved_file_data_ptr->my_save_file.credits=credits;
saved_file_data_ptr->my_save_file.selected_pylon=missile_flip;
saved_file_data_ptr->my_save_file.accumulated_time=(stop_time-start_time)+accumulated_game_time;
saved_file_data_ptr->my_save_file.difficulty=difficulty;
saved_file_data_ptr->my_save_file.target_x=targ_x;
saved_file_data_ptr->my_save_file.target_y=targ_y;
saved_file_data_ptr->my_save_file.surrendered_flag=surrendered_flag;
saved_file_data_ptr->my_save_file.gone_around=gone_around_flag;
saved_file_data_ptr->my_save_file.script_timer1=script_timer1;
saved_file_data_ptr->my_save_file.script_timer2=script_timer2;
saved_file_data_ptr->my_save_file.script_state1=script_state1;
saved_file_data_ptr->my_save_file.script_state2=script_state2;
for (i=0;i<ITEMS_COUNT;i++)
saved_file_data_ptr->my_save_file.save_items[i]=ship_items[i];


for (i=0;i<MAX_HOLD_ITEMS;i++)
 {
 saved_file_data_ptr->my_save_file.hold_items[i]=hold_items[i];
 }
//copy galactic map
BlockMoveData(GM_ptr,&saved_file_data_ptr->the_gmap,sizeof (galactic_map));
LS_FlushEvents(everyEvent,0);
get_callsign_and_name(my_name,docked_at);
CToPascal(my_name,my_pname);
//oldmac//StandardPutFile(my_prompt,(unsigned char const*) my_pname,&my_reply);	//talk about finickety!

//FSSpec save_fsspec;
//make_data_fsspec("P","Zex2005SavedGame"/*my_pname*/,&save_fsspec);

//	err =	FSMakeFSSpec(my_reply.sVRefNum, my_reply.DirID, my_reply.name, &prefs_fsspec);
//    WriteZexFile(my_reply.sfFile,(char*)saved_file_data_ptr, sizeof (saved_file_structure));
    //WriteZexFile(save_fsspec,(char*)saved_file_data_ptr, sizeof (saved_file_structure));

	// write data...
	size_t write_size = fwrite((char*)saved_file_data_ptr, sizeof(char), sizeof(saved_file_structure), file_ptr);
	if(write_size != sizeof(saved_file_structure))
	{
		report_error_id_non_fatal("Problem writing saved game!",0);
	}


	//oldmac//FSClose( fileRefNum );
    }
LS_FlushEvents(-1,0);	//flush all
GraphicsActive();
   //oldmac//DisposePtr((Ptr)saved_file_data_ptr);
delete saved_file_data_ptr;

   gCheckWatchDog=1;	//enable the watchdog

}


int load_game()
{
	#if NO_LOAD_IF_UNREG
		#if EXPIRE		//don't show this if it's a test version
		#else
		if (gzex_prefs.registered!=1)
		{
			//can't load if not registered
			display_unreg(UNREG_LOAD);
			return 0;
		}
		#endif	//EXPIRE
	#endif //NO_LOAD_IF_UNREG

	if(!load_file_selector())
	{
		return 0;			// 0=false=bad
	}

	if(!load_saved_game())
	{
		return 0;
	}

	return 1;		// 1=true=good
}

//this is a tuffy, we have to load up the data and get zex
//into the mothership
int load_core_game_data(FILE* file_ptr)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength,hull_damage,current_level,next_level,difficulty,tutorial;
extern int level_complete,do_next_level,docked_at,mothership_cat;
extern int docked_at_shields, current_quad,global_open_doors;
extern float docked_at_power_rating;
//Str255 my_prompt="Load game...";
//oldmac//StandardFileReply my_reply;
//oldmac//OSType typeList []= 'GaMe';
	int i,return_value;
	//oldmac//short fileRefNum;
saved_file_structure * saved_file_data_ptr;
int callsign;



     global_open_doors=1;

   gCheckWatchDog=0;	//disable the watchdog
GraphicsPaused();
LS_FlushEvents(-1,0);	//flush all
//get memory for file

//oldmac//saved_file_data_ptr=(saved_file_structure*) NewPtr(sizeof(saved_file_structure));
saved_file_data_ptr= new(nothrow) (saved_file_structure);
if (saved_file_data_ptr==0)
      {
	report_error_id_non_fatal("Unable to load game - not enough memory.", 0); //oldmac//MemError());
      goto abort_load;
      }

LS_FlushEvents(everyEvent,0);
//oldmac//StandardGetFile(0,1,typeList,&my_reply);
//oldmac//if (my_reply.sfGood==1)
if(1)		// always true - for the moment there is no fileselector...
{
 return_value=1;	//OK

//	FSSpec load_fsspec;
//	make_data_fsspec("P","Zex2005SavedGame"/*my_pname*/,&load_fsspec);

    //oldmac//ReadZexFile(my_reply.sfFile,(char*)saved_file_data_ptr,sizeof(saved_file_structure));
    //ReadZexFile(load_fsspec,(char*)saved_file_data_ptr,sizeof(saved_file_structure));

	//load data here...
	size_t read_size = fread((char*)saved_file_data_ptr, sizeof(char), sizeof(saved_file_structure), file_ptr);
	if(read_size != sizeof(saved_file_structure))
	{
		report_error_id_non_fatal("Problem reading saved game - sorry.",0);
		delete saved_file_data_ptr;
		goto abort_load;
	}

	//oldmac// bug?? // FSClose( fileRefNum );

//we save by simply writing out the structure
 if (saved_file_data_ptr->my_save_file.platform!='MAC ')
 {
	 if(saved_file_data_ptr->my_save_file.platform==' CAM')		// byte reversed == wrong platform
	 {
		 report_error_id_non_fatal("This saved game is from a different architecture and won't work.",'FAIL');
	 }
	 else
	 {
		 report_error_id_non_fatal("Not a recognised saved game.",'FAIL');
	 }
	 delete saved_file_data_ptr;
	 goto abort_load;
 }
 if (saved_file_data_ptr->my_save_file.vers!=SAVE_VERS)
 {
	report_error_id_non_fatal("Saved Game is wrong version.",'FAIL');
	delete saved_file_data_ptr;
	goto abort_load;
 }

 next_level=saved_file_data_ptr->my_save_file.level+1;
 hull_strength=saved_file_data_ptr->my_save_file.hull;
 hull_damage = HULL_TOTAL - hull_strength;

sleep_timer=saved_file_data_ptr->my_save_file.sleep_timer;

gRank=saved_file_data_ptr->my_save_file.rank;
kills=saved_file_data_ptr->my_save_file.kills;
promotion_points=saved_file_data_ptr->my_save_file.promotion_points;

surrendered_flag=saved_file_data_ptr->my_save_file.surrendered_flag;
gone_around_flag=saved_file_data_ptr->my_save_file.gone_around;
script_timer1=saved_file_data_ptr->my_save_file.script_timer1;
script_timer2=saved_file_data_ptr->my_save_file.script_timer2;
script_state1=saved_file_data_ptr->my_save_file.script_state1;
script_state2=saved_file_data_ptr->my_save_file.script_state2;

targ_x=saved_file_data_ptr->my_save_file.target_x;
targ_y=saved_file_data_ptr->my_save_file.target_y;

selected_item_type=saved_file_data_ptr->my_save_file.selected_item_type;
selected_item_index=saved_file_data_ptr->my_save_file.selected_item_index;
selected_item_screen_x=saved_file_data_ptr->my_save_file.selected_item_screen_x;
selected_item_screen_y=saved_file_data_ptr->my_save_file.selected_item_screen_y;

 score=saved_file_data_ptr->my_save_file.score;
 credits=saved_file_data_ptr->my_save_file.credits;
 difficulty=saved_file_data_ptr->my_save_file.difficulty;
 done_salary=saved_file_data_ptr->my_save_file.done_salary;	//have we credited today?
 tutorial=0;
 missile_flip=saved_file_data_ptr->my_save_file.selected_pylon;

accumulated_game_time=saved_file_data_ptr->my_save_file.accumulated_time;
start_time=LS_TickCount60();

 for (i=0;i<ITEMS_COUNT;i++)
 ship_items[i]=saved_file_data_ptr->my_save_file.save_items[i];

for (i=0;i<MAX_HOLD_ITEMS;i++)  hold_items[i]=saved_file_data_ptr->my_save_file.hold_items[i];

//now we need to load up the OCB's from the galactic map then find the right mothership in the ocb's from the callsign
//we are all ready to run (all objects loaded, so we should be able to go docked...
//copy galactic map

BlockMoveData(&saved_file_data_ptr->the_gmap,GM_ptr,sizeof (galactic_map));

//load OCB's
init_map_control();
current_quad=saved_file_data_ptr->my_save_file.quad;
load_ocbs(saved_file_data_ptr->my_save_file.quad,saved_file_data_ptr->my_save_file.section);

level_complete=3;	//tell Zex we are docked
do_next_level=0;
//find the mothership we were docked at when saved in OCB's
callsign=saved_file_data_ptr->my_save_file.docked_at_mothership_id;

for(i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
 {
	if (ocb_ptr->object_list[i].mship_ptr!=0)	//bugfix for crash on load-game
	{
		if ((*ocb_ptr).object_list[i].mship_ptr->callsign==callsign) goto got_ms;
	}
 }
report_error_id_non_fatal("Couldn't load game. Docked at not found. Saved File corrupt?.",'FAIL');
return_value=0;
delete saved_file_data_ptr;
goto end_load;

got_ms: docked_at=i;
docked_at_shields=(*ocb_ptr).object_list[i].shield_value;
docked_at_power_rating=(*ocb_ptr).object_list[i].mship_ptr->power_rating;

(*ocb_ptr).object_list[0].Oworldx=(*ocb_ptr).object_list[i].Oworldx;
(*ocb_ptr).object_list[0].Oworldy=(*ocb_ptr).object_list[i].Oworldy;

mothership_cat=(*ocb_ptr).object_list[i].object_category;	//that should allow the main loop to dock us!
//oldmac//DisposePtr((Ptr)saved_file_data_ptr);
delete saved_file_data_ptr;


}
else
abort_load: return_value=0;	//cancel


end_load: LS_FlushEvents(-1,0);	//flush all
GraphicsActive();
gCheckWatchDog=1;	//enable the watchdog

return return_value;
}





// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_gets                | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION: Similar to standard libraries fgets, except handles different types
// | of newline.
// |
// | lsf_getstring reads at most the next n-1 characters into the array s, stopping
// | if a newline is encountered; the newline is NOT included in the array, which is
// | terminated by '\0'. lsf_getstring returns the number of characters read, or -1 for EOF
// | at the beginning of a new line
// |
// +----------------------------------------------------------------ROUTINE HEADER----

void lsf_getline(string& output_string, FILE *file)
{
	int c;
	output_string.erase();

	// get character and abort if end of file
	while((c=getc(file)) != EOF)
	{
		if(c == 13 || c == 10)
		{
			if(c == 13)	// CR followed by LF?
			{
				c=getc(file);
				if(c != 10) { ungetc(c,file); }
			}
			break;
		}

		output_string += c;	// write the character into the string
	}

}

//
// Similar to above but with input stream instead
//
void lsf_getline(string& output_string, ifstream& file)
{
	int c;
	output_string.erase();

	// get character and abort if end of file
	while(file.good())
	{
		c=file.get();
		if(c == EOF) break;

		if(c == 13 || c == 10)
		{
			if(c == 13 && file.good())	// CR followed by LF?
			{
				c = file.get();
				if(c != 10) { file.unget(); }
			}
			break;
		}

		output_string += c;	// write the character into the string
	}

}
