#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // prompt for card number
    long cardno = get_long_long();
    printf("%ld\n", cardno);
    
    // now test if the cardno is valid legnth
    // make sure cardno is >=13 and <=16
    
    // use n and digits as variables to help calucalte number of digits
    long n = cardno;
    int digits = 0;
    
    // iterate by dividing card no by 10
    // see how many iterations take u to 0, this is you no. of digits
    while(n > 0)
    {
        // n = n/10
        n /= 10;
        ++ digits;
    }
    
    // print INVALID if not right no. of digits
    if (digits < 13 || digits > 16)
    {
        printf("INVALID\n");
    }
    
    // now get the second to last digit
    // take modulo 10 for the last digit
    // take modulo 100 for the last 2 digits
    // minus modulo 100 by modulo 10 for the second to last digit (but in the ten slot)
    // divide by 10 to make it a single digit int
    
    int sld = (cardno % 100) - (cardno % 10);
    sld = sld / 10;
    printf("%i\n", sld);
}