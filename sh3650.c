/*
 * file:        shell3650.c
 * description: skeleton code for a simple shell
 *
 * Peter Desnoyers, Northeastern Fall 2023
 */

/* <> means don't check the local directory */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>

/* "" means check the local directory */
#include "parser.h"

/* you'll need these includes later: */
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>


int main(int argc, char **argv)
{
    int status;
    char qbuf[16]; // Buffer to hold the exit status string
    bool interactive = isatty(STDIN_FILENO); /* see: man 3 isatty */
    FILE *fp = stdin;
    signal(SIGINT, SIG_IGN);

    if (argc == 2) {
        interactive = false;
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
            exit(EXIT_FAILURE); /* see: man 3 exit */
        }
    }
    if (argc > 2) {
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char line[1024], linebuf[1024];
    const int max_tokens = 32;
    char *tokens[max_tokens];

    /* loop:
     *   if interactive: print prompt
     *   read line, break if end of file
     *   tokenize it
     *   print it out <-- your logic goes here
     */
    while (true) {
        if (interactive) {
            /* print prompt. flush stdout, since normally the tty driver doesn't
             * do this until it sees '\n'
             */
            printf("sh3650> ");
            fflush(stdout);
        }

        /* see: man 3 fgets (fgets returns NULL on end of file)
         */
        if (!fgets(line, sizeof(line), fp))
            break;

        /* read a line, tokenize it, and print it out
         */
        int n_tokens = parse(line, max_tokens, tokens, linebuf, sizeof(linebuf));

	char cwd[PATH_MAX];

        /* replace the code below with your shell:
         */
        if (strcmp(tokens[0], "cd") == 0) { // Check if the command is 'cd'
       	 if (n_tokens == 1) { // No arguments, go to HOME
            char *home = getenv("HOME");
            if (chdir(home) != 0) {
                fprintf(stderr, "cd: %s\n", strerror(errno));
                status = 1;
            }
       	 } else if (n_tokens == 2) { // One argument, go to specified directory
            if (chdir(tokens[1]) != 0) {
                fprintf(stderr, "cd: %s\n", strerror(errno));
                status = 1;
            }
         } else { // Wrong number of arguments
            fprintf(stderr, "cd: wrong number of arguments\n");
            status = 1;
         }
    	}
        else if (strcmp(tokens[0], "pwd") == 0) { // Check if the command is 'pwd'
	  if (n_tokens > 1) {
	    fprintf(stderr, "pwd: too many arguments\n");
	    status = 1;
	  }
          else if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd); // Print the current working directory
            status = 0; // Assume success as per the instructions
          } else {
            // Even though you're assuming success, it's good practice to handle potential errors
            fprintf(stderr, "Error getting current working directory: %s\n", strerror(errno));
            status = 1; // Set status to 1 to indicate an error, contrary to the assumption
          }
    	}

	else if (strcmp(tokens[0], "exit") == 0) { // Check if the command is 'exit'
       	 if (n_tokens == 1) {
            exit(0); // No arguments, exit with status 0
       	 } else if (n_tokens == 2) {
            int exit_status = atoi(tokens[1]); // Convert the argument to an integer
            exit(exit_status); // Exit with the provided status
       	 } else {
            fprintf(stderr, "exit: too many arguments\n");
            status = 1; // Set status to 1 to indicate an error
       	 }
	}

	else {
		pid_t pid = fork();

		if (pid == -1) {
		    // Fork failed
		    fprintf(stderr, "Fork failed: %s\n", strerror(errno));
		    exit(EXIT_FAILURE);
		} else if (pid == 0) {
		    // Child process
		    // Re-enable ^C (SIGINT)
		    signal(SIGINT, SIG_DFL);
		
		    // Execute the command
		    if (execvp(tokens[0], tokens) == -1) {
		        fprintf(stderr, "%s: %s\n", tokens[0], strerror(errno));
		        exit(EXIT_FAILURE);
		    }
		} else {
		    // Parent process
		    int status;
		    do {
		        waitpid(pid, &status, WUNTRACED);
		    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
		
		    status = WEXITSTATUS(status);
		    // Optionally, use exit_status for something
		}
	}

	for (int i = 0; i < n_tokens; i++) {
	    if (strcmp(tokens[i], "$?") == 0) {
		sprintf(qbuf, "%d", status); // Convert the status to a string
	        tokens[i] = qbuf; // Replace "$?" with the exit status string
	    }
	}
    }

    if (interactive)            /* make things pretty */
        printf("\n");           /* try deleting this and then quit with ^D */
}
