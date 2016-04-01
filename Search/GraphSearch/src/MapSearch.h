/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 29,2016
version		: v0.1
Description :
	Map searching algorithms.
*****************************************************************************/

#ifndef MAPSEARCH_H_
#define MAPSEARCH_H_
#include <vector>

#include "Point.h"
//#include "BFS.h"

namespace SearchAlgorithms{

using namespace std;

typedef Point<unsigned int> Point_uint;

/*
 * About the map:
 *  0 means the obstacles,positive values means the cost each grid.Although
 *  in Search these values are used but to make a comparison between different
 *  algorithms,such as A*,Dijkstra shortest path,we need a map that works
 *  for all these algorithms.
 *
 *  Notice about the map coordinate:
 *  map[x][y]:
 *  0-------> y
 *  |
 *  |
 *  v
 *  x
 */
class MapSearch{
public:
	static const unsigned int Obstacle;	//positive value means the cost of each path
	static const char ObstacleSymbol;
	static const unsigned int CostMax;//Don't use UINT_MAX,watch out for OVERFLOW
	MapSearch():Start(0,0),Target(0,0),EffortCount(0){};
	virtual ~MapSearch(){};

	void setStart(const Point_uint &s){Start.x=s.x; Start.y=s.y;Start.dir=s.dir;}
	void setTarget(const Point_uint &t){Target.x=t.x; Target.y=t.y;Target.dir=t.dir;}

	void getRoute(vector<Point_uint > &route);

	virtual bool search( const vector<vector<unsigned int> > &map)  { return false;}
	virtual bool search( const vector<vector<unsigned int> > &map,
						const Point_uint &start,
						const Point_uint &target) 			{ return false;}
protected:
	Point_uint Start;
	Point_uint Target;
	unsigned int EffortCount;

	vector<vector<bool> > Checked;
	vector<vector<Point_uint > > Gradient;
	vector<vector<unsigned int> > CostMap;

	static const int Move[5][2];

	void generateRoute(vector<Point_uint> &route);
	void generateRoute(const vector<vector<unsigned int> > &map);
	void generateRoute(const vector<vector<vector<Point_uint > > > &Gradient);

	unsigned int abs(Point_uint &a, Point_uint &b);

	void initData(const vector<vector<unsigned int> > &map);

	bool isLegal(const vector<vector<unsigned int> > &map,
				 const Point_uint &p);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const unsigned int* const moveCost,
				 const Point_uint &p);
	unsigned int minCost(const vector<vector<unsigned int> > &map,
						const Point_uint &p,
						Point_uint &move);

	unsigned int minCost(const vector<vector<unsigned int> > &map,
						 const unsigned int* const moveCost,
						 const Point_uint &point,
						 Point_uint &move);
};

}

#endif /* MAPSEARCH_H_ */
