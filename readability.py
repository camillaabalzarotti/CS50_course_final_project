from cs50 import get_string
import string

fup = ord('A')
lup = ord('Z')
flow = ord('a')
llow = ord('z')


def main():
    text = get_string("Text: ")

    # counting part
    let = count_letters(text)
    words = count_words(text)
    sent = count_sentences(text)

    L = let / words * 100
    S = sent / words * 100
    ris = 0.0588 * L - 0.296 * S - 15.8
    X = int(ris)
    if (ris - X >= 0.5):
        X = X + 1

    # printing the corrisponding grade
    if X < 1:
        print("Before Grade 1")
    elif X >= 16:
        print("Grade 16+")
    else:
        print("Grade ", X)


def count_letters(text):
    count = 0
    i = 0
    while i < len(text):
        cur = ord(text[i])
        # if text[i] is a letter
        if (cur >= fup and cur <= lup) or (cur >= flow and cur <= llow):
            count += 1
        i += 1
    return count


def count_words(text):
    count = 1
    i = 0
    while i < len(text):
        if text[i] == ' ':
            count += 1
        i += 1
    return count


def count_sentences(text):
    count = 0
    i = 0
    while i < len(text):
        if (text[i] == '.' or text[i] == '?' or text[i] == '!'):
            count += 1
        i += 1
    return count


main()

