#include <stdio.h>
#include <cs50.h>

int main(void)
{

    int height;
    int no_of_hash;
    int no_of_space;
    
    //set variables that will be used for building the pyramid.

    do
    {
        printf("Height: ");
        height = get_int();
    }
    while (height < 0 || height > 23);
    
    //prompt for height of pyramind, reject <0 or >= 24
    
    for (int i = 0; i < height; i++)
    {
        no_of_hash = 2 + i;
        no_of_space = height - 1 - i;
        int space_left = no_of_space;
        int hash_left = no_of_hash;
        
        //establish a for loop that grows the no. of hashes and shrinks the no. of spaces of each row
        
        while (space_left > 0)
        {
            printf(" ");
            space_left--;
        }
        
        //print space until no. of spaces = "height - 1 - i"
        
        while (hash_left > 0)
        {
            printf("#");
            hash_left--;
            
        //print hashes until no. of hashes =  "2 + i"    
      
        }
        printf("\n");
    }
}