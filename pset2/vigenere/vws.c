#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int findkeyshift(char keylet);
bool isplntxtalpha(string plaintext);
void findciphertext(string keyword, string plaintext);

int main(void)
{
    string keyword = "abc";
    string plaintext = "hello my name is tom";
    
    for (int i = 0; i < strlen(plaintext); i++)
    {
        char plnlet = plaintext[i];
        
        //make h only iterate when i is alpha
        int h = i - 1;
        
        if (isalpha(plnlet))
        {
            h = h + 1;
        }
        
        //okay now we want to find the keyshift
        
        //make j wrap around to the beginning when run out of keyword letters
        //make j link to h not i. j only increases when plnlent is alphabetical
        int kLen = strlen(keyword);
        int j = h % kLen;
        char keylet = keyword[j];
        printf("plnlet = %c\n", plnlet);
        printf("i = %i, j = %i\n", i, j);
        printf("keylet = %c\n", keylet);
        
        //use keylet to find keyshift
        
        int keyshift;
        
        if (isupper(keylet) != 0)       //if the keylet is uppercase
        {
            keylet = tolower(keylet);
            keyshift = (int)keylet - 97;
        }
        else                            //if the keylet is lowercase
        {
            keyshift = (int)keylet - 97;
        }
        
        printf("keyshift = %i\n", keyshift);
        
        //now apply keyshift to plnlet
        
        //if plnlet is capital
        if (isupper(plnlet))
        {
            //apply vigenere formula
            int ciphint = (plnlet + keyshift);
            printf("ciphint = %i\n", ciphint);
            
            //make wrap around if ciphint > Z
            if (ciphint > 90)
            {
                ciphint = ciphint - 26;
            }
        
            //turn your number back into its letter
            char ciphlet = (char) ciphint;
            printf("ciphlet = %c\n", ciphlet);
        }
        
        //if plnlet is lowercase
        if (islower(plnlet))
        {
            int ciphint = (plnlet + keyshift);
            printf("ciphint = %i\n", ciphint);
            
            if (ciphint > 122)
            {
                ciphint = ciphint - 26;
            }
            
            char ciphlet = (char) ciphint;
            printf("ciphlet = %c\n", ciphlet);
        }
        printf("\n");
    }
}


//FUNCTION:
int findkeyshift(char keylet)
{
    if (isupper(keylet) != 0)       //if the keylet is uppercase
    {
        keylet = tolower(keylet);
        int keyshift = (int)keylet - 97;
        return keyshift;
    }
    else                            //if the keylet is lowercase
    {
        int keyshift = (int)keylet - 97;
        return keyshift;
    }
}
