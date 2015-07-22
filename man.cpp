#include "man.h"
#include <fstream>

void Man::Initialize(double x, double y)
{
	Vector2D list1[] = {
		Vector2D(-3, 6),
		Vector2D(3, 6),
		Vector2D(1, -5),
		Vector2D(-2, -5)
	};
	Vector2D list2[] = {
		Vector2D(-2, 5),
		Vector2D(2, 5),
		Vector2D(1, -4),
		Vector2D(-1, -4)
	};
	Vector2D list3[] = {
		Vector2D(-1, 4),
		Vector2D(1, 4),
		Vector2D(0, -4)
	};
	Vector2D list4[] = {
		Vector2D(-2, 3),
		Vector2D(2, 3),
		Vector2D(1, -2),
		Vector2D(-2, -2)
	};
	Vector2D list5[] = {
		Vector2D(-1, 4),
		Vector2D(2, 4),
		Vector2D(2, -4),
		Vector2D(-1, -4)
	};
	Vector2D list6[] = {
		Vector2D(-1, 4),
		Vector2D(2, 4),
		Vector2D(1, -4),
		Vector2D(-1, -4)
	};
	mPolygon poly1, poly2, poly3, poly4, poly5, poly6;
	mShape* shape = &poly3;
	poly1.Set(list1, 4);
	poly2.Set(list2, 4);
	poly3.Set(list3, 3);
	poly4.Set(list4, 4);
	poly5.Set(list5, 4);
	poly6.Set(list6, 4);
	//back forearm = 	0
	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly2;
	//back arm =		1
	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly6;
	//back calf =		2
	body.emplace_back(new Body(&shape, 1, x, y-16, 1, false));
	shape = &poly5;
	//back thigh =		3
	body.emplace_back(new Body(&shape, 1, x, y-8, 1, false));
	shape = &poly1;
	//torso =			4
	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly4;
	//head =			5
	body.emplace_back(new Body(&shape, 1, x, y+6, 1, false));
	shape = &poly5;
	//front thigh =		6
	body.emplace_back(new Body(&shape, 1, x, y-8, 1, false));
	shape = &poly6;
	//front calf =		7
	body.emplace_back(new Body(&shape, 1, x, y-16, 1, false));
	shape = &poly2;
	//front arm =		8
	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly3;
	//front forearm =	9
	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	//back shoulder joint =		0
	joint.emplace_back(new RevJoint(body[4], body[1], Vector2D(0, 5), Vector2D(0, 4.5)));
	//((RevJoint*)joint.back())->SetLimit(true, pi, -(3*pi)/8);
	//front shoulder joint =	1
	joint.emplace_back(new RevJoint(body[4], body[8], Vector2D(0, 5), Vector2D(0, 4.5)));
	//((RevJoint*)joint.back())->SetLimit(true, pi, -(3*pi)/8);
	//back elbow joint =		2
	joint.emplace_back(new RevJoint(body[1], body[0], Vector2D(0, -4.5), Vector2D(0, 3)));
	((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, 0);
	//front elbow joint = 		3
	joint.emplace_back(new RevJoint(body[8], body[9], Vector2D(0, -4.5), Vector2D(0, 3)));
	((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, 0);
	//neck joint = 				4
	joint.emplace_back(new RevJoint(body[4], body[5], Vector2D(0, 5.5), Vector2D(0, -2.5)));
	((RevJoint*)joint.back())->SetLimit(true, pi/6, -pi/6);
	((RevJoint*)joint.back())->SetMotor(true, 0, 100000);
	//back hip joint =			5
	joint.emplace_back(new RevJoint(body[4], body[3], Vector2D(-.5, -5), Vector2D(.5, 4)));
	((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, -pi/6);
	//front hip joint =			6
	joint.emplace_back(new RevJoint(body[4], body[6], Vector2D(-.5, -5), Vector2D(.5, 4)));
	((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, -pi/6);
	//back knee joint =			7
	joint.emplace_back(new RevJoint(body[3], body[2], Vector2D(.5, -4), Vector2D(.5, 4)));
	((RevJoint*)joint.back())->SetLimit(true, 0, -(3*pi)/4);
	//front knee joint =		8
	joint.emplace_back(new RevJoint(body[6], body[7], Vector2D(.5, -4), Vector2D(.5, 4)));
	((RevJoint*)joint.back())->SetLimit(true, 0, -(3*pi)/4);
	
	mPolygon box;
	mCircle circle(2);
	shape = &box;
	box.SetBox(3, 5);
	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	body.back()->mass.iI = 0;
	body.back()->nodraw = true;
	shape = &circle;
	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	body.back()->mass.iI = 0;
	body.back()->nodraw = true;
	joint.emplace_back(new RevJoint(body[10], body[11], Vector2D(0, -21), Vector2D(0, 0)));
	joint.emplace_back(new RevJoint(body[10], body[4], Vector2D(0, 2), Vector2D(-.5, 4)));
	
	for (int i = 0; i < body.size(); i++)
		body[i]->filtergroup = 1;
	b = j = 0;
}

Body* Man::RegisterBodies()
{
	if (b == body.size())
		return NULL;
	return body[b++];
}

Joint* Man::RegisterJoints()
{
	if (j == joint.size())
		return NULL;
	return joint[j++];
}

void Man::Rotateto(int j, double angle)
{
	double curangle = ((RevJoint*)joint[j])->GetAngle();
	double diff = curangle - angle;
	if (std::abs(diff) < pi/180)
		((RevJoint*)joint[j])->SetMotor(true, 0, 20000);
	else if (curangle < angle)
		((RevJoint*)joint[j])->SetMotor(true, 1, 20000);
	else if (curangle > angle)
		((RevJoint*)joint[j])->SetMotor(true, -1, 20000);
}

//Man2//

void Man2::Initialize(double x, double y)
{
	Vector2D list1[] = {
		Vector2D(-3, 6),
		Vector2D(3, 6),
		Vector2D(1, -5),
		Vector2D(-2, -5)
	};
	Vector2D list2[] = {
		Vector2D(-2, 5),
		Vector2D(2, 5),
		Vector2D(1, -4),
		Vector2D(-1, -4)
	};
	Vector2D list3[] = {
		Vector2D(-1, 4),
		Vector2D(1, 4),
		Vector2D(0, -4)
	};
	Vector2D list4[] = {
		Vector2D(-2, 3),
		Vector2D(2, 3),
		Vector2D(1, -2),
		Vector2D(-2, -2)
	};
	Vector2D list5[] = {
		Vector2D(-1, 4),
		Vector2D(2, 4),
		Vector2D(2, -4),
		Vector2D(-1, -4)
	};
	Vector2D list6[] = {
		Vector2D(-1, 4),
		Vector2D(2, 4),
		Vector2D(1, -4),
		Vector2D(-1, -4)
	};
	Vector2D list7[] = {
		Vector2D(-1, 1),
		Vector2D(4, 0),
		Vector2D(4, -1),
		Vector2D(-1, -1)
	};
	mPolygon poly1, poly2, poly3, poly4, poly5, poly6, poly7;
	poly1.Set(list1, 4);
	poly2.Set(list2, 4);
	poly3.Set(list3, 3);
	poly4.Set(list4, 4);
	poly5.Set(list5, 4);
	poly6.Set(list6, 4);
	poly7.Set(list7, 4);
	{ /* BODIES */
	mShape* shape = &poly3;	body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly2;			body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly7;			body.emplace_back(new Body(&shape, 1, x, y-20, 1, false));
	shape = &poly6;			body.emplace_back(new Body(&shape, 1, x, y-16, 1, false));
	shape = &poly5;			body.emplace_back(new Body(&shape, 1, x, y-8, 1, false));
	shape = &poly1;			body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly4;			body.emplace_back(new Body(&shape, 1, x, y+6, 1, false));
	shape = &poly5;			body.emplace_back(new Body(&shape, 1, x, y-8, 1, false));
	shape = &poly6;			body.emplace_back(new Body(&shape, 1, x, y-16, 1, false));
	shape = &poly7;			body.emplace_back(new Body(&shape, 1, x, y-20, 1, false));
	shape = &poly2;			body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	shape = &poly3;			body.emplace_back(new Body(&shape, 1, x, y, 1, false));
	}
	//back forearm = 	0
	//back arm =		1
	//back foot =		2
	//back calf =		3
	//back thigh =		4
	//torso =			5
	//head =			6
	//front thigh =		7
	//front calf =		8
	//front foot =		9
	//front arm =		10
	//front forearm =	11
	{ /* JOINTS */
		joint.emplace_back(new RevJoint(body[5], body[1], Vector2D(0, 5), Vector2D(0, 4.5)));
		//((RevJoint*)joint.back())->SetLimit(true, pi, -(3*pi)/8);
		joint.emplace_back(new RevJoint(body[5], body[10], Vector2D(0, 5), Vector2D(0, 4.5)));
		//((RevJoint*)joint.back())->SetLimit(true, pi, -(3*pi)/8);
		joint.emplace_back(new RevJoint(body[1], body[0], Vector2D(0, -4.5), Vector2D(0, 3)));
		((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, 0);
		joint.emplace_back(new RevJoint(body[10], body[11], Vector2D(0, -4.5), Vector2D(0, 3)));
		((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, 0);
		joint.emplace_back(new RevJoint(body[5], body[6], Vector2D(0, 5.5), Vector2D(0, -2.5)));
		((RevJoint*)joint.back())->SetLimit(true, pi/6, -pi/6);
		joint.emplace_back(new RevJoint(body[5], body[4], Vector2D(-.5, -5), Vector2D(.5, 4)));
		((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, -pi/6);
		joint.emplace_back(new RevJoint(body[5], body[7], Vector2D(-.5, -5), Vector2D(.5, 4)));
		((RevJoint*)joint.back())->SetLimit(true, (3*pi)/4, -pi/6);
		joint.emplace_back(new RevJoint(body[4], body[3], Vector2D(.5, -4), Vector2D(.5, 4)));
		((RevJoint*)joint.back())->SetLimit(true, 0, -(3*pi)/4);
		joint.emplace_back(new RevJoint(body[7], body[8], Vector2D(.5, -4), Vector2D(.5, 4)));
		((RevJoint*)joint.back())->SetLimit(true, 0, -(3*pi)/4);
		joint.emplace_back(new RevJoint(body[3], body[2], Vector2D(.5, -4), Vector2D(.5, .5)));
		((RevJoint*)joint.back())->SetLimit(true, pi/36, -pi/2);
		joint.emplace_back(new RevJoint(body[8], body[9], Vector2D(.5, -4), Vector2D(.5, .5)));
		((RevJoint*)joint.back())->SetLimit(true, pi/36, -pi/2);
	}
	//back shoulder joint =		0
	//front shoulder joint =	1
	//back elbow joint =		2
	//front elbow joint = 		3
	//neck joint = 				4
	//back hip joint =			5
	//front hip joint =			6
	//back knee joint =			7
	//front knee joint =		8
	//back ankle = 				9
	//front ankle =				10
	for (int i = 0; i < body.size(); i++)
		body[i]->filtergroup = 1;
	for (int i = 0; i < joint.size(); i++)
		((RevJoint*)joint[i])->SetMotor(true, 0, 2000000);
	b = j = 0;
}

Body* Man2::RegisterBodies()
{
	if (b == body.size())
		return NULL;
	return body[b++];
}

Joint*Man2::RegisterJoints()
{
	if (j == joint.size())
		return NULL;
	return joint[j++];
}

void Man2::Upright(Vector2D down)
{
	SetCoM();
	double angle = body[4]->orient * (180/pi);
	int frontf, backf, frontt, backt, frontc, backc, fronth, backh, frontk, backk, 
		fronta, backa;
	if (body[4]->position.x < body[7]->position.x) {
		frontf = 9;
		backf = 2;
		frontt = 7;
		backt = 4;
		frontc = 8;
		backc = 3;
		fronth = 6;
		backh = 5;
		frontk = 8;
		backk = 7;
		fronta = 10;
		backa = 9;
	}
	else {
		frontf = 2;
		backf = 9;
		frontt = 4;
		backt = 7;
		frontc = 3;
		backc = 8;
		fronth = 5;
		backh = 6;
		frontk = 7;
		backk = 8;
		fronta = 9;
		backa = 10;
	}
	
	if (body[5]->orient > 2*pi/180)
	{
		if (body[backf]->contact)
			((RevJoint*)joint[backh])->SetMotor(true, 1, 20000);
		else
			((RevJoint*)joint[backh])->SetMotor(true, -1, 20000);
		if (body[frontf]->contact)
			((RevJoint*)joint[fronth])->SetMotor(true, 1, 20000);
		else
			((RevJoint*)joint[fronth])->SetMotor(true, -1, 20000);
	}
	else if (body[5]->orient < -2*pi/180)
	{
		if (body[backf]->contact)
			((RevJoint*)joint[backh])->SetMotor(true, -1, 20000);
		else
			((RevJoint*)joint[backh])->SetMotor(true, 1, 20000);
		if (body[frontf]->contact)
			((RevJoint*)joint[fronth])->SetMotor(true, -1, 20000);
		else
			((RevJoint*)joint[fronth])->SetMotor(true, 1, 20000);
	}
	else
	{
		((RevJoint*)joint[fronth])->SetMotor(true, 0, 20000);
		((RevJoint*)joint[backh])->SetMotor(true, 0, 20000);
	}
	if (body[5]->position.x > body[frontc]->position.x)
	{
		if (body[frontf]->contact)
			((RevJoint*)joint[fronta])->SetMotor(true, -.5, 20000);
		if (body[backf]->contact)
			((RevJoint*)joint[backa])->SetMotor(true, -.5, 20000);
	}
	else if (body[5]->position.x < body[backc]->position.x)
	{
		if (body[frontf]->contact)
			((RevJoint*)joint[fronta])->SetMotor(true, .5, 20000);
		if (body[backf]->contact)
			((RevJoint*)joint[backa])->SetMotor(true, .5, 20000);
	}
	else
	{
		((RevJoint*)joint[fronta])->SetMotor(true, 0, 20000);
		((RevJoint*)joint[backa])->SetMotor(true, 0, 20000);
	}
	
	return;
	
	((RevJoint*)joint[backh])->SetMotor(true, 0, 200000);
	((RevJoint*)joint[fronth])->SetMotor(true, 0, 200000);
	if (body[backt]->position.x > CoM.x)
	{
		if (body[backf]->contact)
			((RevJoint*)joint[backh])->SetMotor(true, 1, 20000);
		else
			((RevJoint*)joint[backh])->SetMotor(true, -1, 20000);
	}
	if (body[frontt]->position.x < CoM.x)
	{
		if (body[frontf]->contact)
			((RevJoint*)joint[fronth])->SetMotor(true, -1, 20000);
		else
			((RevJoint*)joint[fronth])->SetMotor(true, 1, 20000);
	}
	if (body[frontc]->position.x > CoM.x and body[backc]->position.x > CoM.x)
	{
		if (body[backf]->contact)
			((RevJoint*)joint[backh])->SetMotor(true, 1, 20000);
		if (body[frontf]->contact)
			((RevJoint*)joint[fronth])->SetMotor(true, 1, 20000);
		Rotateto(0, pi/2);
		Rotateto(1, pi/2);
	}
	else if (body[frontc]->position.x < CoM.x and body[backc]->position.x < CoM.x)
	{
		if (body[backf]->contact)
			((RevJoint*)joint[backh])->SetMotor(true, -1, 20000);
		if (body[frontf]->contact)
			((RevJoint*)joint[fronth])->SetMotor(true, -1, 20000);
		Rotateto(0, -pi/2);
		Rotateto(1, -pi/2);
	}
	else
	{
		((RevJoint*)joint[0])->SetMotor(true, 0, 20000);
		((RevJoint*)joint[1])->SetMotor(true, 0, 20000);
	}
}

void Man2::SetCoM()
{
	CoM.Set(0,0);
	double m = 0;
	for (int i = 0; i < body.size(); i++)
	{
		m += body[i]->mass.m;
		CoM += body[i]->position * body[i]->mass.m;
	}
	CoM /= m;
}

void Man2::Rotateto(int j, double angle)
{
	double curangle = ((RevJoint*)joint[j])->GetAngle();
	double diff = curangle - angle;
	if (std::abs(diff) < pi/180)
		((RevJoint*)joint[j])->SetMotor(true, 0, 20000);
	else if (curangle < angle)
		((RevJoint*)joint[j])->SetMotor(true, 1, 20000);
	else if (curangle > angle)
		((RevJoint*)joint[j])->SetMotor(true, -1, 20000);
}

