#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/freeglut.h>
#include <gl/glwindowpos.h>
#include "world.h"
#include <sstream>
#include <iostream>

int _screenw;
int _screenh;
int lasttime = 0;
bool buffer[256];
bool sbuffer[256];
World* world;
int lastfpscheck = 0;
int fps = 0;
int frame = 0;
double scale = 1;
bool debug = false;

std::string to_string(double number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
void outputString(std::string output, double x, double y, bool window)
{
	if (window)
		glWindowPos2i(x, y);
	else
		glRasterPos2d(x, y);
	glutBitmapString(GLUT_BITMAP_9_BY_15,(const unsigned char *)output.c_str());
}
void outputNum(double num, double x, double y, bool window)
{
	std::string word = to_string(num);
	outputString(word, x, y, window);
}
void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,800.0, 0.0, 600.0);
}
void Reshape (int width, int height)
{
	_screenw = width;
	_screenh = height;
	glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,(GLfloat)width,0,(GLfloat)height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void Idle()
{
	frame++;
	int curtime = glutGet(GLUT_ELAPSED_TIME);
	int diff = curtime - lasttime;
	lasttime = curtime;
	
	if (diff > 20)
		diff = 20;
	
	if (curtime - lastfpscheck >= 1000)
	{
		fps = frame;
		frame = 0;
		lastfpscheck = curtime;
	}
	
	if (buffer['r'])
	{
		delete world;
		world = new World;
	}
	
	if (buffer['+'])
		scale *= 1 + diff/1000.0;
	if (buffer['-'])
		scale /= 1 + diff/1000.0;
	if (buffer[13])
		scale = 1;
	if (buffer['x'])
	{
		debug = !debug;
		buffer['x'] = false;
	}
	if (buffer['a'])
		world->setSpeed(75);
	else if (buffer['d'])
		world->setSpeed(-75);
	else if (buffer['s'])
		world->setBrake();
	else
		world->setOff();
	
	world->Solve(diff);
}
void setWindow(double left, double right, double bottom, double top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);
}
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	double l = (world->getPos().x-(_screenw/2)/scale);
	double r = (world->getPos().x+(_screenw/2)/scale);
	double b = (world->getPos().y-(_screenh/2)/scale);
	double t = (world->getPos().y+(_screenh/2)/scale);
	setWindow(l,r,b,t);
	
	glColor3f(.3,.6,.3);
	for (int i = 0; i < 61; i++)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2d(-3000 + i*100, 1000);
		glVertex2d(-3000 + i*100, -1000);
		glEnd();
	}
	
	world->drawBodies(debug);
	double BP = world->GetProfile("bp");
	double NP = world->GetProfile("np");
	double SLV = world->GetProfile("slv");
	double MV = world->GetProfile("mv");
	glColor3f(1,1,1);
	outputNum(fps, 10, _screenh-20, true);
	outputString("bp:", 10, _screenh-40, true);
	outputNum(((int)(BP*1000000000))/1000000.0, 50, _screenh-40, true);
	outputString("np:", 10, _screenh-60, true);
	outputNum(((int)(NP*1000000000))/1000000.0, 50, _screenh-60, true);
	outputString("slv:", 10, _screenh-80, true);
	outputNum(((int)(SLV*1000000000))/1000000.0, 50, _screenh-80, true);
	outputString("mv:", 10, _screenh-100, true);
	outputNum(((int)(MV*1000000000))/1000000.0, 50, _screenh-100, true);
	
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}
void SpecialInput(int key, int x, int y)
{
	sbuffer[key] = true;
}
void SpecialUp(int key, int x, int y)
{
	sbuffer[key] = false;
}
void Key(unsigned char key, int x, int y)
{
	buffer[key] = true;
}
void KeyUp(unsigned char key, int x, int y)
{
	buffer[key] = false;
}
bool set = false;
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN)
	{
		double X = world->getPos().x + (x - (_screenw/2.0))/scale;
		double Y = world->getPos().y + ((_screenh/2.0) - y)/scale;
		if (buffer['f'])
			world->addLinked(X, Y);
		else if (buffer['j'])
			world->addRope(X, Y);
		else
			//set = !set;
			world->addCircle(X, Y);
			//world->addMagnet(X, Y);
			//world->addBox(X, Y, 10, 10, false);
			//world->addComposite(X, Y);
	}
	if (button == GLUT_RIGHT_BUTTON and state == GLUT_DOWN)
	{
		double X = world->getPos().x + (x - (_screenw/2.0))/scale;
		double Y = world->getPos().y + ((_screenh/2.0) - y)/scale;
		if (buffer['f'])
			world->addStack(X, Y);
		else if (buffer['j'])
			world->addBridge(X, Y);
		else
			world->addRamp(X, Y);
	}
}
void Mouse2(int x, int y)
{
	double X = world->getPos().x + (x - (_screenw/2.0))/scale;
	double Y = world->getPos().y + ((_screenh/2.0) - y)/scale;
	if (set)
		world->setTarget(X, Y);
}
int main(int argc, char** argv)
{
	world = new World;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("testbed");

	Init();
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutSpecialFunc(SpecialInput);
	glutSpecialUpFunc(SpecialUp);
	glutKeyboardFunc(Key);
	glutKeyboardUpFunc(KeyUp);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutPassiveMotionFunc(Mouse2);
	glutMainLoop();
	delete world;
}