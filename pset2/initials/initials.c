#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string name = NULL;
    //prompt for name
    do
    {
        name = get_string();
    }
    while (name == NULL);
    
    /*  get first letter of string ignoring space characters
        print this char in capital letters
    */
     
    for (int i = 0; i < strlen(name); i++)
    {
        if (name[0] != ' ')
        {
            char firstini = name[0];
            printf("%c", toupper(firstini));
        }
        break;
        
    /*  get first letter of subsequent strings that appear after a space
        (once again ignoring chars that are spaces themselves)
        print these chars
    */
    
    }
    for (int j = 1; j < strlen(name); j++)
    {
        if (name[j] == ' ')
        {
            if (name[j + 1] != ' ')
            {
                char secondini = name[j + 1];
                printf("%c", toupper(secondini));
            }
        }
        
    }
    printf("\n");
}