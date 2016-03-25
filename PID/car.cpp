#include "math.h"
#include "car.h"

using namespace std;

void Car::update(const float f){
	float force=f;
	float dragForce;
	float forceDelta;

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

	if(fabs(Velocity)<Epsilon){		//Considered as still
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

	Distance+=Velocity*Period;
}





