/**
 * Author: Jake Wachs
 * Institution: The University of Alabama
 * Class: CS 301, Operating Systems
 * 16 September 2018
 */

/**
 * Notes:
 * 		- Make sure history will only print 10 correct entries
 *
 * 		- Should incorrect commands such as "test" be added to history?
 *
 * 		- Final thing to do is add '&' functionality
 *
 * 		- Maybe change historyArr to only hold the most recent 10 cause prof could overload if lots of test cases
 *
 * 		- Make sure exit doesn't need to handle "Exit" as well
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80		// Max length command
char historyArr[10000][10000];		// Array to store history
int command_id = 1;					// FIXME: should it start at 1 or 0


// static void printHistory(char **, int);
static void tokenize(char *, char **);
static void execute(char **);

int main(void) {
	char *args[MAX_LINE/2 + 1];		// Command line arguments
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


		/* Keep a copy of the input string that's not tokenized for history conditional */
		char tmp[10000];
		strcpy(tmp, input);


		tokenize(input, args);


		if (strcmp(args[0], "exit") == 0)
			exit(0);

		else if (strcmp("history", args[0]) != 0 && strcmp("!!", args[0]) != 0
			&& args[0][0] != '!') {

				strcpy(historyArr[command_id], tmp);
				printf("copying in: %s\n", tmp);
				command_id += 1;
				execute(args);
		}

		else {
			if (strcmp("history", args[0]) == 0) {
				int index = command_id-1;
				//int x;
				int counter = 1;
				while (counter <= 10 && index > 0) {
					printf("%d %s\n", index, historyArr[index]);
					index -= 1;
					counter += 1;
				}
			}
			else if (strcmp("!!", args[0]) == 0) {
				char *old_args[MAX_LINE/2+1];

				if (command_id == 1) {
					printf("No commands in history.\n");
				}
				else {
					char full_cmd[10000];
					char full_cmd2[10000];

					strcpy(full_cmd, historyArr[command_id-1]);
					strcpy(full_cmd2, historyArr[command_id-1]);

					tokenize(full_cmd, old_args);
					
					execute(old_args);

					strcpy(historyArr[command_id], full_cmd2);
					command_id += 1;
				}
			}
			else if (args[0][0] == '!') {
				char *old_args[MAX_LINE/2+1];
				int index = args[0][1] - '0';

				if (index >= command_id || index < 1) {
					printf("No such command in history.\n");
				}
				else {
					char full_cmd[10000];
					char full_cmd2[10000];

					strcpy(full_cmd, historyArr[index]);
					strcpy(full_cmd2, historyArr[index]);

					tokenize(full_cmd, old_args);
					
					execute(old_args);

					strcpy(historyArr[command_id], full_cmd2);
					command_id += 1;
				}
			}
		}

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
 * Breaks @input into space character-separated strings
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

/**
 * Executes commands stored in args and creates child processe(s)
 * @args - command to be executed
 */
static void execute(char **args) {
	pid_t pid;
	int status;

	if ((pid = fork()) < 0) {
		perror("Error: fork() failed()\n");
		exit(1);
	}
	/* Child Process */
	else if (pid == 0) {
		if (execvp(*args, args) < 0) {
			perror("Error: execution failed\n");
			exit(1);
		}
	}
	else
		while (wait(&status) != pid);
}