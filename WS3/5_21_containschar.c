#include <stdio.h>
#include <string.h>

int main(void) {
    const int NUM_ELEMENTS = 20;
    const int LENGTH = 10;
    char userValues[NUM_ELEMENTS][LENGTH];    // Set of data specified by the user
    int count = 0, i = 0;
    char searchCharacter = '\0';

    fscanf(stdin, " %d", &count);

    for(; i < count; i++) {
        fscanf(stdin, " %s", &(userValues[i]));
    }

    fscanf(stdin, " %c", &searchCharacter);

    for(i = 0; i < count; i++) {
        int j = 0;
        while(j < LENGTH && userValues[i][j] != '\0'){
            if(userValues[i][j] == searchCharacter) {
                fprintf(stdout, "%s,", userValues[i]);
                break;
            }
            j++;
        }
    }

    fprintf(stdout, "\n");

   return 0;
}
