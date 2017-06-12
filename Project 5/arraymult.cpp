#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "simd.p5.h"

float array1[aSize];
float array2[aSize];
float array3[aSize];
using namespace std;

int main(){
	ofstream data;
	data.open("data.csv", ios::out | ios::app);
	int rando 				= 0;
	double simdStart		= 0;
	double simdEnd			= 0;
	double owncodeStart		= 0;
	double owncodeEnd		= 0;
	double simdRedStart		= 0;
	double simdRedEnd		= 0;
	double owncodeRedStart	= 0;
	double owncodeRedEnd	= 0;
	
	//Run SEE SIMD
		cout << "SEE SIMD	Array Size: " << aSize << endl;
		simdStart = omp_get_wtime();
		SimdMul(array1, array2, array3, aSize);
		simdEnd	= omp_get_wtime();
		//data << "simd" << "," << aSize << "," << simdEnd-simdStart << endl;

	//Run own code
		cout << "Own Code	Array Size: " << aSize << endl;
		owncodeStart = omp_get_wtime();
		for(int i = 0; i < aSize; i++){
			array3[i] = array1[i] * array2[i];
		}
		owncodeEnd = omp_get_wtime();
		//data << "own Code" << "," << aSize << "," << owncodeEnd-owncodeStart << endl;
		data << "SIMD: ," << (owncodeEnd-owncodeStart)/(simdEnd-simdStart) << "," << aSize << endl;
	
	//run SEE SIMD Reduction
		cout << "SEE SIMD Reduct	Array Size: " << aSize << endl;
		simdRedStart = omp_get_wtime();
		SimdMulSum(array1, array2, aSize);
		simdRedEnd = omp_get_wtime();
		//data << "simd reduct" << "," << aSize << "," << simdRedEnd-simdRedStart << endl;
		
	//run own code reduction
		int sum = 0;
		cout << "Own reduction	Array Size: " << aSize << endl;
		owncodeRedStart = omp_get_wtime();
		for(int i = 0; i < aSize; i++){
			sum+=array1[i] + array2[i];
		}
		owncodeRedEnd = omp_get_wtime();
		//data << "own Reduct" << "," << aSize << "," << owncodeRedEnd-owncodeRedStart << endl;
		data << "Reduction: ," << (owncodeRedEnd-owncodeRedStart)/(simdRedEnd-simdRedStart) << "," << aSize << endl;
	data.close();
	return 0;
}