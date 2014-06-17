#ifndef GRID_H
#define GRID_H

#include "Vec3D.h"
#include <vector>
#include "Vertex.h"
#include <map>

typedef std::map<unsigned, std::vector<unsigned int> > CellContent;
typedef std::map<unsigned, Vertex> RepresentantList;

//The above structures can be used almost like a vector!
//The differenc is that they are sparse. This means: each entry will only exist ONCE in memory and 
//only after it has been used. Further, the index to do the lookup does not have to be an int... 
//only, in the above case, unsigned int was used, so you can actually think about the above structures as an 
//std::vector<unsigned int> and a std::vector<Vertex>

//One big difference occurs when iterating over all elements:
//std::map<unsigned int, unsigned int> t;
//t[2]=3;
//t[3]=5;
//for (std::map<unsigned int, unsigned int>::iterator iter=t.begin; iter!=t.end();++iter)
//{
// std::cout<< "t["<<iter->first<<"]<<"="<<iter->second<<endl;
//}
//empty elements will be left out

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
