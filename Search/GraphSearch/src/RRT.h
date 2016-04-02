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
		Line(Point_uint &s,Point_uint &e):start(s),end(e){}

		Point_uint start;
		Point_uint end;
	};
	RRTSearch(): MaxSearchTime(1000),Epsilon(10){}

	void demo(size_t width,size_t height,size_t searchTime,size_t epsilon);

	bool search( const vector<vector<bool> > &map,
				 vector<Point_uint> &route);
	bool search( const vector<vector<bool> > &map,
				 const Point<unsigned int> &start,
				 const Point<unsigned int> &target,
				 vector<Point_uint> &route);

	size_t getLineSize() const { return Lines.size();}
	Line getLine(size_t i) const {return Lines[i];}
	Line getLineAt(size_t i) const { return Lines.at(i);}
private:
	vector<Point_uint > Nodes;
	size_t MaxSearchTime;
	unsigned int Epsilon;

	vector<Line> Lines;

	Point_uint stepFromTo(Point_uint &a,Point_uint &b);
	Point_uint randomConfig(const vector<vector<bool> > &map);

	void generateRoute(vector<Point_uint > &route);

};

}// End of namespace SearchAlgorithms

#endif /* RRT_H_ */
