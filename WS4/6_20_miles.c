#include <stdio.h>

double LapsToMiles(double userLaps);

int main(void) {

    double yourValue;
    fscanf(stdin, " %lf", &yourValue);
    yourValue = LapsToMiles(yourValue);
    fprintf(stdout,"%0.2lf\n", yourValue);
    return 0;
}

double LapsToMiles(double userLaps) {
    return userLaps/4.0;
}