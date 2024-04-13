#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

int cd(char *path) {
    return chdir(path);
}

char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;
	
    if (command == NULL) {
        perror("malloc failed");
        exit(1);
	}
    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}




int main(void) {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;

    while (1) {
        input = readline("unixsh> ");
        command = get_input(input);

        child_pid = fork();
		
        if (child_pid < 0) {
            perror("Fork failed");
            exit(1);
		}
		
        if (strcmp(command[0], "cd") == 0) {
            if (cd(command[1]) < 0) {
                perror(command[1]);
            }

            /* Skip the fork */
            continue;
        }
		
		if (child_pid == 0) {
            /* Never returns if the call is successful */
            //execvp(command[0], command);
                        
			if (execvp(command[0], command) < 0) {
                perror(command[0]);
                exit(1);
            }
			
			printf("This won't be printed if execvp is successul\n");
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        free(input);
        free(command);
		
		
    }

    return 0;
}




