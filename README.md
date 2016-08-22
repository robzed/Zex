# Zex - 2D Space Game with 3D graphics

Zex is a 1998 space-trading game with 2D game area but with 3D objects rendered with a software render. 

It was written by Stuart Ball and Rob Probin.

In 2005, Rob started to convert the underlying Mac game to SDL (using a custom made Mac-classic abstraction layer) and make the game cross platform. Soon Stu and others (involved in the original development) joined in this effort. Apart from that there were minor changes to get it building (and fix the odd bug) in 2010 and 2015.

We haven’t checked the wormholes still work.

NOTE: The game and manual refer to lightsoft.co.uk - but that web site doesn’t really exist except as a placeholder currently.


## REPOSITORY OVERVIEW

All source code is available and released as open source.

Project files are provided for Mac OS, Windows and Open Pandora. Since the Open Pandora is Linux already we don’t think a x86 desktop Linux port would be difficult at all. You might need to squish some bugs…

.ZLV files are level files and are text files.

.z files are 3D model “mesh” files (exported with a program called ‘Meshworks’).

RAW are raw texture files - and are binary.

.rbs are various files that were copied from the Mac resources. These can be considered binary files, but in reality things are not that simple :-) These contain text, graphics and other data.

Sadly, no original graphics source files are present in the repository currently, but reverse engineering the rbs files is probably possible. We probably have the originals somewhere on a CD…

## LICENSES

Code GPLv3 (Share-a-like, patent grant, etc.) 

Graphics - source not supplied, but:
    Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
(Contact us if you need other terms).

Please only use the ‘Zex’ name - to represent the original game and ‘Lightsoft’ logos (including fonts, rainbows, and Anvil) to represent only the trading name of original authors (Stuart Ball and Robert Probin). We consider these trademarks - although distribution with game is permitted as long as you don’t try to trade with these logos or otherwise represent them as your own.


Documentation:
  Attribution-NonCommercial-ShareAlike 4.0 International
(Contact us if you need other terms).

There are various SDL libraries in the repository  - these have their own license. See the files or http://libsdl.org/


## OUTSTANDING CHANGES REQUIRED

There are quite a few things that need doing, here are a few:

 * Intro sound cuts-off (timing issue?)
 * Some music & sounds don’t play
 * Zex Pandora build has some obvious problems
  - Crash in credits
  - Text is just bar graphics
  - Breaks on game end
 * On some machines full screen is shifted to the left?
 * Split application data directory and game-save data-write directory (saved games, key prefs, game prefs)
 * Consider Double resolution of screen (scale non-3D).
 * Build bugs list (and get original bugs list?)
 * Sort out 64-bit build of Zex
 * What do we want to do with all the Lightsoft.co.uk addresses?
 * Linux build

## BINARIES
A Mac prebuilt binary can be downloaded at:
    http://trx-elgar.synology.me/downloads/ZexMac.zip
    http://robprobin.com/pmwiki.php?n=Main.Zex
(Note, you will need to Ctrl-click on the file to open it, as it isn't signed.)

