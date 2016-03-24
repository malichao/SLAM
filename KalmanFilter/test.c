#include "KalmanFilter.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
	KFParameter kfp={	.dt=0.5,         		//Sampling time
						.R=2,          			//Measurement noise
		                .Q={0.0001,0.0001},   	//Process noise,usually very small
		                .H={1,0},         		//Measurement matrix,set to {1,0}
		                .P={{0,0},{0,0}}, 		//Error covariance,init to 0
		                .X={0,0}};        		//System state,init to 0
*/
int main(){
	float accAngle,lastAccAngle,gyroRate,fuseAngle;
	float simTime=0;
	KFParameter kfp;
	KFParameter *p=&kfp;

	gyroRate=0;
	lastAccAngle=sin(0*3.14/180)*10.0;
	initKalmanFilter(p,0.5,0.001,2);
	setKFCoef(p,0.01);		//usually between 0~1
	setKFMeasureNoise(p,2);	//Noise of the accelerometer
	srand(time(NULL));		//Intializes random number generator

	for(float i=0;i<1000;i++){
		simTime+=p->dt;
		accAngle=sin(simTime*3.14/180)*10.0;
		gyroRate=accAngle-lastAccAngle+0.05;//This gyro has a constant driff of 0.05
		gyroRate+=rand()%8/100.0;			//Add 10% white noise 0~0.08
		lastAccAngle=accAngle;
		accAngle+=rand()%200/100.0;			//Add some white noise 0~1
		if(rand()%100==0)					//Add 1% of spike,-10~10
			accAngle+=(rand()%200-100)/10.0;
		fuseAngle=kalmanFilter(p,accAngle, gyroRate);
		//fuseAngle=kalmanUpdate(gyroRate,accAngle);
		//fuseAngle=kalmanFilter2(accAngle, gyroRate);
		printf("%.3f,%.3f,%.3f\n",accAngle,gyroRate,fuseAngle);
	}
}
