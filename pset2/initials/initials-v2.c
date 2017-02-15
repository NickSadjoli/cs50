/**
 * Initials.c but with method of putting initials into a char array first 
 */

#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h> //dun forget this for toupper!

int main ()
{
    string name = get_string();
    int n =strlen(name);
    char initials[n];
    
    for ( int i = 0, j=0; i<n; i++) //better implementation instead of i = 0; i <strlen(), since it would ask for 
                                            //strlen in every for loop
    {
        if (i == 0)
        {
            if (name[i] != ' ')
            {
                initials[0] = (toupper(name[0])); //not using printf, since putchar is better optimized for char types
                j++;
            }
        }
        else if (name[i-1] == ' ' && name[i] != ' ')
            {
                initials[j] = (toupper(name[i]));
                j++;
            }
    }
    
    printf ("%s\n",initials);
    
}