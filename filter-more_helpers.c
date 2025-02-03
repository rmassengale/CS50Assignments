#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, blue
            int rgb_average =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Update pixel values
            image[i][j].rgbtRed = rgb_average;
            image[i][j].rgbtGreen = rgb_average;
            image[i][j].rgbtBlue = rgb_average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixels
            RGBTRIPLE tmp = image[i][j];

            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Apply blur
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red = 0.0;
            float green = 0.0;
            float blue = 0.0;
            int pixel_count = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Ensure valid neighboring pixels
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        red += copy[ni][nj].rgbtRed;
                        green += copy[ni][nj].rgbtGreen;
                        blue += copy[ni][nj].rgbtBlue;
                        pixel_count++;
                    }
                }
            }
            // Compute the average color values (rounding instead of truncating)
            int avg_red = round(red / pixel_count);
            int avg_green = round(green / pixel_count);
            int avg_blue = round(blue / pixel_count);

            image[i][j].rgbtRed = avg_red;
            image[i][j].rgbtGreen = avg_green;
            image[i][j].rgbtBlue = avg_blue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    
    // Detect edges
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gx_red = 0.0;
            float gy_red = 0.0;
            float gx_green = 0.0;
            float gy_green = 0.0;
            float gx_blue = 0.0;
            float gy_blue = 0.0;
            int Gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
            int Gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Ensure valid neighboring pixels
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // Calcluate Gx and Gy of red, green, and blue channel values
                        gx_red += (copy[ni][nj].rgbtRed * Gx[(di + 1) * 3 + (dj + 1)]);
                        gy_red += (copy[ni][nj].rgbtRed * Gy[(di + 1) * 3 + (dj + 1)]);
                        gx_green += (copy[ni][nj].rgbtGreen * Gx[(di + 1) * 3 + (dj + 1)]);
                        gy_green += (copy[ni][nj].rgbtGreen * Gy[(di + 1) * 3 + (dj + 1)]);
                        gx_blue += (copy[ni][nj].rgbtBlue * Gx[(di + 1) * 3 + (dj + 1)]);
                        gy_blue += (copy[ni][nj].rgbtBlue * Gy[(di + 1) * 3 + (dj + 1)]);
                    }
                }
            }
            int adj_red = (round(sqrt((gx_red * gx_red) + (gy_red * gy_red))));
            if (adj_red > 255)
            {
                adj_red = 255;
            }
            else if (adj_red < 0)
            {
                adj_red = 0;
            }
            int adj_green = (round(sqrt((gx_green * gx_green) + (gy_green * gy_green))));
            if (adj_green > 255)
            {
                adj_green = 255;
            }
            else if (adj_green < 0)
            {
                adj_green = 0;
            }
            int adj_blue = (round(sqrt((gx_blue * gx_blue) + (gy_blue * gy_blue))));
            if (adj_blue > 255)
            {
                adj_blue = 255;
            }
            else if (adj_blue < 0)
            {
                adj_blue = 0;
            }

            image[i][j].rgbtRed = adj_red;
            image[i][j].rgbtGreen = adj_green;
            image[i][j].rgbtBlue = adj_blue;
        }
    }
    return;
}
