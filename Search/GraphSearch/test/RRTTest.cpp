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

// Test RTT demo function
void test1(){
	size_t width=1000,height=1000;
	size_t penWidth=1;
	bitmap_image image(height,width);
	image_drawer draw(image);

	RRTSearch tester;
	tester.demo(width,height,500,500);

	draw.pen_width(penWidth);
	draw.pen_color(255,255,255);
	for(size_t i=0;i<tester.getLineSize();i++){
		RRTSearch::Line l=tester.getLine(i);
		Point<int> start(l.start),end(l.end);
		//printf("(%u,%u)->(%u,%u)\n",start.x,start.y,end.x,end.y);
		draw.line_segment(start.x,start.y,end.x,end.y);
	}


	string fileName("RRT_Test.bmp");
	image.save_image(fileName);
	cout<<"image saved\n";
}

// Test Reading image and extract grids info.
// Convert white(255,255,255) pixels to true,which means free zone,
// and black(0,0,0) to false,which means obstacles.
void test2(){
	string fileName("reading_test1.bmp");
	bitmap_image image(fileName);

	if(!image){
		return;
	}
	vector<vector<bool> > map(image.height(),vector<bool>(image.width(),true));
	unsigned char r,g,b;
	for(size_t i=0;i<image.height();i++)
		for(size_t j=0;j<image.width();j++){
			image.get_pixel(j,i,r,g,b);	//black 0 white 255
			map[i][j]= r+g+b==255*3 ? true:false;
		}

	bitmap_image image1(image.width(),image.height());
	for(size_t i=0;i<image.height();i++)
		for(size_t j=0;j<image.width();j++){
			if(map[i][j])
				image1.set_pixel(j,i,255,255,255);
			else
				image1.set_pixel(j,i,0,0,0);
		}
	image1.save_image("reading_test1-result.bmp");
}

int main(){
	test1();
}
