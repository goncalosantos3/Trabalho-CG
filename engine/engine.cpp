#include "headers/Parser.h"
#include "headers/Transformation.h"
#include "../common/headers/MatrixOpp.h"
#include <GL/freeglut_std.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

#include <filesystem>
#include <iostream>
#include <vector>

// window and model variables
std::pair<float, float> window;
Camera* camera;
float alpha, betaAngle, radius;
std::vector<Group*> rootGroups;
std::vector<Shape*> allShapes;
std::vector<Light*> lights;

// mouse and camera variables
unsigned int picked = -1;
int w,h;
int tracking = 0;
int startX = 0, startY = 0;
float sensivity = 0.05f, camSpeed = 5.0f;

// timed transformations variables
float transfBaseTime,transfTime = 0;
int updatesPerSec = 144;

// fps count variables
int timebase;
float frames;
float timePassed;
float fps;
char *initialTitle;
char title[256];

// VBO variables
GLuint vertices;
int verticeCount;

// color mode variable
int polyMode;


float getShapeColorCode(std::string name, std::string filename)
{
	for (int i=0 ; i<allShapes.size() ; i++)
	{
		Shape* model = allShapes.at(i);
		if (model->getName() == name && model->getFile() == filename)
			return (i+1.0f);
	}
	return 0;
}

unsigned char  picking(int x, int y) {

	// se o rato estiver fora do ecra nao seleciona nada
	if (x<0 || x>w || y<0 || y>h) return 0;
	// desligar iluminacao
	glDisable(GL_LIGHTING);
	// desligar texturas
	glDisable(GL_TEXTURE_2D);
	// definir o modo de desenho como fill
	glPolygonMode(GL_FRONT, GL_FILL);

	// limpar o buffer de cor
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	Point* position = camera->getPosition(),
		* lookAt = camera->getLookAt(),
		* up = camera->getUp();
	gluLookAt(position->getX(), position->getY(), position->getZ(),
			  lookAt->getX(), lookAt->getY(), lookAt->getZ(),
			  up->getX(), up->getY(), up->getZ());


	// definir a funcao de profundidade como <=
	glDepthFunc(GL_LEQUAL);

	// definir cor a preto para desenhar tudo a preto
	glColor3f(0.0f, 0.0f, 0.0f);

    for (Group *g : rootGroups)
        g->draw(transfTime);

	GLint viewport[4];
	unsigned char res[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// ler os valores de cor do pixel na posicao x,y
	glReadPixels(x, viewport[3]-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

	// redefenir a funcao de profundidade
	glDepthFunc(GL_LESS);
	// voltar a ativar a iluminacao
	glEnable(GL_LIGHTING);
	// voltar a ativar as texturas
	glEnable(GL_TEXTURE_2D);
	// voltar a desenhar so as linhas dos triangulos
	if (!polyMode)
		glPolygonMode(GL_FRONT, GL_LINE);
	else
		glPolygonMode(GL_FRONT, GL_FILL);


	return res[0];
}

void renderText() {

    glDisable(GL_LIGHTING);
	char str[40], str2[128];
	if (picked+1 != 0)
	{
		Shape *model = allShapes.at(picked);
		if (model->getName() != "")
			snprintf(str, 40, "Picked %s!!", allShapes.at(picked)->getName().c_str());
		else
			snprintf(str, 40, "Picked %s!!", allShapes.at(picked)->getFile().c_str());
	}
	else
		snprintf(str, 40, "Nothing Selected!!");
	Point* pos = camera->getPosition(),
         * lookAt = camera->getLookAt();
	snprintf(str2, 128, "Pos: %f %f %f, CamSpeed: %f, %f %f %f", pos->getX(), pos->getY(), pos->getZ(), camSpeed, lookAt->getX(), lookAt->getY(), lookAt->getZ());

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	glColor3f(1.0f,1.0f,1.0f);
	// Fazer com que as coordenadas corespondam às coordenadas no ecrã (esquerda->direita, cima->baixo)
	gluOrtho2D(0, w, h, 0);

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2d(10, 20);

	for (char *c = str ; *c ; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	
	glRasterPos2d(10, h-20);

	for (char *c = str2; *c ; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
		

	// voltar às matrizes anteriores e voltar a ativar o teste de profundidade
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	Point* position = camera->getPosition(),
		* lookAt = camera->getLookAt(),
		* up = camera->getUp();
	gluLookAt(position->getX(), position->getY(), position->getZ(),
			  lookAt->getX(), lookAt->getY(), lookAt->getZ(),
			  up->getX(), up->getY(), up->getZ());

    glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		// X axis in red
        // float red[4] = {1.0f, 0.0f, 0.0f};
        // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1000000.0f, 0.0f, 0.0f);
		glVertex3f(1000000.0f, 0.0f, 0.0f);
		// Y Axis in Green
        // float green[4] = {0.0f, 1.0f, 0.0f};
        // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -1000000.0f, 0.0f);
		glVertex3f(0.0f, 1000000.0f, 0.0f);
		// Z Axis in Blue
        // float blue[4] = {0.0f, 0.0f, 1.0f};
        // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -1000000.0f);
		glVertex3f(0.0f, 0.0f, 1000000.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_LIGHTING);


    for (Light *l : lights)
        l->apply();

    glPushMatrix();
        for (Group *g : rootGroups)
            g->draw(transfTime);
    glPopMatrix();

	renderText();

	timePassed = glutGet(GLUT_ELAPSED_TIME);
	frames++;
	if (timePassed - timebase > 1000)
	{
		fps = frames*1000.0 / (timePassed-timebase);
		timebase = timePassed;
		frames = 0;
		snprintf(title, 256, "%s %f", initialTitle, fps);
		glutSetWindowTitle(title);
	}
	// printf("inc : %f, %f %f\n", 1.0f/updatesPerSec, timePassed - transfBaseTime, 1000.0f / updatesPerSec);
	if (timePassed - transfBaseTime >= 1000.0f / updatesPerSec)
	{
		transfTime += 1.0f/updatesPerSec;
		transfBaseTime = timePassed;
	}
	glutSwapBuffers();
}

// void proccessMouseMotion()

void changeSize(int ww, int hh)
{
	w = ww;
	h = hh;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(camera->getFov(), ratio, camera->getNear(), camera->getFar());

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alpha -= 0.1; break;

	case GLUT_KEY_LEFT:
		alpha += 0.1; break;

	case GLUT_KEY_UP:
		betaAngle += 0.1f;
		if (betaAngle > M_PI)
			betaAngle = M_PI;
		break;

	case GLUT_KEY_DOWN:
		betaAngle -= 0.1f;
		if (betaAngle < -0.0f)
			betaAngle = -0.0f;
		break;
	}
	camera->setAlpha(alpha);
	camera->setBeta(betaAngle);
	camera->updateLookAt();
	glutPostRedisplay();

}

void moveCamera(int fb, int lr)
{
	// if the movement is front or back, fb is +1 if front and -1 if back
	if (fb!=0)
	{
		Point* camPos = camera->getPosition();
		camera->setPosition(new Point(camPos->getX() + fb * camSpeed * sin(alpha),
																	camPos->getY() - fb * camSpeed * cos(betaAngle),
																	camPos->getZ() + fb * camSpeed * cos(alpha)));
		camera->updateLookAt();
		return;
	}
	// lr == +1 -> right movement, lr == -1 -> left movement
	Point* camPos = camera->getPosition();
	camera->setPosition(new Point(camPos->getX() - lr * camSpeed * cos(alpha),
																camPos->getY(),
																camPos->getZ() + lr * camSpeed * sin(alpha)));
	camera->updateLookAt();
}

void processNormalKeys(unsigned char key, int x, int y) {

	switch(key) {
	
		case 27: exit(0);
		case 'p':
			picked = picking(x,y) - 1;
			if (picked+1)
			{
				Shape* shape = allShapes.at(picked);
				if (shape->getName() != "")
					std::cout << "Selected Model with name \"" << shape->getName() << "\"!" << std::endl;
				else
					std::cout << "Selected Model with filename \"" << shape->getFile() << "\"!" << std::endl;
			}
			else
				printf("Nothing selected\n");
			break;
		case 'w':
			moveCamera(1,0);
			break;
		case 's':
			moveCamera(-1,0);
			break;
		case 'd':
			moveCamera(0,1);
			break;
		case 'a':
			moveCamera(0,-1);
			break;
		case 'z':
			camSpeed *= 2.0f;
			break;
		case 'x':
			camSpeed /= 2.0f;
			break;
		case 'c':
			polyMode = (polyMode + 1)%2;
			if (polyMode == 0)
				glPolygonMode(GL_FRONT, GL_FILL);
			else
				glPolygonMode(GL_FRONT, GL_LINE);
	}
	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy) 
{
	int alphaAux, betaAngleAux;
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_MIDDLE_BUTTON) { // Middle button
			tracking = 0;
			picked = picking(xx,yy) - 1;
			if (picked+1)
			{
				Shape* shape = allShapes.at(picked);
				if (shape->getName() != "")
					std::cout << "Selected Model with name \"" << shape->getName() << "\"!" << std::endl;
				else
					std::cout << "Selected Model with filename \"" << shape->getFile() << "\"!" << std::endl;
			}
			else
				printf("Nothing selected\n");
			glutPostRedisplay();
		}
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX)*sensivity;
			camera->setAlpha(alpha);
			betaAngle += (yy - startY)*sensivity;
			camera->setBeta(betaAngle);
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy)
{
	float alphaAux, betaAngleAux;
	float deltaX, deltaY;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking) {

		alphaAux = alpha + deltaX*sensivity;
		betaAngleAux = betaAngle+ deltaY*sensivity;

		if (betaAngleAux > M_PI)
			betaAngleAux = M_PI;
		else if (betaAngleAux < 0.0f)
			betaAngleAux = 0.0f;

		rAux = radius;
	}
	Point* camPos = camera->getPosition();
	camera->setLookAt(new Point(camPos->getX() + radius*sin(alphaAux),
															camPos->getY() - radius*cos(betaAngleAux),
															camPos->getZ() + radius*cos(alphaAux)));
	glutPostRedisplay();
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	if (argc < 2)
	   return 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window.first, window.second);
	glutCreateWindow(argv[1]);
	glPolygonMode(GL_FRONT, GL_FILL);

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idle);


	timebase = glutGet(GLUT_ELAPSED_TIME);
	transfBaseTime = timebase;
	initialTitle = argv[1];

	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	Parser parser;

	if (parser.parseXML(argv[1]))
		return 1;
	
	window = parser.getWindow();
	camera = parser.getCamera();
	rootGroups = parser.getGroups();
	allShapes = parser.getShapes();
    lights = parser.getLights();

	camera->calculateAngles();
	alpha = camera->getAlpha();
	betaAngle = camera->getBeta();
	radius = camera->getRadius();

	printf("Instructions:\n\tMove with the 'W', 'A', 'S' and 'D' keys\n\tMove the camera with the arrow keys or with the mouse\n\tIncrease/Decrease camera Speed: 'Z'/'X'\n\tSelect a model by pointing and pressing wither middle mouse button or 'P'\n");


	// #ifndef __APPLE__
	// glewInit();
	// #endif

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	for (Group *g : rootGroups)
		delete g;
	return 0;
}
