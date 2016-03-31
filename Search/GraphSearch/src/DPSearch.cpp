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

//Todo:rewrite this function to make it clear
//Find the minimal cost of the four direction from visited points
unsigned int DPSearch::minCost(  const vector<vector<unsigned int> > &map,
								 const Point<unsigned int> &point,
								 Point<unsigned int> &nextMove) {
	nextMove=point;	//0:Up	1:Down	2:Left	3:Right	4:None
	if(point==Target)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX otherwise it will overflow later on
	for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
		Point<unsigned int> p(point.x+Move[k][0],point.y+Move[k][1],k);

		//In the predicate,must check the bounding first.We don't use isLegal() because we're
		//going to find the minimal cost point in previously visited points
		if ((p.x>=0 && p.y>=0 && p.x < map.size() &&p.y < map[0].size())&& Checked[p.x][p.y]
					&& map[p.x][p.y] != Obstacle){
			if(CostMap[p.x][p.y] < min){
				min=CostMap[p.x][p.y];
				nextMove=p;
			}
		}
	}
	return min;
}

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

	//Searching is done,if target is found then save the route
	if (success) {
		generateRoute();
		return true;
	}
	return false;
}

//Generate a route using the direction info
void DPSearch::generateRoute() {
	if (Gradient.size() == 0)
		return;
	Point<unsigned int> curPos = Target;
	do {
		Route.push_back(curPos);
		curPos = Gradient[curPos.x][curPos.y];
	} while (curPos != Start);
	Route.push_back(Start);
	std::reverse(Route.begin(),Route.end());
}

void DPSearch::printGradientOnMap(const vector<vector<unsigned int> > &map) {
	if (CostMap.size() == 0) {
		cout << "'costMap' data needed.\n";
		return;
	}
	//the original map is unsigned int type,to print route on the map
	//we need to convert it to char
	vector<vector<char> > charMap;
	for (size_t i = 0; i < map.size(); i++) {
		vector<char> row(map[i].size());
		for (size_t j = 0; j < map[i].size(); j++) {
			row[j] = map[i][j] == 0 ? ObstacleSymbol : map[i][j] + '0';
		}

		charMap.push_back(row);
	}

	for (size_t i = 0; i < charMap.size(); i++)
		for (size_t j = 0; j < charMap[i].size(); j++) {
			if (Checked[i][j]) {
				Point<unsigned int> move;
				minCost(map, Point<unsigned int>(i, j), move);
				charMap[i][j] = DirSymbol[move.dir];
			}
		}

	//charMap[Start.x][Start.y] = 'S';	//Mark the start point
	charMap[Target.x][Target.y] = 'T';	//And the target point

	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
}

}// End of namespace SearchAlgorithms
