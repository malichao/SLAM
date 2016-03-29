/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 19,2016
version		: v0.3
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

	-v0.3 : Add constrain--Power=Force*Speed
	-v0.2 : Add system lag
	-v0.1 : Simulate 1-dimensional car movement
*****************************************************************************/
#include <math.h>
#include "car.h"

const double Car::Epsilon=1E-5;
using namespace std;

void Car::setNoise(const double noiseF,const double noiseD,const double noiseV){
	NoiseLevelForce=noiseF;
	NoiseLevelDistance=noiseD;
	NoiseLevelVelocity=noiseV;
	srand(time(NULL));
}

// Paramters are based on Tesla Model S
void Car::setToStandardCar(){
	Mass=2000;		// Kg
	Friction=0.05;	// Dry concrete road
	Resistance=0.05;	// Neglect the wind resistance
	MaxSpeed= 58;	// 130mph,208 km/h
	MaxForce= 9793;	//
	MaxPowerOutput=568000;

	Period=0.1;		// /s
	resetOrigin();
	setSystemLag(1);
	setNoise(0.4,0.7,0.1);	//Force noise,distance and speed reading noise
}

//Here's the current simulation process:
//1. Check the lag of the system
//2. The force applied on the car must be greater then the static friction to make
//	 the car move.
//3. If the car is moving,there will be wind resistance,which is proportional to
//	 the square of velocity.
void Car::update(const double f){
	double force=f+(rand() % 200-100)/100.0*NoiseLevelForce;	//Add some random noise to the input
	double dragForce;
	double forceDelta;
	double forceLimit= fabs(Velocity)<Epsilon ? MaxForce : MaxPowerOutput/Velocity;
	constrain(force,-forceLimit,forceLimit);
	constrain(force,-MaxForce,MaxForce);
	Force=force;
	/*
	 * First we deal with system lag.
	 * If there is no lag,ideally,then Lag=0,and we update the car every time.
	 * If there is lag,we put the force in the queue and simulate the lag
	 */
	if(Lag==0)
		force=f;
	else if(ForceQue.size()==Lag){
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
		Velocity+=forceDelta/Mass*Period;
		//Distance+=Velocity*Period;
	}else if(Velocity>Epsilon){		//If the car is forward
		Direction=Forward;
		dragForce=Mass*Friction*10
					+Velocity*Velocity*Resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta= - dragForce;
			Velocity+=forceDelta/Mass*Period;
			Velocity=Velocity<0 ? 0 :Velocity;
		}else{
			forceDelta=force - dragForce;
			Velocity+=forceDelta/Mass*Period;
		}
	}else if(Velocity<-Epsilon){	//If the car is backward
		Direction=Backward;
		dragForce=Mass*Friction*10
					+Velocity*Velocity*Resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta=  dragForce;
			Velocity+=forceDelta/Mass*Period;
			Velocity=Velocity<0 ? 0 :Velocity;
		}else{
			forceDelta=force + dragForce;
			Velocity+=forceDelta/Mass*Period;
		}
	}
	constrain(Velocity,-MaxSpeed,MaxSpeed);
	Distance+=Velocity*Period;
}





