#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Minutes: ");
    int minutes = get_int();
    
    if (minutes > 0)
    {
        int bottles = minutes * 12;
        printf("Bottles: %i\n", bottles);
    }
    else
        printf("You used no water!\n");
}