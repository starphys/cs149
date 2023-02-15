#include <stdio.h>

int MaxNumber(int num1, int num2, int num3, int num4);
int MinNumber(int num1, int num2, int num3, int num4);

int main(void) {
    const int NUM_ELEMENTS = 4;         // Number of input integers
    int userVals[NUM_ELEMENTS];          // Array to hold the user's input integers
    int i = 0, min = 0, max = 0;

    for(; i < NUM_ELEMENTS; i++) {
        fscanf(stdin, " %d", &userVals[i]);
    }
    /* Type your code here. Your code must call the function.  */
    max = MaxNumber(userVals[0],userVals[1],userVals[2],userVals[3]);
    min = MinNumber(userVals[0],userVals[1],userVals[2],userVals[3]);

    fprintf(stdout,"Maximum is %d\nMinimum is %d\n",max,min);

    return 0;
}

int MaxNumber(int num1, int num2, int num3, int num4) {
    if( num1 >= num2 && num1 >= num3 && num1 >= num4) return num1;
    if( num2 >= num1 && num2 >= num3 && num2 >= num4) return num2;
    if( num3 >= num2 && num3 >= num1 && num3 >= num4) return num3;
    if( num4 >= num2 && num4 >= num3 && num4 >= num1) return num4;
    return 0;
}
int MinNumber(int num1, int num2, int num3, int num4) {
    if( num1 <= num2 && num1 <= num3 && num1 <= num4) return num1;
    if( num2 <= num1 && num2 <= num3 && num2 <= num4) return num2;
    if( num3 <= num2 && num3 <= num1 && num3 <= num4) return num3;
    if( num4 <= num2 && num4 <= num3 && num4 <= num1) return num4;
    return 0;
}