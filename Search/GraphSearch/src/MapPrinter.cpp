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

void MapPrinter::printRoute(const MapSearch &searcher,
							const vector<Point_uint> &route) {
	printf("Start :(%u,%u)\n",searcher.getStart().x,searcher.getStart().y);
	printf("Target:(%u,%u)\n",searcher.getTarget().x,searcher.getTarget().y);
	if (route.size() == 0) {
		cout << "No route.\n\n";
		return;
	}
	cout << "Search effort: " << searcher.getSearchEffort();
	cout << "\nShortest path:" << route.size() << endl;
	size_t i=0;
	for (auto r : route) {
		cout << "(" << r.x << "," << r.y << ")" << "-> ";
		if(++i%10==0) cout<<endl;	// Print on next line
	}
	cout<<"T"<<endl<<endl;
}

void MapPrinter::printRouteOnMap(const MapSearch &searcher,
								 const vector<vector<unsigned int> > &map,
								 const vector<Point_uint> &route) {
	printf("Start :(%u,%u)\n",searcher.getStart().x,searcher.getStart().y);
	printf("Target:(%u,%u)\n",searcher.getTarget().x,searcher.getTarget().y);
	if (route.size() == 0) {
		cout << "No route!\n\n";
		return;
	}
	//the original map is unsigned int type,to print route on the map
	//we need to convert it to char
	vector<vector<char> > charMap;
	for (size_t i = 0; i < map.size(); i++) {
		vector<char> row(map[i].size());
		for (size_t j = 0; j < map[i].size(); j++) {
			row[j] = map[i][j] == 0 ? MapSearch::ObstacleSymbol : '.';
		}
		charMap.push_back(row);
	}

	char dirSymbol[3][3] = {
			{ ' ', '^', ' ' },
			{ '<', ' ', '>' },
			{ ' ', 'V', ' ' }, };

	//TODO: the four-line judging code could be replaced by string function
	for (size_t i = 0; i + 1 < route.size(); i++) {
		int dX = route[i + 1].x - route[i].x;				//dX = -1 or 1
		int dY = route[i + 1].y - route[i].y;				//dY = -1 or 1
		if (charMap[route[i].x][route[i].y] == '^' ||		//This predicate is to solve the route overlap problem,
			charMap[route[i].x][route[i].y] == '>' ||		//if this grid is already a route,then we use '+' to
			charMap[route[i].x][route[i].y] == 'V' ||		//indicate overlap
			charMap[route[i].x][route[i].y] == 'V')

			charMap[route[i].x][route[i].y] = '+';			// Set to '+'
		else
			charMap[route[i].x][route[i].y] = dirSymbol[1 + dX][1 + dY];
	}

	//charMap[route[0].x][route[0].y] = 'S';	//Mark the start point
	charMap[route[route.size() - 1].x][route[route.size() - 1].y] = 'T';//And the target point

	cout << "Search effort: " << searcher.getSearchEffort();
	cout << "\nShortest path:" << route.size() << endl;
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void MapPrinter::printGradientOnMap(const MapSearch &searcher,
									const vector<vector<unsigned int> > &map) {
	if (searcher.getCostMapSize() == 0) {
		cout << "'costMap' data needed.\n";
		return;
	}
	//the original map is unsigned int type,to print route on the map
	//we need to convert it to char
	vector<vector<char> > charMap;
	for (size_t i = 0; i < map.size(); i++) {
		vector<char> row(map[i].size());
		for (size_t j = 0; j < map[i].size(); j++) {
			row[j] = map[i][j] == 0 ? MapSearch::ObstacleSymbol : map[i][j] + '0';
		}

		charMap.push_back(row);
	}

	for (size_t i = 0; i < charMap.size(); i++)
		for (size_t j = 0; j < charMap[i].size(); j++) {
			if (searcher.isChecked(i,j)) {
				Point<unsigned int> move;
				searcher.minCost(map,Point<unsigned int> (i,j),move);
				charMap[i][j] = DirSymbol[move.dir];
			}
		}

	//charMap[Start.x][Start.y] = 'S';	//Mark the start point
	charMap[searcher.getTarget().x][searcher.getTarget().y] = 'T';	//And the target point

	cout << "Search effort: " << searcher.getSearchEffort();
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
}

}// End of namespace SearchAlgorithms
