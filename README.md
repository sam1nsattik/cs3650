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
