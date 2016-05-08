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

using namespace std;

#define PI		3.1415926

int		screenWidth = 600;
int		screenHeight= 600;

bool	bWireFrame = false;

float	baseRadius = 0.5;
float	baseHeight = 0.2;
float	baseRotateStep = 5;

float	cylRadius = 0.3;
float	cylHeight = 1.0;
float   cylMaxScaleY = 2.0;
float   cylMinScaleY = 0.5;
float	cylScaleStep = 0.05;

//Body 1
float	body1SizeX = 3;       
float	body1SizeY = 0.2;     
float	body1SizeZ = 0.8;     

//Body 2
float body2SizeX = 4.5; // 2: body1.scaleX
float body2SizeY = 0.52; // 2: body1.scaleY
float body2SizeZ = 0.52;//// 2: body1.scaleZ

//Body 3
float body3SizeX = 4.5; // 2: body1.scaleX
float body3SizeY = 0.52; // 2: body1.scaleY
float body3SizeZ = 0.52;//// 2: body1.scaleZ

//Body 4
float body4SizeX = 1.5; // 2: body1.scaleX;  1: body2.scaleX
float body4SizeY = 0.78; // 1: body2.scaleY
float body4SizeZ = 1.6;//// 1: body2.scaleZ

//Bolt 1: Bolt large
float	bolt1Radius = 0.32;
float	bolt1Height = 0.15;

//Bolt 2: Bolt small 1
float	bolt2Radius = 0.15;
float	bolt2Height = 0.2;

//Bolt 3: Bolt small 2
float	bolt3Radius = 0.05;
float	bolt3Height = 0.2;

//Rotation arm
float roArm_R1 = 0.15;
float roArm_R2 = 0.5;
float roArm_Height = 0.2;
float roArm_dist = 1.6;

//Connecting Arm
float connectArm_R1 = 0.2;
float connectArm_R2 = 0.2;
float connectArm_Height = 0.15;
float connectArm_dist = 3;

//Block
float BlockSizeX = 0.5; // 2: body1.scaleX;  1: body2.scaleX
float BlockSizeY = 1.2; // 1: body2.scaleY
float BlockSizeZ = 0.52;//// 1: body2.scaleZ
float blockSlideStep = 0.02;

Mesh	base;
Mesh	cyl;
Mesh	body1;
Mesh	body2;
Mesh	body3;
Mesh	body4;
Mesh	bolt1;
Mesh	bolt2;
Mesh	bolt3;
Mesh	roArm;
Mesh	connectArm;
Mesh	block;
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

float tinhx(float tayquay, float taynoi, float gocbeta, float x){
	float temp = x*x + tayquay*tayquay - 2 * x*tayquay*cos(gocbeta*PI / 180) - taynoi*taynoi;
	if (gocbeta == 180.0)
		return 0.0;
	else if (gocbeta == 0.0)
		return tayquay + taynoi;
	else if ((temp >= 0 && temp<0.1) || (temp<0 && temp>-0.1))
		return (float)x;
	return tinhx(tayquay, taynoi, gocbeta, x + 0.075);

}
float gocanpha(float tayquay, float taynoi, float x){
	float sodo = (x*x + taynoi*taynoi - tayquay*tayquay) / (2 * x*taynoi);
	float sodogoc = acos(sodo) * 180 / PI;
	return sodogoc;
}
float chuanhoa_beta(float beta){
	if (beta <= 90.0)
		return 90.0 + beta;
	else if (beta>90.0 && beta <= 270.0)
		return 180.0 - (beta - 90.0);
	else if (beta >270 && beta <= 360.0)
		return beta - 270.0;
}
float tinh_dodoi(float x){
	float path = (body1SizeX - BlockSizeX) / 2;// nua doan duong ma con truot co the di
	if (x<path)
		return path - x;//xem lai 
	else if (x == path)
		return 0.0;
	else return -(x - path);
}

//distance mang tinh chat tuong doi
float Distance_From_Block_To_Body4(float angel, float l_roArm, float l_connectArm){
	float value = 0;
	float result;
	while (true){
		result = value*value + l_roArm*l_roArm - 2 * value*l_roArm*cos(angel*PI / 180) - l_connectArm*l_connectArm;
		if ((result >= 0 && result < 0.1) || (result<0 && result>-0.1)){
			return value;
		}
		else if (angel == 0){
			return l_roArm + l_connectArm;
		}
		else{
			value += 0.075;
		}
	}
}

float Distance_Move_Block(float value){
	float Old = body1SizeX / 2 - body4SizeX / 2;
	if (value < Old)
		return Old - value;//xem lai 
	else if (value == Old)
		return 0.0;
	else return -(value - Old);
}

float Modify_Angel(float angel){
	if (angel >= 180 && angel <= 270){
		return angel - 180;
	}
	else if (angel > 270 && angel <= 360){
		return angel - 270;
	}
	else if (angel > 90){
		return angel - 90;
	}
	else return angel;
}


void myKeyboard(unsigned char key, int x, int y)
{
	float	fRInc;
	float	fAngle=2;
	float new_angel;
	float distance;
	float move;

    switch(key)
    {
	case '1':
		base.rotateY += baseRotateStep;
		if(base.rotateY > 360)
			base.rotateY -=360;
		break;
	case '2':	
		base.rotateY -= baseRotateStep;
		if(base.rotateY < 0)
			base.rotateY +=360;
		break;
	case '3':
		roArm.rotateY += fAngle;
		block.slideX += blockSlideStep;
		if (roArm.rotateY > 360)
			roArm.rotateY -= 360;
	/*	roArm.rotateY += baseRotateStep;
		new_angel = Modify_Angel(roArm.rotateY);
		distance = Distance_From_Block_To_Body4(new_angel, roArm_R1 + roArm_R2, connectArm_R1 + connectArm_R2);
		move = Distance_Move_Block(distance);*/
		//block.slideX = move;
		/*if (roArm.rotateY<90.0 || (tayquay.rotateY>270.0 && tayquay.rotateY<360.0))
			taynoi.rotateY = 90.0 - gocanpha(1.0, 1.0 + tayquayR2*4.375, dodai_x);
		else if (tayquay.rotateY == 360.0)
		{
			tayquay.rotateY -= 360;
			taynoi.rotateY = atan2(1.0, 1.35)*180.0 / PI;
		}
		else
			taynoi.rotateY = 90.0 + gocanpha(1.0, 1.0 + tayquayR2*4.375, dodai_x);*/

		break;
	case '4':
		roArm.rotateY -= fAngle;
		block.slideX -= blockSlideStep;
		if (roArm.rotateY > 360)
			roArm.rotateY -= 360;
		break;
	case 'l':
	case 'L':
		if (cyl.scaleY <= cylMaxScaleY)
			cyl.scaleY += cylScaleStep;
		break;

	case 'x':
	case 'X':
		if (cyl.scaleY >= cylMinScaleY)
			cyl.scaleY -= cylScaleStep;
		break;

	case 'w':
	case 'W':
		bWireFrame = !bWireFrame;
		break;

	}
    glutPostRedisplay();
}

void drawBase()
{
	glPushMatrix();

		glTranslated(0, base.slideY, 0);
		base.scaleX = base.scaleY = base.scaleZ = 1.15;
		glScalef(base.scaleX, base.scaleY, base.scaleZ);
		glRotatef(base.rotateY, 0, 1, 0);

		if(bWireFrame)
			base.DrawWireframe();
		else
			base.DrawColor();

	glPopMatrix();
}
void drawCyl()
{
	glPushMatrix();	
		glTranslated(0, cyl.slideY*cyl.scaleY + baseHeight, 0);


		glScalef(cyl.scaleX, cyl.scaleY, cyl.scaleZ);

		glRotatef(base.rotateY, 0, 1, 0);

		if(bWireFrame)
			cyl.DrawWireframe();
		else
			cyl.DrawColor();
	glPopMatrix();
}

void drawBody1()
{
	glPushMatrix();
		glRotatef(base.rotateY, 0, 1, 0);
		body1.scaleX = 2;
		body1.scaleY = 2;
		body1.scaleZ = 2;

		body1.slideX = 0;//
		body1.slideY = cylHeight*cyl.scaleY + baseHeight*base.scaleY;
		body1.slideZ = 0;
		//Scale it and then move to right position
		glTranslated(body1.slideX, body1.slideY, body1.slideZ);
		glScalef(body1.scaleX, body1.scaleY, body1.scaleZ);
		
		if (bWireFrame){
			body1.DrawWireframe();
		}
		else{
			body1.DrawColor();
		}

	glPopMatrix();
}

void drawBody2()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	body2.scaleX = 1;
	body2.scaleY = 1;
	body2.scaleZ = 1;

	body2.slideX = -(body1SizeX*body1.scaleX / 2 - body2SizeX*body2.scaleX / 2);
	body2.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY+0.05);
	body2.slideZ = -(body1SizeZ*body1.scaleZ / 2 - body2SizeZ*body2.scaleZ / 2);
	glTranslated(body2.slideX, body2.slideY, body2.slideZ);
	

	if (bWireFrame){
		body2.DrawWireframe();
	}
	else{
		body2.DrawColor();
	}

	glPopMatrix();
}

void drawBody3()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	body3.scaleX = 1;
	body3.scaleY = 1;
	body3.scaleZ = 1;

	body3.slideX = -(body1SizeX*body1.scaleX / 2 - body3SizeX*body3.scaleX / 2);
	body3.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY+0.05);
	body3.slideZ = +(body1SizeZ*body1.scaleZ / 2 - body3SizeZ*body3.scaleZ / 2);
	glTranslated(body3.slideX, body3.slideY, body3.slideZ);


	if (bWireFrame){
		body3.DrawWireframe();
	}
	else{
		body3.DrawColor();
	}

	glPopMatrix();
}

void drawBody4()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	body4.scaleX = 1;
	body4.scaleY = 1;
	body4.scaleZ = 1;

	body4.slideX = +(body1SizeX*body1.scaleX / 2 - body4SizeX*body4.scaleX / 2);
	body4.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY+0.2);
	body4.slideZ = 0;
	glTranslated(body4.slideX, body4.slideY, body4.slideZ);

	if (bWireFrame){
		body4.DrawWireframe();
	}
	else{
		body4.DrawColor();
	}

	glPopMatrix();
}

void drawBolt1()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	bolt1.scaleX = 1;
	bolt1.scaleY = 1;
	bolt1.scaleZ = 1;

	bolt1.slideX = +(body1SizeX*body1.scaleX / 2 - bolt1Radius - body4SizeX / 4);
	bolt1.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY + body4SizeY*body4.scaleY);
	bolt1.slideZ = 0;
	glTranslated(bolt1.slideX, bolt1.slideY, bolt1.slideZ);


	if (bWireFrame){
		bolt1.DrawWireframe();
	}
	else{
		bolt1.DrawColor();
	}

	glPopMatrix();
}

void drawBolt2()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	bolt2.scaleX = 1;
	bolt2.scaleY = 1;
	bolt2.scaleZ = 1;

	bolt2.slideX = (roArm_dist)*sin((roArm.rotateY - 90)*PI / 180);
	bolt2.slideZ = (roArm_dist)*cos((roArm.rotateY - 90)*PI / 180);
	//bolt2.slideX = 1;// +(body1SizeX*body1.scaleX / 2 - bolt1Radius - body4SizeX);
	bolt2.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY + body4SizeY*body4.scaleY)+0.1;
	//bolt2.slideZ = 0;
	glTranslated(bolt2.slideX-0.6, 0, bolt2.slideZ);
	glTranslated(3, bolt2.slideY, 0);

	if (bWireFrame){
		bolt2.DrawWireframe();
	}
	else{
		bolt2.DrawColor();
	}

	glPopMatrix();
}

void drawBolt3()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	bolt3.scaleX = 1;
	bolt3.scaleY = 1;
	bolt3.scaleZ = 1;

	float D = sqrt(connectArm_dist*connectArm_dist - roArm_dist*sin(roArm.rotateY*PI / 180)*roArm_dist*sin(roArm.rotateY*PI / 180)) + roArm_dist*cos(roArm.rotateY*PI / 180);

	bolt3.slideX = 2.5;// +(body1SizeX*body1.scaleX / 2 - bolt1Radius - body4SizeX);
	bolt3.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY + body4SizeY*body4.scaleY) + 0.1;
	bolt3.slideZ = 0.01;
	glTranslated(bolt3.slideX - D, bolt3.slideY, bolt3.slideZ);


	if (bWireFrame){
		bolt3.DrawWireframe();
	}
	else{
		bolt3.DrawColor();
	}

	glPopMatrix();
}

void drawRoArm()
{
	glPushMatrix();
	glRotatef(base.rotateY+180, 0, 1, 0);
	roArm.scaleX = 1;
	roArm.scaleY = 1;
	roArm.scaleZ = 1;

	//Modify
	roArm.slideX = -2.4;//+(body1SizeX*body1.scaleX / 2 - bolt1Radius - body4SizeX / 4) - 1.6;
	roArm.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY + body4SizeY*body4.scaleY) - 0.15;
	roArm.slideZ = 0;
	glTranslated(roArm.slideX, roArm.slideY, roArm.slideZ);
	glRotatef(roArm.rotateY, 0, 1, 0);

	if (bWireFrame){
		roArm.DrawWireframe();
	}
	else{
		roArm.DrawColor();
	}

	glPopMatrix();
}

void drawConnectArm()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	connectArm.scaleX = 1;
	connectArm.scaleY = 1;
	connectArm.scaleZ = 1;

	float beta = asin(roArm_dist*sin(roArm.rotateY*PI / 180) / connectArm_dist) / PI * 180;
	float D = sqrt(connectArm_dist*connectArm_dist - roArm_dist*sin(roArm.rotateY*PI / 180)*roArm_dist*sin(roArm.rotateY*PI / 180)) + roArm_dist*cos(roArm.rotateY*PI / 180);


	//Modify
	connectArm.slideX = +(body1SizeX*body1.scaleX / 2 - bolt1Radius - body4SizeX / 4) +0.15;
	connectArm.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY + body4SizeY*body4.scaleY)+0.04;
	connectArm.slideZ = 0.005;
	glTranslated(connectArm.slideX -D, connectArm.slideY, connectArm.slideZ);
	glRotatef(-beta, 0, 1, 0);
	if (bWireFrame){
		connectArm.DrawWireframe();
	}
	else{
		connectArm.DrawColor();
	}

	glPopMatrix();
}

void drawBlock()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	block.scaleX = 1;
	block.scaleY = 1;
	block.scaleZ = 1;

	float D = sqrt(connectArm_dist*connectArm_dist - roArm_dist*sin(roArm.rotateY*PI / 180)*roArm_dist*sin(roArm.rotateY*PI / 180)) + roArm_dist*cos(roArm.rotateY*PI / 180);
	block.slideX = 2.5;
	block.slideY = float(baseHeight*base.scaleY + cylHeight*cyl.scaleY + body1SizeY*body1.scaleY + 0.05)+0.16;
	block.slideZ = 0;
	glTranslated(block.slideX - D, block.slideY, block.slideZ);

	if (bWireFrame){
		block.DrawWireframe();
	}
	else{
		block.DrawColor();
	}

	glPopMatrix();
}

void myDisplay(){
	//{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-8, 6, 8, 0, 1, 0, 0, 1, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screenWidth, screenHeight);

	drawAxis();
	//Base
	drawBase();

	//Cylinder
	drawCyl();

	//Parts of body
	drawBody1();
	drawBody2();
	drawBody3();
	drawBody4();

	//Bolt
	drawBolt1();
	drawBolt2();
	drawBolt3();

	//Rotation Arm
	drawRoArm();

	//Connecting Arm
	drawConnectArm();

	//Block
	drawBlock();

	glFlush();
	glutSwapBuffers();
}

void myInit()
{
	float	fHalfSize = 4;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -1000, 1000);

	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[])
{
	cout << "1, 2: Rotate the base" << endl;
	cout << "3, 4: Rotate the arm" << endl;
	cout << "L, l: Cylinder up" << endl;
	cout << "X, x: Cylinder down" << endl;
	cout << "W, w: Switch between wireframe and solid mode" << endl;
	
	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Lab3-2015-2016"); // open the screen window

	//Base
	base.CreateCylinder(20, baseHeight, baseRadius);
	base.SetColor(2);
	base.slideY = baseHeight / 2.0;

	//Cylinder
	cyl.CreateCylinder(20, cylHeight, cylRadius);
	cyl.SetColor(0);
	cyl.slideY = cylHeight / 2.0;
	cyl.scaleX = 0.9;
	cyl.scaleY = 0.9;
	cyl.scaleZ = 0.9;

	//Body
	body1.CreateCuboid(body1SizeX, body1SizeY, body1SizeZ);
	body1.SetColor(4);

	body2.CreateCuboid(body2SizeX, body2SizeY, body2SizeZ);
	body2.SetColor(5);

	body3.CreateCuboid(body3SizeX, body3SizeY, body3SizeZ);
	body3.SetColor(6);

	body4.CreateCuboid(body4SizeX, body4SizeY, body4SizeZ);
	body4.SetColor(7);
	//Bolt 1: Bolt large
	bolt1.CreateCylinder(20, bolt1Height, bolt1Radius);
	bolt1.SetColor(8);

	//Bolt 2: Bolt small 1
	bolt2.CreateCylinder(20, bolt2Height, bolt2Radius);
	bolt2.SetColor(8);

	bolt2.slideX = 1;// +(body1SizeX*body1.scaleX / 2 - bolt1Radius - body4SizeX);
	bolt2.slideZ = 0;

	//Bolt 3: Bolt small 2
	bolt3.CreateCylinder(20, bolt2Height, bolt2Radius);
	bolt3.SetColor(8);

	//Rotation arm
	//roArm.CreateGeometryShape_Large(roArm_R1, roArm_R2, roArm_Height, 1);
	roArm.CreateMixShape(20, roArm_Height, roArm_dist, roArm_R1, roArm_R2);
	roArm.SetColor(9);
	myInit();

	//Connecting arm
	//connectArm.CreateGeometryShape_Large(connectArm_R1, connectArm_R2, connectArm_Height, 2.5);
	connectArm.CreateMixShape(20, connectArm_Height, connectArm_dist, connectArm_R1, connectArm_R2);
	connectArm.SetColor(10);

	//Block
	block.CreateCuboid(BlockSizeX, BlockSizeY, BlockSizeZ);
	block.SetColor(11);
	// +(body1SizeX*body1.scaleX / 2 - body4SizeX*body4.scaleX / 2);
	myInit();

	glutKeyboardFunc(myKeyboard);
    glutDisplayFunc(myDisplay);
	
	glutMainLoop();
	return 0;
}






