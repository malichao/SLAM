/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 30,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	Breadth First Search.
*****************************************************************************/

#ifndef BFS_H_
#define BFS_H_

#include <vector>
#include "MapSearch.h"

namespace SearchAlgorithms{
using namespace std;

class BFS: public MapSearch{
public:
	bool search( const vector<vector<unsigned int> > &map);
	bool search( const vector<vector<unsigned int> > &map,
				 const Point<unsigned int> &start,
				 const Point<unsigned int> &target);

};

}// End of namespace SearchAlgorithms



#endif /* BFS_H_ */
