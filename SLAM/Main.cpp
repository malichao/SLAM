/*
 * Main.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */

#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>
#include <cmath>
#include <climits>
#include <algorithm>    // std::reverse
#include "Point.h"
#include "GraphSearch.h"
#include "smooth.h"

using namespace std;

vector<vector<unsigned int> > map = {
		{1, 0, 1, 1, 1},
		{1, 0, 1, 0, 1},
		{1, 1, 1, 1, 1},
		{0, 0, 1, 0, 0},
		{0, 0, 1, 0, 0},};

//Define the cost for each action,left turn is expensive in real life
//[0]:Left turn	[1]:Forward	[2]:Right turn
//unsigned int actionCost[]={100,2,4};
unsigned int actionCost[]={100,0,2};

int main(void) {
	//Point start(0,0),target(2,2);
	Point<unsigned int> start(4,2),target(0,0);
	GraphSearch gs;
	vector<Point<unsigned int> > route;

	if (gs.aStar(map,actionCost,start,target)) {
		gs.printRouteOnMap(map);
		gs.getRoute(route);
		for(auto r : route)
			cout << "(" << r.x << "," << r.y << ")" << endl;
	} else {
		cout << "Search failed.\n";
	}

	vector<Point<float> > smoothRoute;
	for(auto r:route){
		Point<float> p(r.x,r.y,r.dir);
		smoothRoute.push_back(p);
	}

	Smooth sm;
	//sm.smooth(smoothRoute);
	sm.smooth(smoothRoute,3);
	for(auto r : smoothRoute)
		printf("%.2f,%.2f\n",r.x,r.y);

   return 0;
}
