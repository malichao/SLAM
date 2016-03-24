#include "KalmanFilter.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	float accAngle,gyroRate,fuseAngle;
	KFParameter kfp={	.dt=0.5,         		//Sampling time
		                .Q={0.0001,0.0001},   	//Process noise,usually very small
		                .R=2,          			//Measurement noise
		                .H={1,0},         		//Measurement matrix,set to {1,0}
		                .P={{0,0},{0,0}}, 		//Error covariance,init to 0
		                .X={0,0}};        		//System state,init to 0
	KFParameter *p=&kfp;
	//float kalmanY = kalmanFilter(p,AccAngle, gyroRate);
	gyroRate=0;
	float lastAccAngle=sin(0*3.14/180)*10.0;

	srand(time(NULL));		// Intializes random number generator

	for(float i=0;i<500;i+=p->dt){
		accAngle=sin(i*3.14/180)*10.0;
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
