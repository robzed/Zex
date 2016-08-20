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


