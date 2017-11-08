/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    //set beginning and end points
    int beg = 0;
    int end = n - 1;
    
    while (beg < end)
    {
        //set middle point
        int mid = (beg + end) / 2;
        
        //if number is equal to mid value, return true
        if (values[mid] == value)
        {
            return true;
        }
        
        //if your number is higher than mid value, make the lower bound index to the left of mid
        if (values[mid] < value)
        {
            beg = mid + 1;
        }
        
        //if your number is lower than mid value, make upper bound index to right of mid
        if (values[mid] > value)
        {
            end = mid - 1;
        }
        
        // else if your number is not found and there are no more values to compare it to:
        // end search and return false
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    // selection sort attempt
    
    for (int i = 0; i < n - 1; i++)
    {
        // set min index value of array to furthest left non-sorted value
        int imin = i;
        
        for (int j = i + 1; j < n; j++)
        {
            // test if current j value is smaller than imin value
            if (values[j] < values[imin])
            {
                // update imin to be j if the case
                imin = j;
            }
            
            // now swap imin value with the current furthest left unsorted array value
            // use temp var to store unswapped value before swap
            int temp = values[i];
            values[i] = values[imin];
            values[imin] = temp;
        }
    }
    return;
}
