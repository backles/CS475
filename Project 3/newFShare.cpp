#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
using namespace std;

#if !defined (NUM)
	#define NUM 0
#endif
#if !defined (PADDING)
	#define PADDING 0
#endif

struct s{
	float value;
	int pad[NUM];
} Array[4];

int main(){
	#ifndef _OPENMP
		cout << "Open MP is not defined. Exiting" << endl;
		return 1;
	#endif
	double T1Pad = 0, T1UnPad = 0;
	long someBigNumber = 1000000000; // if > 4B, use "long unsigned int"
	ofstream data;
	data.open("data.csv", ios::out | ios::app);
	for(int NUMT = 1; NUMT <= 4; NUMT=NUMT*2){	//To do for all cores
		//FIX #1
		if(PADDING == 1){
			omp_set_num_threads(NUMT);
			double padStartTime =  omp_get_wtime();
			#pragma omp parallel for
				for(int i = 0; i < 4; i++){
					for(int j = 0; j < someBigNumber; j++){
						Array[i].value += 2.;// cos((double)i);
					}
				}
				double PaddedComplete = omp_get_wtime() - padStartTime;
				if(NUMT == 1){
					T1Pad = PaddedComplete;
				}
				double Paddedanswer = (((float)(someBigNumber*4)/(float)(PaddedComplete))/1000000.0);
				double speedup = T1Pad/PaddedComplete;
				cout << "Fix #1: Threads: " << NUMT << " Time: " << PaddedComplete << " MegaCalcs: " << Paddedanswer << " Speedup: " << speedup << endl;
				data << "Fix #1," << NUM << "," << NUMT << "," << PaddedComplete << "," << Paddedanswer << "," << speedup << endl;
		}
		else{
			//Fix #2
			omp_set_num_threads(NUMT);
			double StartTime =  omp_get_wtime();
			#pragma omp parallel for 
				for(int i = 0; i < 4; i++){
					float temp = Array[i].value;
					for(int j = 0; j < someBigNumber; j++){
						temp += 2.;// cos((double)i);
					}
					Array[i].value = temp;
				}
				double unPadComplete = omp_get_wtime() - StartTime;
				if(NUMT == 1){
					T1UnPad = unPadComplete;
				}
				double answer = (((float)(someBigNumber*4)/(float)(unPadComplete))/1000000.0);
				double speedup = T1UnPad/unPadComplete;
				cout << "Fix #2: Threads: " << NUMT << " Time: " << (unPadComplete) << " MegaCalcs: " << answer << " Speedup: " << speedup << endl;
				data << "Fix #2," << NUM << "," << NUMT << "," << unPadComplete << "," << answer << "," << speedup << endl;
		}
	}
	data.close();
	return 0;
}