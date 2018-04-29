/*********************************************************************
** Author: James G Wilson
** Date: 4/20/2018
** Description: Project 2 CS 475
*********************************************************************/

#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <omp.h>

#include <iostream>

 

// constants:

 

const double G                    = 6.67300e-11;   // m^3 / ( kg s^2 )

const double EARTH_MASS           = 5.9742e24;     // kg

const double EARTH_DIAMETER       = 12756000.32;   // meters

const double TIMESTEP             =   1.0; // secs

 

#define NUMBODIES         100

#define NUMSTEPS           200

/* These will be varied automatically when using the benchmark script */
#define NUMTHREADS 1


 

struct body

{

         float mass;

         float x, y, z;            // position

         float vx, vy, vz;         // velocity

         float fx, fy, fz;         // forces

         float xnew, ynew, znew;

         float vxnew, vynew, vznew;

};

 

typedef struct body Body;

 

 

Body     Bodies[NUMBODIES];

 

 

// function prototypes:

 

float            GetDistanceSquared( Body *, Body * );

float            GetUnitVector( Body *, Body *, float *, float *, float * );

float            Ranf( float, float );

int              Ranf( int, int );

 

 

int

main( int argc, char *argv[ ] )

{

#ifndef _OPENMP

         fprintf( stderr, "OpenMP is not available\n" );

         return 1;

#endif

 

         omp_set_num_threads( NUMTHREADS );

         int numProcessors = omp_get_num_procs( );

         fprintf( stderr, "Have %d processors.\n", numProcessors );
	 

 

 

         for( int i = 0; i < NUMBODIES; i++ )

         {

                 Bodies[i].mass = EARTH_MASS  * Ranf( 0.5f, 10.f );

                 Bodies[i].x = EARTH_DIAMETER * Ranf( -100.f, 100.f );

                 Bodies[i].y = EARTH_DIAMETER * Ranf( -100.f, 100.f );

                 Bodies[i].z = EARTH_DIAMETER * Ranf( -100.f, 100.f );

                 Bodies[i].vx = Ranf( -100.f, 100.f );;

                 Bodies[i].vy = Ranf( -100.f, 100.f );;

                 Bodies[i].vz = Ranf( -100.f, 100.f );;

         };



	 // Set up timer
	 double time0 = omp_get_wtime( );



	 // Control scheduling by adding a clause to end of 
	 // omp parallel for. Use either schedule(static or dynamic).
	 //std::cout << "Coarse-grained, ";
	 std::cout << "Fine-grained, ";
	 std::cout << "Static\n";
	 //std::cout << "Dynamic\n";

         for( int t = 0; t < NUMSTEPS; t++ )

         {

		 // "Coarse-grained parallelism" omp parallel for here
		 //#pragma omp parallel for default(none), shared(Bodies), schedule(dynamic)
                 for( int i = 0; i < NUMBODIES; i++ )

                 {

                          float fx = 0.;

                          float fy = 0.;

                          float fz = 0.;

                          Body *bi = &Bodies[i];

			  // "Fine-grained parallelism" omp parallel for here
			  // fx, fy, fz need to undergo a reduction-add
			  #pragma omp parallel for default(none), shared(Bodies, i, bi), reduction(+:fx, fy, fz), schedule(static)
                          for( int j = 0; j < NUMBODIES; j++ )

                          {

                                   if( j == i )     continue;

 

                                   Body *bj = &Bodies[j];

 

                                   float rsqd = GetDistanceSquared( bi, bj );

                                   if( rsqd > 0. )

                                   {

                                            float f = G * bi->mass * bj->mass / rsqd;

                                            float ux, uy, uz;

                                            GetUnitVector( bi, bj,   &ux, &uy, &uz );

                                            fx += f * ux;

                                            fy += f * uy;

                                            fz += f * uz;

                                   }

                          }

 

                          float ax = fx / Bodies[i].mass;

                          float ay = fy / Bodies[i].mass;

                          float az = fz / Bodies[i].mass;

 

                          Bodies[i].xnew = Bodies[i].x + Bodies[i].vx*TIMESTEP + 0.5*ax*TIMESTEP*TIMESTEP;

                          Bodies[i].ynew = Bodies[i].y + Bodies[i].vy*TIMESTEP + 0.5*ay*TIMESTEP*TIMESTEP;

                          Bodies[i].znew = Bodies[i].z + Bodies[i].vz*TIMESTEP + 0.5*az*TIMESTEP*TIMESTEP;

 

                          Bodies[i].vxnew = Bodies[i].vx + ax*TIMESTEP;

                          Bodies[i].vynew = Bodies[i].vy + ay*TIMESTEP;

                          Bodies[i].vznew = Bodies[i].vz + az*TIMESTEP;

                 }
 


                 // setup the state for the next animation step:

 

                 for( int i = 0; i < NUMBODIES; i++ )

                 {

                          Bodies[i].x = Bodies[i].xnew;

                          Bodies[i].y = Bodies[i].ynew;

                          Bodies[i].z = Bodies[i].znew;

                          Bodies[i].vx = Bodies[i].vxnew;

                          Bodies[i].vy = Bodies[i].vynew;

                          Bodies[i].vz = Bodies[i].vznew;

                 }

 

 

         }  // t

 

         double time1 = omp_get_wtime( );


	 // Calculate MegaBodies Compared Per Second
	 double megaBodiesPerSec = ((double)(NUMBODIES*NUMBODIES*NUMSTEPS)/(time1-time0)/1000000);

         // print performance here:::
	 std::cout << "time = " << time1 - time0 << " sec\n";
	 std::cout << "megaBodiesPerSec = " << megaBodiesPerSec << "\n"; 
 


         return 0;

}

 

 

float

GetDistanceSquared( Body *bi, Body *bj )

{

         float dx = bi->x - bj->x;

         float dy = bi->y - bj->y;

         float dz = bi->z - bj->z;

         return dx*dx + dy*dy + dz*dz;

}

 

 

float

GetUnitVector( Body *from, Body *to, float *ux, float *uy, float *uz )

{

         float dx = to->x - from->x;

         float dy = to->y - from->y;

         float dz = to->z - from->z;

 

         float d = sqrt( dx*dx + dy*dy + dz*dz );

         if( d > 0. )

         {

                 dx /= d;

                 dy /= d;

                 dz /= d;

         }

 

         *ux = dx;

         *uy = dy;

         *uz = dz;

 

         return d;

}

 

 

float

Ranf( float low, float high )

{

         float r = (float) rand();         // 0 - RAND_MAX

 

         return(   low  +  r * ( high - low ) / (float)RAND_MAX   );

}

 

 

int

Ranf( int ilow, int ihigh )

{

         float low = (float)ilow;

         float high = (float)ihigh + 0.9999f;

 

         return (int)(  Ranf(low,high) );

}
