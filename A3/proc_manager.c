/**
 * Description: This module runs processes sent to it from the command line and logs their results.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 03/11/2023
 * Creation date: 03/11/2023
 **/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_COMMANDS 100
#define MAX_LENGTH 31

int main( int argc, char *argv[] ) {
    int cpid;

    // Create 2D array to track all commands
    int count = 0;

    char commands[MAX_COMMANDS][MAX_LENGTH];
    memset(commands, 0, sizeof(commands));
    
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

        // DEBUG:
        // fprintf(stdout, "Word count: %d\nExecuting: ", word);

        // for(int n = 0; n < word; ++n) {
        //     fprintf(stdout, "%s ", argsPtr[n]);
        // }
        fprintf(stdout, "\n");
        fflush(stdout);

        execvp(args[0], argsPtr);
        fprintf(stderr, "couldn't execute: %s", args[0]);
        return 127;
    }

    // Parent thread coordinates workers and stores their output.
    else {
        while(wait(NULL) > 0) {}
    }

    return 0;
}