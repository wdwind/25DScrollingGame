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

// meshes (terrain, and road)
std::vector<Mesh> meshes;
// Parameters of roads and mountains!!!
int NbVertX = 13;
int NbVertY = 5;
float qurdSize = 1;
float Vx1 = -5;
float Vx2 = (NbVertX - 1)*qurdSize + Vx1;
float threshold = -16.5;
float backgroundSpeed = 0.01;


// Car
Vec3Df defaultCarTrans = Vec3Df(-1.5, 0, 0);
Car car = Car(defaultCarTrans);
//bool carMove = false;
bool drawCar = true;
bool shot = false;
float time1 = 0;
bool jump = false;

// Boss
Enemy Boss;
int boss = 0;
Vec3Df bossStartPos = Vec3Df(3, 1, 1);

// Small enemies
std::vector<Enemy> enemies;
int destroyedE = 0;
Vec3Df enemyStartPos = Vec3Df(3, 0.5, 1);
Vec3Df enemyStartPos2 = Vec3Df(1.5, 2.2, 1);

// For all the spheres
GLUquadricObj *sphere = NULL;

//Background color
float BackgroundColor[] = { 0, 0, 0 };

// Different display modes
// enum Mode{ ORIGINAL_LIGHTING=0, DIFFUSE_LIGHTING, SPECULAR_LIGHTING, COMBINED_LIGHTING, TOON_LIGHTING};
// Currently active mode uses Phong model
Mode mode = COMBINED_LIGHTING;

// If the program runs too slow, change this to false.
// to compute a result, you then need to press 'u'
bool updateAlways = true;

// Select if showing the shades or not
bool shading = false;
std::vector<Vec3Df> shadeLine;

// Light
std::vector<Vec3Df> LightPos;
Vec3Df defaultLightPos = Vec3Df(-1, 2.5, 1);
std::vector<Vec3Df> LightColor;
Vec3Df defaultLightColor = Vec3Df(1, 1, 1);
int SelectedLight = 0;
float lightPower = 8;
float s = 5;

Light light;


// The position of the camera
Vec3Df CamPos;


// Debug
bool debug = false;

//bool drawN = false;

#endif