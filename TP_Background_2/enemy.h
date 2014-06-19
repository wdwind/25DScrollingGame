#ifndef ENEMY
#define ENEMY

#include "mesh.h"
#include "Vec3D.h"
#include "grid.h"

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

	Grid grid1;
	std::map<unsigned int, unsigned int> newIndexRemapping;
	std::vector<Vertex> simplifiedVertices;
	std::vector<Triangle> simplifiedTriangles;

	std::vector<Vertex> originVertices;
	std::vector<Triangle> originTriangles;

	Mesh simplified;
	int resolution = 10;

	bool Explode = false;
	float explosionDist = 0;
	bool disappear = false;

	bool loadMesh(const char * filename){
		Mesh::loadMesh(filename);
		Mesh::computeBoundingCube();
		calculateCenter();
		claculateMaxDist();

		originVertices = vertices;
		originTriangles = triangles;
		
		grid1 = Grid(this->bbOrigin, this->bbEdgeSize, resolution);
		grid1.putVertices(vertices);
		grid1.computeRepresentants(vertices);

		int count = 0;
		for (RepresentantList::iterator it = grid1.representants.begin(); it != grid1.representants.end(); it++, count++){
			newIndexRemapping[(*it).first] = count;
			simplifiedVertices.push_back((*it).second);
		}

		for (int i = 0; i < triangles.size(); i++)
		{
			int index[3] = { 0, 0, 0 };
			for (int j = 0; j < 3; j++)
			{
				index[j] = grid1.isContainedAt(vertices[triangles[i].v[j]].p);
			}

			if (!checkRepeat(index))
			{
				simplifiedTriangles.push_back(Triangle(newIndexRemapping[index[0]], newIndexRemapping[index[1]], newIndexRemapping[index[2]]));
			}
		}

		simplified = Mesh(simplifiedVertices, simplifiedTriangles);
		simplified.computeVertexNormals();

		newIndexRemapping.clear();
		simplifiedVertices.clear();
		simplifiedTriangles.clear();

		return 1;
	}

	bool loadBoss(const char * filename){
		Mesh::loadBoss(filename);
		Mesh::computeBoundingCube();
		calculateCenter();
		claculateMaxDist();

		originVertices = vertices;
		originTriangles = triangles;

		grid1 = Grid(this->bbOrigin, this->bbEdgeSize, resolution);
		grid1.putVertices(vertices);
		grid1.computeRepresentants(vertices);

		int count = 0;
		for (RepresentantList::iterator it = grid1.representants.begin(); it != grid1.representants.end(); it++, count++){
			newIndexRemapping[(*it).first] = count;
			simplifiedVertices.push_back((*it).second);
		}

		for (int i = 0; i < triangles.size(); i++)
		{
			int index[3] = { 0, 0, 0 };
			for (int j = 0; j < 3; j++)
			{
				index[j] = grid1.isContainedAt(vertices[triangles[i].v[j]].p);
			}

			if (!checkRepeat(index))
			{
				simplifiedTriangles.push_back(Triangle(newIndexRemapping[index[0]], newIndexRemapping[index[1]], newIndexRemapping[index[2]]));
			}
		}

		simplified = Mesh(simplifiedVertices, simplifiedTriangles);
		simplified.computeVertexNormals();

		newIndexRemapping.clear();
		simplifiedVertices.clear();
		simplifiedTriangles.clear();

		return 1;
	}

	bool checkRepeat(int *indices){
		return (indices[0] == indices[1] || indices[1] == indices[2] || indices[0] == indices[2]);
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

		int num = 0;

		for (int i = 0; i < vertices.size(); i++)
		{
			if (fabs(vertices[i].p[0] - centerPoint[0]) < 0.5)
			{
				float dist = Vec3Df::distance(centerPoint, vertices[i].p);
				//maxDist = (dist > maxDist) ? dist : maxDist;
				maxDist += dist;
				num++;
			}
		}

		maxDist /= num;
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

	bool Shot(){
		if (resolution < 2)
		{
			return false;
		}

		vertices = simplified.vertices;
		triangles = simplified.triangles;
		lighting.resize(vertices.size());

		Mesh::computeBoundingCube();
		calculateCenter();
		claculateMaxDist();

		resolution -= 2;
		if (resolution <= 0)
			resolution = 1;

		cout << resolution << endl;

		grid1 = Grid(this->bbOrigin, this->bbEdgeSize, resolution);
		grid1.putVertices(vertices);
		grid1.computeRepresentants(vertices);

		int count = 0;
		for (RepresentantList::iterator it = grid1.representants.begin(); it != grid1.representants.end(); it++, count++){
			newIndexRemapping[(*it).first] = count;
			simplifiedVertices.push_back((*it).second);
		}

		for (int i = 0; i < triangles.size(); i++)
		{
			int index[3] = { 0, 0, 0 };
			for (int j = 0; j < 3; j++)
			{
				index[j] = grid1.isContainedAt(vertices[triangles[i].v[j]].p);
			}

			if (!checkRepeat(index))
			{
				simplifiedTriangles.push_back(Triangle(newIndexRemapping[index[0]], newIndexRemapping[index[1]], newIndexRemapping[index[2]]));
			}
		}

		simplified = Mesh(simplifiedVertices, simplifiedTriangles);
		simplified.computeVertexNormals();

		newIndexRemapping.clear();
		simplifiedVertices.clear();
		simplifiedTriangles.clear();

		return true;
	}

	bool explode(){
		if (!Explode)
		{
			std::vector<Vertex> tempV;
			std::vector<Triangle> tempT;

			vertices = originVertices;
			triangles = originTriangles;

			int ind = 0;
			for (int i = 0; i < triangles.size(); i++)
			{
				for (int j = 0; j < 3; j++)
				{
					tempV.push_back(vertices[triangles[i].v[j]]);
					ind++;
				}
				tempT.push_back(Triangle(ind - 3, ind - 2, ind - 1));
			}

			vertices = tempV;
			triangles = tempT;

			lighting.resize(vertices.size());

			Mesh::computeBoundingCube();
			calculateCenter();
			claculateMaxDist();
			computeVertexNormals();

			originTriangles.clear();
			originVertices.clear();
			newIndexRemapping.clear();
			simplifiedVertices.clear();
			simplifiedTriangles.clear();
			grid1 = Grid();
			simplified = Mesh();

			Explode = true;
		}

		float speed = 0.2;
		float threshold = 1;

		explosionDist += speed / 10;
		if (explosionDist > 1)
		{
			disappear = true;
		}

		for (int i = 0; i < vertices.size(); i++)
		{
			//Vec3Df dir = (vertices[i].p - centerPoint).unit();
			vertices[i].p = vertices[i].p + speed * vertices[i].n;
		}

		if (disappear)
		{
			cout << "Disappear" << endl;
		}

		return true;
	}

	void drawWithColors(){
		if (Explode)
		{
			explode();
		}

		Mesh::drawWithColors();
	}

	void drawBoss(){
		if (Explode)
		{
			explode();
		}

		Mesh::drawBoss();
	}
	
};

#endif