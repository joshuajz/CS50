#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < width; z++)
        {
            float avgFloat = image[i][z].rgbtBlue + image[i][z].rgbtGreen + image[i][z].rgbtRed;
            int avgInt = round(avgFloat / 3);
            image[i][z].rgbtBlue = avgInt;
            image[i][z].rgbtGreen = avgInt;
            image[i][z].rgbtRed = avgInt;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < width; z++)
        {
            double sepiaRed = (.393 * image[i][z].rgbtRed) + (.769 * image[i][z].rgbtGreen) + (.189 * image[i][z].rgbtBlue);
            double sepiaGreen = (.349 * image[i][z].rgbtRed) + (.686 * image[i][z].rgbtGreen) + (.168 * image[i][z].rgbtBlue);
            double sepiaBlue = (.272 * image[i][z].rgbtRed) + (.534 * image[i][z].rgbtGreen) + (.131 * image[i][z].rgbtBlue);
            image[i][z].rgbtBlue = round(sepiaBlue);
            image[i][z].rgbtGreen = round(sepiaGreen);
            image[i][z].rgbtRed = round(sepiaRed);
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
    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < width; z++)
        {
            double red = 0, blue = 0, green = 0;
            int heightloc = -1;
            int widthloc = -1;
            int amnt = 0;
            for (int j = 0; j < 3; j++)
            {
                for (int h = 0; h < 3; h++)
                {
                    if ((i + heightloc >= 0 && i + heightloc < height) && (z + widthloc >= 0 && z + widthloc < width))
                    {
                        red += image[i + heightloc][z + widthloc].rgbtRed;
                        green += image[i + heightloc][z + widthloc].rgbtGreen;
                        blue += image[i + heightloc][z + widthloc].rgbtBlue;
                        amnt++;
                    }
                    heightloc++;
                }
                widthloc++;
                heightloc = -1;
            }
            image[i][z].rgbtBlue = round(blue / amnt);
            image[i][z].rgbtRed = round(red / amnt);
            image[i][z].rgbtGreen = round(green / amnt);

        }
    }
}
