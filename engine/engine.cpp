#include "headers/Parser.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <filesystem>
#include <math.h>

std::pair<float, float> window;
Camera* camera;
float alpha, beta, radius;
std::vector<Shape*> models;

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
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	for (Shape* forma : models)
	{
		std::vector<Point> pontos = forma->getPoints();
		for (int i = 0; i < pontos.size(); i += 3)
		{
			Point p1 = pontos.at(i);
			Point p2 = pontos.at(i+1);
			Point p3 = pontos.at(i+2);

			glVertex3f(p1.getX(), p1.getY(), p1.getZ());
			glVertex3f(p2.getX(), p2.getY(), p2.getZ());
			glVertex3f(p3.getX(), p3.getY(), p3.getZ());
		}
	}
	glEnd();

	glutSwapBuffers();
}

void changeSize(int w, int h)
{
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

int main(int argc, char **argv)
{
	if (argc < 2)
	   return 1;

    Parser parser;

    if (parser.parseXML(argv[1]))
		return 1;
	
	window = parser.getWindow();
	camera = parser.getCamera();
	models = parser.getModels();

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
	glutIdleFunc(renderScene);


	glutSpecialFunc(processSpecialKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

    return 0;
}
