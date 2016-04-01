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

#include "DPSearch.h"

namespace SearchAlgorithms{

class MapPrinter:public MapSearch{
public:

	void printRoute(const vector<Point_uint> &route);
	void printRouteOnMap(const vector<vector<unsigned int> > &map,
						 const vector<Point_uint> &route);
	void printGradientOnMap(const vector<vector<unsigned int> > &map);

private:

	static const char DirSymbol[5];

};

}// End of namespace SearchAlgorithms

#endif /* MAPPRINTER_H_ */
