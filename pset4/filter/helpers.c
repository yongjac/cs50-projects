#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float blue = image[i][j].rgbtBlue;
            float green = image[i][j].rgbtGreen;
            float red = image[i][j].rgbtRed;
            average = round((blue + green + red) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*blur_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    // top left corner
    blur_image[0][0].rgbtBlue = round(((float)image[0][0].rgbtBlue + (float)image[0][1].rgbtBlue + (float)image[1][0].rgbtBlue +
                                       (float)image[1][1].rgbtBlue) / 4);
    blur_image[0][0].rgbtGreen = round(((float)image[0][0].rgbtGreen + (float)image[0][1].rgbtGreen + (float)image[1][0].rgbtGreen +
                                        (float)image[1][1].rgbtGreen) / 4);
    blur_image[0][0].rgbtRed = round(((float)image[0][0].rgbtRed + (float)image[0][1].rgbtRed + (float)image[1][0].rgbtRed +
                                      (float)image[1][1].rgbtRed) / 4);

    // top right corner
    blur_image[0][width - 1].rgbtBlue = round(((float)image[0][width - 2].rgbtBlue + (float)image[0][width - 1].rgbtBlue +
                                        (float)image[1][width - 2].rgbtBlue + (float)image[1][width - 1].rgbtBlue) / 4);
    blur_image[0][width - 1].rgbtGreen = round(((float)image[0][width - 2].rgbtGreen + (float)image[0][width - 1].rgbtGreen +
                                         (float)image[1][width - 2].rgbtGreen + (float)image[1][width - 1].rgbtGreen) / 4);
    blur_image[0][width - 1].rgbtRed = round(((float)image[0][width - 2].rgbtRed + (float)image[0][width - 1].rgbtRed +
                                       (float)image[1][width - 2].rgbtRed + (float)image[1][width - 1].rgbtRed) / 4);

    // bottom left corner
    blur_image[height - 1][0].rgbtBlue = round(((float)image[height - 1][0].rgbtBlue + (float)image[height - 1][1].rgbtBlue +
                                         (float)image[height - 2][0].rgbtBlue + (float)image[height - 2][1].rgbtBlue) / 4);
    blur_image[height - 1][0].rgbtGreen = round(((float)image[height - 1][0].rgbtGreen + (float)image[height - 1][1].rgbtGreen +
                                          (float)image[height - 2][0].rgbtGreen + (float)image[height - 2][1].rgbtGreen) / 4);
    blur_image[height - 1][0].rgbtRed = round(((float)image[height - 1][0].rgbtRed + (float)image[height - 1][1].rgbtRed +
                                        (float)image[height - 2][0].rgbtRed + (float)image[height - 2][1].rgbtRed) / 4);

    // bottom right corner
    blur_image[height - 1][width - 1].rgbtBlue = round(((float)image[height - 1][width - 2].rgbtBlue +
            (float)image[height - 1][width - 1].rgbtBlue + (float)image[height - 2][width - 2].rgbtBlue +
            (float)image[height - 2][width - 1].rgbtBlue) / 4);
    blur_image[height - 1][width - 1].rgbtGreen = round(((float)image[height - 1][width - 2].rgbtGreen +
            (float)image[height - 1][width - 1].rgbtGreen + (float)image[height - 2][width - 2].rgbtGreen +
            (float)image[height - 2][width - 1].rgbtGreen) / 4);
    blur_image[height - 1][width - 1].rgbtRed = round(((float)image[height - 1][width - 2].rgbtRed +
            (float)image[height - 1][width - 1].rgbtRed + (float)image[height - 2][width - 2].rgbtRed + (float)image[height - 2][width -
                    1].rgbtRed) / 4);

    // top edge
    for (int i = 1; i < width - 1; i++)
    {
        blur_image[0][i].rgbtBlue = round(((float)image[0][i - 1].rgbtBlue + (float)image[0][i].rgbtBlue +
                                           (float)image[0][i + 1].rgbtBlue + (float)image[1][i - 1].rgbtBlue + (float)image[1][i].rgbtBlue +
                                           (float)image[1][i + 1].rgbtBlue) / 6);
        blur_image[0][i].rgbtGreen = round(((float)image[0][i - 1].rgbtGreen + image[0][i].rgbtGreen + image[0][i + 1].rgbtGreen +
                                            image[1][i - 1].rgbtGreen + image[1][i].rgbtGreen + image[1][i + 1].rgbtGreen) / 6);
        blur_image[0][i].rgbtRed = round(((float)image[0][i - 1].rgbtRed + image[0][i].rgbtRed + image[0][i + 1].rgbtRed + image[1][i -
                                          1].rgbtRed + image[1][i].rgbtRed + image[1][i + 1].rgbtRed) / 6);
    }

    // bottom edge
    for (int i = 1; i < width - 1; i++)
    {
        blur_image[height - 1][i].rgbtBlue = round(((float)image[height - 1][i - 1].rgbtBlue + image[height - 1][i].rgbtBlue + image[height
                                             - 1][i + 1].rgbtBlue + image[height - 2][i - 1].rgbtBlue + image[height - 2][i].rgbtBlue + image[height - 2][i + 1].rgbtBlue) / 6);
        blur_image[height - 1][i].rgbtGreen = round(((float)image[height - 1][i - 1].rgbtGreen + image[height - 1][i].rgbtGreen +
                                              image[height - 1][i + 1].rgbtGreen + image[height - 2][i - 1].rgbtGreen + image[height - 2][i].rgbtGreen + image[height - 2][i +
                                                      1].rgbtGreen) / 6);
        blur_image[height - 1][i].rgbtRed = round(((float)image[height - 1][i - 1].rgbtRed + image[height - 1][i].rgbtRed + image[height -
                                            1][i + 1].rgbtRed + image[height - 2][i - 1].rgbtRed + image[height - 2][i].rgbtRed + image[height - 2][i + 1].rgbtRed) / 6);
    }

    // left edge
    for (int i = 1; i < height - 1; i++)
    {
        blur_image[i][0].rgbtBlue = round(((float)image[i - 1][0].rgbtBlue + image[i][0].rgbtBlue + image[i + 1][0].rgbtBlue + image[i -
                                           1][1].rgbtBlue + image[i][1].rgbtBlue + image[i + 1][1].rgbtBlue) / 6);
        blur_image[i][0].rgbtGreen = round(((float)image[i - 1][0].rgbtGreen + image[i][0].rgbtGreen + image[i + 1][0].rgbtGreen + image[i -
                                            1][1].rgbtGreen + image[i][1].rgbtGreen + image[i + 1][1].rgbtGreen) / 6);
        blur_image[i][0].rgbtRed = round(((float)image[i - 1][0].rgbtRed + image[i][0].rgbtRed + image[i + 1][0].rgbtRed + image[i -
                                          1][1].rgbtRed + image[i][1].rgbtRed + image[i + 1][1].rgbtRed) / 6);
    }

    // right edge
    for (int i = 1; i < height - 1; i++)
    {
        blur_image[i][width - 1].rgbtBlue = round(((float)image[i - 1][width - 1].rgbtBlue + image[i][width - 1].rgbtBlue + image[i +
                                            1][width - 1].rgbtBlue + image[i - 1][width - 2].rgbtBlue + image[i][width - 2].rgbtBlue + image[i + 1][width - 2].rgbtBlue) / 6);
        blur_image[i][width - 1].rgbtGreen = round(((float)image[i - 1][width - 1].rgbtGreen + image[i][width - 1].rgbtGreen + image[i +
                                             1][width - 1].rgbtGreen + image[i - 1][width - 2].rgbtGreen + image[i][width - 2].rgbtGreen + image[i + 1][width - 2].rgbtGreen) /
                                             6);
        blur_image[i][width - 1].rgbtRed = round(((float)image[i - 1][width - 1].rgbtRed + image[i][width - 1].rgbtRed + image[i + 1][width
                                           - 1].rgbtRed + image[i - 1][width - 2].rgbtRed + image[i][width - 2].rgbtRed + image[i + 1][width - 2].rgbtRed) / 6);
    }

    // others
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            blur_image[i][j].rgbtBlue = round(((float)image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue +
                                               image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue
                                               + image[i + 1][j + 1].rgbtBlue) / 9);
            blur_image[i][j].rgbtGreen = round(((float)image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen
                                                +
                                                image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j +
                                                        1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9);
            blur_image[i][j].rgbtRed = round(((float)image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed +
                                              image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed +
                                              image[i + 1][j + 1].rgbtRed) / 9);
        }
    }

    // blur image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blur_image[i][j];
        }
    }

    // free blur image
    free(blur_image);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // initialize variables
    RGBTRIPLE(*edge)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    RGBTRIPLE top_left;
    RGBTRIPLE top;
    RGBTRIPLE top_right;
    RGBTRIPLE left;
    RGBTRIPLE right;
    RGBTRIPLE bottom_left;
    RGBTRIPLE bottom;
    RGBTRIPLE bottom_right;
    RGBTRIPLE solid_black;
    solid_black.rgbtBlue = 0;
    solid_black.rgbtGreen = 0;
    solid_black.rgbtRed = 0;
    int bottom_edge = height - 1;
    int right_edge = width - 1;

    // loop
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // not on edge
            if (i != 0 && i != bottom_edge && j != 0 && j != right_edge)
            {
                top_left = image[i - 1][j - 1];
                top = image[i - 1][j];
                top_right = image[i - 1][j + 1];
                left = image[i][j - 1];
                right = image[i][j + 1];
                bottom_left = image[i + 1][j - 1];
                bottom = image[i + 1][j];
                bottom_right = image[i + 1][j + 1];
            }
            // top edge
            else if (i == 0 && j != 0 && j != right_edge)
            {
                top_left = solid_black;
                top = solid_black;
                top_right = solid_black;
                left = image[i][j - 1];
                right = image[i][j + 1];
                bottom_left = image[i + 1][j - 1];
                bottom = image[i + 1][j];
                bottom_right = image[i + 1][j + 1];
            }
            // bottom edge
            else if (i == bottom_edge && j != 0 && j != right_edge)
            {
                top_left = image[i - 1][j - 1];
                top = image[i - 1][j];
                top_right = image[i - 1][j + 1];
                left = image[i][j - 1];
                right = image[i][j + 1];
                bottom_left = solid_black;
                bottom = solid_black;
                bottom_right = solid_black;
            }
            // left edge
            else if (j == 0 && i != 0 && i != bottom_edge)
            {
                top_left = solid_black;
                top = image[i - 1][j];
                top_right = image[i - 1][j + 1];
                left = solid_black;
                right = image[i][j + 1];
                bottom_left = solid_black;
                bottom = image[i + 1][j];
                bottom_right = image[i + 1][j + 1];
            }
            //right edge
            else if (j == right_edge && i != 0 && i != bottom_edge)
            {
                top_left = image[i - 1][j - 1];
                top = image[i - 1][j];
                top_right = solid_black;
                left = image[i][j - 1];
                right = solid_black;
                bottom_left = image[i + 1][j - 1];
                bottom = image[i + 1][j];
                bottom_right = solid_black;
            }
            // top left pixel
            else if (i == 0 && j == 0)
            {
                top_left = solid_black;
                top = solid_black;
                top_right = solid_black;
                left = solid_black;
                right = image[i][j + 1];
                bottom_left = solid_black;
                bottom = image[i + 1][j];
                bottom_right = image[i + 1][j + 1];
            }
            // top right pixel
            else if (i == 0 && j == right_edge)
            {
                top_left = solid_black;
                top = solid_black;
                top_right = solid_black;
                left = image[i][j - 1];
                right = solid_black;
                bottom_left = image[i + 1][j - 1];
                bottom = image[i + 1][j];
                bottom_right = solid_black;
            }
            // bottom left pixel
            else if (i == bottom_edge && j == 0)
            {
                top_left = solid_black;
                top = image[i - 1][j];
                top_right = image[i - 1][j + 1];
                left = solid_black;
                right = image[i][j + 1];
                bottom_left = solid_black;
                bottom = solid_black;
                bottom_right = solid_black;
            }
            // bottom right pixel
            else if (i == bottom_edge && j == right_edge)
            {
                top_left = image[i - 1][j - 1];
                top = image[i - 1][j];
                top_right = solid_black;
                left = image[i][j - 1];
                right = solid_black;
                bottom_left = solid_black;
                bottom = solid_black;
                bottom_right = solid_black;
            }

            // Gx
            float blue_x = (- top_left.rgbtBlue + top_right.rgbtBlue - 2 * left.rgbtBlue + 2 * right.rgbtBlue - bottom_left.rgbtBlue +
                            bottom_right.rgbtBlue);
            float green_x = (- top_left.rgbtGreen + top_right.rgbtGreen - 2 * left.rgbtGreen + 2 * right.rgbtGreen - bottom_left.rgbtGreen +
                             bottom_right.rgbtGreen);
            float red_x = (- top_left.rgbtRed + top_right.rgbtRed - 2 * left.rgbtRed + 2 * right.rgbtRed - bottom_left.rgbtRed +
                           bottom_right.rgbtRed);

            // Gy
            float blue_y = (- top_left.rgbtBlue - 2 * top.rgbtBlue - top_right.rgbtBlue + bottom_left.rgbtBlue + 2 * bottom.rgbtBlue +
                            bottom_right.rgbtBlue);
            float green_y = (- top_left.rgbtGreen - 2 * top.rgbtGreen - top_right.rgbtGreen + bottom_left.rgbtGreen + 2 * bottom.rgbtGreen +
                             bottom_right.rgbtGreen);
            float red_y = (- top_left.rgbtRed - 2 * top.rgbtRed - top_right.rgbtRed + bottom_left.rgbtRed + 2 * bottom.rgbtRed +
                           bottom_right.rgbtRed);

            // red green blue
            int blue = round(sqrt(pow(blue_x, 2) + pow(blue_y, 2)));
            int green = round(sqrt(pow(green_x, 2) + pow(green_y, 2)));
            int red = round(sqrt(pow(red_x, 2) + pow(red_y, 2)));

            // set edge color
            if (blue <= 255)
            {
                edge[i][j].rgbtBlue = blue;
            }
            else
            {
                edge[i][j].rgbtBlue = 255;
            }

            if (green <= 255)
            {
                edge[i][j].rgbtGreen = green;
            }
            else
            {
                edge[i][j].rgbtGreen = 255;
            }

            if (red <= 255)
            {
                edge[i][j].rgbtRed = red;
            }
            else
            {
                edge[i][j].rgbtRed = 255;
            }
        }
    }

    // edit image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edge[i][j];
        }
    }

    // free edge
    free(edge);
    return;
}
