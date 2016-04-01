/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 31,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	Rapidly-Exploring Random Tree Searching Algorithm.
*****************************************************************************/

#ifndef RRT_H_
#define RRT_H_

#include <vector>

#include "MapSearch.h"

namespace SearchAlgorithms{
using namespace std;

class RRTSearch: public MapSearch{
public:
	struct Line{
		Line(Point<int> &s,Point<int> &e):start(s),end(e){}

		Point<int> start;
		Point<int> end;
	};
	RRTSearch(): MaxSearchTime(1000),Epsilon(10){}

	void demo(size_t width,size_t height,size_t searhTime,size_t epsilon);

	bool search( const vector<vector<unsigned int> > &map,
				 vector<Point_uint> &route);
	bool search( const vector<vector<unsigned int> > &map,
				 const Point<unsigned int> &start,
				 const Point<unsigned int> &target,
				 vector<Point_uint> &route);

	size_t getLineSize() const { return Lines.size();}
	Line getLine(size_t i) const {return Lines[i];}
	Line getLineAt(size_t i) const { return Lines.at(i);}
private:
	vector<Point<int> > Nodes;
	size_t MaxSearchTime;
	unsigned int Epsilon;

	vector<Line> Lines;

	Point<int> stepFromTo(Point<int> &a,Point<int> &b);

	void generateRoute(vector<Point<int> > &route);

};

}// End of namespace SearchAlgorithms

#endif /* RRT_H_ */
