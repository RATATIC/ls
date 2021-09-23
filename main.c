/*
* @file main.c
* @author Renat Kagal <kagal@itspartner.net>
*
* Assembling : gcc main.c libsort.a -o main
*
* Description : program print name of files and directories , for files 
* print size in bytes , sortting files and directories 	
*
* Copyright (c) 2021, ITS Partner LLC.
* All rights reserved.
*
* This software is the confidential and proprietary information of
* ITS Partner LLC. ("Confidential Information"). You shall not
* disclose such Confidential Information and shall use it only in
* accordance with the terms of the license agreement you entered into
* with ITS Partner.
*/

#include "head.h"
#include "sort.h"

int main (int argc, char** argv) {
	char* path_out;
	char* path_dir;

	if (argc < 2) {
		path_out = "output.txt";
		path_dir = ".";
	}
	else if (argc < 3) {
		path_dir = argv[1];
		path_out = "output.txt";
	}
	else {
		path_dir = argv[1];
		path_out = argv[2];
	}

	struct file_info** catalog;
	size_t catalog_size = 0;
	
	catalog = open_directory (path_dir, &catalog_size);
	if(catalog == NULL)
		exit(EXIT_FAILURE);
	
	sort_files(catalog, catalog_size);
	
	ls_print(catalog, catalog_size, stdout);
	
	FILE *fp;
	if ( (fp = fopen(path_out, "wt")) == NULL) {
		puts("Failed open file");
		exit(EXIT_FAILURE);
	}
	ls_print(catalog, catalog_size, fp);
	
	if (fclose(fp)) {
		puts("Failed closing file");
		exit(EXIT_FAILURE);
	}
	
	free_memory(catalog, catalog_size);
}

//Functoin return list of files and folders
struct file_info** open_directory(char* path_dir, size_t* size) {
	struct file_info** catalog;	
	DIR *dir;
	struct dirent *ent;

	//open directory and save information about files and directories
	if ((dir = opendir(path_dir)) != NULL) {
		catalog = (struct file_info**)malloc(sizeof(struct file_info*));
		*size += 1;
		
		if (catalog == NULL) {
				puts("Failed allocate memory");
				return NULL;
		}
		// add in catalog new file or dir
		for (int i = 0; (ent = readdir(dir)) != NULL; i++) {
				if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
					i--;
				else {
					if (*size <= i) {
						catalog = realloc(catalog, ++(*size) * sizeof(struct file_info*));
				
						if (catalog == NULL) {
							puts("Failed allocate memory");
							return NULL;
						}
					}
					catalog[i] = name_file_or_dir(path_dir, ent->d_name);
				}
		}
		closedir(dir);
	}
	else {
		puts("Failed to open directory");
		return NULL;
	}
	return catalog;
}

//Function give info about file or directory
struct file_info* name_file_or_dir (char* path_dir, char* name) {
	struct stat buf;
	struct file_info* info = (struct file_info*)malloc(sizeof(struct file_info));

	char* path_to_file = NULL;
	if (asprintf(&path_to_file, "%s/%s", path_dir, name) == -1) {
		puts("asprintf err");
		exit(EXIT_FAILURE);
	}
	lstat(path_to_file, &buf);
	free(path_to_file);

	//checking bit and take information about files and directories
	if (S_ISREG(buf.st_mode)) {
		info->type = 'f';
		info->name = name;
		info->size = buf.st_size;
		info->size_s = format_size_s(buf.st_size);
	}
	else if (S_ISDIR(buf.st_mode)) {
		info->type = 'd';
		info->name = name;
	}
	else if (S_ISLNK(buf.st_mode)) {
		info->type = 'e';
		info->name = name;
	}
	return info;
}

//Function formats string size_s(file size)  1000000 to 1,000,000
char* format_size_s (size_t size) {
	int pos = 0;
	int sep_pos = 0;
	char buf[32] = {'\0'};
	
	while (size) {
		buf[pos++] = size % 10 + '0';
		size /= 10;
		if (!(++sep_pos % 3) && size)
			buf[pos++] = ',';
	}
	for (int i = 0; i < pos / 2; i++) {
		char temp = buf[i];
		buf[i] = buf[pos-i-1];
		buf[pos-i-1] = temp;
	}
	return strdup (buf);
}
//Function print directory
void ls_print (struct file_info** catalog, size_t size, FILE* point) {
	for (int i = 0; i < size; i++) {
		if (catalog[i]->type == 'f') {
			fprintf (point, "%-20s %-20s\n", catalog[i]->name, catalog[i]->size_s);
		}
		else if (catalog[i]->type == 'd') {
			fprintf (point, "%-20s\n", catalog[i]->name);
		}
		else if (catalog[i]->type == 'e') {
			fprintf (point, "Failed check : %-20s\n", catalog[i]->name);
		}
	}
}

//Function free memory 
void free_memory (struct file_info** catalog, size_t size) {
	for (int i = 0; i < size; i++) {
		if(catalog[i]->type == 'f')
			free(catalog[i]->size_s);
		free(catalog[i]);
	}
	free(catalog);
}