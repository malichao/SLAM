/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 30,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	Breadth First Search.
*****************************************************************************/
#include <queue>
#include "BFS.h"

namespace SearchAlgorithms{

bool BFS::search(const vector<vector<unsigned int> > &map,
				 const Point<unsigned int> &start,const Point<unsigned int> &target){
   setTarget(target);
   setStart(start);
   return search(map);
}

/*
 * Basic idea:
 * -Search from target towards start grid
 * -Check every surrounding grid and put available ones in the queue
 * -Set these appended grid pointing to its previous grid
 * -Read grids from the queue and check if the start grid is reached
 * -When searching is done,iterate the direction vector to generate the route
 */
bool BFS::search(const vector<vector<unsigned int> > &map) {
	MapSearch::initData(map);	//Clear previous search result and init the data

	if (!isLegal(map, Target) || !isLegal(map, Start)) return false;
	if(Start==Target) return true;

	queue<Point<unsigned int> > que;
	que.push(Target);
	Checked[Target.x][Target.y] = true;
	bool success = false;

	while (!que.empty()) {
		Point<unsigned int> curPos = que.front();
		que.pop();
		Checked[curPos.x][curPos.y] = true;
		if (curPos == Start) {
			success = true;
			break;
		}

		//Append available surrounding grid to the queue
		for (size_t k = 0; k < 4; k++) {//Iterate Up,Down,Left,Right 4 direction
			Point<unsigned int> p(curPos.x + Move[k][0], curPos.y + Move[k][1]);
			if (isLegal(map, p)) {
				que.push(p);
				Gradient[p.x][p.y] = curPos;
			}
		}
	}

	return success;
}

}// End of namespace SearchAlgorithms
