// Zex - 2D Space Trading Game with 3D graphics
// Copyright (C) 1998-2017  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

//returns 0=init ok, !0 init bad
/*
LS Sound Driver v1.00
based on Eclipse Sound Driver
Owner: SB
Started: 29/10/98
©Lightsoft 98
*/

#include "options.h"		// should always be first include
#include "SDL.h"
#include "SDL_mixer.h"
#include "LS_Standard.h"

//oldmac//#include <types.h>
//oldmac//#include <sound.h>
//oldmac//#include <CodeFragments.h>
//oldmac//#include <files.h>

//oldmac//#include	<resources.h>
#include	"sound.h"
#include	"Sound_int.h"
#include	"Error_handler.h"
#include "zex_misc.h"
#include "3d_structures_def.h"
#include "3d_top.h"
#include "objects_controls.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"
#include	"zex_misc.h"

unsigned char* sound_chan [17];	//17 sound channels
static Mix_Chunk* the_sounds[max_sounds];	//array of ptrs to sounds
Boolean sound_on;
int music_volume=63;	//0-63 permissable
int sound_volume=63;		//0-63 permissable
int  gMusicPlaying=0;

#define FIRST_CONT_CHANNEL 5
#define SECOND_CONT_CHANNEL 6
#define SIMPLE_SOUND_CHANNEL 7
//divisor list for master volume div into volume
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥Sound, MUSIC \/
unsigned char volume_list[] =
{
1,1,1,1,1,1,1,1,2,2,
2,2,2,3,3,3,3,3,3,3,
4,4,4,4,4,4,4,4,4,4,
5,5,5,5,5,5,5,5,5,5,
6,6,6,6,6,6,6,6,6,6,
7,7,7,7,8,9,10,11,12,16,
20,24,28,32,36,40,44,48,52,56,
};

extern int showing_stats;
//privates
static void init_continous_sound();
static void run_continuous_sound(int csound_chan);
static int find_empty_channel(void);

#define SOUND_DEBUG 0
#if SOUND_DEBUG
#warning "***SOUND_DEBUG is on***"
#endif


static void print_sound_data_in_loading_screen()
{
	//
	// print the SDL_mixer versions
	//
	SDL_version compile_version;
	MIX_VERSION(&compile_version);
	const SDL_version *link_version=Mix_Linked_Version();
	char info_string[200];
	sprintf(info_string, "MIXER: compiled - %d.%d.%d / running - %d.%d.%d\n",
			compile_version.major, compile_version.minor, compile_version.patch,
			link_version->major, link_version->minor, link_version->patch);
	show_text_in_loading_screen(info_string);

	//
	// Print the playback data
	//
	int frequency, channels, numtimesopened;
	Uint16 format;

	numtimesopened = Mix_QuerySpec(&frequency, &format, &channels);
	if(!numtimesopened) {
		printf("Mix_QuerySpec: %s\n",Mix_GetError());
	}
	else {
		const char *format_str="Unknown";
		switch(format) {
			case AUDIO_U8: format_str="U8"; break;
			case AUDIO_S8: format_str="S8"; break;
			case AUDIO_U16LSB: format_str="U16LSB"; break;
			case AUDIO_S16LSB: format_str="S16LSB"; break;
			case AUDIO_U16MSB: format_str="U16MSB"; break;
			case AUDIO_S16MSB: format_str="S16MSB"; break;
		}
		sprintf(info_string, "Sound: opened=%d times  freq=%dHz  format=%s (%d)  channels=%d",
			   numtimesopened, frequency, format_str, format, channels);
		show_text_in_loading_screen(info_string);
	}
}


/*----------------------------------------------------------------------*/
int init_zsound()
{
	int	temp=0;
	int	sound_counter=0;
	int i;

	int audio_rate = 44100;
	Uint16 audio_format = MIX_DEFAULT_FORMAT; /* 16-bit stereo */
	int audio_channels = 2;
	int audio_buffers = 4096;
#if (SOUND_DEBUG)
	printf ("Mix_OpenAudio\n");
#endif

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
	{
		report_error_id_non_fatal("Sound Init failed.",1);
	}

	print_sound_data_in_loading_screen();

for (i=0;i<16;i++) sound_chan[i]=0;	//set channels to nil
/*
temp=temp | SndNewChannel (&sound_chan[0],sampledSynth,initChanLeft+initChanRight,nil);	//ls1
temp=temp | SndNewChannel (&sound_chan[1],sampledSynth,initChanLeft+initChanRight,nil);	//2
temp=temp | SndNewChannel (&sound_chan[2],sampledSynth,initChanLeft+initChanRight,nil);	//3
temp=temp | SndNewChannel (&sound_chan[3],sampledSynth,initChanLeft+initChanRight,nil);	//4
//Stereo chans
temp=temp | SndNewChannel (&sound_chan[4],sampledSynth,initChanLeft+initChanRight,nil);	//5
temp=temp | SndNewChannel (&sound_chan[5],sampledSynth,initChanLeft+initChanRight,nil);	//6
//speech chan
temp=temp | SndNewChannel (&sound_chan[6],sampledSynth,initChanLeft+initChanRight,nil);	//speech
//left and right second channels
temp=temp | SndNewChannel (&sound_chan[7],sampledSynth,initStereo,nil);	//c1
temp=temp | SndNewChannel (&sound_chan[8],sampledSynth,initStereo,nil);	//stereo1


//laser channels
temp=temp | SndNewChannel (&sound_chan[9],sampledSynth,initStereo,nil);	//c2
temp=temp | SndNewChannel (&sound_chan[10],sampledSynth,initStereo,nil);	//c3
//temp=temp | SndNewChannel (&sound_chan[11],sampledSynth,initChanLeft+initChanRight,nil);	  //laser middle
//
////laser2 channels
//temp=temp | SndNewChannel (&sound_chan[12],sampledSynth,initChanRight,nil);	  //laser right
//temp=temp | SndNewChannel (&sound_chan[13],sampledSynth,initChanLeft,nil);	  //laser left
//temp=temp | SndNewChannel (&sound_chan[14],sampledSynth,initChanLeft+initChanRight,nil);	  //laser middle
//2 continuous sound channels
temp=temp | SndNewChannel (&sound_chan[11],sampledSynth,initChanLeft+initChanRight,nil);	//continuous chan1
temp=temp | SndNewChannel (&sound_chan[12],sampledSynth,initChanLeft+initChanRight,nil);	//continuous chan2
*/


//now get our sound resources into sound_array
//we start at resource ID 128 and carry on until either we get an error or reach max_sounds
while (sound_counter!=80)
      {
		char filename[100];
		sprintf(filename, "ZD3/R/%c%c%c%c%05d.rbs",
			static_cast<int>(0xff&('snd '>>24)),
			static_cast<int>(0xff&('snd '>>16)),
			static_cast<int>(0xff&('snd '>> 8)),
			static_cast<int>(0xff&('snd ')),
			sound_counter+128);
			the_sounds[sound_counter]=Mix_LoadWAV(filename);
			if (the_sounds[sound_counter]==0) break;	//fnf - assume done
			sound_counter++;	//next resource
      }
sound_on=1;
show_text_in_loading_screen("F4: Initialising music system");

init_continous_sound();
if (temp!=0)
{
	report_error_id_non_fatal("Sound Init failed.",temp);
	temp=0;	//continue running if we can
}

return temp;
}




//play a sound sound_number given priority priority on channel channel with volume volume

void play_zsound(int sound_number, int priority, int channel, int volume)
{
if (sound_on && showing_stats==0)	//mute the sound whilst showing stats
{

	int chan=find_empty_channel();
	  #if (SOUND_DEBUG)
	  printf ("play_zsound: Mix_PlayChannel chan=%i sound=%i\n", chan, the_sounds[sound_number]);
	  #endif
	chan=Mix_PlayChannel(chan, the_sounds[sound_number], 0);
	  #if (SOUND_DEBUG)
	  printf ("play_zsound: Mix_Volume chan=%i volume=%i\n", chan, volume_list[sound_volume]<<1);
	  #endif
	Mix_Volume(chan,(volume_list[sound_volume]<<1));	//zex is 0-63, SDL_mixer is 0-128
 }

return;
}



//play a sound sound_number given priority priority on channel channel with volume volume
//returns nothing. Always plays the sound by playing it on the last non-continuous sound channel
void play_zsound_always(int sound_number, int priority, int channel, int volume)
{

	  #if (SOUND_DEBUG)
	  printf ("play_zsound_always: Mix_PlayChannel chan=%i sound=%i\n", FIRST_CONT_CHANNEL-1, the_sounds[sound_number]);
	  #endif
	int chan=Mix_PlayChannel(FIRST_CONT_CHANNEL-1, the_sounds[sound_number], 0);

	  #if (SOUND_DEBUG)
	  printf ("play_zsound_always: Mix_Volume chan=%i volume=%i\n", chan, volume_list[sound_volume]<<1);
	  #endif
	Mix_Volume(chan,(volume_list[sound_volume]<<1));
}

extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects

//new 2d sound driver 130799 SB - 6 channels
void play_zsound_relative_to_zex(int the_object, int sound_number)
{
int volume;
int zex_x,zex_y;
int object_x,object_y;
int ear_x,ear_y,ear_ang;
int ear_left_volume,ear_right_volume;
int master_cut;
float tempf;

#define EARDIST 20000;


	zex_x=static_cast<int>((*ocb_ptr).object_list[0].Oworldx);
    zex_y=static_cast<int>((*ocb_ptr).object_list[0].Oworldy);

    object_x=static_cast<int>((*ocb_ptr).object_list[the_object].Oworldx);
    object_y=static_cast<int>((*ocb_ptr).object_list[the_object].Oworldy);
	ear_x=zex_x-EARDIST;
    ear_y=zex_y;

    ear_left_volume=ABS(get_distance_to_from_point(object_x,object_y,ear_x,ear_y));
	ear_x=zex_x+EARDIST;

	ear_right_volume=ABS(get_distance_to_from_point(object_x,object_y,ear_x,ear_y));
//now we need to increase the diff between left and right

	if (ear_right_volume>ear_left_volume+500)
    {
		ear_left_volume/=2;
		ear_right_volume*=2;
    }
    else
    if (ear_right_volume<ear_left_volume-500)
    {
		ear_right_volume/=2;
		ear_left_volume*=2;
    }

//now, we define a maximum range of 200000 and a maximum amplitude of 200 so...
    if (ear_right_volume>125000) ear_right_volume=125000;
    if (ear_left_volume>125000) ear_left_volume=125000;


    ear_right_volume=(125000-ear_right_volume)*2; //0-250,000
    ear_left_volume=(125000-ear_left_volume)*2;
//correct for the distance offset

    ear_right_volume/=1000;	//0-250
    ear_left_volume/=1000;



    master_cut=volume_list[64-sound_volume];	//1 to 64

    ear_right_volume/=master_cut;
    ear_left_volume/=master_cut;

	if (ear_left_volume>254) ear_left_volume=254;
	if (ear_right_volume>254) ear_right_volume=254;

	if ((ear_right_volume>0) || (ear_left_volume>0) )	//don't play if both volumes are 0
	{
		int chan=find_empty_channel();
		if (chan>=0)
		{
			  #if (SOUND_DEBUG)
			  printf ("play_zsound_relative_to_zex: Mix_PlayChannel chan=%i sound=%i\n", chan, the_sounds[sound_number]);
			  #endif

			chan=Mix_PlayChannel(chan, the_sounds[sound_number], 0);
			  #if (SOUND_DEBUG)
			  printf ("play_zsound_relative_to_zex: Mix_SetPanning chan=%i left volume=%i right volume=%i\n", chan, ear_left_volume, ear_right_volume);
			  #endif
			Mix_SetPanning(chan, ear_left_volume, ear_right_volume);
		}
	}

}

int find_empty_channel(void)
{
//we reserve channels 6 & 7 for continuous sounds, so we search 0 to 5
int chan;
	for (chan=0; chan<FIRST_CONT_CHANNEL;chan++)
	{
		if (!Mix_Playing(chan)) break;
	}
	if (chan==6) { return -1; }
	else
	{ return chan; }

}

//simple play sound takes ares id and starts that sound playing.
//simple stop sound stops the sound and frees up the memory
// make a channelDone function
void channelDone(int channel); // set the callback for when a channel stops playing

static Mix_Chunk* simple_sound;
static int simple_sound_chan;
void simple_play_sound(int id)
{
char filename[100];

sprintf(filename, "ZD3/R/%c%c%c%c%05d.rbs",
	static_cast<int>(0xff&('snd '>>24)),
	static_cast<int>(0xff&('snd '>>16)),
	static_cast<int>(0xff&('snd '>> 8)),
	static_cast<int>(0xff&('snd ')),
	id);

	simple_sound=Mix_LoadWAV(filename);
	  #if (SOUND_DEBUG)
	  printf ("simple_play_sound: Mix_PlayChannel chan=%i sound=%i\n", SIMPLE_SOUND_CHANNEL, simple_sound);
	  #endif

	simple_sound_chan=Mix_PlayChannel(SIMPLE_SOUND_CHANNEL, simple_sound, 0);
	Mix_Volume(simple_sound_chan,(volume_list[sound_volume]<<1));	//zex is 0-63, SDL_mixer is 0-128

		  #if (SOUND_DEBUG)
		  printf ("simple_play_sound: Mix_ChannelFinished channel_done=%i\n", channelDone);
		  #endif
	Mix_ChannelFinished(channelDone);	//callback for when channel has finished so we can free the memory
}

void simple_stop_sound()
{
		  #if (SOUND_DEBUG)
		  printf ("simple_stop_sound: Mix_HaltChannel chan=%i\n", simple_sound_chan);
		  #endif
	Mix_HaltChannel(simple_sound_chan);	//stop the sound
	if (simple_sound)
	{
		  #if (SOUND_DEBUG)
		  printf ("simple_stop_sound: Mix_FreeChunk sound=%i\n", simple_sound);
		  #endif
		Mix_FreeChunk(simple_sound);	//get the memory back
	}
}

void channelDone(int channel)
{
	if (channel==simple_sound_chan)
	{
		  #if (SOUND_DEBUG)
		  printf ("channelDone: Mix_FreeChunk sound=%i\n", simple_sound);
		  #endif
		Mix_FreeChunk(simple_sound);	//get the memory back (this might need to be done for all channels?)
		simple_sound=0;
	}
}



typedef struct
{
int object;
int sound;
int in_use;
int volume_percent;	//0 to 100 percent volume
int is_3d;
} ConSound;

ConSound the_cont_sound[2];	//we have two coninuous sound channels available

void init_continous_sound()
{
the_cont_sound[0].in_use=0;
the_cont_sound[1].in_use=0;
}

void set_cont_sound2_note(int midi_note)	//set freq of chan 2
{
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
SndChannelPtr the_chan;
SndCommand mySndCmd;

  the_chan=sound_chan[continuous2];
  mySndCmd.cmd=freqCmd;
  mySndCmd.param1=0;	//8 seconds
  mySndCmd.param2=midi_note;	//midi value - octave 5, C
  SndDoImmediate (the_chan,&mySndCmd);	      //install it
#endif
}


//Play a continuous sound in the center
void play_simple_continuous_sound(int soundID, int midi_note, float normalised_volume /*0-1*/)
{
int ear_left_volume,ear_right_volume;
int master_cut,volume;
float temp_volume;
    ear_right_volume=ear_left_volume=200;

    master_cut=volume_list[64-sound_volume];	//1 to 64

    ear_right_volume/=master_cut;
    ear_left_volume/=master_cut;
	temp_volume=ear_right_volume*normalised_volume;
	ear_right_volume=static_cast<int>(temp_volume);
	temp_volume=ear_left_volume*normalised_volume;
	ear_left_volume=static_cast<int>(temp_volume);
	if (ear_left_volume>254) ear_left_volume=254;
	if (ear_right_volume>254) ear_right_volume=254;


  #if (SOUND_DEBUG)
  printf ("play_simple_continuous_sound: Mix_PlayChannel channel=%i sound=%i\n", SECOND_CONT_CHANNEL, the_sounds[soundID]);
  #endif
	Mix_PlayChannel(SECOND_CONT_CHANNEL, the_sounds[soundID], -1);
  #if (SOUND_DEBUG)
  printf ("play_simple_continuous_sound: Mix_SetPanning channel=%i left=%i right=%i\n", SECOND_CONT_CHANNEL, ear_left_volume, ear_right_volume);
  #endif
	Mix_SetPanning(SECOND_CONT_CHANNEL, ear_left_volume, ear_right_volume);

}


//Stop a simple cont sound
void stop_simple_continuous_sound()
{

  #if (SOUND_DEBUG)
  printf ("stop_simple_continuous_sound: Mix_HaltChannel channel=%i\n", SECOND_CONT_CHANNEL);
  #endif
	Mix_HaltChannel(SECOND_CONT_CHANNEL);	//stop the sound
	the_cont_sound[1].in_use=0;
}



//more complex 2d continuous sound routines relative to an object


//this routine inserts a sound to be played positioned relative to object 0
//the sound should be looped (Use SoundEdit II)
//the sound is actually positioned in run_periodic_sound_tasks by setting the relative volumes of the left
//and right channel

//returns 0=ok,-1=error
int insert_continuous_sound(int object, int soundID, int midi_note)
{
int the_chan;
int n;

//see if object already generating a sound - just a sanity thing
if (the_cont_sound[0].object==object) return -1;
if (the_cont_sound[1].object==object) return -1;

//find a free slot
n=-1;
if (the_cont_sound[0].in_use==0)
{
   n=0;
   the_chan=FIRST_CONT_CHANNEL;
}
else
{
 if (the_cont_sound[1].in_use==0)
 {
   n=1;
   the_chan=SECOND_CONT_CHANNEL;
  }
}


if (n==-1) return -1;

	the_cont_sound[n].in_use=1;
	the_cont_sound[n].sound=soundID;
	the_cont_sound[n].object=object;
	the_cont_sound[n].volume_percent=100;
	the_cont_sound[n].is_3d=1;



  #if (SOUND_DEBUG)
  printf ("insert_continuous_sound: Mix_PlayChannel channel=%i sound=%i\n", the_chan, the_sounds[soundID]);
  #endif
	Mix_PlayChannel(the_chan, the_sounds[soundID], -1);
  #if (SOUND_DEBUG)
  printf ("insert_continuous_sound: Mix_SetPanning channel=%i left=%i right=%i\n", the_chan, 1,1);
  #endif
	Mix_SetPanning(the_chan, 1, 1);
return 0;
}

//set the volume as a percentage 0 to 100
void set_cont_sound_chan_volume(int object, int percent)
{

if (the_cont_sound[0].object==object)
 {
 the_cont_sound[0].volume_percent=percent;
 }
else
{
if (the_cont_sound[1].object==object)
 {
 the_cont_sound[1].volume_percent=percent;

 }

}
}


void clear_all_cont_sound_chans()
{

  #if (SOUND_DEBUG)
  printf ("clear_all_cont_sound_chans: Mix_HaltChannel channel=%i\n", FIRST_CONT_CHANNEL);
  #endif
	Mix_HaltChannel(FIRST_CONT_CHANNEL);	//stop the sound
  #if (SOUND_DEBUG)
  printf ("clear_all_cont_sound_chans: Mix_HaltChannel channel=%i\n", SECOND_CONT_CHANNEL);
  #endif
	Mix_HaltChannel(SECOND_CONT_CHANNEL);	//stop the sound

}


//manually clear a sound. Can also be cleared from run-continuous_sound on death of the object
void clear_cont_sound(int object)
{

	if (the_cont_sound[0].object==object)
	 {
		 the_cont_sound[0].in_use=0;
		 the_cont_sound[0].object=-1;

  #if (SOUND_DEBUG)
  printf ("clear_cont_sound: Mix_HaltChannel channel=%i\n", FIRST_CONT_CHANNEL);
  #endif
		 Mix_HaltChannel(FIRST_CONT_CHANNEL);	//stop the sound
	 }
	else
	{
		if (the_cont_sound[1].object==object)
		 {
			 the_cont_sound[1].in_use=0;
			 the_cont_sound[1].object=-1;

  #if (SOUND_DEBUG)
  printf ("clear_cont_sound: Mix_HaltChannel channel=%i\n", SECOND_CONT_CHANNEL);
  #endif
			 Mix_HaltChannel(SECOND_CONT_CHANNEL);	//stop the sound
		 }
	}
}


int nonlinear_volume[22]=	//can overdrive the sound chip by up to 100%
{
1,5,10,15,20,25,30,38,47,57,
67,80,100,125,140,160,185,200,200,200,200,200
};
int sound_last_run=0;

//this is a _VERY_ private function called from periodic tasks
void run_continuous_sound(int csound_chan)	//0 or 1
{
extern	float SinTable[360], CosTable[360];
int the_chan;
unsigned int the_object,vol_right,vol_left,mvolume;
int volume;
int zex_x,zex_y;
int object_x,object_y;
int ear_x,ear_y,ear_ang;
int ear_left_volume,ear_right_volume;
int master_cut;
float tempf,perc,fright,fleft;
SInt16 angle_to;
int distance;

if (the_cont_sound[csound_chan].in_use==0) return;	//nothing to do

the_object=the_cont_sound[csound_chan].object;

if((*ocb_ptr).object_list[the_object].in_use==1)
    {

		angle_to=get_angle_toobject_fromobject(the_object,0);
		angle_to=make_real_angle(angle_to);	//transform from zex angle to real angle
		distance=get_distance_to_from(the_object,0);
		//get distance in the range 0-255, where 0 is loud and 255 is silent (ish)
		//We say we cannot hear an object more than 125000 units away, so this simply becomes a divide by 500
		distance/=500;
		if (distance>255) distance=255;
		if (distance==0) distance=1;
	   if (csound_chan==0)
	   the_chan= FIRST_CONT_CHANNEL;
	   else
	   the_chan=SECOND_CONT_CHANNEL;;

		//set volume from master cut and channel percentage here

  #if (SOUND_DEBUG)
  printf ("run_continuous_sound: Mix_SetPosition channel=%i angle_to=%i distance=%i\n", the_chan, angle_to, distance);
  #endif
		Mix_SetPosition(the_chan, angle_to, distance);
  #if (SOUND_DEBUG)
  printf ("run_continuous_sound: Mix_Volume channel=%i volume=%i\n", the_chan, (volume_list[sound_volume]<<1));
  #endif
		Mix_Volume(the_chan,(volume_list[sound_volume]<<1));

    }

}


unsigned int rd_timer=0;

void run_periodic_sound_tasks()
{
extern int tractor;
//ongoing looped sound?
//transmitting request dock signal?
 if ((*ocb_ptr).object_list[0].request_dock==1 && tractor==0)
      {
      if (ZexTickCount()>rd_timer)
         {
         rd_timer=ZexTickCount()+60;
          play_zsound(bleeper, sound_high_pri, c1_chan1, sound_vol_7);

         }
      }

//we only do this every 10 frames
sound_last_run--;
if (sound_last_run>0) return;
 {
  sound_last_run=10;
  run_continuous_sound(0);	//do any positioning needed on cont chan 0
  run_continuous_sound(1);	//do any positioning needed on cont chan 1
 }

}
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥MUSIC
//load and init the mod player
static Mix_Music *music = NULL;
static bool running_ejection_music=false;

void load_mod_player()
{
	//Nothing here, using SDL_Mixer now

}

//takes valume 0-63
void music_set_vol(int volume)
{
  #if (SOUND_DEBUG)
  printf ("music_set_vol: Mix_VolumeMusic volume=%i\n", volume<<1);
  #endif

	Mix_VolumeMusic(volume<<1);
}


int gFadeMusic=0;
int firstcall=1;	//set to zero on subsequent calls
int actual_volume;
int gNewTune=0;	//at end of fade, if set music in new_music_ptr is played
UInt32 new_music_id;



void fade_out_music()
{
extern int music_volume;
gFadeMusic=1;
gNewTune=0;
}

//fades the music and then plays the new tune held as a Zex resource
void fade_and_play_zex_resource (unsigned long res_id)
{
 if (music_volume<7)
 {
        stop_music_now();
        return;
 }

        fade_and_play(res_id);
}


void fade_and_play(unsigned long  res_id)
{
gFadeMusic=1;
 running_ejection_music=false;
 if (music_volume>6)
 {
  gNewTune=1;
  new_music_id=res_id;
 }
 else
 {
        stop_music_now();	//a little optimisation; if volume is too low then we just stop playing music
 }
}



//called from our vbl proc - fades the music volume to zero and then stops the player
//or plays a new tune if gNewTune is set
void do_fade_out_music()
{
extern int music_volume;
if (firstcall==1)
 {
 actual_volume=music_volume;	//copy current volume
 firstcall=0;
 }

if (actual_volume>0)
{
  #if (SOUND_DEBUG)
  printf ("do_fade_out_music: Mix_VolumeMusic  volume=%i\n", actual_volume<<1);
  #endif
	Mix_VolumeMusic(actual_volume<<1);	//Zex is 0-63, SDL is 0-128
 actual_volume--;
}

if (actual_volume==0)
{
 gFadeMusic=0;	//stop VBL routine calling us
 firstcall=1;
 stop_music_now();
 if (gNewTune==1)
  {
	gNewTune=0;

	char filename[100];
	sprintf(filename, "ZD3/R/%c%c%c%c%05hu.rbs",
		static_cast<int>(0xff&('MADH'>>24)),
		static_cast<int>(0xff&('MADH'>>16)),
		static_cast<int>(0xff&('MADH'>> 8)),
		static_cast<int>(0xff&('MADH')),
		static_cast<unsigned short>(new_music_id));
  #if (SOUND_DEBUG)
  printf ("do_fade_out_music: Mix_LoadMUS\n");
  #endif

	music=Mix_LoadMUS((const char*)filename);
  #if (SOUND_DEBUG)
  printf ("play_tune: Mix_PlayMusic music=%i\n", music);
  #endif
	Mix_PlayMusic(music, 0);

	gMusicPlaying=1;
	music_set_vol(music_volume);
  }
}
}


//returns 1=yes, 0=no - doesn't work...?
int is_music_busy(void)
{
int busy;
  #if (SOUND_DEBUG)
  printf ("is_music_busy: Mix_PlayingMusic\n");
  #endif

busy = Mix_PlayingMusic();
return busy;
}


FSSpec the_level_file;

//Used to look in the resource fork of the level for a madh128 - if found plays it, else plays 130 from Zex
//It no longert does this (Zex2005)
void run_level_music(FSSpec * the_file)
{
the_level_file=(*the_file);	//make a note of the fsspec
play_level_music();

}


void run_ejection_music(void)
{
	fade_and_play(140);
	running_ejection_music=true;
}


bool are_we_running_ejection_music(void)
{
	return running_ejection_music;
}
//tyupically called from the level controller
void run_level_aux_music(int index)
{
//the_level_file is already set up from the initial level loader
play_level_aux_music(index);
}


//run_level_music must be called before this rouitne, which is typically called when undocking
void play_level_aux_music(int index)
{
short frefnum;
int have_level_music=0;
UInt32 music_id;

if (music_volume<7)
{
 stop_music_now();
 return;	//if volume not zero then play
}

      music_id = 128+index;
       if (music_id>134)
        {
			music_id = 132;
			fade_and_play(music_id);
        }
       else
        {
         have_level_music=1;
         fade_and_play(music_id);
        }
 gMusicPlaying=1;
}





//run_level_music must be called before this rouitne, which is typically called when undocking
void play_level_music()
{
extern int current_quad, training_mission;
int have_level_music=0;
UInt32 music_id;

	if (music_volume<7)
	{
	 stop_music_now();
	 return;	//if volume not zero then play
	}
	if (training_mission==1)
		  music_id = 128;  //Get the Handle to the Resource
	else
	{
	//real game
		  music_id = 150+current_quad;  //Get the Handle to the Resource

	}
//	if (music_id>134)
//	{
//	  music_id = 130;
//	  fade_and_play(music_id);
//	}
//	else
	{
	   have_level_music=1;
	   fade_and_play(music_id);
	}
	gMusicPlaying=1;
}



//play a MADH resource given it's ID
void play_tune(unsigned long res_id)
{
Handle tune_handle;
Ptr tune;
	if (music_volume>6)	//if volume not zero then play
	{
	char filename[100];
	 stop_music_now();

	sprintf(filename, "ZD3/R/%c%c%c%c%05hu.rbs",
			static_cast<int>(0xff&('MADH'>>24)),
			static_cast<int>(0xff&('MADH'>>16)),
			static_cast<int>(0xff&('MADH'>> 8)),
			static_cast<int>(0xff&('MADH')),
			static_cast<unsigned short>(res_id));
  #if (SOUND_DEBUG)
  printf ("play_tune: Mix_LoadMUS\n");
  #endif

	  music=Mix_LoadMUS((const char*)filename);
  #if (SOUND_DEBUG)
  printf ("play_tune: Mix_PlayMusic music=%i\n", music);
  #endif
	  Mix_PlayMusic(music, -1);	//music loop forever
	  gMusicPlaying=1;
	}
	else
	{
  #if (SOUND_DEBUG)
  printf ("play_tune: Mix_PauseMusic\n");
  #endif
	Mix_PauseMusic();
	 gMusicPlaying=0;
	}
}


//stop all music immediately
void stop_music_now()
{
  #if (SOUND_DEBUG)
  printf ("stop_music_now: Mix_PauseMusic\n");
  #endif
	Mix_PauseMusic();
  #if (SOUND_DEBUG)
  printf ("stop_music_now: Mix_HaltMusic\n");
  #endif
	Mix_HaltMusic();
	gMusicPlaying=0;
}


