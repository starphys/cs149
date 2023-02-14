#include <stdio.h>

int main(void) {
    const int NUM_ELEMENTS = 20;
    int userValues[NUM_ELEMENTS];    // Set of data specified by the user
    int count = 0, i = 0, threshold = 0;

    fscanf(stdin, " %d", &count);

    for(; i < count; i++) {
        fscanf(stdin, " %d", &userValues[i]);
    }

    fscanf(stdin, " %d", &threshold);

    for(i = 0; i < count; i++) {
        if(userValues[i] < threshold) fprintf(stdout, "%d,", userValues[i]);
    }
    fprintf(stdout,"\n");
    
    return 0;
}
