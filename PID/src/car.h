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

#ifndef CAR_H_
#define CAR_H_

#include <queue>
using namespace std;
class Car{
public:
	enum Direction{Backward=-1,Still=0,Forward=1};

	Car(float m,float f,float r,float v,float dis,Direction dir,float p,size_t l):
		Mass(m),Friction(f),Resistance(r),Velocity(v),Distance(dis),Direction(dir),Period(p),Lag(l){}
	Car(float m,float f,float r,float p):
			Mass(m),Friction(f),Resistance(r),Velocity(0),Distance(0),Direction(Still),Period(p),Lag(0){}

	void  update(const float force);
	float getVelocity(){return Velocity;}
	float getDistance(){return Distance;}
	void  setSystemLag(size_t l){Lag=l;}
	void  resetOrigin(){
		Velocity=0;
		Distance=0;
		queue<float> q;
		std::swap(q,ForceQue);//Clear the force queue
	}
private:
	float Mass;			//kg
	float Friction;		//ratio
	float Resistance;	//wind resistance
	float Velocity;		//m/s
	float Distance;		//m
	Direction Direction;
	float Period;		//update time
	size_t Lag;
	queue<float> ForceQue;
	const static float Epsilon=1E-5;
};


#endif /* CAR_H_ */
