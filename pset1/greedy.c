#include <stdio.h>
#include <cs50.h>
#include <math.h> //include so we can use floor and ceil functions

int main(void)
{
    float chg_amt;
    
    printf("O hai! How much change is owed?\n");
    
    do
    {
        chg_amt = get_float();
    }
    while (chg_amt < 0);
    // reject amounts that are lower than 0 and are not floats
    
    // convert dollar amount into cents
    chg_amt = chg_amt * 100;
    int change;
    
    //okay now we need to round to the nearest integer
    
    //round up if (ceil - amt) < 0.5 => thus the amt decimal is <.5 
    if (ceil(chg_amt) - chg_amt < 0.5)
    {
        change = ceil(chg_amt);
    }
    
    // otherwise round down
    else
    {
        change = floor(chg_amt);
    }
    
    // find how many quarters can be used
    int quarters = change / 25;
    //find remaining change
    change = change % 25;
    
    // find how many dimes can be used
    int dimes = change / 10;
    change = change % 10;
    
    // find how many 5 cents can be used
    int fivecents = change / 5;
    change = change % 5;
    
    // find how many one cents are needed
    int onecents = change;
    
    int coin_no = quarters + dimes + fivecents + onecents;
    printf("%i\n", coin_no);
}