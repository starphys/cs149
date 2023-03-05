/**
 * Description: This module counts the recurrences of unique strings in any number of files concurrently.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 03/04/2023
 * Creation date: 02/14/2023
 **/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_NAMES 100
#define MAX_LENGTH 31

int main( int argc, char *argv[] ) {
    int argIndex = 1;
    int cpid;

    if(argc < 2) {
        return 0;
    }

    // Create and track children and pipes
    int children[argc-1];
    memset(children, 0, sizeof(children));
    int fd[argc-1][2];

    // Create and initialize two 2D arrays to track names and their counts
    int count = 0;

    char uniqueNames[MAX_NAMES][MAX_LENGTH];
    memset(uniqueNames, 0, sizeof(uniqueNames));

    int nameCounts[MAX_NAMES];
    memset(nameCounts, 0, sizeof(nameCounts));

    // Create one child process per filename
    for(; argIndex < argc; ++argIndex) {
        if(pipe(fd[argIndex-1]) < 0) {
            return 1;
        }
        
        cpid = fork();
        
        if(cpid < 0) {
            return 1;
        }

        // Child should not finish the loop
        if(cpid == 0) {
            // Close the read end of the pipe
            if( close(fd[argIndex-1][0]) < 0) {
                return 1;
            }
            break;
        }
        else {
            // Close write end of pipe
            if( close(fd[argIndex-1][1]) < 0) {
                return 1;
            }
            children[argIndex-1] = cpid;
        }
    }

    // All child threads are worker threads, one per file
    if(cpid == 0) {
        // Create file pointer, open file
        char* filename = argv[argIndex];
        FILE* inFile = fopen(filename, "r");

        if(!inFile) {
            fprintf(stdout,"range: cannot open file\n");
            return 1;
        }

        // Track which line we are on
        int i = 0;
        char input[MAX_LENGTH];

        // Read line by line, discard blank lines, compare non-blank lines to existing array
        while(true) {

            ++i;

            // Prepare temp string for writing
            memset(input, 0, MAX_LENGTH*sizeof(char));
            
            // Watch for errors and EOF
            if(fgets(input,MAX_LENGTH,inFile) == NULL) {
                break;
            }

            // Check blank lines
            if(input[0] == '\n' || (input[0] == ' ' && (input[1] == '\n' || input[1] == '\0'))) {
                fprintf(stderr, "Warning - file %s line %d is empty.\n", argv[argIndex], i);
                continue;
            }

            // Cleanup non-blank read
            if (input[strlen(input) - 1] == '\n') {
                input[strlen(input) - 1] = '\0';
            }

            // Check if this name is present in the names array
            bool found = false;
            for(int j = 0; j < count; ++j) {
                // We found this name already, increment its count
                if(!strcmp(uniqueNames[j], input)) {
                    nameCounts[j]++;
                    found = true;
                    break;
                }
            }

            // This is a new name, add it to list
            if(!found && count < MAX_NAMES) {
                strcpy(uniqueNames[count], input);
                nameCounts[count]++;
                count++;
            }
        }
        
        // Something went wrong while reading
        if(ferror(inFile))
        {
            return 1;
        }

        // Successfully read the entire file
        fclose(inFile);

        // Send results to parent
        for(int j = 0; j < count; ++j) {
            // Write name
            int writtenBytes = write(fd[argIndex-1][1], uniqueNames[j], sizeof(uniqueNames[j]));
            if (writtenBytes < 0 ) {
                return 1;
            }

            // Write count
            writtenBytes = write(fd[argIndex-1][1], &nameCounts[j], sizeof(nameCounts[j]));
            if (writtenBytes < 0 ) {
                return 1;
            }
            
        }

        // This thread is done, sending EOF to pipe will make parsing easier.
        if(close(fd[argIndex-1][1]) < 0) {
            fprintf(stderr,"%d\n", __LINE__);
            return 1;
        }
    }

    // Parent thread coordinates workers and stores their output.
    else {
        int returned, status, readCount = -1;
        char readName[MAX_LENGTH];

        // Process responses from children.
        while(1) {

            // Wait on an arbitrary number of children, process each one when it's ready.
            returned = wait(&status);
            if( returned <= 0) {
                break;
            }

            // Figure out which child process this is
            int i = 0;
            for(; i < argc-1; ++i) {
                if(children[i] == returned) {
                    break;
                }
            }

            // Read from pipe until empty
            while(1) {
                // Do not process failed child
                if(!WIFEXITED(status)) {
                    break;
                }
                
                int readBytes = read(fd[i][0], readName, MAX_LENGTH);
                if (readBytes <= 0) {
                    break;
                }
                readBytes = read(fd[i][0], &readCount, sizeof(int));
                if (readBytes <= 0) {
                    break;
                }

                // Check if this name is present in the names array
                bool found = false;
                for(int j = 0; j < count; ++j) {

                    // Name is present, add its count!
                    if(!strcmp(uniqueNames[j], readName)) {
                        nameCounts[j] += readCount;
                        found = true;
                        break;
                    }
                }

                // This is a new name, add it to list
                if(!found && count < MAX_NAMES) {
                    strcpy(uniqueNames[count], readName);
                    nameCounts[count] += readCount;
                    count++;
                }

            }

            if(close(fd[i][0]) < 0 ) {
                return 1;
            }

        }

        // Output final results (?)
        for(int k = 0; k < count; ++k) {
            fprintf(stdout, "%s. %d\n",uniqueNames[k],nameCounts[k]);
        }
    }

    return 0;
}