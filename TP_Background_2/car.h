#ifndef CAR
#define CAR

#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <iostream>
#include "bullet.h"
#include "texture.h"

/************************************************************
* Class Car
************************************************************/
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

	// Translation
	Vec3Df translate = Vec3Df(0, 0, 0);

	// Sphere, used in draw wheel
	GLUquadricObj *sphere = NULL;

	// Rotation angles of the cannon and wheel
	GLfloat rCanon = 0.0f;
	GLfloat rBall = 0.0f;
	// Speed of the rotation of the wheel
	GLfloat incrementOfrball = 2.8f;

	std::vector<float> PositionBullet;

	// Bullets
	std::vector<Bullet> bullets;
	
	bool debugInCar = false;

	/************************************************************
	* Fonctions draw wheel
	************************************************************/
	void drawBall(void) {
		float ballX = 0.1f;
		float ballY = 0.1f;
		float ballZ = 0.5f;

		glColor3f(1.0, 1.0, 1.0); //set ball colour

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[wheelTex]);

		glPushMatrix();
			glTranslatef(ballX, ballY, ballZ);
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); 
		glPopMatrix();

		glPushMatrix();
			glTranslatef(ballX + .8, ballY, ballZ);
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); 
		glPopMatrix();

		glPushMatrix();
			glTranslatef(ballX, ballY, ballZ + .5);
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); 
		glPopMatrix();

		glPushMatrix();
			glTranslatef(ballX + .8, ballY, ballZ + .5); 
			glRotatef(rBall, 0, 0, -1);
			gluSphere(sphere, .2, 10, 10); 
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);
	}

	/************************************************************
	* Fonctions draw face
	************************************************************/
	void drawUnitFace()
	{
		glNormal3f(0, 0, -1);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(0, 0, .5);
			glTexCoord2f(0, 1);
			glVertex3f(0, .5, .5);
			glTexCoord2f(1, 1);
			glVertex3f(.5, .5, .5);
			glTexCoord2f(1, 0);
			glVertex3f(.5, 0, .5);
		glEnd();

	}

	/************************************************************
	* Fonctions draw unit cube
	************************************************************/
	void drawUnitCube()
	{
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

	/************************************************************
	* Fonctions update the translation of the car to the bullets
	************************************************************/
	void updateBullets(){
		for (int i = 0; i < bullets.size(); i++)
		{
			//if (!bullets[i].shot)
			//{
			//	bullets[i].translate = translate;
			//}

			bullets[i].translate = translate;
		}
	}

	/************************************************************
	* Fonctions shot bullet
	************************************************************/
	void shotBullet(){
		if (bullets.empty())
		{
			bullets.push_back(Bullet());
			bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
			bullets.back().theta = rCanon;
			bullets.back().translate = translate;
		}

		//bullets.end()->speed = 0.05;
		bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
		bullets.back().translate = translate;
		bullets.back().translateWhenShot = translate;
		bullets.back().theta = rCanon;
		bullets.back().shot = true;

		bullets.push_back(Bullet());
		bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
		bullets.back().translate = translate;
		bullets.back().theta = rCanon;
		bullets.back().drawBullet();
	}

	/************************************************************
	* Fonctions draw bullet
	************************************************************/
	void drawBullet2(){
		if (bullets.empty())
		{
			bullets.push_back(Bullet());
			bullets.back().origin = Vec3Df(PositionBullet[0] + 0.5, PositionBullet[1] + 0.5, PositionBullet[2] + 0.5);
			bullets.back().translate = translate;
			bullets.back().theta = rCanon;
		}

		updateBullets();

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

			if (debugInCar)
			{
				bullets[i].drawCurrentPos();
			}
		}
	}

	/************************************************************
	* Fonctions draw car
	************************************************************/
	void drawCar()
	{
		drawBullet2();

		glPushMatrix(); //draw the car body
			glColor3f(1, 1, 1);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture[carBodyTex]);

			glScalef(2, 1, 1);
			drawUnitCube();

			glBindTexture(GL_TEXTURE_2D, 2);
			glDisable(GL_TEXTURE_2D);

		glPushMatrix(); //draw the canon
			glTranslatef(0.5, 0.5, 0.45);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture[cannonTex]);

			glRotatef(rCanon, 0, 0, -1); //control the canon
			glScalef(1.2, 0.4, 0.4);
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

};


#endif