#ifndef MAN_H
#define MAN_H

#include "body.h"
#include "joint.h"
#include <vector>

class Man{
	public:
		void Initialize(double x, double y);
		Body* RegisterBodies();
		Joint* RegisterJoints();
	private:
		void Rotateto(int j, double angle);
		std::vector<Body*> body;
		std::vector<Joint*> joint;
		int b, j;
};

class Man2{
	public:
		void Initialize(double x, double y);
		Body* RegisterBodies();
		Joint* RegisterJoints();
		void Upright(Vector2D down);
		Vector2D CoM;
	private:
		void SetCoM();
		void Rotateto(int j, double angle);
		std::vector<Body*> body;
		std::vector<Joint*> joint;
		int b, j;
};

#endif