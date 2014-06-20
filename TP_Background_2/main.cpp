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

/************************************************************
* Fonctions compute lighting
************************************************************/
void computeLighting()
{
	for (int j = 0; j < meshes.size(); j++)
	{
		std::vector<Vec3Df> *result = &(meshes[j].lighting);

		for (unsigned int i = 0; i<meshes[j].vertices.size(); ++i)
		{
			(*result)[i] = Vec3Df(0, 0, 0);
			if (!meshes[j].shadow.empty())
			{
				if (std::find(meshes[j].shadow.begin(), meshes[j].shadow.end(), i) != meshes[j].shadow.end())
				{
					//cout << i << endl;
					continue;
				}
			}
			(*result)[i] += Mesh::computeLighting(meshes[j].vertices[i].p + meshes[j].translate, meshes[j].vertices[i].n, light, CamPos, mode);
		}
	}

	for (int j = 0; j < enemies.size(); j++)
	{
		std::vector<Vec3Df> *result = &(enemies[j].lighting);

		for (unsigned int i = 0; i<enemies[j].vertices.size(); ++i)
		{
			(*result)[i] = Vec3Df();
			(*result)[i] += Mesh::computeLighting(enemies[j].vertices[i].p + enemies[j].translate, enemies[j].vertices[i].n, light, CamPos, mode);
		}
	}

	if (boss == 1)
	{
		std::vector<Vec3Df> *result = &(Boss.lighting);

		for (unsigned int i = 0; i<Boss.vertices.size(); ++i)
		{
			(*result)[i] = Vec3Df();
			(*result)[i] += Mesh::computeLighting(Boss.vertices[i].p + Boss.translate, Boss.vertices[i].n, light, CamPos, mode);
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


/************************************************************
* Fonctions collision detection
* Used to detect the collision of the bulltes and the enemy
************************************************************/
void collisionDetect2(){
	if (!enemies.empty())
	{
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
	}
	
	if (Boss.disappear)
	{
		boss = 100;
		Boss = Enemy();
	}

	if (!enemies.empty())
	{
		for (vector<Bullet>::iterator it1 = car.bullets.begin(); it1 != car.bullets.end();)
		{
			bool match = false;
			for (vector<Enemy>::iterator it2 = enemies.begin(); it2 != enemies.end();)
			{
				Vec3Df bPos = it1->getCurrentPos();
				Vec3Df ePos = it2->getCurrentPos();

				if (!it2->Explode && (bPos - ePos).getLength() < it2->maxDist && it1->shot)
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

	for (vector<Bullet>::iterator it1 = car.bullets.begin(); it1 != car.bullets.end();)
	{
		bool match = false;

		Enemy* it2 = &Boss;

		Vec3Df bPos = it1->getCurrentPos();
		Vec3Df ePos = it2->getCurrentPos();

		if (!it2->Explode && (bPos - ePos).getLength() < it2->maxDist && it1->shot)
		{
			cout << it2->triangles.size() << endl;

			match = true;
			if (!it2->Shot())
			{
				it2->explode();
			}

			cout << "hit boss" << endl;
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

	car.rBall += car.incrementOfrball;
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

/************************************************************
* Fonctions to initialize the background
************************************************************/
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

void drawBackground();
void drawOther();
void drawBoss();
void computeShadows();

// Display actions 
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	tbVisuTransform(); 

	drawBackground();

	drawOther();

	if (debug)
	{
		glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(light.lightPos[0], light.lightPos[1], light.lightPos[2]);
		glEnd();

		if (!shadeLine.empty())
		{
			for (int i = 0; i < shadeLine.size(); i++)
			{
				glBegin(GL_LINES);
				glColor3f(1, 0, 0);
				glVertex3f(light.lightPos[0], light.lightPos[1], light.lightPos[2]);
				glVertex3f(meshes[(int)shadeLine[i][0]].vertices[(int)shadeLine[i][1]].p[0] + meshes[(int)shadeLine[i][0]].translate[0], meshes[(int)shadeLine[i][0]].vertices[(int)shadeLine[i][1]].p[1] + meshes[(int)shadeLine[i][0]].translate[1], meshes[(int)shadeLine[i][0]].vertices[(int)shadeLine[i][1]].p[2] + meshes[(int)shadeLine[i][0]].translate[2]);
				glEnd();
			}
		}
	}

	if (boss == 1)
	{
		drawBoss();
	}

	if (shading)
	{
		computeShadows();
	}

	glutSwapBuffers();
}

/************************************************************
 * Function to DRAW the background
************************************************************/
void drawBackground()
{
	glPointSize(10);

	// Draw SUN
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture[sunTex]);
		glColor3f(1, 0, 0);
		glTranslatef(LightPos[0][0], LightPos[0][1], LightPos[0][2]);
		gluSphere(sphere, .5, 50, 50);
	glBindTexture(GL_TEXTURE_2D, 2);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	

	if (boss == 1)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[bossBackgroundTex]);
		gluSphere(sphere, 5.0, 20, 20);
		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);
	}

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, Texture[0]);
	//	glColor3f(1, 1, 1);
	//	//gluSphere(sphere, .5, 20, 20);
	//glBindTexture(GL_TEXTURE_2D, 2);
	//glDisable(GL_TEXTURE_2D);

	// Draw distant view
	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[distantBackgroundTex]);

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

	switch( mode )
    {
  //  case ORIGINAL_LIGHTING:
		//{
		//	Vec3Df p;
		//	//if (ShowSelectedVertex&&SelectedVertex>=0)
		//	//{
		//	//	p=meshes[0].vertices[SelectedVertex].p;
		//	//	glBegin(GL_POINTS);
		//	//	glVertex3f(p[0],p[1],p[2]);
		//	//	glEnd();
		//	//}

		//	glEnable(GL_TEXTURE_2D);
		//	glBindTexture(GL_TEXTURE_2D, Texture[14]);

		//	meshes[0].drawWithColors();

		//	glTranslatef(3,0,0);
		//	meshes[1].drawWithColors();

		//	meshes[2].drawWithColors();

		//	glBindTexture(GL_TEXTURE_2D, 2);
		//	glDisable(GL_TEXTURE_2D);
		//}
		//break;
    default:

		// DRAW terrain
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[terrainTex]);

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


		// DRAW road
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[roadTex]);

		glPushMatrix();
			glTranslatef(Vx1, 0, -2);
			meshes[2].draw();
			if (debug)
			{
				meshes[2].drawNormals();
			}
			meshes[2].translate = Vec3Df(Vx1, 0, -2);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[roadTex]);

		glPushMatrix();
			glTranslatef(Vx2, 0, -2);
			meshes[3].draw();
			if (debug)
			{
				meshes[3].drawNormals();
			}
			meshes[3].translate = Vec3Df(Vx2, 0, -2);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

		break;
    }
}

/************************************************************
* Fonctions to DRAW other stuff (like enemy, and car)
************************************************************/
void drawOther(){

	if (!enemies.empty())
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (debug)
			{
				enemies[i].drawNormals();
				enemies[i].drawCurrentPos();
			}

			glPushMatrix();
			glTranslatef(enemies[i].translate[0], enemies[i].translate[1], enemies[i].translate[2]);

			//glEnable(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, Texture[showText]);
			enemies[i].drawWithColors();
			//glBindTexture(GL_TEXTURE_2D, 2);
			//glDisable(GL_TEXTURE_2D);

			glPopMatrix();
		}
	}
	

	if (jump){

		if (car.translate[1] > 0) {
			//cout << car.translate[1] << endl; // show the real-time height
			time1 += 0.05;
			car.translate[1] += 0.05;
			car.translate[1] += (-0.5*0.3)*(time1*time1);
		}
		else{
			car.translate[1] = 0;
			time1 = 0;
		}
	}

	glPushMatrix();
		glTranslatef(car.translate[0], car.translate[1], car.translate[2]);

		if (drawCar)
			car.drawCar();

	glPopMatrix();

}

/************************************************************
* Fonctions to DRAW boss
************************************************************/
void drawBoss(){
	if (Boss.vertices.empty() && boss == 1)
	{
		Boss = Enemy();
		std::vector<Vec3Df> lighting;
		Boss.resolution = 16;
		Boss.loadBoss("monster_2_3.obj");
		lighting.resize(Boss.vertices.size());
		Boss.lighting = lighting;
		Boss.translate = bossStartPos;
		cout << "Start loading BOSS!!!" << endl;
	}

	glPushMatrix();
		
		glTranslatef(bossStartPos[0], bossStartPos[1], bossStartPos[2]);
		//glRotatef(90, 0, -1, 0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture[bossTex]);
		Boss.drawBoss();
		glBindTexture(GL_TEXTURE_2D, 2);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}


/************************************************************
* Fonctions to compute the shadows
************************************************************/
void computeShadows()
{
	std::vector<RealTriangle> eTriangles;
	//eTriangles.resize(enemies[0].triangles.size() + enemies[1].triangles.size());
	shadeLine.clear();

	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = 0; j < enemies[i].triangles.size(); j++)
		{
			Vec3Df v1 = enemies[i].vertices[enemies[i].triangles[j].v[0]].p + enemies[i].translate + enemies[i].origin;
			Vec3Df v2 = enemies[i].vertices[enemies[i].triangles[j].v[1]].p + enemies[i].translate + enemies[i].origin;
			Vec3Df v3 = enemies[i].vertices[enemies[i].triangles[j].v[2]].p + enemies[i].translate + enemies[i].origin;
			RealTriangle r = RealTriangle();
			r.set(v1, v2, v3);
			//eTriangles[ind] = r;
			eTriangles.push_back(r);
		}
	}

	//function to compute shadows of the terrain
	for (int j = 0; j < 2; j++)
	{
		meshes[j].shadow.clear();
		for (int i = 0; i < meshes[j].vertices.size(); i++)
		{
			Vec3Df vertex = meshes[j].vertices[i].p + meshes[j].translate;
			
			for (int k = 0; k < eTriangles.size(); k++)
			{
				if (1 == intersect3D_RayTriangle(vertex, light.lightPos, eTriangles[k]))
				{
					meshes[j].lighting[i] = Vec3Df(0, 0, 0);
					meshes[j].shadow.push_back((unsigned int)i);
					shadeLine.push_back(Vec3Df(j, i, 0));
					break;
				}
			}
		}
	}
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

/************************************************************
* Fonctions to detect the type of the enemy
************************************************************/
void enemiesType(){
	if (enemies.size() < 2 && destroyedE < 20)
	{
		Enemy enemy;
		enemy.loadMesh("enemy.obj");
		if (enemies[0].translate == enemyStartPos)
		{
			enemy.translate = enemyStartPos2;
		}
		else
		{
			enemy.translate = enemyStartPos;
		}
		
		enemies.push_back(enemy);
	}
	if (destroyedE > 3 && boss == 0)
	{
		cout << "LOAD BOSS" << endl;
		destroyedE = 100;
		enemies.clear();
		boss = 1;
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
	glRotatef(5, 1, 0, 0);
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

	// Press number 1 to 5 to change the lighting mode
	// 1: no lighting mode
	// 2: diffuse lighting
	// 3: secular lighting
	// 4: combined lighting (Phong model)
	// 5: Toon lighting
	if (key>'0'&& key <= '7')
	{
		mode = Mode(key - '1');
		computeLighting();
		return;
	}

	switch (key)
	{
	//case 'r':
	//	break;
	//case 'R':
	//	break;
	//case 'g':
	//	break;
	//case 'G':
	//	break;
	//case 'b':
	//	break;
	//case 'B':
	//	break;

	// Press l to change the light position to the camera position
	case 'l':
	{
				LightPos[SelectedLight] = getCameraPosition();
				light.lightPos = LightPos[SelectedLight];
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
	case '{':
	{
				updateAlways = !updateAlways;
				return;
	}

	// Press N to reset all lights
	case 'N':
	{	
				LightPos.resize(1);
				LightPos[0] = defaultLightPos;
				LightColor.resize(1);
				LightColor[0] = defaultLightColor;
				light = Light(defaultLightPos, defaultLightColor, lightPower, s);
				SelectedLight = 0;
	}

	// Press [ to update lighing (only useful for slow computers)
	case '[':
	{
				computeLighting();
				return;
	}

	// Press x to move the terrian
	case 'x':
		Vx1 += 0.1;
		Vx2 += 0.1;
		cout << Vx1 << endl;
		break;

	// Press z to move the terrian
	case 'z':
		Vx1 -= 0.1;
		Vx2 -= 0.1;
		cout << Vx1 << endl;
		break;

	// Press c to change the texture
	// For testing
	case 'c':
		//cout << showText << endl;
		showText = (++temp) % maxTex;
		break;

	// Press b to enter the debug mode
	// For testing
	case 'b':
		debug = true;
		car.debugInCar = true;
		break;

	// Press B to quit the debug mode
	// For testing
	case 'B':
		debug = false;
		car.debugInCar = false;
		break;

	// Useless
	case 'q':
		drawCar = true;
		break;

	// Press . to rotate the cannon
	case '.':
		car.canonClock();
		break;

	// Press , to rotate the cannon
	case ',':
		car.canonCounter();
		break;

	// Press j to shoot
	case 'j':
		shot = true;
		car.shotBullet();
		break;

	// Press a to move the car
	case 'a':
		car.translate -= Vec3Df(0.1, 0, 0);
		break;

	// Press d to move the car
	case 'd':
		car.translate += Vec3Df(0.1, 0, 0);
		break;

	// Press w to jump
	case 'w':
		car.translate[1] += 1;
		jump = true;
		break;

	// Press t, y, u, i, o, p to move the light
	case 't':
		LightPos[0][0] -= 0.1;
		light = Light(LightPos[0], defaultLightColor, lightPower, s);
		break;
	case 'y':
		LightPos[0][0] += 0.1;
		light = Light(LightPos[0], defaultLightColor, lightPower, s);
		break;
	case 'u':
		LightPos[0][1] -= 0.1;
		light = Light(LightPos[0], defaultLightColor, lightPower, s);
		break;
	case 'i':
		LightPos[0][1] += 0.1;
		light = Light(LightPos[0], defaultLightColor, lightPower, s);
		break;
	case 'o':
		LightPos[0][2] -= 0.1;
		light = Light(LightPos[0], defaultLightColor, lightPower, s);
		break;
	case 'p':
		LightPos[0][2] += 0.1;
		light = Light(LightPos[0], defaultLightColor, lightPower, s);
		break;

	// Press s to compute the shadows
	case 's':
		//shading = true;
		computeShadows();
		break;
	}

}