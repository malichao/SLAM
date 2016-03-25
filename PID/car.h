/*
 * car.h
 *
 *  Created on: Mar 25, 2016
 *      Author: Lichao
 */

#ifndef CAR_H_
#define CAR_H_

#include <queue>
using namespace std;
class Car{
public:
	enum Direction{Backward=-1,Still=0,Forward=1};
	void update(const float force);
	//Car simpleCar={1,0.1,0.01,0,0,Still};
	Car(float m,float f,float r,float v,float dis,Direction dir,float p,size_t l):
		Mass(m),Friction(f),Resistance(r),Velocity(v),Distance(dis),Direction(dir),Period(p),Lag(l){}
	Car(float m,float f,float r,float p):
			Mass(m),Friction(f),Resistance(r),Velocity(0),Distance(0),Direction(Still),Period(p),Lag(0){}
	float getVelocity(){return Velocity;}
	float getDistance(){return Distance;}
	void resetOrigin(){
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
