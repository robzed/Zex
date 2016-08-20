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

// bmp.cpp
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#error "This file is not in the Zex2005 build"

#include "bmp.h"
//expand bmp
//sb 8/12/98
//takes a bmp and expands it to dest
UInt16 int16_intel_to_ppc (UInt16 the_data);
UInt32 int32_intel_to_ppc (UInt32 the_data);

void expand_bmp (bmp_header *bmp_src,char *dest)
{
UInt32 i32,Mac32;
UInt16 i16,Mac16;

i32=(*bmp_src).bm_info.bmiHeader.biWidth;
Mac32 =int32_intel_to_ppc(i32);	//swap from intel to ppc

}


//convert from intel format to PPC for UInt16
UInt16 int16_intel_to_ppc (UInt16 the_data)
{
UInt8 reg1;
reg1=the_data>>8;	//upper byte
the_data=the_data<<8;	//move lower to upper
the_data|=reg1;	//or in lower byte
return the_data;
}
//convert from intel format to PPC for int32
UInt32 int32_intel_to_ppc (UInt32 the_data)
{
UInt32 reg1;
UInt32 reg2;
int i;

reg1=the_data;	//copy

for (i=0;i<3;i++)
{
the_data=the_data<<8;	//shift lsb up
reg1=reg1>>8;	//loose lsb
the_data|=(UInt8) reg1;
}

return the_data;
}


