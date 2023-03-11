#include <stdio.h>
#include <stdbool.h>
#define NUM_ELEMENTS 20

bool IsArrayMult10(int inputVals[], int numVals);
bool IsArrayNoMult10(int inputVals[], int numVals);

int main(void) {
    int userValues[NUM_ELEMENTS];
    int count = 0, i = 0;

    fscanf(stdin, " %d", &count);

    for(; i < count; i++) {
        fscanf(stdin, " %d", &(userValues[i]));
    }
    
    if(IsArrayMult10(userValues, count) ) {
       fprintf(stdout, "all multiples of 10\n");
    }
    else if(IsArrayNoMult10(userValues, count)) {
        fprintf(stdout, "no multiples of 10\n");
    }
    else {
       fprintf(stdout, "mixed values\n");
    }

   return 0;
}

bool IsArrayMult10(int inputVals[], int numVals) {
   for(int i = 0; i < numVals; ++i) {
      if(inputVals[i]%10 != 0) {
         return false;
      }
   }
   return true;
}
bool IsArrayNoMult10(int inputVals[], int numVals) {
   for(int i = 0; i < numVals; ++i) {
      if(inputVals[i]%10 == 0) {
         return false;
      }
   }
   return true;
}