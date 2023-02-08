/**
 * Description: This module counts the recurrences of unique strings in an input file.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 11/07/2023
 * Creation date: 11/07/2023
 **/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NAMES 100
#define MAX_LENGTH 31

int main( int argc, char *argv[] ) {
    
    if(argc != 2) {
        return 0;
    }

    // Create file pointer, open file
    char* filename = argv[1];
    FILE* inFile = fopen(filename, "r");

    if(!inFile) {
        printf("error: cannot open file\n");
        return 1;
    }
    
    // Create and initialize two 2D arrays to track names and their counts
    int count = 0;

    char uniqueNames[MAX_NAMES][MAX_LENGTH];
    memset(uniqueNames, 0, MAX_NAMES*MAX_LENGTH*sizeof(char));

    int nameCounts[MAX_NAMES];
    memset(nameCounts, 0, MAX_NAMES*sizeof(int));

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
        if(input[0] == '\n') {
            fprintf(stderr, "Warning - Line %d is empty.\n", i);
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
    
    // Something went wrong
    if(ferror(inFile))
    {
        return 1;
    }

    // Successfully read the entire file
    fclose(inFile);

    // Print results
    for(int j = 0; j < count; ++j) {
        fprintf(stdout, "%s. %d\n",uniqueNames[j],nameCounts[j]);
    }

    return 0;
}