/*
 * Point.h
 *
 *  Created on: Mar 19, 2016
 *      Author: Lichao
 */

#ifndef POINT_H_
#define POINT_H_

//Define Point as template because in GraphSearch cases,map coordinate are positive
//integers.But in smooth algorithm,these points will be converted to float to make
//the path smooth.
template<class T>
struct Point {
   static const int DirectionSize=4;
   //Must define these values in clockwise/counter-clockwise manner
   static const int Up=0;
   static const int Right=1;
   static const int Down=2;
   static const int Left=3;

   T x;
   T y;
   unsigned int dir;	//0:Up	1:Right 2:Down	3:Left

   Point():x(0),y(0),dir(Up){};
   Point(T x,T y):x(x),y(y),dir(Up){};
   Point(T x,T y,unsigned int dir):x(x),y(y),dir(dir){};
   Point(const Point &p):x(p.x),y(p.y),dir(p.dir){};
   void set(const T x,const T y,const unsigned int dir){
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

   Point operator+(const Point &rhs)const{
	   return Point(x+rhs.x,y+rhs.y,(dir+rhs.dir)%DirectionSize);
   }
   Point operator-(const Point &rhs)const{
   	   return Point(x-rhs.x,y-rhs.y,(dir-rhs.dir)%DirectionSize);
    }
   Point operator*(const int rhs){
      return Point(x*rhs,y*rhs,dir);
   }
   Point operator*(const float rhs){
      return Point<float>(x*rhs,y*rhs,dir);
   }

   bool operator== (const Point &rhs) const{
      return (x==rhs.x&&y==rhs.y);		//Different orientation considered as the same
   }
   bool operator!= (const Point &rhs){
		return (x!=rhs.x||y!=rhs.y);	//Different orientation considered as the same
	 }

   bool operator< (const int rhs){
   		return (x<rhs&&y<rhs);	//Different orientation considered as the same
   	 }
   bool operator<= (const int rhs){
		return (x<=rhs&&y<=rhs);	//Different orientation considered as the same
	 }
   bool operator> (const int rhs){
      	return (x>rhs&&y>rhs);	//Different orientation considered as the same
     }
   bool operator>= (const int rhs){
		return (x>=rhs&&y>=rhs);	//Different orientation considered as the same
	}

};

namespace pnt{
int abs(Point<unsigned int> &a,Point<unsigned int> &b);
int dis(Point<unsigned int> &a,Point<unsigned int> &b);

float abs(Point<float> &a,Point<float> &b);
float dis(Point<float> &a,Point<float> &b);
}

#endif /* POINT_H_ */
