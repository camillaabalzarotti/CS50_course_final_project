#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    int numhash, numspace;
    int i, count;
    //i just prefer to declare the variables all together first

    do{
        height = get_int ("Height: ");
    }
    while(height <= 0 || height > 8);

   //number of hashes i + 1
   //number of spaces 8 - (numebr of hashes)
    count = 0;
    for (i = 0; i < height;i ++){
        numhash = i + 1;
        numspace = height - numhash;
        for (count = 0; count < numspace; count ++){
            printf (" ");
        }
        for (count = 0; count < numhash; count ++){
            printf ("#");
        }
        printf ("  ");
        for (count = 0; count < numhash; count ++){
            printf ("#");
        }
        printf ("\n");
    }
}
