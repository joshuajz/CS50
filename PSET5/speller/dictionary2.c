// Implements a dictionary's functionality

#include <stdbool.h>

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

// Prototype
void recursionUnload(node* location);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Find the correct part of the hashtable
    int hashed = hash(word);
    node *tmp = table[hashed];
    
    // The bucket is NULL, therefore it's not there
    if (tmp == NULL)
    {
        return false;
    }
    // Check the first word
    if (strcasecmp(word, tmp->word) == 0)
    {
        return true;
    }

    // Itereates through the entire list
    while(tmp->next != NULL)
    {
        // Iterates
        tmp = tmp->next;
        // Checks to see if the words are the same (ignoring case)
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number (Assumes lowercase)
unsigned int hash(const char *word)
{
    // If the word is only one letter
    if (strlen(word) == 1)
    {
        return (tolower(word[0]) - 97) * 26;
    }
    // 2+ letters
    else
    {
        return ((tolower(word[0]) - 97) * 26) + ((tolower(word[1])) - 97);
    }
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
        int hashed = hash(word);
        node *tmp = table[hashed]
        
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
    while (fscanf(f, "%s", word) != NULL);

    // Frees the FILE pointer
    free(f);
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
        // If the first value is NULL, we can free right away
        if (table[i] == NULL)
        {
            free(table[i]);
        }
        // Otherwise we use the recursive function to free the entire list
        else
        {
            recursionUnload(table[i]);
        }
    }
    return true;
}

void recursionUnload(node* location)
{
    // Base Case
    if (location->next == NULL)
    {
        free(location);
    }
    // Make the problem smaller (ie. get closer to the end of the list)
    else
    {
        recursionUnload(location->next);
    }
}