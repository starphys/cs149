#include <stdio.h>

int main(void) {
    const int NUM_ELEMENTS = 20;         // Number of input integers
    int userVals[NUM_ELEMENTS];          // Array to hold the user's input integers
    int count = 0, i = 0, min = 0, max = 0;

    fscanf(stdin, " %d", &count);

    for(; i < count; i++) {
        fscanf(stdin, " %d", &userVals[i]);
    }
    fscanf(stdin, " %d %d", &min, &max);

    for(i = 0; i < count; i++) {
        if(userVals[i] <= max && userVals[i] >= min) {
            fprintf(stdout,"%d,",userVals[i]);
        }
    }    
    fprintf(stdout, "\n");

    return 0;
}