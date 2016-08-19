// PrefsFile.h

//oldmac//#include <Files.h>
//oldmac//#include <folders.h>
//oldmac//#include <script.h>

//#ifdef __cplusplus
//extern "C" {
//#endif

//oldmac//FSSpec PrefsFileSpec( Str255 );
OSErr WritePrefsFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen );
OSErr ReadPrefsFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen );
	
//#ifdef __cplusplus
//}
//#endif


