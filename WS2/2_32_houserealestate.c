#include <stdio.h>

    int main(void) {
    int currentPrice;
    int lastMonthsPrice;

    scanf("%d", &currentPrice);
    scanf("%d", &lastMonthsPrice);

    int change = currentPrice - lastMonthsPrice;

    float mortgage = (currentPrice * 0.051) / 12;

    fprintf(stdout, "This house is $%d. The change is $%d since last month.\nThe estimated monthly mortgage is $%0.6f.\n", currentPrice, change, mortgage);

    return 0;
}