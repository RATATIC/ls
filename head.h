#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

//Functoin return list of files and folders
struct file_info** open_directory (char* path_dir, size_t* size);

//Function give info about file or directory
struct file_info* name_file_or_dir (char* path_dir, char* name);

//Function formats string size_s(file size)
char* format_size_s (size_t size_s);

//Function free memory 
void free_memory (struct file_info** catalog, size_t size);

//Function print directory
void ls_print (struct file_info** catalog, size_t size, FILE* point);