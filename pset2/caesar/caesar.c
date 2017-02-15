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
    
    int k = atoi(argv[1]);
    printf("plaintext: ");
    string plaintext = get_string();
    int length = strlen(plaintext);
    char ciphertext[length+1];
    
    for ( int i = 0; i<=length; i++) //better implementation instead of i = 0; i <strlen(), since it would ask for 
                                            //strlen in every for loop
    {
        if (isalpha(plaintext[i]))
        {
            //if letter is a capital letter
            if (isupper(plaintext[i]))
            {
                //turn plaintext into alphabetical first, then put into cipher text before being turned back to ASCII index
                ciphertext[i] = (((plaintext[i]-65)+k)%26)+65; //65 = ASCII value of 'A'
            }
            
            //if letter is a lowercase letter
            else if (islower(plaintext[i]))
            {
                //turn plaintext into alphabetical first, then put into cipher text before being turned back to ASCII index
                ciphertext[i] = (((plaintext[i]-97)+k)%26)+97; //97 = ASCII value of 'a'
            }
        }
        else if (plaintext[i] == '\0') ciphertext[i] = '\0';
        else ciphertext[i] = plaintext[i];
        
    }
    
    printf ("ciphertext: %s\n",ciphertext);
    return 0;
}