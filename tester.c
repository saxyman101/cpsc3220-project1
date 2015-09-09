#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int n=10;
	int *ptr = malloc(sizeof(int)*n);
	
	for (unsigned int i=0; i<n; i++) {
		ptr[i]=i;
	}
	
	for (unsigned int i=0; i<n; i++) {
		printf("%d: %d\n",i, ptr[i]);
	}
	
	int x=12;
	int *ptr2 = malloc(sizeof(int)*x);
	
	for (unsigned int i=0; i<x; i++) {
		ptr2[i]=i;
	}
	
	for (unsigned int i=0; i<x; i++) {
		printf("%d: %d\n",i, ptr2[i]);
	}
	
	free(ptr2);
	
	
	return 0;
}
