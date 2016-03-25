/*
 * car.h
 *
 *  Created on: Mar 25, 2016
 *      Author: Lichao
 */

#ifndef CAR_H_
#define CAR_H_

using namespace std;
class Car{
public:
	enum Direction{Backward=-1,Still=0,Forward=1};
	void update(const float force);
	//Car simpleCar={1,0.1,0.01,0,0,Still};
	Car(float m,float f,float r,float v,float dis,Direction dir):
		mass(m),friction(f),resistance(r),velocity(v),distance(dis),direction(dir){}
	float getVelocity(){return velocity;}
	float getDistance(){return distance;}
private:
	float mass;			//kg
	float friction;		//ratio
	float resistance;	//wind resistance
	float velocity;		//m/s
	float distance;		//m
	Direction direction;
	const static float Epsilon=1E-5;
};


#endif /* CAR_H_ */
