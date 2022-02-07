#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//for constant elements for the problem
#define NARG 2 //number of command-line arguments
#define LEN 26 //how many
#define OFFSET 32 //'A' - 'a'
#define FUP 'A'
#define LUP 'Z'
#define FLOW 'a'
#define LLOW 'z'

int main(int argc, string argv[])
{
    int i; //to scroll the string
    int pos; //position of the letter in the alphabet
    char cur; //current character considered
    int low; //to keep in whether the character is uppercase or lowercase
    char new;
    char  * ciphertext;
    int len; //lenght of text

    //handling the comman-line arguments part
    if (argc == 2)
    {
        if (strlen(argv[1]) != LEN)
        {
            printf("Key must contain 26 characters.");
            return 1;
        }
        else //implementation
        {
            //easier to switch all characters to upper case
            for (i = 0; * (argv[1] + i) != '\0'; i ++)
            {
                * (argv[1] + i) = toupper (* (argv[1] + i));
            }
            string text = get_string("plain text: ");
            len = strlen(text);
            ciphertext = malloc ((len + 1) * sizeof(char)); //dynamic memory allocation
            if (ciphertext != NULL) //everything went well
            {
                for (i = 0; text[i] != '\0'; i++)
                {
                    cur = text[i];
                    if ((cur >= FUP && cur <= LUP) || (cur >= FLOW && cur <= LLOW)) //meaning cur is an alphabetical character
                    {
                        low = islower (cur);
                        if (low == 1) //the letter is lowercase
                        {
                            pos = cur - FLOW;
                            new = * (argv[1] + pos);
                            if (isupper(new))
                            {
                                new = tolower(new);
                            }
                            ciphertext[i] = new;
                        }
                        else if (low != 1) //the letter is uppercase
                        {
                            pos = cur - FUP;
                            new = * (argv[1] + pos);
                            ciphertext[i] = new;
                        }
                    }
                    else
                    {
                        ciphertext[i] = text[i];
                    }
                }
                ciphertext[len + 1] = '\0';
                printf ("ciphertext : %s\n", ciphertext);
                return 0;
            }
            else
            {
                printf ("error in dynamic memory allocation\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage : ./substitution key");
        return 1;
    }
}
