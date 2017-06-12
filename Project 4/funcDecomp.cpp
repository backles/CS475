#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <omp.h>
#include <fstream>

using namespace std;

int		NowYear = 2016;		//2016 - 2021
int		NowMonth = 0;		//0 - 11
int		NowZombies = 0;
int		NowNumDeer = 1;	//number of deer in the current population
float	NowPrecip;			//inches of rain per month
float	NowTemp;			//temperature this month
float	NowHeight = 1;		//grain height in inches
//Time step variables
const float GRAIN_GROWS_PER_MONTH 	=		8.0;	//inches
const float ONE_DEER_EATS_PER_MONTH =		0.5;	//inches
const float AVG_PRECIP_PER_MONTH 	=		6.0;	//inches
const float AMP_PRECIP_PER_MONTH 	=		6.0;	//inches
const float RANDOM_PRECIP 			=		2.0;	//inches
const float AVG_TEMP 				=		50.0;	//Fahrenheit
const float AMP_TEMP 				=		20.0;	//Fahrenheit
const float RANDOM_TEMP 			=		10.0;	//Fahrenheit

int GrainDeer();
int Grain();
int Watcher();
int MyAgent();
double Ranf(double, double);

int main(){
	if(remove("data.csv" )!= 0){
		cout << "Error deleting file" << endl;
	}
	//Global Set
	//Calculate starting temp and precipitation
	float ang = (30.*(float)NowMonth + 15.)*(M_PI / 180.);
	float temp = AVG_TEMP - AMP_TEMP * cos(ang);
	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP);
	NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP);
	if(NowPrecip < 0.){
		NowPrecip = 0.;
	}
	do{
		//Split the sections
		omp_set_num_threads(4);
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				GrainDeer();
			}
			#pragma omp section
			{
				Grain();
			}
			#pragma omp section
			{
				Watcher();
			}
			#pragma omp section
			{
				MyAgent();
			}
		}// implied barrier -- all functions must return in order to allow any of them to get past here
	}while(NowYear < 2022);
	return 0;
}

int GrainDeer(){
	int tempDeers = NowNumDeer;
	if(tempDeers < NowHeight){
		tempDeers = NowNumDeer + 1;
	}
	else{
		tempDeers = NowNumDeer - 1;
		if(tempDeers < 1){
			tempDeers = 0;
		}
	}
	#pragma omp barrier	//Done Compute
		if(tempDeers < 1){
			tempDeers = 0;
		}
		NowNumDeer = tempDeers;
	#pragma omp barrier	//Done Assign
	#pragma omp barrier	//Done Printing
}

int Grain(){
	int numberOfDeer = NowNumDeer;
	double tempChanger = exp(-pow(((NowTemp - AVG_TEMP)/10),2));
	double precipChanger = exp(-pow(((NowPrecip - AVG_PRECIP_PER_MONTH)/10),2));
	double height = NowHeight;
	height += tempChanger * precipChanger * GRAIN_GROWS_PER_MONTH;
	height -= numberOfDeer * ONE_DEER_EATS_PER_MONTH;
	if(height < 0.){
		height = 0;
	}
	#pragma omp barrier	//Done Compute
		NowHeight = height;
	#pragma omp barrier	//Done Assign
	#pragma omp barrier	//Done Printing
}

int MyAgent(){
		#pragma omp barrier	//Done Compute
		//Zombie Attack and eats one deer
		int tempDeer = NowNumDeer;
		int tempZombies = NowZombies;
		int eaten = 0;
		if ((tempDeer/2) > NowZombies){
			tempZombies = tempZombies + 1;
		}else{
			tempZombies = tempZombies - 1;
		}
		eaten = tempZombies / 3;
		tempDeer = tempDeer - eaten;
		#pragma omp barrier	//Done Assign
			if(tempDeer < 1){
				tempDeer = 0;
			}
			if(tempZombies < 1){
				tempZombies = 0;
			}
			NowNumDeer = tempDeer;
			NowZombies = tempZombies;
		#pragma omp barrier	//Done Printing
}

int Watcher(){
	ofstream data;
	data.open("data.csv", ios::out | ios::app);
	#pragma omp barrier	//Done Compute
	#pragma omp barrier	//Done Assign
	cout<<"CurTemp: "<<NowTemp<<" CurPerc: "<<NowPrecip<<" NumDeer: "<<NowNumDeer<<" nowHeight: "<<NowHeight<<endl;
	data << NowMonth << "," << NowTemp << "," << NowPrecip << "," << NowNumDeer << "," << NowHeight << "," << NowZombies << endl;
		if(NowMonth >= 11){			//If its greater than December
			NowMonth = 0;			//Set to December
			NowYear += 1;
		}
		else{
			NowMonth += 1;
		}
		
		//Set Temperature
		float ang = (30.*(float)NowMonth + 15.) * (M_PI / 180.);
		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );
		NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
		if(NowPrecip < 0.){
			NowPrecip = 0.;
		}
	#pragma omp barrier	//Done Printing
	data.close();
}

double Ranf(double low, double high){
	double temp = (double)rand();
	return (low + temp * (high-low)/(double)RAND_MAX);
}