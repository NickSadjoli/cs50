#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

#include "bmp.h"

int main (int argc, char *argv[] ) 
{
    
    //ensure only 2 arguments are inputted
    if (argc != 3)
    {
        fprintf(stderr, "Usage: needs 2 input arguments! \n");
        return 1;
    }
    
    
    //initialize infile and outfile pointers
    char *infile = argv[1];
    char *outfile = argv[2];
    
    //check and open input file
    FILE *inptr = fopen(infile,"r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Cannot open %s file, make sure you have inputted a correct file! \n", infile); //infile (which is 
                                                                                                        //basically a file's name)
                                                                                                        //acts as pointer to a file,
                                                                                                        //hence %s here would print 
                                                                                                        //the 'file name'
        return 2;
    }
    
    //check and open output file
    FILE *outptr = fopen(outfile,"w");
    if (outptr == NULL)
    {
        fprintf(stderr, "Cannot write to %s file, make sure you have inputted a correct output file name! \n", outfile); //similar to the case above
        return 3;
    }
    
    //initialize file's header and info header
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER),1,inptr);
    
    BITMAPINFOHEADER bi;
    fread(&bi,sizeof(BITMAPINFOHEADER),1,inptr);
    
    //note that now the pointer of the fread for the infile is already somewhere after BITMAPINFOHEADER (thus probably at 54th bit)
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    //continue if input file is legit
    
    // write outfile's BITMAPFILEHEADER, copying infile's BITMAPFILEHEADER if it was confirmed legit
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER, copying outfile's BITMAPINFOHEADER if it was confirmed legit
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    //note that now the pointer of the fwrite for the outfile is already somewhere after BITMAPINFOHEADER (thus probably at 54th bit)
    
    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines. Note that pixels are stored more like a matrix (with each row being basically scanlines)
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // struct declaring RGBTriple, and acting as temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            //note that clue.bmp contains MANY pure red pixels (pixels containing only red colors). 
            //Hence it might be better doing the following:
            if ((triple.rgbtRed==0xff) && (triple.rgbtGreen == 0x00) && (triple.rgbtBlue == 0x00)) //meaning that the pixel has full red 
                                                                                                   //and no other color set. Clue only 
                                                                                                   //has other colors set for the actual 
                                                                                                   //face it seems
   
                {                               
                    triple.rgbtGreen=0xff;
                    triple.rgbtBlue=0xff;   //basically turning the pixel into a white pixel so it's easier to read the clue. Note that 
                                            //setting the value of all rgbt (Green, Blue and Red) of the pixel would turn it into a black pixel 
                                            //instead
                }                                                                                   
            

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr); //cannot use fread or fwriting on padding, since it is NOT an RGBTRIPLE
        }
    }

    
    

    //close outfile
    fclose(outptr);
    
    //close infile
    fclose(inptr);
    
    return 0; //success
}