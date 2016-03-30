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
#include "BFS.h"
namespace SearchAlgorithms{
using namespace std;
class MapSearch{
public:
	static const unsigned int Obstacle;	//positive value means the cost of each path
	static const char ObstacleSymbol;
	static const unsigned int CostMax;//Don't use UINT_MAX,watch out for OVERFLOW
	MapSearch():Start(0,0),Target(0,0),EffortCount(0){};
	virtual ~MapSearch();

	void setStart(const Point<unsigned int> &s){Start.x=s.x; Start.y=s.y;Start.dir=s.dir;}
	void setTarget(const Point<unsigned int> &t){Target.x=t.x; Target.y=t.y;Target.dir=t.dir;}

	int 	getMinDistance(){return Route.size()-1;}
	void printRoute();
	void getRoute(vector<Point<unsigned int> > &route);
	void printRouteOnMap(const vector<vector<unsigned int> > &map);
	void printGradientOnMap(const vector<vector<unsigned int> > &map);

	virtual bool search( const vector<vector<unsigned int> > &map);
	virtual bool search( const vector<vector<unsigned int> > &map,
						const Point<unsigned int> &start,
						const Point<unsigned int> &target);
protected:
	Point<unsigned int> Start;
	Point<unsigned int> Target;
	unsigned int EffortCount;

	vector<Point<unsigned int> > Route;
	vector<vector<bool> > Checked;
	vector<vector<Point<unsigned int> > > Gradient;
	vector<vector<unsigned int> > CostMap;

	void generateRoute();
	void generateRoute(const vector<vector<unsigned int> > &map);
	void generateRoute(const vector<vector<vector<Point<unsigned int> > > > &Gradient);
	unsigned int abs(Point<unsigned int> &a, Point<unsigned int> &b);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const Point<unsigned int> &p);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const unsigned int* const moveCost,
				 const Point<unsigned int> &p);
	unsigned int minCost(const vector<vector<unsigned int> > &map,
						const Point<unsigned int> &p,
						Point<unsigned int> &move);

	unsigned int minCost(const vector<vector<unsigned int> > &map,
						 const unsigned int* const moveCost,
						 const Point<unsigned int> &point,
						 Point<unsigned int> &move);

	//const unsigned int NoneDirection = 4;
	static const int Move[5][2];
	static const char DirSymbol[5];

};

}

#endif /* MAPSEARCH_H_ */
