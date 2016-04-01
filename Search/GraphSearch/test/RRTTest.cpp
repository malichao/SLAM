/*
 * RRTTest.cpp
 *
 *  Created on: Apr 1, 2016
 *      Author: Lichao
 */
#include <iostream>
#include <stdio.h>
#include <string>
#include "../src/RRT.h"
#include "../include/bitmap_image.hpp"

using namespace std;
using namespace SearchAlgorithms;
int main(){
	size_t width=1000,height=1000;
	size_t penWidth=1;
	bitmap_image image(height+penWidth*2,width+penWidth*2);
	image_drawer draw(image);

	RRTSearch tester;
	tester.demo(width,height,1000);

	draw.pen_width(penWidth);
	draw.pen_color(255,255,255);
	for(size_t i=0;i<tester.getLineSize();i++){
		RRTSearch::Line l=tester.getLine(i);
		Point_uint start(l.start),end(l.end);
		printf("(%u,%u)->(%u,%u)\n",start.x,start.y,end.x,end.y);
		draw.line_segment(start.x,start.y,end.x,end.y);
	}


	string fileName("RRT_Test.bmp");
	image.save_image(fileName);
	cout<<"image saved\n";
}
