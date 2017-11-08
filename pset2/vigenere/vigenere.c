#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//command line arg checker
bool argchecker(int chkargc, string chkargv[]);
int findkeyshift(char keylet);


//START OF MAIN FUNC
int main(int argc, string argv[])
{    
    if (argchecker(argc, argv) == false)
    {
        return 1;
    }
  
    string keyword = argv[1];
    
    //prompt for plaintext
    printf("plaintext: ");
    string plaintext = get_string();
    
    printf("ciphertext: ");
    
    // z will keep track of the no. of non-alpha chars plaintext chars that have appeared in our
    // following loop so far
    
    int z = 0;
    
    for (int i = 0; i < strlen(plaintext); i++)
    {
        char plnlet = plaintext[i];
        
        //h will be used to make sure that j only interates when plnlet is a letter
        
        int h = i - 1 - z;
        
        //if plnlet is not alphabetical, ignore the encipher and print it as is
        
        if (isalpha(plnlet) == false)
        {
            printf("%c", plnlet);
            
            // but also iterate z!
            // I minus z from i (line 38) everytime to make sure to find the appropriate j
            // I do this to make sure the non-alpha chars don't make j unwantedly iterate!
            
            z++;
        }
        
        // OTHERWISE encipher!
        
        if (isalpha(plnlet))
        {
            // make h only iterate when i is a letter
            h = h + 1;
            
            //okay now we want to find the keyshift
            
            // make j wrap around to the beginning when run out of keyword letters
            // make j link to h not i => j only increases when plnlent is alphabetical
            // h is a function of z (line 38), which means that j doesn't increase due to numbers or spaces
            
            int kLen = strlen(keyword);
            int j = h % kLen;
            char keylet = keyword[j];
            
            
            // use keyletter to find keyshift
            
            int keyshift;
            
            if (isupper(keylet) != 0)       // if the keylet is uppercase
            {
                keylet = tolower(keylet);
                keyshift = (int)keylet - 97;
            }
            else                            // if the keylet is lowercase
            {
                keyshift = (int)keylet - 97;
            }
            
            // now apply keyshift to plnlet to find ciphint and ciphlet
            int ciphint = 0;
            char ciphlet = 'a';
            
            // if plnlet is capital find the capital output
            if (isupper(plnlet))
            {
                // apply vigenere formula
                ciphint = (plnlet + keyshift);
                
                // make wrap around if ciphint > Z
                if (ciphint > 90)
                {
                    ciphint = ciphint - 26;
                }
            
                // turn your number back into its letter
                ciphlet = (char) ciphint;
            }
            
            // now do the same but for lowercase plnlet
            if (islower(plnlet))
            {
                ciphint = (plnlet + keyshift);
                
                if (ciphint > 122)
                {
                    ciphint = ciphint - 26;
                }
                
                ciphlet = (char) ciphint;
            }
            
            // print ciphlet
            printf("%c", ciphlet);

        }
    }
    
    // print newline when finished
    printf("\n");
}

//FUNCTION that checks that the command line argument has only one extra argument
//also checks that the extra argument contrains only alphabetical characters
bool argchecker(int chkargc, string chkargv[])
{
    // make sure cla has only 2 arguments (name of program and keyword)
    if (chkargc > 2)
    {
        printf("You must only have one keyword!\n");
        return false;
    }
    if (chkargc < 2)
    {
        printf("You must have atleast one command line argument for the keyword!\n");
        return false;
    }
    
    // now check that your keyword is only alphabetical
    string argv1 = chkargv[1];
    
    //loop that chekcs that argv1 only contains alphabetical chars
    for (int i = 0; i < strlen(argv1); i++)
    {
        char argv1curr = argv1[i];
    
        if (isalpha(argv1curr) == false)
        {
            printf("You must only use letters in your keyword!\n");
            return false;
        }
    }
    printf("Your keyword is %s\n", argv1);
    return true;
}