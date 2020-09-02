#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

// Design and implement a program, readability, that computes the Coleman-Liau index of the text
int main(void)
{
    // prompt the user for a string of text
    string text = get_string("Text: ");

    int numLetters = count_letters(text);
    //printf("%d letter(s)\n", numLetters);

    int numWords = count_words(text);
    //printf("%d word(s)\n", numWords);

    int numSentences = count_sentences(text);
    //printf("%d sentence(s)\n", numSentences);

    // calculate grade
    float L = (float)numLetters / (float)numWords * 100;
    float S = (float)numSentences / (float)numWords * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // print result
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}

// return number of letters in text
int count_letters(string text)
{
    int numLetters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' & text[i] <= 'Z'))
        {
            numLetters++;
        }
    }

    return numLetters;
}

// return number of words in text
int count_words(string text)
{
    int numWords = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ')
        {
            numWords++;
            while (text[i + 1] == ' ')
            {
                i++;
            }
        }
    }

    if (text[strlen(text) - 1] != ' ')
    {
        numWords++;
    }

    return numWords;
}

// return number of sentences in text
int count_sentences(string text)
{
    int numSentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            numSentences++;
        }
    }

    return numSentences;
}