/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 25,2016
version		: v0.2
Description :
	PID controller program for controlling the car

	-v0.2 : Add twiddle() to self optimize the PID coefficient
	-v0.1 : Basic PID controller
*****************************************************************************/

#ifndef PID_H_
#define PID_H_

class PID{
public:
	PID(const float p,const float i,const float d) :
					kP(p), kI(i), kD(d), output(0) {}
	void setPID(float p,const float i,const float d){
		kP=p;
		kI=i;
		kD=d;
	}
	float getP() const { return kP; }
	float getI() const { return kI; }
	float getD() const { return kD; }

	void update(Car &car,const float target);

	float getOutput() const { return output; }

	//Simply simulate the car motion using PID control and return the average error.
	//If you want to record the speed and distance of the car at every time step,
	//use update() and manually record the speed and distance.
	float simulate(Car &car,const float target,const std::size_t simulationTime);

	//PID parameter self optimizing function,parameter description:
	//car	: this program use a one-dimensional car as trail example
	//target: the target position of the car
	//tolerance: the minimal steps to probe the PID parameters
	//simulationTimes: the duration to try out each PID setting
	bool twiddle(	Car &car,
					const float target,
					const float tolerance,
					const std::size_t simulationTimes);
private:
	float kP;
	float kI;
	float kD;
	float output;

	//Constrain the output of each term
	void constrain(float &val, const float min, const float max) const{
		if (val < min) val = min;
		if (val > max) val = max;
	}
};

#endif /* PID_H_ */
