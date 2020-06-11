// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 2;

// Hash table
node *table[N];

// count of words loaded from dictionary
int count_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // Hash the text's word
    int hash_index;
    hash_index = hash(word);
    // prepare to check word
    int check = 0;

    // Go to the linked list which is in the location 'hash_index' of the array 'table' using table[hash_index]
    // traverse linked list, looking for the text's word
    for (node *cursor = table[hash_index]; cursor != NULL; cursor = cursor->next)
    {
        node *test = cursor;
        // Compare the text's word with the dictionary word (strcasecmp)
        if (strcasecmp(cursor->word, word) == 0)
        {
            check = 1;
            break;
        }
    }
    if (check == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
    //return false;
}

// Hashes word to a number
// Using DJB hash function (Professor Daniel J. Bernstein hash function)
// 23/04/2020 from http://www.partow.net/programming/hashfunctions/index.html#SDBMHashFunction
unsigned int hash(const char *word)
{
    // TODO
    unsigned int hash = 5381;
    unsigned int i = 0;

    // word++ move to the next character in word by adding one to the pointer
    // LENGHT is the max lenght of a word
    for (i = 0; i < LENGTH; word++, i++)
    {
        // loop until the word ends as LENGTH is 45 and the word might be shorter. the word ends when the character is NULL
        // so the value in word (*word) is zero.
        if (*word == 0)
        {
            break;
        }
        else
        {
            hash = ((hash << 5) + hash) + (*word);
        }
    }
    hash = hash % N;
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // Open dictionary file
    FILE *inptr = fopen(dictionary, "r");
    // Check inptr is valid
    if (inptr == NULL)
    {
        printf("Could not open %s\n", dictionary);
        return 1;
    }
    // Prepare to read words
    char *word = malloc(sizeof(char) * (LENGTH + 1));

    // prepare to create a new node. it stores a pointer to a node in n
    node *n = NULL;

    // Read words from the dictionary
    // Add first word into a list
    while (fscanf(inptr, "%s", word) != EOF)
    {
        //tracking of the number of words
        count_words++;
        // hash the word
        int hash_index;
        hash_index = hash(word);

        // check if table[hash_index] is NULL or if list already has elements. table[hash_index] stores an address of type node (it's only a pointer)
        // and its the head of the linked list, when table[hash_index] == NULL means it doesn't have any nodes yet, it is an empty list.
        if (table[hash_index] == NULL)
        {
            // create new node
            // get new location into pointer n
            n = malloc(sizeof(node));
            if (n == NULL)
            {
                return 1;
            }
            // copy word into n.word
            strcpy(n->word, word);
            n->next = NULL;
            // go to the array of linked lists called 'table'
            // save the pointer of the new node into the hash index of the array
            table[hash_index] = n;
        }
        else
        {
            // create new node
            // get new location into pointer n
            n = malloc(sizeof(node));
            // Check if malloc was succesful
            if (n == NULL)
            {
                return 1;
            }
            // Add words into list when list has already a value
            // copy word into n.word
            strcpy(n->word, word);
            // new node.next points at the head of the list
            n->next = table[hash_index];
            // list.next gets updated with the address of the new node
            //list = n;
            // go to the array of linked lists called 'table'
            // save the pointer to the new node
            table[hash_index] = n;
        }
    }
    free(word);

    // close infile?
    fclose(inptr);

    if (size() != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (count_words != 0)
    {
        return count_words;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // Loop trough each element of the array 'table'
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        // unload node of each linked list
        for (node *temp = cursor; cursor != NULL; temp = cursor)
        {
            cursor = cursor->next;
            free(temp);
            count_words--;
        }
    }
    if(count_words == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


