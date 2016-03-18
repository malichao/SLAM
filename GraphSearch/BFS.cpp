#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>
#include <cmath>
#include <climits>
using namespace std;
struct Point {
   const unsigned int DirectionSize=4;
   const unsigned int Up=0;
   const unsigned int Down=1;
   const unsigned int Left=2;
   const unsigned int Right=3;

   unsigned int x;
   unsigned int y;
   unsigned int dir;	//0:Up	1:Down 2:Left	2:Right

   Point():x(0),y(0),dir(Up){};
   Point(unsigned int x,unsigned int y):x(x),y(y),dir(Up){};
   Point(unsigned int x,unsigned int y,unsigned int dir):x(x),y(y),dir(dir){};
   Point(const Point &p):x(p.x),y(p.y),dir(p.dir){};
   void set(const Point &p){
	   x=p.x;
	   y=p.y;
	   dir=p.dir;
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
struct AStarPoint{
	Point point;
	//f=h+g
	int f;	//f is the final evaluation of a grid
	int h;	//h is the heuristic value of a grid
	int g;	//g is the cost of a grid given by the map
	AStarPoint(Point p,int h,int g):
		point(p),f(h+g),h(h),g(g){};
};
struct DPPoint{
	Point point;
	int cost;
	DPPoint(const Point &p,unsigned int cost):
		point(p),cost(cost){};
};
/*
struct Movement{
	const unsigned int DirectionSize=4;
	const unsigned int Up=0;
	const unsigned int Down=1;
	const unsigned int Left=2;
	const unsigned int Right=3;
	const unsigned int NoneDirection = 4;

	const unsigned int ThetaSize=3;
	const unsigned int Forward=0;
	const unsigned int TurnLeft=1;
	const unsigned int TurnRight=2;

	Movement():direction(NoneDirection),theta(Forward){};
	Movement(unsigned int direction,unsigned int theta):
								direction(direction),theta(theta){};
	unsigned int direction;	//For x and y movement 0:Up	1:Down	2:Left	3:Right	4:None
	unsigned int theta;	//FOr steering movement 0:Forward	1:Left turn	2:Right turn
};
*/
class Search {
public:
   const unsigned int Wall =0;			//positive value means the cost of each path
   const char WallSymbol = '#';
   const unsigned int CostMax= 1000;		//Don't use UINT_MAX,watch out for OVERFLOW
   Search():Start(0,0),Target(0,0),EffortCount(0){};

   void setStart(const Point &s){Start.x=s.x; Start.y=s.y;}
   void setTarget(const Point &t){Target.x=t.x; Target.y=t.y;}

   int 	getMinDistance(){return Route.size()-1;}
   void printRoute();
   void printRouteOnMap(const vector<vector<unsigned int> > &map);
   void printGradientOnMap(const vector<vector<unsigned int> > &map);

   bool bfs(const vector<vector<unsigned int> > &map);
   bool bfs(const vector<vector<unsigned int> > &map,
		    const Point &start,const Point &target);

   bool aStar(const vector<vector<unsigned int> > &map);
   bool aStar(const vector<vector<unsigned int> > &map,
		      const Point &start,const Point &target);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost);
   bool aStar(const vector<vector<unsigned int> > &map,
		   	  const unsigned int *const moveCost,
   		      const Point &start,const Point &target);

   bool dpSearch(const vector<vector<unsigned int> > &map);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const Point &start,const Point &target);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	     const unsigned int *const moveCost);
   bool dpSearch(const vector<vector<unsigned int> > &map,
		   	   	 const unsigned int *const moveCost,
   		   	     const Point &start,const Point &target);
private:
	Point Start;
	Point Target;
	unsigned int EffortCount;

	vector<Point> Route;
	vector<vector<bool> > Checked;
	vector<vector<Point> > Gradient;
	vector<vector<unsigned int> > CostMap;

	vector<vector<vector<bool> > > Checked3D;
	vector<vector<vector<Point> > > Gradient3D;
	vector<vector<vector<unsigned int> > > CostMap3D;

	void generateRoute();
	void generateRoute(const unsigned int* const moveCost);
	unsigned int abs(Point &a, Point &b);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const Point &p);
	bool isLegal(const vector<vector<unsigned int> > &map,
				 const unsigned int* const moveCost,
				 const Point &p);
	unsigned int minCost(const vector<vector<unsigned int> > &map,
   						const Point &p,
						Point &move);

	unsigned int minCost(const vector<vector<unsigned int> > &map,
						 const unsigned int* const moveCost,
						 const Point &point,
						 Point &move);

	//const unsigned int NoneDirection = 4;
	int Move[5][2] = {
		   {-1,0},	// x-1,y
		   { 1,0},	// x+1,y
		   {0,-1},	// x,y-1
		   {0, 1},	// x,y+1
		   {0, 0}};	// x,y

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
			row[j] = map[i][j]==0 ? WallSymbol: map[i][j] + '0';
		}
		charMap.push_back(row);
	}

	char dirSymbol[3][3]={
			{' ','^',' '},
			{'<',' ','>'},
			{' ','V',' '},};

	for (size_t i = 0; i + 1 < Route.size(); i++) {
		int dX = Route[i + 1].x - Route[i].x;	//dX = -1 or 1
		int dY = Route[i + 1].y - Route[i].y;	//dY = -1 or 1
		charMap[Route[i].x][Route[i].y]=dirSymbol[1+dX][1+dY];
	}

	//charMap[Route[0].x][Route[0].y] = 'S';	//Mark the start point
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

void Search::printGradientOnMap(const vector<vector<unsigned int> > &map) {
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
			row[j] = map[i][j]==0 ? WallSymbol: map[i][j] + '0';
		}
		charMap.push_back(row);
	}

	char dirSymbol[5]={'^','V','<','>',' '};
	for (size_t i = 0; i < charMap.size(); i++)
		for (size_t j = 0; j < charMap[i].size(); j++) {
			if (Checked[i][j]) {
				Point move;
				minCost(map, Point(i, j), move);
				charMap[i][j] =dirSymbol[move.dir];
			}
		}

	//charMap[Start.x][Start.y] = 'S';	//Mark the start point
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
	if(Gradient.size()==0)
		return;
    Point curPos=Start;
    do{
       Route.push_back(curPos);
       curPos=Gradient[curPos.x][curPos.y];
    }while(curPos!=Target);
    Route.push_back(Target);
}
//Generate a route using the direction info
void Search::generateRoute(const unsigned int* const moveCost){
	if(Gradient3D.size()==0)
		return;
    Point curPos=Start;
    do{
       Route.push_back(curPos);
       curPos=Gradient3D[curPos.dir][curPos.x][curPos.y];
    }while(curPos!=Target);
    Route.push_back(Target);
}

bool Search::isLegal(const vector<vector<unsigned int> > &map,const Point &p){
	EffortCount++;
	if(p.x>=map.size()||p.y>=map[0].size())		//x and y are unsigned int
	  return false;
	if(Checked[p.x][p.y])   	//Current position has been searched
	  return false;
	if(map[p.x][p.y]==Wall)		//Don't hit the wall
	  return false;
	return true;
}

bool Search::isLegal(const vector<vector<unsigned int> > &map,
					 const unsigned int* const moveCost,
					 const Point &p){
	EffortCount++;
	if(p.x>=map.size()||p.y>=map[0].size())		//x and y are unsigned int
	  return false;
	if(Checked3D[p.dir][p.x][p.y])   	//Current position has been searched
	  return false;
	if(map[p.x][p.y]==Wall)		//Don't hit the wall
	  return false;
	return true;
}

bool Search::bfs(const vector<vector<unsigned int> > &map,
				 const Point &start,const Point &target){
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
	Gradient.clear();
   for(size_t i=0;i<map.size();i++){				//Init the direction map
      vector<Point> zero(map[i].size(),Point(0,0));
      Gradient.push_back(zero);
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
			 Gradient[p.x][p.y]=curPos;
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

//Comparison object to be used to order the heap in A* search
struct lowestF{
	bool operator()(const AStarPoint &lhs,const AStarPoint &rhs) const{
		return lhs.f>rhs.f;		//minimal priority queue
	}
};

bool Search::aStar(const vector<vector<unsigned int> > &map,
				   const Point &start,const Point &target) {
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
	Gradient.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point> zero(map[i].size(), Point(0, 0));
		Gradient.push_back(zero);
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
			  Gradient[tempP.x][tempP.y] = p;
		  }
		}
	}
	if (success) {
		generateRoute();
		return true;
	}
	return false;
}

//Todo:rewrite this function to make it clear
//Find the minimal cost of the four direction from visited points
unsigned int Search::minCost(const vector<vector<unsigned int> > &map,
							 const Point &point,
							 Point &nextMove) {
	nextMove=point;	//0:Up	1:Down	2:Left	3:Right	4:None
	if(point==Target)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX otherwise it will overflow later on
	for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
		Point p(point.x+Move[k][0],point.y+Move[k][1],k);

		//In the predicate,must check the bounding first.We don't use isLegal() because we're
		//going to find the minimal cost point in previously visited points
		if ((p.x < map.size() && p.y < map[0].size())&& Checked[p.x][p.y]
					&& map[p.x][p.y] != Wall){
			if(CostMap[p.x][p.y] < min){
				min=CostMap[p.x][p.y];
				nextMove=p;
			}
		}
	}
	return min;
}

unsigned int Search::minCost(const vector<vector<unsigned int> > &map,
							 const unsigned int* const moveCost,
							 const Point &point,
							 Point &nextMove) {
	nextMove=point;//0:Up	1:Down	2:Left	3:Right	4:None
	if(point==Target)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX because it will overflow later on

	for(size_t dir=0;dir<point.DirectionSize;dir++){	//Four orientation
		for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
			Point p(point.x+Move[k][0],point.y+Move[k][1],dir);

			//In the predicate,must check the bounding first.We don't use isLegal() because we're
			//going to find the minimal cost point in previously visited points
			if ((p.x < map.size() && p.y < map[0].size())&& Checked3D[dir][p.x][p.y]
						&& map[p.x][p.y] != Wall)
				if(CostMap3D[dir][p.x][p.y] < min){
					min = CostMap3D[dir][p.x][p.y];
					nextMove=p;
				}
		}
	}

	return min;
}

//Comparison object to be used to order the heap in dpSearch
struct lessCost{
	bool operator()(const DPPoint &lhs,const DPPoint &rhs) const{
		return lhs.cost>rhs.cost;		//minimal priority queue
	}
};

bool Search::dpSearch(const vector<vector<unsigned int> > &map,
					  const Point &start,const Point &target) {
	setTarget(target);
	setStart(start);
	return dpSearch(map);
}

bool Search::dpSearch(const vector<vector<unsigned int> > &map) {
	Gradient.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point> zero(map[i].size(), Point(0, 0));
		Gradient.push_back(zero);
	}
	Checked.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the checked set
		vector<bool> temp(map[i].size(), false);
		Checked.push_back(temp);
	}

	if (!isLegal(map, Target) || !isLegal(map, Start))
		return false;

	CostMap.clear();
	CostMap=vector<vector<unsigned int> >
				(map.size(),vector<unsigned int> (map[0].size(),CostMax));
	Checked[Target.x][Target.y] = true;
	CostMap[Target.x][Target.y]	=0;

	//DPPoint is basically a Point with a 'cost',which is used to sort in the priority queue
	priority_queue<DPPoint, vector<DPPoint>, lessCost> pQue;
	pQue.push(DPPoint(Target,0));
	Point nextMove;

	EffortCount = 1;			//Reset the EffortCount to count the search effort
	Route.clear();	//Clear the previous route
	while (!pQue.empty()) {
		DPPoint curPos = pQue.top();
		Point p = curPos.point;
		pQue.pop();
		Checked[p.x][p.y] = true;

		//CostMap[curPos.x][curPos.y]=minCost(map,costMap,curPos,minDir)+map[curPos.x][curPos.y];
		CostMap[p.x][p.y] = minCost(map,p,nextMove)+map[p.x][p.y];
		//Point the current point to the lowest cost point
		Gradient[p.x][p.y] = nextMove;

		//Append available surrounding grid to the queue
		for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
			Point tempP(p.x+Move[k][0],p.y+Move[k][1]);
			if (isLegal(map,tempP))  //move up
				pQue.push(DPPoint(tempP,CostMap[tempP.x][tempP.y]));
		}
	}

	//Test:output the cost map
	//	for(size_t i=0;i<CostMap.size();i++){
	//		for(size_t j=0;j<CostMap[i].size();j++){
	//		cout<<CostMap[i][j]<<"\t";
	//		}
	//		cout<<endl;
	//	}

	generateRoute();
	return true;
}

bool Search::dpSearch(const vector<vector<unsigned int> > &map,
					  const unsigned int *const moveCost,
					  const Point &start,
					  const Point &target){
	setTarget(target);
	setStart(start);
	return dpSearch(map,moveCost);
}

bool Search::dpSearch(const vector<vector<unsigned int> > &map,
					  const unsigned int *const moveCost) {
	Gradient3D.clear();
	Point nextMove;
	for(size_t dir=0;dir<nextMove.DirectionSize;dir++){
		vector<vector<Point> > gradient2D;
		for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
			vector<Point> zero(map[i].size(), Point(0, 0,dir));
			gradient2D.push_back(zero);
		}
		Gradient3D.push_back(gradient2D);
	}

	Checked3D.clear();
	for(size_t dir=0;dir<nextMove.DirectionSize;dir++){
		vector<vector<bool> > checked2D;
		for (size_t i = 0; i < map.size(); i++) {			//Init the checked set
			vector<bool> temp(map[i].size(), false);
			checked2D.push_back(temp);
		}
		Checked3D.push_back(checked2D);
	}


	if (!isLegal(map,moveCost,Target) || !isLegal(map, moveCost,Start))	//Todo:Check left/right turn availability
		return false;

	CostMap3D.clear();
	for(size_t dir=0;dir<nextMove.DirectionSize;dir++){									//Construct a 3D vector
		CostMap3D.push_back(vector<vector<unsigned int> >			//Construct a 2D vector
								(map.size(),vector<unsigned int>	//Construct a 1D vector
											(map[0].size(),CostMax)));
	}
	for(size_t dir=0;dir<nextMove.DirectionSize;dir++){
		Checked3D[dir][Target.x][Target.y] = true;
		CostMap3D[dir][Target.x][Target.y]	=0;
	}

	priority_queue<DPPoint, vector<DPPoint>, lessCost> pQue;
	pQue.push(DPPoint(Target,0));


	EffortCount = 1;		//Reset the EffortCount to count the search effort
	Route.clear();			//Clear the previous route
	while (!pQue.empty()) {
		DPPoint curPos = pQue.top();
		Point p= curPos.point;
		pQue.pop();
		Checked3D[p.dir][p.x][p.y] = true;

		//CostMap[curPos.x][curPos.y]=minCost(map,costMap,curPos,minDir)+map[curPos.x][curPos.y];
		unsigned int min=minCost(map,moveCost,p,nextMove);
		min+=map[p.x][p.y]+moveCost[p.dir];
		CostMap3D[p.dir][p.x][p.y]=min;

		//if(move.direction!=move.NoDirection)
		/*
			Gradient3D[p.dir][p.x][p.y] = Point(	p.x+Move[move.dir][0],
														p.y+Move[move.dir][1],
														move.dir);
		 	 */
		//Append available surrounding grid to the queue
		for(size_t action=0;action<3;action++){

		}
		for(size_t dir=0;dir<nextMove.DirectionSize;dir++){
			for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
				Point tempP(p.x+Move[k][0],p.y+Move[k][1],dir);

				if (isLegal(map,moveCost,tempP))  //move up
					pQue.push(DPPoint(tempP,map[p.x][p.y]+moveCost[p.dir]));
			}
		}

	}

	for(size_t k=0;k<5;k++){
		cout<<"layer: "<<k<<endl;
		for(size_t i=0;i<CostMap3D.size();i++){	//Test output the costMap
			for(size_t j=0;j<CostMap3D[k][i].size();j++){
			cout<<CostMap3D[k][i][j]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
	}


	generateRoute(moveCost);
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
		{1, 1, 2, 3, 1},
		{1, 0, 6, 5, 1}};
//Define the cost for each action,left turn is expensive in real life
//[0]:Forward	[1]:Left turn	[2]:Right turn
unsigned int actionCost[]={1,20,2};

/*
{1, 0, 1, 1, 1},
{1, 0, 1, 0, 1},
{1, 1, 1, 1, 1},
{0, 0, 1, 0, 0},
{0, 0, 1, 0, 0},
*/
int main(void) {
   Point start(4,4),target(0,0);
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
		b.printGradientOnMap(map);
	} else {
		cout << "Search failed.\n";
	}

   //Point start1(4,2),target1(0,0);
   //b.dpSearch(map,moveCost,start1,target1);
   return 0;
}
