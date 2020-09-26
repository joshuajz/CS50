#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("Enter your name: "); // Prompts the user for a string
    printf("hello, %s\n", name); // Prints hello + the input
}
