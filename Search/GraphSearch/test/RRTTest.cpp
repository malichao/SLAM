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
#include "smooth.h"

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
		Point_uint start(l.start),end(l.end);
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
// Notice the bitmap_image always takes width parameter first!!!Not height!
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

void test3(){
	string fileName("search_test1.bmp");
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

	image_drawer draw(image);

	RRTSearch tester;
	Point_uint start(85,70),target(500,850);
	vector<Point_uint> route;
	tester.search(map,start,target,route);

	// Look at the parameter of draw.circle(),first time it's (x,y),second time (y,x)
	// Pretty sure it's the bitmap_image library bug.
	cout<<"Search Effort: "<<tester.getSearchEffort()<<endl;
	cout<<"printing on image\n";
	draw.pen_width(3);
	draw.pen_color(255,0,0);
	draw.circle(start.y,start.x,6);
	draw.pen_color(255,127,39);
	draw.circle(target.y,target.x,6);

	draw.pen_width(1);
	draw.pen_color(0,162,232);
	for(size_t i=0;i<tester.getLineSize();i++){
		RRTSearch::Line l=tester.getLine(i);
		Point_uint start(l.start),end(l.end);
		//printf("(%u,%u)->(%u,%u)\n",start.x,start.y,end.x,end.y);
		draw.line_segment(start.y,start.x,end.y,end.x);
	}

	if(!route.empty()){
		draw.pen_width(2);
		draw.pen_color(163,73,164);
		Point_uint prev=route[0];
		for(size_t i=1;i<route.size();i++){
			draw.line_segment(prev.y,prev.x,route[i].y,route[i].x);
			prev=route[i];
		}

		vector<Point<float> > smoothRoute;
		for(auto r:route){
			Point<float> p(r.x,r.y,r.dir);
			for(size_t i=0;i<10;i++)
				smoothRoute.push_back(p);
		}
		Smooth sm;
		sm.set(0.05,0.48,0.1,1);
		sm.smooth(smoothRoute,1);
		cout<<"========smooth end========\n";

		draw.pen_width(3);
		draw.pen_color(34,177,76);
		prev=Point_uint(smoothRoute[0].x,smoothRoute[0].y);
		for(size_t i=1;i<smoothRoute.size();i++){
			draw.line_segment(prev.y,prev.x,smoothRoute[i].y,smoothRoute[i].x);
			prev=Point_uint(smoothRoute[i].x,smoothRoute[i].y);
			printf("(%.1f,%.1f)\n",smoothRoute[i].x,smoothRoute[i].y);
		}
	}


	image.save_image("search_test1-result.bmp");
	cout<<"image saved\n";
}

int main(){
	test3();
}
