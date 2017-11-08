#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // make sure there are only 2 args
    if (argc != 2)
    {
        printf("There must be 2 argumnets!\n");
        return 1;
    }
    
    // turn argv[1] into an int
    string keynumber = argv[1];
    int key = atoi(keynumber);
    // add a modulo to wrap key around when key > 26
    key = key % 26;

    string plntxt = NULL;
    // prompt for plaintext input
    do {
        printf("plaintext: ");
        plntxt = get_string();
    }
    while (plntxt == NULL);
    
    //print cipertext indicator
    printf("ciphertext: ");
    
    // use a for loop to cycle through letters of input string
    for (int i = 0; i < strlen(plntxt); i++)
    {
        int letter = plntxt[i];
        // add one to each letter in the loop
        
        // make cipher only happen if the letter is a letter
        if (isalpha(letter))
        {
            int newlet = letter + key;
            if (newlet > 90 && newlet < 97)
            {
                //make letters that go >z, swing back round to the beginning of the alphabet
                newlet = newlet - 26;
            }
            if (newlet > 122)
            {
                newlet = newlet - 26;
            }
            //print the changed letter
            printf("%c", (char) newlet);
        }
        
        // if letter is not alphabetical make program print as is
        else
        {
            printf("%c", letter);
        }
    }
    printf("\n");
}