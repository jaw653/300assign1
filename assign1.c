/**
 * Author: Jake Wachs
 * Institution: The University of Alabama
 * Class: CS 301, Operating Systems
 * 16 September 2018
 */

/**
 * Notes:
 * 		- Must still add functionality for '&' w/ wait()
 *
 * 		- Must add history functionality that supports
 * 			1. osh>history (prints last ten commands executed)
 * 			2. osh>!! (executes most recent command)
 * 			3. osh! N (executes the Nth command in history)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80		// Max length command

int main(void) {
	char *args[MAX_LINE/2 + 1];		// Command line arguments
	int should_run = 1;				// program exit flag

	char input[10000];

	pid_t process;

	while (should_run) {
		printf("osh>");				// Print command line
		fflush(stdout);				// Flush input for same-line entry

		fgets(input, 10000, stdin);				// Scan entire line to input var
		input[strlen(input)-1] = '\0';			// Get rid of trailing '\n'


		char *str = strtok(input, " ");			// Split String by spaces
		int i;
		for (i = 0; str; i++) {
			//printf("%s\n", str);
			str[strlen(str)] = '\0';
			args[i] = str;						// Send tokens to args[]
			str = strtok(NULL, " ");
		}

		for (int j = i; j < MAX_LINE/2 + 1; j++) {	// Fill all remaining with NULL
			args[j] = NULL;
		}

		printf("args[i-1] is: %s\n", args[i-1]);

		process = fork();						// Create child process

		if (strcmp(args[i-1], "&") == 0) {
			wait(NULL);
		}

		if (strcmp(args[0], "exit") == 0) {
			exit(0);
		}
		else if (process < 0) {
			perror("fork() error.\n");
			exit(-1);
		}
		else if (process == 0) {				// Child process
			if (strcmp(args[i-1], "&") == 0)
				wait(NULL);

			if (strcmp(args[0], "exit") == 0) {
				exit(0);
			}
			execvp(args[0], args);
		}
		else {
			wait(NULL);
		}


/*

/*
		int j;									// testing for tokens
		for (j = 0; j < i; j++) {
			printf("%s\n", args[j]);
		}

		if (strcmp(args[0], "exit") == 0)
			exit(0);
		if (strcmp(args[i], "&") == 0)
			//invoke wait(&status)

		return 0;
		//pid_t process = fork();
		int process;
		if (process < 0) {
			fprintf(stderr, "Error: fork() issue.\n");
			exit(-1);
		}
		else if (process == 0) {				// Child process
			//invoke execvp()

		}
*/
/*
		if (strcmp(input, "exit") == 0) {
			printf("flag\n");
			should_run = 0;
			return 0;
		}
		
		pid_t process = fork();					// Create child process

		if (process < 0) {						// fork() error handling
			fprintf(stderr, "fork() error\n");
			exit(-1);
		}

		if (process == 0) {						// Child process
			printf("child process\n");

			if (strcmp(input, "exit") == 0) {
				should_run = 0;
				return 0;
			}
		}
		else									// Parent process
			wait(&status);
*/

		/**
		 * After reading user input, the steps are:
		 * (1) fork a child process using fork()
		 * (2) the child process will invoke execvp()
		 * (3) if command included &, parent will invoke wait()
		 */

		//execvp(args[0], args);
	}

	return 0;
}
