#include <cs50.h>
#include <stdio.h>

bool printHash(int spaces1, int hash1, int hash2);


int main(void)
{
    int h;
    do 
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);
    
    if (h == 1)
    {
        printHash(0, 1, 1);
    }
    else if (h != 1)
    {
        int n = 1;
        for (int i = h; i > 0; i--)
        {
            printHash(i - 1, n, n);
            n++;
        }
    }
    
    // Example for 8:
    //printHash(7, 1, 1)
    //printHash(6, 2, 2)
    //printHash(5, 2, 2)
    //...
}

bool printHash(int spaces1, int hash1, int hash2)
{
    for (int i = 0; i < spaces1; i++)
    {
        printf(" ");
    }
    for (int z = 0; z < hash1; z++)
    {
        printf("#");
    }
    printf("  ");
    for (int zz = 0; zz < hash2; zz++)
    {
        printf("#");
    }    
    printf("\n");
    return true;
}
