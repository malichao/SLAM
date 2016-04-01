/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 31,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	A class to print out the search result on screen
*****************************************************************************/


#ifndef MAPPRINTER_H_
#define MAPPRINTER_H_

#include <queue>
#include <algorithm> // std::reverse()
#include <iostream>
#include "DPSearch.h"

namespace SearchAlgorithms{

class MapPrinter:public MapSearch{
public:

	void printRoute();
	void printRouteOnMap(const vector<vector<unsigned int> > &map);
	void printGradientOnMap(const vector<vector<unsigned int> > &map);

	void getRoute(vector<Point<unsigned int> > &route) {
		if (Route.size() == 0)
			return;
		route = Route;
	}

private:
	//const unsigned int NoneDirection = 4;

	static const char DirSymbol[5];

	vector<Point_uint > Route;

};

}// End of namespace SearchAlgorithms

#endif /* MAPPRINTER_H_ */
