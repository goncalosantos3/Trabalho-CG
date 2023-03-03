#pragma once

#include <fstream>
#include <vector>
#include <utility>

#include "Camera.h"
#include "Shape.h"
#include "Point.h"
#include "rapidxml.hpp"

class Parser
{
private:
    std::pair<float, float> window;
    Camera* camera;
    std::vector<Shape*> models;
    // static string filesDirectory;

public:
    Parser():window(500,500), camera(new Camera()), models(){};
    ~Parser(){};

    int parseWindow(rapidxml::xml_node<>* windowNode);
    int parseCamera(rapidxml::xml_node<>* cameraNode);
    int parseGroup(rapidxml::xml_node<>* groupNode);
    int parseXML(char *filePath);

    int parseModel(char *filePath);

    std::pair<float, float> getWindow();
    Camera* getCamera();
    std::vector<Shape*> getModels();
};