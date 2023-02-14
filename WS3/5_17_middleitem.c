#include <stdio.h>

int main(void) {
   const int NUM_ELEMENTS = 9;
   int userValues[NUM_ELEMENTS];    // Set of data specified by the user
   int i = 0, temp = 0;

   while(1) {
        fscanf(stdin, " %d", &temp);
        
        if (temp < 0) break;

        userValues[i] = temp;
        i++;

        if (i > NUM_ELEMENTS) {
            fprintf(stdout,"Too many numbers\n");
            return 0;
        }
   }

   fprintf(stdout, "Middle item: %d\n", userValues[i/2]);


   return 0;
}
