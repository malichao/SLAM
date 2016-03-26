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
#include <stdlib.h>
#include <time.h>
#include <math.h>

class Car{
public:
	enum Direction{Backward=-1,Still=0,Forward=1};

	Car(const float m, const float f, const float r, const float v,
			const float dis, Direction dir, const float p, std::size_t l) :
						Mass(m), Friction(f), Resistance(r), Velocity(v),
						Distance(dis), Direction(dir), Period(p), Lag(l),
						Force(0),MaxSpeed(30), MaxForce(3),MaxPowerOutput(150),
						NoiseLevelForce(0),NoiseLevelDistance(0), NoiseLevelVelocity(0) {}
	Car(const float m, const float f, const float r, const float p) :
						Mass(m), Friction(f), Resistance(r), Velocity(0),
						Distance(0),Direction(Still), Period(p), Lag(0),
						Force(0),MaxSpeed(30), MaxForce(3),MaxPowerOutput(150),
						NoiseLevelForce(0),NoiseLevelDistance(0), NoiseLevelVelocity(0) {}

	void update(const float force);
	void setNoise(const float noiseF,const float noiseD,const float noiseV);
	void setMaxPowerOutput(const float p) { MaxPowerOutput=fabs(p);}
	void setMaxSpeed(const float s) { MaxSpeed=fabs(s);}
	float getVelocity() const { return Velocity + (rand() % 200-100)/100.0*NoiseLevelVelocity;}
	float getDistance() const { return Distance + (rand() % 200-100)/100.0*NoiseLevelDistance;}
	float getForce()const { return Force;}
	void  setSystemLag(std::size_t l){ Lag=l; }
	void  resetOrigin(){
		Velocity=0;
		Distance=0;
		std::queue<float> q;
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
	std::size_t Lag;
	float Force;
	float MaxSpeed;
	float MaxForce;
	float MaxPowerOutput;
	float NoiseLevelForce;
	float NoiseLevelDistance;
	float NoiseLevelVelocity;
	std::queue<float> ForceQue;
	const static float Epsilon=1E-5;

	void constrain(float &val, const float min, const float max) const{
		if (val < min) val = min;
		if (val > max) val = max;
	}
};


#endif /* CAR_H_ */
