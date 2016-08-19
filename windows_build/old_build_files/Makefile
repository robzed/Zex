# Rob's Zex2005 Makefile
#
# Used for building Zex 1 for Win32 on Linux
# Based on the Z_dungeon make file
#
# !!!!!!!
# WARNING: This Makefile contains (and needs) TABS in front of commands!!!
# !!!!!!!
#
# $Author: rob $
# $Date: 2006/05/30 21:36:35 $
# $Revision: 1.4 $
#
# For log see end of file...
#

#
# These define the files we are going to compile
# (Avoid spaces in filenames - I haven't set the make up to quote filenames
# before passing to the command line or gcc)
#

# Note for Zex - these are the files that are used (there are files not selected for use in Xcode).
Zex_src_3d_Engine = 3d_top.cpp light_control.cpp matrix_primitives.cpp
Zex_src_angular_stuff = angular_stuff.cpp
Zex_src_Camera_Control = Camera_control.cpp
Zex_src_Drawing_Primitives = Batcom.cpp Credits.cpp Damage.cpp dlp.cpp dlp_explosions.cpp Fuel.cpp instrum.cpp lives.cpp missiles.cpp pictures.cpp radar.cpp score.cpp thrust.cpp txtsplat.cpp
Zex_src_Error_Handler = Error_handler.cpp emergency_varfont.cpp
Zex_src_files = enhanced_string.cpp file_io.cpp file_save_description_menu.cpp PrefsFile.cpp game_file_selector.cpp game_file_parser.cpp game_save.cpp save_path_destination.cpp simple_command_interpreter.cpp timing.cpp
Zex_src_Menu_Processing = get_text_string.cpp input_string_for_menu.cpp menu_driver.cpp text_entry_menu.cpp zdungeon_printing_emulation.cpp
Zex_src_flotsam_and_jetsam = do_intro.cpp docked.cpp score_and_credits.cpp zex_misc.cpp
Zex_src_Galaxy_control = game_script.cpp Galaxy.cpp map_control.cpp planet_gen_main.cpp Planet_Generator.cpp
Zex_src_init = init_memory.cpp z_init.cpp
Zex_src_Input_Handler = input_control.cpp key_setting.cpp key_setup_display.cpp
Zex_src_Level_Scripter_Loader = compile_zmap.cpp handle_level.cpp level_control.cpp load_complex_object.cpp load_level.cpp
Zex_src_Levels = level1.cpp level2.cpp level3.cpp level4.cpp level5.cpp level_game.cpp
Zex_src_Main = game_pause.cpp main_sel_screen.cpp ZEX.cpp game_registration_menu.cpp register_game.cpp
Zex_src_Monster_control = monster1.cpp monster2.cpp monster_control_misc.cpp NPC_Control.cpp transporters.cpp wingman.cpp wingman_game.cpp
Zex_src_Object_Drawing_Manager = drawing.cpp
Zex_src_OGL = OGL.cpp ogl_text.cpp
Zex_src_Other_Object_Control = animation.cpp boid.cpp cargo.cpp check_bounding_boxes.cpp CPanel.cpp explode.cpp fire_weapon.cpp insert_object.cpp load_objects.cpp MeshToZex.cpp misc_controllers.cpp motherships.cpp object_control_misc_routines.cpp object_control_top.cpp objects_misc.cpp
Zex_src_Player_Control = real_time.cpp refuel.cpp ship_control.cpp
Zex_src_Screen_Splat_Control = Splat_control.cpp
Zex_src_SDLMain = mac_emulation.cpp main.cpp
Zex_src_Software_3d_Renderer = fcircle.cpp fdistort.cpp fdistort_mask.cpp fdistort_multi.cpp fgouraud.cpp FPOLY4_1.cpp polyclip.cpp texture_management.cpp
Zex_src_Sound_and_Music_Control = sound.cpp
Zex_src_static_stuff = buy.cpp command_and_control.cpp hiscore.cpp inventory.cpp show_stats.cpp
Zex_src_weapons_misc = aim.cpp

#
# and the actual munging of those groupings into one long linee
# (The groupings are entirely for our convience)
#
ZD_sources = $(Zex_src_3d_Engine) $(Zex_src_angular_stuff) $(Zex_src_Camera_Control) $(Zex_src_Drawing_Primitives) $(Zex_src_Error_Handler) $(Zex_src_files) $(Zex_src_Menu_Processing) $(Zex_src_flotsam_and_jetsam) $(Zex_src_Galaxy_control) $(Zex_src_init) $(Zex_src_Input_Handler) $(Zex_src_Level_Scripter_Loader) $(Zex_src_Levels) $(Zex_src_Main) $(Zex_src_Monster_control) $(Zex_src_Object_Drawing_Manager) $(Zex_src_OGL) $(Zex_src_Other_Object_Control) $(Zex_src_Player_Control) $(Zex_src_Screen_Splat_Control) $(Zex_src_SDLMain) $(Zex_src_Software_3d_Renderer) $(Zex_src_Sound_and_Music_Control) $(Zex_src_static_stuff) $(Zex_src_weapons_misc)

#
# Which directories are the source files in??? 
# BIG NOTE: Do not use spaces in directory names - it doesn't work!
#
ZD_directories = ../Code/3d_Engine:../Code/angular_stuff:../Code/Camera_Control:../Code/debug_timing:../Code/Drawing_Primitives:../Code/Error_Handler:../Code/files:../Code/flotsam_and_jetsam:../Code/Galaxy_control:../Code/Init:../Code/Input_Handler:../Code/Level_Scripter_Loader:../Code/Levels:../Code/Main:../Code/Menu_Processing:../Code/Monster_control:../Code/Object_Drawing_Manager:../Code/OGL:../Code/Other_Object_Control:../Code/Player_Control:../Code/Screen_Splat_Control:../Code/SDLMain:../Code/Software_3d_Renderer:../Code/Sound_and_Music_Control:../Code/Standard_Headers:../Code/static_stuff:../Code/weapons_misc



# These are general rubblish used to initialise environment
#
# See http://www.gnu.org/prep/standards_51.html#SEC51
# This is the standard good make file practice
#
SHELL = /bin/sh
srcdir = .
# ensure we only deal with these suffixes (I think this is obsolete - and not used by us)
#.SUFFIXES:
#.SUFFIXES: .cpp .o .exe .d


# These are various constants that define what we are doing
#
# Specifically these define exactly how the compiler will operate
#
CC = g++
#DEFS =  -DPACKAGE=\"SDL_tests\" -DVERSION=\"1.2.3\"  -I. -I$(srcdir) 
DEFS =  -I. -I$(srcdir) 
CPPFLAGS =  
LDFLAGS = 
INCLUDES = $(patsubst %,-I%,$(subst :, ,$(ZD_directories)))
additional_LDFLAGS =
LIBS =  -L/usr/local/cross-tools/i386-mingw32msvc/lib -lmingw32 -lSDLmain -lSDL -lSDL_mixer -mwindows
CFLAGS = -g -O2 -I/usr/local/cross-tools/i386-mingw32msvc/include/SDL -Dmain=SDL_main -DHAVE_OPENGL -Wno-multichar
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(CFLAGS) $(LDFLAGS) -o $@


# thess vpaths defines where the make looks for our source files
vpath %.h $(ZD_directories)
vpath %.cpp $(ZD_directories)

# There are objects, and these are made up one for each c file
ZD_objects = $(ZD_sources:.cpp=.o)

#
# this auto-generates the individual dependancies files
# -M is dependances including system files, -MM is gcc for dependances not including system files
#
# See http://www.gnu.org/manual/make/html_chapter/make_4.html#SEC43 for how this works
#
# It actually says "this is the general rule for making .d (make files) from .cpp files"
# and the command afterwards says compile and just make dependances (-MM)
# and then change the dependance file (sed) and write to a file. If there is a
# file, but it is empty, delete it. (I think).
#
%.d: %.cpp
	@echo -----Rebuilding make file $@ from $< -----
	@set -e; $(COMPILE) -MM $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@


#
# Tell it how to make a .res file from a .rc plus an .ico file
# This is to allow us to set the application icon
# (the first icon resource in the file)
# http://gcc.gnu.org/ml/gcc-help/2002-10/msg00102.html
# http://www.cygwin.com/cygwin-ug-net/windres.html
# http://www.cygwin.com/ml/cygwin/1999-05/msg00249.html
# http://en.wikipedia.org/wiki/ICO_(icon_image_file_format)
# http://homepage.ntlworld.com/config/max/appiconguide.htm
#
%.res : %.rc
	@echo ----Building resource $@ from $< ----
	@i386-mingw32msvc-windres $< -O coff -o $@


# Tell make how to make .cpp from .o - we override the inbuilt mechanism since we have a DEF section
# $< is the source file
# $@ is the output file
#
%.o : %.cpp
	@echo -----Rebuilding Object file $@ from $< -----
	@$(COMPILE) -c $<

#
# this is what we actually building. We link the objects (.o files) with the resource file and the libraries.
#
Zex.exe : $(ZD_objects) zex_resfile.res
	@echo -----Building Zex.exe-----
	@rm -f Zex.exe
	@$(LINK) $(additional_LDFLAGS) $(ZD_objects) zex_resfile.res $(LDADD) $(LIBS)

#
# now lets actually build the C into object files
# ===============================================
#foo.o: foo.cpp
#	$(COMPILE) -c $< -o $@
#
# We include individual make files for all source files and let the rules
# above compile them... (this is just a list of output : inputs ).
include $(ZD_sources:.cpp=.d)


# command to build all
all:
	@echo "This command is unsupported currently"

# command to install - no install provided
install:
	@echo "This command is unsupported"

# command to remove all built targets
# (I'm sort of not sure of how to stop the make running first...)
clean:
	rm -f $(ZD_objects) Zex.exe $(ZD_sources:.cpp=.d)
    
# Tell make which targets are not really files (i.e. make commands)
.PHONY: clean install all
        
# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:

# CVS LOG
#
# $Log: Makefile,v $
# Revision 1.4  2006/05/30 21:36:35  rob
# Zex2005: Updated Windows makefile with new files for beta 4 release build.
#
# Revision 1.3  2006/03/04 21:37:28  rob
# Zex2005: Update windows build system so that it includes the icon resource in the executable. Also avoid annoying four-char-constants warning.
#
# Revision 1.2  2005/12/11 23:38:43  rob
# Zex2005: Updated windows build for 0.97 alpha.
#
# Revision 1.1  2005/10/10 20:07:52  rob
# Zex2005: Added basic build stuff for windows build
#
# Revision 1.16  2005/01/22 23:12:57  rob
# ZD: Windows Makefile changed to create build products in a sub-directory.
#
# Revision 1.15  2005/01/10 19:57:47  rob
# ZD: Added SDL_mixer library. We can now build Windows SDL applications from the Mac!
#
# Revision 1.14  2005/01/09 23:00:29  rob
# ZD: Progress! - copied libraries and headers across. Minor corrections to some files to avoid build warnings. Changed gcc to g++ in Makefile now get's rid of most errors.
#
# Revision 1.13  2005/01/02 18:45:02  robp
# ZD: Minor change to make file - remove SDL test reference.
#
# Revision 1.12  2005/01/02 10:09:17  robp
# ZD: Yet Another typo
#
# Revision 1.11  2005/01/02 10:04:23  robp
# ZD: Another typo
#
# Revision 1.10  2005/01/02 10:01:16  robp
# ZD: Type in files to build line
#
# Revision 1.9  2005/01/01 23:06:21  robp
# ZD: Correct include error.
#
# Revision 1.8  2005/01/01 20:55:40  robp
# ZD: updated Makefile for windows build using cpp files.
#
# Revision 1.5  2005/01/01 19:48:19  robp
# ZD: updated Makefile for windows build.
#
# Revision 1.4  2003/06/14 09:47:41  robp
# Corrected spelling of new filename menu_driver
#
# Revision 1.3  2003/06/14 09:43:27  robp
# Updated windows build file with extra files and added headers and footers.
#
#
