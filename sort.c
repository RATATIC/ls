#include <string.h>
#include "sort.h"

#define SWAP(a, b) \
	( {	typeof(a) temp = a; 	 \
		a = b; 	 \
		b = temp; } )

//Sorting files and directories
int sort_files (struct file_info** catalog, size_t size) {

	for (int i = 0; i < size - 1; i++)
		for (int j = i + 1; j < size; j++) {
			if (catalog[j]->type == 'd') {
				if (catalog[i]->type != 'd')
					SWAP(catalog[i], catalog[j]);
				else
					if (strcmp(catalog[i]->name, catalog[j]->name) > 0)
						SWAP(catalog[i], catalog [j]);
			} else 
				if (catalog[j]->type == 'f' && catalog[i]->type != 'd'){
					if (catalog[i]->type != 'f')
						SWAP (catalog[i], catalog[j]);
					else
						if(catalog[j]->size > catalog[i]->size)
							SWAP(catalog[i], catalog[j]);
				}
		}
}