#include "headers/Parser.h"
#include "headers/Transformation.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include<GL/gl.h>
#include <GL/glut.h>
#endif

#include <filesystem>
#include <iostream>
#include <vector>

// window and model variables
std::pair<float, float> window;
Camera* camera;
float alpha, beta, radius;
std::vector<Group*> rootGroups;
std::vector<Shape*> allModels;

// mouse and camera variables
unsigned int picked = -1;
int w,h;
int tracking = 0;
int startX = 0, startY = 0;
float sensivity = 0.05f, camSpeed = 5.0f;

// timed transformations variables
float curveTime = 0;
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
int colorMode;

void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


float getShapeColorCode(std::string name, std::string filename)
{
	for (int i=0 ; i<allModels.size() ; i++)
	{
		Shape* model = allModels.at(i);
		if (model->getName() == name && model->getFile() == filename)
			return (i+1.0f);
	}
	return 0;
}

void applyTransformations(Group* g)
{
	for (Transformation *t : g->getTransformations())
	{
		float x = t->getX(),
					y = t->getY(),
					z = t->getZ(),
					angle = t->getAngle();
		switch(t->getType())
		{
			case Translate:
				glTranslatef(x,y,z);
				break;
			case TimeTranslate:
				float pos[3], deriv[3];
				((Curve*)t)->getGlobalCatmullRomPoint(curveTime, pos, deriv);
				break;
			case Rotate:
				glRotatef(angle, x,y,z);
				break;
			case Scale:
				glScalef(x,y,z);
				break;
			case Color:
				glColor3f(x,y,z);
				break;
		}
	}
}

void drawModels(std::vector<Shape*> models, bool paint)
{
	for (Shape *s : models)
	{
		if (paint)
		{
			float code = getShapeColorCode(s->getName(), s->getFile())/255.0f;
			glColor3f(code, code, code);
		}

		int start = s->getVBOStartIndex(), count = s->getVBOStopIndex() - start;
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vertices);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, start, count);
		// glDrawArrays(GL_POINTS, start, count);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void drawGroups(std::vector<Group*> groups, bool paint)
{
	for (Group *g : groups)
	{
		glPushMatrix();
		applyTransformations(g);
		drawModels(g->getModels(), paint);
		drawGroups(g->getGroups(), paint);
		glPopMatrix();
	}
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

	drawGroups(rootGroups, true);

	GLint viewport[4];
	unsigned char res[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// ler os valores de cor do pixel na posicao x,y
	glReadPixels(x, viewport[3]-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

	// redefenir a funcao de profundidade
	glDepthFunc(GL_LESS);
	// voltar a ativar a iluminacao
	// glEnable(GL_LIGHTING);
	// voltar a ativar as texturas
	// glEnable(GL_TEXTURE_2D);
	// voltar a desenhar so as linhas dos triangulos
	glPolygonMode(GL_FRONT, GL_LINE);
	
	return res[0];
}

void renderText() {

	char str[40], str2[128];
	if (picked+1 != 0)
	{
		Shape *model = allModels.at(picked);
		if (model->getName() != "")
			snprintf(str, 40, "Picked %s!!", allModels.at(picked)->getName().c_str());
		else
			snprintf(str, 40, "Picked %s!!", allModels.at(picked)->getFile().c_str());
	}
	else
		snprintf(str, 40, "Nothing Selected!!");
	Point* pos = camera->getPosition();
	snprintf(str2, 128, "Pos: %f %f %f, CamSpeed: %f", pos->getX(), pos->getY(), pos->getZ(), camSpeed);

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

	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1000000.0f, 0.0f, 0.0f);
		glVertex3f(1000000.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -1000000.0f, 0.0f);
		glVertex3f(0.0f, 1000000.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -1000000.0f);
		glVertex3f(0.0f, 0.0f, 1000000.0f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	drawGroups(rootGroups, false);

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
		beta += 0.1f;
		if (beta > 3.1f)
			beta = 3.1f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -0.05f)
			beta = -0.05f;
		break;
	}
	camera->setAlpha(alpha);
	camera->setBeta(beta);
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
																	camPos->getY() - fb * camSpeed * cos(beta),
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
				Shape* shape = allModels.at(picked);
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
			colorMode = (colorMode + 1)%2;
			if (colorMode == 0)
				glPolygonMode(GL_FRONT, GL_LINE);
			else
				glPolygonMode(GL_FRONT, GL_FILL);
	}
	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy) 
{
	int alphaAux, betaAux;
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
				Shape* shape = allModels.at(picked);
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
			beta += (yy - startY)*sensivity;
			camera->setBeta(beta);
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy)
{
	float alphaAux, betaAux;
	float deltaX, deltaY;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking) {

		alphaAux = alpha + deltaX*sensivity;
		betaAux = beta + deltaY*sensivity;

		if (betaAux > 3.1f)
			betaAux = 3.1f;
		else if (betaAux < 0.05f)
			betaAux = 0.05f;

		rAux = radius;
	}
	Point* camPos = camera->getPosition();
	camera->setLookAt(new Point(camPos->getX() + radius*sin(alphaAux),
															camPos->getY() - radius*cos(betaAux),
															camPos->getZ() + radius*cos(alphaAux)));
	glutPostRedisplay();
}

void idle(void)
{
	glutPostRedisplay();
}

int findName (std::vector<std::string>* vec, std::string name)
{
	for (int i=0 ; i<vec->size() ; i++)
		if (vec->at(i) == name)
			return i;
	return -1;
}

void setVBOindexes(std::vector<Group*> groups, 
									 std::vector<std::string>* models,
									 std::vector<int>* vboStartIndexes, 
									 std::vector<int>* vboStopIndexes)
{
	for (Group* group : groups)
	{
		for (Shape* model : group->getModels())
		{
			int fileIdx = findName(models, model->getFile()); 
			model->setVBOStartIndex(vboStartIndexes->at(fileIdx));
			model->setVBOStopIndex(vboStopIndexes->at(fileIdx));
			// printf("%s %d %d %d\n", model->getFile().c_str(), fileIdx, model->getVBOStartIndex(), model->getVBOStopIndex());
		}
		setVBOindexes(group->getGroups(), models, vboStartIndexes, vboStopIndexes);
	}
}

void buildVBO()
{
	std::vector<std::string> setModels;
	setModels.reserve(allModels.size());
	std::vector<int> vboStartIndexes, vboStopIndexes;
	vboStartIndexes.reserve(allModels.size());
	vboStopIndexes.reserve(allModels.size());
	
	std::vector<float> points;
	points.reserve(100000);

	for (Shape* m : allModels)
	{
		int nameIdx = findName(&setModels, m->getFile());
		if (nameIdx == -1)
		{
			vboStartIndexes.push_back(points.size()/3);
			for (Point p : m->getPoints())
			{
				points.push_back(p.getX());
				points.push_back(p.getY());
				points.push_back(p.getZ());
			}
			vboStopIndexes.push_back(points.size()/3);
			setModels.push_back(m->getFile());
			nameIdx = setModels.size()-1;
		}
		m->setVBOStartIndex(vboStartIndexes.at(nameIdx));
		m->setVBOStopIndex(vboStopIndexes.at(nameIdx));
	}

	setVBOindexes(rootGroups, &setModels, &vboStartIndexes, &vboStopIndexes);

	verticeCount = points.size();
	glEnableClientState(GL_VERTEX_ARRAY);

	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*points.size(), points.data(), GL_STATIC_DRAW);

	glDisableClientState(GL_ARRAY_BUFFER);
}


int main(int argc, char **argv)
{
	if (argc < 2)
	   return 1;

	Parser parser;

	if (parser.parseXML(argv[1]))
		return 1;
	
	window = parser.getWindow();
	camera = parser.getCamera();
	rootGroups = parser.getGroups();
	allModels = parser.getModels();

	camera->calculateAngles();
	alpha = camera->getAlpha();
	beta = camera->getBeta();
	radius = camera->getRadius();

	printf("Instructions:\n\tMove with the 'W', 'A', 'S' and 'D' keys\n\tMove the camera with the arrow keys or with the mouse\n\tIncrease/Decrease camera Speed: 'Z'/'X'\n\tSelect a model by pointing and pressing wither middle mouse button or 'P'");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window.first, window.second);
	glutCreateWindow(argv[1]);
	glPolygonMode(GL_FRONT, GL_LINE);

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idle);


	timebase = glutGet(GLUT_ELAPSED_TIME);
	initialTitle = argv[1];

	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	#ifndef __APPLE__
	glewInit();
	#endif

	buildVBO();
	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	for (Group *g : rootGroups)
		delete g;
	return 0;
}
