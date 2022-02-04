#include <cs50.h>
#include <stdio.h>

//constant elements
#define FUP 'A' //first upper case letter
#define LUP 'Z' //last upper case letter
#define FLOW 'a' //first lower case letter
#define LLOW 'z' //last lower case letter
#define SPACE ' ' //separates words
#define PERIOD '.' //separates sentences
#define QUESTION '?' //separates sentences
#define EXCLAM '!' //separates sentences

int count_letters (string);
int count_words (string);
int count_sentences (string);

int main(void)
{
    int let; //to count the letters in the text
    int words; //to count the words in the text
    int sent; //to count sentences
    float L, S;
    int X; //final grade
    float ris;//mathematical grade

    //input
    string text = get_string("Text: ");
    //printing the same text
    printf("%s\n", text);

    //counting part
    let = count_letters (text);
    words = count_words (text);
    sent = count_sentences (text);

    //printing the results
    printf ("%d letters\n%d words\n%d sentences\n", let, words, sent);

    L =  ((float)let / words) * 100;
    printf("%f\n", L);
    S =  ((float)sent / words) * 100;
    printf("%f\n", S);

    ris = (0.0588 * L) - (0.296 * S) - 15.8;
    X = ris;
    if (ris - X >= 0.5)
    {
        X = X + 1;
    }
    printf ("%d\n", X);

    //printing the corrisponding grade
    if (X < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (X >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf ("Grade %d\n", X);
    }



}

int count_letters (string text)
{
    int count, i;

    count = 0;
    for (i = 0; text[i] != '\0'; i++)
    {
        if ((text[i] >= FUP && text[i]<= LUP) || (text[i] >= FLOW && text[i] <= LLOW)) //if text[i] is a letter
        {
            count ++;
        }
    }

    return count;
}

int count_words (string text)
{
    int count, i;

    count = 1;
    for (i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == SPACE)
        {
            count ++;
        }
    }

    return count;
}

int count_sentences (string text)
{
    int count, i;

    count = 0;
    for (i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == PERIOD || text[i] == QUESTION || text[i] == EXCLAM)
        {
            count ++;
        }
    }

    return count;
}
