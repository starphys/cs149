#include <stdio.h>

void SwapValues(int* userVal1, int* userVal2, int* userVal3, int* userVal4);

int main(void) {
   int userVal1 = 0, userVal2 = 0, userVal3 = 0, userVal4 = 0;
   
   fscanf(stdin, " %d %d %d %d", &userVal1, &userVal2, &userVal3, &userVal4);
   
   SwapValues(&userVal1, &userVal2, &userVal3, &userVal4);

   return 0;
}

void SwapValues(int* userVal1, int* userVal2, int* userVal3, int* userVal4) {
   int temp = *userVal1;
   *userVal1 = *userVal2;
   *userVal2 = temp;
   temp = *userVal3;
   *userVal3 = *userVal4;
   *userVal4 = temp;
   
   fprintf(stdout, "%d %d %d %d\n", *userVal1, *userVal2, *userVal3, *userVal4);
}