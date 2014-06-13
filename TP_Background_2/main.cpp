#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "traqueboule.h"
#include "mesh.h"
#include "loadppm.h"
#include "car.h"
#include "move.h"
#include "utils.h"

using namespace std;

// Functions
void computeLighting();
void dealWithUserInput(int x, int y);
void drawCar();
void initTexture();
void keyboard(unsigned char key, int x, int y);
void display(void);
void reshape(int w, int h);

// Basic variables
unsigned int W_fen = 800;  // largeur fenetre
unsigned int H_fen = 800;  // hauteur fenetre

std::vector<GLuint> Texture;
//Mesh MyMesh, MyMesh2, enemy;
std::vector<Mesh> meshes;
std::vector<std::vector<Vec3Df>> lightings;
std::vector<Vec3Df> translates;

int enemyIndex = 4;

//std::vector<Vec3Df> lighting;
//std::vector<Vec3Df> lighting2;
//std::vector<Vec3Df> lighting3;

//Background color
float BackgroundColor[]={0,0,0};
// Different display modes
enum Mode{ ORIGINAL_LIGHTING=0, DIFFUSE_LIGHTING, SPECULAR_LIGHTING, COMBINED_LIGHTING, TOON_LIGHTING};
//Currently active mode
Mode mode = COMBINED_LIGHTING;

//if the program runs too slow, change this to false.
//to compute a result, you then need to press 'u'
bool updateAlways=true;

//There are several light positions and you can later add light sources
//for the moment, just work with LightPos[0]
//to set the light position to the camera location, you can use 'l'
std::vector<Vec3Df> LightPos;
//Later in the exercise, you can also modify and use a special color per light source.
//The LightColor array is used for this purpose:
std::vector<Vec3Df> LightColor;
//the current light
int SelectedLight=0;

//The position of the camera
//this variable will be updated by the program! Do not change it manually!
//Of course, you can USE its value
Vec3Df CamPos;// = Vec3Df(0.0f, 0.0f, -4.0f);


//Pressing 's' will display the currently chosen vertex
bool ShowSelectedVertex=false;

//The index of the chosen vertex is stored in this variable
//the value is -1 if no vertex is chosen
//to choose a vertex, hover over it and press space
int SelectedVertex=-1;

//per vertex attributes, useful for materials - see later exercises
//std::vector<Vec3Df> MeshMaterial;

float lightPower = 5;
float s = 5;
float showDot = true;


int NbVertX = 8;
int NbVertY = 3;
float qurdSize = 1;

float Vx1 = -2;
float Vx2 = (NbVertX - 1)*qurdSize + Vx1;
float threshold = -9;

int showText = 2;
int temp = showText;
bool drawN = false;
bool drawC = false;
float backgroundSpeed = 0.005;

Vec3Df defaultLightPos = Vec3Df(2, 3, 2);

bool shot = false;
bool temp1 = false;
bool temp2 = false;
bool temp3 = true;

bool drawB = false;

float mv[16];

//Lighting of the model (you should not need to touch this one...


template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
	return max(lower, min(n, upper));
}

//this is the MAIN function in which you have to work
//it is automatically iterated over the entire mesh and all light sources (light is the current light index)
//it receives the current vertex location, its normal, and the index of the current vertex (yes, this is somewhat redundant because the mesh  
Vec3Df computeLighting(Vec3Df & vertexPos, Vec3Df & normal, unsigned int light, unsigned int index)
{
	Vec3Df MaterialDiffuseColor = Vec3Df(1, 1, 1);
	Vec3Df MaterialAmbientColor = 0 * MaterialDiffuseColor;

	//do not change any global variables here! This function will be called for EACH vertex 
	//of the mesh, so your change would happen several times
	switch(mode)
	{
	case ORIGINAL_LIGHTING:
		{
			return Vec3Df(1,1,1);
		}
	case DIFFUSE_LIGHTING:
		{
			Vec3Df lightDir = (LightPos[light] - vertexPos).unit();
			float distance = (LightPos[light] - vertexPos).getSquaredLength();
			float intensity1 = Vec3Df::dotProduct(normal, lightDir);
			intensity1 = clip(Vec3Df::dotProduct(normal, lightDir), (float)0, (float)1);

			//lightDir = lightDir.unit();
			//clip(1, 2, 3);
			//Vec3Df test = LightColor[light];
			//test = Vec3Df::dotProduct(normal, currentLight)*LightColor[light];

			return lightPower*intensity1*LightColor[light] / distance + MaterialAmbientColor;
		}
	case SPECULAR_LIGHTING:
		{
			Vec3Df lightDir = (LightPos[light] - vertexPos).unit();
			float distance = (LightPos[light] - vertexPos).getSquaredLength();
			Vec3Df camDir = (CamPos - vertexPos).unit();
			Vec3Df H = (camDir + lightDir).unit();
			float NdotH = Vec3Df::dotProduct(normal, H);

			NdotH = clip(NdotH, (float)0, (float)1);

			float intensity2 = pow(NdotH, s);

			return lightPower*intensity2*LightColor[light] / distance + MaterialAmbientColor;
		}
	case COMBINED_LIGHTING:
		{
			Vec3Df lightDir = (LightPos[light] - vertexPos).unit();
			float intensity1 = clip(Vec3Df::dotProduct(normal, lightDir), (float)0, (float)1);

			// need to divide distance???
			float distance = (LightPos[light] - vertexPos).getSquaredLength();
			//distance = 1;

			Vec3Df camDir = (CamPos - vertexPos).unit();
			Vec3Df H = (camDir + lightDir).unit();
			float NdotH = Vec3Df::dotProduct(normal, H);
			NdotH = clip(NdotH, (float)0, (float)1);
			float intensity2 = pow(NdotH, s);

			//cout << distance << endl;

			return lightPower*(intensity2*LightColor[light] + intensity1*LightColor[light]) / (distance)+MaterialAmbientColor;
		}
	case TOON_LIGHTING:
		{
			Vec3Df lightDir = (LightPos[light] - vertexPos).unit();
			float intensity1 = Vec3Df::dotProduct(normal, lightDir);
			intensity1 = clip(Vec3Df::dotProduct(normal, lightDir), (float)0, (float)1);

			float distance = (LightPos[light] - vertexPos).getSquaredLength();

			//cout << Vec3Df::dotProduct(normal, lightDir) << endl;
			float threshold = 0.5;
			float temp = 0;
			if (Vec3Df::dotProduct(normal, lightDir) > threshold)
			{
				temp = 0.7;
			}
			else
			{
				temp = 0;
			}


			//int s = 8;
			Vec3Df camDir = (CamPos - vertexPos).unit();
			Vec3Df H = (camDir + lightDir).unit();
			float NdotH = Vec3Df::dotProduct(normal, H);
			NdotH = clip(NdotH, (float)0, (float)1);
			float intensity2 = pow(NdotH, s);

			float temp2 = 0;
			float threshold2 = 0.7;
			if (intensity2 > threshold2)
			{
				temp2 = 0.7;
			}
			else
			{
				temp2 = 0;
			}

			return (temp * LightColor[light] + temp2 * LightColor[light]) * lightPower / distance + MaterialAmbientColor;
		}

	default:
		return Vec3Df(0,1,0);
	}
}

void computeLighting()
{
	for (int j = 0; j < lightings.size(); j++)
	{
		std::vector<Vec3Df> *result = &(lightings[j]);

		for (unsigned int i = 0; i<meshes[j].vertices.size(); ++i)
		{
			(*result)[i] = Vec3Df();
			for (int l = 0; l < LightPos.size(); ++l)
				(*result)[i] += computeLighting(meshes[j].vertices[i].p + translates[j], meshes[j].vertices[i].n, l, i);
		}
	}

/*
	std::vector<Vec3Df> *result = &lighting;


	for (unsigned int i = 0; i<MyMesh.vertices.size(); ++i)
	{
		(*result)[i] = Vec3Df();
		for (int l = 0; l < LightPos.size(); ++l)
			(*result)[i] += computeLighting(MyMesh.vertices[i].p + Vec3Df(Vx1, 0, 0), MyMesh.vertices[i].n, l, i);
	}

	std::vector<Vec3Df> *result2 = &lighting2;


	for (unsigned int i = 0; i<MyMesh2.vertices.size(); ++i)
	{
		(*result2)[i] = Vec3Df();
		for (int l = 0; l < LightPos.size(); ++l)
			(*result2)[i] += computeLighting(MyMesh2.vertices[i].p + Vec3Df(Vx2, 0, 0), MyMesh2.vertices[i].n, l, i);
	}

	std::vector<Vec3Df> *result3 = &lighting3;

	for (unsigned int i = 0; i<enemy.vertices.size(); ++i)
	{
		(*result3)[i] = Vec3Df();
		for (int l = 0; l < LightPos.size(); ++l)
			(*result3)[i] += computeLighting(enemy.vertices[i].p, enemy.vertices[i].n, l, i);
	}*/
}


void collisionDetect(Vec3Df start){
	glBegin(GL_LINES);
	glPushMatrix();
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5 + PositionBullet[0], 0.5 + PositionBullet[1], 0.5 + PositionBullet[2]);
	glPopMatrix();
	glEnd();

	int t;
	for (int i = 0; i < meshes[enemyIndex].triangles.size(); i++)
	{
		Vec3Df* vv;
		Vec3Df t1 = meshes[enemyIndex].vertices[meshes[enemyIndex].triangles[i].v[0]].p;
		Vec3Df t2 = meshes[enemyIndex].vertices[meshes[enemyIndex].triangles[i].v[1]].p;
		Vec3Df t3 = meshes[enemyIndex].vertices[meshes[enemyIndex].triangles[i].v[2]].p;
		RealTriangle rt = RealTriangle(t1, t2, t3);
		t = intersect3D_RayTriangle(Vec3Df(PositionBullet[0], PositionBullet[1], PositionBullet[2]), Vec3Df(PositionBullet[0] + 10, PositionBullet[1], PositionBullet[2]), rt);

		if (t==1)
			cout << t << endl;
	}
	
}

/**
* Animation
*/
void animate()
{
	Vx1 -= backgroundSpeed;
	Vx2 -= backgroundSpeed;

	if (temp3){
		rBall += incrementOfrball;
	}

	if (temp1) {
		int t = 1;
		while (t < 100) {
			t += 1;
			angleUpper += .001;
			angleFore += 0.05;
			angleHand -= .005;
		}
	}

	if (temp2) {
		glPushMatrix();
		glTranslatef(0, 0, 1.5);
		angleFore += incrementAngle;
		if (angleFore >= 100 || angleFore <= 30){
			incrementAngle = -incrementAngle;
		}
		glPopMatrix();
	}

	//collisionDetect();
}



//Everything below, you do NOT have to read!!!
//________________________________
//________________________________
//________________________________
//________________________________
//________________________________




/************************************************************
 * Function to initialize the mesh
 ************************************************************/
void init(){

	//this function loads a mesh
	Mesh enemy;
	enemy.loadMesh("enemy.obj");
	meshes.push_back(enemy);

	std::vector<Vec3Df> lighting3;
	lighting3.resize(enemy.vertices.size());

	lightings.push_back(lighting3);

	translates.push_back(Vec3Df(4, 0.5, 1));
	//MeshMaterial.resize(MyMesh.vertices.size());
	//for (int i=0; i<MyMesh.vertices.size();++i)
	//	MeshMaterial[i]=Vec3Df(0,0,0);
		
	LightPos.push_back(defaultLightPos);
	LightColor.push_back(Vec3Df(1,1,1));
	//computeLighting();
}



/************************************************************
 * Call different drawing functions
************************************************************/
void dessinerBackground()
{
	glPointSize(10);

	// Draw light
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(LightPos[0][0], LightPos[0][1], LightPos[0][2]);
		glutSolidSphere(.5, 50, 50);
	glPopMatrix();

	switch( mode )
    {
    case ORIGINAL_LIGHTING:
		{
			Vec3Df p;
			if (ShowSelectedVertex&&SelectedVertex>=0)
			{
				p=meshes[0].vertices[SelectedVertex].p;
				glBegin(GL_POINTS);
				glVertex3f(p[0],p[1],p[2]);
				glEnd();
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture[showText]);

			meshes[0].drawWithColors(lightings[0]);

			glTranslatef(3,0,0);
			meshes[1].drawWithColors(lightings[1]);

			meshes[2].drawWithColors(lightings[2]);

			glBindTexture(GL_TEXTURE_2D, 2);
			glDisable(GL_TEXTURE_2D);
		}
		break;
    default:
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[showText]);

		if (Vx1 < threshold)
		{
			Vx1 += 2 * (NbVertX - 1) * qurdSize;
		}

		if (Vx2 < threshold)
		{
			Vx2 += 2 * (NbVertX - 1) * qurdSize;
		}

		glPushMatrix();
			glTranslatef(Vx1, 0, 0);
			meshes[0].drawWithColors(lightings[0]);
			translates[0] = Vec3Df(Vx1, 0, 0);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(Vx2, 0, 0);
			meshes[1].drawWithColors(lightings[1]);
			translates[1] = Vec3Df(Vx2, 0, 0);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);


		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[1]);

		glPushMatrix();
			glTranslatef(Vx1, 0, -2);
			meshes[2].drawWithColors(lightings[2]);
			meshes[2].drawNormals();
			translates[2] = Vec3Df(Vx1, 0, -2);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(Vx2, 0, -2);
			meshes[3].drawWithColors(lightings[3]);
			meshes[3].drawNormals();
			translates[3] = Vec3Df(Vx2, 0, -2);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

		break;
    }
}

void dessinerOther(){
	glPushMatrix();
	glTranslatef(translates[enemyIndex][0], translates[enemyIndex][1], translates[enemyIndex][2]);
	//glRotatef(180, 0, 1, 0);
	//glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	//enemy.draw();
	meshes[enemyIndex].drawWithColors(lightings[enemyIndex]);
	glPopMatrix();

	if (drawN)
	{
		meshes[enemyIndex].drawNormals();
		meshes[enemyIndex].drawSomeP();
	}

	if (drawC)
		drawCar();

	if (shot){
		if (PositionBullet[0] < 6){
			PositionBullet[0] += 0.003;
		}
		else if (PositionBullet[0] >= 6){
			PositionBullet[0] = 0.03;
		}
	}

	if (drawB)
		collisionDetect(Vec3Df(0, 0, 1));

}


///////////////////////////////////////////////////////////////////////////////////////////

// prise en compte du clavier
//Vous allez ajouter quelques fonctionalites pendant le TP
//ce qui est important pour vous: key contient le caractère correspondant ?la touche appuy?par l'utilisateur

void keyboard(unsigned char key, int x, int y)
{
	printf("key %d pressed at %d,%d\n", key, x, y);
	fflush(stdout);
	if (key>'0'&& key <= '7')
	{
		mode = Mode(key - '1');
		computeLighting();
		return;
	}

	switch (key)
	{
		//A remplir
	case 'r':
		break;
	case 'R':
		break;
	case 'g':
		break;
	case 'G':
		break;
	case 'b':
		drawB = true;
		break;
	case 'B':
		break;


		//less important
	case 'l':
	{
				LightPos[SelectedLight] = getCameraPosition();
				return;
	}
	case 'L':
	{
				LightPos.push_back(getCameraPosition());
				LightColor.push_back(Vec3Df(1, 1, 1));
				return;
	}
	case '+':
	{
				++SelectedLight;
				if (SelectedLight >= LightPos.size())
					SelectedLight = 0;
				return;
	}
	case '-':
	{
				--SelectedLight;
				if (SelectedLight<0)
					SelectedLight = LightPos.size() - 1;
				return;
	}
	case 'U':
	{
				updateAlways = !updateAlways;
				return;
	}

	case 'N':
	{	//reset all lights
				LightPos.resize(1);
				LightPos[0] = defaultLightPos;
				LightColor.resize(1);
				LightColor[0] = Vec3Df(1, 1, 1);
				SelectedLight = 0;
	}

	case 'u':
	{
				//update lighing (only useful for slow computers)
				computeLighting();
				return;
	}
	case 's':
	{
				ShowSelectedVertex = !ShowSelectedVertex;
	}
	case 'x':
		Vx1 += 0.1;
		Vx2 += 0.1;
		cout << Vx1 << endl;
		break;
	case 'z':
		Vx1 -= 0.1;
		Vx2 -= 0.1;
		cout << Vx1 << endl;
		break;
	case 'c':
		//cout << showText << endl;
		showText = (++temp) % 3;
		break;
	case 'n':
		//cout << showText << endl;
		drawN = true;
		break;
	case 'a':
		cout << "draw car" << endl;
		drawC = true;
		break;
	case '.':
		canonClock();
		break;
	case ',':
		canonCounter();
		break;
	case 'v':
		temp1 = true;
		break;
	case 'f':
		temp2 = true;
		break;
	case 'j':
		shot = true;
		break;
	case 'p':
		printVector(mv, 16);
		break;
	}

}

void background(){
	Mesh MyMesh;
	std::vector<Vec3Df> lighting;

	MyMesh.loadMesh(NbVertX, NbVertY, qurdSize);
	lighting.resize(MyMesh.vertices.size());

	Mesh MyMesh2;
	std::vector<Vec3Df> lighting2;

	MyMesh2.loadMesh(NbVertX, NbVertY, qurdSize);
	lighting2.resize(MyMesh2.vertices.size());

	Mesh MyMesh3;
	std::vector<Vec3Df> lighting3;

	MyMesh3.loadRoad(NbVertX, 3, 1, 3);
	lighting3.resize(MyMesh3.vertices.size());

	Mesh MyMesh4;
	std::vector<Vec3Df> lighting4;

	MyMesh4.loadRoad(NbVertX, 3, 1, 3);
	lighting4.resize(MyMesh4.vertices.size());

	meshes.push_back(MyMesh);
	meshes.push_back(MyMesh2);
	meshes.push_back(MyMesh3);
	meshes.push_back(MyMesh4);

	lightings.push_back(lighting);
	lightings.push_back(lighting2);
	lightings.push_back(lighting3);
	lightings.push_back(lighting4);

	translates.push_back(Vec3Df(0, 0, 0));
	translates.push_back(Vec3Df(0, 0, 0));
	translates.push_back(Vec3Df(0, 0, 0));
	translates.push_back(Vec3Df(0, 0, 0));

	//computeLighting();
}

void idle()
{
	//CamPos=getCameraPosition();

	if (updateAlways)
		computeLighting();

	glutPostRedisplay();
	animate();
}

//this function loads the textures in the GPU memory
//the function is called once when the program starts
void initTexture()
{
	//Mmm... you understood the mechanism of moving the checkerboard, but can you produce something else?
	//1) load more textures using the file "brick.ppm" and put it in Texture[1], then "sand.ppm" and put it in Texture[2]
	//	btw. the wise Imp named Gerold, or short "Gimp" (it is actually a software), knows how to write PPM files (P6 - colored images)
	//  Hence, you can convert any image into a file that you can then use as a texture, but first stick to the given ones...
	//3) To make use of these textures, you first need a magic wand from the village (function) display().
	//	But be careful my young apprentice, you will need to traverse a desert...

	Texture.resize(3);
	Texture[0] = 0;
	Texture[1] = 0;
	Texture[2] = 0;

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
}



/************************************************************
* Programme principal
************************************************************/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	background();
	init();
	computeLighting();

	// layers of the framebuffer used by the application
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// position and size of the window
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(W_fen, H_fen);
	glutCreateWindow(argv[0]);

	// Initializing the viewpoint
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//////////////////////////////////////////////////////////////
	// Initial position
	glTranslatef(-1, -1, -5);
	glRotatef(10, 1, 0, 0);
	tbInitTransform();     // initialization viewpoint
	tbHelp();                      // displays help on traqueboule

	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	// wiring of callback
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseFunc(tbMouseFunc);    // traqueboule uses the mouse
	glutMotionFunc(tbMotionFunc);  // traqueboule uses the mouse
	glutIdleFunc(idle);


	// Details on how to stage fright?
	glEnable(GL_DEPTH_TEST);            // perform depth testing
	glShadeModel(GL_SMOOTH);

	// Clear all
	glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // the color and the z

	initTexture();

	// start of the main loop
	glutMainLoop();

	return 0;  // instruction jamais exécutée
}


/************************************************************
* Management functions opengl? Do not touch
************************************************************/
// Display actions 
// Do not change
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // la couleur et le z

	glLoadIdentity();  // repere camera

	tbVisuTransform(); // origine et orientation de la scene

	dessinerBackground();

	dessinerOther();

	glutSwapBuffers();
}

// to change the size or desiconification
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glTranslatef(0, 0, 3);
	gluPerspective(50, (float)w / h, 1, 10);
	glMatrixMode(GL_MODELVIEW);
}