# SLAM
Robotics Simultaneous Localization and Mapping  
### Current Progress:  
#### [● LIDAR sensor data acquisition and visulization](https://github.com/malichao/SLAM/tree/master/LIDAR)  
#### [● Map search algorithm comparison(BFS, A* ,Dynamic programming,3-dimensional A* )](https://github.com/malichao/SLAM/tree/master/SLAM)  
#### [● Smooth algorithm](https://github.com/malichao/SLAM/tree/master/SLAM)  
#### [● PID simulator(one dimension)](https://github.com/malichao/SLAM/tree/master/PID)  
    
=====
    
#### ● LIDAR sensor data acquisition and visulization  
![alt text](https://github.com/malichao/SLAM/blob/master/LIDAR/snapshots/2016%20Feb%2004%20-3.jpg)  
![alt text](https://github.com/malichao/SLAM/blob/master/LIDAR/snapshots/2016%20Feb%2004%20-5.jpg)  
  
#### ● Map search algorithm comparison(BFS,A* , Dynamic programming, 3-dimensional A* )  
![alt text](https://github.com/malichao/SLAM/blob/master/Search/GraphSearch/doc/comparison-s.jpg)  
  
In real life,situation is more complicated.For example,if we are heading to a place in rush hour and we need to go through a traffic light.Then perhaps we don't want to take a left turn because it's gonna wait for 5 minutes.Instead we may want to take a detour,which is faster then taking a left turn.This problem can be solved by adding a third dimension,which is orientation factor.Consider the following example:  
start(4,2)  
target(0,0)  
{1, 0, 1, 1, 1},  
{1, 0, 1, 0, 1},  
{1, 1, 1, 1, 1},  
{0, 0, 1, 0, 0},  
{0, 0, 1, 0, 0},	 
    
Using three dimensional(x,y,orientation) A* we can get the following path:  
Search effort: 1  
Shortest path:14  
![alt text](https://github.com/malichao/SLAM/blob/master/SLAM/snapshot/search%20result-s.jpg)  
  
Where  
'T' is target  
'#' is obstacle  
"^,>,V,<" are trajectories  
'+' is an overlap of trajectories  
    
#### ● Smooth algorithm  
After calculating the shortest path,we want to smooth the path.  
![alt text](https://github.com/malichao/SLAM/blob/master/SLAM/snapshot/search%20and%20smooth%20result-s.jpg)  
   
Here is another example of trajectories smooth algorithm:  
![alt text](https://github.com/malichao/SLAM/blob/master/SLAM/snapshot/search%20and%20smooth%20result2-s.jpg)  
   
#### ● PID simulator(one dimension) 
Here is an one dimensional PID controller example,in which a car is trying to reach at 100m position. The following three curves show the result of different PID coefficients.  
![alt text](https://github.com/malichao/SLAM/blob/master/PID/doc/pid1-s.png)   
  
