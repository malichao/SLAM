#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>
using namespace std;
struct Point {
   unsigned int x;
   unsigned int y;
   Point(int i,int j):x(i),y(j){};
   bool operator== (const Point rhs){
      return (x==rhs.x&&y==rhs.y);
   }
   bool operator!= (const Point rhs){
            return (x!=rhs.x||y!=rhs.y);
         }
};
class Search {
public:
   const int Wall =0;	//positive value means the cost of each path
   Search():start(0,0),target(0,0),steps(0){};
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
   unsigned int steps;
   vector<vector<bool> > checked;
   vector<vector<Point> > direction;
   vector<Point> route;
   bool isLegal(const vector<vector<int> > &map,Point p);
};

void Search::printRoute() {
	if (route.size() == 0) {
		cout << "No route.\n";
		return;
	}
	cout << "Search effort: " << steps;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (auto r : route) {
		cout << "(" << r.x << "," << r.y << ")" << endl;
	}
}

void Search::printRouteOnMap(const vector<vector<int> > &m) {
	if (route.size() == 0) {
		cout << "No route.\n";
		return;
	}
	vector<vector<char> > map;
	for (size_t i = 0; i < m.size(); i++) {
		vector<char> row(m[i].size());
		for (size_t j = 0; j < m[i].size(); j++) {
			row[j] = m[i][j] + '0';
		}
		map.push_back(row);
	}

	for (size_t i = 1; i + 1 < route.size(); i++) {
		int dX = route[i + 1].x - route[i].x;
		int dY = route[i + 1].y - route[i].y;
		if (dX == 1)
			map[route[i].x][route[i].y] = 'v';
		else if (dX == -1)
			map[route[i].x][route[i].y] = '^';
		else if (dY == 1)
			map[route[i].x][route[i].y] = '>';
		else if (dX == -1)
			map[route[i].x][route[i].y] = '<';
	}

	map[route[0].x][route[0].y] = 'S';
	map[route[route.size() - 1].x][route[route.size() - 1].y] = 'T';

	cout << "Search effort: " << steps;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (size_t i = 0; i < map.size(); i++) {
		for (size_t j = 0; j < map[i].size(); j++) {
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
}

bool Search::bfs(const vector<vector<int> > &map,const Point &start,const Point &target){
   setTarget(target);
   setStart(start);
   return this->bfs(map);
}

bool Search::isLegal(const vector<vector<int> > &map,Point p){
	steps++;
	if(p.x>=map.size()||p.y>=map[0].size())		//x and y are unsigned int
	  return false;
	if(checked[p.x][p.y])   	//Current position has been searched
	  return false;
	if(map[p.x][p.y]==Wall)
	  return false;
	return true;
}

bool Search::bfs(const vector<vector<int> > &map){
   for(size_t i=0;i<map.size();i++){				//Init the direction map
      vector<Point> zero(map[i].size(),Point(0,0));
      direction.push_back(zero);
   }

   for(size_t i=0;i<map.size();i++){				//Init the checked set
      vector<bool> temp(map[i].size(),false);
      checked.push_back(temp);
   }

   if(!isLegal(map,target)||!isLegal(map,start))
      return false;

   queue<Point> q;
   q.push(target);
   checked[target.x][target.y]=true;
   bool success=false;
   steps=1;			//Reset the steps to count the search effort
   route.clear();	//Clear the previous route
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
 *  in Search these values are used but to make a comparison between different
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
	      {1, 1, 1, 1, 1} };
/*
 * Test result:
   S 1 > > v
   v 1 ^ 1 v
   > > ^ 1 v
   0 1 1 1 v
   0 0 0 1 T
 */
int main(void) {
   Point start(0,1),target(0,1);
   Search b;
   if(b.bfs(map,start,target)){
      b.printRouteOnMap(map);
   }
   else{
      cout<<"Search failed.\n";
   }
   return 0;
}
