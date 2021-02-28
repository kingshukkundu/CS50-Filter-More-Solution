#include "helpers.h"
#include <math.h>
int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //iterate each pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //for each pixels, calculate the average of Red, Green and Blue Values
            BYTE temp = 0;
            temp = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            //set each of RGB Values to average
            image[i][j].rgbtBlue = temp;
            image[i][j].rgbtGreen = temp;
            image[i][j].rgbtRed = temp;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //switch the position of pixels to create illusion of reflection
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

//copy from the temp_image to image
void copyimage(int row, int column, RGBTRIPLE temp_image[row][column], RGBTRIPLE image[row][column])
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            image[i][j] = temp_image[i][j];
        }
    }
}

//Blurs Pixel According to Box Blur Algorithm
RGBTRIPLE blur_helper(int height, int width, RGBTRIPLE image[height][width], int i, int j)
{
    RGBTRIPLE temp_pixel;
    float count_pixels = 0;
    int blue = 0, red = 0, green = 0;
    //finds the average RBG values of the surrounding 3x3 grid
    for (int k = -1; k <= 1; k++)
    {
        int temp_height = i + k;
        for (int l = -1; l <= 1; l++)
        {
            int temp_width = j + l;
            //checks if the input pixels are valid
            if (0 <= temp_height && temp_height < height && 0 <= temp_width && temp_width < width)
            {
                blue += image[temp_height][temp_width].rgbtBlue;
                red += image[temp_height][temp_width].rgbtRed;
                green += image[temp_height][temp_width].rgbtGreen;
                count_pixels++;
            }
        }
    }
    temp_pixel.rgbtBlue = round(blue / count_pixels);
    temp_pixel.rgbtRed = round(red / count_pixels);
    temp_pixel.rgbtGreen = round(green / count_pixels);
    return temp_pixel;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];

    //iterate through each pixels and set its RBG values equal to the average RBG values of its surrounding pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_image[i][j] = blur_helper(height, width, image, i, j);
        }
    }
    copyimage(height, width, temp_image, image);
}

//limits a value from exceeding 255
int limit(int a)
{
    return value < 255 ? value : 255;
}

//compares a pixel to its surroudning values to detect edges
RGBTRIPLE edges_helper(int height, int width, RGBTRIPLE image[height][width], int i, int j)
{
    RGBTRIPLE output_pixel;
    int bluex = 0;
    int redx = 0;
    int greenx = 0;
    int bluey = 0;
    int redy = 0;
    int greeny = 0;

    for (int k = -1; k <= 1; k++)
    {
        int temp_height = i + k;
        for (int l = -1; l <= 1; l++)
        {
            int temp_width = j + l;
            if (0 <= temp_height && temp_height < height && 0 <= temp_width && temp_width < width)
            {
                int multiplier = Gx[k + 1][l + 1];
                bluex += multiplier * image[temp_height][temp_width].rgbtBlue;
                redx += multiplier * image[temp_height][temp_width].rgbtRed;
                greenx += multiplier * image[temp_height][temp_width].rgbtGreen;

                multiplier = Gy[k + 1][l + 1];
                bluey += multiplier * image[temp_height][temp_width].rgbtBlue;
                redy += multiplier * image[temp_height][temp_width].rgbtRed;
                greeny += multiplier * image[temp_height][temp_width].rgbtGreen;
            }
        }
    }
    output_pixel.rgbtBlue = limit(round(sqrt(bluex * bluex + bluey * bluey)));
    output_pixel.rgbtRed = limit(round(sqrt(redx * redx + redy * redy)));
    output_pixel.rgbtGreen = limit(round(sqrt(greenx * greenx + greeny * greeny)));
    return output_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //iterate through each pixels
    RGBTRIPLE temp_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_image[i][j] = edges_helper(height, width, image, i, j);
        }
    }
    copyimage(height, width, temp_image, image);
}