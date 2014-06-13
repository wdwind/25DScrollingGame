#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include <vector>

/************************************************************
 * Class de triangle de base
 ************************************************************/
class Triangle {
public:
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
    }
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }
    inline virtual ~Triangle () {}
    inline Triangle & operator= (const Triangle & t) {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
        return (*this);
    }
    unsigned int v[3];
};

/************************************************************
 * Class de maillage basique
 ************************************************************/
class Mesh {
public:
    Mesh();
    inline Mesh (const std::vector<Vertex> & v, const std::vector<Triangle> & t) : vertices (v), triangles (t)  {}
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
	std::vector<float> texcords2f;

    bool loadMesh(const char * filename);
	bool loadMesh(int NbVertX = 7, int NbVertY = 3, float qurdSize = 1);
	void loadRoad(int NbVertX = 7, int NbVertY = 3, float qurdSizeX = 1, float qurdSizeY = 1);
    void computeVertexNormals ();
    void centerAndScaleToUnit ();
    void draw();
    void drawSmooth();
	void drawWithColors(const std::vector<Vec3Df> & colors);
	void drawWithColors();
	void drawNormals();
	void drawSomeP(float* mv);
	void drawSomeP();
    void computeBoundingCube();
	int getClosestVertexIndex(const Vec3Df & origin, const Vec3Df & direction);

	//void setLighting(std::vector<Vec3Df> & l) { lighting = l;};
	//void setTranslate(Vec3Df t) { translate = t; };
	//
	//std::vector<Vec3Df> & getLighting() { return lighting;}
	//Vec3Df getTranslate() { return translate; }

    //Bounding box information
    Vec3Df bbMinPos;
    float bbEdgeSize;

	std::vector<Vec3Df> lighting;
	Vec3Df translate = Vec3Df(0, 0, 0);
};

#endif // MESH_H
