#include <stdio.h>
#include <cs50.h>

int getheight();
void printpyramid (int h);
void printgap(int g);
void printhash(int hash);

int main(void)
{
    int height = getheight();
    printpyramid(height);

}

int getheight()
{
    printf("Height of pyramid: ");
    int height = get_int();
    while (height>23 || height <0 )
    {
        getheight();
        
    }
    
    return height;
    
}

void printpyramid(int h)
{
    int i=0;
    for (i=1;i<=h;i++)
    {
        printgap(h-i);
        printhash(i);
        printgap(2);
        printhash(i);
        printf("\n");
    }
}

void printgap(int g)
{
    int i=0;
    for(i=0;i<g;i++) printf (" ");

    
}

void printhash(int hash)
{
    int i=0;
    for (i=0;i<hash;i++) printf("#");
}