#ifndef WORLD_H
#define WORLD_H

#include "solver.h"
#include "man.h"

class World{
	public:
		World();
		
		//testing functions
		Body* addBox(double x, double y, double w, double h, bool fixed);
		void addCircle(double x, double y);
		void addLinked(double x, double y);
		void addCar(double x, double y);
		void addRamp(double x, double y);
		void addStack(double x, double y);
		void addRope(double x, double y);
		void addBridge(double x, double y);
		void addComposite(double x, double y);
		void addMagnet(double x, double y);
		void setTarget(double x, double y);
		void initIK(double x, double y);
		
		void drawBodies(bool debug);
		void Solve(int time);
		double GetProfile(std::string s) {return solver.GetProfile(s);}
		
		void setSpeed(double speed) {motor->SetMotor(true, speed, 100000);}
		void setBrake() {motor->SetMotor(true, 0, 200000);}
		void setOff() {motor->SetMotor(true, 0, 1000);}
		Vector2D getPos() {return car->position;}
	private:
		Solver solver;
		WheelJoint* motor;
		Man2 man;
		Body* car;
		
		//test the IK
		IKJoint* ik;
};

#endif