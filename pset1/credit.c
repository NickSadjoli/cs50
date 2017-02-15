#include <cs50.h>
#include <stdio.h>

void checksum(long long int c);


int main (void)
{
    printf("Input credit card number here: ");
    long int card_num=get_long_long();
    checksum(card_num);
}

void checksum(long long int c)
{
    int card_length=0;
    int first_two=0;
    
    //other = even numbers! (hence indexes 2, 4, 6, ... if we're using 1-indexing)
    int other_sum=0;
    int nonother_sum=0;
    while (c>0)
    {
        card_length++;
        if (card_length%2==0)
        {
            if ((c%10*2)>=10) other_sum+=((c%10*2) + 1);
            else other_sum+=(c%10*2);
            
        }
        else
        {
            nonother_sum+=(c%10);
        }
        
        c=c/10;
        if(c<100 && c>=10) first_two=c;
    }
    
    
    if((card_length!=13 && card_length!=15 && card_length!=16)||(((other_sum+nonother_sum)%10)!=0)) 
    {
        printf("INVALID\n");
    }
    else
    {
        if ((card_length==15) && (first_two==34||first_two==37)) printf("AMEX\n");
        else if ((card_length==16) && (51<=first_two && first_two<=55)) printf("MASTERCARD\n");
        else if (((card_length==13)||(card_length==16)) && (39<first_two && first_two<50)) printf("VISA\n");
        
    }
}