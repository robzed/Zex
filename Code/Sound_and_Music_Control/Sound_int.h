//sound_int.h
//fixed data - basically sound channel codes for various ops

//oldmac//struct SndCommand quiet_now = { quietCmd, 0, 0 };
//oldmac//struct SndCommand flush_now = { flushCmd, 0, 0 };
//oldmac//struct SndCommand amp_command = {ampCmd, 0, 0 }; 
//oldmac//struct SndCommand volume_command = {volumeCmd, 0, 0 }; 

//status buffer
//oldmac//struct SCStatus chan_status;

/*
flush_now:	dc.h	flushCmd
	dc.h	0
	dc.w	0
quiet_now:	dc.h	quietCmd
	dc.h	0
	dc.w	0	
amp_command:	data
	dc.h	ampCmd
	dc.h	0	*Amplitude 0-255
	dc.w	0	*NU
*/
//extern SndChannelPtr sound_chan [12];
//extern	SndListHandle the_sounds[max_sounds];	//array of handles for sounds

#define Async	-1


