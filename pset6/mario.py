import cs50

def main():
    height = getheight()
    printpyramid(height)

def getheight():
    height = int(input("Height of pyramid: "))
    while height > 23 or height < 0:
        height = getheight()
    return height

def printpyramid(h):
    for i in range(1,h+1):
        {
            #note the reduction of the supposed gaps by -1! This is because, by default
            #the concatenation of print (i.e. those ',') would print an extra " " automatically
            print(" "*(h-i-1), "#" * i, " " * 1, "#" * i)
        }

if __name__ == '__main__':
    main()