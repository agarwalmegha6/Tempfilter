/**************************************************************************************************/

/*
 * File: main.c
 * Author: Megha Agarwal
 * NetID: meghaagarwal
 * Date: 09/16/2015
 *
 * Description: Main functionality for simple C program that reads an input file formatted with
 * temperature data, manipulates and smooths that data, and write analysis to an output file.
 *
 */

/**************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "bool.h"
#include "tempdata.h"
#include "filter.h"

/**************************************************************************************************/

int
main(int argc, char *argv[])
{
   ACTempData acTemps[MinPerDay];
   
   /*
    * check for the correct number of command line arguments.  If incorrect
    * provide a simple usage message to the assist the user
    *
    */
	if( argc != 3 ) {
      printf("\nUsage: %s inputFile outputFile \n\n", argv[0]);
      return -1;
	}
	
   if( !ReadTempDataFromFile(acTemps, argv[1]) ) {
      printf("Could not read from input file %s\n", argv[1]);
      return -1;
   }
    
   // Calls the necessary methods.
   RemoveErroneousData(acTemps);
   LowPassFilter(acTemps);
   TrendExtraction(acTemps);
   
   if( !WriteTempDataToFile(acTemps, argv[2]) ) {
      printf("Could not write to output file %s\n", argv[2]);
      return -1;
   }

	return 0;
}

/**************************************************************************************************/
