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
#include <algorithm>	//reverse()
#include "RRT.h"

namespace SearchAlgorithms{
using namespace std;

Point_uint RRTSearch::stepFromTo(Point_uint &a,Point_uint &b){
	if((unsigned int)pnt::dis(a,b)<Epsilon)
		return b;
	double theta=atan2(double(b.y)-double(a.y),double(b.x)-double(a.x));
	return Point_uint(a.x+Epsilon*cos(theta),a.y+Epsilon*sin(theta));
}

bool RRTSearch::search(const vector<vector<bool> > &map,
				   const Point<unsigned int> &start,
				   const Point<unsigned int> &target,
				   vector<Point_uint> &route) {
	setTarget(target);
	setStart(start);
	return search(map,route);
}

Point_uint RRTSearch::randomConfig(const vector<vector<bool> > &map){
	unsigned int x,y;
	do{
		x=rand()%map.size();
		y=rand()%map[0].size();
		MapSearch::EffortCount++;
	}while(map[x][y]==false);
	return Point_uint(x,y);
}

void RRTSearch::findShortestNode(Node &n,Node &shortest){
	shortest=Nodes[0];
	for(size_t i=0;i<Nodes.size();i++){
		if(pnt::dis(n.val,Nodes[i].val)<pnt::dis(n.val,shortest.val)){
			shortest=Nodes[i];
		}
		MapSearch::EffortCount++;
	}
}

bool RRTSearch::search(const vector<vector<bool> > &map,vector<Point_uint > &route) {
	MapSearch::EffortCount=0;
	srand(time(NULL));
	Nodes.clear();
	Nodes.push_back(Node(Start,0));

	Epsilon=2000;
	size_t searchTime=500;

	for(size_t i=0;i<searchTime;i++){
		Point_uint randPoint=randomConfig(map);

		Point_uint shortestPoint=Nodes[0].val;
		size_t prev=0;
		for(size_t i=0;i<Nodes.size();i++){
			if(pnt::dis(randPoint,Nodes[i].val)<pnt::dis(randPoint,shortestPoint)){
				shortestPoint=Nodes[i].val;
				prev=i;
			}
			MapSearch::EffortCount++;
		}

		Point_uint newNode = stepFromTo(shortestPoint,randPoint);

	    int length=pnt::dis(shortestPoint,newNode);
	    if(length==0)
	    	continue;

	    int lengthX=newNode.x-shortestPoint.x;
	    int lengthY=newNode.y-shortestPoint.y;
	    bool collision=false;

	    for(int i=0;i<length;i++){
	    	int x=(int)shortestPoint.x+i*lengthX/length;
	    	int y=(int)shortestPoint.y+i*lengthY/length;
	    	if(map[x][y]==false){
	    		collision=true;
	    		break;
	    	}
	    	MapSearch::EffortCount++;
	    }
	    if(!collision){
	    	Node temp(newNode,prev);
			Nodes.push_back(temp);
			Lines.push_back(Line(shortestPoint, newNode));
			if(newNode==Target){
				generateRoute(route,temp);
				return true;
			}
	    }
	}
	return false;
}

void RRTSearch::generateRoute(vector<Point_uint> &route,Node n){
	auto it=route.end()-1;
	do{
		route.push_back(n.val);
		n=Nodes[n.prev];
	}while(n.prev!=0);
	route.push_back(n.val);	//Start point;
	std::reverse(it,route.end());
}

void RRTSearch::demo(size_t width,size_t height,size_t searchTime,size_t epsilon) {
	srand(time(NULL));
	Nodes.clear();
	Point_uint x(height/2, width/2);
	Nodes.push_back(Node(x,0));

	Epsilon=epsilon;

	for(size_t i=0;i<searchTime;i++){
		Point_uint randPoint(rand()%height,rand()%width);

		Point_uint shortestPoint=Nodes[0].val;
		for(auto n:Nodes){
			if(pnt::dis(randPoint,n.val)<pnt::dis(randPoint,shortestPoint))
				shortestPoint=n.val;
		}

		Point_uint newNode = stepFromTo(shortestPoint,randPoint);
		Nodes.push_back(Node(newNode,0));
		Lines.push_back(Line(shortestPoint, newNode));

	}
}
}// End of namespace SearchAlgorithms
