#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check user input two arguments
    if (argc != 2)
    {
        printf("usage: %s infile", argv[0]);
        return 1;
    }

    char *infile = argv[1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    // check *inptr is valid
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // Calculate lenght of infile
    fseek(inptr, 0, SEEK_END);
    int len = ftell(inptr);

    // Go back to the beginning of infile
    rewind(inptr);

    // Calculate number of blocks. each bloack has 512 bytes
    int n_blocks = len / 512;
    // check for slack space
    if ((len % 512) > 0)
    {
        n_blocks = n_blocks + 1;
    }

    // Initialize count for the images
    int image_count = 0;

    // Initialize pointer for images files
    FILE *img = NULL;

    // Buffer or temp storage. char
    unsigned char *block = malloc(sizeof(BYTE) * 512);

    // iterate over each block of the card
    for (int i = 0; i < n_blocks; i++)
    {
        // check if block has 512 bytes to avoid going into a slack space
        if (fread(block, sizeof(BYTE), 512, inptr) == 512)
        {
            // find beginning of JPEG
            if (block[0] == 0xff &&
                block[1] == 0xd8 &&
                block[2] == 0xff &&
                (block[3] & 0xf0) == 0xe0)
            {
                // Prepare names for images
                char file_name[sizeof"999.jpg"];
                sprintf(file_name, "%03d.jpg", image_count);

                // Check if JPEG is the not first in order to close previous JPEG
                if (image_count != 0)
                {
                    fclose(img);
                }

                // Open new file for writing
                img = fopen(file_name, "w");

                // Store JPEG in file
                fwrite(block, sizeof(BYTE), 512, img);

                image_count++;
            }
            // check if block is part of JPEG but not the first block of the JPEG
            else if (image_count != 0)
            {
                // Store in open JPEG file
                fwrite(block, sizeof(BYTE), 512, img);
            }
        }
    }
    // close last working JPEG
    fclose(img);
    // free malloc from the heap
    free(block);
    // close infile
    fclose(inptr);
    return 0;
}
