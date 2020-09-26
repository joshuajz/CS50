#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Filename not provided
    if (argc != 2)
    {
        printf("./recover [filename]\n");
        return 1;
    }

    // Opens the file
    FILE *f = fopen(argv[1], "r");

    // If the file is invalid, break out
    if (f == NULL)
    {
        printf("Invalid File");
        return 1;
    }

    // Variable Declaration
    bool endOfFile = false;
    bool foundJpeg = false;
    int filenumber = 0;
    char *filename = malloc(sizeof(char) * 7);
    BYTE *read = malloc(512 * sizeof(BYTE));
    FILE *img = NULL;


    // While we haven't reached end of file
    while (!endOfFile)
    {
        // If we have read under 512 bytes than we've reached the last block,
        // and this is the last time we write
        if (fread(read, 1, 512, f) != 512)
        {
            endOfFile = true;
        }
        // If it's a Jpeg
        if (read[0] == 0xff &&
            read[1] == 0xd8 &&
            read[2] == 0xff &&
            ((read[3] & 0xf0) == 0xe0))
        {
            // If we've found a jpeg before, we need to close the currently open file
            if (foundJpeg)
            {
                fclose(img);
            }
            // Opens a new Jpeg + writes the first block
            sprintf(filename, "%03i.jpg", filenumber);
            img = fopen(filename, "w");
            filenumber++;
            fwrite(read, 512, 1, img);
            foundJpeg = true;
        }
        // If we haven't founda new jpeg, continue to write to the same file
        else if (foundJpeg && !endOfFile)
        {
            fwrite(read, 512, 1, img);
        }
    }

}