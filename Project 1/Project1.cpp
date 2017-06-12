#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define Z00	0.
#define Z10	1.
#define Z20	0.
#define Z30	0.

#define Z01	1.
#define Z11	6.
#define Z21	1.
#define Z31	0.

#define Z02	0.
#define Z12	1.
#define Z22	0.
#define Z32	4.

#define Z03	3.
#define Z13	2.
#define Z23	3.
#define Z33	3.

double Height(int iu, int iv, int NUMS){// iu,iv = 0 .. NUMS-1

	double u = (double)iu / (double)(NUMS-1);
	double v = (double)iv / (double)(NUMS-1);

	// the basis functions:

	double bu0 = (1.-u) * (1.-u) * (1.-u);
	double bu1 = 3. * u * (1.-u) * (1.-u);
	double bu2 = 3. * u * u * (1.-u);
	double bu3 = u * u * u;

	double bv0 = (1.-v) * (1.-v) * (1.-v);
	double bv1 = 3. * v * (1.-v) * (1.-v);
	double bv2 = 3. * v * v * (1.-v);
	double bv3 = v * v * v;

	// finally, we get to compute something:

	double height = 	  bu0 * ( bv0*Z00 + bv1*Z01 + bv2*Z02 + bv3*Z03 )
			+ bu1 * ( bv0*Z10 + bv1*Z11 + bv2*Z12 + bv3*Z13 )
			+ bu2 * ( bv0*Z20 + bv1*Z21 + bv2*Z22 + bv3*Z23 )
			+ bu3 * ( bv0*Z30 + bv1*Z31 + bv2*Z32 + bv3*Z33 );

	return height;
}

double tileArea(int NUMS){
	return (((XMAX-XMIN)/(double)(NUMS-1)) * ((YMAX - YMIN)/(double)(NUMS-1)));
}



int main(int argc, char *argv[ ]){
	double totalVolume = 0, startTime = 0, endTime;
	int NUMT = 0, NUMS = 0;
	
	#ifndef _OPENMP
		cout << "OPENMP is not defined! Exiting\n";
		return 1;
	#endif
	
	if(argc == 1){
		cout << "How many threads would you like to use?" << endl;
		cin >> NUMT;
		cout << "How many subdivisions would you like to use?" << endl;
		cin >> NUMS;
	}
	else if(argc = 3){
		NUMT = atoi(argv[1]);
		NUMS = atoi(argv[2]);
	}
	else{
		cout << "Invalid number of parameters, exiting now!" << endl;
		return 1;
	}
	
	cout << "The number of threads used is: " << NUMT << endl;
	cout << "The number of subdivisions is: " << NUMS << endl;
	
	double fullTileArea = tileArea(NUMS);
	omp_set_num_threads(NUMT);
	startTime = omp_get_wtime();
	#pragma omp parallel for default(none), shared(fullTileArea, NUMS), reduction(+:totalVolume)
	for(int i = 0; i< NUMS*NUMS; i++){
		int iu = i % NUMS;
		int iv = i / NUMS;
		if(iv == 0 && iu > 0){
			totalVolume = totalVolume + (fullTileArea/2) * Height(iu, iv, NUMS); //Bottom edge of graph
		}
		else if(iu == 0 && iv > 0){
			totalVolume = totalVolume + (fullTileArea/2) * Height(iu, iv, NUMS); //Left edge of graph
		}
		else if(iv == NUMS && (iu < NUMS && iu != 0)){
			totalVolume = totalVolume + (fullTileArea/2) * Height(iu, iv, NUMS); //Top edge of graph
		}
		else if(iu == NUMS && (iv < NUMS && iv != 0)){
			totalVolume = totalVolume + (fullTileArea/2) * Height(iu, iv, NUMS); //Right edge of graph
		}
		else if(iv == 0 && iu == 0){
			totalVolume = totalVolume + (fullTileArea/4) * Height(iu, iv, NUMS); // Left most bottom edge 0,0
		}
		else if(iv == 0 && iu == NUMS){
			totalVolume = totalVolume + (fullTileArea/4) * Height(iu, iv, NUMS); // Left most Top edge 0, NUMS
		}
		else if(iv == NUMS && iu == 0){
			totalVolume = totalVolume + (fullTileArea/4) * Height(iu, iv, NUMS); // Right most bottom edge NUMS,0
		}
		else if(iv == NUMS && iu == NUMS){
			totalVolume = totalVolume + (fullTileArea/4) * Height(iu, iv, NUMS); // Right most top edge NUMS,NUMS
		}
		else{
			totalVolume = totalVolume + fullTileArea * Height(iu, iv, NUMS); // Normal tile
		}
	}
	
	endTime = omp_get_wtime();
	cout << "It took: " << std::setprecision(15) << endTime - startTime << " to calculate the numerical integration" << endl;
	cout << "The integration is: " << totalVolume << endl;
	cout << "T1 = " << endTime << endl << "T0 = " << startTime << endl;
	cout << "MegaFlops per second: " << NUMS*NUMS/(endTime-startTime)/1000000 << endl;
	return 0;
}
