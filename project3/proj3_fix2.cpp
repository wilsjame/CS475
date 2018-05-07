/*********************************************************************
** Author: James G Wilson
** Date: 5/2/2018
** Description: Project 3 CS 475
** False sharing fix #1 and #2. 
** Fix #1: Pad the structure to try and put data on different cache lines.
** 	   Force succesive array elements  on different cache lines so less or no cache line conflicts exist!
** Fix #2: Use local variables instead of contiguous array locations.
**	   Recall, each thread has its own stack. 
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include <iostream>

// These will be varied automatically whe using the benchmark script
// If Fix #2, no need to vary NUMPAD
#define NUMPAD 0
//#define NUMT 1

struct s

{

         float value;

         int pad[NUMPAD];

} Array[4];

 
int main()
{

#ifndef _OPENMP

         fprintf( stderr, "OpenMP is not available\n" );

         return 1;

#endif

 

         omp_set_num_threads( NUMT );

         int numProcessors = omp_get_num_procs( );

         //fprintf( stderr, "Have %d processors.\n", numProcessors );

        

 

         int someBigNumber = 1000000000;

 

	// Set up timer
	double time0 = omp_get_wtime( );
	 
	 
         #pragma omp parallel for 

         for( int i = 0; i < 4; i++ )

         {

		 // Fix #2
		 // Makes this a private variable that lives in each thread's
		 // individual stack :)
		 // This works because a localized temporary variable is 
		 // created in each core (threads) stack area, so little or no cache
		 // line conflicts exist! :D
		 float tmp = Array[ i ].value;

                 for( int j = 0; j < someBigNumber; j++ )

                 {

                          Array[ i ].value = Array[ i ].value + 2.;

                 }

         }

	double time1 = omp_get_wtime( );


	// Calculate MegaAdds Per Second
	float megaAddsPerSec = (((float)someBigNumber * NUMT)/(time1-time0)/1000000);

        // print performance here:::
	std::cout << "time = " << time1 - time0 << " sec\n";
	std::cout << "megaAddsPerSec = " << megaAddsPerSec << "\n"; 
	//std::cout << megaAddsPerSec << "\n";

}
