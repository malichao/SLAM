# SLAM
Robotics Simultaneous Localization and Mapping  
### Current Progress:  
#### [- LIDAR Sensor Data Acquisition and Visulization](https://github.com/malichao/SLAM/tree/master/LIDAR)  
#### [- Kalman Filter](/KalmanFilter)  
#### [- Map Search Algorithm Comparison(BFS, A* ,Dynamic Programming,RRT)](/Search/GraphSearch)  
#### [- Smooth Algorithm](/Search/GraphSearch)  
#### [- PID Simulator(one dimension)](/PID)  
#### [- Vehicle Dynamic Model Simulation](Search/GraphSearch)  
    
=====
    
###XV11_LIDAR_Visualizer
A small program to debug the XV-11 LIDAR module.Built with Qt and it's cross platform on Windows/Mac/Linux.
Here's the main features:  
-Visualizing the dot data with selectable range from 1-10 meters  
-Options to filter out the bad point data  
-Showing how many point data are valid on status bar  
-Showing all the text data with format 0 - 360   : speed /rpm | point1  point2  point3  point4  

![alt tag](LIDAR/snapshots/2016%20Feb%2004%20-4.jpg)   

![alt tag](LIDAR/snapshots/2016%20Feb%2004%20-5.jpg)   
  
  ===
###Kalman Filter  
Implemented a highly portable Kalman filter module in C.Here are some experiments of fusing gyroscope and accelerometer data.As we can see,Kalman filter does a good job fusing the sensors data and getting rid of gyro drifting.But it can not filter out the accelerometer spike noise.  
Notice that at the beginning of the red curve there’s a bump,which is caused by gyro drift and filtered out by Kalman filter later on.  
![alt text](KalmanFilter/doc/1-s.png)  
![alt text](KalmanFilter/doc/2-s.png)  
![alt text](KalmanFilter/doc/3-s.png)  
  
  ===
###Searching Algorithms
Following searching algorithms are implemented in this projects:  
-BFS 
-Dynamic Programing
-A* ,2D(x,y) and 3D(x,y,theta)  
-RRT Using Vehicle Dynamic Model,5D(x,y,theta,speed,steering speed)  
  
  
Here are the test results.  
##Comparison of BFS, DP, A* 
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
![alt text](SLAM/snapshot/search%20result-s.jpg)  
Where  
'T' is target  
'#' is obstacle  
"^,>,V,<" are trajectories  
'+' is an overlap of trajectories  
![alt text](Search/GraphSearch/doc/comparison-s.jpg)  
  
##RRT
RRT Searching with Smooth Path and Obstacle Avoidance  
![alt text](Search/GraphSearch/doc/RTT%20and%20Smooth/search_test1-result-smooth-avoid obstacle.bmp)  
  
RRT Searching Using Vehicle Dynamic Model  
![alt text](Search/GraphSearch/doc/RTT%20Vehicle/RRT_Vehicle_Search_Result-7.bmp)  
  
  
![alt text](Search/GraphSearch/doc/RTT%20Vehicle%20Maze%20Test/RRT_Vehicle_Search_Result1.bmp)  
    
    ===
#### ● Smooth algorithm  
After calculating the shortest path,we want to smooth the path.  
![alt text](https://github.com/malichao/SLAM/blob/master/SLAM/snapshot/search%20and%20smooth%20result-s.jpg)  
   
Here is another example of trajectories smooth algorithm:  
![alt text](https://github.com/malichao/SLAM/blob/master/SLAM/snapshot/search%20and%20smooth%20result2-s.jpg)  
   
   ===
##PID Control and Vehicle Kinematic Simulation  
Here's what have been done in this project:  
-Standard PID controller  
-PID twiddler(coefficient self optimization)  
-One dimensional Car simulation(speed,distance,acceleration,friction,wind resistance,etc)  
-Two dimensional Vehicle simulation(vehicle kinematic,dynamic model,etc)  
  
Here are some test results:  
##PID Control  
![alt text](PID/doc/pid1-s.png)  
  
##PID Coefficient Self Optimization  
![alt text](PID/doc/twiddle%20result.jpg)  
  
##2D Vehicle Simulation  
A vehicle steering at a fixed angle and fixed speed,whose trajectory is exactly a circle.  
![alt text](PID/doc/vehicle%20simulation.jpg)  
  
