/*
 * Smooth.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */
#include <vector>
#include <iostream>
#include "Smooth.h"
#include <stdio.h>
using namespace std;
using namespace pnt;//Point operation

void Smooth::set(float wD,float wS,float t,float s){
	WeightData=wD;
	WeightSmooth=wS;
	Tolerance=t;
	Scale=s;
}

void Smooth::smooth(vector<Point<float> > &route,const size_t s){
	Scale=s;
	smooth(route);
}

void Smooth::smooth(vector<Point<float> > &route){

	vector<Point<float> > oldRoute;
	for(size_t i=0;i<route.size();i++){
		for(size_t j=0;j<Scale;j++){
			oldRoute.push_back(route[i]);
			printf("(%.1f,%.1f)\n",oldRoute[i].x,oldRoute[i].y);
		}
	}
	if(route.size()!=oldRoute.size()){
		route.resize(oldRoute.size());
		route=oldRoute;
		cout<<"expand\n";
	}
	float change=Tolerance;
	int test=0;
	while(change>=Tolerance){
		change=0;
		for(size_t i=1;i<route.size()-1;i++){
			float deltaX,deltaY;

			deltaX=(oldRoute[i].x-route[i].x)*WeightData;
			deltaY=(oldRoute[i].y-route[i].y)*WeightData;
			route[i].set(route[i].x+deltaX,route[i].y+deltaY,route[i].dir);
			change+=deltaX*deltaX+deltaY*deltaY;

			deltaX=(route[i-1].x+route[i+1].x-route[i].x*2)*WeightSmooth;
			deltaY=(route[i-1].y+route[i+1].y-route[i].y*2)*WeightSmooth;
			route[i].set(route[i].x+deltaX,route[i].y+deltaY,route[i].dir);
			change+=deltaX*deltaX+deltaY*deltaY;

			//route[i]=route[i]+(oldRoute[i]-route[i])*WeightData;
			//route[i]=route[i]+(route[i]*2-route[i-1]-route[i+1])*WeightSmooth;
			//change+=pnt::abs(temp,route[i]);
		}
		if(test++>100)
			break;
	}
}
