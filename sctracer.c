#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
	pid_t child;
	
	child = fork();
	
	if (child == 0) {
		// in child process
	
		execl("./stester", "./stester", NULL);
	}
	else {
		// in parent process

	}
	
	
	return 0;
}
