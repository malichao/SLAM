#include "GraphSearch.h"
using namespace std;
using namespace pnt;//Point<int> operation
template<class T>struct Point;
struct AStarPoint;
struct DPPoint;

void GraphSearch::getRoute(vector<Point<int> > &route){
	if(Route.size()==0)
		return;
	route=Route;
}

void GraphSearch::printRoute() {
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

void GraphSearch::printRouteOnMap(const vector<vector<unsigned int> > &map) {
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
			row[j] = map[i][j]==0 ? ObstacleSymbol: '.';
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
		if(charMap[Route[i].x][Route[i].y]=='^'||	//This predicate is to solve the route overlap problem,
			charMap[Route[i].x][Route[i].y]=='>'||	//if this grid is already a route,then we use '+' to
			charMap[Route[i].x][Route[i].y]=='V'||	//indicate overlap
			charMap[Route[i].x][Route[i].y]=='V')
			charMap[Route[i].x][Route[i].y]='+';
		else
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

void GraphSearch::printGradientOnMap(const vector<vector<unsigned int> > &map) {
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
			row[j] = map[i][j]==0 ? ObstacleSymbol: map[i][j] + '0';
		}
		charMap.push_back(row);
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
void GraphSearch::generateRoute(){
	if(Gradient.size()==0)
		return;
    Point<int> curPos=Start;
    do{
       Route.push_back(curPos);
       curPos=Gradient[curPos.x][curPos.y];
    }while(curPos!=Target);
    Route.push_back(Target);
}
void GraphSearch::generateRoute(const vector<vector<vector<Point<int> > > > &Gradient){
	if(Gradient3D.size()==0)
		return;
    Point<int> curPos=Target;	//Todo: reverse the route
    do{
       Route.push_back(curPos);
       curPos=Gradient3D[curPos.dir][curPos.x][curPos.y];
    }while(curPos!=Start);
    Route.push_back(Start);
    std::reverse(Route.begin(),Route.end());
}
//Generate a route using the direction info
void GraphSearch::generateRoute(const vector<vector<unsigned int> > &map){
	if(Gradient3D.size()==0)
		return;
	Point<int> prevP=Target;
    Point<int> nextP=Start;
    do{
       Route.push_back(nextP);
       unsigned int min=INT_MAX;
       Point<int> bestP=Target;
		Point<int> p(nextP.x,nextP.y,(nextP.dir-1)%nextP.DirectionSize);//left turn,same grid
		if ((p.x < map.size() && p.y < map[0].size())&&Checked3D[p.dir][p.x][p.y]&&map[p.x][p.y] != Obstacle){
			if(CostMap3D[p.dir][p.x][p.y] < min && p!=prevP){
				min=CostMap3D[p.dir][p.x][p.y];
				bestP=p;
			}
		}

		p.set(nextP.x+Move[nextP.dir][0],nextP.y+Move[nextP.dir][1],nextP.dir);//move forward
		if ((p.x < map.size() && p.y < map[0].size())&&Checked3D[p.dir][p.x][p.y]&&map[p.x][p.y] != Obstacle){
			if(CostMap3D[p.dir][p.x][p.y] < min && p!=prevP){
				min=CostMap3D[p.dir][p.x][p.y];
				bestP=p;
			}
		}

		p.set(nextP.x,nextP.y,(nextP.dir+1)%nextP.DirectionSize);//right turn,same grid
		if ((p.x < map.size() && p.y < map[0].size())&&Checked3D[p.dir][p.x][p.y]&&map[p.x][p.y] != Obstacle){
			if(CostMap3D[p.dir][p.x][p.y] < min && p!=prevP){
				min=CostMap3D[p.dir][p.x][p.y];
				bestP=p;
			}
		}
		prevP=nextP;
		nextP=bestP;
    }while(nextP!=Target);
    Route.push_back(Target);
}

bool GraphSearch::isLegal(const vector<vector<unsigned int> > &map,const Point<int> &p){
	EffortCount++;
	if(p.x<0||p.y<0||p.x>=map.size()||p.y>=map[0].size())		//x and y are int
	  return false;
	if(Checked[p.x][p.y])   	//Current position has been searched
	  return false;
	if(map[p.x][p.y]==Obstacle)		//Don't hit the wall
	  return false;
	return true;
}

bool GraphSearch::isLegal(const vector<vector<unsigned int> > &map,
					 const unsigned int* const moveCost,
					 const Point<int> &p){
	EffortCount++;
	if(p.x<0||p.y<0||p.x>=map.size()||p.y>=map[0].size())		//x and y are int
	  return false;
	if(Checked3D[p.dir][p.x][p.y])   	//Current position has been searched
	  return false;
	if(map[p.x][p.y]==Obstacle)		//Don't hit the wall
	  return false;
	return true;
}

bool GraphSearch::bfs(const vector<vector<unsigned int> > &map,
				 const Point<int> &start,const Point<int> &target){
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
bool GraphSearch::bfs(const vector<vector<unsigned int> > &map){
	Gradient.clear();
   for(size_t i=0;i<map.size();i++){				//Init the direction map
      vector<Point<int> > zero(map[i].size(),Point<int>(0,0));
      Gradient.push_back(zero);
   }
   Checked.clear();
   for(size_t i=0;i<map.size();i++){				//Init the checked set
      vector<bool> temp(map[i].size(),false);
      Checked.push_back(temp);
   }

   if(!isLegal(map,Target)||!isLegal(map,Start))
      return false;

   queue<Point<int> > que;
   que.push(Target);
   Checked[Target.x][Target.y]=true;
   bool success=false;
   EffortCount=1;			//Reset the EffortCount to count the search effort
   Route.clear();	//Clear the previous route
   while(!que.empty()){
      Point<int> curPos=que.front();
      que.pop();
      Checked[curPos.x][curPos.y]=true;
      if(curPos==Start){
         success=true;
         break;
      }

      //Append available surrounding grid to the queue
      for(size_t k=0;k<4;k++){	//Iterate Up,Down,Left,Right 4 direction
    	  Point<int> p(curPos.x+Move[k][0],curPos.y+Move[k][1]);
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


//Comparison object to be used to order the heap in A* search
struct lowestF{
	bool operator()(const AStarPoint &lhs,const AStarPoint &rhs) const{
		return lhs.f>rhs.f;		//minimal priority queue
	}
};

bool GraphSearch::aStar(const vector<vector<unsigned int> > &map,
				   const Point<int> &start,const Point<int> &target) {
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
bool GraphSearch::aStar(const vector<vector<unsigned int> > &map) {
	Gradient.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point<int> > zero(map[i].size(), Point<int>(0, 0));
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
	pQue.push(AStarPoint(Target, map[Target.x][Target.y], pnt::abs(Target, Start)));
	Checked[Target.x][Target.y] = true;
	bool success = false;
	EffortCount = 1;			//Reset the EffortCount to count the search effort
	Route.clear();		//Clear the previous route
	while (!pQue.empty()) {
		AStarPoint curPos = pQue.top();
		Point<int> p = curPos.point;
		pQue.pop();
		Checked[p.x][p.y] = true;
		if (p == Start) {
			success = true;
			break;
		}
		unsigned int g=curPos.g;

		//Append available surrounding grid to the pQue
		for(size_t k=0;k<4;k++){	//Iterate Up,Down,Left,Right 4 direction
		  Point<int> tempP(p.x+Move[k][0],p.y+Move[k][1]);
		  if(isLegal(map,tempP)){
			  pQue.push(AStarPoint(	tempP,						//Point(x,y)
					  	  	  	  	pnt::abs(tempP, Target),			//heuristic value
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
unsigned int GraphSearch::minCost(const vector<vector<unsigned int> > &map,
							 const Point<int> &point,
							 Point<int> &nextMove) {
	nextMove=point;	//0:Up	1:Down	2:Left	3:Right	4:None
	if(point==Target)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX otherwise it will overflow later on
	for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
		Point<int> p(point.x+Move[k][0],point.y+Move[k][1],k);

		//In the predicate,must check the bounding first.We don't use isLegal() because we're
		//going to find the minimal cost point in previously visited points
		if ((p.x>=0 && p.y>=0 && p.x < map.size() &&p.y < map[0].size())&& Checked[p.x][p.y]
					&& map[p.x][p.y] != Obstacle){
			if(CostMap[p.x][p.y] < min){
				min=CostMap[p.x][p.y];
				nextMove=p;
			}
		}
	}
	return min;
}

unsigned int GraphSearch::minCost(const vector<vector<unsigned int> > &map,
							 const unsigned int* const moveCost,
							 const Point<int> &point,
							 Point<int> &nextMove) {
	nextMove=point;//0:Up	1:Down	2:Left	3:Right	4:None
	if(point==Start)
		return 0;
	unsigned int min = CostMax;		//Don't use UINT_MAX because it will overflow later on

	//Append available surrounding grid to the queue
	Point<int> p(point.x,point.y,(point.dir-1)%point.DirectionSize);//check left turn,same grid
	//In the predicate,must check the bounding first.We don't use isLegal() because we're
	//going to find the minimal cost point in previously visited points
	if ((p.x>=0 && p.y>=0 && p.x < map.size() && p.y < map[0].size())&&
			Checked3D[p.dir][p.x][p.y]&&
			map[p.x][p.y] != Obstacle){
		if(CostMap3D[p.dir][p.x][p.y] < min){
			min=CostMap3D[p.dir][p.x][p.y];
			nextMove=p;
		}
	}
	p.set(point.x+Move[(point.dir+2)%p.DirectionSize][0],//check backward
		  point.y+Move[(point.dir+2)%p.DirectionSize][1],
		  point.dir);
	if(p.x>=0 && p.y>=0 && p.x < map.size() && p.y < map[0].size()&&Checked3D[p.dir][p.x][p.y]&&map[p.x][p.y] != Obstacle)
		if(CostMap3D[p.dir][p.x][p.y] < min){
			min=CostMap3D[p.dir][p.x][p.y];
			nextMove=p;
		}

	p.set(point.x,point.y,(point.dir+1)%point.DirectionSize);//check right turn,same grid
	if((p.x>=0 && p.y>=0 && p.x < map.size() && p.y < map[0].size())&&
			Checked3D[p.dir][p.x][p.y]&&
			map[p.x][p.y] != Obstacle){
		if(CostMap3D[p.dir][p.x][p.y] < min){
			min=CostMap3D[p.dir][p.x][p.y];
			nextMove=p;
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

bool GraphSearch::aStar(const vector<vector<unsigned int> > &map,
					  const unsigned int *const moveCost,
					  const Point<int> &start,
					  const Point<int> &target){
	setTarget(target);
	setStart(start);
	return aStar(map,moveCost);
}

bool GraphSearch::aStar(const vector<vector<unsigned int> > &map,
					  const unsigned int *const moveCost){
	Gradient3D.clear();
	Point<int> nextMove;
	for(size_t dir=0;dir<nextMove.DirectionSize;dir++){
		vector<vector<Point<int> > > gradient2D;
		for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
			vector<Point<int> > zero(map[i].size(), Point<int>(0, 0,nextMove.Up));
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
		Checked3D[dir][Start.x][Start.y] = true;
		CostMap3D[dir][Start.x][Start.y] =CostMax;	//Todo: init the value according to the action
	}
	CostMap3D[Start.dir][Start.x][Start.y] =0;

	//Similar to BFS,but we use minimal priority queue instead
	priority_queue<AStarPoint, vector<AStarPoint>, lowestF> pQue;
	pQue.push(AStarPoint(Start, map[Start.x][Start.y], pnt::abs(Target, Start)));
	Checked3D[Start.dir][Start.x][Start.y] = true;
	bool success = false;
	EffortCount = 1;			//Reset the EffortCount to count the search effort
	Route.clear();		//Clear the previous route
	unsigned int searchTime=0;
	while (!pQue.empty()) {
		cout<<searchTime++<<endl;
		if(searchTime==101)
			searchTime=101;
		AStarPoint curPos = pQue.top();
		Point<int> p = curPos.point;
		pQue.pop();
		Checked3D[p.dir][p.x][p.y] = true;
		//CostMap3D[p.dir][p.x][p.y]=curPos.f;
		printf("\n(%u,%u,%c) f=%u\n",p.x,p.y,DirSymbol[p.dir],curPos.f);
		if (p == Target) {
			success = true;
			break;
		}
		unsigned int g=curPos.g;

		//Append available surrounding grid to the queue
		Point<int> nextP(p.x+Move[(p.dir-1)%p.DirectionSize][0],
					p.y+Move[(p.dir-1)%p.DirectionSize][1],
					(p.dir-1)%p.DirectionSize);		//left turn,next grid
		unsigned int valueG;
			if(nextP.x>=0 && nextP.y>=0 && nextP.x<map.size()&&nextP.y<map[0].size()&&map[nextP.x][nextP.y]!=Obstacle){
				valueG=map[nextP.x][nextP.y]+moveCost[0]+g;
				if(pnt::abs(nextP, Target)+valueG<CostMap3D[nextP.dir][nextP.x][nextP.y]){
					pQue.push(AStarPoint(	nextP,						//Point(x,y)
											pnt::abs(nextP, Target),			//heuristic value
											valueG));	//use left turn cost
					  Gradient3D[nextP.dir][nextP.x][nextP.y] = p;
					  CostMap3D[nextP.dir][nextP.x][nextP.y]=pnt::abs(nextP, Target)+valueG;
					  //printf("  =)L (%u,%u,%c) f=%u\n",nextP.x,nextP.y,DirSymbol[nextP.dir],valueG+abs(nextP, Target));
				}
			  }

		nextP.set(p.x+Move[p.dir][0],p.y+Move[p.dir][1],p.dir);					//move forward
		if(nextP.x>=0 && nextP.y>=0 && nextP.x<map.size()&&nextP.y<map[0].size()&&map[nextP.x][nextP.y]!=Obstacle){
			valueG=map[nextP.x][nextP.y]+moveCost[1]+g;
			if(pnt::abs(nextP, Target)+valueG<CostMap3D[nextP.dir][nextP.x][nextP.y]){
				pQue.push(AStarPoint(	nextP,						//Point(x,y)
										pnt::abs(nextP, Target),			//heuristic value
										valueG));	//use left turn cost
				  Gradient3D[nextP.dir][nextP.x][nextP.y] = p;
				  CostMap3D[nextP.dir][nextP.x][nextP.y]=pnt::abs(nextP, Target)+valueG;
				  //printf("  =)F (%u,%u,%c) f=%u\n",nextP.x,nextP.y,DirSymbol[nextP.dir],valueG+abs(nextP, Target));
			}
		  }

		nextP.set(p.x+Move[(p.dir+1)%p.DirectionSize][0],
				  p.y+Move[(p.dir+1)%p.DirectionSize][1],
				  (p.dir+1)%p.DirectionSize);						//right turn,same grid
		if(nextP.x>=0 && nextP.y>=0 && nextP.x<map.size()&&nextP.y<map[0].size()&&map[nextP.x][nextP.y]!=Obstacle){
			valueG=map[nextP.x][nextP.y]+moveCost[2]+g;
			if(pnt::abs(nextP, Target)+valueG<CostMap3D[nextP.dir][nextP.x][nextP.y]){
				pQue.push(AStarPoint(	nextP,						//Point(x,y)
										pnt::abs(nextP, Target),			//heuristic value
										valueG));	//use left turn cost
				  Gradient3D[nextP.dir][nextP.x][nextP.y] = p;
				  CostMap3D[nextP.dir][nextP.x][nextP.y]=pnt::abs(nextP, Target)+valueG;
				  //printf("  =)R (%u,%u,%c) f=%u\n",nextP.x,nextP.y,DirSymbol[nextP.dir],valueG+abs(nextP, Target));
			}
		  }
	}

	//Test: Output CostMasp3D
	for(size_t k=0;k<4;k++){
		cout<<"layer: "<<k<<endl;
		for(size_t i=0;i<map.size();i++){	//Test output the costMap
			for(size_t j=0;j<map[i].size();j++){
			cout<<CostMap3D[k][i][j]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	//Test: Output Gradient3D
	//for(size_t k=0;k<4;k++){
	//		cout<<"layer: "<<k<<endl;
	//		for(size_t i=0;i<map.size();i++){	//Test output the costMap
	//			for(size_t j=0;j<map[i].size();j++){
	//			printf("(%u,%u,%c)\t",Gradient3D[k][i][j].x,Gradient3D[k][i][j].y,DirSymbol[Gradient3D[k][i][j].dir]);
	//			}
	//			cout<<endl;
	//		}
	//		cout<<endl;
	//	}
	if (success) {

		generateRoute(Gradient3D);
		return true;
	}
	return false;

}

bool GraphSearch::dpSearch(const vector<vector<unsigned int> > &map,
					  const Point<int> &start,const Point<int> &target) {
	setTarget(target);
	setStart(start);
	return dpSearch(map);
}

bool GraphSearch::dpSearch(const vector<vector<unsigned int> > &map) {
	Gradient.clear();
	for (size_t i = 0; i < map.size(); i++) {			//Init the direction map
		vector<Point<int> > zero(map[i].size(), Point<int>(0, 0));
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

	//DPPoint is basically a Point<int> with a 'cost',which is used to sort in the priority queue
	priority_queue<DPPoint, vector<DPPoint>, lessCost> pQue;
	pQue.push(DPPoint(Target,0));
	Point<int> nextMove;

	EffortCount = 1;			//Reset the EffortCount to count the search effort
	Route.clear();	//Clear the previous route
	while (!pQue.empty()) {
		DPPoint curPos = pQue.top();
		Point<int> p = curPos.point;
		pQue.pop();
		Checked[p.x][p.y] = true;

		CostMap[p.x][p.y] = minCost(map,p,nextMove)+map[p.x][p.y];
		//Point<int> the current point to the lowest cost point
		Gradient[p.x][p.y] = nextMove;

		//Append available surrounding grid to the queue
		for(size_t k=0;k<4;k++){		//Iterate Up,Down,Left,Right 4 direction
			Point<int> tempP(p.x+Move[k][0],p.y+Move[k][1]);
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

bool GraphSearch::dpSearch(const vector<vector<unsigned int> > &map,
					  const unsigned int *const moveCost,
					  const Point<int> &start,
					  const Point<int> &target){
	setTarget(target);
	setStart(start);
	return dpSearch(map,moveCost);
}

bool GraphSearch::dpSearch(const vector<vector<unsigned int> > &map,
					  const unsigned int *const moveCost) {

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
/*
vector<vector<unsigned int> > map = {
		{1, 0, 1, 1, 1},
		{1, 0, 1, 0, 1},
		{1, 1, 1, 1, 1},
		{0, 0, 1, 0, 0},
		{0, 0, 1, 0, 0},};

//Define the cost for each action,left turn is expensive in real life
//[0]:Left turn	[1]:Forward	[2]:Right turn
//unsigned int actionCost[]={100,2,4};
unsigned int actionCost[]={1,0,50};
*/
/*
{1, 0, 1, 1, 1},
{1, 0, 1, 0, 1},
{1, 1, 1, 1, 1},
{0, 0, 1, 0, 0},
{0, 0, 1, 0, 0},
*/
/*
 * unsigned int actionCost[]={15,0,10};
 * Point<int> start1(8,13),target1(0,13);
		{5,	20,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10},
		{5,	0,	0,	0,	15,	0,	10,	10,	0,	15,	0,	0,	0,	10},
		{5,	0,	15,	15,	15,	0,	0,	10,	0,	15,	0,	0,	0,	10},
		{5,	0,	15,	0,	15,	0,	0,	10,	15,	15,	0,	0,	0,	90},
		{5,	0,	15,	0,	15,	0,	0,	10,	15,	0,	0,	0,	0,	10},
		{5,	0,	15,	0,	15,	0,	0,	10,	20,	15,	0,	15,	15,	10},
		{5,	0,	15,	15,	15,	10,	10,	10,	0,	15,	15,	15,	0,	90},
		{5,	0,	0,	0,	0,	0,	0,	10,	0,	0,	15,	15,	15,	90},
		{5,	50,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5},
 */
/*
int main(void) {
   //Point<int> start(0,0),target(2,2);
   Point<int> start(8,13),target(2,2);
   Point<int> dummy;
   GraphSearch b;


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
*/
/*
   Point<int> start1(0,0,dummy.Down),target1(4,2);
   if (b.aStar(map,actionCost,start1,target1)) {
   		b.printRouteOnMap(map);
   	} else {
   		cout << "Search failed.\n";
   	}

   return 0;
}
*/
