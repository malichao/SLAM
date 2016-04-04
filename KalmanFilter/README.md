#Kalman Filter  
Implemented a highly portable Kalman filter module in C.Here are some experiments of fusing gyroscope and accelerometer data.As we can see,Kalman filter does a good job fusing the sensors data and getting rid of gyro drifting.But it can not filter out the accelerometer spike noise.  
Notice that at the beginning of the red curve there’s a bump,which is caused by gyro drift and filtered out by Kalman filter later on.  
![alt text](doc/1-s.png)  
![alt text](doc/2-s.png)  
![alt text](doc/3-s.png)  

