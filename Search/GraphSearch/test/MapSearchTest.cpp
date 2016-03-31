/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 29,2016
version		: v0.1
Description :
	Test file for testing the graph search algorithms
*****************************************************************************/

#define CATCH_CONFIG_MAIN// This tells Catch to provide a main() - only do this in one cpp file
#include "..\include\catch.hpp"

#include <vector>
#include "..\src\BFS.h"
#include "..\src\AStar.h"
#include "..\src\DPSearch.h"

using namespace std;
using namespace SearchAlgorithms;
TEST_CASE( "Testing AStar searching", "[BFS]" ) {
	vector<vector<unsigned int> > map = {
			{1, 0, 1, 1, 1},
			{1, 1, 1, 0, 1},
			{1, 1, 0, 0, 1},
			{1, 1, 2, 3, 1},
			{1, 0, 6, 5, 1},};

	//Define the cost for each action,left turn is expensive in real life
	//[0]:Left turn	[1]:Forward	[2]:Right turn
	unsigned int actionCost[]={100,0,2};

	//BFS tester;
	AStar tester;
	//TODO: Add generate route for DPSearch
	//DPSearch tester;
	vector<Point_unit> route;

	SECTION("Null input test"){
		REQUIRE( tester.search(map,Point_unit {0,0},Point_unit {0,0})== true );
		REQUIRE( tester.search(map,Point_unit {4,4},Point_unit {4,4})== true );
	}

	SECTION("Out of range test"){
		REQUIRE( tester.search(map,Point_unit {10,0},Point_unit {0,0})== false );
		REQUIRE( tester.search(map,Point_unit {0,0},Point_unit {0,10})== false );
	}

	SECTION("Common successful cases test(Search & print on map)"){
		REQUIRE( tester.search(map,Point_unit {0,0},Point_unit {4,4})== true );
		tester.printRouteOnMap(map);
		REQUIRE( tester.search(map,Point_unit {4,4},Point_unit {0,0})== true );
		tester.printRouteOnMap(map);
		REQUIRE( tester.search(map,Point_unit {0,2},Point_unit {4,3})== true );
		tester.printRouteOnMap(map);
		REQUIRE( tester.search(map,Point_unit {4,0},Point_unit {0,4})== true );
		tester.printRouteOnMap(map);
	}

	SECTION("Common successful cases test(Search & print routes)"){
		REQUIRE( tester.search(map,Point_unit {0,0},Point_unit {4,4})== true );
		tester.printRoute();
		REQUIRE( tester.search(map,Point_unit {4,4},Point_unit {0,0})== true );
		tester.printRoute();
		REQUIRE( tester.search(map,Point_unit {0,2},Point_unit {4,3})== true );
		tester.printRoute();
		REQUIRE( tester.search(map,Point_unit {4,0},Point_unit {0,4})== true );
		tester.printRoute();
	}

	SECTION("Common failure cases test(obstacle grids as setpoint)"){
		REQUIRE( tester.search(map,Point_unit {0,1},Point_unit {4,4})== false );
		tester.printRouteOnMap(map);
		REQUIRE( tester.search(map,Point_unit {0,1},Point_unit {0,3})== false );
		tester.printRouteOnMap(map);
		REQUIRE( tester.search(map,Point_unit {4,0},Point_unit {4,1})== false );
		tester.printRoute();
		REQUIRE( tester.search(map,Point_unit {0,4},Point_unit {4,1})== false );
		tester.printRoute();
	}

	// Build a map with a wall in the middle
	map = { {1, 0, 0, 1, 1},
			{1, 1, 0, 1, 1},
			{1, 1, 0, 1, 1},
			{1, 1, 0, 3, 1},
			{1, 0, 0, 5, 1},};

	SECTION("Common failure cases test(no available route)"){
		REQUIRE( tester.search(map,Point_unit {0,0},Point_unit {4,4})== false );
		tester.printRouteOnMap(map);
		REQUIRE( tester.search(map,Point_unit {4,4},Point_unit {0,0})== false );
		tester.printRouteOnMap(map);
		REQUIRE( tester.search(map,Point_unit {2,0},Point_unit {2,4})== false );
		tester.printRoute();
		REQUIRE( tester.search(map,Point_unit {0,4},Point_unit {4,0})== false );
		tester.printRoute();
	}

}

