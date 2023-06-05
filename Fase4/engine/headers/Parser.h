#pragma once

#include <vector>
#include <fstream>
#include <utility>
#include <string.h>
#include <sys/stat.h>

#include "TextureLoader.h"
#include "VBOManager.h"
#include "Camera.h"
#include "Model.h"
#include "Group.h"
#include "../../common/headers/Shape.h"
#include "rapidxml.hpp"
#include "Curve.h"
#include "Transformation.h"
#include "Curve.h"
#include "Color.h"
#include "Light.h"

class Parser
{
private:
	std::pair<float, float> window;
	Camera* camera;
    std::vector<Light*> lights;
	std::vector<Group*> groups;
	std::vector<Shape*> allShapes;
    TextureLoader *textureLoader;
    VBOManager *vboManager;

	// static string filesDirectory;

public:
	Parser():window(500,500), camera(new Camera()), groups(), allShapes(){groups.reserve(10);allShapes.reserve(10);textureLoader=new TextureLoader();vboManager=new VBOManager();};
	// Parser():window(500,500), camera(new Camera()), groups(){groups.reserve(10);};
	~Parser();

	int parseWindow(rapidxml::xml_node<>* windowNode);
	int parseCamera(rapidxml::xml_node<>* cameraNode);
    int parseLight(Light *light, rapidxml::xml_node<>* lightNode, int lightNum);
    int parseLights(std::vector<Light*>* lights, rapidxml::xml_node<>* lightsNode);
	int parseTransformations(std::vector<Transformation*>* transformations, rapidxml::xml_node<>* transformNode);
    std::string parseTexture(rapidxml::xml_node<>* textureNode);
    int parseColor(rapidxml::xml_node<>* colorNode, class::Color *color);
	int parseModel(Model* model, std::string filePath);
	int parseModels(std::vector<Model*>* models, rapidxml::xml_node<>* groupNode);
	int parseGroup(Group* group, rapidxml::xml_node<>* groupNode);
	int parseGroups(std::vector<Group*>* groups, rapidxml::xml_node<>* node);
	int parseXML(char *filePath);

	Shape* getShapeFromShapes(std::string name, std::string file);

	std::pair<float, float> getWindow();
	Camera* getCamera();
    std::vector<Light*> getLights();
	std::vector<Group*> getGroups();
	std::vector<Shape*> getShapes();
};
