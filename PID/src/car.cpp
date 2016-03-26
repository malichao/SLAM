/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 19,2016
version		: v0.2
Description :
	A simplified car model to simulate car motion.Currently only moves in one
	dimension(horizontal).
	Simulation model includes the following parameters:
	Mass		: the mass of the car
	Friction	: always in the opposite direction of the car motion
	Resistance	: wind resistance,proportional to velocity square
	Velocity	: m/s
	Distance	: meter
	Direction	: Backward,Still,Forward
	Period		: simulation update period,e.g.,0.1s,0.5s,1s
	Lag			: simulate the delay of the system
*****************************************************************************/
#include <math.h>
#include "car.h"

using namespace std;

void Car::setNoise(const float noiseF,const float noiseD,const float noiseV){
	NoiseLevelForce=noiseF;
	NoiseLevelDistance=noiseD;
	NoiseLevelVelocity=noiseV;
	srand(time(NULL));
}

//Here's the current simulation process:
//1. Check the lag of the system
//2. The force applied on the car must be greater then the static friction to make
//	 the car move.
//3. If the car is moving,there will be wind resistance,which is proportional to
//	 the square of velocity.
void Car::update(const float f){
	float force=f+(rand() % 200-100)/100.0*NoiseLevelForce;	//Add some random noise to the input
	float dragForce;
	float forceDelta;
	float forceLimit= fabs(Velocity)<Epsilon ? MaxForce : MaxPowerOutput/Velocity;
	constrain(force,-forceLimit,forceLimit);
	constrain(force,-MaxForce,MaxForce);
	Force=force;
	/*
	 * First we deal with system lag.
	 * If there is no lag,ideally,then Lag=0,and we update the car every time.
	 * If there is lag,we put the force in the queue and simulate the lag
	 */
	if(Lag!=0&&ForceQue.size()==Lag){
		force=ForceQue.front();
		ForceQue.pop();
		ForceQue.push(f);
	}
	else{	//The lag time is not reached,we push the force in and don't update anything
		ForceQue.push(f);
		return;
	}

	//If the car is moving too slow,we consider it as stopped
	if(fabs(Velocity)<Epsilon){
		Direction=Still;
		if(force>0){
			forceDelta=force-Mass*Friction*10;
			forceDelta=forceDelta<0 ?0:forceDelta;	
		}else{
			forceDelta=force+Mass*Friction*10;
			forceDelta=forceDelta>0 ?0:forceDelta;	
		}
		Velocity+=forceDelta*Period;
		Distance+=Velocity*Period;
	}else if(Velocity>Epsilon){		//If the car is forward
		Direction=Forward;
		dragForce=Mass*Friction*10
					+Velocity*Velocity*Resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta= - dragForce;
			Velocity+=forceDelta*Period;
			Velocity=Velocity<0 ? 0 :Velocity;
		}else{
			forceDelta=force - dragForce;
			Velocity+=forceDelta*Period;
		}
	}else if(Velocity<-Epsilon){	//If the car is backward
		Direction=Backward;
		dragForce=Mass*Friction*10
					+Velocity*Velocity*Resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta=  dragForce;
			Velocity+=forceDelta*Period;
			Velocity=Velocity<0 ? 0 :Velocity;
		}else{
			forceDelta=force + dragForce;
			Velocity+=forceDelta*Period;
		}
	}
	constrain(Velocity,-MaxSpeed,MaxSpeed);
	Distance+=Velocity*Period;
}





