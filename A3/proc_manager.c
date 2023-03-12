/**
 * Description: This module runs processes sent to it from the command line and logs their results.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 03/11/2023
 * Creation date: 03/11/2023
 **/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define MAX_COMMANDS 100
#define MAX_LENGTH 31

int main( int argc, char *argv[] ) {
    int cpid;

    // Create 2D array to track all commands
    int count = 0;

    char commands[MAX_COMMANDS][MAX_LENGTH];
    memset(commands, 0, sizeof(commands));
    
    // For managing files
    char fileName[MAX_LENGTH];

    // Get commands from stdin
    for(; count < MAX_COMMANDS; ++count) {
        if( fgets(commands[count], MAX_LENGTH, stdin) == NULL) {
            break;
        }
        // Cleanup non-blank read
        if (commands[count][strlen(commands[count]) - 1] == '\n') {
            commands[count][strlen(commands[count]) - 1] = '\0';
        }
    }

    // Create one child process per command
    int i = 0;
    for(; i < count; ++i) {
        cpid = fork();
        
        if(cpid < 0) {
            return 1;
        }

        // Child should not finish the loop
        if(cpid == 0) {
            // Use i to know which command this child is responsible for
            break;
        }
    }

    // Execute one command per child
    if(cpid == 0) {
        // Prepare log files
        sprintf(fileName, "%d.out", getpid());
        int fd_out = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0777);
        FILE* out = fopen(fileName, "a");
        sprintf(fileName, "%d.err", getpid());
        int fd_err = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0777);
        FILE* err = fopen(fileName, "a");

        if(dup2(fd_out, 1) != 1) {
            fprintf(stdout, "dup2 didn't work!");
        }
        if(dup2(fd_err, 2) != 2) {
            fprintf(stderr, "dup2 didn't work!");
        }

        fprintf(out, "Starting command %d: child %d pid of parent %d\n", i, getpid(), getppid());
        fflush(out);

        // Begin processing commands
        char args[MAX_LENGTH][MAX_LENGTH];
        memset(args, 0, sizeof(args));
        
        char *argsPtr[MAX_LENGTH];
        int length = strlen(commands[i]);
        int word = 0;
        int letterCount = 0;

        // Break up input into argument vector for exec
        for(int j = 0; j <= length; ++j) {
            // End of word, null terminate and prepare for next word
            if(commands[i][j] == ' ') {
                args[word][letterCount] = '\0';
                argsPtr[word] = args[word];
                letterCount = 0;
                ++word;
            }
            // End of word and commands
            else if (commands[i][j] == '\0') {
                args[word][letterCount] = '\0';
                argsPtr[word] = args[word];
                letterCount = 0;
                ++word;
                break;
            }
            // Normal character, add to current word
            else {
                args[word][letterCount++] = commands[i][j];
            }
        }
        // Null terminate argument vector
        argsPtr[word] = NULL;

        execvp(args[0], argsPtr);
        fprintf(err, "Could not execute: %s\n", commands[i]);
        return 2;
    }

    // Parent thread coordinates workers and stores their output.
    else {
        int pid, status;
        
        while ((pid = wait(&status)) > 0) {
            sprintf(fileName, "%d.out", pid);
            FILE* fd_out = fopen(fileName, "a");
            sprintf(fileName, "%d.err", pid);
            FILE* fd_err = fopen(fileName, "a");

            if (WIFEXITED(status)) {
                fprintf(fd_err, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                fprintf(fd_err, "Killed with signal %d\n", WTERMSIG(status));  
            }

            fprintf(fd_out,"Finished child %d pid of parent %d\n", pid, getpid());
            fclose(fd_out);
            fclose(fd_err);
        }
    }

    return 0;
}