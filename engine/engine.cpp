#include "headers/Parser.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

std::pair<float, float> window;
Camera* camera;
std::vector<Forma*> models;

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	Ponto* position = camera->getPosition(),
		* lookAt = camera->getLookAt(),
		* up = camera->getUp();
	gluLookAt(position->getX(), position->getY(), position->getZ(),
			  lookAt->getX(), lookAt->getY(), lookAt->getZ(),
			  up->getX(), up->getY(), up->getZ());

	glBegin(GL_TRIANGLES);
	for (Forma* forma : models)
	{
		std::vector<Ponto> pontos = forma->getPontos();
		for (int i = 0; i < pontos.size(); i += 3)
		{
			Ponto p1 = pontos.at(i);
			Ponto p2 = pontos.at(i+1);
			Ponto p3 = pontos.at(i+2);

			glVertex3f(p1.getX(), p1.getY(), p1.getZ());
			glVertex3f(p2.getX(), p2.getY(), p2.getZ());
			glVertex3f(p3.getX(), p3.getY(), p3.getZ());
		}
	}
	glEnd();
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

int main(int argc, char **argv)
{
    //if (argc < 2)
      //  return 1;

    Parser parser;

    parser.parseXML("../files/exemplo.xml");
	

	window = parser.getWindow();
	camera = parser.getCamera();
	models = parser.getModels();

		
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window.first, window.second);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard callbacks
	//glutKeyboardFunc(mover);
	//glutSpecialFunc(transformar);


	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

    return 0;
}