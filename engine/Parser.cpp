#include "headers/Parser.h"

#include <string>
#include <regex>

int Parser::parseWorld (std::string input)
{

    return 0;
}

int Parser::parseXML(char *filePath)
{
    std::ifstream file(filePath);

    std::string text;

    while( getline(file,text) )
    {
        if (regex_match(text, std::regex("<[^<>]>")))
        {
        }
    }
}

std::pair<float, float> Parser::getWorld()
{
    return this->world;
}

Camera* Parser::getCamera()
{
    return this->camera;
}

std::vector<Forma> Parser::getModels()
{
    std::vector<Forma> dup(this->models);
    return dup;
}