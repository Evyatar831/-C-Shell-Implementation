#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <fcntl.h>  
#include <unistd.h>  


#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

int main() {
    char cmd[MAX_CMD_LEN];
    char* args[MAX_ARGS];
    char* token, * path, * path_copy;
    int status;

    while (1) {
        printf("$ ");  // Print prompt
        fgets(cmd, MAX_CMD_LEN, stdin);  // Get command from user

        // Check if the command is "leave"
        if (strcmp(cmd, "leave\n") == 0)
        {
            break;
        }

        cmd[strlen(cmd) - 1] = '\0';


        // Tokenize the input and store command and arguments in args array
        int arg_count = 0;
        args[arg_count] = strtok(cmd, " ");

        while (args[arg_count] != NULL) {
            arg_count++;
            if (arg_count == MAX_ARGS) {
                printf("Error: Too many arguments\n");
                break;
            }
            args[arg_count] = strtok(NULL, " ");
        }

        // Get PATH environment variable
        path = getenv("PATH");
        path_copy = strdup(path);

        // Iterate through directories in PATH
        token = strtok(path_copy, ":");
        while (token != NULL) {
            char full_path[MAX_CMD_LEN + 1];
            snprintf(full_path, sizeof(full_path), "%s/%s", token, args[0]);

            pid_t pid = fork();
            if (pid == 0) {
                // Child process
                 // Redirect stderr to /dev/null
                int devnull = open("/dev/null", O_WRONLY);
                dup2(devnull, STDERR_FILENO);
                close(devnull);

                execv(full_path, args);
                exit(EXIT_FAILURE);        // If execv returns, there was an error

            }
            else if (pid > 0) {
                // Parent process
                int status;
                waitpid(pid, &status, 0);

                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    // Command executed successfully
                    break;
                }

            }
            else {
                // Fork failed
                perror("fork");
                exit(EXIT_FAILURE);
            }

            token = strtok(NULL, ":");
        }

        free(path_copy);

        if (token == NULL) {
            printf("Error: File not found.\n");
        }
    }

    return 0;
}
