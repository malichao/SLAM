/******************************************************************************
 Author		: Lichao Ma
 Date  		: Mar 29,2016
 Version	: v0.2
 Description :
	-v0.2	Remove print functions so that this class is more portable.
	-v0.1	Map searching algorithms base class.Provides basic function calls.
 *****************************************************************************/
#include <limits.h>	//INT_MAX

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

//Todo:rewrite this function to make it clear
//Find the minimal cost of the four direction from visited points
unsigned int MapSearch::minCost(  const vector<vector<unsigned int> > &map,
								 const Point<unsigned int> &point,
								 Point<unsigned int> &nextMove) const{
	nextMove=point;	//0:Up	1:Down	2:Left	3:Right	4:None
	if(point==Target)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX otherwise it will overflow later on
	for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
		Point<unsigned int> p(point.x+Move[k][0],point.y+Move[k][1],k);

		//In the predicate,must check the bounding first.We don't use isLegal() because we're
		//going to find the minimal cost point in previously visited points
		if ((p.x>=0 && p.y>=0 && p.x < map.size() &&p.y < map[0].size())&& Checked[p.x][p.y]
					&& map[p.x][p.y] != Obstacle){
			if(CostMap[p.x][p.y] < min){
				min=CostMap[p.x][p.y];
				nextMove=p;
			}
		}
	}
	return min;
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

