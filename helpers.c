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
        for (j = 0; j < width / 2; j++)
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
    RGBTRIPLE copy[height][width]; //copy of the image
    int i, j;
    int k;
    int sum[COLORS]; //to add the color values of the neighbouring pixels
    int count; //to count how many values you have counted

    for (i = 0; i < height; i ++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++) //for each element of the 2D image array
        {
            sum[0] = copy[i][j].rgbtRed;
            sum[1] = copy[i][j].rgbtGreen;
            sum[2] = copy[i][j].rgbtBlue;
            count = 1;
            if (j < width - 1) //adding the pixel on the same row but on the right
            {
                sum[0] += copy[i][j + 1].rgbtRed;
                sum[1] += copy[i][j + 1].rgbtGreen;
                sum[2] += copy[i][j + 1].rgbtBlue;
                count ++;
            }
            if (j > 0) //adding the pixel on the same row but on the left
            {
                sum[0] += copy[i][j - 1].rgbtRed;
                sum[1] += copy[i][j - 1].rgbtGreen;
                sum[2] += copy[i][j - 1].rgbtBlue;
                count ++;
            }
            if (i > 0) //adding pixels above
            {
                sum[0] += copy[i - 1][j].rgbtRed;
                sum[1] += copy[i - 1][j].rgbtGreen;
                sum[2] += copy[i - 1][j].rgbtBlue;
                count ++;
                if (j < width - 1)
                {
                    sum[0] += copy[i - 1][j + 1].rgbtRed;
                    sum[1] += copy[i - 1][j + 1].rgbtGreen;
                    sum[2] += copy[i - 1][j + 1].rgbtBlue;
                    count ++;
                }
                if (j > 0)
                {
                    sum[0] += copy[i - 1][j - 1].rgbtRed;
                    sum[1] += copy[i - 1][j - 1].rgbtGreen;
                    sum[2] += copy[i - 1][j - 1].rgbtBlue;
                    count ++;
                }
            }
            if (i < height - 1) //adding pixels below
            {
                sum[0] += copy[i + 1][j].rgbtRed;
                sum[1] += copy[i + 1][j].rgbtGreen;
                sum[2] += copy[i + 1][j].rgbtBlue;
                count ++;
                if (j < width - 1)
                {
                    sum[0] += copy[i + 1][j + 1].rgbtRed;
                    sum[1] += copy[i + 1][j + 1].rgbtGreen;
                    sum[2] += copy[i + 1][j + 1].rgbtBlue;
                    count ++;
                }
                if (j > 0)
                {
                    sum[0] += copy[i + 1][j - 1].rgbtRed;
                    sum[1] += copy[i + 1][j - 1].rgbtGreen;
                    sum[2] += copy[i + 1][j - 1].rgbtBlue;
                    count ++;
                }
            }
            for (k = 0; k < COLORS; k++)
            {
                average[k] = (int)round((float)sum[k] / count);
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
    RGBTRIPLE copy[height][width]; //copy of the image
    int posx, posy;
    int r, c;
    RGBTRIPLE tmp[DIM][DIM]; //temporary 2D array in which the 3x3 box of neighbouring pixels is saved
    int i, j; //to scroll the image array
    int Gx[COLORS], Gy[COLORS];
    int final[COLORS];
    int index;

    for (i = 0; i < height; i ++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }
    //for each element of the copy of the image
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            //inserting in tmp
            if (i > 0) //so if there is actually a line above
            {
                tmp[0][1].rgbtRed = copy[i - 1][j].rgbtRed;
                tmp[0][1].rgbtGreen = copy[i - 1][j].rgbtGreen;
                tmp[0][1].rgbtBlue = copy[i - 1][j].rgbtBlue;
                if (j > 0)
                {
                    tmp[0][0].rgbtRed = copy[i - 1][j - 1].rgbtRed;
                    tmp[0][0].rgbtGreen = copy[i - 1][j - 1].rgbtGreen;
                    tmp[0][0].rgbtBlue = copy[i - 1][j - 1].rgbtBlue;
                }
                else
                {
                    tmp[0][0].rgbtRed = 0;
                    tmp[0][0].rgbtGreen = 0;
                    tmp[0][0].rgbtBlue = 0;
                }
                if (j < width - 1)
                {
                    tmp[0][2].rgbtRed = copy[i - 1][j + 1].rgbtRed;
                    tmp[0][2].rgbtGreen = copy[i - 1][j + 1].rgbtGreen;
                    tmp[0][2].rgbtBlue = copy[i - 1][j + 1].rgbtBlue;
                }
                else
                {
                    tmp[0][2].rgbtRed = 0;
                    tmp[0][2].rgbtGreen = 0;
                    tmp[0][2].rgbtBlue = 0;
                }
            }
            else
            {
                tmp[0][0].rgbtRed = 0;
                tmp[0][0].rgbtGreen = 0;
                tmp[0][0].rgbtBlue = 0;
                tmp[0][1].rgbtRed = 0;
                tmp[0][1].rgbtGreen = 0;
                tmp[0][1].rgbtBlue = 0;
                tmp[0][2].rgbtRed = 0;
                tmp[0][2].rgbtGreen = 0;
                tmp[0][2].rgbtBlue = 0;
            }
            if (i < height - 1) //if there is a line below
            {
                tmp[2][1] = copy[i + 1][j];
                if (j > 0)
                {
                    tmp[2][0].rgbtRed = copy[i + 1][j - 1].rgbtRed;
                    tmp[2][0].rgbtGreen = copy[i + 1][j - 1].rgbtGreen;
                    tmp[2][0].rgbtBlue = copy[i + 1][j - 1].rgbtBlue;
                }
                else
                {
                    tmp[2][0].rgbtRed = 00;
                    tmp[2][0].rgbtGreen = 00;
                    tmp[2][0].rgbtBlue = 00;
                }
                if (j < width - 1)
                {
                    tmp[2][2].rgbtRed = copy[i + 1][j + 1].rgbtRed;
                    tmp[2][2].rgbtGreen = copy[i + 1][j + 1].rgbtGreen;
                    tmp[2][2].rgbtBlue = copy[i + 1][j + 1].rgbtBlue;
                }
                else
                {
                    tmp[2][2].rgbtRed = 00;
                    tmp[2][2].rgbtGreen = 00;
                    tmp[2][2].rgbtBlue = 00;
                }
            }
            else
            {
                tmp[2][0].rgbtRed = 0;
                tmp[2][0].rgbtGreen = 0;
                tmp[2][0].rgbtBlue = 0;
                tmp[2][1].rgbtRed = 0;
                tmp[2][1].rgbtGreen = 0;
                tmp[2][1].rgbtBlue = 0;
                tmp[2][2].rgbtRed = 0;
                tmp[2][2].rgbtGreen = 0;
                tmp[2][2].rgbtBlue = 0;
            }
            if (j > 0)
            {
                tmp[1][0].rgbtRed = copy[i][j - 1].rgbtRed;
                tmp[1][0].rgbtGreen = copy[i][j - 1].rgbtGreen;
                tmp[1][0].rgbtBlue = copy[i][j - 1].rgbtBlue;
            }
            else
            {
                tmp[1][0].rgbtRed = 00;
                tmp[1][0].rgbtGreen = 00;
                tmp[1][0].rgbtBlue = 00;
            }
            if (j < width - 1)
            {
                tmp[1][2].rgbtRed = copy[i][j + 1].rgbtRed;
                tmp[1][2].rgbtGreen = copy[i][j + 1].rgbtGreen;
                tmp[1][2].rgbtBlue = copy[i][j + 1].rgbtBlue;
            }
            else
            {
                tmp[1][2].rgbtRed = 00;
                tmp[1][2].rgbtGreen = 00;
                tmp[1][2].rgbtBlue = 00;
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
                final[index] = (int) round(sqrt(Gx[index] * Gx[index] + Gy[index] * Gy[index]));
                if (final[index] > LIMIT)
                {
                    final[index] = LIMIT;
                }
            }
            image[i][j].rgbtBlue = final[0];
            image[i][j].rgbtGreen = final[1];
            image[i][j].rgbtRed = final[2];
        }
    }
    return;
}


