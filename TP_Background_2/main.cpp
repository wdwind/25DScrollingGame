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
#include "enemy.h"
#include "bullet.h"
#include "light.h"
#include "texture.h"

using namespace std;

// Functions
void computeLighting();
void keyboard(unsigned char key, int x, int y);
void display(void);
void reshape(int w, int h);

void computeLighting()
{
	for (int j = 0; j < meshes.size(); j++)
	{
		std::vector<Vec3Df> *result = &(meshes[j].lighting);

		for (unsigned int i = 0; i<meshes[j].vertices.size(); ++i)
		{
			(*result)[i] = Vec3Df();
			for (int l = 0; l < LightPos.size(); ++l)
				(*result)[i] += Mesh::computeLighting(meshes[j].vertices[i].p + meshes[j].translate, meshes[j].vertices[i].n, light, CamPos, mode);
		}
	}

	for (int j = 0; j < enemies.size(); j++)
	{
		std::vector<Vec3Df> *result = &(enemies[j].lighting);

		for (unsigned int i = 0; i<enemies[j].vertices.size(); ++i)
		{
			(*result)[i] = Vec3Df();
			for (int l = 0; l < LightPos.size(); ++l)
				(*result)[i] += Mesh::computeLighting(enemies[j].vertices[i].p + enemies[j].translate, enemies[j].vertices[i].n, light, CamPos, mode);
		}
	}
}


//void collisionDetect(Vec3Df start){
//	glBegin(GL_LINES);
//	glPushMatrix();
//	glColor3f(1, 0, 0);
//	glVertex3f(0, 0, 0);
//	glVertex3f(0.5 + car.PositionBullet[0], 0.5 + car.PositionBullet[1], 0.5 + car.PositionBullet[2]);
//	glPopMatrix();
//	glEnd();
//
//	int t;
//	for (int i = 0; i < meshes[enemyIndex].triangles.size(); i++)
//	{
//		Vec3Df* vv;
//		Vec3Df t1 = meshes[enemyIndex].vertices[meshes[enemyIndex].triangles[i].v[0]].p;
//		Vec3Df t2 = meshes[enemyIndex].vertices[meshes[enemyIndex].triangles[i].v[1]].p;
//		Vec3Df t3 = meshes[enemyIndex].vertices[meshes[enemyIndex].triangles[i].v[2]].p;
//		RealTriangle rt = RealTriangle(t1, t2, t3);
//		t = intersect3D_RayTriangle(Vec3Df(car.PositionBullet[0], car.PositionBullet[1], car.PositionBullet[2]), Vec3Df(car.PositionBullet[0] + 10, car.PositionBullet[1], car.PositionBullet[2]), rt);
//
//		if (t==1)
//			cout << t << endl;
//	}
//}

void collisionDetect2(){
	//for (int i = 0; i < car.bullets.size(); i++)
	//{
	//	for (int j = 0; j < enemies.size(); j++)
	//	{
	//		Vec3Df bPos = car.bullets[i].getCurrentPos();
	//		Vec3Df ePos = enemies[j].getCurrentPos();

	//		if ((bPos - ePos).getLength() <= enemies[j].maxDist)
	//		{
	//			car.bullets.erase(car.bullets.begin() + i);
	//			//enemies.erase(enemies.begin() + j);
	//			if (!enemies[j].Shot())
	//			{
	//				enemies.erase(enemies.begin() + j);
	//				destroyedE++;
	//			}
	//		}
	//	}
	//}
	
	for (vector<Enemy>::iterator it2 = enemies.begin(); it2 != enemies.end();)
	{
		if (it2->disappear)
		{
			it2 = enemies.erase(it2);
			destroyedE++;
		}
		else
			++it2;
	}

	for (vector<Bullet>::iterator it1 = car.bullets.begin(); it1 != car.bullets.end();)
	{
		bool match = false;
		for (vector<Enemy>::iterator it2 = enemies.begin(); it2 != enemies.end();)
		{
			Vec3Df bPos = it1->getCurrentPos();
			Vec3Df ePos = it2->getCurrentPos();

			if (!it2->Explode && (bPos - ePos).getLength() < it2->maxDist)
			{
				match = true;
				// it1 = car.bullets.erase(it1);
				//enemies.erase(enemies.begin() + j);
				if (!it2->Shot())
				{
					it2->explode();
					//it2 = enemies.erase(it2);
					//destroyedE++;
				}
				/*if (it2->disappear)
				{
					it2 = enemies.erase(it2);
					destroyedE++;
				}*/
				break;
			}
			else
			{
				++it2;
			}
		}
		if (match)
		{
			it1 = car.bullets.erase(it1);
		}
		else
		{
			++it1;
		}
	}
}

/**
* Animation
*/
void animate()
{
	Vx1 -= backgroundSpeed;
	Vx2 -= backgroundSpeed;

	//if (carMove){
	//	car.rBall += car.incrementOfrball;
	//}

	car.rBall += car.incrementOfrball;

	/*if (temp1) {
		int t = 1;
		while (t < 100) {
			t += 1;
			car.angleUpper += .001;
			car.angleFore += 0.05;
			car.angleHand -= .005;
		}
	}

	if (temp2) {
		glPushMatrix();
		glTranslatef(0, 0, 1.5);
		car.angleFore += car.incrementAngle;
		if (car.angleFore >= 100 || car.angleFore <= 30){
			car.incrementAngle = -car.incrementAngle;
		}
		glPopMatrix();
	}*/

	//collisionDetect();
}


/************************************************************
 * Function to initialize the mesh
 ************************************************************/
void init(){

	//this function loads a mesh
	Enemy enemy;
	enemy.loadMesh("enemy.obj");
	enemy.translate = enemyStartPos;
	enemies.push_back(enemy);

	Enemy enemy2;
	enemy2.loadMesh("enemy.obj");
	enemy2.translate = enemyStartPos2;
	enemies.push_back(enemy2);

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	//MeshMaterial.resize(MyMesh.vertices.size());
	//for (int i=0; i<MyMesh.vertices.size();++i)
	//	MeshMaterial[i]=Vec3Df(0,0,0);
		
	LightPos.push_back(defaultLightPos);
	LightColor.push_back(defaultLightColor);

	light = Light(defaultLightPos, defaultLightColor, lightPower, s);
	//computeLighting();
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

	MyMesh3.loadRoad(NbVertX, 2, qurdSize, 4);
	lighting3.resize(MyMesh3.vertices.size());

	Mesh MyMesh4;
	std::vector<Vec3Df> lighting4;

	MyMesh4.loadRoad(NbVertX, 2, qurdSize, 4);
	lighting4.resize(MyMesh4.vertices.size());

	meshes.push_back(MyMesh);
	meshes.push_back(MyMesh2);
	meshes.push_back(MyMesh3);
	meshes.push_back(MyMesh4);

	meshes[0].lighting = lighting;
	meshes[1].lighting = lighting2;
	meshes[2].lighting = lighting3;
	meshes[3].lighting = lighting4;
}

void dessinerBackground();
void dessinerOther();
void drawBoss();

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

	if (boss)
	{
		drawBoss();
	}

	glutSwapBuffers();
}

/************************************************************
 * Call different drawing functions
************************************************************/
void dessinerBackground()
{
	glPointSize(10);

	// Draw SUN
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
		glColor3f(1, 0, 0);
		glTranslatef(LightPos[0][0], LightPos[0][1], LightPos[0][2]);
		gluSphere(sphere, .5, 50, 50);
	glBindTexture(GL_TEXTURE_2D, 2);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	

	if (boss)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[1]);
		gluSphere(sphere, 5.0, 20, 20);
		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
		glColor3f(1, 1, 1);
		//gluSphere(sphere, .5, 20, 20);
	glBindTexture(GL_TEXTURE_2D, 2);
	glDisable(GL_TEXTURE_2D);

	// Draw distant view
	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[11]);

		glColor3f(1, 1, 1);
		glTranslatef(-5, 0, -5);
		glNormal3f(0, 0, 1);
		//glScalef(40, 40, 40);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-5, 0, 0);
			glTexCoord2f(1, 0);
			glVertex3f(15, 0, 0);
			glTexCoord2f(1, 1);
			glVertex3f(15, 6, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-5, 6, 0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();


	// Draw front view
	//glPushMatrix();

	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, Texture[showText]);

	//	glColor3f(1, 1, 1);
	//	glTranslatef(-5, 0, 2);
	//	glNormal3f(0, 0, 1);
	//	//glScalef(40, 40, 40);

	//	glBegin(GL_QUADS);
	//	glTexCoord2f(-5, 0);
	//	glVertex3f(-5, 0, 0);
	//	glTexCoord2f(-5, -1);
	//	glVertex3f(-5, -1, 0);
	//	glTexCoord2f(15, -1);
	//	glVertex3f(15, -1, 0);
	//	glTexCoord2f(15, 0);
	//	glVertex3f(15, 0, 0);
	//	glEnd();

	//	glBindTexture(GL_TEXTURE_2D, 2);
	//	glDisable(GL_TEXTURE_2D);

	//glPopMatrix();

	switch( mode )
    {
    case ORIGINAL_LIGHTING:
		{
			Vec3Df p;
			//if (ShowSelectedVertex&&SelectedVertex>=0)
			//{
			//	p=meshes[0].vertices[SelectedVertex].p;
			//	glBegin(GL_POINTS);
			//	glVertex3f(p[0],p[1],p[2]);
			//	glEnd();
			//}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture[showText]);

			meshes[0].drawWithColors();

			glTranslatef(3,0,0);
			meshes[1].drawWithColors();

			meshes[2].drawWithColors();

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
			//meshes[0].loadMesh(NbVertX, NbVertY, qurdSize);
			Mesh temp;
			temp.loadMesh(NbVertX, NbVertY, qurdSize);
			meshes[0] = temp;
		}

		if (Vx2 < threshold)
		{
			Vx2 += 2 * (NbVertX - 1) * qurdSize;
		}

		glPushMatrix();
			glTranslatef(Vx1, 0, -2);
			meshes[0].drawWithColors();
			meshes[0].translate = Vec3Df(Vx1, 0, -2);
			//translates[0] = Vec3Df(Vx1, 0, -2);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(Vx2, 0, -2);
			meshes[1].drawWithColors();
			meshes[1].translate = Vec3Df(Vx2, 0, -2);
			//translates[1] = Vec3Df(Vx2, 0, -2);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);


		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[8]);

		glPushMatrix();
			glTranslatef(Vx1, 0, -2);
			meshes[2].draw();
			meshes[2].drawNormals();
			meshes[2].translate = Vec3Df(Vx1, 0, -2);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[8]);

		glPushMatrix();
			glTranslatef(Vx2, 0, -2);
			meshes[3].draw();
			meshes[3].drawNormals();
			meshes[3].translate = Vec3Df(Vx2, 0, -2);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

		break;
    }
}

void dessinerOther(){

	for (int i = 0; i < enemies.size(); i++)
	{
		//enemies[i].move();
		enemies[i].drawCurrentPos();
		glPushMatrix();
			glTranslatef(enemies[i].translate[0], enemies[i].translate[1], enemies[i].translate[2]);
			enemies[i].drawWithColors();
		glPopMatrix();
	}
	
	if (drawN)
	{
		enemies[0].drawNormals();
		enemies[0].drawSomeP();
	}

	//b.drawCurrentPos();

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, Texture[4]);
	glPushMatrix();
		glTranslatef(car.translate[0], car.translate[1], car.translate[2]);

		if (drawC)
			car.drawCar();

	glPopMatrix();

	//glBindTexture(GL_TEXTURE_2D, 2);
	//glDisable(GL_TEXTURE_2D);
}

void drawBoss(){
	if (Boss.vertices.empty())
	{
		std::vector<Vec3Df> lighting;
		Boss.loadMesh("monster.obj");
		lighting.resize(Boss.vertices.size());
		Boss.lighting = lighting;
		Boss.translate = bossStartPos;
		cout << "Start loading BOSS!!!" << endl;
	}

	glPushMatrix();
		
		glTranslatef(bossStartPos[0], bossStartPos[1], bossStartPos[2]);
		glRotatef(90, 0, -1, 0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[showText]);
		Boss.draw();
		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void enemiesType();

void idle()
{
	CamPos=getCameraPosition();

	if (updateAlways)
		computeLighting();

	collisionDetect2();

	enemiesType();

	glutPostRedisplay();
	animate();
}

void enemiesType(){
	if (enemies.size() < 2 && destroyedE < 20)
	{
		Enemy enemy;
		enemy.loadMesh("enemy.obj");
		enemy.translate = enemyStartPos;
		enemies.push_back(enemy);
	}
	if (destroyedE > 3)
	{
		destroyedE = 100;
		enemies.clear();
		boss = true;
	}
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
	glutInitWindowPosition(0, 50);
	glutInitWindowSize(W_fen, H_fen);
	glutCreateWindow(argv[0]);

	// Initializing the viewpoint
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//////////////////////////////////////////////////////////////
	// Initial position
	glTranslatef(-1, -1.2, -5);
	glRotatef(4, 1, 0, 0);
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
				LightColor.push_back(defaultLightPos);
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
		showText = (++temp) % maxTex;
		break;
	case 'n':
		//cout << showText << endl;
		drawN = true;
		break;
	case 'q':
		cout << "draw car" << endl;
		drawC = true;
		break;
	case '.':
		car.canonClock();
		break;
	case ',':
		car.canonCounter();
		break;
	case 'v':
		temp1 = true;
		break;
	case 'f':
		temp2 = true;
		break;
	case 'j':
		shot = true;
		car.shotBullet();
		break;
	case 'a':
		car.translate -= Vec3Df(0.1, 0, 0);
		break;
	case 'd':
		car.translate += Vec3Df(0.1, 0, 0);
		break;
	}

}