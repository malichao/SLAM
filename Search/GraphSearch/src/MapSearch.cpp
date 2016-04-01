/******************************************************************************
 Author		: Lichao Ma
 Date  		: Mar 29,2016
 version		: v0.1
 Description :
 Map searching algorithms.
 *****************************************************************************/
#include <limits.h>	//INT_MAX
#include <iostream>
#include <stdio.h>
#include "MapSearch.h"

namespace SearchAlgorithms {

using namespace std;

const unsigned int MapSearch::CostMax = INT_MAX; //Don't use UINT_MAX,watch out for OVERFLOW
const unsigned int MapSearch::Obstacle = 0;	//positive value means the cost of each path
const char MapSearch::ObstacleSymbol = '#';

//const unsigned int NoneDirection = 4;
const int MapSearch::Move[5][2] = {
		{ -1,  0 },	// x-1,y
		{  0,  1 },	// x,y+1
		{  1,  0 },	// x+1,y
		{  0, -1 },	// x,y-1
		{  0,  0 } };	// x,y

void MapSearch::initData(const vector<vector<unsigned int> > &map){
	Gradient.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point<unsigned int> > zero(map[i].size(), Point<unsigned int>(0, 0));
		Gradient.push_back(zero);
	}
	Checked.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the checked set
		vector<bool> temp(map[i].size(), false);
		Checked.push_back(temp);
	}

	EffortCount = 1;	//Reset the EffortCount to count the search effort
}


//Generate a route using the direction info
void MapSearch::generateRoute(vector<Point_uint> &route) {
	if (Gradient.size() == 0)
		return;
	Point_uint curPos = Start;
	do {
		route.push_back(curPos);
		curPos = Gradient[curPos.x][curPos.y];
	} while (curPos != Target);
	route.push_back(Target);
}

bool MapSearch::isLegal(const vector<vector<unsigned int> > &map,
		const Point<unsigned int> &p) {
	EffortCount++;
	if (p.x >= map.size() || p.y >= map[0].size())	//x and y are unsigned int
		return false;
	if (Checked[p.x][p.y])   	//Current position has been searched
		return false;
	if (map[p.x][p.y] == Obstacle)		//Don't hit the wall
		return false;
	return true;
}

}		//End namespace SearchAlgorithms

