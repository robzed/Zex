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


