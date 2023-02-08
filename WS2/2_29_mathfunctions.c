#include <stdio.h>
#include <math.h>

int main(void) {
    double x;
    double y;
    double z;
    double temp;

    fscanf(stdin, " %lf %lf %lf", &x, &y, &z);

    temp = pow(x, z);
    printf("%0.2lf ", temp);
    
    temp = pow(x, pow(y, 2));
    printf("%0.2lf ", temp);
    
    temp = fabs(y);
    printf("%0.2lf ", temp);
    
    temp = sqrt(pow(x * y, z));
    printf("%0.2lf\n", temp);

    return 0;
}
