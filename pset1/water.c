#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Time for shower: ");
    int t = get_int();
    
    printf("Number of water bottles used: %i \n", t*12);
}
