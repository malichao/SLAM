/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 30,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	A* Searching Algorithm.
*****************************************************************************/

#ifndef ASTAR_H_
#define ASTAR_H_

#include "MapSearch.h"

namespace SearchAlgorithms{
using namespace std;

class AStar: public MapSearch{
public:
	struct AStarPoint{
		Point<unsigned int> point;
		//f=h+g
		unsigned int f;	//f is the final evaluation of a grid
		unsigned int h;	//h is the heuristic value of a grid
		unsigned int g;	//g is the cost of a grid given by the map
		AStarPoint(Point<unsigned int> &p,int i,int j):
			point(p),f(i+j),h(i),g(j){};
	};

	bool search( const vector<vector<unsigned int> > &map,
				 vector<Point_uint> &route);
	bool search( const vector<vector<unsigned int> > &map,
				 const Point<unsigned int> &start,
				 const Point<unsigned int> &target,
				 vector<Point_uint> &route);

};

}// End of namespace SearchAlgorithms


#endif /* ASTAR_H_ */
