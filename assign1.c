/**
 * Author: Jake Wachs
 * Institution: The University of Alabama
 * Class: CS 301, Operating Systems
 * 16 September 2018
 */

/**
 * Notes:
 * 		- Exit functionality still not working
 *			1. currently only doesn't work when run right after the history function
 *			2. make sure it handles 'exit' and 'Exit'
 *			3. when you misspell test after typing it twice the program hangs up. If you type during the hang then attempt to exit later it takes 2 tries
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
char historyArr[10000][10000];		// Array to store history
int command_id = 1;					// FIXME: should it start at 1 or 0


static void printHistory(char **, int);
static void tokenize(char *, char **);

int main(void) {
	char *args[MAX_LINE/2 + 1];		// Command line arguments
	char *most_recent_args[MAX_LINE/2 + 1];		// For saving the last cmd executed

	int should_run = 1;				// program exit flag


	char input[10000];

	pid_t process;

	while (should_run) {
		/* Print command line, flush input for same-line entry */
		printf("osh>");
		fflush(stdout);


		/* Scan entire line to input var, get rid of trailing '\n' */
		fgets(input, 10000, stdin);
		input[strlen(input)-1] = '\0';

		
		/* Store command in historyArr, increment size counter */
		if (strcmp("history", input) != 0 && strcmp("!!", input) != 0
			&& strcmp("!", input) != 0) {
			// printf("adding %s to historyArr\n", input);  // testing purposes
			strcpy(historyArr[command_id], input);
			command_id += 1;
		}


		/* Split string by spaces, send tokens to args[] var */
		char *str = strtok(input, " ");
		int i;
		for (i = 0; str; i++) {
			//printf("%s\n", str);
			str[strlen(str)] = '\0';
			args[i] = str;
			str = strtok(NULL, " ");
		}


		/* Fill all remaining entries in args[] arr w/ NULL */
		for (int j = i; j < MAX_LINE/2 + 1; j++)
			args[j] = NULL;


		/* Create child process */
		process = fork();

		if (strcmp(args[i-1], "&") == 0) {			// FIXME: need to do add this functionality
			wait(NULL);
		}

		/* Catch exit before executing anything */
		if (strcmp(args[0], "exit") == 0) {
			should_run = 0;
			exit(0);
		}
		else if (process < 0) {
			perror("fork() error.\n");
			exit(-1);
		}

		else if (process == 0) {
			/* Child process */

			if (strcmp(args[i-1], "&") == 0)
				wait(NULL);

			if (strcmp(args[0], "exit") == 0) {
				should_run = 0;
				exit(0);
			}
			else if (strcmp(args[0], "history") == 0) {
				int index = command_id;
				int x;
				for (x = command_id-1; x >= 1; x--)
					printf("%d %s\n", --index, historyArr[x]);
			}
			else if (strcmp(args[0], "!!") == 0) {
				/* Execute most recent command in history */
				char *old_args[MAX_LINE/2+1];
				tokenize(historyArr[command_id-1], old_args);
				execvp(old_args[0], old_args);
			}
			else if (strcmp(args[0], "!") == 0) {
				/* Execute n'th command in history */
				char *old_args[MAX_LINE/2+1];
				int index = atoi(args[1]);

				tokenize(historyArr[index], old_args);
				execvp(old_args[0], old_args);
			}

			execvp(args[0], args);
		}
		else
			wait(NULL);
/*
		//copy args into most recent args
		for (j = 0; j < i; j++) {
			strcpy(args[j], most_recent_args[j]);
		}
*/
		/**
		 * After reading user input, the steps are:
		 * (1) fork a child process using fork()
		 * (2) the child process will invoke execvp()
		 * (3) if command included &, parent will invoke wait()
		 */
	}

	return 0;
}

/*
static void printHistory(char **commandHistory, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printf("%s\n", commandHistory[i]);
	}

	return;
}
*/

/**
 * Break @input into space character-separated strings
 * @input - full command entered at prompt
 * @arr - array of separeted strings
 */
static void tokenize(char *input, char *arr[]) {
	char *str = strtok(input, " ");
	int i;
	for (i = 0; str; i++) {
		//printf("%s\n", str);
		str[strlen(str)] = '\0';
		arr[i] = str;
		str = strtok(NULL, " ");
	}


	/* Fill all remaining entries in args[] arr w/ NULL */
	int j;
	for (j = i; j < MAX_LINE/2 + 1; j++)
		arr[j] = NULL;

	return;
}