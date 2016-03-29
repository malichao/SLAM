/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 25,2016
Version		: v0.2
Description :
	PID controller program for controlling the car

	-v0.2 : Add twiddle() to self optimize the PID coefficient
	-v0.1 : Basic PID controller
*****************************************************************************/
#include <math.h>
#include <numeric>      // std::accumulate
#include "car.h"
#include "pid.h"

using namespace std;

const double PID::EpsilonTime=1E-5;
const double PID::EpsilonError=1E-2;

double PID::calculate(const double target,const double input) {
	double error, dError;

	error = target - input;
	dError = (error - LastError) / Period;

	Integral += error > EpsilonError ? error * Period : 0;

	LastError = error;

	double output;
	output = Kp * error + Ki * Integral + Kd * dError;
	constrain(output, -OutputMax, OutputMax);
	Output = output;
	//car.update(output);
	return output;
}


//Simply simulate the car motion using PID control and return the average error.
//If you want to record the speed and distance of the car at every time step,
//use update() and manually record the speed and distance.
double PID::simulate(Car &car, const double target, const size_t simulationTimes) {
	double error = 0;
	if (simulationTimes == 0)
		return 0;

	car.resetOrigin();
	for (size_t i = 0; i < simulationTimes; i++) {
		double output=calculate(target,car.getDistance());
		car.update(output);
		error += fabs(target - car.getDistance());
	}
	return error / simulationTimes;
}

//PID parameter self optimizing function,parameter description:
//car	: this program use a one-dimensional car as trail example
//target: the target position of the car
//tolerance: the minimal steps to probe the PID parameters
//simulationTimes: the duration to try out each PID setting
bool PID::twiddle(	Car &car,
					const double target,
					const double tolerance,
					const unsigned int simulationTimes){

	double pidBackup[3] = { Kp, Ki, Kd };
	double pid[3] = { Kp, Ki, Kd };		//put it in array to ease the iteration
	double deltaPID[3] = { 10, 10, 10 };	//The steps it takes to probe the PID values
	double bestError = simulate(car, target, simulationTimes);

	//Set maximum trail times to 200 to prevent dead loop
	unsigned int watchDog = 500;
	bool success = true;

	//Tolerance is the minimal steps to probe the PID coefficient
	while (accumulate(deltaPID, deltaPID + 3, 0) > tolerance) {
		double error = 0;
		for (size_t i = 0; i < 3; i++) {	//Probing P,I,D one by one

			if (--watchDog == 0) {
				success = false;
				break;
			}
			pid[i] += deltaPID[i];			//First,try increasing the PID
			setPIDCoef(pid[0], pid[1], pid[2]);
			resetPIDCache();
			error = simulate(car, target, simulationTimes);

			//After simulation,we evaluate the performance of the setting
			if (error < bestError) {		//Quite good,increment the step size
				bestError = error;
				deltaPID[i] *= 1.1;
			} else {
				pid[i] -= deltaPID[i] * 2;	//Second,try decreasing the PID
				setPIDCoef(pid[0], pid[1], pid[2]);
				resetPIDCache();
				error = simulate(car, target, simulationTimes);

				//After simulation,we evaluate the performance of the setting
				if (error < bestError) {
					bestError = error;
					deltaPID[i] *= 1.1;		//Quite good,increment the step size
				} else {
					pid[i] += deltaPID[i];
					deltaPID[i] *= 0.9;		//Bad move,decrease the step size
				}
			}
		}
	}

	//If optimization succeeded,update the pid coefficient,otherwise restore the values
	if (success) {
		setPIDCoef(pid[0], pid[1], pid[2]);
		resetPIDCache();
		return true;
	} else {
		setPIDCoef(pidBackup[0], pidBackup[1], pidBackup[2]);
		resetPIDCache();
		return false;
	}
	return false;
}

