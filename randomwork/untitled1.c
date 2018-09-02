#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>

int main()
{
    char ch1 = 'P';
    char ch2 = tolower(ch1);
    int ch3 = ch2 - 'a';

    char c = 'c' - 'a';


    printf("ch1 = %i\n", ch1);
    printf("ch1 lower = ch2 = %c\n", ch2);
    printf("ch2 - 'a' = ch3 = %i\n", ch3);
    printf("c - a = %i\n", c);



    return 0;
}




