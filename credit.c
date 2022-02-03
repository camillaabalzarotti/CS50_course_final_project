#include <stdio.h>
#include <cs50.h>

#define BASE 10
#define MAX 16
#define EVEN 2

int main(void)
{
    int product; //product of each digit * 2
    int checksum;
    int isvalid;
    long cur;
    int i; //index for the array
    int digits[MAX];
    int start; //to save the starting numbers
    int count; //counts the digits so the lenght of the card number
    long val; //adding a support variable in order not to modify the cardnum inserted

    long cardnum = get_long("Number: "); //the number as more than 8 digits/bits
    val = cardnum;
    count = 0;
    while (val != 0){
        cur = val % BASE;
        digits[count] = (int) cur;
        count ++;
        val = val / BASE;
    }
    checksum = 0;
    for (i = 1; i < count; i = i + 2){
        product = digits[i] * 2;
        while (product != 0){
            checksum += product % BASE;
            product = product / BASE;
        }
    }

    for (i = 0; i < count; i = i + 2){
        checksum += digits[i];
    }
    isvalid = 0;
    if (checksum % BASE == 0){
        isvalid = 1;
        start = digits[count - 1] * BASE + digits[count - 2];
        if (start == 34 || start == 37)
            printf("AMEX\n");
        else if (start == 51 || start == 52 || start == 53 || start == 54 || start == 55)
            printf("MASTERCARD\n");
        else if (digits[count - 1] == 4)
            printf("VISA\n");
        else
            isvalid = 0;
    }
    if (isvalid == 0)
        printf("INVALID\n");
}
