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


void PID::update(Car &car, const float target) {
	float error, dError;
	static float lastError = 0;
	static float integral = 0;

	error = target - car.getDistance();
	dError = error - lastError;
	integral += error;
	constrain(integral, -100, 100);
	lastError = error;

	float output;
	output = kP * error + kI * integral + kD * dError;
	constrain(output, -100, 100);
	car.update(output);
}


//Simply simulate the car motion using PID control and return the average error.
//If you want to record the speed and distance of the car at every time step,
//use update() and manually record the speed and distance.
float PID::simulate(Car &car, const float target, const size_t simulationTimes) {
	float error = 0;
	if (simulationTimes == 0)
		return 0;

	car.resetOrigin();
	for (size_t i = 0; i < simulationTimes; i++) {
		update(car, target);
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
					const float target,
					const float tolerance,
					const unsigned int simulationTimes){

	float pidBackup[3] = { kP, kI, kD };
	float pid[3] = { kP, kI, kD };		//put it in array to ease the iteration
	float deltaPID[3] = { 1, 1, 1 };	//The steps it takes to probe the PID values
	float bestError = simulate(car, target, simulationTimes);

	//Set maximum trail times to 200 to prevent dead loop
	unsigned int watchDog = 200;
	bool success = true;

	//Tolerance is the minimal steps to probe the PID coefficient
	while (accumulate(deltaPID, deltaPID + 3, 0) > tolerance) {
		float error = 0;
		for (size_t i = 0; i < 3; i++) {	//Probing P,I,D one by one

			if (--watchDog == 0) {
				success = false;
				break;
			}
			pid[i] += deltaPID[i];			//First,try increasing the PID
			setPID(pid[0], pid[1], pid[2]);
			error = simulate(car, target, simulationTimes);

			//After simulation,we evaluate the performance of the setting
			if (error < bestError) {		//Quite good,increment the step size
				bestError = error;
				deltaPID[i] *= 1.1;
			} else {
				pid[i] -= deltaPID[i] * 2;	//Second,try decreasing the PID
				setPID(pid[0], pid[1], pid[2]);
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
		setPID(pid[0], pid[1], pid[2]);
		return true;
	} else {
		setPID(pidBackup[0], pidBackup[1], pidBackup[2]);
		return false;
	}
	return false;
}

