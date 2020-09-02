#include <cs50.h>
#include <stdio.h>

// prompts the user for a credit card number and then reports (via printf) whether it is a valid American Express, MasterCard, or Visa card number
int main(void)
{
    long cardNumber = get_long("Number: ");
    int startNumber = 0;
    int digit = 0;  // current digit
    int digitCount = 0;
    int luhn = 0;

    // go through digits
    for (digitCount = 0; digitCount < 16 && cardNumber != 0; digitCount++)
    {
        // get last digit
        digit = cardNumber % 10;
        if (digitCount % 2 == 0)
        {
            luhn += digit;
        }
        else
        {
            if (digit * 2 >= 10)
            {
                luhn += (digit * 2 % 10) + (digit * 2 / 10);
            }
            else
            {
                luhn += digit * 2;
            }
        }

        // remove last digit
        cardNumber /= 10;

        // get starting number
        if (cardNumber < 100 && cardNumber > 10)
        {
            startNumber = cardNumber;
        }
    }

    // luhn == 0 if valid
    luhn = luhn % 10;

    // print results
    if (digitCount == 15 && luhn == 0 && (startNumber == 34 || startNumber == 37))
    {
        printf("AMEX\n");
    }
    else if (digitCount == 16 && luhn == 0 && (startNumber >= 51 && startNumber <= 55))
    {
        printf("MASTERCARD\n");
    }
    else if ((digitCount == 13 || digitCount == 16) && luhn == 0 && (startNumber >= 40 && startNumber < 50))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}