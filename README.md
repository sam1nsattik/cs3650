# cs3650

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

if (strcmp(tokens[0], "pwd") == 0) { // Check if the command is 'pwd'
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd); // Print the current working directory
            status = 0; // Assume success as per the instructions
        } else {
            // Even though you're assuming success, it's good practice to handle potential errors
            fprintf(stderr, "Error getting current working directory: %s\n", strerror(errno));
            status = 1; // Set status to 1 to indicate an error, contrary to the assumption
        }
    }

if (strcmp(tokens[0], "exit") == 0) { // Check if the command is 'exit'
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
