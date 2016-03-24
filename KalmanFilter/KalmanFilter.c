struct KFParameter{
  float dt;       //Sampling period
  float Q[2];     //Process noise
  float R;        //Measurement noise
  float H[2];     //Measurement Matrix
  float P[2][2];  //Error covariance
  float X[2];     //State of the system
};

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
float kalmanFilter(KFParameter *p,float accAngle, float gyroRate){
  float dt=p->dt;
  /*
  Step 1. Predict the next state
    Take measuring car angle using gyro as an example:
    X(k)=|Angle(k)| = Angle(k-1) + Speed(k-1)*dt
         |Speed(k)| =              Speed(k-1)
    yield
    X(k)=A*X(k-1)=|1 dt| *X(k-1)
                  |0  1|
  */
  p->X[0] += dt * (gyroRate - p->X[1]);
  
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
  p->P[0][0] +=  - dt * (p->P[1][0] + p->P[0][1]) + p->Q[0] * dt;
  p->P[0][1] +=  - dt * p->P[1][1];
  p->P[1][0] +=  - dt * p->P[1][1];
  p->P[1][1] +=  + p->Q[1] * dt;
 
 /*
  Step3. Update the Kalman gain
  Since the accelerometer only measure the angle not the 
  angular speed,so
    H=|1|
      |0|
 */
  float Z[2];
  float denominator;
  float K[2];     //Kalman gain

  Z[0]=accAngle;
  Z[1]=0;
  //denominator = p->P[0][0] + p->R;
  K[0] = p->P[0][0] / (p->P[0][0] + p->R);
  K[1] = p->P[1][0] / (p->P[1][0] + p->R);
 
  /*
  Step4. Update estimate
 */
  p->X[0] += K[0] * (Z[0] - p->X[0]);
  p->X[1] += K[1] * (Z[0] - p->X[0]);

  /*
  Step5. Update error covariance
 */
  p->P[0][0] -=  K[0] * p->P[0][0];
  p->P[0][1] -=  K[0] * p->P[0][1];
  p->P[1][0] -=  K[1] * p->P[0][0];
  p->P[1][1] -=  K[1] * p->P[0][1];

  return p->X[0];
}

/*
int main(){
  float accAngle,gyroRate,fuseAngle;
  KFParameter kfp={ .dt=0.5,            //Sampling time
                    .Q={0.0001,0.0001},     //Process noise,usually very small
                    .R=2,               //Measurement noise
                    .H={1,0},             //Measurement matrix,set to {1,0}
                    .P={{0,0},{0,0}},     //Error covariance,init to 0
                    .X={0,0}};            //System state,init to 0
  KFParameter *p=&kfp;

  for(float i=0;i<500;i+=p->dt){
    fuseAngle=kalmanFilter(p,accAngle, gyroRate);
    printf("%.3f,%.3f,%.3f\n",accAngle,gyroRate,fuseAngle);
  }
}
*/
