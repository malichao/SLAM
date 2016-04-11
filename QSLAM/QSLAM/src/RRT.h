/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 31,2016
Version		: v0.2
Built		: Eclipse CDT, -std=c++11
Description :
	-v0.2 Add vehicle dynamic model in searching
	-v0.1 Rapidly-Exploring Random Tree Searching Algorithm.
*****************************************************************************/

#ifndef RRT_H_
#define RRT_H_

#include <vector>
#include "MapSearch.h"
#include "vehicle.h"

namespace SearchAlgorithms{
using namespace std;

class RRTSearch: public MapSearch{
public:
	struct Line{
		Line(Point_uint &s,Point_uint &e):start(s),end(e){}

		Point_uint start;
		Point_uint end;
	};
	struct Node{
		Node():val(Point_uint(0,0)),prev(0){}
		Node(Point_uint &v,size_t p):val(v),prev(p){}
		Node(const Point_uint &v,size_t p,const Vehicle::VehicleState &vs):val(v),prev(p),state(vs){}

		Point_uint val;
		size_t prev;

		Vehicle::VehicleState state;
	};
	RRTSearch(): MaxSearchTime(1000),Epsilon(10){}

	void demo(size_t width,size_t height,size_t searchTime,size_t epsilon);

	bool search( const vector<vector<bool> > &map,
				 vector<Point_uint> &route);
	bool search( const vector<vector<bool> > &map,
				 const Point<unsigned int> &start,
				 const Point<unsigned int> &target,
				 vector<Point_uint> &route);
	bool searchUsingVehicle( const vector<vector<bool> > &map,
							 const Point_uint &start,
							 const Point_uint &target,
							 vector<Point_uint> &route);

	size_t getLineSize() const { return Lines.size();}
	Line getLine(size_t i) const {return Lines[i];}
	Line getLineAt(size_t i) const { return Lines.at(i);}

	double getScale() const {return Scale;}
	void setScale(const double s) { Scale=s;}

    void getRoute(vector<Node> &route);

private:
	vector<Node> Nodes;
    vector<Node> Route;
	size_t MaxSearchTime;
	unsigned int Epsilon;
	double Scale;	// pixels/meter

	vector<Line> Lines;

	Point_uint stepFromTo(Point_uint &a,Point_uint &b);
	void stepFromTo( Vehicle &v,
					 const Vehicle::VehicleState &a,
					 const Vehicle::VehicleState &b,
					 Vehicle::VehicleState &next);

	Point_uint randomPoint(const vector<vector<bool> > &map);
	size_t findShortestNode(Point_uint &n,Node &shortest);
	bool checkCollision(const vector<vector<bool> > &map,Node &a,Node &b);

	void generateRoute(vector<Point_uint> &route,Node n);


};

}// End of namespace SearchAlgorithms

#endif /* RRT_H_ */
