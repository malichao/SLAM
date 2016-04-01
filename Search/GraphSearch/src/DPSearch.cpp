/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 30,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	Dynamic Programming Searching Algorithm.
*****************************************************************************/
#include <queue>
#include <algorithm> // std::reverse()
#include <iostream>
#include "DPSearch.h"

namespace SearchAlgorithms{



//Comparison object to be used to order the heap in dpSearch
struct lessCost{
	bool operator()(const DPSearch::DPPoint &lhs,
					const DPSearch::DPPoint &rhs) const{
		return lhs.cost>rhs.cost;		//minimal priority queue
	}
};

bool DPSearch::search(const vector<vector<unsigned int> > &map,
					  const Point<unsigned int> &start,
					  const Point<unsigned int> &target) {
	setTarget(target);
	setStart(start);
	return search(map);
}

bool DPSearch::search(const vector<vector<unsigned int> > &map) {
	MapSearch::initData(map);	//Clear previous search result and init the data

	if (!isLegal(map, Target) || !isLegal(map, Start)) return false;
	if(Start==Target) return true;

	CostMap.clear();
	CostMap=vector<vector<unsigned int> >
				(map.size(),vector<unsigned int> (map[0].size(),CostMax));
	Checked[Target.x][Target.y] = true;
	CostMap[Target.x][Target.y]	=0;

	//DPPoint is basically a Point<unsigned int> with a 'cost',which is used to sort in the priority queue
	priority_queue<DPPoint, vector<DPPoint>, lessCost> pQue;
	pQue.push(DPPoint(Target,0));
	Point<unsigned int> nextMove;
	bool success=false;

	while (!pQue.empty()) {
		DPPoint curPos = pQue.top();
		Point<unsigned int> p = curPos.point;
		pQue.pop();
		Checked[p.x][p.y] = true;
		if (p == Start) {
			success = true;
			break;
		}

		CostMap[p.x][p.y] = minCost(map,p,nextMove)+map[p.x][p.y];
		//Point<unsigned int> the current point to the lowest cost point
		Gradient[p.x][p.y] = nextMove;

		//Append available surrounding grid to the queue
		for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
			Point<unsigned int> tempP(p.x+Move[k][0],p.y+Move[k][1]);
			if (isLegal(map,tempP))  //move up
				pQue.push(DPPoint(tempP,CostMap[tempP.x][tempP.y]));
		}
	}

	//Test:output the cost map
	//	for(size_t i=0;i<CostMap.size();i++){
	//		for(size_t j=0;j<CostMap[i].size();j++){
	//		cout<<CostMap[i][j]<<"\t";
	//		}
	//		cout<<endl;
	//	}

	return success;
}

//Generate a route using the direction info
void DPSearch::generateRoute(vector<Point_uint> &route) {
	if (Gradient.size() == 0)
		return;
	Point<unsigned int> curPos = Target;
	do {
		route.push_back(curPos);
		curPos = Gradient[curPos.x][curPos.y];
	} while (curPos != Start);
	route.push_back(Start);
	std::reverse(route.begin(),route.end());
}



}// End of namespace SearchAlgorithms
