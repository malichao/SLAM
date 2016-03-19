/*
 * Smooth.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */
#include <vector>
#include "Point.h"
using namespace std;
using namespace pnt;//Point operation

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
	SmoothRoute=route;
	float change=Tolerance;
	while(change>=Tolerance){
		for(size_t i=1;i<route.size()-1;i++){
			SmoothRoute[i]=pnt
		}
	}
}
