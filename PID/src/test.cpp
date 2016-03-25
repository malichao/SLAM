/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 25,2016
version		: v0.2
Description :
	Test file for testing the car model,PID controller,and PID self optimization
	algorithm.The simulated result are both printed on screen and saved to the
	log file for further use.
*****************************************************************************/

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

	//////// Testing the PID self optimization algorithm //////
	float twiddleTolerance=0.5;
	if(pid.twiddle(simpleCar,target,twiddleTolerance,simulationTime)){
		cout <<"twiddle success\n";
	}else{
		cout << "twiddle() runtime exceeded!\n";
	}
	printf("P %.3f\t\tI %.3f\t\tD %.3f\n",pid.getP(),pid.getI(),pid.getD());

	////////////// Simulate the car movement ////////////////
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

	///////// Save the result to result.csv for visualizing the data ////////
	ofstream file("result.csv");
	for(size_t i=0;i<simulationTime;i++)
		file<<speed[i]<<","<<distance[i]<<endl;
	file.close();

}
