// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table: 27^2
const unsigned int N = 676;

// Hash table
node *table[N];

// The amount of words in the dictionary
int loaded = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lowerWord[LENGTH + 1];
    for (int i = 0; i < strlen(word); i++)
    {
        lowerWord[i] = tolower(word[i]);
    }
    lowerWord[strlen(word)] = '\0';


    // Find the correct part of the hashtable
    unsigned int hashed = hash(lowerWord);
    node *tmp = table[hashed];

    // The bucket is NULL, therefore it's not there
    if (tmp == NULL)
    {
        return false;
    }
    // Check the first word
    if (strcasecmp(lowerWord, tmp->word) == 0)
    {
        return true;
    }

    // Itereates through the entire list
    while (tmp->next != NULL)
    {
        // Iterates
        tmp = tmp->next;
        // Checks to see if the words are the same (ignoring case)
        if (strcasecmp(lowerWord, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number (Assumes lowercase)
// Using djb2 hash function from: https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f#file-djb2-hash-function-c-L3
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Opens the file
    FILE *f = fopen(dictionary, "r");
    if (!f)
    {
        return false;
    }

    // Creates a variable for the word
    char word[LENGTH + 1];

    // Reads words until EOF
    fscanf(f, "%s", word);
    do
    {
        // Creates a Node
        node *n = malloc(sizeof(node));
        if (!n)
        {
            return false;
        }

        // Copies values into node
        strcpy(n->word, word);
        n->next = NULL;

        // Finds the table location
        unsigned int hashed = hash(word);
        node *tmp = table[hashed];

        // tmp is NULL, than we fill the first table value
        if (tmp == NULL)
        {
            table[hashed] = n;
        }
        // Othwerise we have to insert the value at the start
        else
        {
            n->next = table[hashed]; // Pointer of the new node points to the first node
            table[hashed] = n; // The hashtable now points to the new node
        }

        // Increases the amount of words loaded
        loaded++;
    }
    while (fscanf(f, "%s", word) != EOF); // Ensures we haven't reached EOF

    // Frees the FILE pointer
    fclose(f);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return loaded;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // For every value in the hashtable
    for (int i = 0; i < N; i++)
    {
        //node *cursor = table[i];
        node *cursor = table[i];
        if (cursor == NULL)
        {
            free(cursor);
        }
        else
        {
            node *tmp = cursor;
            while (cursor != NULL)
            {
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
    }
    return true;
}