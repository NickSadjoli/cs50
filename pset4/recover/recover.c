/**
 * Resizes a BMP according to a factor of f given by user
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // open image and assign it to raw_image pointer
    FILE *raw_image = fopen(argv[1],"r"); 
    
    //check whether you can open file pointed by raw_image
    if (raw_image == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    //define &buffer array of 512 bytes long
    unsigned char buffer[512]; 
    
    //define test variable to check whether each read is indeed 512 bytes long
    int test = fread(&buffer, 1, 512, raw_image);

    FILE *jpg_image = fopen(argv[1], "r"); //init jpg_image first
    
    int numofjpg = 0;
    
    char filename[7];
    
    bool jpgfound = true;
    
    while (test == 512) //perform looping until &buffer is not reading 512 bytes anymore (at which point it is probably Eof)
    {   
        //check whether we have found start of a new JPG. Note that & is bitwise AND operator in this case
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
            {
                
                if (jpgfound == true) //if jpgfound == true
                {
                    fclose(jpg_image);
                    sprintf(filename, "%03i.jpg", numofjpg);
                    // open resulting jpg file
                    jpg_image = fopen(filename, "w");
                    //perform checking
                    if (jpg_image == NULL)
                    {
                        fclose(jpg_image);
                        fprintf(stderr, "Could not create %s.\n", filename);
                        return 3;
                    }
                    else
                    {
                        fwrite(&buffer, 1, 512, jpg_image);
                        jpgfound = true;
                        numofjpg++;
                    }

                   
                }
                

            }
            
        else //if header is not a JPG file
        {
            if (jpgfound == true) // if previously there was a jpg file, it means that we're still in the same file
            {
                fwrite(&buffer, 1, 512,jpg_image); //write into the same jpg file
            }
            //if jpgfound == false, and there weren't any jpg files previously, do nothing
        }
    
    test = fread(&buffer, 1, 512, raw_image);
    }

    // close jpg_image
    fclose(jpg_image);

    // close raw_image
    fclose(raw_image);

    // success
    return 0;
}