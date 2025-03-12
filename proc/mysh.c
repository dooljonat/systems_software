#include <stdio.h>
#include <unistd.h>    // fork()
#include <stdlib.h>    // exit()
#include <inttypes.h>  // intmax_t
#include <sys/wait.h>  // wait()
#include <string.h>    // strcspn(), strtok
#include <errno.h>     // ECHILD
#include <fcntl.h>     // O_RDONLY, open

// Program to emulate a shell using syscalls


// NOTES:
// Redirection
// * There should only be one redirection of each kind at the most
// Piping
//  * Only a single pipe is required to be supported

int main() {
    // Init variables for obtaining user input
    const int BUFSIZE = 1024;
    char *input;

    // Get user input while no error / input
    while (1) {
        // Init variables for commands / arguments
        char *commands[3];
        char *arguments[5];
        int narguments;
        int ncommands = 0;
        int i, j;

        // Init variables for piping/forking/executing
        int pipefd[2];
        int pid;
        int exec_retval;

        input = malloc(sizeof(char) * BUFSIZE);

        // Prompt user
        printf("$ ");

        // Read in command with maximum size
        if (!fgets(input, BUFSIZE, stdin)) {
            // Use Ctrl - D to end the input
            exit(EXIT_SUCCESS);
        }

        // Remove newline
        input[strcspn(input, "\n")] = '\0';

        // Continue if no input was given
        if (strlen(input) == 0) {
            continue;
        }

        // Tokenize the input string by |
        for (commands[ncommands] = strtok(input, "|"); 
            commands[ncommands] != NULL;
            commands[++ncommands] = strtok(NULL, "|"));

        // Initialize the pipe
        //  ( Only if there is more than one command )
        if (ncommands > 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // Process the commands
        for (i = 0; i < ncommands; i++) {

            // Process command arguments
            narguments = 0;
            for (arguments[narguments] = strtok(commands[i], " ");
                arguments[narguments] != NULL;
                arguments[++narguments] = strtok(NULL, " "));

            // Check for output OR input redirection
            char *output_file = NULL;
            char *input_file  = NULL;

            for (j = 0; j < narguments; j++) {
                
                // Check for > character (OUTPUT)
                if (strchr(arguments[j], '>')) {
                    output_file = (arguments[j]+1);

                    // If there was output redirection, we know that the
                    // output file is located at the last ARG
                    // so we need to ignore it when we execute the desired command
                    arguments[j] = NULL;
                    break;
                }

                // Check for < character (INPUT)
                else if (strchr(arguments[j], '<')) {
                    input_file = (arguments[j]+1);

                    // If there was output redirection, we know that the
                    // output file is located at the last ARG
                    // so we need to ignore it when we execute the desired command
                    arguments[j] = NULL;
                    break;
                }
            }

            // Init fork
            switch(pid = fork()) {

                // ... Failure
                case -1:
                    perror("fork");
                    exit(EXIT_FAILURE);
                    break;

                // ... Child process
                case 0:

                    // Handle output redirection
                    if (output_file != NULL) {
                        // Open the file
                        int file_descriptor = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (file_descriptor < 0) {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }

                        // Redirect WRITE to file descriptor
                        dup2(file_descriptor, STDOUT_FILENO);
                        close(file_descriptor);
                    }
                    else if (input_file != NULL) {
                        // Open the file
                        int file_descriptor = open(input_file, O_RDONLY, 0644);
                        if (file_descriptor < 0) {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }

                        // Redirect READ to file descriptor
                        dup2(file_descriptor, STDIN_FILENO);
                        close(file_descriptor);
                    }

                    // In FIRST command
                    // ( and multiple commands exist )
                    if (i == 0 && ncommands > 1) {
                        // Close READ end of the pipe
                        close(pipefd[0]);
                        // Redirect WRITE end of pipe
                        dup2(pipefd[1], STDOUT_FILENO);
                        // Close WRITE end of pipe
                        close(pipefd[1]);
                    }

                    // In SECOND command
                    // ( and multiple commands exist )
                    else {
                        if (ncommands > 1) {
                            // Redirect READ end of  pipe
                            dup2(pipefd[0], STDIN_FILENO);
                            // Close READ end of pipe
                            close(pipefd[0]);  
                        }
                    }

                    // Use different exec calls
                    //  depending on whether command uses full path or PATH,
                    //  and whether it has arguments

                    // If command is a full path
                    if (strchr(arguments[0], '/') != NULL) {
                        // Arguments
                        if (narguments > 1) {
                            exec_retval = execv(arguments[0], arguments);

                            if (exec_retval == -1) {
                                perror("execv");
                                exit(EXIT_FAILURE);
                            }
                        }

                        // No arguments
                        else {
                            exec_retval = execl(arguments[0], arguments[0], NULL);

                            if (exec_retval == -1) {
                                perror("execl");
                                exit(EXIT_FAILURE);
                            }
                        }
                    }

                    // Not full path
                    else {
                        // Arguments
                        if (narguments > 1) {
                            exec_retval = execvp(arguments[0], arguments);

                            if (exec_retval == -1) {
                                perror("execvp");
                                exit(EXIT_FAILURE);
                            }
                        }

                        // No arguments
                        else {
                            exec_retval = execlp(arguments[0], arguments[0], NULL);

                            if (exec_retval == -1) {
                                perror("execlp");
                                exit(EXIT_FAILURE);
                            }
                        }
                    }

                    break;

                // ... Parent process (Case: Child's PID)
                default:

                    // Wait until child process is finished
                    do {
                        wait(NULL);
                    } while (ECHILD != errno);

                    // In FIRST command
                    // ( and multiple commands exist )
                    if (i == 0 && ncommands > 1) {
                        // Close the WRITE end of the pipe
                        close(pipefd[1]);
                    }

                    // In SECOND command
                    // ( and multiple commands exist )
                    else {
                        if (ncommands > 1) {
                            // Close the READ end of the pipe
                            close(pipefd[0]);
                        }
                    }

                    break;
            }
        }
    }
    free(input);

    exit(EXIT_SUCCESS);
}