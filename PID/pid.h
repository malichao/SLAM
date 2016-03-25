/*
 * pid.h
 *
 *  Created on: Mar 25, 2016
 *      Author: Lichao
 */

#ifndef PID_H_
#define PID_H_

class PID{
public:
	void update(Car &car,float target);
	PID(float p,float i,float d):kP(p),kI(i),kD(d),output(0){}
	void setPID(float p,float i,float d){
		kP=p;
		kI=i;
		kD=d;
	}
	float getOutput(){return output;}
private:
	float kP;
	float kI;
	float kD;
	float output;
	void constrain(float &val,const float min,const float max);
};



#endif /* PID_H_ */
