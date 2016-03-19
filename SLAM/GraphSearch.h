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
	Point<int> point;
	//f=h+g
	unsigned int f;	//f is the final evaluation of a grid
	unsigned int h;	//h is the heuristic value of a grid
	unsigned int g;	//g is the cost of a grid given by the map
	AStarPoint(Point<int> &p,int i,int j):
		point(p),f(i+j),h(i),g(j){};
};
struct DPPoint{
	Point<int> point;
	int cost;
	DPPoint(const Point<int> &p,unsigned int cost):
		point(p),cost(cost){};
};

class GraphSearch {
public:
   const unsigned int Obstacle =0;			//positive value means the cost of each path
   const char ObstacleSymbol = '#';
   const unsigned int CostMax= 999;		//Don't use UINT_MAX,watch out for OVERFLOW
   GraphSearch():Start(0,0),Target(0,0),EffortCount(0){};

   void setStart(const Point<int> &s){Start.x=s.x; Start.y=s.y;Start.dir=s.dir;}
   void setTarget(const Point<int> &t){Target.x=t.x; Target.y=t.y;Target.dir=t.dir;}

   int 	getMinDistance(){return Route.size()-1;}
   void printRoute();
   void getRoute(vector<Point<int> > &route);
   void printRouteOnMap(const vector<vector<unsigned int> > &map);
   void printGradientOnMap(const vector<vector<unsigned int> > &map);

   bool bfs(const vector<vector<unsigned int> > &map);
   bool bfs(const vector<vector<unsigned int> > &map,
		    const Point<int> &start,const Point<int> &target);

   bool aStar(const vector<vector<unsigned int> > &map);
   bool aStar(const vector<vector<unsigned int> > &map,
		      const Point<int> &start,const Point<int> &target);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost,
   		      const Point<int> &start,const Point<int> &target);

   bool dpSearch(const vector<vector<unsigned int> > &map);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const Point<int> &start,const Point<int> &target);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const unsigned int *const moveCost);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	   	 const unsigned int *const moveCost,
   		   	     const Point<int> &start,const Point<int> &target);
private:
	Point<int> Start;
	Point<int> Target;
	unsigned int EffortCount;

	vector<Point<int> > Route;
	vector<vector<bool> > Checked;
	vector<vector<Point<int> > > Gradient;
	vector<vector<unsigned int> > CostMap;

	vector<vector<vector<bool> > > Checked3D;
	vector<vector<vector<Point<int> > > > Gradient3D;
	vector<vector<vector<unsigned int> > > CostMap3D;

	void generateRoute();
	void generateRoute(const vector<vector<unsigned int> > &map);
	void generateRoute(const vector<vector<vector<Point<int> > > > &Gradient);
	unsigned int abs(Point<int> &a, Point<int> &b);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const Point<int> &p);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const unsigned int* const moveCost,
				 const Point<int> &p);
	unsigned int minCost(const vector<vector<unsigned int> > &map,
   						const Point<int> &p,
						Point<int> &move);

	unsigned int minCost(const vector<vector<unsigned int> > &map,
						 const unsigned int* const moveCost,
						 const Point<int> &point,
						 Point<int> &move);

	//const unsigned int NoneDirection = 4;
	int Move[5][2] = {
		   {-1,0},	// x-1,y
		   { 0,1},	// x,y+1
		   { 1,0},	// x+1,y
		   {0,-1},	// x,y-1
		   {0, 0}};	// x,y
	char DirSymbol[5]={'^','>','V','<',' '};

};



#endif /* GRAPHSEARCH_H_ */
