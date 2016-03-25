/*
 * pid.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: Lichao
 */
#include <numeric>      // std::accumulate
#include "car.h"
#include "pid.h"
#include "math.h"
#include <iostream>
#include "stdio.h"
using namespace std;

void PID::update(Car &car,const float target){
	float error,dError;
	static float lastError=0;
	static float integral=0;

	error=target-car.getDistance();
	dError=error-lastError;
	integral+=error;
	constrain(integral,-100,100);
	lastError=error;

	float output;
	output=kP*error+kI*integral+kD*dError;
	constrain(output,-100,100);
	car.update(output);
}


void PID::constrain(float &val,const float min,const float max){
	if(val<min)
		val=min;
	if(val>max)
		val=max;
}

float PID::simulate(Car &car,const float target,const size_t simulationTime){
	float error=0;
	if(simulationTime==0)
		return 0;

	car.resetOrigin();
	for(size_t i=0;i<simulationTime;i++){
		update(car,target);
		error+=fabs(target-car.getDistance());
	}
	return error/simulationTime;
}

void PID::twiddle(	Car &car,
					const float target,
					const float tolerance,
					const unsigned int simulationTime){

	float pidBackup[3]={kP,kI,kD};
	float pid[3]={kP,kI,kD};
	float deltaPID[3]={1,1,1};
	float bestError=simulate(car,target,simulationTime);

	unsigned int watchDog=1000;	//Prevent dead loop
	bool success=true;
	while(accumulate(deltaPID,deltaPID+3,0)>tolerance){
		if(--watchDog==0){
			cout<<"twiddle() runtime exceeded!\n"<<endl;
			success=false;
			break;
		}

		float error=0;
		for(size_t i=0;i<3;i++){
			pid[i]+=deltaPID[i];
			setPID(pid[0],pid[1],pid[2]);
			error=simulate(car,target,simulationTime);
			if(error<bestError){
				bestError=error;
				deltaPID[i]*=1.1;
			}else{
				pid[i]-=deltaPID[i]*2;
				setPID(pid[0],pid[1],pid[2]);
				error=simulate(car,target,simulationTime);
				if(error<bestError){
					bestError=error;
					deltaPID[i]*=1.1;
				}else{
					pid[i]+=deltaPID[i];
					deltaPID[i]*=0.9;
				}
			}
		}
	}
	if(success){
		cout<<"twiddle success\n";
		printf("Optimal PID setting:\nP%.3f\tI%.3f\tD%.3f\n",pid[0],pid[1],pid[2]);
		setPID(pid[0],pid[1],pid[2]);
	}
	else{
		setPID(pidBackup[0],pidBackup[1],pidBackup[2]);
	}


}














