#ifndef TEXTURE
#define TEXTURE

#include "loadppm.h"
#include <vector>
#include <GL/glut.h>

std::vector<GLuint> Texture;
int showText = 14;
int temp = showText;
int maxTex = 25;

int sunTex = 2;
int bossBackgroundTex = 1;
int distantBackgroundTex = 11;
int terrainTex = 14;
int roadTex = 8;
int bossTex = 17;
int wheelTex = 4;
int carBodyTex = 23;
int cannonTex = 20;

//this function loads the textures in the GPU memory
//the function is called once when the program starts
void initTexture()
{
	Texture.resize(maxTex);

	PPMImage image("checker.ppm");
	glGenTextures(1, &Texture[0]);
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.sizeX, image.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glBindTexture(GL_TEXTURE_2D, 0);

	PPMImage image2("brick.ppm");
	glGenTextures(1, &Texture[1]);
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image2.sizeX, image2.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image2.data);
	glBindTexture(GL_TEXTURE_2D, 1);

	PPMImage image3("sand.ppm");
	glGenTextures(1, &Texture[2]);
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image3.sizeX, image3.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image3.data);
	glBindTexture(GL_TEXTURE_2D, 2);

	PPMImage image4("textureBullet01X.ppm");
	glGenTextures(1, &Texture[3]);
	glBindTexture(GL_TEXTURE_2D, Texture[3]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image4.sizeX, image4.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image4.data);
	glBindTexture(GL_TEXTURE_2D, 3);

	PPMImage image5("textureBullet02X.ppm");
	glGenTextures(1, &Texture[4]);
	glBindTexture(GL_TEXTURE_2D, Texture[4]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image5.sizeX, image5.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image5.data);
	glBindTexture(GL_TEXTURE_2D, 4);

	PPMImage image6("textureJeep01X.ppm");
	glGenTextures(1, &Texture[5]);
	glBindTexture(GL_TEXTURE_2D, Texture[5]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image6.sizeX, image6.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image6.data);
	glBindTexture(GL_TEXTURE_2D, 5);

	PPMImage image7("textureJeep02X.ppm");
	glGenTextures(1, &Texture[6]);
	glBindTexture(GL_TEXTURE_2D, Texture[6]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image7.sizeX, image7.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image7.data);
	glBindTexture(GL_TEXTURE_2D, 6);

	PPMImage image8("textureJeep03X.ppm");
	glGenTextures(1, &Texture[7]);
	glBindTexture(GL_TEXTURE_2D, Texture[7]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image8.sizeX, image8.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image8.data);
	glBindTexture(GL_TEXTURE_2D, 7);

	PPMImage image9("textureRoad01.ppm");
	glGenTextures(1, &Texture[8]);
	glBindTexture(GL_TEXTURE_2D, Texture[8]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image9.sizeX, image9.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image9.data);
	glBindTexture(GL_TEXTURE_2D, 8);

	PPMImage image10("textureRoad02.ppm");
	glGenTextures(1, &Texture[9]);
	glBindTexture(GL_TEXTURE_2D, Texture[9]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image10.sizeX, image10.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image10.data);
	glBindTexture(GL_TEXTURE_2D, 9);

	PPMImage image11("textureSky01X.ppm");
	glGenTextures(1, &Texture[10]);
	glBindTexture(GL_TEXTURE_2D, Texture[10]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image11.sizeX, image11.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image11.data);
	glBindTexture(GL_TEXTURE_2D, 10);

	PPMImage image12("textureSky02X.ppm");
	glGenTextures(1, &Texture[11]);
	glBindTexture(GL_TEXTURE_2D, Texture[11]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image12.sizeX, image12.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image12.data);
	glBindTexture(GL_TEXTURE_2D, 11);

	PPMImage image13("sun01.ppm");
	glGenTextures(1, &Texture[12]);
	glBindTexture(GL_TEXTURE_2D, Texture[12]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image13.sizeX, image13.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image13.data);
	glBindTexture(GL_TEXTURE_2D, 12);

	PPMImage image14("sun03.ppm");
	glGenTextures(1, &Texture[13]);
	glBindTexture(GL_TEXTURE_2D, Texture[13]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image14.sizeX, image14.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image14.data);
	glBindTexture(GL_TEXTURE_2D, 13);

	PPMImage image15("textureTerrainX.ppm");
	glGenTextures(1, &Texture[14]);
	glBindTexture(GL_TEXTURE_2D, Texture[14]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image15.sizeX, image15.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image15.data);
	glBindTexture(GL_TEXTURE_2D, 14);

	PPMImage image16("textureWheel01X.ppm");
	glGenTextures(1, &Texture[15]);
	glBindTexture(GL_TEXTURE_2D, Texture[15]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image16.sizeX, image16.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image16.data);
	glBindTexture(GL_TEXTURE_2D, 15);

	PPMImage image17("textureWheel02X.ppm");
	glGenTextures(1, &Texture[16]);
	glBindTexture(GL_TEXTURE_2D, Texture[16]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image17.sizeX, image17.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image17.data);
	glBindTexture(GL_TEXTURE_2D, 16);

	PPMImage image18("boss.ppm");
	glGenTextures(1, &Texture[17]);
	glBindTexture(GL_TEXTURE_2D, Texture[17]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image18.sizeX, image18.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image18.data);
	glBindTexture(GL_TEXTURE_2D, 17);

	PPMImage image19("boss02.ppm");
	glGenTextures(1, &Texture[18]);
	glBindTexture(GL_TEXTURE_2D, Texture[18]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image19.sizeX, image19.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image19.data);
	glBindTexture(GL_TEXTURE_2D, 18);

	PPMImage image20("car03.ppm");
	glGenTextures(1, &Texture[19]);
	glBindTexture(GL_TEXTURE_2D, Texture[19]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image20.sizeX, image20.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image20.data);
	glBindTexture(GL_TEXTURE_2D, 19);

	PPMImage image21("car04.ppm");
	glGenTextures(1, &Texture[20]);
	glBindTexture(GL_TEXTURE_2D, Texture[20]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image21.sizeX, image21.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image21.data);
	glBindTexture(GL_TEXTURE_2D, 20);

	PPMImage image22("car06.ppm");
	glGenTextures(1, &Texture[21]);
	glBindTexture(GL_TEXTURE_2D, Texture[21]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image22.sizeX, image22.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image22.data);
	glBindTexture(GL_TEXTURE_2D, 21);

	PPMImage image23("car07.ppm");
	glGenTextures(1, &Texture[22]);
	glBindTexture(GL_TEXTURE_2D, Texture[22]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image23.sizeX, image23.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image23.data);
	glBindTexture(GL_TEXTURE_2D, 22);

	PPMImage image24("jeep01.ppm");
	glGenTextures(1, &Texture[23]);
	glBindTexture(GL_TEXTURE_2D, Texture[23]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image24.sizeX, image24.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image24.data);
	glBindTexture(GL_TEXTURE_2D, 23);

	PPMImage image25("jeep02.ppm");
	glGenTextures(1, &Texture[24]);
	glBindTexture(GL_TEXTURE_2D, Texture[24]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image25.sizeX, image25.sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image25.data);
	glBindTexture(GL_TEXTURE_2D, 24);
}

#endif