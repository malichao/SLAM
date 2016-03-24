const float DT=0.02;      //Sampling period
float Q_angle  =  0.01;   //
float Q_gyro   =  0.0003;
float R_angle  =  0.01;
float x_bias = 0;
float y_bias = 0;
float XP_00 = 0, XP_01 = 0, XP_10 = 0, XP_11 = 0;
float YP_00 = 0, YP_01 = 0, YP_10 = 0, YP_11 = 0;
float KFangleX = 0.0;
float KFangleY = 0.0;

float kalmanFilterY(float accAngle, float gyroRate){
  float  y, S;
  float K_0, K_1;   //Kalman gain
 
  KFangleY += DT * (gyroRate - y_bias);
 
  YP_00 +=  - DT * (YP_10 + YP_01) + Q_angle * DT;
  YP_01 +=  - DT * YP_11;
  YP_10 +=  - DT * YP_11;
  YP_11 +=  + Q_gyro * DT;
 
  y = accAngle - KFangleY;
  S = YP_00 + R_angle;
  K_0 = YP_00 / S;
  K_1 = YP_10 / S;
 
  KFangleY +=  K_0 * y;
  y_bias  +=  K_1 * y;
  YP_00 -= K_0 * YP_00;
  YP_01 -= K_0 * YP_01;
  YP_10 -= K_1 * YP_00;
  YP_11 -= K_1 * YP_01;
 
  return KFangleY;
}

int main(){
  float AccYangle,rate_gyr_y;
  //float kalmanX = kalmanFilterX(AccXangle, rate_gyr_x);
  float kalmanY = kalmanFilterY(AccYangle, rate_gyr_y);
}