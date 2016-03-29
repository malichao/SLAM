/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 29,2016
version		: v0.1
Description :
	Test file for testing the graph search algorithms
*****************************************************************************/

#define CATCH_CONFIG_MAIN// This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <vector>
#include "GraphSearch.h"

TEST_CASE( "Testing AStar searching", "[AStar]" ) {
	vector<vector<unsigned int> > map = {
			{1, 0, 1, 1, 1},
			{1, 1, 1, 0, 1},
			{1, 1, 0, 0, 1},
			{1, 1, 2, 3, 1},
			{1, 0, 6, 5, 1},};

	//Define the cost for each action,left turn is expensive in real life
	//[0]:Left turn	[1]:Forward	[2]:Right turn
	unsigned int actionCost[]={100,0,2};

	GraphSearch gs;
	vector<Point<unsigned int> > route;

	SECTION("Null input test"){
		Point<unsigned int> start(0,0),target(0,0);
		REQUIRE(gs.aStar(map,actionCost,start,target)==false);
	}

	SECTION("Null input test"){
		Point<unsigned int> start(2,2),target(2,2);
		REQUIRE(gs.aStar(map,actionCost,start,target)==false);
	}

	SECTION("Out of range test"){
		Point<unsigned int> start(0,0),target(10,0);
		REQUIRE(gs.aStar(map,actionCost,start,target)==false);
	}

	SECTION("Out of range test"){
		Point<unsigned int> start(0,0),target(0,10);
		REQUIRE(gs.aStar(map,actionCost,start,target)==false);
	}

	SECTION("Common case test"){
		Point<unsigned int> start(0,0),target(4,4);
		REQUIRE(gs.aStar(map,actionCost,start,target)==true);
	}

	SECTION("Common case test"){
		Point<unsigned int> start(4,4),target(0,0);
		REQUIRE(gs.aStar(map,actionCost,start,target)==true);
	}

}
