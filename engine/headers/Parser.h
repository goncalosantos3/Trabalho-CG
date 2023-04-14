#pragma once

#include <vector>
#include <fstream>
#include <utility>
#include <string.h>

#include "Camera.h"
#include "Group.h"
#include "../../common/headers/Shape.h"
#include "rapidxml.hpp"

class Parser
{
private:
	std::pair<float, float> window;
	Camera* camera;
	std::vector<Group*> groups;
	std::vector<Shape*> allModels;
	// static string filesDirectory;

public:
	Parser():window(500,500), camera(new Camera()), groups(), allModels(){groups.reserve(10);allModels.reserve(10);};
	// Parser():window(500,500), camera(new Camera()), groups(){groups.reserve(10);};
	~Parser(){};

	int parseWindow(rapidxml::xml_node<>* windowNode);
	int parseCamera(rapidxml::xml_node<>* cameraNode);
	int parseTransformations(std::vector<Transformation*>* transformations, rapidxml::xml_node<>* transformNode);
	int parseModel(Shape* shape, std::string filePath);
	int parseModels(std::vector<Shape*>* models, rapidxml::xml_node<>* groupNode);
	int parseGroup(Group* group, rapidxml::xml_node<>* groupNode);
	int parseGroups(std::vector<Group*>* groups, rapidxml::xml_node<>* node);
	int parseXML(char *filePath);

	Shape* getModelFromModels(std::string name, std::string file);

	std::pair<float, float> getWindow();
	Camera* getCamera();
	std::vector<Group*> getGroups();
	std::vector<Shape*> getModels();
};
