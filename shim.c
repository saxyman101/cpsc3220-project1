#define _GNU_SOURCE
/*
void __attribute__ ((constructor)) free_init(void);
void __attribute__ ((destructor)) free_cleanup(void);

void __attribute__ ((constructor)) malloc_init(void);
void __attribute__ ((destructor)) malloc_cleanup(void);
*/

void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


FILE *fp;
int x;
const int SIZE = 1000;
void* ptrs[SIZE];

void (*original_free)(void *ptr) = NULL;
void* (*original_malloc)(size_t size);

void init() {
	original_free = dlsym(RTLD_NEXT, "free");
	original_malloc = dlsym(RTLD_NEXT, "malloc");
	fp = fopen("leakcount.txt", "w");
	

	for (unsigned int i = 0; i<SIZE; i++)
		ptrs[i] = NULL;
}

void cleanup() {
	fclose(fp);
}

void free(void *ptr) {
	int i=0;
	int done=0;
	while (ptrs[i] != NULL && !done) {
		if (ptrs[i] == ptr)
			done = 1;
		i++;	
	}
	i--;
	
	if (fp != NULL)
		if (i >= 0)
		fprintf(fp,"-%d ", i);
	original_free(ptr);
}

void* malloc(size_t size) {
	void* ptr = original_malloc(size);
	
	if (fp != NULL) {
		ptrs[x] = ptr;
		fprintf(fp,"%d ",(int)size);
		x++;
	}
	
	return ptr;
}
