#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
	if (argc<2) {
		fprintf(stderr, "Need to enter a executable file\n");
		exit(1);
	}

	
	pid_t p;	
	
	char *args[argc];	
	for (unsigned int i = 0; i <argc-1; i++) {
		args[i] = argv[i+1];
	}
	args[argc-1] = NULL;
	

	char *const envp[] = {"LD_PRELOAD=./memory_shim.so", NULL};
	
	// start program and wait till done to print results
	if ((p=fork())==0) {	
		execvpe(argv[1], args, envp);
	}
	waitpid(p, NULL, 0);
	
	FILE *fp = fopen("leakcount.txt", "r");

	int token;
	int mallocs[1000];
	int i=0;

	while (fscanf(fp, "%d", &token) == 1) {
		if (token > 0) {
			mallocs[i] = token;
			i++;
		}
		else
			mallocs[token*-1] = 0;
	}
	
	int sumMallocs = 0;
	int count = 0;
	for (int n=0; n<i; n++) {
		if (mallocs[n] != 0) {
			count++;
			printf("LEAK\t%d\n", mallocs[n]);
			sumMallocs += mallocs[n];
		}
	}
	fprintf(stderr,"TOTAL\t%d\t%d\n", count, sumMallocs);
	
	return 0;
}
