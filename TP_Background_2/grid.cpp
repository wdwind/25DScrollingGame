#include "grid.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

unsigned int Grid::isContainedAt(const Vec3Df & pos){
    //Complete return index that contains the position
    int indice = 0;
	float cellSize = size / r;

	Vec3Df pos2 = pos - origin;

	int x = pos2[0] / cellSize + 1;
	int y = pos2[1] / cellSize + 1;
	int z = pos2[2] / cellSize + 1;

	indice = r*r*(z - 1) + r*(y - 1) + x - 1;

    return indice;
}

void Grid::putVertices(const std::vector<Vertex> & vertices){
    //put vertices in the corresponding voxels.
	for (int i = 0; i < vertices.size(); i++)
	{
		int index = isContainedAt(vertices[i].p);
		//verticesInCell.find()
		if (verticesInCell.find(index) == verticesInCell.end())
		{
			std::vector<unsigned int> temp;
			temp.clear();
			temp.push_back(i);
			verticesInCell[index] = temp;
		}
		else
		{
			verticesInCell[index].push_back(i);
		}
	}

}

void Grid::computeRepresentants(const std::vector<Vertex> & vertices){
	for (std::map<unsigned int, std::vector<unsigned int>>::iterator iter = verticesInCell.begin(); iter != verticesInCell.end(); ++iter)
	{
		Vec3Df point = Vec3Df(0, 0, 0);
		Vec3Df normal = Vec3Df(0, 0, 0);

		for (int i = 0; i < iter->second.size(); i++)
		{
			point = point + vertices[iter->second[i]].p;
			normal = normal + vertices[iter->second[i]].n;
		}

		point = point / iter->second.size();
		normal = normal / iter->second.size();
		representants[iter->first] = Vertex(point, normal);
	}
}

void Grid::drawCell(const Vec3Df & min,const Vec3Df& Max) {

	/*std::cout << min[0] << std::endl;
	std::cout << Max[2] << std::endl;*/

    const Vec3Df corners[8] =
    {
        Vec3Df(min[0],min[1],min[2]),
        Vec3Df(Max[0],min[1],min[2]),
        Vec3Df(min[0],Max[1],min[2]),
        Vec3Df(Max[0],Max[1],min[2]),
        Vec3Df(min[0],min[1],Max[2]),
        Vec3Df(Max[0],min[1],Max[2]),
        Vec3Df(min[0],Max[1],Max[2]),
        Vec3Df(Max[0],Max[1],Max[2])
    };


    static const unsigned short faceCorners[6][4] =
    {
        { 0,4,6,2 },
        { 5,1,3,7 },
        { 0,1,5,4 },
        { 3,2,6,7 },
        { 0,2,3,1 },
        { 6,4,5,7 }
    };

    glBegin(GL_QUADS);
    for (unsigned short f=0;f<6;++f)
    {
        const unsigned short* face = faceCorners[f];
        for(unsigned int v = 0; v<4; v++)
            glVertex3f(corners[face[v]][0], corners[face[v]][1], corners[face[v]][2]);

    }
    glEnd();

}

bool Grid::activateCellContaining(int x, int y, int z){
	int index = x + r*y + r*r*z;

	if (verticesInCell.find(index) == verticesInCell.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Grid::drawGrid(){
	float cellSize = size / r;

	/*std::cout << size << std::endl;
	std::cout << cellSize << std::endl;*/

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLineWidth(1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    //Complete this function by calling draw cell several times.
	for (int x = 0; x < r; x++)
	{
		for (int y = 0; y < r; y++)
		{
			for (int z = 0; z < r; z++)
			{
				if (activateCellContaining(x, y, z))
				{
					Vec3Df interval = Vec3Df(x*cellSize, y*cellSize, z*cellSize);
					drawCell(origin + interval, origin + interval + Vec3Df(cellSize, cellSize, cellSize));
				}
			}
		}
	}

    glPopAttrib();
}


