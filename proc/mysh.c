#include <stdio.h>
#include <unistd.h>    // fork()
#include <stdlib.h>    // exit()
#include <inttypes.h>  // intmax_t
#include <sys/wait.h>  // wait()
#include <string.h>    // strcspn(), strtok
#include <errno.h>     // ECHILD
#include <fcntl.h>     // O_RDONLY, open


// NOTES:
// Redirection
// * There should only be one redirection of each kind at the most
// Piping
//  * Only a single pipe is required to be supported

int main() {
    // Init variables for obtaining user input
    const int BUFSIZE = 1024;
    char *prog;
    char *arg;
    char *buffer;

    // Init variables for commands / arguments
    char *commands[3];
    char *arguments[5];
    int narguments;
    int ncommands = 0;
    int i, j;

    // Init variables for piping/forking/executing
    int pipefd[2];
    int pid;
    int execvp_retval;

    // Get user input while no error / input
    while (1) {
        buffer = malloc(sizeof(char) * BUFSIZE);

        // Prompt user
        printf("$ ");

        // Read in command with maximum size
        if (!fgets(buffer, BUFSIZE, stdin)) {
            
        }
    }

    // // Tokenize the input string by |
    // for (commands[ncommands] = strtok(input, "|"); 
    //     commands[ncommands] != NULL;
    //     commands[++ncommands] = strtok(NULL, "|"));

    // // // Print obtained commands
    // // printf("%d\n", ncommands);
    // // for (i = 0; i < ncommands; i++) {
    // //     printf("%s\n", commands[i]);
    // // }

    // // Initialize the pipe
    // if (pipe(pipefd) == -1) {
    //     perror("pipe");
    //     exit(EXIT_FAILURE);
    // }

    // // Process the commands
    // for (i = 0; i < ncommands; i++)
    // {
    //     // // Print i and current command
    //     // printf("%d %s \n", i, commands[i]);

    //     // Process command arguments
    //     narguments = 0;
    //     for (arguments[narguments] = strtok(commands[i], " ");
    //         arguments[narguments] != NULL;
    //         arguments[++narguments] = strtok(NULL, " "));

    //     // // Print args for command
    //     // for (j = 0; j < narguments; j++)
    //     // {
    //     //     printf("%s\n", arguments[j]);
    //     // }

    //     // Init fork
    //     switch(pid = fork()) {

    //         // ... Failure
    //         case -1:
    //             perror("fork");
    //             exit(EXIT_FAILURE);
    //             break;

    //         // ... Child process
    //         case 0:

    //             // // Print current pid
    //             // printf("\t%d\n", pid);

    //             // In FIRST command
    //             if (i == 0) {
    //                 // Close READ end of the pipe
    //                 close(pipefd[0]);
    //                 // Redirect WRITE end of pipe
    //                 dup2(pipefd[1], STDOUT_FILENO);
    //                 // Close WRITE end of pipe
    //                 close(pipefd[1]);
    //             }

    //             // In SECOND command
    //             else {
    //                 // Redirect READ end of  pipe
    //                 dup2(pipefd[0], STDIN_FILENO);
    //                 // Close READ end of pipe
    //                 close(pipefd[0]);
    //             }

    //             // Execute command given by arguments
    //             execvp_retval = execvp(arguments[0], arguments);
    //             if (execvp_retval == -1) {
    //                 perror("execvp");
    //                 exit(EXIT_FAILURE);
    //             }

    //             break;

    //         // ... Parent process (Case: Child's PID)
    //         default:

    //             // // Print current pid
    //             // printf("\t%d\n", pid);

    //             // Wait until child process is finished
    //             do {
    //                 wait(NULL);
    //             } while (ECHILD != errno);

    //             // In FIRST command
    //             if (i == 0) {
    //                 // Close the WRITE end of the pipe
    //                 close(pipefd[1]);
    //             }

    //             // In SECOND command
    //             else {
    //                 // Close the READ end of the pipe
    //                 close(pipefd[0]);
    //             }

    //             break;
    //     }
    // }



    exit(EXIT_SUCCESS);
}