#ifndef UTILS
#define UTILS

#include <iostream>
#include "enemy.h"

void printVector(float* v, int num){
	for (int i = 0; i < num; i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

// Basic variables
unsigned int W_fen = 1200;  // largeur fenetre
unsigned int H_fen = 800;  // hauteur fenetre

// meshes and enemies
std::vector<Mesh> meshes;

Vec3Df defaultCarTrans = Vec3Df(-1.5, 0, 0);
Car car = Car(defaultCarTrans);
bool carMove = false;
bool drawC = true;

Mesh Boss;
bool boss = false;
Vec3Df bossStartPos = Vec3Df(3, 1, 1);

std::vector<Enemy> enemies;
int destroyedE = 0;
Vec3Df enemyStartPos = Vec3Df(3, 0.5, 1);
Vec3Df enemyStartPos2 = Vec3Df(2, 2, 1);

// For all the spheres
GLUquadricObj *sphere = NULL;

//Background color
float BackgroundColor[] = { 0, 0, 0 };

// Different display modes
// enum Mode{ ORIGINAL_LIGHTING=0, DIFFUSE_LIGHTING, SPECULAR_LIGHTING, COMBINED_LIGHTING, TOON_LIGHTING};
//Currently active mode
Mode mode = COMBINED_LIGHTING;

//if the program runs too slow, change this to false.
//to compute a result, you then need to press 'u'
bool updateAlways = true;

// Light
std::vector<Vec3Df> LightPos;
Vec3Df defaultLightPos = Vec3Df(-1, 2.5, 1);
std::vector<Vec3Df> LightColor;
Vec3Df defaultLightColor = Vec3Df(1, 1, 1);
int SelectedLight = 0;
float lightPower = 8;
float s = 5;

Light light;


//The position of the camera
//this variable will be updated by the program! Do not change it manually!
//Of course, you can USE its value
Vec3Df CamPos;// = Vec3Df(0.0f, 0.0f, -4.0f);

//The index of the chosen vertex is stored in this variable
//the value is -1 if no vertex is chosen
//to choose a vertex, hover over it and press space
//int SelectedVertex = -1;

//per vertex attributes, useful for materials - see later exercises
//std::vector<Vec3Df> MeshMaterial;

// Texture
int texTerrain = 4;
int texRoad1 = 5;
int texRoad2 = 6;
int texSky = 0;
int texSun = 0;

int temp = showText;
int maxTex = 17;


// Roads and mountains!!!
int NbVertX = 13;
int NbVertY = 5;
float qurdSize = 1;

float Vx1 = -5;
float Vx2 = (NbVertX - 1)*qurdSize + Vx1;
float threshold = -16.5;

float backgroundSpeed = 0.01;





bool drawN = false;



bool shot = false;
bool temp1 = false;
bool temp2 = false;


bool drawB = false;





#endif