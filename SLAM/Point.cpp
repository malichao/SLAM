/*
 * Point.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */
#include "Point.h"

namespace pnt{
int abs(Point &a,Point &b){
	int x = a.x > b.x ? a.x - b.x : b.x - a.x;
	int y = a.y > b.y ? a.y - b.y : b.y - a.y;
	return x+y;
}
int dis(Point &a,Point &b){
	int x = a.x > b.x ? a.x - b.x : b.x - a.x;
	int y = a.y > b.y ? a.y - b.y : b.y - a.y;
	return x*x+y*y;
}
}
