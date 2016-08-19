//texture_management.c
//SB 3/1/99
//manages Zex's textures for me cause I'm too lazy to do it myself.
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
//oldmac//#include <Files.h>

#include "3d_structures_def.h"
#include "texture_management.h"
#include "Error_handler.h"
#include "pictures.h"
#include "file_io.h"
#include "load_level.h"
#include "OGL.h"
#include    "fpoly4.h"
#include "genpoly_int.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "dlp.h"


#define MAX_TEXTURES 100
texture_slot the_textures [MAX_TEXTURES];	//our array of texture ptrs and ID's

int textures_index = 0;	//index into our array of textures
static GLuint texture_name = 0;



//returns an ogl name for a texture
int get_unique_texture_name()
{
texture_name++;
return texture_name;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 25 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void goto_next_texture_filename(char**file_list_ptr, int *file_size_remaining)
{
	while(*file_size_remaining!=0)		// ensure there is file remaining
	{
		// search for the end of line
		if(**file_list_ptr==13 || **file_list_ptr==10)
		{
			while(*file_size_remaining!=0 && (**file_list_ptr==13 || **file_list_ptr==10))
			{
				// get past all end of line characters
				(*file_list_ptr)++;
				(*file_size_remaining)--;
			}
			// break out - it's the beginning of the next line
			break;

		}
		
		// skip characters in the old filename
		(*file_list_ptr)++;
		(*file_size_remaining)--;
	}
	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 25 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

int get_texture_filename(const char *file_list_ptr, int file_size_remaining)
{
	int filename_len = 0;
	
	while(file_size_remaining!=0)		// ensure there is file remaining
	{
		// search for the first end of line
		if(*file_list_ptr==13 || *file_list_ptr==10 || *file_list_ptr==0)	// notice: the read_file ensure a zero terminated file - so we might get a zero terminated filename
		{
			// break out - it's the end of the word
			break;
		}
		
		filename_len++;
		file_list_ptr++;
		file_size_remaining--;
	}
	
	return filename_len;
}



//load all textures from the textures folder/resource
int load_textures()
{
FSSpec texture_folder_FSSpec;
FSSpec the_texture_FSSpec;

extern FSSpec Zex_FSSpec;
//oldmac//CInfoPBRec pb;
//oldmac//DirInfo *dpb=(DirInfo*) &pb;
//oldmac//HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
int folder_DirID;
Str255 the_file_name;
int return_value;
int get_file_err = 0;
short file_index=1;
//get the textures folder
//	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"Textures",&texture_folder_FSSpec);
////get the folders dirctory ID
//     dpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
//     dpb->ioDrDirID = texture_folder_FSSpec.parID;
//     dpb->ioNamePtr = texture_folder_FSSpec.name;
//     dpb->ioFDirIndex=0;	  //query
//     return_value=PBGetCatInfo(&pb,0);
//     if (return_value) return return_value;
//     folder_DirID=dpb->ioDrDirID;
////now get all the files in the folder
//       glInitNames();
        texture_name=1;

// new code (since SDL conversion) to read a list of textures as a file into 
// memory, then read these files as textures. This avoids us having to do 
// platform specific directory code.
		FSSpec texture_list_FSSpec;
		make_data_fsspec("T", "texture.lst", &texture_list_FSSpec);	// the list file
		Handle texture_filelist_handle = read_file(texture_list_FSSpec);
		char* texture_filelist_ptr = *texture_filelist_handle;		// de-reference handle
		int texture_filelist_size = GetHandleSize(texture_filelist_handle);	// get size of file
		
		char c_filename[255];
		// we now have the texture file list in memory - so we use a function to step through the memory and return filenames
		int filename_size = get_texture_filename(texture_filelist_ptr, texture_filelist_size);
		while(filename_size != 0)
		{
			int i;
			for(i=0; i<filename_size; i++)
			{
				c_filename[i] = texture_filelist_ptr[i];			// copy the filename text
			}
			c_filename[i] = 0;
			
			make_data_fsspec("T", c_filename, &the_texture_FSSpec);
			load_texture(the_texture_FSSpec);	//in texture_management.c funnily enough    

			texture_name++;		// one more texture loaded successfully

			// get next filename
			goto_next_texture_filename(&texture_filelist_ptr, &texture_filelist_size);
			filename_size = get_texture_filename(texture_filelist_ptr, texture_filelist_size);
		}

		return 0;			// no error to return
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
//get the objects' folder
	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"ZD3",&texture_folder_FSSpec);
//get zd3's dirctory ID
     dpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = texture_folder_FSSpec.parID;
     dpb->ioNamePtr = texture_folder_FSSpec.name;
     dpb->ioFDirIndex=0;	//query
     return_value=PBGetCatInfo(&pb,0);
     folder_DirID=dpb->ioDrDirID;

return_value=0;

//find sub folder
//      fpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = "T";
     fpb->ioFDirIndex=0;	//query the file name

     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
     report_error ("load_textures couldn't find a folder named:","T",return_value);
     return return_value;
     }
     folder_DirID=dpb->ioDrDirID;

return_value=0;
while (get_file_err==0)
 {
      fpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
      fpb->ioDirID= folder_DirID;
//     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = the_file_name;
     fpb->ioFDirIndex=file_index;	//gimmee the file name
     get_file_err=PBGetCatInfo(&pb,0);
     if(get_file_err) return return_value;
//now load it
     if (fpb->ioFlFndrInfo.fdType=='RAW ')	//dont load those pesky icons...
     {
      FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,the_file_name,&the_texture_FSSpec);
      load_texture(the_texture_FSSpec);	//in texture_management.c funnily enough    
      texture_name++;
     }
     file_index++;	 //darn't ferget this un Stu (like you did...)
 }
return return_value;
#endif //DISABLED_BY_ROB_WHEN_GETTING_RUNNING
}

//load a texture given the fsspec of the file containing the texture in RAW format
#if OGL==1
//ogl version which gets an OGL name for each texture
void load_texture(FSSpec the_texture_file)
{
unsigned long insert_id;
Handle temp_handle;
Handle tempb_handle;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
Ptr raw_data;
Ptr raw_rgba_data;
int err;
int draw_code;
extern int OGL_active;

//make ID from first 4 chars as a UInt32
insert_id=(the_texture_file.name[1]<<24) + (the_texture_file.name[2]<<16) +
(the_texture_file.name[3]<<8) + the_texture_file.name[4]; 
if (the_texture_file.name[3]=='&') draw_code=1;
else draw_code=0;

//load file
temp_handle=read_file(the_texture_file);
if (temp_handle==0) report_error ("Texture Management: Load_Texture: Zero Handle returned from read_file. Out of Memory.","",8);
LS_HLock(temp_handle);	//bit important this...
the_raw = (LSRAW*)*temp_handle;	//dereference
//now convert from 8 bit clut to rgba
tempb_handle=NewHandleClear((128*128*4)+8);
if (tempb_handle==0) report_error ("Texture Management: Load_Texture: Zero Handle returned from read_file. Out of Memory.","",8);
LS_HLock(tempb_handle);	//bit important this...
the_raw_rgba = (LSRAW_RGBA*)*tempb_handle;	//dereference

raw_data=(char *)&the_raw->raw_data;
raw_rgba_data=(char *)&the_raw_rgba->raw_data;


//check for OGL 32 bit textures
//if (the_texture_file.name[1]=='!')
//convert_to_rgba_from_rgbp(raw_data,raw_rgba_data,draw_code);
//else
convert_to_rgba(raw_data,raw_rgba_data,draw_code);

raw_data=(char *)&the_raw->raw_data;
raw_rgba_data=(char *)&the_raw_rgba->raw_data;


the_textures[textures_index].the_raw_data=raw_data;	//8 bit data for static screens
the_textures[textures_index].the_raw_rgba_data=raw_rgba_data;	//rgba data for OGL
the_textures[textures_index].the_handle=temp_handle;
the_textures[textures_index].the_rgb_handle=tempb_handle;
the_textures[textures_index].id=insert_id;

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glPixelStorei(GL_PACK_ALIGNMENT, 1); 
//glGenTextures(1,&texture_name);
the_textures[textures_index].gl_text_name=texture_name;


// LS_DisposeHandle(temp_handle);	  //delete our temp mem - ogl has the texture now

textures_index++;
if (textures_index>MAX_TEXTURES) report_error ("Texture_Management:Load_Texture: Too many textures.","",7);
}

#else
//non ogl version
void load_texture(FSSpec the_texture_file)
{
unsigned long insert_id;
Handle temp_handle;
LSRAW * the_raw;
Ptr raw_data;

//check and discard OGL 32 bit textures
if (the_texture_file.name[1]=='!') return;

//make ID from first 4 chars as a UInt32
insert_id=(the_texture_file.name[1]<<24) + (the_texture_file.name[2]<<16) +
(the_texture_file.name[3]<<8) + the_texture_file.name[4]; 

//load file
temp_handle=read_file(the_texture_file);
if (temp_handle==0) report_error_c ("Texture Management: Load_Texture: Zero Handle returned from read_file. Out of Memory.","",8);
LS_HLock(temp_handle);	//bit important this...
the_raw = (LSRAW*)*temp_handle;	//dereference
raw_data=(char *)&the_raw->raw_data;

the_textures[textures_index].the_raw_data=raw_data;
the_textures[textures_index].id=insert_id;
the_textures[textures_index].the_handle=temp_handle;
textures_index++;
if (textures_index>MAX_TEXTURES) report_error_c ("Texture_Management:Load_Texture: Too many textures.","",7);
}
#endif

#if OGL==1

void load_ogl_textures()
{
int i,err;

if (OGL_active==1)
 {
  for (i=0;i<textures_index;i++)
  {
  
  glBindTexture(GL_TEXTURE_2D, the_textures[i].gl_text_name);	//we supply the name...


  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA,
  GL_UNSIGNED_BYTE, the_textures[i].the_raw_rgba_data);
 
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   err=aglGetError();
   if (err!=0) report_error ("Texture_Management:load_ogl_textures: AGL error.","",err);
  }
 }
}
#endif

//converts 256 colour raw to rgba quad format
//draw code=0=normal
//1=255 is transparent and all others alpha is 50%
//2=255 is transparent
void convert_to_rgba(char * raw_data, char * raw_rgba_data, int draw_code)
{
int line_count, pixel_count;
unsigned char the_pixel;
int red,green,blue;

for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
     the_pixel=*raw_data;
     raw_data++;
     convert_to_rgb(the_pixel, &red, &green, &blue);
     *raw_rgba_data=red;
     raw_rgba_data++;
     *raw_rgba_data=green;
     raw_rgba_data++;
     *raw_rgba_data=blue;
     raw_rgba_data++;
     if (draw_code==0)
     {
      *raw_rgba_data=255;	//opaque
     }
     else     
     if (draw_code==2)
     {
      if (the_pixel==255)
      *raw_rgba_data=0;	//transparent
      else
      *raw_rgba_data=255;	//opaque
      
     }
     else
     if (draw_code==1)
       {
        if (the_pixel==255)
        *raw_rgba_data=0;	//transparent
        else
        *raw_rgba_data=128;	//translucent
       }
     
     raw_rgba_data++;

     }
  }
}


void convert_to_rgba_wh(char * raw_data, char * raw_rgba_data, int draw_code,int w, int h)
{
int line_count, pixel_count;
unsigned char the_pixel;
int red,green,blue;

for (line_count=0; line_count<h; line_count++)
  {
   for (pixel_count=0; pixel_count<w; pixel_count++)
     {
     the_pixel=*raw_data;
     raw_data++;
     convert_to_rgb(the_pixel, &red, &green, &blue);
     *raw_rgba_data=red;
     raw_rgba_data++;
     *raw_rgba_data=green;
     raw_rgba_data++;
     *raw_rgba_data=blue;
     raw_rgba_data++;
     if (draw_code==0)
     {
      *raw_rgba_data=255;	//opaque
     }
     else     
     if (draw_code==2)
     {
      if (the_pixel==255)
      *raw_rgba_data=0;	//transparent
      else
      *raw_rgba_data=255;	//opaque
      
     }
     else
     if (draw_code==1)
       {
        if (the_pixel==255)
        *raw_rgba_data=0;	//transparent
        else
        *raw_rgba_data=128;	//translucent
       }
     
     raw_rgba_data++;

     }
  }
}


//converts 256 colour raw to rgba quad format
//colour 255 is transparent
void convert_to_rgba_trans(char * raw_data, char * raw_rgba_data)
{
int line_count, pixel_count;
unsigned char the_pixel;
int red,green,blue;

for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
     the_pixel=*raw_data;
     raw_data++;
     convert_to_rgb(the_pixel, &red, &green, &blue);
     
     *raw_rgba_data=red;
     raw_rgba_data++;
     *raw_rgba_data=green;
     raw_rgba_data++;
     *raw_rgba_data=blue;
     raw_rgba_data++;

     if (the_pixel==255)
     *raw_rgba_data=0;	//alpha
     else
     *raw_rgba_data=255;	//alpha
     raw_rgba_data++;

     }
  }
}

//unloads all the textures
void unload_textures()
{
while (textures_index>=0)
  {
  textures_index--;
  LS_DisposeHandle (the_textures[textures_index].the_handle);

#if OGL==1
  LS_DisposeHandle (the_textures[textures_index].the_rgb_handle);
#endif
   
  }
#if OGL==1
 
if (OGL_active==1)
 {
 GLuint the_name;
 int i;
 
 for (i=0;i<textures_index;i++)
  {
   the_name=the_textures[i].gl_text_name;
   glDeleteTextures(1,&the_name);
  }
  
 }	//end of OGL active
#endif

textures_index=0;	//reset
}

//given an id, returns the ptr to the raw data OR 0
texture_slot* find_texture_ptr(unsigned long wanted_texture_id)
{
int i=0;
while (i<textures_index)
  {
  if (the_textures[i].id==wanted_texture_id) 
   {
   return &the_textures[i];

   }
  i++;
  }
return 0;
}



//memory for dynamic textures for planets etc



dynamic_texture dynamic_textures_block [MAX_DYN_TEXTURES];

void init_dynamic_textures()
{
int i;
for (i=0;i<MAX_DYN_TEXTURES;i++) 
 {
 dynamic_textures_block[i].in_use=0;
 dynamic_textures_block[i].name=-1;	//for ogl
 }
}




int kill_dynamic_texture(dynamic_texture * the_tex_ptr)
{
int i;
GLuint the_name;

for (i=0;i<MAX_DYN_TEXTURES;i++) 
  {
        if (&dynamic_textures_block[i]==the_tex_ptr) 
         {
#if OGL==1
        
         //tell ogl to ditch the texture
         the_name=dynamic_textures_block[i].name;
         glDeleteTextures(1,&the_name);
#endif
         dynamic_textures_block[i].in_use=0;
         dynamic_textures_block[i].name=-1;
         return 0;
         }
  }

return -1;
}

//returns either the ptr to storage or 0
dynamic_texture * get_memory_for_dyn_tex()
{
int i;
for (i=0;i<MAX_DYN_TEXTURES;i++) 
  {
        if (dynamic_textures_block[i].in_use==0) 
        {
          dynamic_textures_block[i].in_use=1;
          dynamic_textures_block[i].name=get_unique_texture_name();	//for ogl
          return &dynamic_textures_block[i];
        }
  }

return 0;

}

#if OGL==1
//011199 SB
void load_ogl_dynamic_textures()
{
int i;
for (i=0;i<MAX_DYN_TEXTURES;i++) 
  {
        if (dynamic_textures_block[i].in_use!=0)	//found one in use 
        {
           make_ogl_dynamic_texture(&dynamic_textures_block[i]);
        }
  }

}

void make_ogl_dynamic_texture(dynamic_texture* dyn_tex_block_ptr)
{
Handle temp_rgbaH;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
Ptr raw_data;
Ptr raw_rgba_data;
int err;

   err=aglGetError();
   if (err!=0) report_error ("Texture_Management:make_ogl_dynamic_texture(1): AGL error.","",err);

//from the 8 bit data make rgba data and bind it to name
temp_rgbaH=NewHandle(128*128*4);



if (temp_rgbaH==0)
   {
          add_to_text_display("Zex Exec> WARNING: OGL: no memory for RGBA dyn_texture",1);
          dyn_tex_block_ptr->name=0;	//not a valid name
   }
else
   {
     LS_HLock(temp_rgbaH);
    
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 128);
    glPixelStorei(GL_PACK_ROW_LENGTH, 128);

     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     glPixelStorei(GL_PACK_ALIGNMENT, 1); 

     raw_rgba_data = *temp_rgbaH;	//dereference

     raw_data=(char *)&dyn_tex_block_ptr->tex_data;
//     draw_code=dyn_tex_block_ptr->draw_code;

     convert_to_rgba_trans(raw_data,raw_rgba_data);

     raw_rgba_data = *temp_rgbaH;	//dereference


     glBindTexture(GL_TEXTURE_2D, dyn_tex_block_ptr->name);	//we supply the name...


     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA,
     GL_UNSIGNED_BYTE, raw_rgba_data);
 
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   err=aglGetError();
   if (err!=0) report_error ("Texture_Management:make_ogl_dynamic_texture(2): AGL error.","",err);


     
     LS_DisposeHandle(temp_rgbaH);
   }
   
}
#endif



