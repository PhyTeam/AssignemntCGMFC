// Lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "supportClass.h"
#include "Mesh.h"
#include "PLYReader.h"

using namespace std;

#define PI		3.1415926

int width, height;

int		screenWidth = 600;
int		screenHeight = 600;

bool	bWireFrame = false;

Mesh*	object3d;

/* Camera variables */
float camera_angle;
float camera_height;
float camera_dis;
float camera_X, camera_Y, camera_Z;
float lookAt_X, lookAt_Y, lookAt_Z;

bool	b4View = false;

void initLight();
void initLight2();
void drawAll();

void drawAxis()
{
	glPushMatrix();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-4, 0, 0);//x
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);//y
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -4);//z
	glVertex3f(0, 0, 4);
	glEnd();

	glPopMatrix();
}

float Modify_Angel(float angel) {
	if (angel >= 180 && angel <= 270) {
		return angel - 180;
	}
	else if (angel > 270 && angel <= 360) {
		return angel - 270;
	}
	else if (angel > 90) {
		return angel - 90;
	}
	else return angel;
}

void myKeyboard(unsigned char key, int x, int y)
{
	float	fAngle = 2;
	GLboolean isOn = false;
	switch (key)
	{
	case '1':
		break;
	case '2':
		break;
	case '3':
		break;
	case '4':
		break;
	case 'l':
	case 'L':
		break;
	case 'x':
	case 'X':
		break;
	case 'w':
	case 'W':
		bWireFrame = !bWireFrame;
		break;
	case 'D': 
	case 'd':
		glGetBooleanv(GL_LIGHT1, &isOn);
		if (isOn)
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);

		break;
	case 'v':
	case 'V':
		b4View = !b4View;
		break;
	case '-': 
	case '_':
		camera_dis -= 0.1f;
		break;
	case '+': 
	case '=':
		camera_dis += 0.1f;
		break;
	
	}
	printf("PRESSED %d\n", key);
	glutPostRedisplay();
}

void keyboardSpecialDown(int k, int x, int y) {
	switch (k) {
	case 100:
		camera_angle -= 0.1f;
		break;
	case 102:
		camera_angle += 0.1f;
		break;
	case GLUT_KEY_UP:
		camera_height += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		camera_height -= 0.1f;
		break;
	default:
		break;
	}
	glutPostRedisplay();
	//printf("PRESSED %d\n", k);
}


void myDisplay() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float eyex = camera_dis * cos(camera_angle);
	float eyez = camera_dis * sin(camera_angle);
	float eyey = camera_height;
	// Setup camera
	if (!b4View) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		gluPerspective(45.0f, (float)(width) / height, 1.f, 10000.f);
		gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, screenWidth, screenHeight);
		
		drawAll();
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, screenWidth / screenHeight, 1, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//// View goc duoi ben trai
		glViewport(0, 0, screenWidth / 2, screenHeight / 2);

		glPushMatrix();
		gluLookAt(0, 5, 0, 0, 0, 0, 0, 0, -1);
		drawAll();
		glPopMatrix();

		//// View goc duoi ben phai
		glViewport(screenWidth / 2, 0, screenWidth / 2, screenHeight / 2);

		glPushMatrix();
		gluLookAt(eyex, eyey, eyez, 0, 1, 0, 0, 1, 0);
		drawAll();
		glPopMatrix();
		//// View goc tren ben trai

		glViewport(0, screenHeight / 2, screenWidth / 2, screenHeight / 2);

		glPushMatrix();
		gluLookAt(0, 0, 6, 0, 1, 0, 0, 1, 0);
		drawAll();
		glPopMatrix();
		// View goc tren ben phai

		glViewport(screenWidth / 2, screenHeight / 2, screenWidth / 2, screenHeight / 2);

		glPopMatrix();
		gluLookAt(6, 0, 0, 0, 1, 0, 0, 1, 0);
		drawAll();
		glPopMatrix();
	}
	

	glFlush();
	glutSwapBuffers();
}

void drawAll() {
	drawAxis();
	//Base
	GLfloat	Diffuse[] = { 0.5f, 0.5f, 0.1f };
	GLfloat	Specular[] = { 0.5f, 0.5f, 0.5f };
	GLfloat	Ambient[] = { 1.0f, 0.0f, 1.0f };
	//glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	//glScaled(0.05, 0.05, 0.05);
	object3d->DrawWireframe();
}

void myInit()
{
	// Load Mesh
	object3d = readFile();
	float	fHalfSize = 4;

	camera_dis = 500.0f;
	camera_angle = 0.0f;
	camera_height = 1.0f;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	initLight();
	initLight2();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -1000, 1000);
	
	glMatrixMode(GL_MODELVIEW);
}

void initLight() {
	GLfloat	lightDiffuse[] = { 0.5f, 0.5f, 0.5f };
	GLfloat	lightSpecular[] = { 1.0f, 1.0f, 1.0f };
	GLfloat	lightAmbient[] = { 0.2f, 0.2f, 0.2f, .7f };
	GLfloat 	light_position1[] = { 60.0f, 60.0f, 60.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void initLight2() {
	GLfloat	lightDiffuse[] = { 0.5f, 0.5f, 0.5f };
	GLfloat	lightSpecular[] = { 1.0f, 1.0f, 1.0f };
	GLfloat	lightAmbient[] = { 1.0f, 0.2f, 0.1f, 1.0f };
	GLfloat 	light_position1[] = { -6.0f, -6.0f, 6.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	width = 600; height = 800;
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Lab3-2015-2016"); // open the screen window

	myInit();

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(keyboardSpecialDown);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;
}
