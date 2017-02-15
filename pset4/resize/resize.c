/**
 * Resizes a BMP according to a factor of f given by user
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: Needs 3 arguments exactly!\n");
        return 1;
    }

    // remember filenames and float scaler
    float f = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding for infile's scanlines
    int inpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    int initwidth=bi.biWidth; //will be used for later

    //updates infoheader fields for outfile
    bi.biWidth *= f;
    bi.biHeight *= f ;
    
    //determine padding for outfile's scanlines
    int outpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //updates sizeimage and file size fields for outfile
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + outpadding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
     
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    

    // iterate over outfile's scanlines
    for (int i = 0, scancounter=0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0, counter=0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            if (f>=1.0 && ((f-(int)f)==0)) //if float f is more than or equals 1 (resizing to be equal size or larger)
            {
                fread(&triple,sizeof(RGBTRIPLE), 1, inptr);
                counter++;
                
                if(counter<f) //keep reading the same pixel until  counter reaches f times
                {
                    // keep the pointer to be on the same pixel using fseek which moves infile pointer
                    fseek(inptr,-(long)sizeof(RGBTRIPLE),SEEK_CUR);
                }
                
                else if (counter>=f)
                {
                    counter=0; //counter reset
                }
                
                // write current RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
            else if (f<1.0) //refer to here: http://stackoverflow.com/questions/4668407/check-whether-a-float-number-contains-decimals-or-not
            {
                int jump = floor(1/f)-1; /*case of f<1, meaning resizing to be smaller. Remember 
                                                    that we are using index 0, and that every fread will increment 
                                                    pointer by 1*/
        
                
                
                fread(&triple,sizeof(RGBTRIPLE),1,inptr);
                
                //write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
                fseek(inptr,jump*(sizeof(RGBTRIPLE)),SEEK_CUR); //skipping some pixels to make possible of smaller width
            }
            
            else if (f>1.0)
            {
                int del = floor(f)-1;
                
                fread(&triple,sizeof(RGBTRIPLE),1,outptr);
                counter++;
                fwrite(&triple,sizeof(RGBTRIPLE),1,outptr);
                if(counter%del==0)
                {
                    fseek(inptr,-(long)sizeof(RGBTRIPLE),SEEK_CUR); //basically having this particular pixel to be rewritten into outfile
                }
            }

            
        }
        
        // skip over padding, if any
        fseek(inptr, inpadding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < outpadding; k++)
        {
            fputc(0x00, outptr);
        }
        
    
        //resizing vertically. This will be done in a similar manner to that when resizing horizontally
        
        if (f>=1.0&& ((f-(int)f)==0)) //if float f is more than 1 (resizing to be equal size or larger)
            {
                scancounter++;
                if (scancounter<f) // while scancounter is less than f, then don't update the scanline pointer in infile (i), 
                                         // and write the same scanline into outfile multiple times
                {
                    //move pointer in infile back by one width (which means including steppig back through any padding), 
                    //to effectively print the same scanline again
                    
                    fseek(inptr, -(inpadding), SEEK_CUR); 
                    fseek(inptr, -((long)sizeof(RGBTRIPLE)*initwidth), SEEK_CUR); //careful not to use the CURRENT width!
                      
                    /*Note that movement cannot be simply done by using inptr-=bi.biWidth; This would effectively 
                    make infile pointer to only move back by a certain amount of bytes pointing somewhere inside 
                    the same scanline again (perhaps even pointing content of RGBTRIPLE struct. Yes remember that 
                    the pixels that we iterated in is actually a struct!), and can cause scanning error*/
                                                              
                }
                
                else if (scancounter>=f) //when scancounter is more than f, allow scanline pointer to continue
                {
                    scancounter = 0; //reset scancounter 
                }
            }
            
            
        else if (f<1.0) //if float f is less than 1 (resizing to be smaller)
            {
                int jump = floor(1/f)-1;
                
                fseek(inptr,jump*((sizeof(RGBTRIPLE)*initwidth)+inpadding),SEEK_CUR); //skipping some scanlines to make possible 
                                                                                     //of smaller width
                                                                                     
                /*Note that movement cannot be simply done by using inptr*=initwidth or something similar; This would 
                make infile pointer to only move forward by a certain amount of bytes pointing somewhere inside 
                the same scanline again (perhaps even pointing content of RGBTRIPLE struct. Yes remember that 
                the pixels that we iterated in is actually a struct!), and can cause scanning error*/
                
            }
            
        else if (f>1.0)
        {
        
            int del = floor(f)-1;
            scancounter++;
            if(scancounter%del==0)
            {
                //effectively restarting the whole scanline again.
                fseek(inptr, -(inpadding), SEEK_CUR); 
                fseek(inptr, -((long)sizeof(RGBTRIPLE)*initwidth), SEEK_CUR); //careful not to use the CURRENT width!
            }
        
        }
            
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}