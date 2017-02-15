import cs50
import sys ##needed for sys.argv

if len(sys.argv) is not 2:
    print ("Usage: ./caesar k")
    sys.exit(1) ##replacing return 1 in c
k = int(sys.argv[1]) ##remember that sys.argv is all read as strings
plaintext = input("plaintext: ")
ciphertext = []

for c in plaintext:
    if c.isalpha():
        if c.isupper():
            n = ord(c) ##returns the ASCII value of Unicode character c
            ciphertext.append(chr((((n - 65) + k) % 26) + 65)) ##chr is opposite of ord(), returning string 
                                                               ##character with matching unicode integer value
        elif c.islower():
            n = ord(c) ##returns ASCII value of Unicode character c
            ciphertext.append(chr((((n - 97) + k) % 26) + 97))
    else: 
        ciphertext.append(c) 
print ("ciphertext is:", ''.join(ciphertext)) ##use join so that it is not printed as a list
sys.exit()
    