// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    uint8_t * p;
    int16_t cur; //current sample read
    int16_t new; //cur modified according to factor
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    p = malloc(HEADER_SIZE * sizeof(uint8_t));
    if (p != NULL)
    {
        fread(p, sizeof(uint8_t), HEADER_SIZE, input);
        fwrite (p, sizeof(uint8_t), HEADER_SIZE, output);
    }else
        printf ("error in dynamic memory allocation\n");

    // TODO: Read samples from input file and write updated data to output file
    while ( ! feof(input))
    {
        fread(&cur, sizeof(int16_t), 1, input);
        new = cur * factor;
        fwrite(&new, sizeof(int16_t), 1, output);
    }
    //free the malloc when it's not useful anymore
    free(p);
    // Close files
    fclose(input);
    fclose(output);
}
