const float DT=0.02;      //Sampling period
float Q_angle  =  0.01;   //Process noise
float Q_gyro   =  0.0003; //Process noise
float Q[2]={0.01,0.003};  //Process noise
float Rk  =  0.01;   //Measurement noise
float x_bias = 0;
float y_bias = 0;
float XP_00 = 0, XP_01 = 0, XP_10 = 0, XP_11 = 0;
float YP_00 = 0, YP_01 = 0, YP_10 = 0, YP_11 = 0;
float KFangleX = 0.0;
float Xk = 0.0;

float kalmanFilterY(float accAngle, float gyroRate){
  float  y, denominator;
  float K[2];   //Kalman gain
  float Zk=accAngle;
  float numerator[2][2]={{0,0},{0,0}};
  float Pk[2][2]={{0,0},{0,0}};
  /*
  Step 1. Predict the next state
    Take measuring car angle using gyro as an example:
    X(k)=|Angle(k)| = Angle(k-1) + Speed(k-1)*dt
         |Speed(k)| =              Speed(k-1)
    yield
    X(k)=A*X(k-1)=|1 dt| *X(k-1)
                  |0  1|
  */
  Xk += DT * (gyroRate - y_bias);
  
  /*
  Step2. Update the error covariance
    P=AP(AT) + Q
    where
      (AT) is the transpose of A
      Q is the process noise
  */
  Pk[0][0] +=  - DT * (Pk[1][0] + Pk[0][1]) + Q[0] * DT;
  Pk[0][1] +=  - DT * Pk[1][1];
  Pk[1][0] +=  - DT * Pk[1][1];
  Pk[1][1] +=  + Q[1] * DT;
 
 /*
  Step3. Update the Kalman gain
 */
  y = Zk - Xk;
  denominator = YP_00 + Rk;
  K[0] = YP_00 / denominator;
  K[1] = YP_10 / denominator;
 
  Xk +=  K[0] * y;
  y_bias  +=  K[1] * y;
  YP_00 -= K[0] * YP_00;
  YP_01 -= K[0] * YP_01;
  YP_10 -= K[1] * YP_00;
  YP_11 -= K[1] * YP_01;
 
  return Xk;
}

int main(){
  float AccYangle,rate_gyr_y;
  //float kalmanX = kalmanFilterX(AccXangle, rate_gyr_x);
  float kalmanY = kalmanFilterY(AccYangle, rate_gyr_y);
}