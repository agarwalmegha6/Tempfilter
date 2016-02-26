/**************************************************************************************************/

/*
 * File: filter.c
 * Author: Megha Agarwal
 * NetID: meghaagarwal
 * Date: 09/10/2015
 *
 * Description: Implementation of low-pass filter. This file is the only file that need by submitted
 * as part of the "alpha" for this assignment.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "filter.h"

/*
 * Applies a low-pass filter to sequences of valid temperature readings for consecutive times.
 * If a non-valid temperature reading is found, the filter should restart at the next valid
 * temperature reading. The following describes the low-pass filter used:
 *
 *    filtered_output(0) = unfiltered_input(0)
 *    filtered_output(n) = 0.9250 * filtered_output(n-1) + 0.0750 * unfiltered_input(n)
 *
 */
void LowPassFilter(ACTempData acTemps[])
{
    // Initialize a counter variable.
    int i=0;
    // Check if the first temperature reading is valid or not. Print accordingly.
    if(acTemps[0].valid == true)
    {
        
    }
    // Run for loop to check for the validity of all the other temperature readings. Calculate and print accordingly.
    for(i=1;i<MinPerDay;i++)
    {
        if(acTemps[i].valid == true && acTemps[i-1].valid == true)
        {
            acTemps[i].temperature = (0.9250*acTemps[i-1].temperature) + (0.0750*acTemps[i].temperature);
        }
    }
    
}

