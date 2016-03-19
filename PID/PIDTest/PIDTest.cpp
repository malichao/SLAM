/*
 * PIDTest.cpp
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
#include <GraphSearch.cpp>

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
unsigned int actionCost[]={1,0,50};

int main(void) {
   //Point start(0,0),target(2,2);
   Point start(8,13),target(2,2);
   Point dummy;
   GraphSearch b;

   /*
	if (b.bfs(map, start, target)) {
		b.printRouteOnMap(map);
	} else {
		cout << "Search failed.\n";
	}

	if (b.aStar(map, start, target)) {
		b.printRouteOnMap(map);
	} else {
		cout << "Search failed.\n";
	}


	if (b.dpSearch(map, start, target)) {
		b.printGradientOnMap(map);
	} else {
		cout << "Search failed.\n";
	}
*/

   Point start1(0,0,dummy.Down),target1(4,2);
   if(b.aStar(map,actionCost,start1,target1));
   if (b.aStar(map,actionCost,start1,target1)) {
   		b.printRouteOnMap(map);
   	} else {
   		cout << "Search failed.\n";
   	}

   return 0;
}
