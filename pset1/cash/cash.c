#include <cs50.h>
#include <stdio.h>
#include <math.h>

// program that first asks the user how much change is owed and then prints the minimum number of coins with which that change can be made.
int main(void)
{
    // get change owed
    float dollars = get_float("Change owed: ");

    // prompt again if input is negative
    while (dollars < 0)
    {
        dollars = get_float("Change owed: ");
    }

    // convert to cents
    int cents = round(dollars * 100);
    int totalCoins = 0;

    // Calculate total coins
    while (cents >= 25)
    {
        cents -= 25;
        totalCoins++;
    }

    while (cents >= 10)
    {
        cents -= 10;
        totalCoins++;
    }

    if (cents >= 5)
    {
        cents -= 5;
        totalCoins++;
    }

    totalCoins += cents;

    // print result
    printf("%d\n", totalCoins);
}
