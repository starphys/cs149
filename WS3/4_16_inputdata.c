#include <stdio.h>

int main(void) {
    int temp = 0, max = -1, total = 0;
    float count = 0;

    while (1) {
        fscanf(stdin, " %d", &temp);
        if (temp < 0) break;
        if ( temp > max) max = temp;
        total += temp;
        count++;
    }

    fprintf(stdout, "%d %0.2lf\n", max, total/count);

    return 0;
}
