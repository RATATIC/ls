struct file_info{	
	char type;
	char* name;
	size_t size;
	char* size_s;
};

//Functoin sorting files and directories
void  sort_files(struct file_info** catalog, size_t size);