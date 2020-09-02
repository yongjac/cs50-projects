// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 50000;

// Hash table
node *table[N];

// Number of words
int num_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // Hash word to obtain hash value
    int hash_num = hash(word);

    //Access linked list at that index in the hash table
    node *cursor;
    cursor = table[hash_num];

    // Tranversing Linked List
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // Referenced: https://stackoverflow.com/questions/4384359/quick-way-to-implement-dictionary-in-c
    unsigned int hashval;
    for (hashval = 0; *word != '\0'; word++)
    {
        hashval = tolower(*word) + 31 * hashval;
    }
    return hashval % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO

    // Open dictionary file
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    // Read strings from file
    while(fscanf(dict_file, "%s", word) != EOF)
    {
        // Create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);

        // Increment number of words
        num_words++;

        // Hash word
        int hash_num = hash(word);

        // Insert node into hash table
        n->next = table[hash_num];
        table[hash_num] = n;
    }

    // close file
    fclose(dict_file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return num_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node *cursor;
    node *tmp;
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
