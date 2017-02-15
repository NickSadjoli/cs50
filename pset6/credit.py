import cs50

def checksum (c):
    card_length = 0
    first_two = 0
    
    ##other = even numbers, not odd ones!
    other_sum = 0
    nonother_sum = 0
    while c > 0: 
        card_length+=1
        if card_length % 2 == 0 :
            if (c % 10 * 2) >= 10:
                other_sum += (c%10 * 2) + 1
            else:
                other_sum += (c%10 * 2)
        else:
            nonother_sum += (c%10)
        c=c//10 ##unlike in c, normal division would not round integer, since it would produce a float
        if c < 100 and c >=10:
            first_two = c
            
    if ((card_length != 13 and card_length is not 15 and card_length is not 16) or (((other_sum + nonother_sum) % 10) is not 0)):
        print("INVALID\n")
        
    else:
        if card_length == 15 or (first_two == 34 and first_two == 37):
            print ("AMEX\n")
        elif (card_length == 16 and (51 <= first_two and first_two <= 55)):
            print ("MASTERCARD\n")
        elif (card_length == 13 or card_length == 16) and (39 < first_two and first_two < 50):
            print ("VISA\n")
            
def main():
    print ("Input credit card number here: ", end="")
    card_num = cs50.get_int() ##to take care of the possibility of user inputting a non-integer input
    checksum(card_num)
            
if __name__ == '__main__':
    main()