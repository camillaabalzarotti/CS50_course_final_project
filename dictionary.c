// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

#define ALPHABETH 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 26; //for each letter 26 subgroups

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index;
    int found;
    node *cursor;

    index = hash(word);
    found = 0;
    for (cursor = table[index]; cursor != NULL && found == 0; cursor = cursor->next)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            found = 1;
        }
    }
    if (found == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int pos1, pos2;
    int index;

    pos1 = toupper(word[0]) - 'A';
    if (strlen(word) > 1){
        pos2 = toupper(word[1]) - 'A';
    }
    else
    {
        pos2 = 0;
    }

    index = pos1 * ALPHABETH + pos2;
    return index;
}
int count = 0; //global variable to count words then useful for size

// Loads dictionary into memory/the hash table, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *f;
    char cur[LENGTH + 1];
    int index;

    f = fopen(dictionary, "r"); //opening the file
    if (f != NULL) //if able to open the file
    {
        //while not EOF
        while (fscanf (f, "%s", cur) != EOF)//read one word at a time until the end of the file
        {
            count ++;
            //create a new node for each word
            node * n = malloc(sizeof(node));
            if (n != NULL)
            {
                strcpy(n->word, cur);
                n->next = NULL;
            }
            else
            {
                printf ("probably out of memory\n");
                return false;
            }
            //hash that word using an hash function >>result = location
            index = hash(n->word);
            //insert node into the hash table at that location
            n->next = table[index];
            table[index] = n;
        }
        fclose(f);
        return true;
    }
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (count != 0)
    {
        return count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int i;
    node * cursor;
    int isOK = 1;

    //freeing each node in every bucket of the hash table
    for (i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            cursor = table[i];
            table[i] = table[i]->next;
            free(cursor);
        }
        free (table[i]);
    }
    if (isOK == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

