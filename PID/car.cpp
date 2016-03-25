#include "math.h"
#include "car.h"

using namespace std;

void Car::update(const float force){
	float time=0.1;		//update time
	float dragForce;
	float forceDelta;
	if(fabs(velocity)<Epsilon){		//Considered as still
		direction=Still;
		if(force>0){
			forceDelta=force-mass*friction*10;
			forceDelta=forceDelta<0 ?0:forceDelta;	
		}else{
			forceDelta=force+mass*friction*10;
			forceDelta=forceDelta>0 ?0:forceDelta;	
		}
		velocity+=forceDelta*time;
		distance+=velocity*time;
	}else if(velocity>Epsilon){		//If the car is forward
		direction=Forward;
		dragForce=mass*friction*10
					+velocity*velocity*resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta= - dragForce;
			velocity+=forceDelta*time;
			velocity=velocity<0 ? 0 :velocity;
		}else{
			forceDelta=force - dragForce;
			velocity+=forceDelta*time;
		}
	}else if(velocity<-Epsilon){	//If the car is backward
		direction=Backward;
		dragForce=mass*friction*10
					+velocity*velocity*resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta=  dragForce;
			velocity+=forceDelta*time;
			velocity=velocity<0 ? 0 :velocity;
		}else{
			forceDelta=force + dragForce;
			velocity+=forceDelta*time;
		}
	}

	distance+=velocity*time;
}





