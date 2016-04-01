/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 31,2016
Version		: v0.1
Built		: Eclipse CDT, -std=c++11
Description :
	A class to print out the search result on screen
*****************************************************************************/

#include <queue>
#include <algorithm> // std::reverse()
#include <iostream>
#include <stdio.h>
#include "MapPrinter.h"

namespace SearchAlgorithms{


const char MapPrinter::DirSymbol[5] = { '^', '>', 'V', '<', ' ' };

void MapPrinter::printRoute() {
	printf("Start :(%u,%u)\n",Start.x,Start.y);
	printf("Target:(%u,%u)\n",Target.x,Target.y);
	if (Route.size() == 0) {
		cout << "No route.\n\n";
		return;
	}
	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << Route.size() << endl;
	size_t i=0;
	for (auto r : Route) {
		cout << "(" << r.x << "," << r.y << ")" << "-> ";
		if(++i%10==0) cout<<endl;	// Print on next line
	}
	cout<<"T"<<endl<<endl;
}

void MapPrinter::printRouteOnMap(const vector<vector<unsigned int> > &map) {
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

	char dirSymbol[3][3] = {
			{ ' ', '^', ' ' },
			{ '<', ' ', '>' },
			{ ' ', 'V',' ' }, };

	//TODO: the four-line judging code could be replaced by string function
	for (size_t i = 0; i + 1 < Route.size(); i++) {
		int dX = Route[i + 1].x - Route[i].x;				//dX = -1 or 1
		int dY = Route[i + 1].y - Route[i].y;				//dY = -1 or 1
		if (charMap[Route[i].x][Route[i].y] == '^' ||		//This predicate is to solve the route overlap problem,
			charMap[Route[i].x][Route[i].y] == '>' ||		//if this grid is already a route,then we use '+' to
			charMap[Route[i].x][Route[i].y] == 'V' ||		//indicate overlap
			charMap[Route[i].x][Route[i].y] == 'V')

			charMap[Route[i].x][Route[i].y] = '+';			// Set to '+'
		else
			charMap[Route[i].x][Route[i].y] = dirSymbol[1 + dX][1 + dY];
	}

	//charMap[Route[0].x][Route[0].y] = 'S';	//Mark the start point
	charMap[Route[Route.size() - 1].x][Route[Route.size() - 1].y] = 'T';//And the target point

	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << Route.size() << endl;
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void MapPrinter::printGradientOnMap(const vector<vector<unsigned int> > &map) {
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
	cout << "\nShortest path:" << Route.size() << endl;
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
}

}// End of namespace SearchAlgorithms
