#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //loop each scanline (row)
    for (int i = 0; i < height; i++)
    {
        //loop each pixel (column)
        for (int j = 0; j < width; j++)
        {
            // Get average value per pixel
            int new_RGB = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // assigned average value to each channel (RGB) in each pixel
            image[i][j].rgbtBlue = new_RGB;
            image[i][j].rgbtGreen = new_RGB;
            image[i][j].rgbtRed = new_RGB;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop each scanline (row)
    for (int i = 0; i < height; i++)
    {
        // Loop each pixel
        for (int j = 0; j < width; j++)
        {
            int originalBlue = image[i][j].rgbtBlue;
            int originalGreen = image[i][j].rgbtGreen;
            int originalRed = image[i][j].rgbtRed;

            // Calculate sepia values
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            if (sepiaBlue <= 0xff)
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
            else
            {
                image[i][j].rgbtBlue = 0xff;
            }

            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            if (sepiaGreen <= 0xff)
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
            else
            {
                image[i][j].rgbtGreen = 0xff;
            }

            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            if (sepiaRed <= 0xff)
            {
                image[i][j].rgbtRed = sepiaRed;
            }
            else
            {
                image[i][j].rgbtRed = 0xff;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop each scanline (row)
    for (int i = 0; i < height; i++)
    {
        // calculate the number of pixels and check if number is either even or odd
        // if even all pixels swap
        int half = width / 2;
        if (width % 2 != 0)
        {
            half = (width + 1) / 2;
        }

        // loop each pixel
        for (int j = 0; j < half; j++)
        {
            //swap first pixel with last pixel
            int new_width = width - 1 - j;
            int temp_Blue = image[i][j].rgbtBlue;
            int temp_Green = image[i][j].rgbtGreen;
            int temp_Red = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][new_width].rgbtBlue;
            image[i][new_width].rgbtBlue = temp_Blue;

            image[i][j].rgbtGreen = image[i][new_width].rgbtGreen;
            image[i][new_width].rgbtGreen = temp_Green;

            image[i][j].rgbtRed = image[i][new_width].rgbtRed;
            image[i][new_width].rgbtRed = temp_Red;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make a temporary copy of the original image
    RGBTRIPLE(*copy_image)[width] = calloc(height, (width * sizeof(RGBTRIPLE)));
    // make sure there is space for temporary memory
     if (copy_image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }

    for (int k = 0; k < height; k++)
    {
        for (int m = 0; m < width; m++)
        {
            copy_image[k][m].rgbtBlue = image[k][m].rgbtBlue;
            copy_image[k][m].rgbtGreen = image[k][m].rgbtGreen;
            copy_image[k][m].rgbtRed = image[k][m].rgbtRed;
        }
    }


    // loop each scanline (row)
    for (int i = 0; i < height; i++)
    {
        // loop each pixel
        for (int j = 0; j < width; j++)
        {
            // find neighboring pixels
            float total_pix;
            int i_before = i - 1;
            int i_after = i + 1;
            int j_before = j - 1;
            int j_after = j + 1;


            // is pixel in a corner?
            if ((i == 0 || (i == height - 1)) && (j == 0  || j == (width - 1)))
            {
                total_pix = 4.0;
                // left top corner?
                if (i == 0 && j == 0)
                {
                    image[i][j].rgbtBlue =  round((copy_image[i][j].rgbtBlue + copy_image[i][j_after].rgbtBlue + copy_image[i_after][j].rgbtBlue +
                                                   copy_image[i_after][j_after].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen =  round((copy_image[i][j].rgbtGreen + copy_image[i][j_after].rgbtGreen + copy_image[i_after][j].rgbtGreen +
                                                    copy_image[i_after][j_after].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed =  round((copy_image[i][j].rgbtRed + copy_image[i][j_after].rgbtRed + copy_image[i_after][j].rgbtRed +
                                                  copy_image[i_after][j_after].rgbtRed) / total_pix);
                }
                // Right top corner?
                else if (i == 0 && j == (width - 1))
                {
                    image[i][j].rgbtBlue =  round((copy_image[i][j].rgbtBlue + copy_image[i][j_before].rgbtBlue + copy_image[i_after][j].rgbtBlue +
                                                   copy_image[i_after][j_before].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen =  round((copy_image[i][j].rgbtGreen + copy_image[i][j_before].rgbtGreen + copy_image[i_after][j].rgbtGreen +
                                                    copy_image[i_after][j_before].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed =  round((copy_image[i][j].rgbtRed + copy_image[i][j_before].rgbtRed + copy_image[i_after][j].rgbtRed +
                                                  copy_image[i_after][j_before].rgbtRed) / total_pix);
                }
                // Left bottom corner?
                else if ((i == height - 1) && j == 0)
                {
                    image[i][j].rgbtBlue =  round((copy_image[i][j].rgbtBlue + copy_image[i_before][j].rgbtBlue +
                                                   copy_image[i_before][j_after].rgbtBlue +
                                                   copy_image[i][j_after].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen =  round((copy_image[i][j].rgbtGreen + copy_image[i_before][j].rgbtGreen +
                                                    copy_image[i_before][j_after].rgbtGreen +
                                                    copy_image[i][j_after].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed =  round((copy_image[i][j].rgbtRed + copy_image[i_before][j].rgbtRed + copy_image[i_before][j_after].rgbtRed +
                                                  copy_image[i][j_after].rgbtRed) / total_pix);
                }
                else
                {
                    image[i][j].rgbtBlue =  round((copy_image[i][j].rgbtBlue + copy_image[i_before][j_before].rgbtBlue +
                                                   copy_image[i_before][j].rgbtBlue +
                                                   copy_image[i][j_before].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen =  round((copy_image[i][j].rgbtGreen + copy_image[i_before][j_before].rgbtGreen +
                                                    copy_image[i_before][j].rgbtGreen +
                                                    copy_image[i][j_before].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed =  round((copy_image[i][j].rgbtRed + copy_image[i_before][j_before].rgbtRed + copy_image[i_before][j].rgbtRed +
                                                  copy_image[i][j_before].rgbtRed) / total_pix);
                }
            }

            // is pixel along an edge?
            else if (i == 0 || (i == height - 1) || j == 0 || (j == width - 1))
            {
                total_pix = 6.0;

                // top edge?
                if (i == 0)
                {
                    image[i][j].rgbtBlue = round((copy_image[i][j_before].rgbtBlue + copy_image[i][j].rgbtBlue + copy_image[i][j_after].rgbtBlue +
                                                  copy_image[i_after][j_before].rgbtBlue + copy_image[i_after][j].rgbtBlue +
                                                  copy_image[i_after][j_after].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen = round((copy_image[i][j_before].rgbtGreen + copy_image[i][j].rgbtGreen + copy_image[i][j_after].rgbtGreen +
                                                   copy_image[i_after][j_before].rgbtGreen + copy_image[i_after][j].rgbtGreen +
                                                   copy_image[i_after][j_after].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed = round((copy_image[i][j_before].rgbtRed + copy_image[i][j].rgbtRed + copy_image[i][j_after].rgbtRed +
                                                 copy_image[i_after][j_before].rgbtRed + copy_image[i_after][j].rgbtRed +
                                                 copy_image[i_after][j_after].rgbtRed) / total_pix);
                }
                // bottom edge?
                else if (i == height - 1)
                {
                    image[i][j].rgbtBlue = round((copy_image[i_before][j_before].rgbtBlue + copy_image[i_before][j].rgbtBlue +
                                                  copy_image[i_before][j_after].rgbtBlue +
                                                  copy_image[i][j_before].rgbtBlue + copy_image[i][j].rgbtBlue +
                                                  copy_image[i][j_after].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen = round((copy_image[i_before][j_before].rgbtGreen + copy_image[i_before][j].rgbtGreen +
                                                   copy_image[i_before][j_after].rgbtGreen +
                                                   copy_image[i][j_before].rgbtGreen + copy_image[i][j].rgbtGreen +
                                                   copy_image[i][j_after].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed = round((copy_image[i_before][j_before].rgbtRed + copy_image[i_before][j].rgbtRed +
                                                 copy_image[i_before][j_after].rgbtRed +
                                                 copy_image[i][j_before].rgbtRed + copy_image[i][j].rgbtRed +
                                                 copy_image[i][j_after].rgbtRed) / total_pix);
                }
                // left edge?
                else if (j == 0)
                {
                    image[i][j].rgbtBlue = round((copy_image[i_before][j].rgbtBlue + copy_image[i_before][j_after].rgbtBlue +
                                                  copy_image[i][j].rgbtBlue +
                                                  copy_image[i][j_after].rgbtBlue + copy_image[i_after][j].rgbtBlue +
                                                  copy_image[i_after][j_after].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen = round((copy_image[i_before][j].rgbtGreen + copy_image[i_before][j_after].rgbtGreen +
                                                   copy_image[i][j].rgbtGreen +
                                                   copy_image[i][j_after].rgbtGreen + copy_image[i_after][j].rgbtGreen +
                                                   copy_image[i_after][j_after].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed = round((copy_image[i_before][j].rgbtRed + copy_image[i_before][j_after].rgbtRed + copy_image[i][j].rgbtRed +
                                                 copy_image[i][j_after].rgbtRed + copy_image[i_after][j].rgbtRed +
                                                 copy_image[i_after][j_after].rgbtRed) / total_pix);

                }
                // right edge?
                else
                {
                    image[i][j].rgbtBlue = round((copy_image[i_before][j_before].rgbtBlue + copy_image[i_before][j].rgbtBlue +
                                                  copy_image[i][j_before].rgbtBlue + copy_image[i][j].rgbtBlue + copy_image[i_after][j_before].rgbtBlue +
                                                  copy_image[i_after][j].rgbtBlue) / total_pix);
                    image[i][j].rgbtGreen = round((copy_image[i_before][j_before].rgbtGreen + copy_image[i_before][j].rgbtGreen +
                                                   copy_image[i][j_before].rgbtGreen + copy_image[i][j].rgbtGreen + copy_image[i_after][j_before].rgbtGreen +
                                                   copy_image[i_after][j].rgbtGreen) / total_pix);
                    image[i][j].rgbtRed = round((copy_image[i_before][j_before].rgbtRed + copy_image[i_before][j].rgbtRed +
                                                 copy_image[i][j_before].rgbtRed +
                                                 copy_image[i][j].rgbtRed + copy_image[i_after][j_before].rgbtRed +
                                                 copy_image[i_after][j].rgbtRed) / total_pix);
                }
            }
            // pixel has eight neighbours
            else
            {
                total_pix = 9.0;
                image[i][j].rgbtBlue = round((copy_image[i_before][j_before].rgbtBlue + copy_image[i_before][j].rgbtBlue +
                                              copy_image[i_before][j_after].rgbtBlue + copy_image[i][j_before].rgbtBlue + copy_image[i][j].rgbtBlue +
                                              copy_image[i][j_after].rgbtBlue + copy_image[i_after][j_before].rgbtBlue + copy_image[i_after][j].rgbtBlue +
                                              copy_image[i_after][j_after].rgbtBlue) / total_pix);
                image[i][j].rgbtGreen = round((copy_image[i_before][j_before].rgbtGreen + copy_image[i_before][j].rgbtGreen +
                                               copy_image[i_before][j_after].rgbtGreen + copy_image[i][j_before].rgbtGreen + copy_image[i][j].rgbtGreen +
                                               copy_image[i][j_after].rgbtGreen + copy_image[i_after][j_before].rgbtGreen + copy_image[i_after][j].rgbtGreen +
                                               copy_image[i_after][j_after].rgbtGreen) / total_pix);
                image[i][j].rgbtRed = round((copy_image[i_before][j_before].rgbtRed + copy_image[i_before][j].rgbtRed +
                                             copy_image[i_before][j_after].rgbtRed + copy_image[i][j_before].rgbtRed +
                                             copy_image[i][j].rgbtRed + copy_image[i][j_after].rgbtRed +
                                             copy_image[i_after][j_before].rgbtRed + copy_image[i_after][j].rgbtRed +
                                             copy_image[i_after][j_after].rgbtRed) / total_pix);
            }
        }
    }
    free(copy_image);
    return;
}
