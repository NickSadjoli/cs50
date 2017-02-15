/** Program's function is to resize an input file by the amount of input f to an output file. 
 *  resizing works by 
*/


#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main (int argc, char *argv[] ) 
{
    
    //ensure only 3 arguments are inputted
    if (argc != 4)
    {
        fprintf(stderr, "Usage: needs 3 input arguments! \n");
        return 1;
    }
    
    
    //initialize infile and outfile 
    float f = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    
    //checks whether float is between 0.0 (not including 0.0) and 100.0 (inclusive) range
    
    if (f<=0.0 || f>100)
    {
        fprintf(stderr, "Usage: please input a floating point number that is in the range between 0.0 (not inclusive) and 100.0 (inclusive)");
        return 1;
    }
    
    
    
    //check and open input file
    FILE *inptr = fopen(infile,"r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Cannot open %s file, make sure you have inputted a correct file! \n", infile); //infile (which is 
                                                                                                        //basically a file's name)
                                                                                                        //acts as pointer to a file,
                                                                                                        //hence %s here would print 
                                                                                                        //the 'file name'
        return 1;
    }
    

    //check and open output file
    FILE *outptr = fopen(outfile,"w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Cannot write to %s file, make sure you have inputted a correct output file name! \n", outfile); //similar to the case above
        return 1;
    }
    
    //initialize file's header and info header
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER),1,inptr);
    
    BITMAPINFOHEADER bi;
    fread(&bi,sizeof(BITMAPINFOHEADER),1,inptr);
    
    //note that now the pointer of the fread for the infile is already somewhere after BITMAPINFOHEADER (thus probably at 54th bit)
    
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);//close output file
        fclose(inptr);//close input file
        fprintf(stderr, "Unsupported file format. Needs to be BMP File\n");
        return 4;
    }
    
    //continue if input and output files are legit
    
    
    /**
     * Since the file will be resized by the factor of floating point f, then the some parameters of the size needs to be changed.
     * In particular, we need to change some fields as per specified in the BITMAPFILEHEADER and BITMAPINFOHEADER for the new output file.
     * - Hence in the INFOHEADER (bi), we need to change the fields specifying the pixel paramaters, and any fields affected by them. 
     *   This means changing the biSize (size of structure needed), biWidth (width of bitmap in pixels), and biSizeImage(size of Image in bytes)
     * - Meanwhile, in the FILEHEADER (bf), we need to change the bfSize field since we are resizing the size of the whole bitmap file itself
    */
    
    /**if already confirmed to be valid, FIRST need to change the bi (BITMAPINFOHEADER) fields first, since it affects bf 
    (BITMAPFILEHEADER) too */
    
    // determine padding for scanlines  when reading input file
    int inpadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    
    //declare variables to store initial values of width, height, and other relevant fields
    int initwidth = bi.biWidth; 
    int initheight = bi.biHeight;  

        
    bi.biWidth *= f;
    bi.biHeight *= f;
    //declare new padding for outfile
    int outpadding = (4 - (bi.biWidth* sizeof(RGBTRIPLE)) % 4) % 4;
    
    //calculating new sizeimage for outfile
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth)+outpadding)*abs(bi.biHeight); 
    /*the logic is basically calculating the size of the new rows due to the new biWidth, and then multpying 
    it by the new biHeight to give the actual Size of the Image (like calculating surface area) */
    
    //if already confirmed to be valid, resize BITMAPFILEHEADER's bf.Size field for the new outfile
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER); // same size, but with different contents/fields 
    
    
                                                    

    // write outfile's BITMAPINFOHEADER (biout) copying outfile's BITMAPINFOHEADER if it was confirmed legit
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    
    // write outfile's BITMAPFILEHEADER (bfout), copying infile's BITMAPFILEHEADER if it was confirmed legit
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    

    //note that now the pointer of the fwrite for the outfile is already somewhere after BITMAPINFOHEADER (thus probably at 54th bit)
    

    // iterate over outfile's scanlines. Note that pixels are stored more like a matrix (with each row being basically scanlines)
    for (int i = 0, biHeight = abs(initheight); i < biHeight; i++)
    {
        // iterate over pixels in scanline for outfile
        for (int j = 0; j < initwidth; j++)
        {
            // struct declaring RGBTriple, and acting as temporary storage
            RGBTRIPLE triple;
            
            fread(&triple,sizeof(RGBTRIPLE),1,inptr);
            
                                
            /**NOTE: concerning the above action, we cannot immediately use fwrite a pixel f times, since f is not an integer number. 
             * Note that based on the hints, the better way we write out the rgbtriple in the outfile would be to have the 
             * index of pointer at outfile (j) be compared to the factor float f, such that while it is not more than f, it will 
             * keep reading from the same spot in the infile. This would basically position the new out pixel to be corresponding to 
             * the correct pixel (based on its indexing) from the infile
             * index at infile *divided* by factor of f , i.e. outfile[i] = infile[i/f]. Hence
            */

            // write RGB triple to outfile, with outfile being spaced according to the pattern that outfile[i]=infile[i/f]
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // when reading ,skip over padding in the infile, if any
        fseek(inptr, inpadding, SEEK_CUR);

        // then add it back to the outfile when writing (to demonstrate how)
        for (int k = 0; k < outpadding; k++)
        {
            fputc(0x00, outptr); //cannot use fread or fwriting on padding, since it is NOT an RGBTRIPLE, and instead a 0x00!
        }
    }

    

    //close outfile
    fclose(outptr);
    
    //close infile
    fclose(inptr);
    
    free(inptr);
    free(outptr);
    
    
    return 0; //success
}