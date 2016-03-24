/******************************************************************************
Author		: Lichao Ma
Date  		: Mar 23,2016
Description :
	Kalman filter for accelerometer and gyroscope fusion.All the intermediate
	data are store in the struct and thus it available for multiple sensors
	fusion.
	
	A usable setting for accelerometer and gyro:
  KFParameter kfp={ .dt=0.5,            //Sampling time
                    .R=2,               //Measurement noise
                    .Q={0.0001,0.0001},     //Process noise,usually very small
                    .H={1,0},             //Measurement matrix,set to {1,0}
                    .P={{0,0},{0,0}},     //Error covariance,init to 0
                    .X={0,0}};            //System state,init to 0
*****************************************************************************/

#ifndef KalmanFilter_H
#define KalmanFilter_H

struct KFParameter{
  float dt;       //Sampling period
  float R;        //Measurement noise covariance
  float coef;     //Tuning coefficient of Q,usually 0~1
  float Q[2];     //Process noise covariance
  float H[2];     //Measurement Matrix
  float P[2][2];  //Error covariance
  float X[2];     //State of the system
};

void initKalmanFilter(KFParameter *p,float sampleTime,float R);
float kalmanFilter(KFParameter *p,float accAngle, float gyroRate);
void setKFCoef(KFParameter *p,float coef);
void setKFMeasureNoise(KFParameter *p,float R);

#endif