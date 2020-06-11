// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // initialize the current node at root
    node *current = root;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        // iterate each character of a word
        for (int j = 0, w_lenght = (LENGTH + 1); j < w_lenght && word[j] != NULL; j++)
        {
            // get ascii value from each character and scale the value from 0 to 26
            if (word[j] >= 97 && word[j] < 123)
            {
                word[j] = word[j] - 97;
            }

            // scale ascii value for apostrophe which is 39
            else if (word[j] == 39)
            {
                word[j] = (word[j] - 39) + 27;
            }

            // go to node and search for index[scaled ascii value] and check if NULL
            int index = atoi(word[j]);

            if (current->children[index] == NULL);
            {
                // if NULL then create a new node
                // Initialize a new node
                node *n_node = malloc(sizeof(node));
                // check if there is memory for new node
                if (n_node == NULL)
                {
                    return false;
                }
                n_node->is_word = false;
                for (int k = 0; k < N; k++)
                {
                    n_node->children[k] = NULL;
                }

                // get the address value of new node into current node index[scaled ascii value]
                current->children[index] = n_node;
            }

            // move to new node
            current = current->children[index];
        }
        // when iteration over word ends then turn is_word to TRUE
        current->is_word = True;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // if file is empty free root
    // else recursive function to free current

    return false;
}
