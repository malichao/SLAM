/*
 * Vehicle.h
 *
 *  Created on: Mar 28, 2016
 *      Author: Lichao
 */

#ifndef SRC_VEHICLE_H_
#define SRC_VEHICLE_H_
#include "Car.h"

class Vehicle: public Car {
public:
	struct VehicleState{
		double x;
		double y;
		double orientation;
		double speedLongitudinal;
		double speedYaw;
	};
	struct VehicleAttribute{
		double mass;
		double re
	};
	Vehicle():  Length(1),Width(1),X(0),Y(0),Orientation(0),SpeedLateral(Car::Epsilon),SpeedYaw(0),
				SpeedLongitudinal(0),SteeringAngle(0),Thrust(0),
				TireCoefFront(0),TireCoefRear(0),Inertia(0),TireRadius(0){};
	void setToStandardVehicle();
	void setState(	const double x,const double y,const double o,
					const double sLa,const double sLo,const double sY){
			X=x;
			Y=y;
			Orientation=o;
			SpeedLateral=sLa;
			SpeedLongitudinal=sLo;
			SpeedYaw=sY;}
	void setSteeringAngle(const double sA){ SteeringAngle=sA; }

	void drive(const double force, const double steeringAngle);
	void driveWithoutPID(const double speed,const double steeringAngle);

	double getX() const { return X; }
	double getY() const { return Y; }
	double getSpeedYaw() const { return SpeedYaw; }
	double getOrientation() const { return Orientation; }
	double getSpeedLateral() const { return SpeedLateral; }
	double getSpeedLongitudinal() const { return SpeedLongitudinal; }



private:
	double Length;
	double Width;

	double X;
	double Y;
	double Orientation;
	double SpeedLateral;
	double SpeedYaw;

	double SpeedLongitudinal;
	double SteeringAngle;
	double Thrust;

	double TireCoefFront;
	double TireCoefRear;
	//double SlipeAngleFront;
	//double SlipeAngleRear;
	double Inertia;
	double TireRadius;

	void updateKinematic(const double speed,const double steeringAngle);
	void updateDynamic(const double speed,const double steeringAngle);
	void compute_dx(double *dx, double *x, double *u, double *p);
	void compute_y(double *y, double *x, double *u, double *p);
};



#endif /* SRC_VEHICLE_H_ */
