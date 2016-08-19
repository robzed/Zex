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


