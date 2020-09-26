#include <stdio.h>
#include <cs50.h>

int main(void)
{
    char *x = "apple";
    int g = 'a' - 97;
    int g2 = 'b' - 97;
    char *y = "xy";

    printf("%i\n", g + g2);
    printf("%i\n", (x[0] - 97) * 26 + x[1]);
    printf("%i\n", (y[0] - 97) * 26 + y[1]);

    // Formula:
    int Fletter = ((x[0] - 97) * 26);
    int Sletter = x[1] - 97;
    printf("%i\n", Fletter + Sletter);
}