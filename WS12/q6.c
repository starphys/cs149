/**
 * Description: This program dynamically allocates an array of integers, initializes them,
 * frees them, and then tries to print the value of one of the elements of the array.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 04/23/2023
 * Creation date: 04/23/2023
 * 
 * This program outputs garbage data instead of 3. Valgrind reports an invalid read of size 4.
 **/

#include <stdio.h>
#include <stdlib.h>

int main (void) {

    int *data = (int*)malloc(100*sizeof(int));

    for(int i = 0; i < 100; i++) {
        data[i] = i;
    }

    free(data);

    fprintf(stdout, "%d\n", data[3]);

    return 0;
}