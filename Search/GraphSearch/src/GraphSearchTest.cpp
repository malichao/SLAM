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
		REQUIRE( gs.aStar( 	map,actionCost,
							Point<unsigned int>{0,0},
							Point<unsigned int>{0,0})
							== false );
		REQUIRE( gs.aStar(	map,actionCost,
							Point<unsigned int>{2,2},
							Point<unsigned int>{2,2})
							== false );
	}

	SECTION("Out of range test"){
		REQUIRE( gs.aStar(	map,actionCost,
							Point<unsigned int>{10,0},
							Point<unsigned int>{2,2})
							== false );
		REQUIRE( gs.aStar(	map,actionCost,
							Point<unsigned int>{4,4},
							Point<unsigned int>{0,10})
							== false );
	}

	SECTION("Common cases test"){
		REQUIRE( gs.aStar(	map,actionCost,
							Point<unsigned int>{0,0},
							Point<unsigned int>{4,4})
							== false );
		REQUIRE( gs.aStar(	map,actionCost,
							Point<unsigned int>{0,0},
							Point<unsigned int>{4,4})
							== false );
		REQUIRE( gs.aStar(	map,actionCost,
							Point<unsigned int>{0,0},
							Point<unsigned int>{4,4})
							== false );
	}

}
