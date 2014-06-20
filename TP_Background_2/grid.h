#ifndef GRID_H
#define GRID_H

#include "Vec3D.h"
#include <vector>
#include "Vertex.h"
#include <map>

typedef std::map<unsigned, std::vector<unsigned int> > CellContent;
typedef std::map<unsigned, Vertex> RepresentantList;

class Grid
{
public:
    Grid(){}
    inline Grid (const Vec3Df & origin, float size, unsigned int r) : origin (origin), size(size) , r(r) {}

    void drawCell(const Vec3Df & min,const Vec3Df& Max);
    void drawGrid();

    unsigned int r;
    Vec3Df origin;
    float size;

	bool activateCellContaining(int x, int y, int z);
    void addToCell(const Vec3Df & vertexPos);
    void putVertices(const std::vector<Vertex> & vertices);
    unsigned int isContainedAt(const Vec3Df & pos);


	void computeRepresentants(const std::vector<Vertex> & vertices);
    Vec3Df getRepresentant(const Vec3Df & pos);

    CellContent verticesInCell;
	RepresentantList representants;
};

#endif // GRID_H
