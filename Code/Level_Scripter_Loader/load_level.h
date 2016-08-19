// load_level.h

#ifndef LOAD_LEVEL_H
#define LOAD_LEVEL_H

void load_level (int the_level);
void interpret_file(Ptr the_level);
void show_level_text();
//oldmac//void GetBGRAW(FSSpec* the_file);
void make_data_fsspec(const char* the_folder_name, const char* the_filename, FSSpec* the_fsspec);	//make fsspec of file name in zd3 folder


#include <string>
std::string get_saved_file_path();		// where should we save stuff


#endif // LOAD_LEVEL_H
