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

Point_uint RRTSearch::stepFromTo(Point_uint &a,Point_uint &b){
	if((unsigned int)pnt::dis(a,b)<Epsilon)
		return b;
	double theta=atan2(double(b.y)-double(a.y),double(b.x)-double(a.x));
	return Point_uint(a.x+Epsilon*cos(theta),a.y+Epsilon*sin(theta));
}

bool RRTSearch::search(const vector<vector<unsigned int> > &map,
				   const Point<unsigned int> &start,
				   const Point<unsigned int> &target,
				   vector<Point_uint> &route) {
	setTarget(target);
	setStart(start);
	//return search(map,route);
}

void RRTSearch::demo(size_t width,size_t height,size_t SearhTime) {
	srand(time(NULL));
	Nodes.clear();
	Nodes.push_back(Point_uint(height/2, width/2));

	for(size_t i=0;i<SearhTime;i++){
		Point_uint randPoint(rand()%height,rand()%width);

		Point_uint shortestPoint=Nodes[0];
		for(auto n:Nodes){
			if(pnt::dis(randPoint,n)<pnt::dis(randPoint,shortestPoint))
				shortestPoint=n;
		}

		Point_uint newNode = stepFromTo(shortestPoint,randPoint);
		Nodes.push_back(newNode);
		Lines.push_back(Line(shortestPoint, newNode));

	}
}
}// End of namespace SearchAlgorithms
