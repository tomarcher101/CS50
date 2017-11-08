#include <stdio.h>
#include <cs50.h>

bool validtriang(float x, float y, float z);

int main(void)
{
    //get x, y and z values
    printf("How long is side x? ");
    float a = get_int();
    printf("How long is side y? ");
    float b = get_int();
    printf("How long is side z? ");
    float c = get_int();
    //input x, y and z into our validtriang function test!
    if (validtriang(a,b,c) == true){
        //print success if valid triangle
        printf("This is a valid triangle!\n");
    }
    else{
        //print fail if not valid traingle
        printf("You cannot make a triangle with these dimensions!\n");
    }
}
//triangle test function
bool validtriang(float x, float y, float z)
{
    //test pythagorean theorum for valid triangle sides
    if (x > y + z || y > x + z || z > z + y)
    {
        return false;
    }
    //test for valid natural side legnth
    if (x <= 0 || y <= 0 || z <= 0)
    {
        return false;
    }
    return true;
}