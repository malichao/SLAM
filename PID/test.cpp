/*
 * test.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: Lichao
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include "car.h"
#include "pid.h"
using namespace std;

int main(){
	float kP=1;
	float kI=0;
	float kD=3;
	//float kP=19.188;
	//float kI=0;
	//float kD=58.473;
	vector<float> speed;
	vector<float> distance;
	float target=100;
	size_t simulationTime=200;

	Car simpleCar(1,0.1,0.01,0.1);//Mass(1),Friction(0.1),Resistance(0.01),Period(0.1)
	simpleCar.setSystemLag(2);
	PID pid(kP,kI,kD);

	///////////////Testing Twiddle////////////////////
	//float twiddleTolerance=0.5;
	//pid.twiddle(simpleCar,target,twiddleTolerance,simulationTime);

	//cout<<"Input kP kI kD and file number\n";
	//string number;
	//cin>>kP>>kI>>kD>>number;

	cout<<"PID simulation:\n";
	cout<<"time\tVelocity\tDistance\n";
	simpleCar.resetOrigin();
	for(size_t i=0;i<simulationTime;i++){
		//cout<<update(simpleCar,force)<<endl;
		pid.update(simpleCar,target);
		printf("%d\t%.3f\t\t%.3f\n",i,simpleCar.getVelocity(),simpleCar.getDistance());
		speed.push_back(simpleCar.getVelocity());
		distance.push_back(simpleCar.getDistance());
	}

	ofstream file("result.csv");
	for(size_t i=0;i<simulationTime;i++)
		file<<speed[i]<<","<<distance[i]<<endl;
	file.close();

}
