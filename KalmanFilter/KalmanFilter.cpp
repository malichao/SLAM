struct KFParameter{
  float dt;       //Sampling period
  float Q[2];     //Process noise
  float R;        //Measurement noise
  float H[2];     //Measurement Matrix
  float K[2];     //Kalman gain
  float P[2][2];  //Error covariance
  float X[2];     //State of the system
};
/*
float dt=0.02;                //Sampling period
float Q[2]={0.01,0.003};      //Process noise
float Rk  =  0.01;            //Measurement noise
float K[2];                   //Kalman gain
float P[2][2]={{0,0},{0,0}}; //Error covariance
float H[2]={1,0};             //Measurement Matrix
float X[2]={0,0};            //State of the system
*/
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
float kalmanFilter(KFParameter *kfp,float accAngle, float gyroRate){
  float dt=kfp->dt;
  /*
  Step 1. Predict the next state
    Take measuring car angle using gyro as an example:
    X(k)=|Angle(k)| = Angle(k-1) + Speed(k-1)*dt
         |Speed(k)| =              Speed(k-1)
    yield
    X(k)=A*X(k-1)=|1 dt| *X(k-1)
                  |0  1|
  */
  kfp->X[0] += dt * (gyroRate - kfp->X[1]);
  
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
  kfp->P[0][0] +=  - dt * (kfp->P[1][0] + kfp->P[0][1]) + kfp->Q[0] * dt;
  kfp->P[0][1] +=  - dt * kfp->P[1][1];
  kfp->P[1][0] +=  - dt * kfp->P[1][1];
  kfp->P[1][1] +=  + kfp->Q[1] * dt;
 
 /*
  Step3. Update the Kalman gain
  Since the accelerometer only measure the angle not the 
  angular speed,so
    H=|1|
      |0|
 */
  float Z[2];
  float denominator;

  Z[0]=accAngle;
  Z[1]=0;
  denominator = kfp->P[0][0] + kfp->R;
  kfp->K[0] = kfp->P[0][0] / denominator;
  kfp->K[1] = kfp->P[1][0] / denominator;
 
  /*
  Step4. Update estimate
 */
  kfp->X[0] += kfp->K[0] * (Z[0] - kfp->X[0]);
  kfp->X[1] += kfp->K[1] * (Z[1] - kfp->X[1]);

  /*
  Step5. Update error covariance
 */
  kfp->P[0][0] -=  kfp->K[0] * kfp->P[0][0];
  kfp->P[0][1] -=  kfp->K[0] * kfp->P[0][1];
  kfp->P[1][0] -=  kfp->K[1] * kfp->P[0][0];
  kfp->P[1][1] -=  kfp->K[1] * kfp->P[0][1];

  return kfp->X[0];
}

int main(){
  float AccAngle,gyroRate;
  KFParameter kfp={ .dt=0.02,         //Sampling time
                    .Q={0.01,0.03},   //Process noise
                    .R=0.01,          //Measurement noise
                    .H={1,0},         //Measurement matrix,set to {1,0}
                    .K={0,0},         //Kalman gain,init to 0
                    .P={{0,0},{0,0}}, //Error covariance,init to 0
                    .X={0,0}};        //System state,init to 0
  KFParameter *p=&kfp;
  float kalmanY = kalmanFilter(p,AccAngle, gyroRate);
}