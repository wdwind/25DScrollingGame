#ifndef BULLET
#define BULLET

#include "Vertex.h"
#include <vector>

#define PI 3.14159265

class Bullet {
public:
	Bullet() {};
	Bullet(Vec3Df o){
		origin = o;
		Texture.resize(3);
		Texture[0] = 0;
		Texture[1] = 0;
		Texture[2] = 0;
		//drawBullet();
	}

	// Starting position corresponding to the absolute coordinate system
	Vec3Df origin = Vec3Df(0, 0, 0);

	// Translation corresponding to the starting position
	Vec3Df translate = Vec3Df(0, 0, 0);

	// Rotation corresponding to the starting position
	Vec3Df rotate = Vec3Df(0, 0, -1);
	float theta = 0;

	float speedX = 0;
	bool outOfRange = false;
	bool shot = false;

	std::vector<GLuint> Texture;

	void drawBullet(){
		GLUquadricObj *sphere = NULL;
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere, GLU_SMOOTH);

		glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, Texture[1]);
		glColor3f(0, 1, 0);
		glPushMatrix();
			//cout << theta << endl;
			
			glTranslatef(origin[0], origin[1], origin[2]);
			glRotatef(theta, rotate[0], rotate[1], rotate[2]);

			if (shot)
			{
				glTranslatef(speedX, 0, 0);
				speedX += 0.05;
			}
			//glTranslatef(0, 0, .23);
			gluSphere(sphere, .1, 20, 20);
		glPopMatrix();

		//glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);
	}

	Vec3Df getCurrentPos(){
		Vec3Df cp;
		cp = Vec3Df(origin[0] * cos(theta*PI / 180) + origin[1] * sin(theta*PI / 180), -origin[0] * sin(theta*PI / 180) + origin[1] * cos(theta*PI / 180), origin[2]);
		cp = cp + translate;

		return cp;
	}

	void drawCurrentPos(){
		glBegin(GL_LINES);

		glColor3f(1, 1, 0);
		//glTranslatef(SurfaceVertices3f[i], SurfaceVertices3f[i+1], SurfaceVertices3f[i+2]);
		glVertex3f(0, 0, 0);
		Vec3Df cp = getCurrentPos();
		glVertex3f(cp[0], cp[1], cp[2]);

		glEnd();

		cout << cp[0] << " " << cp[1] << " " << cp[2] << endl;
	}

	void drawShot(){

	}

	//std::vector<Vertex> vertices;
	//std::vector<float> texcords2f;

	//bool loadMesh(const char * filename);
	//bool loadMesh(int NbVertX = 7, int NbVertY = 3, float qurdSize = 1);
	//void loadRoad(int NbVertX = 7, int NbVertY = 3, float qurdSizeX = 1, float qurdSizeY = 1);
	//void computeVertexNormals();
	//void centerAndScaleToUnit();
	//void draw();
	//void drawSmooth();
	//void drawWithColors(const std::vector<Vec3Df> & colors);
	//void drawNormals();
	//void drawSomeP(float* mv);
	//void drawSomeP();
	//void computeBoundingCube();
	//int getClosestVertexIndex(const Vec3Df & origin, const Vec3Df & direction);

	////Bounding box information
	//Vec3Df bulletPos;
	//Vec3Df bulletTranslate;
	//Vec3Df bulletTex;
};

#endif 
