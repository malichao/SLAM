float DT=0.02;                //Sampling period
float Q[2]={0.01,0.003};      //Process noise
float Rk  =  0.01;            //Measurement noise
float K[2];                   //Kalman gain
float Pk[2][2]={{0,0},{0,0}}; //Error covariance
float H[2]={1,0};             //Measurement Matrix
float Xk[2]={0,0};            //State of the system
/*
Quick RECAP of state space matrix:
A general linear system could be described as
  Xdot = AX + Bu
  Y    = CX + D
  where 
    X is the state of the system
    Y is the output of the system
    u is the control signal
    Xdot is the derivative of the state X
    A is the physical equation of the system 
    B is a control matrix
    C is a observe matrix
    D is a constant matrix
*/
float kalmanFilter(float accAngle, float gyroRate){
  /*
  Step 1. Predict the next state
    Take measuring car angle using gyro as an example:
    X(k)=|Angle(k)| = Angle(k-1) + Speed(k-1)*dt
         |Speed(k)| =              Speed(k-1)
    yield
    X(k)=A*X(k-1)=|1 dt| *X(k-1)
                  |0  1|
  */
  Xk[0] += DT * (gyroRate - Xk[1]);
  
  /*
  Step2. Update the error covariance
    P=AP(AT) + Q
    where
      P is the corvariance of Angle(A) and Speed(S)
      P=|Corv(AA) Corv(AS)|
        |Corv(SA) Corv(SS)|
      (AT) is the transpose of A(Matrix,not Angle)
      Q is the process noise
  */
  Pk[0][0] +=  - DT * (Pk[1][0] + Pk[0][1]) + Q[0] * DT;
  Pk[0][1] +=  - DT * Pk[1][1];
  Pk[1][0] +=  - DT * Pk[1][1];
  Pk[1][1] +=  + Q[1] * DT;
 
 /*
  Step3. Update the Kalman gain
  Since the accelerometer only measure the angle not the 
  angular speed,so
    H=|1|
      |0|
 */
  float Zk[2];
  float denominator;

  Zk[0]=accAngle;
  Zk[1]=0;
  denominator = Pk[0][0] + Rk;
  K[0] = Pk[0][0] / denominator;
  K[1] = Pk[1][0] / denominator;
 
  /*
  Step4. Update estimate
 */
  Xk[0] += K[0] * (Zk[0] - Xk[0]);
  Xk[1] += K[1] * (Zk[1] - Xk[1]);

  /*
  Step5. Update error covariance
 */
  Pk[0][0] -=  K[0] * Pk[0][0];
  Pk[0][1] -=  K[0] * Pk[0][1];
  Pk[1][0] -=  K[1] * Pk[0][0];
  Pk[1][1] -=  K[1] * Pk[0][1];

  return Xk[0];
}

int main(){
  float AccYangle,rate_gyr_y;
  //float kalmanX = kalmanFilterX(AccXangle, rate_gyr_x);
  float kalmanY = kalmanFilter(AccYangle, rate_gyr_y);
}