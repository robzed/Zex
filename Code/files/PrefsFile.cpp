// Zex - 2D Space Trading Game with 3D graphics
// Copyright (C) 1998-2005  Stuart Ball and Robert Probin
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

// PrefsFile.cpp

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "file_io.h"
#include "PrefsFile.h"

//#ifdef __cplusplus
//extern "C" {
//#endif
OSType gPrefsCreator = 'LsZx';
//#ifdef __cplusplus
//}
//#endif

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
// not used
// if we need to port this - it's easy enough - use make_data_fsspec()
//
FSSpec PrefsFileSpec( Str255 fileName )
{
	short foundVRefNum;
	long foundDirID;
	FSSpec out;
	OSErr err;
	
	// find the Preferences folder
	err = FindFolder(	kOnSystemDisk,
						kPreferencesFolderType,
						kCreateFolder,
						&foundVRefNum,&foundDirID);
	if (!err)
		// make it into a FSSpec (given the file name)
		FSMakeFSSpec(foundVRefNum, foundDirID, fileName, &out);
	return out;
}
#endif

OSErr WritePrefsFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen ) {

	return WriteZexFile(fileSpec, buffer, bufferLen);
	
	// old stuff
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
	short fileRefNum;
	OSErr err;
	
	// create our prefs file
	err = FSpCreate( &fileSpec, gPrefsCreator, 'zprf', smSystemScript );
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

OSErr ReadPrefsFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen ) {
	
	return ReadZexFile(fileSpec, buffer, bufferLen);
	
	// old stuff
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING

	short fileRefNum;
	OSErr err;
	
	// open prefs file for reading
	err = FSpOpenDF( &fileSpec, fsCurPerm, &fileRefNum);
	if (err) return err;

	// read the data
	err = FSRead( fileRefNum, &bufferLen, buffer );
	FSClose( fileRefNum );

	return err;
#endif
}


