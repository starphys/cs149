#include <stdio.h>

int main(void) {

    int x = 1, y = 1, a = 1, b = 1, c = 1;

    fscanf(stdin, " %d %d", &y, &x);
    
    if (!x) {
        fprintf(stdout, "Divide by 0 is not a funny joke.");
        return 1;
    }

    a = y/x;
    b = a/x;
    c = b/x;

    fprintf(stdout, "%d %d %d\n", a, b, c);

    return 0;
}
