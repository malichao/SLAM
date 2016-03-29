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
	const static double EpsilonTime;
	const static double EpsilonError;

	PID(const double p,const double i,const double d,double t) :
					Kp(p), Ki(i), Kd(d),Output(0),OutputMax(10000),
					LastError(0), Integral(0),Period(t) {}
	void setPIDCoef(double p,const double i,const double d){
		Kp=p;
		Ki=i;
		Kd=d;
	}
	void setPeriod(const double t){ Period=t<EpsilonTime ? EpsilonTime : t;}
	double getP() const { return Kp; }
	double getI() const { return Ki; }
	double getD() const { return Kd; }

	double calculate(const double target,const double input);

	void resetPIDCache(){
		Output=0;
		LastError=0;
		Integral=0;
	}

	double getOutput() const { return Output; }

	//Simply simulate the car motion using PID control and return the average error.
	//If you want to record the speed and distance of the car at every time step,
	//use update() and manually record the speed and distance.
	double simulate(Car &car,const double target,const std::size_t simulationTime);

	//PID parameter self optimizing function,parameter description:
	//car	: this program use a one-dimensional car as trail example
	//target: the target position of the car
	//tolerance: the minimal steps to probe the PID parameters
	//simulationTimes: the duration to try out each PID setting
	bool twiddle(	Car &car,
					const double target,
					const double tolerance,
					const std::size_t simulationTimes);
private:
	double Kp;
	double Ki;
	double Kd;
	double Output;
	double OutputMax;
	double LastError;
	double Integral;
	double Period;

	//Constrain the output of each term
	void constrain(double &val, const double min, const double max) const{
		if (val < min) val = min;
		if (val > max) val = max;
	}
};

#endif /* PID_H_ */
