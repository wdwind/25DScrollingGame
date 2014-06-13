#ifndef ENEMY
#define ENEMY

#include "mesh.h"
#include "Vec3D.h"

class Enemy : public Mesh{
public:
	Enemy(){}
	Enemy(Vec3Df v) : origin (v) {};

	//void calculateCenter();
	//void claculateMaxDist();
	//bool loadMesh(const char * filename);

	Vec3Df centerPoint = Vec3Df(0, 0, 0);
	Vec3Df origin = Vec3Df(0, 0, 0);
	float maxDist = 0;
	float speed = 0.05;

	bool loadMesh(const char * filename){
		Mesh::loadMesh(filename);
		calculateCenter();
		claculateMaxDist();

		return 1;
	}

	void calculateCenter(){
		centerPoint = Vec3Df(0, 0, 0);

		for (int i = 0; i < vertices.size(); i++)
		{
			centerPoint += vertices[i].p;
		}

		centerPoint /= vertices.size();
	}

	void claculateMaxDist(){
		maxDist = 0;

		for (int i = 0; i < vertices.size(); i++)
		{
			float dist = Vec3Df::distance(centerPoint, vertices[i].p);
			maxDist = (dist > maxDist) ? dist : maxDist;
		}
	}

	Vec3Df getCurrentPos(){
		Vec3Df cp = origin + translate + centerPoint;

		return cp;
	}

	void move(){
		int randomvalX = rand() % 3 - 1;
		int randomvalY = rand() % 3 - 1;

		translate = translate + Vec3Df(randomvalX*speed, randomvalY*speed, 0);
	}

	void drawCurrentPos(){
		glBegin(GL_LINES);

		glColor3f(1, 1, 0);
		glVertex3f(0, 0, 0);
		Vec3Df cp = getCurrentPos();
		glVertex3f(cp[0], cp[1], cp[2]);

		glEnd();
	}
	
};

#endif