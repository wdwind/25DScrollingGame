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

using namespace std;
void computeLighting();
void dealWithUserInput(int x, int y);
Mesh MyMesh, MyMesh2;
unsigned int W_fen = 800;  // largeur fenetre
unsigned int H_fen = 800;  // hauteur fenetre
void drawCar();

std::vector<GLuint> Texture;

//________________________________
//________________________________
//________________________________
//________________________________
//________________________________
//Start reading here!!!

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
Vec3Df CamPos = Vec3Df(0.0f,0.0f,-4.0f);


//Pressing 's' will display the currently chosen vertex
bool ShowSelectedVertex=false;

//The index of the chosen vertex is stored in this variable
//the value is -1 if no vertex is chosen
//to choose a vertex, hover over it and press space
int SelectedVertex=-1;

//per vertex attributes, useful for materials - see later exercises
std::vector<Vec3Df> MeshMaterial;

float lightPower = 5;
float s = 5;
float showDot = true;


int NbVertX = 10;
int NbVertY = 4;
float qurdSize = 1;

float Vx1 = -2;
float Vx2 = (NbVertX - 1)*qurdSize + Vx1;
float threshold = -13;

int showText = 2;
int temp = showText;
bool drawN = false;
bool drawC = false;
float backgroundSpeed = 0.005;

bool shot = false;
bool temp1 = false;
bool temp2 = false;

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



//User interaction - when the user chooses a vertex, you receive its position, normal, its index 
//you can use it to NOW modify all global variables, such as the light position, or change material properties etc.
void userInteraction(const Vec3Df & selectedPos, const Vec3Df & selectedNormal, int selectedIndex)
{
}

/**
* Animation
*/
void animate()
{
	Vx1 -= backgroundSpeed;
	Vx2 -= backgroundSpeed;

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
}


// prise en compte du clavier
//Vous allez ajouter quelques fonctionalites pendant le TP
//ce qui est important pour vous: key contient le caract�re correspondant ?la touche appuy?par l'utilisateur

void keyboard(unsigned char key, int x, int y)
{
    printf("key %d pressed at %d,%d\n",key,x,y);
    fflush(stdout);
    if (key>'0'&& key<='7')
	{
		mode=Mode(key-'1');
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
	break;
	case 'B':
	break;

	
	//less important
	case 'l':
		{
			LightPos[SelectedLight]=getCameraPosition();
			return;
		}
	case 'L':
		{
			LightPos.push_back(getCameraPosition());
			LightColor.push_back(Vec3Df(1,1,1));
			return;
		}
	case '+':
		{
			++SelectedLight;
			if (SelectedLight>=LightPos.size())
				SelectedLight=0;
			return;
		}
	case '-':
		{
			--SelectedLight;
			if (SelectedLight<0)
				SelectedLight=LightPos.size()-1;
			return;
		}
	case 'U':
		{
			updateAlways=!updateAlways;
			return;
		}

	case 'N':
		{	//reset all lights
			LightPos.resize(1);
			LightPos[0]=Vec3Df(0,0,3);
			LightColor.resize(1);
			LightColor[0]=Vec3Df(1,1,1);
			SelectedLight=0;
		}

	case 'u':
		{
			//update lighing (only useful for slow computers)
			computeLighting();
			return;
		}
	case 's':
	{
		ShowSelectedVertex=!ShowSelectedVertex;
	}
	case ' ':
		{
			//You do not need to look at the function below
			//it does some computations then calls dealWithUserInput
			dealWithUserInput(x,y);
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
	}

}

//Everything below, you do NOT have to read!!!
//________________________________
//________________________________
//________________________________
//________________________________
//________________________________


//Lighting of the model (you should not need to touch this one...
std::vector<Vec3Df> lighting;
std::vector<Vec3Df> lighting2;

/************************************************************
 * Fonction pour initialiser le maillage
 ************************************************************/
void init(const char * fileName){

	//this function loads a mesh
    MyMesh.loadMesh(NbVertX, NbVertY, qurdSize);
	lighting.resize(MyMesh.vertices.size());

	MyMesh2.loadMesh(NbVertX, NbVertY, qurdSize);
	lighting2.resize(MyMesh2.vertices.size());
	
	MeshMaterial.resize(MyMesh.vertices.size());
	for (int i=0; i<MyMesh.vertices.size();++i)
		MeshMaterial[i]=Vec3Df(0,0,0);
		
	LightPos.push_back(Vec3Df(0,0,3));
	LightColor.push_back(Vec3Df(1,1,1));
	computeLighting();
}



/************************************************************
 * Appel des diff�rentes fonctions de dessin
************************************************************/


void dealWithUserInput(int x, int y)
{
	Vec3Df worldPoint=getWorldPositionOfPixel(x, H_fen-y);
	SelectedVertex=MyMesh.getClosestVertexIndex(CamPos, worldPoint-CamPos);
	if (SelectedVertex>=0)
	{
		Vec3Df selectedPos=MyMesh.vertices[SelectedVertex].p;
		Vec3Df selectedNormal=MyMesh.vertices[SelectedVertex].n;
		userInteraction(selectedPos, selectedNormal, SelectedVertex);				
	}
}

void dessiner( )
{

	glPointSize(10);
	glBegin(GL_POINTS);
	//LightColor
	glColor3f(1,0,0);	
	
	for (int i=0; i<LightPos.size();++i)	
	{	
		glVertex3f(LightPos[i][0],LightPos[i][1],LightPos[i][2]);
	}
	glEnd();
	
	glPointSize(40);
	glColor3f(1,1,0);	
	glBegin(GL_POINTS);
	glVertex3f(LightPos[SelectedLight][0],LightPos[SelectedLight][1],LightPos[SelectedLight][2]);
	glEnd();

	switch( mode )
    {
    case ORIGINAL_LIGHTING:
		{
			Vec3Df p;
			if (ShowSelectedVertex&&SelectedVertex>=0)
			{
				p=MyMesh.vertices[SelectedVertex].p;
				glBegin(GL_POINTS);
				glVertex3f(p[0],p[1],p[2]);
				glEnd();
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture[showText]);

			MyMesh.drawWithColors(lighting);

			glTranslatef(3,0,0);
			MyMesh2.drawWithColors(lighting2);

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
		MyMesh.drawWithColors(lighting);

		if (drawN)
		{
			MyMesh.drawNormals();
		}

		glPopMatrix();

		glPushMatrix();
		glTranslatef(Vx2, 0, 0);
		MyMesh2.drawWithColors(lighting2);
		glPopMatrix();


		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

		if (drawC)
			drawCar();

		if (shot){
				if (PositionBullet[0] < 6){
					PositionBullet[0] += 0.3;
					drawBullet();
				}
				else if (PositionBullet[0] >= 6){
					PositionBullet[0] = 0.03;
					PositionBullet[0] += 0.3;
					drawBullet();
				}

			//shot = false;
		}
			
		break;
    }
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


void idle()
{
	CamPos=getCameraPosition();

	if (updateAlways)
		computeLighting();

	glutPostRedisplay();
	animate();
}

void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);


void computeLighting()
{
	std::vector<Vec3Df> *result=&lighting;


	for (unsigned int i=0; i<MyMesh.vertices.size();++i)
	{
		(*result)[i]=Vec3Df();
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
}



/************************************************************
 * Programme principal
 ************************************************************/
int main(int argc, char** argv)
{
    glutInit (&argc, argv);

    init(argc == 2 ? argv[1] : "DavidHeadCleanMax.obj");

    // couches du framebuffer utilisees par l'application
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // position et taille de la fenetre
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(W_fen,H_fen);
    glutCreateWindow(argv[0]);	

    // Initialisation du point de vue
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);
    tbInitTransform();     // initialisation du point de vue
    tbHelp();                      // affiche l'aide sur la traqueboule

    glDisable( GL_LIGHTING );
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    
    // cablage des callback
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(tbMouseFunc);    // traqueboule utilise la souris
    glutMotionFunc(tbMotionFunc);  // traqueboule utilise la souris
    glutIdleFunc(idle);


    // Details sur le mode de trac?
    glEnable( GL_DEPTH_TEST );            // effectuer le test de profondeur
    glShadeModel(GL_SMOOTH);

    // Effacer tout
    glClearColor (BackgroundColor[0],BackgroundColor[1], BackgroundColor[2], 0.0);
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); // la couleur et le z
   
	initTexture();

    // lancement de la boucle principale
    glutMainLoop();

    return 0;  // instruction jamais ex�cut�e
}


/************************************************************
 * Fonctions de gestion opengl ?ne pas toucher
 ************************************************************/
// Actions d'affichage
// Ne pas changer
void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); // la couleur et le z

	glLoadIdentity();  // repere camera

    tbVisuTransform(); // origine et orientation de la scene

    dessiner( );    

    glutSwapBuffers();
}

// pour changement de taille ou desiconification
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50, (float)w/h, 1, 10);
    glMatrixMode(GL_MODELVIEW);
}


///////////////////////////////////////////////////////////////////////////////////////////

