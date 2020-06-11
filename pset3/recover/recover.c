#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    //Check entry of 1 argument
    if (argc != 2)
    {
        printf("Usage: %s infile", argv[0]);
        return 1;
    }

    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // Calculate lenght of file
    fseek(inptr, 0, SEEK_END);
    int len = ftell(inptr);

    // Go back to the beginning of the file
    rewind(inptr);


    // calculate number of blocks
    int n = (len / 512);
    if ((len % 512) > 0)
    {
        n = n + 1;
    }

    // initialize counter for img
    int count = 0;

    //initialise pointer for the images
    FILE *img = NULL;

    // temporary storage
    unsigned char *b_arr = malloc(sizeof(unsigned char) * 512);

    // iterate over each block
    for (int i = 0; i < n; i++)
    {
        // read each byte from infile only if block has 512 bytes, so it won't process the block with the end of the file - EOF if it has less than 512
        if (fread(b_arr, sizeof(unsigned char), 512, inptr) == 512)
        {
            // check first 4 bytes
            if (b_arr[0] == 0xff && b_arr[1] == 0xd8 && b_arr[2] == 0xff && (b_arr[3] >= 0xe0 && b_arr[3] <= 0xef))
            {

                //is JPEG
                //printf("it's JPEG\n");

                // create secuentially file names
                char filename[sizeof"999.jpg"];
                sprintf(filename, "%03d.jpg", count);
                //printf("%s ",filename);

                // close img if opened
                if (count != 0)
                {
                    fclose(img);
                }

                // open new file for writing
                img = fopen(filename, "w");

                //store in new file
                fwrite(b_arr, sizeof(unsigned char), 512, img);

                count++;
            }


            else if (count != 0)
            {
                //printf("%i part of JPEG\n", count);

                // store in opened file
                fwrite(b_arr, sizeof(unsigned char), 512, img);
            }

            else
            {
                //printf("Not a JPEG\n");
            }
        }

    }

    // free malloc from the heap
    free(b_arr);

    // close img
    fclose(img);

    // close infile
    fclose(inptr);

    // Success
    return 0;
}
