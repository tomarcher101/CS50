#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <stdint.h>

int main(int argc, string argv[])
{
    // producer error output if input commands = 1
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // set input file pointer to argv[1]
    char *infile = argv[1];
    
    // open input file
    // producer error if input file cannot be opened
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open specified file\n");
        return 2;
    }
    
    uint8_t buffer[512]; // set 512 byte block to use as buffer
    int jpgcount = 0; // set jpeg counter
    FILE *outptr = NULL; // set outfile pointer to NULL
    
    // start read loop
    while(fread(buffer, 512, 1, inptr))
    {
        if (buffer[0] == 0xff && // check if start of jpeg
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // if start of JPEG and there is currently a JPEG being written
            // close currently written JPEG
            if (outptr != NULL)
                fclose(outptr);
                
            // start new output file
            char filename[30];
            sprintf(filename, "%03d.jpg", jpgcount);
            outptr = fopen(filename, "w");
            jpgcount++;
        }
        
        // If not start of JPEG check if there is currently a JPEG being written
        
        // If yes carry on writing current JPEG
        if (outptr != NULL)
            fwrite(buffer, 512, 1, outptr);
    }

    // If done reading close current output
    fclose(outptr);
    fclose(inptr);
    
    return 0;
}