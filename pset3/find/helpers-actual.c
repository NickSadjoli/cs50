/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // implement binary search, assuming list is sorted
    int init = 0, end = n-1, middle = 0;

    while (end >= init) //need '=' sign for edge cases (i.e. if the value turns out to be on the edge of the arrays)
    {
        middle = (end+init)/2; //NOT end-init!
        if (value == values[middle]) return true;
        else if (value < values[middle])
        {
            end = middle-1; //more efficient since we have checked for middle too.
        }
        
        else if (value > values[middle])
        {
            init = middle+1;
        }

    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // implementing counting sort for O(n) runtime algorithm
    long count[65537];
    for (int i = 0; i < n; i++)
    {
        count[values[i]]++;
    }
    
    for (int k=0, j=0; j<n; k++)
    {
        if(count[k]!=0)
        {
            values[j]=k;
            count[k]--;
            k--; //decrement to repeat the same count
            j++;
        }
    }
    
    return;
}
