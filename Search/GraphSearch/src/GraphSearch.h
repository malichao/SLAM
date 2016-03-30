/*
 * GraphSearch.h
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */

#ifndef GRAPHSEARCH_H_
#define GRAPHSEARCH_H_
#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>
#include <cmath>
#include <climits>
#include <algorithm>    // std::reverse
#include "Point.h"
using namespace std;

struct AStarPoint{
	Point<unsigned int> point;
	//f=h+g
	unsigned int f;	//f is the final evaluation of a grid
	unsigned int h;	//h is the heuristic value of a grid
	unsigned int g;	//g is the cost of a grid given by the map
	AStarPoint(Point<unsigned int> &p,int i,int j):
		point(p),f(i+j),h(i),g(j){};
};
struct DPPoint{
	Point<unsigned int> point;
	int cost;
	DPPoint(const Point<unsigned int> &p,unsigned int cost):
		point(p),cost(cost){};
};

class GraphSearch {
public:
   static const unsigned int Obstacle =0;		//positive value means the cost of each path
   static const char ObstacleSymbol = '#';
   const unsigned int CostMax= 999;		//Don't use UINT_MAX,watch out for OVERFLOW
   GraphSearch():Start(0,0),Target(0,0),EffortCount(0){};

   void setStart(const Point<unsigned int> &s){Start.x=s.x; Start.y=s.y;Start.dir=s.dir;}
   void setTarget(const Point<unsigned int> &t){Target.x=t.x; Target.y=t.y;Target.dir=t.dir;}

   int 	getMinDistance(){return Route.size()-1;}
   void printRoute();
   void getRoute(vector<Point<unsigned int> > &route);
   void printRouteOnMap(const vector<vector<unsigned int> > &map);
   void printGradientOnMap(const vector<vector<unsigned int> > &map);

   bool bfs(const vector<vector<unsigned int> > &map);
   bool bfs(const vector<vector<unsigned int> > &map,
		    const Point<unsigned int> &start,const Point<unsigned int> &target);

   bool aStar(const vector<vector<unsigned int> > &map);
   bool aStar(const vector<vector<unsigned int> > &map,
		      const Point<unsigned int> &start,const Point<unsigned int> &target);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost,
   		      const Point<unsigned int> &start,const Point<unsigned int> &target);

   bool dpSearch(const vector<vector<unsigned int> > &map);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const Point<unsigned int> &start,const Point<unsigned int> &target);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const unsigned int *const moveCost);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	   	 const unsigned int *const moveCost,
   		   	     const Point<unsigned int> &start,const Point<unsigned int> &target);
private:
	Point<unsigned int> Start;
	Point<unsigned int> Target;
	unsigned int EffortCount;

	vector<Point<unsigned int> > Route;
	vector<vector<bool> > Checked;
	vector<vector<Point<unsigned int> > > Gradient;
	vector<vector<unsigned int> > CostMap;

	vector<vector<vector<bool> > > Checked3D;
	vector<vector<vector<Point<unsigned int> > > > Gradient3D;
	vector<vector<vector<unsigned int> > > CostMap3D;

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
	const int Move[5][2] = {
		   {-1,0},	// x-1,y
		   { 0,1},	// x,y+1
		   { 1,0},	// x+1,y
		   {0,-1},	// x,y-1
		   {0, 0}};	// x,y
	const char DirSymbol[5]={'^','>','V','<',' '};

};



#endif /* GRAPHSEARCH_H_ */
