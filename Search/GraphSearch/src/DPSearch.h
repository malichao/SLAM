/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 30,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	Dynamic Programming Searching Algorithm.
*****************************************************************************/

#ifndef DPSEARCH_H_
#define DPSEARCH_H_

#include "MapSearch.h"

namespace SearchAlgorithms{
using namespace std;

class DPSearch: public MapSearch{
public:
	struct DPPoint{
		Point<unsigned int> point;
		int cost;
		DPPoint(const Point<unsigned int> &p,unsigned int cost):
			point(p),cost(cost){};
	};

	bool search( const vector<vector<unsigned int> > &map);
	bool search( const vector<vector<unsigned int> > &map,
				 const Point<unsigned int> &start,
				 const Point<unsigned int> &target);

	void printGradientOnMap(const vector<vector<unsigned int> > &map);
private:
	void generateRoute(vector<Point_uint> &route);
};

}// End of namespace SearchAlgorithms



#endif /* DPSEARCH_H_ */
