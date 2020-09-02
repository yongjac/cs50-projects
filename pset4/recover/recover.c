#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Could not open %s\n", argv[1]);
        return 2;
    }

    int file_num = 0; // number of image
    int count = 0;
    char image[7]; // image name
    bool found = false; // found JPEG
    FILE *img; // JPEG file
    BYTE buffer[512]; // buffer

    // Read 512 bytes into a buffer until end of card
    while (fread(buffer, 1, 512, file) == 512)
    {
        // If start of new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If first JPEG
            if (!found)
            {
                sprintf(image, "%03i.jpg", file_num);
                img = fopen(image, "w");
                fwrite(buffer, 512, 1, img);
                count++;
                file_num++;
                found = true;
            }
            else
            {
                fclose(img);
                sprintf(image, "%03i.jpg", file_num);
                img = fopen(image, "w");
                fwrite(buffer, 512, 1, img);
                count++;
                file_num++;
            }
        }
        else
        {
            // If already found JPEG
            if (found)
            {
                fwrite(buffer, 512, 1, img);
            }
        }
    }

    // Close files
    fclose(img);
    fclose(file);
}
