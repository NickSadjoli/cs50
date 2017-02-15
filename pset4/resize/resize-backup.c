/**
 * Resizes a BMP according to a factor of f given by user, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

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
            
            if(j==0) //first pixel of infile's scanline must be printed into outfile
            { 
                fread(&triple,sizeof(RGBTRIPLE),1,inptr);
                counter++;
                fwrite(&triple, sizeof(RGBTRIPLE),1,outptr);
            }
            
            else if (f>=1.0) //if float f is more than 1 (resizing to be equal size or larger)
            {
                if(counter>=f) /*only update triple (or move pointer in infile) if counter is more than or equals f. 
                                    This will result in some pixels to be written multiple times*/
                {
                    // read and update RGB triple from the read pixel in infile
                    fread(&triple,sizeof(RGBTRIPLE),1,inptr)
                    counter=1; //reset counter to 1, NOT 0, since this will make counter effectively restart from beginning of 
                               //scanline and 1 more pixel for a value of f
                }
                
                else 
                    {
                        fseek(inptr,-sizeof(RGBTRIPLE),SEEK_CUR);   
                        counter++; //only update counter and NOT RGBTRIPLE
                    }
                
                // write current RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
            else if (f<1.0) //if float f is less than 1 (resizing to be smaller)
            {
                if((counter/f)>=bi.biWidth) /**if counter/f > widthsize (since f<1, counter/f will always result > 1, 
                                            execute read twice to skip one pixel in infile*/
                {   
                    //read and update RGB triple from infile twice (basically moving inptr twice and skipping a pixel, reducing size)
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    counter=1;
                }
                else 
                {
                    fread(&triple,sizeof(RGBTRIPLE),1,inptr); //else, read next pixel from infile as usual
                    counter++;
                }
                
                //write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
            }
            
        }
        
        // skip over padding, if any
        fseek(inptr, inpadding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < outpadding; k++)
        {
            fputc(0x00, outptr);
        }
        
    
        if (i==0) scancounter++;
        
        else if (f>=1.0) //if float f is more than 1 (resizing to be equal size or larger)
            {
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
                                                              
                    scancounter++; //similar to the resizing horizontally, scancounter is instead reset to 1, NOT 0
                }
                
                else if (scancounter>=f) //when scancounter is more than f, allow scanline pointer to continue
                {
                    scancounter = 1; //reset scancounter 
                }
            }
            
        else if (f<1.0) //if float f is less than 1 (resizing to be smaller)
            {
                if ((scancounter/f)>=biHeight) //if scancounter/f > biHeight, then skip a scanline
                {
                    //move pointer in infile ahead by one width (which means including stepping over through any padding), 
                    //to effectively print the same scanline again
                    
                      fseek(inptr, (sizeof(RGBTRIPLE)*initwidth), SEEK_CUR); //careful not to use the CURRENT width!
                      fseek(inptr,inpadding,SEEK_CUR);
                      
                      /*Note that movement cannot be simply done by using inptr+=bi.biWidth; This would effectively 
                      make infile pointer to only move forward by a certain amount of bytes pointing somewhere inside 
                      the same scanline again (perhaps even pointing content of RGBTRIPLE struct. Yes remember that 
                      the pixels that we iterated in is actually a struct!), and can cause scanning error*/
                                                                
                    scancounter = 1; //similar to the resizing horizontally, scancounter is instead reset to 1, NOT 0
                }
                
                else scancounter++;
            }
            
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
