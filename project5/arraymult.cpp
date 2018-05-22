/*********************************************************************
** Author: James G Wilson
** Date: 5/21/2018
** Description: Project 5 CS 475
** arraymult.cpp 
*********************************************************************/
#include <iostream>
#include <omp.h>
#include "simd.p5.h"
//#define ARRAYSIZE 

void ArrayMult(int n, float *a, float *b, float *c);
void ArrayMultSum(int n, float *a, float *b);

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

int main()
{

	std::cout << "ARRAYSIZE: " << ARRAYSIZE << "\n";

	std::cout << "ArrayMult ";
	double time0 = omp_get_wtime();
	ArrayMult(ARRAYSIZE, A, B, C);
	double time1 = omp_get_wtime();
	std::cout << "Time: " << time1 - time0 << " sec\n";

	std::cout << "SimdMul ";
	time0 = omp_get_wtime();
	SimdMul(A, B, C, ARRAYSIZE);
	time1 = omp_get_wtime();
	std::cout << "Time: " << time1 - time0 << " sec\n";

	std::cout << "ArrayMultSum ";
	time0 = omp_get_wtime();
	ArrayMultSum(ARRAYSIZE, A, B);
	time1 = omp_get_wtime();
	std::cout << "Time: " << time1 - time0 << " sec\n";

	std::cout << "SimdMulSum";
	time0 = omp_get_wtime();
	SimdMulSum(A, B, ARRAYSIZE);
	time1 = omp_get_wtime();
	std::cout << "Time: " << time1 - time0 << " sec\n";

	return 0;

}

void ArrayMult(int n , float *a, float *b, float *c)
{
	
	for(int i = 0; i < n; i++)
	{
		c[i] = a[i] * b[i];
	}

}

void ArrayMultSum(int n , float *a, float *b)
{
	double sum = 0;
	
	for(int i = 0; i < n; i++)
	{
		sum += a[i] * b[i];
	}

}
