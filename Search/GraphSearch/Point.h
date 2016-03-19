/*
 * Point.h
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */

#ifndef POINT_H_
#define POINT_H_


struct Point {
   //Must define these values in clockwise/counter-clockwise manner
   const unsigned int DirectionSize=4;
   const unsigned int Up=0;
   const unsigned int Right=1;
   const unsigned int Down=2;
   const unsigned int Left=3;

   unsigned int x;
   unsigned int y;
   unsigned int dir;	//0:Up	1:Down 2:Left	2:Right

   Point():x(0),y(0),dir(Up){};
   Point(unsigned int x,unsigned int y):x(x),y(y),dir(Up){};
   Point(unsigned int x,unsigned int y,unsigned int dir):x(x),y(y),dir(dir){};
   Point(const Point &p):x(p.x),y(p.y),dir(p.dir){};
   void set(const unsigned int x,const unsigned int y,const unsigned int dir){
	   this->x=x;
	   this->y=y;
	   this->dir=dir;
   }
   Point& operator=(const Point &rhs){
	   x=rhs.x;
	   y=rhs.y;
	   dir=rhs.dir;
	   return *this;
   }

   bool operator== (const Point &rhs) const{
      return (x==rhs.x&&y==rhs.y);		//Different orientation considered as the same
   }
   bool operator!= (const Point &rhs){
		return (x!=rhs.x||y!=rhs.y);	//Different orientation considered as the same
	 }
};

namespace pnt{
unsigned int abs(Point &a,Point &b){
	unsigned int x = a.x > b.x ? a.x - b.x : b.x - a.x;
	unsigned int y = a.y > b.y ? a.y - b.y : b.y - a.y;
	return x+y;
}

unsigned int dis(Point &a,Point &b){
	unsigned int x = a.x > b.x ? a.x - b.x : b.x - a.x;
	unsigned int y = a.y > b.y ? a.y - b.y : b.y - a.y;
	return x*x+y*y;
}

}

#endif /* POINT_H_ */
