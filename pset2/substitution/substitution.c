#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Design and implement a program, substitution, that encrypts messages using a substitution cipher.
int main(int argc, string argv[])
{
    // print error message if number of arguments is not 1
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // print error message if argument not containing 26 characters
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    char key[26];
    bool chars[265] = { 0 };

    // check if key contains non alphabet characters or contains repeated characters
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must contain only alphabet characteres.\n");
            return 1;
        }

        key[i] = toupper(argv[1][i]);

        if (chars[(int)(key[i])] == true)
        {
            printf("Key must contain each letter exactly once.\n");
            return 1;
        }

        chars[(int)(key[i])] = true;
    }

    // get plaintext
    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;
    int charIndex;

    // encrypt message
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (isupper(plaintext[i]))
        {
            charIndex = plaintext[i] - 'A';
            ciphertext[i] = toupper(key[charIndex]);
        }
        else if (islower(plaintext[i]))
        {
            charIndex = plaintext[i] - 'a';
            ciphertext[i] = tolower(key[charIndex]);
        }
    }

    // print result
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}