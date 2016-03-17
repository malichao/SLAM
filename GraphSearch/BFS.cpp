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
   const unsigned int Wall =0;			//positive value means the cost of each path
   const unsigned int CostMax= 100;		//Don't use UINT_MAX,watch out for OVERFLOW
   Search():Start(0,0),Target(0,0),EffortCount(0){};

   void setStart(const Point &s){Start.x=s.x; Start.y=s.y;}
   void setTarget(const Point &t){Target.x=t.x; Target.y=t.y;}

   int 	getMinDistance(){return Route.size()-1;}
   void printRoute();
   void printRouteOnMap(const vector<vector<unsigned int> > &map);
   void printDirectionOnMap(const vector<vector<unsigned int> > &map);

   bool bfs(const vector<vector<unsigned int> > &map);
   bool bfs(const vector<vector<unsigned int> > &map,
		    const Point &start,const Point &target);

   bool aStar(const vector<vector<unsigned int> > &map);
   bool aStar(const vector<vector<unsigned int> > &map,
		      const Point &start,const Point &target);

   bool dpSearch(const vector<vector<unsigned int> > &map);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const Point &start,const Point &target);
private:
   Point Start;
   Point Target;
   unsigned int EffortCount;

   vector<vector<bool> > Checked;
   vector<vector<Point> > Direction;
   vector<Point> Route;
   vector<vector<unsigned int> > CostMap;

   void generateRoute();
   unsigned int abs(Point &a,Point &b);
   bool isLegal(const vector<vector<unsigned int> > &map,Point p);
   unsigned int minCost(const vector<vector<unsigned int> > &map,
   						const Point &p,
						unsigned int &dir);

   const unsigned int NoneDirection=4;
   int Move[4][2]={
		   {-1,0},	// x-1,y
		   { 1,0},	// x+1,y
		   {0,-1},	// x,y-1
		   {0, 1}};	// x,y+1
};

void Search::printRoute() {
	if (Route.size() == 0) {
		cout << "No route.\n";
		return;
	}
	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (auto r : Route) {
		cout << "(" << r.x << "," << r.y << ")" << endl;
	}
}

void Search::printRouteOnMap(const vector<vector<unsigned int> > &map) {
	if (Route.size() == 0) {
		cout << "No route.\n";
		return;
	}
	//the original map is unsigned int type,to print route on the map
	//we need to convert it to char
	vector<vector<char> > charMap;
	for (size_t i = 0; i < map.size(); i++) {
		vector<char> row(map[i].size());
		for (size_t j = 0; j < map[i].size(); j++) {
			row[j] = map[i][j] + '0';
		}
		charMap.push_back(row);
	}

	for (size_t i = 1; i + 1 < Route.size(); i++) {
		int dX = Route[i + 1].x - Route[i].x;
		int dY = Route[i + 1].y - Route[i].y;
		if (dX == 1)
			charMap[Route[i].x][Route[i].y] = 'v';	//Down
		else if (dX == -1)
			charMap[Route[i].x][Route[i].y] = '^';	//Up
		else if (dY == 1)
			charMap[Route[i].x][Route[i].y] = '>';	//Right
		else if (dX == -1)
			charMap[Route[i].x][Route[i].y] = '<';	//Left
	}

	charMap[Route[0].x][Route[0].y] = 'S';	//Mark the start point
	charMap[Route[Route.size() - 1].x][Route[Route.size() - 1].y] = 'T';	//And the target point

	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
}

void Search::printDirectionOnMap(const vector<vector<unsigned int> > &map) {
	if (CostMap.size()==0) {
		cout << "'costMap' data needed.\n";
		return;
	}
	//the original map is unsigned int type,to print route on the map
	//we need to convert it to char
	vector<vector<char> > charMap;
	for (size_t i = 0; i < map.size(); i++) {
		vector<char> row(map[i].size());
		for (size_t j = 0; j < map[i].size(); j++) {
			row[j] = map[i][j] + '0';
		}
		charMap.push_back(row);
	}

	for (size_t i = 0; i < charMap.size(); i++)
		for (size_t j = 0; j < charMap[i].size(); j++) {
			if (Checked[i][j]) {
				size_t dir;
				minCost(map, Point(i, j), dir);
				switch (dir) {
				case 0:	charMap[i][j] = '^';break;
				case 1:	charMap[i][j] = 'v';break;
				case 2:	charMap[i][j] = '<';break;
				case 3:	charMap[i][j] = '>';break;
				default:break;
				}
			}
		}

	charMap[Start.x][Start.y] = 'S';	//Mark the start point
	charMap[Target.x][Target.y] = 'T';	//And the target point

	cout << "Search effort: " << EffortCount;
	cout << "\nShortest path:" << getMinDistance() << endl;
	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			cout << charMap[i][j] << " ";
		}
		cout << endl;
	}
}

//Generate a route using the direction info
void Search::generateRoute(){
	if(Direction.size()==0)
		return;
    Point curPos=Start;
    do{
       Route.push_back(curPos);
       curPos=Direction[curPos.x][curPos.y];
    }while(curPos!=Target);
    Route.push_back(Target);
}

bool Search::isLegal(const vector<vector<unsigned int> > &map,Point p){
	EffortCount++;
	if(p.x>=map.size()||p.y>=map[0].size())		//x and y are unsigned int
	  return false;
	if(Checked[p.x][p.y])   	//Current position has been searched
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
	Direction.clear();
   for(size_t i=0;i<map.size();i++){				//Init the direction map
      vector<Point> zero(map[i].size(),Point(0,0));
      Direction.push_back(zero);
   }
   Checked.clear();
   for(size_t i=0;i<map.size();i++){				//Init the checked set
      vector<bool> temp(map[i].size(),false);
      Checked.push_back(temp);
   }

   if(!isLegal(map,Target)||!isLegal(map,Start))
      return false;

   queue<Point> que;
   que.push(Target);
   Checked[Target.x][Target.y]=true;
   bool success=false;
   EffortCount=1;			//Reset the EffortCount to count the search effort
   Route.clear();	//Clear the previous route
   while(!que.empty()){
      Point curPos=que.front();
      que.pop();
      Checked[curPos.x][curPos.y]=true;
      if(curPos==Start){
         success=true;
         break;
      }

      //Append available surrounding grid to the queue
      for(size_t k=0;k<4;k++){	//Iterate Up,Down,Left,Right 4 direction
    	  Point p(curPos.x+Move[k][0],curPos.y+Move[k][1]);
    	  if(isLegal(map,p)){
			 que.push(p);
			 Direction[p.x][p.y]=curPos;
    	  }
      }
   }

   //Searching is done,if target is found then save the route
   if(success){
      generateRoute();
      return true;
   }
   return false;
}

unsigned int Search::abs(Point &a,Point &b){
	return std::abs(a.x-b.x)+std::abs(a.y-b.y);
}

//Comparison object to be used to order the heap
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
	Direction.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point> zero(map[i].size(), Point(0, 0));
		Direction.push_back(zero);
	}
	Checked.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the checked set
		vector<bool> temp(map[i].size(), false);
		Checked.push_back(temp);
	}

	if (!isLegal(map, Target) || !isLegal(map, Start))
		return false;

	//Similar to BFS,but we use minimal priority queue instead
	priority_queue<AStarPoint, vector<AStarPoint>, lowestF> pQue;
	pQue.push(AStarPoint(Target, map[Target.x][Target.y], abs(Target, Start)));
	Checked[Target.x][Target.y] = true;
	bool success = false;
	EffortCount = 1;			//Reset the EffortCount to count the search effort
	Route.clear();		//Clear the previous route
	while (!pQue.empty()) {
		AStarPoint curPos = pQue.top();
		Point p = curPos.point;
		pQue.pop();
		Checked[p.x][p.y] = true;
		if (p == Start) {
			success = true;
			break;
		}
		unsigned int g=curPos.g;

		//Append available surrounding grid to the pQue
		for(size_t k=0;k<4;k++){	//Iterate Up,Down,Left,Right 4 direction
		  Point tempP(p.x+Move[k][0],p.y+Move[k][1]);
		  if(isLegal(map,tempP)){
			  pQue.push(AStarPoint(	tempP,						//Point(x,y)
			  						abs(tempP, Target),			//heuristic value
			  						map[tempP.x][tempP.y]+g));	//grid cost
			  Direction[tempP.x][tempP.y] = p;
		  }
		}
	}
	if (success) {
		generateRoute();
		return true;
	}
	return false;
}

unsigned int Search::minCost(const vector<vector<unsigned int> > &map,
							 const Point &point,
							 unsigned int &dir) {
	dir=NoneDirection;	//0:Up	1:Down	2:left	3:Down	4:None
	if(point==Target)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX because it will overflow later on
	Point p(point);
	for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
		Point p(point.x+Move[k][0],point.y+Move[k][1]);
		if ((p.x < map.size() && p.y < map[0].size())&& Checked[p.x][p.y] //Bounding check first
					&& map[p.x][p.y] != Wall)
			min = CostMap[p.x][p.y] < min ? dir=k,CostMap[p.x][p.y] : min;
	}
	return min;
}

bool Search::dpSearch(const vector<vector<unsigned int> > &map,
					  const Point &start,const Point &target) {
	setTarget(target);
	setStart(start);
	return dpSearch(map);
}

bool Search::dpSearch(const vector<vector<unsigned int> > &map) {
	Direction.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point> zero(map[i].size(), Point(0, 0));
		Direction.push_back(zero);
	}
	Checked.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the checked set
		vector<bool> temp(map[i].size(), false);
		Checked.push_back(temp);
	}

	if (!isLegal(map, Target) || !isLegal(map, Start))
		return false;

	CostMap.clear();
	CostMap=vector<vector<unsigned int> >(map.size(),vector<unsigned int> (map[0].size(),CostMax));

	queue<Point> que;
	que.push(Target);
	Checked[Target.x][Target.y] = true;
	CostMap[Target.x][Target.y]	=0;
	unsigned int minDir;

	EffortCount = 1;			//Reset the EffortCount to count the search effort
	Route.clear();	//Clear the previous route
	while (!que.empty()) {
		Point curPos = que.front();
		que.pop();
		Checked[curPos.x][curPos.y] = true;

		//CostMap[curPos.x][curPos.y]=minCost(map,costMap,curPos,minDir)+map[curPos.x][curPos.y];
		unsigned int min=minCost(map,curPos,minDir);
		min+=map[curPos.x][curPos.y];
		CostMap[curPos.x][curPos.y]	=min;

		if(minDir!=NoneDirection)
			Direction[curPos.x][curPos.y] = Point(	curPos.x+Move[minDir][0],
													curPos.y+Move[minDir][1]);

		//Append available surrounding grid to the queue
		for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
			Point p(curPos.x+Move[k][0],curPos.y+Move[k][1]);

			if (isLegal(map,p))  //move up
				que.push(p);
		}
	}

	//	for(size_t i=0;i<CostMap.size();i++){	//Test output the costMap
	//		for(size_t j=0;j<CostMap[i].size();j++){
	//		cout<<CostMap[i][j]<<"\t";
	//		}
	//		cout<<endl;
	//	}

	generateRoute();
	return true;
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
		{1, 0, 6, 5, 1},};
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
	if (b.dpSearch(map, start, target)) {
		b.printDirectionOnMap(map);
	} else {
		cout << "Search failed.\n";
	}
   return 0;
}
