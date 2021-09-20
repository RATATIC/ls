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
	
	char path_dir[strlen(path_dir_with_0) - 1];
	
	strncpy(path_dir, path_dir_with_0, strlen(path_dir_with_0) - 1);
	
	char** catalog;
	
	catalog = open_directory(path_dir);
		
	if(catalog == NULL)
		exit(EXIT_FAILURE);
	
	for(int i = 0; i < sizeof(catalog); i++)
		printf("%s\n", catalog[i]);
}

char** open_directory(char* path_dir){

	char** catalog ;	
	
	DIR *dir;
	struct dirent *ent;

	if((dir = opendir(path_dir)) != NULL){
		catalog = (char**)malloc(sizeof(char*));
		size_t size = 1;
		
		if(catalog == NULL){
			puts("Failed allocate memory");
			return NULL;
		}
		
		for(int i = 0; (ent = readdir(dir)) != NULL; i++){
			if(size == i)
				catalog = realloc(catalog, size * 2);
			
			catalog[i] = ent->d_name;
		}
		closedir(dir);
	}
	else {
		puts("Failed to open directory");
		return NULL;
	}
	return catalog;
}
