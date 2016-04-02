/*
 * Vehicle.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: Lichao
 */
#include "Vehicle.h"

void Vehicle::setToStandardVehicle(){
	setToStandardCar();
	resetOrigin();
	setSystemLag(2);
	setNoise(0.4,0.7,0.1);	//Force noise,distance and speed reading noise

	Length=4.5;	// 4.5m
	Width=1.9;	// 1.9m
	TireCoefFront=1.5E5;	// 0.25 N/N/degree
	TireCoefRear =4E4;	// 0.25 N/N/degree
	Inertia= 1/12.0*Car::getMass()*(Length*Length+Width*Width);
	TireRadius= 0.35;	//0.35m
}

void Vehicle::drive(const double force,const double steeringAngle){
	Car::update(force);
	SpeedLongitudinal=Car::getVelocity();
	SteeringAngle=steeringAngle;
	updateDynamic(SpeedLongitudinal,steeringAngle);
}

void Vehicle::driveWithoutPID(const double speed,const double steeringAngle){
	SpeedLongitudinal=speed;
	SteeringAngle=steeringAngle;
	updateDynamic(speed,steeringAngle);
	//updateKinematic(speed,steeringAngle);
}

void Vehicle::updateKinematic(const double speed,const double steeringAngle){
	double xDot=speed*cos(Orientation);
	double yDot=speed*sin(Orientation);
	double yawDot=speed/Length*tan(steeringAngle);

	float t=Car::getPeriod();
	X+=xDot*t;
	Y+=yDot*t;
	Orientation+=yawDot*t;
	SpeedYaw=yawDot;
}

/*
void Vehicle::updateDynamic(const double speed,const double steeringAngle){
	float t=Car::getPeriod();
	float A,B,C,D,E,F;
	float cosSA=cos(steeringAngle);
	float coefCosSA=TireCoefFront*cosSA;

	SpeedLongitudinal=fabs(SpeedLongitudinal) <Car::Epsilon? Car::Epsilon :SpeedLongitudinal;

	//float slipeAngleFront=(SpeedLateral+Length/2.0*TireRadius)/SpeedLongitudinal - SteeringAngle;
	//float slipeAngleRear=(SpeedLateral+Length/2.0*TireRadius)/SpeedLongitudinal;

	float mass=Car::getMass();
	A=-(coefCosSA+TireCoefRear)/(mass*SpeedLongitudinal);
	B= (-Length/2.0*coefCosSA+Length/2.0*TireCoefRear)/(mass*SpeedLongitudinal)-SpeedLongitudinal;
	C= (-Length/2.0*coefCosSA+Length/2.0*TireCoefRear)/(Inertia*SpeedLongitudinal);
	D=-((coefCosSA + TireCoefRear)*Length*Length/4.0/(Inertia*SpeedLongitudinal));
	E= coefCosSA/mass;
	F= Length/2.0*coefCosSA/Inertia;

	float speedLateralDot =A*SpeedLateral + C*SpeedYaw + E*steeringAngle;
	float speedYawDot	  =B*SpeedLateral + D*SpeedYaw + F*steeringAngle;

	SpeedLateral += speedLateralDot * t;
	SpeedYaw += speedYawDot * t;

	float xDot= SpeedLongitudinal*cos(Orientation) - SpeedLateral*sin(Orientation);
	float yDot= SpeedLongitudinal*sin(Orientation) + SpeedLateral*cos(Orientation);
	float orienDot= SpeedYaw;

	X +=xDot * t;
	Y +=yDot * t;
	Orientation +=orienDot * t;
}
*/

/*
void Vehicle::update(const double speed,const double steeringAngle){
	float t=Car::getPeriod();
	float cosSA=cos(steeringAngle);
	float coefCosSA=TireCoefFront*cosSA;

	SpeedLongitudinal=fabs(SpeedLongitudinal) <Car::Epsilon? Car::Epsilon :SpeedLongitudinal;

	//float slipeAngleFront=(SpeedLateral+Length/2.0*TireRadius)/SpeedLongitudinal - SteeringAngle;
	//float slipeAngleRear=(SpeedLateral+Length/2.0*TireRadius)/SpeedLongitudinal;

	float mass=Car::getMass();
	float a11=-(TireCoefFront+TireCoefRear)/(mass*SpeedLongitudinal);
	float a12=(TireCoefRear-TireCoefFront)/Length/2.0/(mass*SpeedLongitudinal);
	float a21=(TireCoefRear-TireCoefFront)/Length/2.0/Inertia;
	float a22=-((TireCoefFront+TireCoefRear)*Length*Length/4.0/(Inertia*SpeedLongitudinal));
	float b11=TireCoefFront/(mass*SpeedLongitudinal);
	float b21=TireCoefFront*Length/2.0/Inertia;

	float beta=0.03;
	float xDot=SpeedLongitudinal*cos(steeringAngle+beta);
	float yDot=SpeedLongitudinal*sin(steeringAngle+beta);
	float yawDot=a21*beta+a22*SpeedYaw+b21*steeringAngle;


	X +=xDot * t;
	Y +=yDot * t;
	SpeedYaw+=yawDot*t;
	//Orientation +=orienDot * t;
}
*/


void Vehicle::updateDynamic(const double speed,const double steeringAngle){
	double mass=Car::getMass();
	double resistance=Car::getResistance();
	double p[]={mass,Length/2,Length/2,TireCoefFront,TireCoefRear,resistance};
	double dx[]={0,0,0};
	double x[]={SpeedLongitudinal,SpeedLateral,SpeedYaw};
	double y[]={0,0,0};
	double u[]={0.001,0.001,0,0,steeringAngle};
	compute_dx(dx,x,u,p);
	compute_y(y,x,u,p);

	double t=Car::getPeriod();
	SpeedLateral+=dx[1]*t;
	SpeedYaw+=dx[2]*t;

	double xDot= SpeedLongitudinal*cos(Orientation) - SpeedLateral*sin(Orientation);
	double yDot= SpeedLongitudinal*sin(Orientation) + SpeedLateral*cos(Orientation);
	double orienDot= SpeedYaw;

	X +=xDot * t;
	Y +=yDot * t;
	Orientation +=orienDot * t;
}


/* State equations. */
  void Vehicle::compute_dx(double *dx,double *x, double *u, double *p)
  {
      /* Retrieve model parameters. */
      double *m, *a, *b, *Cx, *Cy, *CA;
      m  = &p[0];   /* Vehicle mass.                    */
      a  = &p[1];   /* Distance from front axle to COG. */
      b  = &p[2];   /* Distance from rear axle to COG.  */
      Cx = &p[3];   /* Longitudinal tire stiffness.     */
      Cy = &p[4];   /* Lateral tire stiffness.          */
      CA = &p[5];   /* Air resistance coefficient.      */
      /* x[0]: Longitudinal vehicle velocity. */
      /* x[1]: Lateral vehicle velocity. */
      /* x[2]: Yaw rate. */
      dx[0] = x[1]*x[2]+1/m[0]*(Cx[0]*(u[0]+u[1])*cos(u[4])
              -2*Cy[0]*(u[4]-(x[1]+a[0]*x[2])/x[0])*sin(u[4])
              +Cx[0]*(u[2]+u[3])-CA[0]*pow(x[0],2));
      dx[1] = -x[0]*x[2]+1/m[0]*(Cx[0]*(u[0]+u[1])*sin(u[4])
              +2*Cy[0]*(u[4]-(x[1]+a[0]*x[2])/x[0])*cos(u[4])
              +2*Cy[0]*(b[0]*x[2]-x[1])/x[0]);
      dx[2] = 1/(pow(((a[0]+b[0])/2),2)*m[0])
              *(a[0]*(Cx[0]*(u[0]+u[1])*sin(u[4])
              +2*Cy[0]*(u[4]-(x[1]+a[0]*x[2])/x[0])*cos(u[4]))
              -2*b[0]*Cy[0]*(b[0]*x[2]-x[1])/x[0]);
  }

  /* Output equations. */
  void Vehicle::compute_y(double *y,double *x,double *u,double *p)
  {
      /* Retrieve model parameters. */
      double *m  = &p[0];   /* Vehicle mass.                    */
      double *a  = &p[1];   /* Distance from front axle to COG. */
      double *b  = &p[2];   /* Distance from rear axle to COG.  */
      double *Cx = &p[3];   /* Longitudinal tire stiffness.     */
      double *Cy = &p[4];   /* Lateral tire stiffness.          */
      /* y[0]: Longitudinal vehicle velocity. */
      /* y[1]: Lateral vehicle acceleration. */
      /* y[2]: Yaw rate. */
      y[0] = x[0];
      y[1] = 1/m[0]*(Cx[0]*(u[0]+u[1])*sin(u[4])
             +2*Cy[0]*(u[4]-(x[1]+a[0]*x[2])/x[0])*cos(u[4])
             +2*Cy[0]*(b[0]*x[2]-x[1])/x[0]);
      y[2] = x[2];
  }

void Vehicle::calculateVehicleState( const Vehicle::VehicleInput &vi,
									 const Vehicle::VehicleState &vs,
									 Vehicle::VehicleState &vsNew){

	vsNew.speedLongitudinal =
			fabs(vs.speedLongitudinal) < Car::Epsilon ?
					Car::Epsilon : vs.speedLongitudinal;

	float mass=Car::getMass();
	float beta=3.14/180.0*3;

	double v=vs.speedLongitudinal/cos(beta);
	float a11=-(TireCoefFront+TireCoefRear)/(mass*v);
	float a12=(TireCoefRear-TireCoefFront)/Length/2.0/(mass*v*v);
	float b11 = TireCoefFront / (mass * v);

	float xDot = v * cos(vsNew.orientation + beta);
	float yDot = v * sin(vsNew.orientation + beta);
	float SpeedYawDot = a11 * beta + a12 * vs.speedYaw + b11 * vi.steerAngle;

	vsNew.x = vs.x+xDot * vi.period;
	vsNew.y = vs.y+yDot * vi.period;
	vsNew.orientation = vs.orientation+SpeedYawDot * vi.period * vi.period;
}
