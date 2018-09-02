/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include <string.h>
#include <ctype.h>

#define ALPHABET 27 // 26 letters plus apostraphe

// Implement trie datastructure
typedef struct _node
{
    bool is_word;
    struct _node* children[ALPHABET];
}
node;

node* rootNode;

// declare freenode function for recursion
void freeNode(node *currentNode);

//declare wordCount
int wordCount = 0;


// Returns true if word is in dictionary else false.
bool check(const char *word)
{
    // set current node to root node
    node* currNode = rootNode;

    for (int i = 0; i < strlen(word); i++)
    {
        char letter = word[i];
        int letInt = 0;

        if (isupper(letter)) // make letter lowercase
        {
            letter = tolower(letter);
        }

        if (letter == '\'')
        {
            letInt = 26;
        }
        else
        {
            letInt = letter - 'a';
        }

        if (currNode -> children[letInt] == NULL)    // if no node exists for this letter
            return false;                       // then word is false

        currNode = currNode -> children[letInt];   // enter the node if there is one
    }

    // if we are at the end of a word
    if (currNode -> is_word)
        return true;
    else
        return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // declare root node
    rootNode = malloc(sizeof(node));

    if (rootNode == NULL)
    {
        return false;
    }

    // open the dictionary file
    FILE* dict_ptr = fopen(dictionary, "r");

    // make sure file pointer points to something
    if (dict_ptr == NULL)
    {
        fprintf(stderr, "Dictionary not found\n");
        return false;
    }

    // declare current node and make it rootnode
    node* currNode = rootNode;

    // use fgetc to iterate through every letter in dictionary file
    char ch;
    while ((ch = fgetc(dict_ptr)) != EOF)
    {
        // make ch an int
        int intCh = ch - 'a';

        if (ch != '\n') // if we are still inside a word
        {
            if (isalpha(ch)) // if ch is a leter
            {
                if (currNode->children[intCh] == NULL)   // and there is no node for current letter
                {
                    currNode->children[intCh] = malloc(sizeof(node));   // malloc a new node
                    currNode = currNode->children[intCh];                 // go to address
                }
                else    // else if there is already a node
                {
                    currNode = currNode->children[intCh];    // enter said node
                }
            }
            else // if ch is an apostraphe
            {
                if (currNode->children[26] == NULL)   // and there is no node for current letter
                {
                    node* newNode = malloc(sizeof(node));   // malloc a new node
                    currNode->children[26] = newNode;  // put new node in the current trie
                    currNode = newNode;     // move into this new node
                }
                else    // else if there is already a node
                {
                    currNode = currNode->children[26];    // enter said node
                }
            }
        }
        else // else if we are at a \n and the word has ended
        {
            currNode->is_word = true;    // set is_word to true!
            wordCount++;
            currNode = rootNode;        // go back to rootnode

        }
    }

    fclose(dict_ptr);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (wordCount > 0)          // if wordCount exist and has iterated return
        return wordCount;

    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // start at the root node
    node* currNode = rootNode;

    freeNode(currNode);

    return true;
}

void freeNode(node* currentNode)
{
    for (int i = 0; i < 27; i++)        // for each child
    {
        if (currentNode->children[i] != NULL)      // if curr child exists
            freeNode(currentNode->children[i]);        //  call function agan to go as deepp as poss
    }
    free(currentNode);                           // once all children are explored, free self
}

