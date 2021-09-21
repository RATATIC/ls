#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

//Functoin return list of files and folders
char** open_directory(char*, size_t*);

char* name_file_or_dir (char* , char* );

void free_memory(char** , int);
