#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Goes through every value in the picture
    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < width; z++)
        {
            // Creates an average of the 3 and divides it by 3
            float avgFloat = image[i][z].rgbtBlue + image[i][z].rgbtGreen + image[i][z].rgbtRed;
            int avgInt = round(avgFloat / 3);
            // Replaces each value in the image w/ that value
            image[i][z].rgbtBlue = avgInt;
            image[i][z].rgbtGreen = avgInt;
            image[i][z].rgbtRed = avgInt;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Every value in the picture
    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < width; z++)
        {
            // Values as per the formula
            int red = round(.393 * image[i][z].rgbtRed + .769 * image[i][z].rgbtGreen + .189 * image[i][z].rgbtBlue);
            int green = round(.349 * image[i][z].rgbtRed + .686 * image[i][z].rgbtGreen + .168 * image[i][z].rgbtBlue);
            int blue = round(.272 * image[i][z].rgbtRed + .534 * image[i][z].rgbtGreen + .131 * image[i][z].rgbtBlue);
            // Checks for values > 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            // Replaces the values
            image[i][z].rgbtBlue = blue;
            image[i][z].rgbtGreen = green;
            image[i][z].rgbtRed = red;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflected[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < width; z++)
        {
            reflected[i][z] = image[i][width - z - 1];
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int x = 0; x < width; x++)
        {
            image[j][x] = reflected[j][x];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i2 = 0; i2 < height; i2++)
    {
        for (int z2 = 0; z2 < width; z2++)
        {
            copy[i2][z2] = image[i2][z2];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < width; z++)
        {
            double red = 0, blue = 0, green = 0;
            int amnt = 0;
            for (int j = -1; j < 2; j++)
            {
                for (int x = -1; x < 2; x++)
                {
                    if (0 <= j + i && j + i < height && 0 <= z + x && z + x < width)
                    {
                        amnt++;
                        blue += copy[i + j][z + x].rgbtBlue;
                        red += copy[i + j][z + x].rgbtRed;
                        green += copy[i + j][z + x].rgbtGreen;
                    }
                }
            }

            image[i][z].rgbtBlue = round(blue / amnt);
            image[i][z].rgbtRed = round(red / amnt);
            image[i][z].rgbtGreen = round(green / amnt);

        }
    }
}
