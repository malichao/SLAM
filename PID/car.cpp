#include "math.h"
#include "car.h"

using namespace std;

void Car::update(const float force){


	float dragForce;
	float forceDelta;
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





