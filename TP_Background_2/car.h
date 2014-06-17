/** \file car.h

Draw car.

*/
#ifndef CAR
#define CAR

#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <iostream>
#include "bullet.h"
#include "texture.h"

//static const float mf[] = { (float)0.03, (float)0.1, (float)0.23 };
//std::vector<float> PositionBullet(mf, mf + sizeof(mf) / sizeof(float));

class Car{
public:
	Car(){
		PositionBullet = { (float)0.03, (float)0.1, (float)0.23 };

		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere, GLU_SMOOTH);
	};

	Car(Vec3Df defaultT){
		PositionBullet = { (float)0.03, (float)0.1, (float)0.23 };

		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere, GLU_SMOOTH);

		translate = defaultT;
	};

	Vec3Df translate = Vec3Df(0, 0, 0);

	GLUquadricObj *sphere = NULL;

	GLfloat rCanon = 0.0f;
	GLfloat rBall = 0.0f;

	std::vector<float> PositionBullet;
	 //= { (float)0.03, (float)0.1, (float)0.23 };
	//PositionBullet.push_back(1);

	//Bullet temp1 = Bullet();

	std::vector<Bullet> bullets;
	
	//GLfloat firstVertexOfTri = 0.0;
	//GLfloat incrementOfVertex = .001;
	
	//GLfloat angleUpper = 0;
	//GLfloat angleFore = -30;
	//GLfloat angleHand = -30;
	//GLfloat incrementAngle = .05;
	GLfloat incrementOfrball = 2.8f;

	void drawBall(void) {
		float ballX = 0.1f;
		float ballY = 0.1f;
		float ballZ = 0.5f;

		glColor3f(1.0, 1.0, 1.0); //set ball colour

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[5]);

		glPushMatrix();
			glTranslatef(ballX, ballY, ballZ); //moving it toward the screen a bit on creation
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); //create ball.
		glPopMatrix();

		glPushMatrix();
			glTranslatef(ballX + .8, ballY, ballZ); //moving it toward the screen a bit on creation
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); //
		glPopMatrix();

		glPushMatrix();
			glTranslatef(ballX, ballY, ballZ + .5); //moving it toward the screen a bit on creation
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); //
		glPopMatrix();

		glPushMatrix();
			glTranslatef(ballX + .8, ballY, ballZ + .5); //moving it toward the screen a bit on creation
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); //
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);
	}

	void drawUnitFace()
	{
		//1) draw a unit quad in the x,y plane oriented along the z axis
		//2) make sure the orientation of the vertices is positive (counterclock wise)
		//3) What happens if the order is inversed?
		//glColor3f(1, 1, 0);
		glNormal3f(0, 0, -1);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(0, 0, .5);
			glTexCoord2f(0, 2);
			glVertex3f(0, .5, .5);
			glTexCoord2f(4, 2);
			glVertex3f(.5, .5, .5);
			glTexCoord2f(4, 0);
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

		//glPushMatrix();
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

	void updateBullets(){
		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i].translate = translate;
		}
	}

	void shotBullet(){
		if (bullets.empty())
		{
			bullets.push_back(Bullet());
			bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
			bullets.back().theta = rCanon;
			bullets.back().translate = translate;
		}

		updateBullets();

		//bullets.end()->speed = 0.05;
		bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
		bullets.back().translate = translate;
		bullets.back().theta = rCanon;
		bullets.back().shot = true;

		bullets.push_back(Bullet());
		bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
		bullets.back().translate = translate;
		bullets.back().theta = rCanon;
		bullets.back().drawBullet();
	}

	void drawBullet2(){
		if (bullets.empty())
		{
			bullets.push_back(Bullet());
			bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
			bullets.back().translate = translate;
			bullets.back().theta = rCanon;
		}

		//bullets.back().drawBullet();

		for (int i = 0; i < bullets.size(); i++)
		{
			if (bullets[i].outOfRange)
			{
				bullets.erase(bullets.begin() + i);
			}

			if (!bullets[i].shot)
			{
				bullets[i].theta = rCanon;
			}

			bullets[i].drawBullet();
			bullets[i].drawCurrentPos();
		}
	}

	void drawCar()
	{
		drawBullet2();

		glPushMatrix(); //draw the car body
			glColor3f(1, 1, 1);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture[showText]);

			glScalef(2, 1, 1);
			drawUnitCube();

			glBindTexture(GL_TEXTURE_2D, 2);
			glDisable(GL_TEXTURE_2D);

		glPushMatrix(); //draw the canon
			glTranslatef(0.5, 0.5, 0.5);

		//temp1.rotate = Vec3Df(0, 0, -1);
		//temp1.translate = Vec3Df(PositionBullet[0], PositionBullet[1], PositionBullet[2]);
		//temp1.theta = rCanon;

		//drawBullet2();
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture[showText]);

			glRotatef(rCanon, 0, 0, -1); //control the canon
			glScalef(1.2, 0.3, 0.3);
			//glNormal3f(1, 2, 0);
			drawUnitCube();

			glBindTexture(GL_TEXTURE_2D, 2);
			glDisable(GL_TEXTURE_2D);

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

	//void drawEnemy(){
	//	glPushMatrix();
	//	glTranslatef(3, 0, 0);
	//	glScalef(2, 2, 2);
	//	drawUnitFace();
	//	glPopMatrix();
	//}
};


#endif