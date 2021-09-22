#include <string.h>
#include "sort.h"

#define swap(a, b) \
	({	typeof(a) temp = a; 	 \
		a = b; 	 \
		b = temp; })

//Sorting files and directories
void sort_files(struct file_info** catalog, size_t size){

	for(int i = 0; i < size - 1; i++)
		for(int j = i + 1; j < size; j++){
			if(catalog[j]->type == 'd'){
				if(catalog[i]->type != 'd')
					swap(catalog[i], catalog[j]);
				else
					if(strcmp(catalog[i]->name, catalog[j]->name) > 0)
						swap(catalog[i], catalog [j]);
			} else 
				if(catalog[j]->type == 'f' && catalog[i]->type != 'd'){
					if(catalog[i]->type != 'f')
						swap(catalog[i], catalog[j]);
					else
						if(catalog[j]->size > catalog[i]->size)
							swap(catalog[i], catalog[j]);
				}
		}
}
