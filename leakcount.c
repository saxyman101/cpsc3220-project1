#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
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

	return 0;
}
