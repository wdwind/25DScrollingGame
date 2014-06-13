/** \file car.h

Draw car.

*/
#ifndef CAR
#define CAR
#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <string.h>
using namespace std;

float ballX = 0.1f;
float ballY = 0.1f;
float ballZ = 0.5f;
GLfloat rCanon = 0.0f;
GLfloat rBall = 0.0f;
std::vector<float> PositionBullet = { (float)0.03, (float)0.1, (float)0.23 };

GLfloat firstVertexOfTri = 0.0;
GLfloat incrementOfVertex = .001;
GLfloat angleUpper = 0;
GLfloat angleFore = -30;
GLfloat angleHand = -30;
GLfloat incrementAngle = .05;
GLfloat incrementOfrball = 0.3f;

void drawBall(void) {

	glColor3f(0.0, 1.0, 0.0); //set ball colour
	glPushMatrix();
		glTranslatef(ballX, ballY, ballZ); //moving it toward the screen a bit on creation
		//glRotatef(ballX,ballX,ballY,ballZ);
		glRotatef(rBall, 0, 0, -1);
		glutSolidSphere(0.2, 10, 10); //create ball.
	glPopMatrix();
	glPushMatrix();
		glTranslatef(ballX + .8, ballY, ballZ); //moving it toward the screen a bit on creation
		glRotatef(rBall, 0, 0, -1);
		glutSolidSphere(0.2, 10, 10); //
	glPopMatrix();
	glPushMatrix();
		glTranslatef(ballX, ballY, ballZ + .5); //moving it toward the screen a bit on creation
		glRotatef(rBall, 0, 0, -1);
		glutSolidSphere(0.2, 10, 10); //
	glPopMatrix();
	glPushMatrix();
		glTranslatef(ballX + .8, ballY, ballZ + .5); //moving it toward the screen a bit on creation
		glRotatef(rBall, 0, 0, -1);
		glutSolidSphere(0.2, 10, 10); //
	glPopMatrix();

}

void drawUnitFace()
{
	//1) draw a unit quad in the x,y plane oriented along the z axis
	//2) make sure the orientation of the vertices is positive (counterclock wise)
	//3) What happens if the order is inversed?
	glColor3f(1, 1, 0);
	glNormal3f(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, .5);
		glVertex3f(0, .5, .5);
		glVertex3f(.5, .5, .5);
		glVertex3f(.5, 0, .5);
	glEnd();

}

void drawUnitCube()
{
	//1) draw a cube using your function drawUnitFace
	//rely on glTranslate, glRotate, glPushMatrix, and glPopMatrix
	//the latter two influence the model matrix, as seen during the course.
	//glPushMatrix stores the current matrix and puts a copy on
	//the top of a stack.
	//glPopMatrix pops the top matrix on the stack
	glPushMatrix();
	drawUnitFace();
	glRotatef(180, 0, -1, 0);
	glTranslatef(-.5, 0, -1.5);
	drawUnitFace();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-90, 0, -1, 0);
	glTranslatef(-1, 0, -.5);
	drawUnitFace();
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 0, -1, 0);
	glTranslatef(.5, 0, -1);
	drawUnitFace();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-90, -1, 0, 0);
	glTranslatef(0, .5, -1);
	drawUnitFace();
	glPopMatrix();
	glRotatef(90, -1, 0, 0);
	glTranslatef(0, -1, -.5);
	drawUnitFace();
	glPopMatrix();
}

void drawBullet(){
	glColor3f(0, 0, 1);
	glPushMatrix();
		glRotatef(rCanon, 0, 0, -1);
		glTranslatef(PositionBullet[0], PositionBullet[1], PositionBullet[2]);
		glutSolidSphere(.05, 50, 50);
	glPopMatrix();
}

void drawCar()
{
	glPushMatrix(); //draw the car body
		glScalef(2, 1, 1);
		//glNormal3f(1, 2, 0);
		drawUnitCube();
	glPushMatrix(); //draw the canon
		glTranslatef(0.5, 0.5, 0.5);
		drawBullet();
		glRotatef(rCanon, 0, 0, -1); //control the canon
		glScalef(1.2, 0.3, 0.3);
		//glNormal3f(1, 2, 0);
		drawUnitCube();
		drawBall(); //draw the wheels
	glPopMatrix();
	glPopMatrix();
}

void canonClock(){
	if (rCanon >= -45 && rCanon < 0){
		rCanon += 3;
	}
}

void canonCounter(){
	if (rCanon > -45 && rCanon <= 0){
		rCanon -= 3;
	}
}

void drawEnemy(){
	glPushMatrix();
		glTranslatef(3, 0, 0);
		glScalef(2, 2, 2);
		drawUnitFace();
	glPopMatrix();
}

#endif