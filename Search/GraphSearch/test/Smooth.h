/*
 * Smooth.h
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */

#ifndef SMOOTH_H_
#define SMOOTH_H_

#include <vector>
#include <iostream>
#include "../src/Point.h"
#include "Smooth.h"
using namespace std;
using namespace pnt;//Point operation

class Smooth{
private:
	float WeightData=0.5;
	float WeightSmooth=0.46;
	float Tolerance=0.01;
	size_t Scale=1;
public:
	Smooth(){}
	Smooth(float wD,float wS,float t,float s):
		WeightData(wD),
		WeightSmooth(wS),
		Tolerance(t),
		Scale(s){}
	void set(float wD,float wS,float t,float s);
	void smooth(vector<Point<float> > &route);
	void smooth(vector<Point<float> > &route,const size_t s);
};



#endif /* SMOOTH_H_ */
