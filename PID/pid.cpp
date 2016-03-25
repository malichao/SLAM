/*
 * pid.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: Lichao
 */
#include "car.h"
#include "pid.h"


void PID::update(Car &car,float target){
	float error,dError;
	static float lastError=0;
	static float integral=0;

	error=target-car.getDistance();
	dError=error-lastError;
	integral+=error*dIt;
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
