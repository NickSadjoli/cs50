import cs50
import sys ##needed for sys.argv

if len(sys.argv) is not 2:
    print ("Usage: ./caesar k")
    sys.exit(1) ##replacing return 1 in c
    
##lower all argument inputs as vigenere wants to treat them all the same (either upper or lowercase will be treated as lowercase)    
k = sys.argv[1].lower() ##remember that sys.argv is all read as strings
klen=len(k) #length of vigenere key k

for j in k:
    if not j.isalpha(): 
            print("Usage: ./caesar k. k is all alphabetical string!");
            sys.exit(1);

kp = 0 ##index pointer to k
plaintext = input("plaintext: ")
ciphertext = []

for c in plaintext:
    if c.isalpha():
        if c.isupper():
            n = ord(c) ##returns the ASCII value of Unicode character c
            ciphertext.append(chr((((n - 65) + (ord(k[kp%klen])-97)) % 26) + 65)) ##chr is opposite of ord(), returning string 
                                                               ##character with matching unicode integer value
            
        elif c.islower():
            n = ord(c) ##returns ASCII value of Unicode character c
            ciphertext.append(chr((((n - 97) + (ord(k[kp%klen])-97)) % 26) + 97))
        
        kp+=1 ##increment pointer kp to point to next letter in k
    
    else: 
        ciphertext.append(c) 
print ("ciphertext is:", ''.join(ciphertext)) ##use join so that it is not printed as a list
sys.exit()
    