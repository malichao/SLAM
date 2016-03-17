#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>
#include <cmath>
#include <climits>
using namespace std;
struct Point {
   unsigned int x;
   unsigned int y;
   Point(unsigned int x,unsigned int y):x(x),y(y){};
   Point(const Point &p):x(p.x),y(p.y){};
   bool operator== (const Point rhs) const{
      return (x==rhs.x&&y==rhs.y);
   }
   bool operator!= (const Point rhs){
            return (x!=rhs.x||y!=rhs.y);
         }
};
struct AStarPoint{
	Point point;
	//f=h+g
	int f;	//f is the final evaluation of a grid
	int h;	//h is the heuristic value of a grid
	int g;	//g is the cost of a grid given by the map
	AStarPoint(Point p,int h,int g):
		point(p),f(h+g),h(h),g(g){};
};
class Search {
public:
   const unsigned int Wall =0;	//positive value means the cost of each path
   const unsigned int CostMax= 100;//Don't use UINT_MAX
   Search():start(0,0),target(0,0),steps(0){};
   bool bfs(const vector<vector<unsigned int> > &map);
   bool bfs(const vector<vector<unsigned int> > &map,const Point &start,const Point &target);
   bool aStar(const vector<vector<unsigned int> > &map);
   bool aStar(const vector<vector<unsigned int> > &map,const Point &start,const Point &target);
   bool dpSearch(const vector<vector<unsigned int> > &map);
   bool dpSearch(const vector<vector<unsigned int> > &map,const Point &start,const Point &target);
   void setStart(const Point &s){start.x=s.x; start.y=s.y;}
   void setTarget(const Point &s){target.x=s.x; target.y=s.y;}
   int 	getMinDistance(){return route.size()-1;}
   void printRoute();
   void printRouteOnMap(const vector<vector<unsigned int> > &m);
private:
   Point start;
   Point target;
   unsigned int steps;
   vector<vector<bool> > checked;
   vector<vector<Point> > direction;
   vector<Point> route;
   bool isLegal(const vector<vector<unsigned int> > &map,Point p);
   unsigned int abs(Point &a,Point &b);
   unsigned int minCost(const vector<vector<unsigned int> > &map,
   						const vector<vector<unsigned int> > &costMap,
   						const Point &p);
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

void Search::printRouteOnMap(const vector<vector<unsigned int> > &m) {
	if (route.size() == 0) {
		cout << "No route.\n";
		return;
	}
	//the original map is unsigned int type,to print route on the map
	//we need to convert it to char
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
			map[route[i].x][route[i].y] = 'v';	//Down
		else if (dX == -1)
			map[route[i].x][route[i].y] = '^';	//Up
		else if (dY == 1)
			map[route[i].x][route[i].y] = '>';	//Right
		else if (dX == -1)
			map[route[i].x][route[i].y] = '<';	//Left
	}

	map[route[0].x][route[0].y] = 'S';	//Mark the start point
	map[route[route.size() - 1].x][route[route.size() - 1].y] = 'T';	//And the target point

	cout << "Search effort: " << steps;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (size_t i = 0; i < map.size(); i++) {
		for (size_t j = 0; j < map[i].size(); j++) {
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
}

bool Search::isLegal(const vector<vector<unsigned int> > &map,Point p){
	steps++;
	if(p.x>=map.size()||p.y>=map[0].size())		//x and y are unsigned int
	  return false;
	if(checked[p.x][p.y])   	//Current position has been searched
	  return false;
	if(map[p.x][p.y]==Wall)		//Don't hit the wall
	  return false;
	return true;
}

bool Search::bfs(const vector<vector<unsigned int> > &map,const Point &start,const Point &target){
   setTarget(target);
   setStart(start);
   return bfs(map);
}

/*
 * Basic idea:
 * -Search from target towards start grid
 * -Check every surrounding grid and put available ones in the queue
 * -Set these appended grid pointing to its previous grid
 * -Read grids from the queue and check if the start grid is reached
 * -When searching is done,iterate the direction vector to generate the route
 */
bool Search::bfs(const vector<vector<unsigned int> > &map){
	direction.clear();
   for(size_t i=0;i<map.size();i++){				//Init the direction map
      vector<Point> zero(map[i].size(),Point(0,0));
      direction.push_back(zero);
   }
   checked.clear();
   for(size_t i=0;i<map.size();i++){				//Init the checked set
      vector<bool> temp(map[i].size(),false);
      checked.push_back(temp);
   }

   if(!isLegal(map,target)||!isLegal(map,start))
      return false;

   queue<Point> que;
   que.push(target);
   checked[target.x][target.y]=true;
   bool success=false;
   steps=1;			//Reset the steps to count the search effort
   route.clear();	//Clear the previous route
   while(!que.empty()){
      Point curPos=que.front();
      que.pop();
      checked[curPos.x][curPos.y]=true;
      if(curPos==start){
         success=true;
         break;
      }

      //Append available surrounding grid to the queue
      if(isLegal(map,Point(curPos.x-1,curPos.y))){ //move up
    	 que.push(Point(curPos.x-1,curPos.y));
         direction[curPos.x-1][curPos.y]=curPos;
      }
      if(isLegal(map,Point(curPos.x+1,curPos.y))){ //move down
    	 que.push(Point(curPos.x+1,curPos.y));
         direction[curPos.x+1][curPos.y]=curPos;
      }
      if(isLegal(map,Point(curPos.x,curPos.y-1))){ //move left
    	 que.push(Point(curPos.x,curPos.y-1));
         direction[curPos.x][curPos.y-1]=curPos;
      }
      if(isLegal(map,Point(curPos.x,curPos.y+1))){ //move right
    	 que.push(Point(curPos.x,curPos.y+1));
         direction[curPos.x][curPos.y+1]=curPos;
      }
   }

   //Searching is done,if target is found then save the route
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

unsigned int Search::abs(Point &a,Point &b){
	return std::abs(a.x-b.x)+std::abs(a.y-b.y);
}

struct lowestF{
	bool operator()(const AStarPoint &lhs,const AStarPoint &rhs) const{
		return lhs.f>rhs.f;		//minimal priority queue
	}
};

bool Search::aStar(const vector<vector<unsigned int> > &map,const Point &start,const Point &target) {
	setTarget(target);
	setStart(start);
	return aStar(map);
}

/*
 * Basic idea:
 * -Search from target towards start grid
 * -Check every surrounding grid,calculate their values and put them in the
 *  minimal priority queue
 * -Set these appended grid pointing to its previous grid
 * -Get the minimal cost grid from the priority queue and iterate
 * -When searching is done,iterate the direction vector to generate the route
 */
bool Search::aStar(const vector<vector<unsigned int> > &map) {
	direction.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point> zero(map[i].size(), Point(0, 0));
		direction.push_back(zero);
	}
	checked.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the checked set
		vector<bool> temp(map[i].size(), false);
		checked.push_back(temp);
	}

	if (!isLegal(map, target) || !isLegal(map, start))
		return false;

	//Similar to BFS,but we use minimal priority queue instead
	priority_queue<AStarPoint, vector<AStarPoint>, lowestF> pQue;
	pQue.push(AStarPoint(target, map[target.x][target.y], abs(target, start)));
	checked[target.x][target.y] = true;
	bool success = false;
	steps = 1;			//Reset the steps to count the search effort
	route.clear();		//Clear the previous route
	while (!pQue.empty()) {
		AStarPoint curPos = pQue.top();
		Point p = curPos.point;
		pQue.pop();
		checked[p.x][p.y] = true;
		if (p == start) {
			success = true;
			break;
		}

		Point tempP(p.x,p.y);
		tempP.x = p.x - 1; tempP.y = p.y;
		unsigned int g=curPos.g;

		//Append available surrounding grid to the pQue
		if (isLegal(map, tempP)) { 							//Move up
			pQue.push(	AStarPoint(	tempP,					//Point(x,y)
								abs(tempP, target),			//heuristic value
								map[tempP.x][tempP.y]+g));	//grid cost
			direction[tempP.x][tempP.y] = p;
		}
		tempP.x = p.x + 1; tempP.y = p.y;
		if (isLegal(map, tempP)) { 							//Move down
			pQue.push(	AStarPoint(	tempP,
								abs(tempP, target),
								map[tempP.x][tempP.y]+g));
			direction[tempP.x][tempP.y] = p;
		}
		tempP.x = p.x; tempP.y = p.y-1;
		if (isLegal(map, tempP)) { 							//Move left
			pQue.push(	AStarPoint(	tempP,
								abs(tempP, target),
								map[tempP.x][tempP.y]+g));
			direction[tempP.x][tempP.y] = p;
		}
		tempP.x = p.x; tempP.y = p.y+1;
		if (isLegal(map, tempP)) { 							//Move right
			pQue.push(	AStarPoint(	tempP,
								abs(tempP, target),
								map[tempP.x][tempP.y]+g));
			direction[tempP.x][tempP.y] = p;
		}
	}
	if (success) {
		Point curPos = start;
		do {
			route.push_back(curPos);
			curPos = direction[curPos.x][curPos.y];
		} while (curPos != target);
		route.push_back(target);
		return true;
	}
	return false;
}

unsigned int Search::minCost(const vector<vector<unsigned int> > &map,
							 const vector<vector<unsigned int> > &costMap,
							 const Point &point) {
	if(point==target)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX because it will overflow later on
	Point p(point);
	p.x=point.x-1;p.y=point.y;
	if ((p.x < map.size() && p.y < map[0].size())&& checked[p.x][p.y] //Bounding check first
			&& map[p.x][p.y] != Wall)
		min = costMap[p.x][p.y] < min ? costMap[p.x][p.y] : min;

	p.x=point.x+1;p.y=point.y;
	if ((p.x < map.size() && p.y < map[0].size())&& checked[p.x][p.y]
			&& map[p.x][p.y] != Wall)
		min = costMap[p.x][p.y] < min ? costMap[p.x][p.y] : min;

	p.x=point.x;p.y=point.y-1;
	if ((p.x < map.size() && p.y < map[0].size())&& checked[p.x][p.y]
			&& map[p.x][p.y] != Wall)
		min = costMap[p.x][p.y] < min ? costMap[p.x][p.y] : min;

	p.x=point.x;p.y=point.y+1;
	if ((p.x < map.size() && p.y < map[0].size())&& checked[p.x][p.y]
			&& map[p.x][p.y] != Wall)
		min = costMap[p.x][p.y] < min ? costMap[p.x][p.y] : min;

	return min;
}

bool Search::dpSearch(const vector<vector<unsigned int> > &map,
					  const Point &start,const Point &target) {
	setTarget(target);
	setStart(start);
	return dpSearch(map);
}

bool Search::dpSearch(const vector<vector<unsigned int> > &map) {
	direction.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point> zero(map[i].size(), Point(0, 0));
		direction.push_back(zero);
	}
	checked.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the checked set
		vector<bool> temp(map[i].size(), false);
		checked.push_back(temp);
	}

	if (!isLegal(map, target) || !isLegal(map, start))
		return false;

	vector<vector<unsigned int> > costMap(map.size(),
									vector<unsigned int> (map[0].size(),CostMax));

	queue<Point> que;
	que.push(target);
	checked[target.x][target.y] = true;
	costMap[target.x][target.y]	=0;

	steps = 1;			//Reset the steps to count the search effort
	route.clear();	//Clear the previous route
	while (!que.empty()) {
		Point curPos = que.front();
		que.pop();
		checked[curPos.x][curPos.y] = true;
		unsigned int min=minCost(map,costMap,curPos);
		min+=map[curPos.x][curPos.y];
		costMap[curPos.x][curPos.y]	=min;
		//costMap[curPos.x][curPos.y]	=minCost(map,costMap,curPos)+map[curPos.x][curPos.y];

		//Append available surrounding grid to the queue
		if (isLegal(map, Point(curPos.x - 1, curPos.y))) { //move up
			que.push(Point(curPos.x - 1, curPos.y));

		}
		if (isLegal(map, Point(curPos.x + 1, curPos.y))) { //move down
			que.push(Point(curPos.x + 1, curPos.y));

		}
		if (isLegal(map, Point(curPos.x, curPos.y - 1))) { //move left
			que.push(Point(curPos.x, curPos.y - 1));

		}
		if (isLegal(map, Point(curPos.x, curPos.y + 1))) { //move right
			que.push(Point(curPos.x, curPos.y + 1));

		}
	}

	//Searching is done,if target is found then save the route
	for(size_t i=0;i<costMap.size();i++){
		for(size_t j=0;j<costMap[i].size();j++){
			cout<<costMap[i][j]<<"\t";
		}
		cout<<endl;
	}

	return false;
}

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
vector<vector<unsigned int> > map = {
{1, 0, 1, 1, 1},
{1, 1, 1, 0, 1},
{1, 1, 0, 0, 1},
{1, 1, 2, 2, 1},
{1, 0, 6, 5, 1} };
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
   Search b;

   /*
	if (b.bfs(map, start, target)) {
		b.printRouteOnMap(map);
	} else {
		cout << "Search failed.\n";
	}

	if (b.aStar(map, start, target)) {
		b.printRouteOnMap(map);
	} else {
		cout << "Search failed.\n";
	}
	*/
   b.dpSearch(map,start,target);
   return 0;
}
