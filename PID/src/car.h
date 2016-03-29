/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 19,2016
Version		: v0.3
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

#ifndef CAR_H_
#define CAR_H_

#include <queue>
#include <stdlib.h>
#include <time.h>
#include <math.h>

class Car{
public:
	enum Direction{Backward=-1,Still=0,Forward=1};
	const static double Epsilon;

	Car() :	Mass(0), Friction(0), Resistance(0), Velocity(0),
			Distance(0), Direction(Still), Period(1), Lag(0),
			Force(0),MaxSpeed(30), MaxForce(3),MaxPowerOutput(150),
			NoiseLevelForce(0),NoiseLevelDistance(0), NoiseLevelVelocity(0) {}
	Car(const double m, const double f, const double r, const double v,
			const double dis, Direction dir, const double p, std::size_t l) :
						Mass(m), Friction(f), Resistance(r), Velocity(v),
						Distance(dis), Direction(dir), Period(p), Lag(l),
						Force(0),MaxSpeed(30), MaxForce(3),MaxPowerOutput(150),
						NoiseLevelForce(0),NoiseLevelDistance(0), NoiseLevelVelocity(0) {}
	Car(const double m, const double f, const double r, const double p) :
						Mass(m), Friction(f), Resistance(r), Velocity(0),
						Distance(0),Direction(Still), Period(p), Lag(0),
						Force(0),MaxSpeed(30), MaxForce(3),MaxPowerOutput(150),
						NoiseLevelForce(0),NoiseLevelDistance(0), NoiseLevelVelocity(0) {}
	virtual ~Car(){}

	virtual void update(const double force);

	void setNoise(const double noiseF,const double noiseD,const double noiseV);
	void setMaxPowerOutput(const double p) { MaxPowerOutput=fabs(p);}
	void setMaxSpeed(const double s) { MaxSpeed=fabs(s);}
	void setSystemLag(std::size_t l){ Lag=l; }
	void setToStandardCar();
	void resetOrigin(){
		Velocity=0;
		Distance=0;
		std::queue<double> q;
		std::swap(q,ForceQue);//Clear the force queue
	}
	double getVelocity() const { return Velocity + (rand() % 200-100)/100.0*NoiseLevelVelocity;}
	double getDistance() const { return Distance + (rand() % 200-100)/100.0*NoiseLevelDistance;}
	double getForce()const { return Force;}
	double getMass() const {return Mass;}
	double getPeriod() const {return Period;}

private:
	double Mass;			//kg
	double Friction;		//ratio
	double Resistance;	//wind resistance
	double Velocity;		//m/s
	double Distance;		//m
	Direction Direction;
	double Period;		//update time
	std::size_t Lag;
	double Force;
	double MaxSpeed;
	double MaxForce;
	double MaxPowerOutput;
	double NoiseLevelForce;
	double NoiseLevelDistance;
	double NoiseLevelVelocity;
	std::queue<double> ForceQue;

	void constrain(double &val, const double min, const double max) const{
		if (val < min) val = min;
		if (val > max) val = max;
	}
};


#endif /* CAR_H_ */
