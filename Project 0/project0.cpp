#include<math.h>
#include<omp.h>
#include<stdio.h>

#define NUMT 4
#define NUMT1 1
#define ARRAYSIZE 1000000000 //one billion
#define NUMTRIES 5

int main(){
	double maxmmults = 0.;
	double summmults = 0.;
	double time0 = 0.000;
	double time1 = 0.000;
	double mmults = 0.000;
	#ifndef _OPENMP
			fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
			return 1;
	#endif
	float *A = new float[ARRAYSIZE];
	float *B = new float[ARRAYSIZE];
	float *C = new float[ARRAYSIZE];
	
	//Setup for using 1 thread
	omp_set_num_threads(NUMT1);
	fprintf(stderr, "Using %d threads\n", NUMT1);
	for(int t = 0; t < NUMTRIES; t++){
			double time0 = omp_get_wtime();
			#pragma omp parallel for
			for(int i = 0; i < ARRAYSIZE; i++){
					C[i] = A[i] * B[i];
			}

			double time1 = omp_get_wtime( );
			double mmults = (double)ARRAYSIZE/(time1-time0)/1000000.;
			summmults += mmults;
			if(mmults > maxmmults){
				maxmmults = mmults;
			}
	}
	printf( "\nPeak Performance = %8.2lf MegaMults/Sec\n", maxmmults );
	printf( "\nAverage Performance = %8.2lf MegaMults/Sec\n", summmults/(double)NUMTRIES );
	
	//Setup for using 4 thread
	maxmmults = 0.;
	summmults = 0.;
	time0 = 0.000;
	time1 = 0.000;
	mmults = 0.000;
	omp_set_num_threads(NUMT);
	fprintf(stderr, "Using %d threads\n", NUMT);
	for(int t = 0; t < NUMTRIES; t++){
			time0 = omp_get_wtime();
			#pragma omp parallel for
			for(int i = 0; i < ARRAYSIZE; i++){
					C[i] = A[i] * B[i];
			}

			time1 = omp_get_wtime( );
			mmults = (double)ARRAYSIZE/(time1-time0)/1000000.;
			summmults += mmults;
			if(mmults > maxmmults){
				maxmmults = mmults;
			}
	}
	printf( "\nPeak Performance = %8.2lf MegaMults/Sec\n", maxmmults );
	printf( "\nAverage Performance = %8.2lf MegaMults/Sec\n", summmults/(double)NUMTRIES );
	
    return 0;
}