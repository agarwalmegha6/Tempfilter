/**************************************************************************************************/

/*
 * File: tempdata.c
 * Author: Megha Agarwal
 * NetID: meghaagarwal
 * Date: 09/16/2015
 *
 * Description: Implementation of functions for operating on the ACTempData structure.
 *              Note that the LowPassFilter fcn has been moved to filter.{h,c}
 *
 */

/**************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "bool.h"
#include "tempdata.h"

/**************************************************************************************************/

/* Reads tempertures from an input file specified by fileName line-by-line using the format:
 *
 *    HH:MM TT.T
 *
 * HH:MM represents the time at which the temperature was recorded and is specific in 24 hour
 * format.
 *
 * TT.T represents the recorded temperature reported in Fahrenheit as a double precision value.
 *
 * The time and temperature will be separated by one or more whitespace characters.
 *
 * All lines that do not exactly match this format will be ignored.
 *
 * Returns false if the specified file could not be opened, and true otherwise.
 *
 */
bool ReadTempDataFromFile(ACTempData acTemps[], char *fileName)
{
    int j = 0; // Counter Variable
    int hr = 0; // hr and min are used to store the values of time.
    int min = 0;
    double temp = 0.0;
    char lineBuffer[100];
    FILE* inFile = NULL; // Resding from file.
    inFile = fopen(fileName,"r");
    if(inFile==NULL) // Checks if the file could be opened or not.
        return false;
    for(j=0;j<MinPerDay;j++)
    {
        acTemps[j].temperature = 101.0;
        acTemps[j].valid = false;
        acTemps[j].status = false;
    }
    while(!feof(inFile)) // Reads the hr, min and the temperature.
    {
        fgets(lineBuffer, 100, inFile);
        sscanf(lineBuffer,"%2d:%2d %lf",&hr, &min, &temp);
        acTemps[(hr*60)+min].temperature = temp;
        acTemps[(hr*60)+min].valid = true;
    }
    fclose(inFile); // Closes file.
    
   return true;
}

/**************************************************************************************************/

/*
 * Remove any erroneous temperature readings. An error temperature reading is defined as a
 * temperature reading that is either more than 5 degrees above or 5 below the temperature
 * recorded in the previous minute. All erroneous temperature readings should be marked as invalid.
 * The first element read in from a file is always considered valid.
 */
void RemoveErroneousData(ACTempData acTemps[])
{
    int k=0; // Initialize counter variable.
    for(k=1;k<MinPerDay;k++)
    {
        // Checks if temperature is invalid or not.
        if(acTemps[k].temperature == 101.000)
        {
            continue;
        }
        if(acTemps[k-1].temperature == 101.000)
            acTemps[k].valid = true;
        else if(((acTemps[k].temperature-acTemps[k-1].temperature) > 5 || (acTemps[k-1].temperature-acTemps[k].temperature) > 5) && (acTemps[k-1].valid == true))
        {
            acTemps[k].valid = false;
        }
    }
}

/**************************************************************************************************/

/*
 * Analyzes the sequence of valid temperatures to determine the state of the AC. If a non-valid
 * temperature reading is found, the trend extraction should restart at the next valid temperature
 * reading. Trend extraction analysis operate as follows:
 *
 * 1. The AC status for the first entry in a sequence of valid reading is always Off.
 *
 * 2. For a valid temperature reading that does not start a sequence:
 *
 *    A. If the previous AC status was Off, then consider the following: If (1) the previous 
 *       temperature is valid, and (2) the current temperature is less than the previous temperature, 
 *       and (3) the next temperature is valid and less than the current temperature, then the AC 
 *       status for the current temperature will be On.
 *
 *    B. If the previous AC status was On, then consider the following: If the current temperature 
 *       is less than or equal to the previous temperature, then the AC status for the current 
 *       temperature will be On
 *
 *    C. Otherwise, the AC status for the current temperature will be Off.
 *
 */
void TrendExtraction(ACTempData acTemps[])
{
    int l = 0; // Initialize counter variable.
    acTemps[0].status = false; // Sets first reading's status to false.
    for(l=1;l<MinPerDay;l++)
    {
        // Checks for validity.
        if(acTemps[l].valid == true)
        {
            // Goes through the conditions one by one.
            if(acTemps[l-1].valid == false)
                acTemps[l].status = false;
            else
            {
                if(acTemps[l-1].status == false)
                {
                    if(acTemps[l-1].valid == true && acTemps[l].temperature<acTemps[l-1].temperature && acTemps[l+1].valid == true && acTemps[l+1].temperature<acTemps[l].temperature)
                        acTemps[l].status = true;
                    else
                        acTemps[l].status = false;
                }
                else
                {
                    if(acTemps[l].temperature <= acTemps[l-1].temperature)
                        acTemps[l].status = true;
                    else
                        acTemps[l].status = false;
                }
                }
            }
        }
    }

/**************************************************************************************************/

/* Writes all valid tempertures readings and AC status to an output file specified by fileName
 * using the format:
 *
 *    HH:MM TT.TTT AC
 *
 * HH:MM reports the time at which the temperature was recorded specified in 24 hour format.
 *
 * TT.TTT represents the filtered temperature value in Fahrenheit as a double precision value with
 * three decimal digits of precision.
 *
 * AC reports the determined status of the air conditioner, where a 0 represents the the AC
 * was Off and a 1 represents the AC was On.
 *
 * Each entry is separated by a single tab character (\t).
 *
 * Retruns false if the specified file could not be written to, and true otherwise.
 *
 */
bool WriteTempDataToFile(ACTempData acTemps[], char *fileName)
{
    int m = 0, n = 0; // Used as counter variables.
    FILE* outFile = NULL; // Writing into a file.
    outFile = fopen(fileName, "w");
    if(outFile == NULL)
        return false;
    for(m=0;m<MinPerDay;m++) // Checks for temperature validity and then converts array size to time.
    {
        if(acTemps[m].valid == true && acTemps[m].temperature != 101.0)
        {
            if(acTemps[m].status == true)
                n = 1;
            else
                n = 0;
        fprintf(outFile, "%02d:%02d\t%02.3lf\t%d\n",(m/60),(m%60),acTemps[m].temperature,n);
        }
    }
    fclose(outFile); // Closes file.
   return true; // Returns true.
}

/**************************************************************************************************/

