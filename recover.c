#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NARG 2
#define BLOCK_SIZE 512
#define LEN 7

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    FILE *file;
    FILE *img;
    int found;
    BYTE buffer[BLOCK_SIZE];
    int count;
    char filename[LEN + 1];

    if (argc != NARG)
    {
        printf("Usage: image name\n");
        return 1;
    }
    else
    {
        file = fopen(argv[1], "r");
        if (file != NULL)
        {
            count = 0;
            while (fread(buffer, 1, BLOCK_SIZE, file)== BLOCK_SIZE)
            {
                if (buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff && (buffer[3] & 0xf0)== 0xe0)
                //verifing if there is the header of a JPEG file
                {
                    if (count == 0)
                    {
                        //printing a new file with a ###.jpg name
                        sprintf(filename, "%03i.jpg", count);
                        count ++;
                        img = fopen(filename, "w");
                        fwrite(buffer, 1, BLOCK_SIZE, img);
                    }else
                    {
                        fclose(img);
                        sprintf(filename, "%03i.jpg", count);
                        count ++;
                        img = fopen(filename, "w");
                        fwrite(buffer, 1, BLOCK_SIZE, img);
                    }
                    found = 1;
                }
                else
                {
                    if (found == 1)
                    {
                        fwrite(buffer, 1, BLOCK_SIZE, img);
                    }
                }
            }
            fclose(img);
            fclose(file);
        }
        else
        {
            printf ("Cannot open the file %s\n", argv[1]);
            return 1;
        }
    }
}