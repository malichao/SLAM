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
#include "..\src\MapPrinter.h"

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

	// route won't be clear in search function,must do it manually
	vector<Point_uint> route;
	MapPrinter printer;

	SECTION("Null input test"){
		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,0},Point_uint {0,0},route)== true );

		route.clear();
		REQUIRE( tester.search(map,Point_uint {4,4},Point_uint {4,4},route)== true );
	}

	SECTION("Out of range test"){
		route.clear();
		REQUIRE( tester.search(map,Point_uint {10,0},Point_uint {0,0},route)== false );

		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,0},Point_uint {0,10},route)== false );
	}

	SECTION("Common successful cases test(Search & print on map)"){
		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,0},Point_uint {4,4},route)== true );
		printer.printRouteOnMap(map,route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {4,4},Point_uint {0,0},route)== true );
		printer.printRouteOnMap(map,route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,2},Point_uint {4,3},route)== true );
		printer.printRouteOnMap(map,route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {4,0},Point_uint {0,4},route)== true );
		printer.printRouteOnMap(map,route);
	}

	SECTION("Common successful cases test(Search & print routes)"){
		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,0},Point_uint {4,4},route)== true );
		printer.printRoute(route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {4,4},Point_uint {0,0},route)== true );
		printer.printRoute(route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,2},Point_uint {4,3},route)== true );
		printer.printRoute(route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {4,0},Point_uint {0,4},route)== true );
		printer.printRoute(route);
	}

	SECTION("Common failure cases test(obstacle grids as setpoint)"){
		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,1},Point_uint {4,4},route)== false );
		printer.printRouteOnMap(map,route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,1},Point_uint {0,3},route)== false );
		printer.printRouteOnMap(map,route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {4,0},Point_uint {4,1},route)== false );
		printer.printRoute(route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,4},Point_uint {4,1},route)== false );
		printer.printRoute(route);
	}

	// Build a map with a wall in the middle
	map = { {1, 0, 0, 1, 1},
			{1, 1, 0, 1, 1},
			{1, 1, 0, 1, 1},
			{1, 1, 0, 3, 1},
			{1, 0, 0, 5, 1},};

	SECTION("Common failure cases test(no available route)"){
		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,0},Point_uint {4,4},route)== false );
		printer.printRouteOnMap(map,route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {4,4},Point_uint {0,0},route)== false );
		printer.printRouteOnMap(map,route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {2,0},Point_uint {2,4},route)== false );
		printer.printRoute(route);

		route.clear();
		REQUIRE( tester.search(map,Point_uint {0,4},Point_uint {4,0},route)== false );
		printer.printRoute(route);
	}

}

