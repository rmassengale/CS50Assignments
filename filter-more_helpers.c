#include "helpers.h"
#include <math.h>
#include <stdio.h>

int calc_pixel_value(float gx, float gy);

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
            float gx_red = 0.0, gy_red = 0.0;
            float gx_green = 0.0, gy_green = 0.0;
            float gx_blue = 0.0, gy_blue = 0.0;

            int Gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
            int Gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    int index = (di + 1) * 3 + (dj + 1);

                    // Ensure valid neighboring pixels
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // Calculate Gx and Gy for each color channel
                        gx_red += copy[ni][nj].rgbtRed * Gx[index];
                        gy_red += copy[ni][nj].rgbtRed * Gy[index];
                        gx_green += copy[ni][nj].rgbtGreen * Gx[index];
                        gy_green += copy[ni][nj].rgbtGreen * Gy[index];
                        gx_blue += copy[ni][nj].rgbtBlue * Gx[index];
                        gy_blue += copy[ni][nj].rgbtBlue * Gy[index];
                    }
                }
            }

            // Use the helper function
            image[i][j].rgbtRed = calc_pixel_value(gx_red, gy_red);
            image[i][j].rgbtGreen = calc_pixel_value(gx_green, gy_green);
            image[i][j].rgbtBlue = calc_pixel_value(gx_blue, gy_blue);
        }
    }
}

// Helper function to calculate pixel value
int calc_pixel_value(float gx, float gy)
{
    int pixel_value = 0;
    pixel_value = round(sqrt(pow(gx, 2) + pow(gy, 2)));
    if (pixel_value > 255)
    {
        pixel_value = 255;
    }
    if (pixel_value < 0)
    {
        pixel_value = 0;
    }
    return pixel_value;
}
