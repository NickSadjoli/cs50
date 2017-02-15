#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h> //dun forget this for toupper!

int main ()
{
    string name = get_string();
    
    int i = 0, n =strlen(name);
    
    for ( i = 0; i<n; i++) //better implementation instead of i = 0; i <strlen(), since it would ask for 
                                            //strlen in every for loop
    {
        if (i == 0)
        {
            if (name[i] != ' ')
            {
                putchar(toupper(name[0])); //not using printf, since putchar is better optimized for char types
            }
        }
        else if (name[i-1] == ' ' && name[i] != ' ')
            putchar(toupper(name[i]));
    }
    
    printf ("\n");
    
}