/**
 * Implementing caesar cipher
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h> //need this for atoi!
#include <ctype.h> //need this for isupper and islower!
#include <cs50.h>

int main (int argc, char* argv[])
{
    if (argc != 2 )
    {
        printf("Usage: ./caesar k");
        return 1;
    }
    
    int klen = strlen(argv[1]);
    char k[klen];
    
    //check whether input is all alphabetical
    for (int i=0; i<klen;i++)
    {
        if (!isalpha(argv[1][i])) 
        {
            printf("Usage: ./caesar k. k is all alphabetical string!");
            return 1;
        }
        
        else k[i]=tolower(argv[1][i]);
    }
    
    printf("plaintext: ");
    string plaintext = get_string();
    int length = strlen(plaintext);
    char ciphertext[length+1];
    
    for ( int i = 0, j=0; i<=length; i++) //better implementation instead of i = 0; i <strlen(), since it would ask for 
                                            //strlen in every for loop
    {
        if (isalpha(plaintext[i]))
        {
            //if letter is a capital letter
            if (isupper(plaintext[i]))
            {
                //turn plaintext into alphabetical first, then put into cipher text before being turned back to ASCII index
                ciphertext[i] = (((plaintext[i]-65)+(k[j%klen]-97))%26)+65; //65 = ASCII value of 'A', note that we mod j with klen to invoke wraparound
                                                                        // when j reaches klen. Need to substract this by 97 or 'a' to make it into 
                                                                        // decimal indexing (not ASCII indexing)
            }
            
            //if letter is a lowercase letter
            else if (islower(plaintext[i]))
            {
                //turn plaintext into alphabetical first, then put into cipher text before being turned back to ASCII index
                ciphertext[i] = (((plaintext[i]-97)+(k[j%klen])-97)%26)+97; //97 = ASCII value of 'a'
            }
            
            j++; //increment pointer for k
        }
        else if (plaintext[i] == '\0') ciphertext[i] = '\0';
        
        else ciphertext[i] = plaintext[i];
        
    }
    
    printf ("ciphertext: %s",ciphertext);
    printf ("\n");
    return 0;
}