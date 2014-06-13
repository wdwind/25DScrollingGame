#ifndef ENEMY
#define ENEMY

#include "mesh.h"
#include "Vec3D.h"

class Enemy : public Mesh{
public:
	Enemy() {};

	//void calculateCenter();
	//void claculateMaxDist();
	//bool loadMesh(const char * filename);

	Vec3Df centerPoint = Vec3Df(0, 0, 0);
	float maxDist = 0;

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
		return Vec3Df(0, 0, 0);
	}
	
};

#endif