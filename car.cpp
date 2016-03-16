#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;
enum Direction{Backward=-1,Still=0,Forward=1};
const float Epsilon=1E-5;
struct Car{
	float mass;			//kg
	float friction;		//ratio
	float resistance;	//wind resistance
	float velocity;		//m/s
	float distance;		//m
	Direction direction;
};

void update(Car &car,const float force){
	float time=0.1;		//update time
	float dragForce;
	float forceDelta;
	if(fabs(car.velocity)<Epsilon){		//Considered as still
		car.direction=Still;
		if(force>0){
			forceDelta=force-car.mass*car.friction*10;
			forceDelta=forceDelta<0 ?0:forceDelta;	
		}else{
			forceDelta=force+car.mass*car.friction*10;
			forceDelta=forceDelta>0 ?0:forceDelta;	
		}
		car.velocity+=forceDelta*time;
		car.distance+=car.velocity*time;
	}else if(car.velocity>Epsilon){		//If the car is forward
		car.direction=Forward;
		dragForce=car.mass*car.friction*10
					+car.velocity*car.velocity*car.resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta= - dragForce;
			car.velocity+=forceDelta*time;
			car.velocity=car.velocity<0 ? 0 :car.velocity;
		}else{
			forceDelta=force - dragForce;
			car.velocity+=forceDelta*time;
		}
	}else if(car.velocity<-Epsilon){	//If the car is backward
		car.direction=Backward;
		dragForce=car.mass*car.friction*10
					+car.velocity*car.velocity*car.resistance;

		if(fabs(force)<Epsilon){	//If the car is not driven
			forceDelta=  dragForce;
			car.velocity+=forceDelta*time;
			car.velocity=car.velocity<0 ? 0 :car.velocity;
		}else{
			forceDelta=force + dragForce;
			car.velocity+=forceDelta*time;
		}
	}

	car.distance+=car.velocity*time;
}

void constrain(float &val,const float min,const float max){
	if(val<min)
		val=min;
	if(val>max)
		val=max;
}
void PID(Car &car,float target){
	float error,dError;
	static float lastError=0;
	static float integral=0;
	float kP=1;
	float kI=0;
	float kD=0;

	error=target-car.distance;
	dError=error-lastError;
	integral+=error;
	constrain(integral,-100,100);
	lastError=error;

	float output;
	output=kP*error+kI*integral+kD*dError;
	constrain(output,-20,20);
	update(car,output);
	cout<<output<<endl;
}

int main(){
	Car simpleCar={1,0.1,0.01,0,0,Still};
	float force=10;
	vector<float> speed;
	vector<float> distance;

	int simulationTime=100;
	for(int i=0;i<simulationTime;i++){
		//cout<<update(simpleCar,force)<<endl;
		PID(simpleCar,100);
		cout<<i<<"\t";
		speed.push_back(simpleCar.velocity);
		distance.push_back(simpleCar.distance);
	}

	ofstream file("result.csv");
	for(int i=0;i<simulationTime;i++)
		file<<speed[i]<<","<<distance[i]<<endl;
	file.close();
}