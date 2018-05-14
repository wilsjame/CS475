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
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

// Grainville functions.
void GrainDeer();
void Grain();
void Watcher();
//void MyAgent();

// Utility functions. 
float SQR(float);
float Ranf(unsigned int*, float, float);
int Randf(unsigned int*, int, int);
unsigned int seed = 0;  // a thread-private variable
//float x = Ranf( &seed;, -1.f, 1.f );

// The "state" of the system consists of the following global variables.
int	NowYear;			// 2017 - 2022
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;			// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population

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
	
	// Calculate temperature and precipitation.
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
	
	// Know how many threads already, start them with parallel sections
	omp_set_num_threads( 3 );	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			//GrainDeer( );
		}
	
		#pragma omp section
		{
			//Grain( );
		}
	
		#pragma omp section
		{
			//Watcher( );
		}
	
		#pragma omp section
		{
			//MyAgent( );	// your own
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
		//. . .
	
		// DoneAssigning barrier:
		#pragma omp barrier
		//. . .
	
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
		float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
		float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
		float tempNowHeight = NowHeight;
		
		NowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 		tempNowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
 		
 		if(tempNowHeight <= 0)
 		{
 			NowHeight = 0;
 		}
 		else
 		{
 			NowHeight = tempNowHeight; 
 		}
	
		// DoneComputing barrier:
		#pragma omp barrier
		//. . .
	
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
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		//. . .
	
		// DoneComputing barrier:
		#pragma omp barrier
		//. . .
	
		// DoneAssigning barrier:
		#pragma omp barrier
		//. . .
	
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}
	
}

//void MyAgent();

float SQR(float x)
{
	
	return x*x;

}

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

