/*********************************************************************
** Author: James G Wilson
** Date: 5/14/2018
** Description: Project 4 CS 475
** Grainville and graindeer simulation.
**
** A month-by-month simulation of a grain-growing operation.
** The amount the grain grows is affected by the temperature,
** amount of precipitation, and the number of "graindeer" around 
** to eat it. The number of graindeer depends on the amount of grain 
** available to eat. 
*********************************************************************/
#include <iostream>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

// Grainville functions.
void GrainDeer();
void Grain();
void Watcher();
void alienVisitor();

// Utility functions. 
void computeWeather(); 
void printState();
float SQR(float);
float Ranf(unsigned int*, float, float);
int Randf(unsigned int*, int, int);
unsigned int seed = 0;  // a thread-private variable

// The "state" of the system consists of the following global variables.
int	NowYear;			// 2017 - 2022
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;			// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int   aliensVisited = 0; 

// Initialize temperature and precipitation constants. 
const float GRAIN_GROWS_PER_MONTH =		8.0;	// inches
const float ONE_DEER_EATS_PER_MONTH =	0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average (inches)
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =				2.0;	// plus or minus noise

const float AVG_TEMP =						50.0;	// average (deg_F)
const float AMP_TEMP =						20.0;	// plus or minus
const float RANDOM_TEMP =					10.0;	// plus or minus noise

const float MIDTEMP =						40.0;
const float MIDPRECIP =						10.0;

int main()
{
	
	// Starting date and time:
	NowMonth =    0;
	NowYear  = 2017;

	// Starting state (feel free to change):
	NowNumDeer = 1;
	NowHeight =  1.;
	
	// Starting temperature and precipitation.
	computeWeather();
	
	printState();
	
	// Know how many threads already, start them with parallel sections
	omp_set_num_threads( 4 );	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer( );
		}
	
		#pragma omp section
		{
			Grain( );
		}
	
		#pragma omp section
		{
			Watcher( );
		}
	
		#pragma omp section
		{
			alienVisitor( );	
		}
	}	// implied barrier -- all functions must return in order
		// to allow any of them to get past here
		
	return 0;
	
}

// Compute next number of graindeer, based on set of global variables,
// into local, temporary, variables. 
void GrainDeer()
{
	
	while( NowYear < 2023 )
	{
		
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		float tempNumDeer = NowNumDeer;
		
		if(tempNumDeer > NowHeight)
		{
			tempNumDeer--;
		}
		else
		{
			tempNumDeer++;
		}
		
		// DoneComputing barrier:
		#pragma omp barrier
		if(aliensVisited == 0)
		{
			NowNumDeer = tempNumDeer;
		}
	
		// DoneAssigning barrier:
		#pragma omp barrier
		
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}

}

// Compute next grain growth, based on set of global variables,
// into local, temporary, variables. 
void Grain()
{
	
		while( NowYear < 2023 )
	{
		
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		float tempNowHeight = NowHeight;
		float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
		float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
		
		tempNowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		float temptempNowHeight = tempNowHeight; // crappy hack to clamp height to zero xD 
 		temptempNowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
 		
 		if(temptempNowHeight <= 0)
 		{
 			tempNowHeight = 0;
 		}
 		else
 		{
 			tempNowHeight = temptempNowHeight; 
 		}
	
		// DoneComputing barrier:
		#pragma omp barrier
		if(aliensVisited == 0)
		{
			NowHeight = tempNowHeight;
		}
			
		// DoneAssigning barrier:
		#pragma omp barrier
		//. . .
	
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}
	
}

void Watcher()
{
	
		while( NowYear < 2023 )
	{
	
		// DoneComputing barrier:
		#pragma omp barrier

		// DoneAssigning barrier:
		#pragma omp barrier
		
		// Print current set of global state variables. 
		printState();
		
		// Increment month count.
		NowMonth++;
		
		if(NowMonth == 12)
		{
			NowMonth = 0;
			NowYear++; 
		}
		
		// Compute new temperature and precipitation. 
		computeWeather();
		
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}
	
}

// Aliens visit Grainville and control the growth of grain or the graindeer population. 
void alienVisitor()
{
	
	while( NowYear < 2023 )
	{
		
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		float tempNumDeer = NowNumDeer;
		float tempNowHeight = NowHeight;
				
		aliensVisited = 0;
		
		if(NowYear == 2020 && NowMonth == 2)
		{
			
			aliensVisited = 1;
			
			// Graindeer from a far away galaxy find refuge in Grainville.
			// They will not land if the current population is too high.
			if(tempNumDeer < 10)
			{
				tempNumDeer++;
				tempNumDeer++;
				tempNumDeer++;
				tempNumDeer++;
				tempNumDeer++;
				tempNumDeer++;
			}
			
			// They bring with them the grain salvaged from their alien grainville. 
			// Even if they dont stay, they leave the grain behind. 
			tempNowHeight++;
			tempNowHeight++;
			tempNowHeight++;
			
		}
		
		// DoneComputing barrier:
		#pragma omp barrier
		if(aliensVisited == 1)
		{
			NowNumDeer = tempNumDeer;
			NowHeight = tempNowHeight;
		}

		// DoneAssigning barrier:
		#pragma omp barrier
		
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}
	
}

// Calculate temperature and precipitation.
void computeWeather()
{
	
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	unsigned int seed = 0;
	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
	
	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
	{
		NowPrecip = 0.;
	}
	
}

// Print current set of global state variables. 
void printState()
{
	std::cout << "Year #: " << NowYear << "\n";
	std::cout << "Month #: " << NowMonth << "\n";
	
	if(aliensVisited == 1)
	{
		std::cout << "***ALIENS VISITED***" << "\n";
	}
	
	std::cout << "Temperature (F): " << NowTemp << "\n";
	std::cout << "Temperature (C): " << (5./9.)*(NowTemp-32) << "\n";
	std::cout << "Precipitation (in): " << NowPrecip << "\n";
	std::cout << "Precipitation (cm): " << NowPrecip*2.54 << "\n";
	std::cout << "Number of graindeer: " << NowNumDeer << "\n";
	std::cout << "Height of grain (in): " << NowHeight << "\n";
	std::cout << "Height of grain (cm): " << NowHeight*2.54 << "\n";

}

// Utility function.
float SQR(float x)
{
	
	return x*x;

}

// Random utility function. 
float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

