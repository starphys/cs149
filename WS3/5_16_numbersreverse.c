#include <stdio.h>

int main(void) {
    const int NUM_ELEMENTS = 20;         // Number of input integers
    int userVals[NUM_ELEMENTS];          // Array to hold the user's input integers
    int count = 0, i = 0;

    fscanf(stdin, " %d", &count);

    for(; i < count; i++) {
        fscanf(stdin, " %d", &userVals[i]);
    }

    for (i = count - 1; i > -1; i--) {
        fprintf(stdout,"%d,", userVals[i]);
    }
    fprintf(stdout, "\n");

    return 0;
}