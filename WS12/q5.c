/**
 * Description: This program creates a dynamic array of integers called data of size 100 using malloc; 
 * then attempts to set data[100] to zero.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 04/23/2023
 * Creation date: 04/23/2023
 * 
 * This program runs and sets data[100] to 0. Hoever, it leaks memory just like in q4.c.
 * This program is not correct, in that data[100] is out of bounds of the memory we originally allocated.
 **/

#include <stdlib.h>

int main (void) {

    int *data = (int*)malloc(100*sizeof(int));

    data[100] = 0;

    return 0;
}