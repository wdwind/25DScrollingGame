#ifndef BULLET
#define BULLET

#include "Vertex.h"
#include <vector>
#include "loadppm.h"
#include "texture.h"

#define PI 3.14159265

class Bullet {
public:
	Bullet() {
		//initTexture();
	};
	Bullet(Vec3Df o){
		origin = o;
		//initTexture();
		//drawBullet();
	}

	// Starting position corresponding to the absolute coordinate system
	Vec3Df origin = Vec3Df(0, 0, 0);

	// Translation corresponding to the starting position
	Vec3Df translate = Vec3Df(0, 0, 0);
	Vec3Df translateWhenShot = Vec3Df(0, 0, 0);

	// Rotation corresponding to the starting position
	Vec3Df rotate = Vec3Df(0, 0, -1);
	float theta = 0;

	float speedX = 0;
	bool outOfRange = false;
	bool shot = false;

	int texInd = 0;

	void drawBullet(){
		GLUquadricObj *sphere = NULL;
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere, GLU_SMOOTH);

		texInd = (int)(3 * speedX);
		while (texInd > 16)
		{
			texInd = texInd%17;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[texInd]);
		glColor3f(1, 1, 1);
		glPushMatrix();
			//cout << theta << endl;
			
			glTranslatef(origin[0], origin[1], origin[2]);
			glRotatef(theta, rotate[0], rotate[1], rotate[2]);

			if (shot)
			{
				Vec3Df offset = translateWhenShot - translate;
				//cout << translateWhenShot << endl;
				glTranslatef(offset[0], offset[1], offset[2]);
				glTranslatef(speedX, 0, 0);
				speedX += 0.03;

				if (speedX > 6)
				{
					outOfRange = true;
				}
			}
			//glTranslatef(0, 0, .23);
			gluSphere(sphere, .1, 20, 20);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);
	}

	Vec3Df getCurrentPos(){
		Vec3Df cp = origin;
		if (shot)
		{
			cp = cp + translateWhenShot;
		}
		else
			cp = cp + translate;
		//cp = Vec3Df(origin[0] * cos(theta*PI / 180) + origin[1] * sin(theta*PI / 180), -origin[0] * sin(theta*PI / 180) + origin[1] * cos(theta*PI / 180), origin[2]);
		if (shot)
		{
			cp = cp + Vec3Df(speedX * cos(-theta*PI / 180), speedX * sin(-theta*PI / 180), 0);
		}

		return cp;
	}

	void drawCurrentPos(){
		glBegin(GL_LINES);

		glColor3f(1, 1, 0);
		glVertex3f(0, 0, 0);
		Vec3Df cp = getCurrentPos() - translate;
		glVertex3f(cp[0], cp[1], cp[2]);

		glEnd();

		//cout << cp[0] << " " << cp[1] << " " << cp[2] << endl;
	}

};

#endif 
