#pragma once

#include <vector>
#include <fstream>
#include <utility>
#include <string.h>

#include "Camera.h"
#include "Group.h"
#include "Shape.h"
#include "rapidxml.hpp"

class Parser
{
private:
	std::pair<float, float> window;
	Camera* camera;
	std::vector<Group*> groups;
	// static string filesDirectory;

public:
	Parser():window(500,500), camera(new Camera()), groups(){};
	~Parser(){};

	int parseWindow(rapidxml::xml_node<>* windowNode);
	int parseCamera(rapidxml::xml_node<>* cameraNode);
	int parseTransformations(std::vector<Transformation*>* transformations, rapidxml::xml_node<>* transformNode);
	int parseModels(std::vector<Shape*>* models, rapidxml::xml_node<>* groupNode);
	int parseGroup(Group* group, rapidxml::xml_node<>* groupNode);
	int parseGroups(std::vector<Group*>* groups, rapidxml::xml_node<>* node);
	int parseXML(char *filePath);

	int parseModel(Shape* shape, char *filePath);

	std::pair<float, float> getWindow();
	Camera* getCamera();
	std::vector<Group*> getGroups();
};
