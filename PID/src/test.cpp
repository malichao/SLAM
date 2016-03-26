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
	//float kP=4.8;
	//float kI=0;
	//float kD=25;
	vector<float> speed;
	vector<float> distance;
	float target=100;
	size_t simulationTime=200;

	Car simpleCar(1,0.1,0.01,0.1);//Mass(1),Friction(0.1),Resistance(0.01),Period(0.1)
	simpleCar.setSystemLag(2);
	simpleCar.setNoise(0.4,0.7,0.1);	//Force noise,distance and speed reading noise
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
	simpleCar.resetOrigin();
	float targetQue[]={100,50,120,40};
	size_t queSize=sizeof(targetQue)/sizeof(targetQue[0]);
	cout<<"Target number = "<<queSize<<endl;
	cout<<"PID simulation:\n";
	cout<<"time\tForce\tVelocity\tDistance\n";
	for(size_t j=0;j<queSize;j++){
		target=targetQue[j];
		cout<<"\nTarget position = "<<target<<endl;
		for(size_t i=0;i<simulationTime/queSize;i++){
			//cout<<update(simpleCar,force)<<endl;
			pid.update(simpleCar,target);
			printf("%d\t%.2f\t%.2f\t\t%.2f\n",
					i,simpleCar.getForce(),simpleCar.getVelocity(),simpleCar.getDistance());
			speed.push_back(simpleCar.getVelocity());
			distance.push_back(simpleCar.getDistance());
		}
	}

	///////// Save the result to result.csv for visualizing the data ////////
	ofstream file("result.csv");
	for(size_t i=0;i<simulationTime;i++)
		file<<speed[i]<<","<<distance[i]<<endl;
	file.close();

}
