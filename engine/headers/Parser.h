#pragma once

#include <fstream>
#include <vector>
#include <utility>

#include "Camera.h"
#include "Forma.h"
#include "Ponto.h"

class Parser
{
private:
    std::pair<float, float> world;
    Camera* camera;
    std::vector<Forma> models;

public:
    Parser(){};
    ~Parser(){};

    int parseWorld(std::string input);
    int parseCamera(std::string input);
    int parseGroup(std::string input);
    int parseXML(char *filePath);

    std::pair<float, float> getWorld();
    Camera* getCamera();
    std::vector<Forma> getModels();
};