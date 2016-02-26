/**************************************************************************************************/

/*
 * File: filter.h
 * Author: Megha Agarwal
 * NetID: meghaagarwal
 * Date: 09/16/2015
 *
 * Description: Declaration of function to perform simple low-pass filter.
 *
 */

#ifndef FILTER_H
#define FILTER_H

/**************************************************************************************************/

#include "bool.h"
/* Contains the ACTempData struct definition */
#include "tempdata.h"

/**************************************************************************************************/

/*
 * Applies a low-pass filter to sequences of valid temperature readings for consecutive times.
 * If a non-valid temperature reading is found, the filter should restart at the next valid
 * temperature reading. The following describes the low-pass filter used:
 *
 *    filtered_output(0) = unfiltered_input(0)
 *    filtered_output(n) = 0.9250 * filtered_output(n-1) + 0.0750 * unfiltered_input(n)
 *
 */
void LowPassFilter(ACTempData acTemps[]);

#endif // FILTER_H
