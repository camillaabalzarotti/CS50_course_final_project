#include "helpers.h"
#include <math.h>

#define COLORS 3
#define DIM 3
#define LIMIT 255
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //two digits in the hexadecimal = 1 byte/ 8 bits
    //image[][].rgbtRed/rgbtGreen/rgbtBlue >>all 0 = black all 1 white
    int sum;
    float average;
    int i, j;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            sum = 0;
            sum += image[i][j].rgbtRed;
            sum += image[i][j].rgbtGreen;
            sum += image[i][j].rgbtBlue;
            average = (float)sum / COLORS;
            image[i][j].rgbtRed = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtBlue = round(average);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j; //to scrool the 2D array
    RGBTRIPLE tmp; //temporary variable for the swapp

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width/2; j++)
        {
            tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float average[COLORS];
    int i, j;
    int sum[COLORS]; //to add the color values of the neighbouring pixels
    int count; //to count how many values you have counted

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++) //for each element of the 2D image array
        {
            sum[0] = image[i][j].rgbtRed;
            sum[1] = image[i][j].rgbtGreen;
            sum[2] = image[i][j].rgbtBlue;
            count = 1;
            if (j < width - 1)
            {
                sum[0] += image[i][j + 1].rgbtRed;
                sum[1] += image[i][j + 1].rgbtGreen;
                sum[2] += image[i][j + 1].rgbtBlue;
                count ++;
            }
            if (j > 0)
            {
                sum[0] += image[i - 1][j - 1].rgbtRed;
                sum[1] += image[i - 1][j - 1].rgbtGreen;
                sum[2] += image[i - 1][j - 1].rgbtBlue;
                count ++;
            }
            if (i > 0)
            {
                sum[0] += image[i - 1][j].rgbtRed;
                sum[1] += image[i - 1][j].rgbtGreen;
                sum[2] += image[i - 1][j].rgbtBlue;
                count ++;
                if (j < width - 1)
                {
                    sum[0] += image[i - 1][j + 1].rgbtRed;
                    sum[1] += image[i - 1][j + 1].rgbtGreen;
                    sum[2] += image[i - 1][j + 1].rgbtBlue;
                    count ++;
                }
                if (j > 0)
                {
                    sum[0] += image[i - 1][j - 1].rgbtRed;
                    sum[1] += image[i - 1][j - 1].rgbtGreen;
                    sum[2] += image[i - 1][j - 1].rgbtBlue;
                    count ++;
                }
            }
            if (i < height - 1)
            {
                sum[0] += image[i + 1][j].rgbtRed;
                sum[1] += image[i + 1][j].rgbtGreen;
                sum[2] += image[i + 1][j].rgbtBlue;
                count ++;
                if (j < width - 1)
                {
                    sum[0] += image[i + 1][j + 1].rgbtRed;
                    sum[1] += image[i + 1][j + 1].rgbtGreen;
                    sum[2] += image[i + 1][j + 1].rgbtBlue;
                    count ++;
                }
                if (j > 0)
                {
                    sum[0] += image[i - 1][j - 1].rgbtRed;
                    sum[1] += image[i - 1][j - 1].rgbtGreen;
                    sum[2] += image[i - 1][j - 1].rgbtBlue;
                    count ++;
                }
            }
            for (i = 0; i < COLORS; i++)
            {
                average[i] = round ((float)sum[i] / count);
            }
            image[i][j].rgbtRed = average[0];
            image[i][j].rgbtGreen = average[1];
            image[i][j].rgbtBlue = average[2];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int kernelx[DIM][DIM] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernely[DIM][DIM] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int posx, posy;
    int r, c;
    RGBTRIPLE tmp[DIM][DIM]; //temporary 2D array in which the 3x3 box of neighbouring pixels is saved
    int i, j; //to scroll the image array
    int Gx[COLORS], Gy[COLORS];
    int final[COLORS];
    int index;

    //for each element of image
    for (i = 1; i < height - 1; i++)
    {
        for (j = 1; j < width - 1; j++) //since we don't care about the pixels in the corner or at the edge
        {
            //creating tmp
            r = 0;
            c = 0;
            for (posx = i - 1; posx < i + 3 && r < 3; posx++)
            {
                for (posy = j - 1; posy < j + 3 && c < 3; posy++)
                {
                    tmp[r][c] = image[posx][posy];
                    c ++;
                }
                r ++;
            }
            //calculate Gx and Gy for each colour
            for (index = 0; index < COLORS; index ++) //initialization
            {
                Gx[index] = 0;
                Gy[index] = 0;
            }
            for (r = 0; r < 3; r++)
            {
                for (c = 0; c < 3; c++)
                {
                    Gx[0] += tmp[r][c].rgbtBlue * kernelx[r][c];
                    Gy[0] += tmp[r][c].rgbtBlue * kernely[r][c];
                    Gx[1] += tmp[r][c].rgbtGreen * kernelx[r][c];
                    Gy[1] += tmp[r][c].rgbtGreen * kernely[r][c];
                    Gx[2] += tmp[r][c].rgbtRed * kernelx[r][c];
                    Gy[2] += tmp[r][c].rgbtRed * kernely[r][c];
                }
            }
            for (index = 0; index < COLORS; index++)
            {
                final[index] = round(sqrt (Gx[index] * Gx[index] + Gy[index] * Gy[index]));
                if (final[index] > LIMIT)
                {
                    final[index] = LIMIT;
                }
            }
        }
    }
    return;
}

