#include <stdio.h>
#include <cs50.h>

// print pyramid made of # with 2 spaces in between after getting height from user. Height range is 1-8.
int main(void)
{
    // get height from user
    int height = get_int("Height: ");

    // get height until valide input
    while (height < 1 || height > 8)
    {
        height = get_int("Height: ");
    }

    // print pyramid
    for (int i = 1; i <= height; i++)
    {
        for (int j = 0; j < height - i; j++)
        {
            printf(" ");
        }

        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        printf("  ");

        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}