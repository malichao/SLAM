/*
 * pid.h
 *
 *  Created on: Mar 25, 2016
 *      Author: Lichao
 */

#ifndef PID_H_
#define PID_H_

class PID{
public:
	PID(float p, float i, float d) : kP(p), kI(i), kD(d), output(0) {}
	void setPID(float p,float i,float d){
		kP=p;
		kI=i;
		kD=d;
	}
	float getP(){return kP;}
	float getI(){return kI;}
	float getD(){return kD;}

	void update(Car &car,const float target);

	float getOutput(){ return output; }

	//Simply simulate the car motion using PID control and return the average error.
	//If you want to record the speed and distance of the car at every time step,
	//use update() and manually record the speed and distance.
	float simulate(Car &car,const float target,const size_t simulationTime);

	//PID parameter self optimizing function,parameter description:
	//car	: this program use a one-dimensional car as trail example
	//target: the target position of the car
	//tolerance: the minimal steps to probe the PID parameters
	//simulationTimes: the duration to try out each PID setting
	bool twiddle(	Car &car,
					const float target,
					const float tolerance,
					const unsigned int simulationTimes);
private:
	float kP;
	float kI;
	float kD;
	float output;

	//Constrain the output of each term
	void constrain(float &val,const float min,const float max);
};

#endif /* PID_H_ */
