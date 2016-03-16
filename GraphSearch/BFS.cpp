#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>
using namespace std;
struct Point {
   int x;
   int y;
   Point(int i,int j):x(i),y(j){};
   bool operator== (const Point rhs){
      return (x==rhs.x&&y==rhs.y);
   }
   bool operator!= (const Point rhs){
            return (x!=rhs.x||y!=rhs.y);
         }
};
class BFS {
public:
   const int Wall =0;	//positive value means the cost of each path
   BFS():start(0,0),target(0,0){};
   bool bfs(const vector<vector<int> > &map);
   bool bfs(const vector<vector<int> > &map,const Point &start,const Point &target);
   void setStart(const Point &s){start.x=s.x; start.y=s.y;}
   void setTarget(const Point &s){target.x=s.x; target.y=s.y;}
   int getMinDistance(){return route.size()-1;}
   void printRoute();
   void printRouteOnMap(const vector<vector<int> > &m);
private:
   Point start;
   Point target;
   vector<vector<bool> > checked;
   vector<vector<Point> > direction;
   vector<Point> route;
   bool isLegal(const vector<vector<int> > &map,Point p);
};

void BFS::printRoute(){
   for(auto r :route){
      cout<<"("<<r.x<<","<<r.y<<")"<<endl;
   }
}

void BFS::printRouteOnMap(const vector<vector<int> > &m){
   vector<vector<char> > map;
   for(int i=0;i<m.size();i++){
      vector<char> row(m[i].size());
      for(int j=0;j<m[i].size();j++){
         row[j]=m[i][j]+'0';
      }
      map.push_back(row);
   }

   for(int i=1;i<route.size()-1;i++){
      int dX=route[i+1].x-route[i].x;
      int dY=route[i+1].y-route[i].y;
      if(dX==1)
         map[route[i].x][route[i].y]='v';
      else if(dX==-1)
         map[route[i].x][route[i].y]='^';
      else if(dY==1)
         map[route[i].x][route[i].y]='>';
      else if(dX==-1)
         map[route[i].x][route[i].y]='<';
   }

   map[route[0].x][route[0].y]='S';
   map[route[route.size()-1].x][route[route.size()-1].y]='T';

   for(int i=0;i<map.size();i++){
      for(int j=0;j<map[i].size();j++){
         cout<<map[i][j]<<" ";
      }
      cout<<endl;
   }
}

bool BFS::bfs(const vector<vector<int> > &map,const Point &start,const Point &target){
   setTarget(target);
   setStart(start);
   return this->bfs(map);
}

bool BFS::isLegal(const vector<vector<int> > &map,Point p){
   if(p.x<0||p.y<0||p.x>=map.size()||p.y>=map[0].size())
      return false;
   if(checked[p.x][p.y])   	//Current position has been searched
      return false;
   if(map[p.x][p.y]==Wall)
      return false;
   return true;
}

bool BFS::bfs(const vector<vector<int> > &map){
   queue<Point> q;

   for(int i=0;i<map.size();i++){
      vector<Point> zero(map[i].size(),Point(0,0));
      direction.push_back(zero);
   }

   for(int i=0;i<map.size();i++){
      vector<bool> temp(map[i].size(),false);
      checked.push_back(temp);
   }

   if(!isLegal(map,target)||!isLegal(map,start))
      return true;

   q.push(target);
   checked[target.x][target.y]=true;
   bool success=false;
   while(!q.empty()){
      Point curPos=q.front();
      q.pop();
      checked[curPos.x][curPos.y]=true;
      if(curPos==start){
         success=true;
         break;
      }
      if(isLegal(map,Point(curPos.x-1,curPos.y))){ //move up
         q.push(Point(curPos.x-1,curPos.y));
         direction[curPos.x-1][curPos.y]=curPos;
      }
      if(isLegal(map,Point(curPos.x+1,curPos.y))){ //move down
         q.push(Point(curPos.x+1,curPos.y));
         direction[curPos.x+1][curPos.y]=curPos;
      }
      if(isLegal(map,Point(curPos.x,curPos.y-1))){ //move left
         q.push(Point(curPos.x,curPos.y-1));
         direction[curPos.x][curPos.y-1]=curPos;
      }
      if(isLegal(map,Point(curPos.x,curPos.y+1))){ //move right
         q.push(Point(curPos.x,curPos.y+1));
         direction[curPos.x][curPos.y+1]=curPos;
      }
   }
   if(success){
      Point curPos=start;
      do{
         route.push_back(curPos);
         curPos=direction[curPos.x][curPos.y];
      }while(curPos!=target);
      route.push_back(target);
      return true;
   }
   return false;
}

const int MapSize = 5;
/*
 * About the map:
 *  0 means the wall,positive values means the cost each grid.Although
 *  in BFS these values are used but to make a comparison between different
 *  algorithms,such as A*,Dijkstra shortest path,we need a map that works
 *  for all these algorithms.
 *
 *  Notice about the map coordinate:
 *  map[x][y]:
 *  0-------> y
 *  |
 *  |
 *  v
 *  x
 */
vector<vector<int> > map = {
	      {1, 0, 1, 1, 1},
	      {1, 0, 1, 0, 1},
	      {1, 1, 1, 0, 1},
	      {1, 0, 0, 0, 1},
	      {1, 1, 1, 0, 1} };
/*
 * Test result:
   S 1 > > v
   v 1 ^ 1 v
   > > ^ 1 v
   0 1 1 1 v
   0 0 0 1 T
 */
int main(void) {
   Point start(0,0),target(4,4);
   BFS b;
   if(b.bfs(map,start,target)){
      b.printRouteOnMap(map);
   }
   else{
      cout<<"BFS failed.\n";
   }
   return 0;
}
