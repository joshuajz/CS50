#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string input);
int count_words(string input);
int count_sentences(string input);

int main(void)
{
    // Input from User
    string input = get_string("Text: ");

    // Uses functions to find letters, words, and sentences
    int letters = count_letters(input);
    //printf("Letters: %i\n", letters);
    int words = count_words(input);
    //printf("Words: %i\n", words);
    int sentences = count_sentences(input);
    //printf("Sentences: %i\n", sentences);


    // L = avg number of letters per 100 words
    // L = letters / words
    float L = ((float)letters / ((float)words / 100));
    float S = ((float)sentences / ((float)words / 100));
    int index = roundf(0.0588 * L - 0.296 * S - 15.8);

    //float index = (0.0588 * ((float) letters / ( (float) words / 100 )))
    //    - (0.296 * (float) sentences / ((float) words / 100)) - 15.8;
    //printf("L: %f | S: %f | Index: %d\n", L, S, index);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (1 <= index && index <= 15)
    {

        printf("Grade %i\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }

}

// Determines the amount of letters
int count_letters(string input)
{
    int letters = 0;
    //printf("%s\n", input);
    //printf("%lu\n", strlen(input));

    // For every charchter in the string (even spaces)
    for (int i = 0, length = strlen(input); i < length; i++)
    {
        //printf("%c\n", input[i]);

        // Checks to see if the charchter is a blank space
        if (('a' <= input[i] && input[i] <= 'z') || ('A' <= input[i] && input[i] <= 'Z'))
        {
            // If it isn't, we increment the total by 1
            letters++;
        }
    }
    // We return the total amount of letters
    return letters;
}

// Determines the amount of words
int count_words(string input)
{
    int words = 1;

    // For every charchter in the string
    for (int i = 0, length = strlen(input); i < length; i++)
    {
        // If it is a space
        if (isblank(input[i]) != 0)
        {
            // Then we've found a word, increment the amount of words
            words++;
        }
    }
    // Return the amount of words
    return words;
}

int count_sentences(string input)
{
    int sentences = 0;

    for (int i = 0, length = strlen(input); i < length; i++)
    {
        if (input[i] == '!' || input[i] == '?' || input[i] == '.')
        {
            sentences++;
        }
    }
    return sentences;
}