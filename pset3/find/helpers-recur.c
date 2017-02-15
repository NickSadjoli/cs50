/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

bool recur (int value, int values[], int init, int end);

bool recur (int value, int values[], int init, int end) //recursive function to be used for binary search
{
        if (value == values[(end+init)/2]) return true;
        else if (end<init) return false;
        else if (value < values[(end+init)/2])
        {
            return recur(value,values,init,(end+init)/2-1);
        }
        
        else if (value > values[(end+init)/2])
        {
            return recur(value,values,(end+init)/2+1,end);
        }
        
        return false;
}

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    return recur(value,values,0,n-1);
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
