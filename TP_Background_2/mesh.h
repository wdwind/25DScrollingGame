#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include <vector>
#include "light.h"

/************************************************************
 * Class triangle
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

enum Mode{ ORIGINAL_LIGHTING = 0, DIFFUSE_LIGHTING, SPECULAR_LIGHTING, COMBINED_LIGHTING, TOON_LIGHTING };

/************************************************************
 * Class Mesh
 ************************************************************/
class Mesh {
public:
    Mesh();
    inline Mesh (const std::vector<Vertex> & v, const std::vector<Triangle> & t) : vertices (v), triangles (t)  {}
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
	std::vector<float> texcords2f;

	// Load mesh from .obj file
    bool loadMesh(const char * filename);
	bool loadBoss(const char * filename);

	// Load mesh by constructing terrains
	bool loadMesh(int NbVertX = 7, int NbVertY = 3, float qurdSize = 1);
	void loadRoad(int NbVertX = 7, int NbVertY = 3, float qurdSizeX = 1, float qurdSizeY = 1);

    void computeVertexNormals ();
    void centerAndScaleToUnit ();
    void draw();
	void drawBoss();
    void drawSmooth();
	void drawWithColors(const std::vector<Vec3Df> & colors);
	void drawWithColors();
	void drawNormals();
	//id drawSomeP(float* mv);
	void drawSomeP();
    void computeBoundingCube();
	//t getClosestVertexIndex(const Vec3Df & origin, const Vec3Df & direction);
	static Vec3Df computeLighting(Vec3Df vertexPos, Vec3Df normal, Light light, Vec3Df CamPos, Mode mode);

    //Bounding box information
    Vec3Df bbOrigin;
    float bbEdgeSize;

	std::vector<Vec3Df> lighting;
	Vec3Df translate = Vec3Df(0, 0, 0);

	std::vector<unsigned int> shadow;
};

#endif // MESH_H
