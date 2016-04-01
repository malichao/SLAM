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

class MapPrinter{
public:

	void printRoute();
	void printRouteOnMap(const vector<vector<unsigned int> > &map);
	void printGradientOnMap(const vector<vector<unsigned int> > &map);

};

}// End of namespace SearchAlgorithms

#endif /* MAPPRINTER_H_ */
