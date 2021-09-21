#include "head.h"

int main(int argc, char** argv){

	char* path_out;

	if(argc < 2)
		path_out = "output.txt";
	else
		path_out = argv[1];
	
	char* path_dir_with_0 = NULL;
	size_t size = 0;
	
	getline(&path_dir_with_0, &size, stdin);
	puts("\n");
	
	char path_dir[strlen(path_dir_with_0) - 1];
	
	strncpy(path_dir, path_dir_with_0, strlen(path_dir_with_0) - 1);
	
	char** catalog;
	size_t catalog_size = 0;
	
	catalog = open_directory(path_dir, &catalog_size);
		
	if(catalog == NULL)
		exit(EXIT_FAILURE);
	
	for(int i = 0; i < catalog_size; i++){
		printf("%s\n", catalog[i]);
	}
	free_memory(catalog, catalog_size);
}

char** open_directory(char* path_dir, size_t* size){

	char** catalog;	
	DIR *dir;
	struct dirent *ent;

	if((dir = opendir(path_dir)) != NULL){
		catalog = (char**)malloc(sizeof(char*));
		*size += 1;
		
		if(catalog == NULL){
				puts("Failed allocate memory");
				return NULL;
		}
		for(int i = 0; (ent = readdir(dir)) != NULL; i++){
			if(*size <= i){
				catalog = realloc(catalog, ++(*size) * sizeof(char*) );
				
				if(catalog == NULL){
					puts("Failed allocate memory");
					return NULL;
				}
			}
			catalog[i] = name_file_or_dir(path_dir, ent->d_name);
		}
		closedir(dir);
	}
	else {
		puts("Failed to open directory");
		return NULL;
	}
	return catalog;
}

char* name_file_or_dir(char* path_dir, char* name)
{
	struct stat buf;
	char* file_info = (char*)malloc( (strlen(name)  + 20) * sizeof(char));
	{
	 	char path_to_file[strlen(path_dir) + strlen(name) + 1];
		sprintf(path_to_file, "%s/%s", path_dir, name);
		
		lstat(path_to_file, &buf);
	}

	if(S_ISREG(buf.st_mode)){
		sprintf(file_info, "f %s %ld", name, buf.st_size);
	}
	else if(S_ISDIR(buf.st_mode)){
		sprintf(file_info, "d %s", name);
	}
	else if(S_ISLNK(buf.st_mode)){
		sprintf(file_info, "error %s", name);
	}
	
	return file_info;
}

void free_memory(char** arr,int size){

	for(int i = 0; i < size; i++){
		free(arr[i]);
	}	
	free(arr);
}
