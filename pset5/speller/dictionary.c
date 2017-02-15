/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h> //needed for FILE datatype and its functions, fopen, fclose, etc. to be used
#include <stdlib.h>
#include <string.h> //needed for strcpy, strcmp
#include <strings.h> //needed for strcasecmp
#include <stdbool.h>
#include <ctype.h>
#include "dictionary.h"


typedef struct node{
    
    char word[LENGTH + 1];
    struct node *next;
}
node;

    
//declare a hashtable structure for storing dictionary, length is of 26 nodes. No need for malloc, but we need free later
//note that 0 = 'a', 1 = 'b', ... , 25 = 'z'
node *hashtable[26];
bool loaded = false;
int dict_size = 0;

/**
 * hash function for the dictionary hashtable, to check word index
*/
int hash_dict(const char word[])
{
    if (!isalpha(word[0])) return -1;
    else 
    {
        char first_letter = tolower(word[0]);
        
        //returns proper index of character
        return (first_letter - 'a');
    }
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
        int index = hash_dict(word);
        node *cursor = hashtable[index];
        while(cursor!=NULL)
        {
            if (!strcasecmp(cursor->word,word)) //compare the two strings using strcasecmp (strcmp but ignoring word cases). 
                                                //NOTE that strcasecmp (and strcmp actually) return value of 0 if str1 is equivalent with str2
                                                //for strcasecmp(const char *str1, const char *str2). 
                                                //REFERENCE: http://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/rtref/strcasecmp.htm
            {
                return true; 
            }
            else 
            {
                cursor = cursor->next; // if it still hasn't found it, go to the next node in linked list until NULL
            }

        }
        return false; // indicating word is not in dictionary
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //don't forget to declare a file pointer to point at actual dictionary (you're only given a dictionary name in the argument!)
    FILE *file = fopen(dictionary,"r");

    
    char new_word[LENGTH + 1]; //prepare a char array for new words to be put into as temporary storage
    
    //do scanning of dictionary until EOF is found. Scanning with fscanf and input is stored inside new_word
    while (fscanf(file,"%s", new_word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            fclose(file);
            return false;
        }
        else
        {
            //put new word into new node
            strcpy(new_node->word, new_word);
            
            //hash the new word to get a proper index
            int index = hash_dict(new_node->word);
            
            //put the new node into proper index
            new_node->next = hashtable[index];
            hashtable[index] = new_node;
            dict_size++;
        }
        
    }
    loaded = true;
    fclose(file);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(loaded == true)
    {
        return dict_size;
    }    
    else return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //iterate for each node in the hashtable, and perform linked list checking and freeing for each node
    for (int i = 0; i<26; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            //create a temporary node to point to the cursor first
            node *temp = cursor;
            
            //then we can safely assign the cursor to the next node in the linked list first
            cursor=cursor->next;
            
            //before we free the current node. This would prevent us losing access to the rest of the linked list, and thus causing them to
            //not be freed.
            free(temp);
        }
    }
    return true;
}
