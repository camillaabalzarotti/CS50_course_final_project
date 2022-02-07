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
    char   *ciphertext;
    int len; //lenght of text
    int found[LEN]; //to check the key is valid
    int isOK;

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
            for (i = 0; i < LEN; i++)
            {
                found[i] = 0;
            }
            //easier to switch all characters to upper case + invalid characters in key + repetition
            isOK = 1;
            for (i = 0; * (argv[1] + i) != '\0' && isOK == 1; i ++)
            {
                if (isalpha(* (argv[1] + i)))
                {
                    * (argv[1] + i) = toupper(* (argv[1] + i));
                    pos = * (argv[1] + i) - FUP;
                    if (found[pos] == 1)
                    {
                        isOK = 0;
                    }
                    else
                    {
                        found[pos] = 1;
                    }
                }
                else
                {
                    isOK = 0;
                }
            }
            if (isOK == 0)
            {
                return 1;
            }
            string text = get_string("plain text: ");
            len = strlen(text);
            ciphertext = malloc((len + 1) * sizeof(char));  //dynamic memory allocation
            if (ciphertext != NULL) //everything went well
            {
                for (i = 0; text[i] != '\0'; i++)
                {
                    cur = text[i];
                    if ((cur >= FUP && cur <= LUP) || (cur >= FLOW && cur <= LLOW)) //meaning cur is an alphabetical character
                    {
                        low = islower(cur);
                        if (low != 0) //the letter is lowercase
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
                ciphertext[len] = '\0';
                printf("ciphertext: %s", ciphertext);
                printf("\n");
                return 0;
            }
            else
            {
                printf("error in dynamic memory allocation\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage : ./substitution key\n");
        return 1;
    }
}


