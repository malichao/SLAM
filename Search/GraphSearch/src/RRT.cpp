/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 31,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	Rapidly-Exploring Random Tree Searching Algorithm.
*****************************************************************************/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RRT.h"

namespace SearchAlgorithms{
using namespace std;

Point<int> RRTSearch::stepFromTo(Point<int> &a,Point<int> &b){
	if((unsigned int)pnt::dis(a,b)<Epsilon)
		return b;
	double theta=atan2(double(b.y)-double(a.y),double(b.x)-double(a.x));
	return Point<int>(a.x+Epsilon*cos(theta),a.y+Epsilon*sin(theta));
}

bool RRTSearch::search(const vector<vector<bool> > &map,
				   const Point<unsigned int> &start,
				   const Point<unsigned int> &target,
				   vector<Point<int>> &route) {
	setTarget(target);
	setStart(start);
	return search(map,route);
}

bool RRTSearch::search(const vector<vector<bool> > &map,vector<Point<int> > &route) {
	srand(time(NULL));
	Nodes.clear();
	//Nodes.push_back(Start);

	Epsilon=10;
	size_t searchTime=1000;

	for(size_t i=0;i<searchTime;i++){
		Point<int> randPoint(rand()%map.size(),rand()%map[0].size());

		Point<int> shortestPoint=Nodes[0];
		for(auto n:Nodes){
			if(pnt::dis(randPoint,n)<pnt::dis(randPoint,shortestPoint))
				shortestPoint=n;
		}

		Point<int> newNode = stepFromTo(shortestPoint,randPoint);
		if(newNode.x>1000||newNode.y>1000)
		{
			int x=1;
			newNode = stepFromTo(shortestPoint,randPoint);
		}
		Nodes.push_back(newNode);
		Lines.push_back(Line(shortestPoint, newNode));

	}
}

void RRTSearch::demo(size_t width,size_t height,size_t searchTime,size_t epsilon) {
	srand(time(NULL));
	Nodes.clear();
	Nodes.push_back(Point<int>(height/2, width/2));

	Epsilon=epsilon;

	for(size_t i=0;i<searchTime;i++){
		Point<int> randPoint(rand()%height,rand()%width);

		Point<int> shortestPoint=Nodes[0];
		for(auto n:Nodes){
			if(pnt::dis(randPoint,n)<pnt::dis(randPoint,shortestPoint))
				shortestPoint=n;
		}

		Point<int> newNode = stepFromTo(shortestPoint,randPoint);
		if(newNode.x>1000||newNode.y>1000)
		{
			int x=1;
			newNode = stepFromTo(shortestPoint,randPoint);
		}
		Nodes.push_back(newNode);
		Lines.push_back(Line(shortestPoint, newNode));

	}
}
}// End of namespace SearchAlgorithms
