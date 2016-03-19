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

using namespace std;
struct Point {
   //Must define these values in clockwise/counter-clockwise manner
   const unsigned int DirectionSize=4;
   const unsigned int Up=0;
   const unsigned int Right=1;
   const unsigned int Down=2;
   const unsigned int Left=3;

   unsigned int x;
   unsigned int y;
   unsigned int dir;	//0:Up	1:Down 2:Left	2:Right

   Point():x(0),y(0),dir(Up){};
   Point(unsigned int x,unsigned int y):x(x),y(y),dir(Up){};
   Point(unsigned int x,unsigned int y,unsigned int dir):x(x),y(y),dir(dir){};
   Point(const Point &p):x(p.x),y(p.y),dir(p.dir){};
   void set(const unsigned int x,const unsigned int y,const unsigned int dir){
	   this->x=x;
	   this->y=y;
	   this->dir=dir;
   }
   Point& operator=(const Point &rhs){
	   x=rhs.x;
	   y=rhs.y;
	   dir=rhs.dir;
	   return *this;
   }

   bool operator== (const Point &rhs) const{
      return (x==rhs.x&&y==rhs.y);		//Different orientation considered as the same
   }
   bool operator!= (const Point &rhs){
		return (x!=rhs.x||y!=rhs.y);	//Different orientation considered as the same
	 }
};
struct AStarPoint{
	Point point;
	//f=h+g
	unsigned int f;	//f is the final evaluation of a grid
	unsigned int h;	//h is the heuristic value of a grid
	unsigned int g;	//g is the cost of a grid given by the map
	AStarPoint(Point p,int i,int j):
		point(p),f(i+j),h(i),g(j){};
};
struct DPPoint{
	Point point;
	int cost;
	DPPoint(const Point &p,unsigned int cost):
		point(p),cost(cost){};
};

class GraphSearch {
public:
   const unsigned int Obstacle =0;			//positive value means the cost of each path
   const char ObstacleSymbol = '#';
   const unsigned int CostMax= 999;		//Don't use UINT_MAX,watch out for OVERFLOW
   GraphSearch():Start(0,0),Target(0,0),EffortCount(0){};

   void setStart(const Point &s){Start.x=s.x; Start.y=s.y;Start.dir=s.dir;}
   void setTarget(const Point &t){Target.x=t.x; Target.y=t.y;Target.dir=t.dir;}

   int 	getMinDistance(){return Route.size()-1;}
   void printRoute();
   void printRouteOnMap(const vector<vector<unsigned int> > &map);
   void printGradientOnMap(const vector<vector<unsigned int> > &map);

   bool bfs(const vector<vector<unsigned int> > &map);
   bool bfs(const vector<vector<unsigned int> > &map,
		    const Point &start,const Point &target);

   bool aStar(const vector<vector<unsigned int> > &map);
   bool aStar(const vector<vector<unsigned int> > &map,
		      const Point &start,const Point &target);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost,
   		      const Point &start,const Point &target);

   bool dpSearch(const vector<vector<unsigned int> > &map);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const Point &start,const Point &target);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const unsigned int *const moveCost);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	   	 const unsigned int *const moveCost,
   		   	     const Point &start,const Point &target);
private:
	Point Start;
	Point Target;
	unsigned int EffortCount;

	vector<Point> Route;
	vector<vector<bool> > Checked;
	vector<vector<Point> > Gradient;
	vector<vector<unsigned int> > CostMap;

	vector<vector<vector<bool> > > Checked3D;
	vector<vector<vector<Point> > > Gradient3D;
	vector<vector<vector<unsigned int> > > CostMap3D;

	void generateRoute();
	void generateRoute(const vector<vector<unsigned int> > &map);
	void generateRoute(const vector<vector<vector<Point> > > &Gradient);
	unsigned int abs(Point &a, Point &b);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const Point &p);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const unsigned int* const moveCost,
				 const Point &p);
	unsigned int minCost(const vector<vector<unsigned int> > &map,
   						const Point &p,
						Point &move);

	unsigned int minCost(const vector<vector<unsigned int> > &map,
						 const unsigned int* const moveCost,
						 const Point &point,
						 Point &move);

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
