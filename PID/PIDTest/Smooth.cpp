/*
 * Smooth.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */
#include <vector>
#include <iostream>
#include "Point.h"
using namespace std;
//using namespace pnt;//Point operation
/*
class Smooth{
private:
	vector<Point> SmoothRoute;
	float Tolerance=0.01;
	float WeightData=0.5;
	float WeightSmooth=0.1;
public:
	void smooth(const vector<Point> &route);
};

void Smooth::smooth(const vector<Point> &route){
	/*
	SmoothRoute=route;
	float change=Tolerance;
	int test=0;
	while(change>=Tolerance){
		change=0;
		for(size_t i=1;i<route.size()-1;i++){
			if(test++>1000)
				break;
			Point temp(SmoothRoute[i]);
			SmoothRoute[i]=SmoothRoute[i]+(route[i]-SmoothRoute[i])*WeightData;
			SmoothRoute[i]=SmoothRoute[i]+(SmoothRoute[i]*2-SmoothRoute[i-1]-SmoothRoute[i+1])*WeightSmooth;
			change+=pnt::abs(temp,SmoothRoute[i]);
		}
		if(test++>1000)
						break;
	}
	cout<<"smooth failed\n";
	*/
//}
