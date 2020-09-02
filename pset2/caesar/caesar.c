#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Design and implement a program, caesar, that encrypts messages using Caesar’s cipher.
int main(int argc, string argv[])
{
    // print error message if number of argument is not 1
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // print error message if argument is not a decimal digit
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // get plaintext
    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;

    // calculate shift
    int shift;
    sscanf(argv[1], "%d", &shift);
    shift = shift % 26;

    int charIndex;

    // encrypt message
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (isalpha(plaintext[i]))
        {
            charIndex = plaintext[i] + shift;
            if (isupper(plaintext[i]) && charIndex > 'Z')
            {
                charIndex -= 26;
            }
            else if (islower(plaintext[i]) && charIndex > 'z')
            {
                charIndex -= 26;
            }
            ciphertext[i] = charIndex;
        }
    }

    // print result
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}