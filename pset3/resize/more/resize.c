#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    //Check entry of 4 arguments
    if (argc != 4)
    {
        printf("Usage: %s n infile outfile", argv[0]);
        return 1;
    }

    float n = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    //Check n is valid
    if (n < 0 || n > 100)
    {
        printf("Invalid number. Please enter a value 0 to 100 ");
        return 2;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }
    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(outptr);
        printf("Could not create %s.\n", outfile);
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
        printf("Unsupported file format.\n");
        return 4;
    }

    // determine padding for scanlines
    int padding_old = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_new = (4 - ((bi.biWidth * n) * sizeof(RGBTRIPLE)) % 4) % 4;


    // update values for outptr

    // read outfile's BITMAPFILEHEADER
    BITMAPFILEHEADER bfout = bf;
    fread(&bfout, sizeof(BITMAPFILEHEADER), 1, outptr);

    // read outfile's BITMAPINFOHEADER
    BITMAPINFOHEADER biout = bi;
    fread(&biout, sizeof(BITMAPINFOHEADER), 1, outptr);

    biout.biWidth = bi.biWidth * n;
    biout.biHeight = bi.biHeight * n;
    biout.biSizeImage = (((sizeof(RGBTRIPLE) * biout.biWidth) + padding_new) * abs(biout.biHeight));
    bfout.bfSize = 54 + biout.biSizeImage;


    // write outfile's BITMAPFILEHEADER
    fwrite(&bfout, sizeof(BITMAPFILEHEADER), 1, outptr);


    // write outfile's BITMAPINFOHEADER
    fwrite(&biout, sizeof(BITMAPINFOHEADER), 1, outptr);



    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // write n times the scanline vertically
        for (int s = 0; s < n; s++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);


                // write RGB triple to outfile the pixel n times horizontally
                for (int p = 0; p < n; p++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // Go back to read first pixel
            fseek(inptr, -(bi.biWidth * 3), SEEK_CUR);

            // then add padding to new image using padding_new
            for (int k = 0; k < padding_new; k++)
            {
                fputc(0x00, outptr);
            }

        }
        // skip over the scanline (after previous fseek the cursor its at the begining of the scanline) and padding_old, if any padding
        fseek(inptr, ((bi.biWidth * 3) + padding_old), SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}