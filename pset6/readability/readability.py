from cs50 import get_string

def count_letters(text):
    numLetters = 0
    for i in range(len(text)):
        if (text[i] >= 'a' and text[i] <= 'z') or (text[i] >= 'A' and text[i] <= 'Z'):
            numLetters += 1

    return numLetters

def count_words(text):
    numWords = 0
    for i in range(len(text)):
        if text[i] == ' ':
            numWords+= 1
            while text[i + 1] == ' ':
                i += 1

    if text[len(text) - 1] != ' ':
        numWords += 1

    return numWords

def count_sentences(text):
    numSentences = 0
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            numSentences += 1

    return numSentences

text = get_string("Text: ")

numLetters = count_letters(text)

numWords = count_words(text)

numSentences = count_sentences(text)

L = numLetters / numWords * 100
S = numSentences / numWords * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade", index)