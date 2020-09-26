#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float n;
    do
    {
        n = get_float("Change: ");
    }
    while (n < 0);
    int coins = round(n * 100);

    int t = 0;

    //printf("Coins: %i\n", coins);

    while (coins >= 25) // Checks for quarters
    {
        coins -= 25;
        t++;
        printf("Quarter");
    }
    while (coins >= 10) // Checks for dimes
    {
        coins -= 10;
        t++;
        printf("Dime");
    }
    while (coins >= 5) // Checks for nickels
    {
        coins -= 5;
        t++;
        printf("Nickel");
    }
    while (coins > 0) // Checks for pennies
    {
        coins -= 10;
        t++;
        printf("Penny");
    }
    printf("%i\n", t);
}
