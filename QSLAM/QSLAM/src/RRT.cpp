/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 31,2016
Version		: v0.2
Built		: Eclipse CDT, -std=c++11
Description :
	-v0.2 Add vehicle dynamic model in searching
	-v0.1 Rapidly-Exploring Random Tree Searching Algorithm.
*****************************************************************************/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <algorithm>	//reverse()
#include <exception>
#include <iostream>
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

Point_uint RRTSearch::randomPoint(const vector<vector<bool> > &map){
	unsigned int x,y;
	do{
		x=rand()%map.size();
		y=rand()%map[0].size();
		MapSearch::EffortCount++;
	}while(map[x][y]==false);
	return Point_uint(x,y);
}

/*
void RRTSearch::talorApprox(Vehicle &v,
							const Vehicle::VehicleInput &vi,
							const Vehicle::VehicleState &old,
							Vehicle::VehicleState &next){
	Vehicle::VehicleState k1,k2,k3,k4;
	double period=vi.period;
	v.calculateVehicleStateDelta(vi,old,k1);
	vi.period=period/2;
	v.calculateVehicleState(vi,old+k1/2,k2);
}
*/
// According to the target state,generate a best steering angle to
// get closest to it,assume speed is constant.
void RRTSearch::stepFromTo(  Vehicle &v,
							 const Vehicle::VehicleState &a,
							 const Vehicle::VehicleState &b,
							 Vehicle::VehicleState &next){
	Vehicle::VehicleInput vi;
	vi.period=0.05;
	vi.speed=4;

	double minDis=DBL_MAX ;
	double bestAngle=0;
	// -45~45 degree,0.5 tolerance
	for(double angle=-45.0;angle<45.0;angle+=5){
		vi.steerAngle=angle/180*3.14;
		v.calculateVehicleState(vi,a,next);
		double length=pnt::dis(b.x,b.y,next.x,next.y);
		if(length<minDis){
			minDis=length;
			bestAngle=angle/180*3.14;;
		}
	}
	vi.steerAngle=bestAngle;
	v.calculateVehicleState(vi,a,next);
}

size_t RRTSearch::findShortestNode(Point_uint &p,Node &shortest){
	shortest=Nodes[0];
	size_t prev=0;
	for(size_t i=0;i<Nodes.size();i++){
		if(pnt::dis(p,Nodes[i].val)<pnt::dis(p,shortest.val)){
			shortest=Nodes[i];
			prev=i;
		}
		MapSearch::EffortCount++;
	}
	return prev;
}
bool RRTSearch::checkCollision(const vector<vector<bool> > &map,Node &a,Node &b){
	// Cautious!! Notice the bound! it's '>=' !
	if(a.val.x>=map.size()||a.val.y>=map[0].size()) return true;
	if(b.val.x>=map.size()||b.val.y>=map[0].size()) return true;
	int length=pnt::dis(a.val,b.val);
	if(length==0)
		return false;

	int lengthX=(int)b.val.x-(int)a.val.x;
	int lengthY=(int)b.val.y-(int)a.val.y;
	bool collision=false;

	for(int i=0;i<length-1;i++){
		int x=(int)a.val.x+i*lengthX/length;
		int y=(int)a.val.y+i*lengthY/length;
		if(map[x][y]==false){
			collision=true;
			break;
		}
		MapSearch::EffortCount++;
	}
	return collision;
}

bool RRTSearch::search(const vector<vector<bool> > &map,vector<Point_uint > &route) {
	MapSearch::EffortCount=0;
	srand(time(NULL));
	Nodes.clear();
	Nodes.push_back(Node(Start,0));

	Epsilon=2000;
	size_t searchTime=500;

	for(size_t i=0;i<searchTime;i++){
		Point_uint randPoint=randomPoint(map);
		Node shortestNode;
		int prev=findShortestNode(randPoint,shortestNode);
		Point_uint newPoint=stepFromTo(shortestNode.val,randPoint);
		Node newNode(newPoint,prev);
	    if(!checkCollision(map,shortestNode,newNode)){
			Nodes.push_back(newNode);
			Lines.push_back(Line(shortestNode.val, newNode.val));
			if(pnt::dis(Target,newNode.val)<10){
				generateRoute(route,newNode);
				return true;
			}
	    }
	}
	Node shortest;
	findShortestNode(Target,shortest);
	generateRoute(route,shortest);

	return false;
}

void RRTSearch::generateRoute(vector<Point_uint> &route,Node n){
	auto it=route.end()-1;
	do{
		route.push_back(n.val);
		n=Nodes[n.prev];
	}while(n.prev!=0);
	route.push_back(n.val);	// n.prev->Start;
	route.push_back(Start);
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

//Todo: Use Taylor series to calculate the next vehicle state and increase the
// calculation period.
bool RRTSearch::searchUsingVehicle(  const vector<vector<bool> > &map,
									 const Point_uint &start,
									 const Point_uint &target,
									 vector<Point_uint> &route){
	Scale =300.0;	// 22 pixels/meter

	setTarget(target);
	setStart(start);

	Vehicle v;
	v.setToStandardVehicle();
	Vehicle::VehicleState startState;
	startState.x=start.x/Scale;
	startState.y=start.y/Scale;
	startState.orientation=0;
	startState.speedLongitudinal=1;
	startState.speedYaw=0;

	MapSearch::EffortCount=0;
	srand(time(NULL));
	Nodes.clear();
	Nodes.push_back(Node(start,0,startState));

	Epsilon=2000;
	size_t searchTime=5000;

	for(size_t i=0;i<searchTime;i++){
		Point_uint randPoint=randomPoint(map);
		Node shortestNode;
		int prev=findShortestNode(randPoint,shortestNode);

		// Calculate the new vehicle state
		Node randomNode(randPoint,prev,shortestNode.state);

		Vehicle::VehicleState randomState;
		randomState.x=randPoint.x/Scale;
		randomState.y=randPoint.y/Scale;
		Vehicle::VehicleState newState;

		stepFromTo(v,shortestNode.state,randomState,newState);
		Point_uint temp(newState.x*Scale,newState.y*Scale);
		Node newNode(temp,prev,newState);

	    if(!checkCollision(map,shortestNode,newNode)){
			Nodes.push_back(newNode);
			Lines.push_back(Line(shortestNode.val, newNode.val));
			if(pnt::dis(Target,newNode.val)<10){
				generateRoute(route,newNode);
				return true;
			}
	    }
	}
	Node shortest;
	findShortestNode(Target,shortest);
	generateRoute(route,shortest);

	return false;
}

}// End of namespace SearchAlgorithms
