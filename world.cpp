#include "world.h"
#include <gl/freeglut.h>

int f = 2;

void gravity(std::deque<Body*>& body)
{
	for (int i = 0; i < body.size(); i++)
	{
		if (!body[i]->planet)
			body[i]->applyG(10000, Vector2D(0,-1), body[i]->position.y + 1000);
	}
}

World::World()
{
	addCar(-2400, 30);
	addBox(0,0,6000,5,true);
	man.Initialize(-2300, 30);
	Body* b; Joint* j;
	while (b = man.RegisterBodies())
		solver.addBody(b);
	while (j = man.RegisterJoints())
		solver.addJoint(j);
	//initIK(-2350, 30);
	solver.SetGravityFunc(gravity);
}

void World::addCircle(double x, double y)
{
	mCircle circle(5);
	mShape* shape = &circle;
	Body* b = new Body(&shape, 1, x, y, 1, false);
	solver.addBody(b);
}

Body* World::addBox(double x, double y, double w, double h, bool fixed)
{
	mPolygon poly;
	mShape* shape = &poly;
	poly.SetBox(w/2,h/2);
	Body* b = new Body(&shape, 1, x, y, 1, fixed);
	solver.addBody(b);
	b->restitution = .3;
	return b;
}

void World::addLinked(double x, double y)
{
	Body* b1 = addBox(x-5, y, 10, 10, false);
	Body* b2 = addBox(x+5, y, 10, 10, false);
	b1->restitution = .3;
	b2->restitution = .3;
	RevJoint* j = new RevJoint(b1, b2, Vector2D(5, -5), Vector2D(-5, -5));
	solver.addJoint(j);
}

void World::addCar(double x, double y)
{
	mPolygon poly;
	mShape* shape = &poly;
	Vector2D vertices[] = {
		Vector2D(-20, -8),
		Vector2D(20, -8),
		Vector2D(20, -2),
		Vector2D(0, 8),
		Vector2D(-15, 8),
		Vector2D(-20, 0)
	};
	poly.Set(vertices, 6);
	Body* b1 = new Body(&shape, 1, x, y, 1, false);
	mCircle circle(5);
	shape = &circle;
	Body* b2 = new Body(&shape, 1, x-14, y-9, 1, false);
	Body* b3 = new Body(&shape, 1, x+13, y-9, 1, false);
	solver.addBody(b1);
	solver.addBody(b2);
	solver.addBody(b3);
	b1->filtergroup = f;
	b2->filtergroup = f;
	b3->filtergroup = f;
	f++;
	b1->restitution = .3;
	b2->restitution = .3;
	b3->restitution = .3;
	b2->Friction = 2;
	b3->Friction = 2;
	WheelJoint* j1 = new WheelJoint(b1, b2, Vector2D(-14, -9), Vector2D(0, 0),
		Vector2D(0,1), 4, .7);
	WheelJoint* j2 = new WheelJoint(b1, b3, Vector2D(13, -9), Vector2D(0, 0),
		Vector2D(0,1), 4, .7);
	j1->SetMotor(true, 0, 1000);
	j2->SetMotor(true, 0, 1000);
	solver.addJoint(j1);
	solver.addJoint(j2);
	car = b1;
	motor = j1;
}

void World::addRamp(double x, double y)
{
	mPolygon poly;
	mShape* shape = &poly;
	Vector2D vertices[] = {
		Vector2D(-20, -8),
		Vector2D(20, -8),
		Vector2D(20, 8)
	};
	poly.Set(vertices, 3);
	Body* b = new Body(&shape, 1, x, y, 1, false);
	solver.addBody(b);
}

void World::addStack(double x, double y)
{
	for (int i = 0; i < 5; i++)
		addBox(x, y + i*10, 10, 10, false);
}

void World::addRope(double x, double y)
{
	Body* b1 = addBox(x, y, 5, 10, true);
	Body* bp = b1;
	b1->filtergroup = f;
	for (int i = 1; i < 20; i++)
	{
		Body* b2 = addBox(x, y-(i*10), 5, 10, false);
		RevJoint* j = new RevJoint(bp, b2, Vector2D(0, -5), Vector2D(0, 5));
		solver.addJoint(j);
		b2->filtergroup = f;
		bp = b2;
	}
	f++;
}

void World::addBridge(double x, double y)
{
	Body* b1 = addBox(x, y, 20, 5, true);
	Body* b2 = addBox(x+200, y, 20, 5, true);
	b1->filtergroup = f;
	b2->filtergroup = f;
	Body* bp = b1;
	for (int i = 1; i < 10; i++)
	{
		Body* bc = addBox(x+(i*20), y, 20, 5, false);
		RevJoint* j = new RevJoint(bp, bc, Vector2D(10, 0), Vector2D(-10, 0));
		solver.addJoint(j);
		bp = bc;
		bc->filtergroup = f;
	}
	RevJoint* j = new RevJoint(bp, b2, Vector2D(10, 0), Vector2D(-10, 0));
	solver.addJoint(j);
	f++;
}

void World::addComposite(double x, double y)
{
	mPolygon poly1;
	mPolygon poly2;
	mShape* shapes[] = {
		&poly1,
		&poly2
	};
	Vector2D vertices1[] = {
		Vector2D(0,10),
		Vector2D(8,-10),
		Vector2D(0,-2)
	};
	Vector2D vertices2[] = {
		Vector2D(0,-2),
		Vector2D(-8,-10),
		Vector2D(0,10)
	};
	poly1.Set(vertices1, 3);
	poly2.Set(vertices2, 3);
	Body* b = new Body(shapes, 2, x, y, 1, false);
	solver.addBody(b);
}

void World::addMagnet(double x, double y)
{
	Body* b = addBox(x, y, 10, 20, false);
	Magnet* m1 = new Magnet(b, Vector2D(0, 8), 500, true);
	Magnet* m2 = new Magnet(b, Vector2D(0, -8), 500, false);
	solver.addMagnet(m1);
	solver.addMagnet(m2);
}

void World::setTarget(double x, double y)
{
	ik->SetTarget(Vector2D(x, y));
}

void World::initIK(double x, double y)
{
	Body* stat,* arm,* forearm;
	stat = addBox(x, y, 2, 2, true);
	arm = addBox(x+10, y, 20, 4, false);
	mPolygon poly;
	mShape* shape = &poly;
	Vector2D vertices[] = {
		Vector2D(-10, -2),
		Vector2D(10, 0),
		Vector2D(-10, 2),
	};
	poly.Set(vertices, 3);
	forearm = new Body(&shape, 1, x+30, y, 1, false);
	solver.addBody(forearm);
	Joint* shoulder,* elbow;
	stat->filtergroup = f;
	arm->filtergroup = f;
	forearm->filtergroup = f++;
	shoulder = new RevJoint(stat, arm, Vector2D(0,0), Vector2D(-9,0));
	elbow = new RevJoint(arm, forearm, Vector2D(9,0), Vector2D(-9,0));
	solver.addJoint(shoulder);
	solver.addJoint(elbow);
	ik = new IKJoint((RevJoint*)elbow, Vector2D(10, 0));
	ik->AddJoint((RevJoint*)shoulder);
}

void World::drawBodies(bool debug)
{
	solver.drawBodies(debug);
	if (debug)
	{
		glBegin(GL_POINTS);
		glVertex2d(man.CoM.x, man.CoM.y);
		glEnd();
	}
}

void World::Solve(int time)
{
	man.Upright(Vector2D(0,-1));
	solver.Solve(time);
}