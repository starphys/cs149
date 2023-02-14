#include <stdio.h>

int main(void) {
    int x;

    fscanf(stdin, " %d", &x);

   while ( x > 0) {
    fprintf(stdout, "%d", x%2);
    x /= 2;
   }
   fprintf(stdout,"\n");

    return 0;
}
