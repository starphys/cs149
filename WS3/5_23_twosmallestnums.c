#include <stdio.h>

int main(void) {
    int count = 0, i = 0, temp = 0, min = __INT_MAX__, nextMin = __INT_MAX__;

    fscanf(stdin, " %d", &count);

    for(; i < count; i++) {
        fscanf(stdin, " %d", &temp);
        if(temp < min) {
            nextMin = min;
            min = temp;
        }
        else if(temp < nextMin) {
            nextMin = temp;
        }
    }
  
    fprintf(stdout, "%d and %d\n", min, nextMin);

    return 0;
}