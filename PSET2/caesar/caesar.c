#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // If they don't include a key value
    if (argc != 2)
    {
        printf("Invalid commandline arguments.\n");
        return 1;
    }

    // The keyvalue as a string
    string key_rawinput = argv[1];

    // Checks to ensure the key's digits are all numbers
    for (int i = 0, l = strlen(key_rawinput); i < l; i++)
    {
        if ('0' > key_rawinput[i] || key_rawinput[i] > '9')
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Gets the plaintext from the user
    string plaintext = get_string("plaintext: ");
    string output = plaintext;

    // Actual shift value
    int key = atoi(key_rawinput);

    // Loop for every value in the plaintext
    for (int i = 0, l = strlen(plaintext); i < l; i++)
    {
        // The actual plaintext value
        char c = plaintext[i];

        // Ensures that the value is a charchter that is shifted
        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
        {
            // If it's uppercase we +- 65
            if ('A' <= c && c <= 'Z')
            {
                int x = (((c - 65) + key) % 26) + 65;
                char y = x;
                output[i] = y;
            }
            // If it's lowercase we +- 97
            else
            {
                int x = (((c - 97) + key) % 26) + 97;
                char y = x;
                output[i] = y;
            }
        }
        else
        {
            output[i] = plaintext[i];
        }
    }
    printf("ciphertext: %s\n", output);
}