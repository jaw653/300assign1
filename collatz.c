/**
 * Author: Jake Wachs
 * Institution: The University of Alabama
 * Class: CS 301, Operating Systems
 * 16 September 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Error: not enough commandl line arguments given. Exiting.\n");
		return 0;
	}
	
	/* Convert cmdline arg 1 to int, save to mem */
	int inputNum = atoi(argv[1]);

	if (inputNum < 1) {
		printf("Error: number provided is not positive integer. Exiting.\n");
		return 0;
	}

	/* Initialize process variable, forks operation */
	pid_t process;
	process = fork();
	//process = -1;
	int status;

	/* fork() error checking */
	if (process < 0) {
		perror("fork() error");
		exit(-1);
	}

	/* If 0, in the child process */
	if (process == 0) {
		printf("%d", inputNum);
		if (inputNum != 1)
			printf(",");
													//FIXME: make sure spacing between commas and the newline at the end is correct

		while (inputNum != 1) {
			if (inputNum % 2 == 0)
				inputNum /= 2;							//If even, divide by 2
			else
				inputNum = (inputNum * 3) + 1;			//Else, carry out algo

			printf("%d", inputNum);
			if (inputNum != 1)
				printf(",");
		}
		
		printf("\n");
	}
	else
		wait(&status);

	return 0;
}