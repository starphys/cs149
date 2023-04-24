/**
 * Description: This program allocates and initializes dynamic memory then fails to free it.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 04/23/2023
 * Creation date: 04/23/2023
 * 
 * This program allocates a block of 400 bytes, assigns values to it, and then exits. When main
 * returns, ptr goes out of scope and is deallocated, and no pointer to the 400 bytes remains.
 * Therefore, valgrind reports this as a memory leak where those 400 bytes are definitely lost.
 * Generally this would be considered bad practice, and ptr should be freed before termination.
 * However, the OS can retrieve this memory, so this example would not cause any long term problems.
 **/

#include <stdlib.h>

int main (void) {

    int *ptr = (int*)malloc(100*sizeof(int));

    for(int i = 0; i < 100; i++) {
        ptr[i] = i;
    }

    return 0;
}