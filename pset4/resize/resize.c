/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // use argvs to set the factor or resizing and infile / outfile pointer
    char *infile = argv[2];
    char *outfile = argv[3];
    int n = atoi(argv[1]);
    
    
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
    
    // set new bf and bi
    BITMAPFILEHEADER nbf;
    BITMAPINFOHEADER nbi;
    nbf = bf;
    nbi = bi;
    
    // resize width, height
    nbi.biWidth = bi.biWidth * n;
    nbi.biHeight = bi.biHeight * n;
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int npadding = (4 - (nbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // resize bisizeimage and bfsize
    nbi.biSizeImage = ((sizeof(RGBTRIPLE) * nbi.biWidth) + npadding) * abs(nbi.biHeight);
    nbf.bfSize = nbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER - change read pointer structure to nbf / nbi
    fwrite(&nbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&nbi, sizeof(BITMAPINFOHEADER), 1, outptr);
    

    // iterate over the infile's scanlines (rows)
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // repeat n-1 times for verticle expansion
        for (int v = 0; v < n; v ++)
        {
            // iterate over pixels in scanline (row)
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // write RGB triple to outfile n times
                for (int l = 0; l < n; l++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); 
            }
        
            // add padding at the end of scanline
            for (int k = 0; k < npadding; k++)
                fputc(0x00, outptr);
            
            // Go back to beginning of scanline if line has not been repeated n times
            // this makes each row repeat itself n times
            // Use of "recopy" method
            if (v < n - 1)
            {
                long offset = bi.biWidth * sizeof(RGBTRIPLE);
                fseek(inptr, -offset, SEEK_CUR);
            }
                // fseek(inptr, -(bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR); kept providing error:
                // negation of 24 cannot be represented in type 'unsigned long'
                // so made new variable
        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
