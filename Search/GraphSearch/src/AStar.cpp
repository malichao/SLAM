/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 30,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	A* Searching Algorithm.
*****************************************************************************/
#include <vector>
#include <queue>
#include "AStar.h"
using namespace std;

namespace SearchAlgorithms{



//Comparison object to be used to order the heap in A* search
struct lowestF{
	bool operator()(const AStar::AStarPoint &lhs,
					const AStar::AStarPoint &rhs) const {
		return lhs.f>rhs.f;		//minimal priority queue
	}
};

bool AStar::search(const vector<vector<unsigned int> > &map,
				   const Point<unsigned int> &start,
				   const Point<unsigned int> &target,
				   vector<Point_uint> &route) {
	setTarget(target);
	setStart(start);
	return search(map,route);
}

/*
 * Basic idea:
 * -Search from target towards start grid
 * -Check every surrounding grid,calculate their values and put them in the
 *  minimal priority queue
 * -Set these appended grid pointing to its previous grid
 * -Get the minimal cost grid from the priority queue and iterate
 * -When searching is done,iterate the direction vector to generate the route
 */
bool AStar::search(const vector<vector<unsigned int> > &map,vector<Point_uint> &route) {
	MapSearch::initData(map);	//Clear previous search result and init the data

	if (!isLegal(map, Target) || !isLegal(map, Start)) return false;
	if(Start==Target) return true;

	//Similar to BFS,but we use minimal priority queue instead
	priority_queue<AStarPoint, vector<AStarPoint>, lowestF> pQue;
	pQue.push(AStarPoint(Target, map[Target.x][Target.y], pnt::abs(Target, Start)));
	Checked[Target.x][Target.y] = true;
	bool success = false;

	while (!pQue.empty()) {
		AStarPoint curPos = pQue.top();
		Point<unsigned int> p = curPos.point;
		pQue.pop();
		Checked[p.x][p.y] = true;
		if (p == Start) {
			success = true;
			break;
		}
		unsigned int g=curPos.g;

		//Append available surrounding grid to the pQue
		for(size_t k=0;k<4;k++){	//Iterate Up,Down,Left,Right 4 direction
		  Point<unsigned int> tempP(p.x+Move[k][0],p.y+Move[k][1]);
		  if(isLegal(map,tempP)){
			  pQue.push(AStarPoint(	tempP,						//Point(x,y)
					  	  	  	  	pnt::abs(tempP, Target),			//heuristic value
			  						map[tempP.x][tempP.y]+g));	//grid cost
			  Gradient[tempP.x][tempP.y] = p;
		  }
		}
	}

	if(success){
		generateRoute(route);
		return true;
	}
	return false;
}
}// End of namespace SearchAlgorithms
