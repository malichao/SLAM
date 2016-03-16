#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
struct Car{
	float mass;			//kg
	float friction;		//ratio
	float resistance;	//wind resistance
	float velocity;		//m/s
	float distance;		//m
};

float update(Car &car,const float force){
	float time=0.1;		//update time
	float forceDelta=force-car.mass*car.friction*9.8;
	forceDelta=forceDelta<0 ?0:forceDelta;			
	car.velocity+=forceDelta*time;
	car.distance+=car.velocity*time;
	return car.distance;
}

int main(){
	Car simpleCar={1,0.1,0,0};
	float force=15;
	vector<float> result;
	for(int i=0;i<100;i++){
		//cout<<update(simpleCar,force)<<endl;
		result.push_back(update(simpleCar,force));
	}

	ofstream file("result.txt");
	for(auto x : result)
		file<<x<<endl;
	file.close();
}