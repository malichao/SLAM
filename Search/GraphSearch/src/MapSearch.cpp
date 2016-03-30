/******************************************************************************
 Author		: Lichao Ma
 Date  		: Mar 29,2016
 version		: v0.1
 Description :
 Map searching algorithms.
 *****************************************************************************/
#include "iostream"
#include "stdio.h"
#include "MapSearch.h"

namespace SearchAlgorithms {

using namespace std;

const unsigned int MapSearch::CostMax = 999; //Don't use UINT_MAX,watch out for OVERFLOW
const unsigned int MapSearch::Obstacle = 0;	//positive value means the cost of each path
const char MapSearch::ObstacleSymbol = '#';

//const unsigned int NoneDirection = 4;
const int MapSearch::Move[5][2] = {
		{ -1, 0 },	// x-1,y
		{ 0, 1 },	// x,y+1
		{ 1, 0 },	// x+1,y
		{ 0, -1 },	// x,y-1
		{ 0, 0 } };	// x,y
const char MapSearch::DirSymbol[5] = { '^', '>', 'V', '<', ' ' };

void MapSearch::getRoute(vector<Point<unsigned int> > &route) {
	if (Route.size() == 0)
		return;
	route = Route;
}

void MapSearch::printRoute() {
	printf("Start :(%u,%u)\n",Start.x,Start.y);
	printf("Target:(%u,%u)\n",Target.x,Target.y);
	if (Route.size() == 0) {
		cout << "No route.\n\n";
		return;
	}
	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << getMinDistance() << endl;
	size_t i=0;
	for (auto r : Route) {
		cout << "(" << r.x << "," << r.y << ")" << endl;
		if(i++%5==0) cout<<endl;	// Print on next line
	}
}

void MapSearch::printRouteOnMap(const vector<vector<unsigned int> > &map) {
	printf("Start :(%u,%u)\n",Start.x,Start.y);
	printf("Target:(%u,%u)\n",Target.x,Target.y);
	if (Route.size() == 0) {
		cout << "No route!\n\n";
		return;
	}
	//the original map is unsigned int type,to print route on the map
	//we need to convert it to char
	vector<vector<char> > charMap;
	for (size_t i = 0; i < map.size(); i++) {
		vector<char> row(map[i].size());
		for (size_t j = 0; j < map[i].size(); j++) {
			row[j] = map[i][j] == 0 ? ObstacleSymbol : '.';
		}
		charMap.push_back(row);
	}

	char dirSymbol[3][3] = { { ' ', '^', ' ' }, { '<', ' ', '>' }, { ' ', 'V',
			' ' }, };

	for (size_t i = 0; i + 1 < Route.size(); i++) {
		int dX = Route[i + 1].x - Route[i].x;				//dX = -1 or 1
		int dY = Route[i + 1].y - Route[i].y;				//dY = -1 or 1
		if (charMap[Route[i].x][Route[i].y] == '^' ||		//This predicate is to solve the route overlap problem,
				charMap[Route[i].x][Route[i].y] == '>' ||	//if this grid is already a route,then we use '+' to
				charMap[Route[i].x][Route[i].y] == 'V' ||	//indicate overlap
				charMap[Route[i].x][Route[i].y] == 'V')
			charMap[Route[i].x][Route[i].y] = '+';
		else
			charMap[Route[i].x][Route[i].y] = dirSymbol[1 + dX][1 + dY];
	}

	//charMap[Route[0].x][Route[0].y] = 'S';	//Mark the start point
	charMap[Route[Route.size() - 1].x][Route[Route.size() - 1].y] = 'T';//And the target point

	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void MapSearch::printGradientOnMap(const vector<vector<unsigned int> > &map) {
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

//Generate a route using the direction info
void MapSearch::generateRoute() {
	if (Gradient.size() == 0)
		return;
	Point<unsigned int> curPos = Start;
	do {
		Route.push_back(curPos);
		curPos = Gradient[curPos.x][curPos.y];
	} while (curPos != Target);
	Route.push_back(Target);
}

bool MapSearch::isLegal(const vector<vector<unsigned int> > &map,
		const Point<unsigned int> &p) {
	EffortCount++;
	if (p.x >= map.size() || p.y >= map[0].size())	//x and y are unsigned int
		return false;
	if (Checked[p.x][p.y])   	//Current position has been searched
		return false;
	if (map[p.x][p.y] == Obstacle)		//Don't hit the wall
		return false;
	return true;
}

}		//End namespace SearchAlgorithms

