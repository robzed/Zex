Summary of how to get MinGW and SDL installed to make Windows exe from Mac Terminal.
By Rob Probin 9th October 2005


1. Install MinGW
================

The easiest way (and in fact the only way I've done this) is to run the script:

	sudo ./build-cross_updated_by_rob.sh


Some *really* important notes:

(i) Copy this file before running into a directory without spaces in the pathname. I think
a path with spaces will break it. You can get rid of this directory afterwards. 

An example path would be:

	/Users/rob/make_cross_tools


(ii) This needs to be done with superuser privs to install to /usr/local/cross-tools - hence the
sudo. But you could alter the script to install them elsewhere - theoretically. You will need to 
alter the build scripts if you do this thou.

(iii) I do run this script from Kconsole in KDE under X11. I do this primarily because it's closer 
to the Linux environment this script was generated in. Theoretically it's possible to do run this script
under Mac OS X terminal - but you'll need to install wget because the script relies on that to 
download things.

(iv) Some of the build process requires the command line tool msgfmt - which is part of the GNU 
gettext-tools package so you'll need that as well. There might be other tools required as well.

(v) Use gcc3.3. The four gcc MinGW packages currently don't compile under gcc 4. If you can't 
remember how to do this it's the following command:

sudo gcc_select 3.3

Also note 'gcc_select -l' tells you what's available and plain gcc_select tells you what you are set as.

(vi) After a while you might want or need to update the package version numbers. Go to the MinGW site 
and the download page and alter the version numbers of the four package in the script to the ones that
you want to install.

(vii) When the process is complete you'll have a set of tools that works under the Mac OS X terminal.



2. Installing SDL
=================

I've booked these two files in for Zex2005. However, for completeness I talk about 
downloading them here.

Download the Windows development library from http://www.libsdl.org which is called:

	SDL-devel-1.2.9-mingw32.tar.gz


You might as well get the Windows runtime library as well which is called:

	SDL-1.2.9-win32.zip


Take the development library and unpack it.

Theoretically you can do a 'make cross'. However, it doesn't work for me. So manually isn't 
too bad:

The directories in SDL-1.2.9 you want are:

bin
include
lib
share

You want to merge the files inside these into /usr/local/cross-tools/i386-mingw32msvc (assuming 
you didn't change the compiler target path). 

You can do this using 'Go to folder' in the finder or via the command line. 

**BIG NOTE**: For some reason the include files in 1.2.9 are directly inside 'include/' rather than
being inside 'include/SDL/'. However this will break the build. So therefore move them to
a SDL/ folder under include/ the target directory.


Side Note: This manual method doesn't set up the link for sdl-config. But that's OK because we don't
use that at the moment. (sdl-config allows the path to the headers and libraries to be determined
at run-time rather than when creating the makefile). The make file does this:
	cp -rv bin include lib share /usr/local/cross-tools/i386-mingw32msvc/
	@ln -sf i386-mingw32msvc-sdl-config $(prefix)bin/sdl-config


2b. Installing SDL Mixer
========================

Get the windows SDL_mixer developer library:
http://www.libsdl.org/projects/SDL_mixer/
SDL_mixer-devel-1.2.6-VC6.zip

Unpack it. 
Contents of the lib directory go into /usr/local/cross-tools/i386-mingw32msvc/lib/
Contents of the include directory go into /usr/local/cross-tools/i386-mingw32msvc/include/SDL/


That's it.


3. Test compile
===============

Test compile by running ./win_build.sh in the root directory of Zex2005.

Notice: If you are unsure of the compiler being run you can add -v to the compiler options 
line in the Makefile.

Additionally, you can find some examples of basic windows program on the MinGW site. However
you'll probably want a build script in order to select a target and some such. There are some
example programs and scripts below.

You can put the windows SDL DLL in the same directory as the application for testing.



That's it!


-- END OF INSTRUCTIONS --

--------------------------------------------------------------------------------------
WINDOWS EXAMPLES
================

#### hello.sh #####

#!/bin/sh

PREFIX=/usr/local/cross-tools
TARGET=i386-mingw32msvc
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec gcc -o hello.exe hello.c


#### hello.c #####

#include <stdio.h>

int main(int argc, char **argv)
{
	printf("Hello\n");
	return (0);
}




#### hello2.sh #####

#!/bin/sh

PREFIX=/usr/local/cross-tools
TARGET=i386-mingw32msvc
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec g++ -o hello2.exe hello2.cpp


#### hello2.cpp

#include <iostream>
int main(int argc, char **argv)
{
	std::cout << "Hello" << std::endl;
	return (0);
}


#### sdl_test.sh ####

#!/bin/sh

PREFIX=/usr/local/cross-tools
TARGET=i386-mingw32msvc
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec gcc -o sdl_test.exe sdl_test.c -I/usr/local/cross-tools/i386-mingw32msvc/include/SDL -Dmain=SDL_main -L/usr/local/cross-tools/i386-mingw32msvc/lib -lmingw32 -lSDLmain -lSDL -lSDL_mixer -mwindows


#### sdl_test.c ####

#include <stdio.h>

#include "SDL.h"
#include "SDL_mixer/SDL_mixer.h"


int main(int argc, char **argv)
{
	printf("sdl test: Hello (on %s at %s)\n",__DATE__,__TIME__);

	// start SDL with audio support
	if(SDL_Init(SDL_INIT_AUDIO)==-1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}

	printf("sdl test: inited\n");
	
	// open 44.1KHz, signed 16bit, system byte order,
	//      stereo audio, using 1024 byte chunks
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}
	

	printf("sdl test: Goodbye\n");

	return (0);
}



#### winhello.sh ####

#!/bin/sh

PREFIX=/usr/local/cross-tools
TARGET=i386-mingw32msvc
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
gcc -c winhello.c
gcc -o winhello.exe winhello.o -mwindows


#### winhello.c ####

#include <windows.h>

int WINAPI WinMain (HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					PSTR szCmdLine, 
					int iCmdShow) 
{
	MessageBox (NULL, "Hello", "Hello Demo", MB_OK);
	return (0);
}





--------------------------------------------------------------------------------------

DIARY OF GETTING RE_INSTALLED
=============================

Hi,

It's 21:47 06-10-2005 and I'm trying to remember how to build Windows MinGW and install 
the windows SDL libraries on MacOSX - I did have them once - but no more. This is a 
basic overview of the process just in case (when) I (or anyone else) need to do it 
again.

MinGW
=====

There appear to be two ways of doing this:

1. Using Sam Lantinga's (of SDL) cross build script off the SDL site to build and install 
the tools and libraries. This was originally written for cross building from Linux, but
since it compiles the tools it should be ok.

http://www.libsdl.org/extras/win32/cross/README.txt

This is ok but you will need to:
	1. update the script to download versions of the tool that are on the server.
	2. make sure you run it from a path without spaces
	3. Run it from an X11 session - say Konsole in KDE (this is for tools such as 
	wget - it should be possible from a standard Mac OS X terminal but you'll obviously
	need to install what's missing.)
	4. run it with sudo
	5. some other stuff I never got to.
	
It didn't work so onto method 2.


2. Use fink to install the necessary packages. At the time of writing these are:

mingw-binutils	2.16.91-20050827-1-1	GNU binutils for MinGW
mingw-gcc	 	3.4.4-20050522-1-3	 	GNU GCC for MinGW
mingw-runtime	3.8-1					Minimalist GNU for Windows - Runtime
mingw-w32api	3.3-1					Minimalist GNU for Windows - Win32 API


Notice the build script also installs OPENGL_ARCHIVE and DIRECTX_ARCHIVE. I'm not
sure whether we need these are not. It doesn't appear to install the runtime.

-------------------------


SDL Install
===========

The install guidelines in the Win32: SDL-devel-1.2.8-mingw32.tar.gz binary say you can do 
a 'make native' or a 'make cross'. Theoretically make cross should do the right things. It
appears not to however.

Therefore, instead I copied the following directories:

	bin
	include
	lib
	share


wholesale into the folder:

	/usr/local/cross-tools/i386-mingw32msvc/


This path didn't exist past /usr/local/ on my machine so I had to create it. If it does - and 
the folders inside also exist then you will need to merge the files. I didn't have to do this.


-------------------------


Results:

Compiled via the standard MacOSX command line. Thought we should need to compile via KDE Kconsole
in X11 but it appears this works ok? Strange. Anyhow...

Had a problem with the file CPanel.c since there are two of these - one is redundant and needs 
deleting (which I did).

The second one is some compile errors. It appears you always get some different ones with different
versions of gcc.

Detail: -fpascal-strings - Allow Pascal-style string literals to be constructed. This is APPLE ONLY, i.e. not in 
the main gcc branch. Therefore the "/p" strings need to be removed from the project. (There is a long 
conversation with a poor Apple engineer - Ziemowit Laski - trying to get it submitted. The gcc 
maintainers are PICKY!).  -- By the fun-jobs-are-us dept.

Fixed that.

-----------------------


More problems: 

Using -v on compiler line found out we are still building with Apple's gcc4 for PowerPC. Doh. This
applies whether we use MacOSX's Terminal or X11/KDE Konsole. After some head scratching it appears
that of course it will do this because of the cross-make.sh will select a compiler directory and
target that will not exist.

There appears to be two options:
	1. Retarget the cross-make script to use the Fink package (which is i686 package - P2 and above - 
	probably ok. 
	2. Get the download script working.

I choose 2 - since that's how I had it building before. 

Previously I'd modified the script to use the same package versions as fink. According to the MinGW download
page (http://www.mingw.org/download.shtml) the compiler is Candidate package. I probably want the current
version?

The versions are:

build-cross.sh
	GCC_VERSION=3.4.1-20040711-1
	BINUTILS=binutils-2.15.90-20040222-1
	MINGW=mingw-runtime-3.3
	W32API=w32api-2.5
	
MinGW - Current
	gcc-core-3.4.2-20040916-1-src.tar.gz (also g++)
	mingw-runtime-3.8-src.tar.gz
	binutils-2.15.91-20040904-1-src.tar.gz
	w32api-3.3-src.tar.gz
	
MinGW - Candidate
	gcc-core-3.4.4-20050522-1-src.tar.gz (also g++)
	binutils-2.16.91-20050827-1-src.tar.gz
	
MinGW - Previous
	gcc-core-3.3.1-20030804-1-src.tar.gz
	gcc-3.2.3-20030504-1-src.tar.gz
	runtime - most between 2.4 and 3.7
	win32api - most 2.2 and 3.2
	binutils 2.13
	
Fink today (9th Oct 2005)
	gcc-3.4.4-20050522-1-3
	binutils-2.16.91-20050827-1-1
	runtime-3.8-1
	win32api-3.3-1


When I build Fink/Candiate version it had problems finding a tool:

file=/Users/rob/make_cross_tools/source/binutils-2.16.91-20050827-1/bfd/po/`echo fr | sed 's,.*/,,'`.gmo \
  && rm -f $file && PATH=../src:$PATH msgfmt -o $file /Users/rob/make_cross_tools/source/binutils-2.16.91-20050827-1/bfd/po/fr.po
/bin/sh: line 1: msgfmt: command not found
make[3]: *** [fr.gmo] Error 127


Target for new script will be MinGW current.

Note: this download script needs to be run as sudo ....

Again msgfmt: command not found. Also there was a problem with a zero block in the tar.

msgfmt is part of the GNU gettext-tools. It appears to be (at least partially) installed - except for 
the shared library.

Decided to update fink. 

Fink didn't update properly. However, the msgfmt command is working in Konsole.

Tried building binutils again. This time there is a compile error (something about an incomplete
array definition).

Changed the compiler to 3.3 with:

sudo gcc_select 3.3
(note also "sudo gcc_select -l" to view which versions you have installed and "gcc_select" to view the currently
active gcc_compiler).

Tried script again - all works (apart from two can't process messages?).

Seems to give a different set of warnings. Adding -v to make file.

GNU C++ version 3.4.2 (mingw-special) (i386-mingw32msvc)
        compiled by GNU C version 3.3 20030304 (Apple Computer, Inc. build 1809).
		
Hurrah!!!

There are several things which appear to need fixing - but it should be fine now.

--Rob



