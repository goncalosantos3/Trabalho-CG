#include "headers/Parser.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <filesystem>
#include <math.h>

std::pair<float, float> window;
Camera* camera;
float alpha, beta, radius, alphaAux, betaAux;
std::vector<Group*> rootGroups;
std::vector<Shape*> allModels;

unsigned int picked;
int w,h;
int tracking = 0;
int startX = 0, startY = 0;


int timebase;
float frames;
float timePassed;
float fps;
char *initialTitle;
char title[256];



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
			case Rotate:
				glRotatef(angle, x,y,z);
				break;
			case Scale:
				glScalef(x,y,z);
				break;
		}
	}
}

void drawModels(std::vector<Shape*> models, bool paint)
{
	glBegin(GL_TRIANGLES);
		for (Shape *s : models)
		{
			if (paint)
			{
				float code = getShapeColorCode(s->getName(), s->getFile())/255.0f;
				glColor3f(code, code, code);
			}
			std::vector<Point> points = s->getPoints();
			for (int i=0 ; i<points.size() ; i+=3)
			{
				Point p = points.at(i);
				glVertex3f(p.getX(), p.getY(), p.getZ());
				p = points.at(i+1);
				glVertex3f(p.getX(), p.getY(), p.getZ());
				p = points.at(i+2);
				glVertex3f(p.getX(), p.getY(), p.getZ());
			}
		}
	glEnd();
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
	glEnable(GL_TEXTURE_2D);
	// voltar a desenhar so as linhas dos triangulos
	glPolygonMode(GL_FRONT, GL_LINE);
	
	return res[0];
}

void renderText() {

	char str[40];
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

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	// Fazer com que as coordenadas corespondam às coordenadas no ecrã (esquerda->direita, cima->baixo)
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2d(10, 20);

	for (char *c = str ; *c ; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

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
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;

	}
	camera->setPosition(new Point(radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha)));
	glutPostRedisplay();

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
	}
	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy) 
{
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else { // Middle button
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
			alphaAux += (xx - startX);
			betaAux += (yy - startY);
		}
		else if (tracking == 2) {
			
			radius -= yy - startY;
			if (radius < 3)
				radius = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alpha = alphaAux + deltaX;
		beta = betaAux + deltaY;

		if (beta > 85.0)
			beta = 85.0;
		else if (beta < -85.0)
			beta = -85.0;

		rAux = radius;
	}
	else if (tracking == 2) {

		alpha = alphaAux;
		beta = betaAux;
		rAux = radius - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camera->setPosition(new Point(radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha)));

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

	Parser parser;

	if (parser.parseXML(argv[1]))
		return 1;
	
	window = parser.getWindow();
	camera = parser.getCamera();
	rootGroups = parser.getGroups();
	allModels = parser.getModels();

	Point *cameraPosition = camera->getPosition();

	radius = sqrt(cameraPosition->getX()*cameraPosition->getX() + cameraPosition->getY()*cameraPosition->getY() + cameraPosition->getZ()*cameraPosition->getZ());
	beta = asin(cameraPosition->getY()/radius);
	alpha = asin(cameraPosition->getX()/(radius*cos(beta)));
	

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

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	for (Group *g : rootGroups)
		delete g;
	return 0;
}
