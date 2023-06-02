#pragma once

#include "../../common/headers/Shape.h"
#include "Color.h"
#include "TextureLoader.h"
#include <string>

using namespace std;

class Model
{
    private:
        Shape *shape;
        Color color;
        string textureFile;
        TextureLoader *textureLoader;
    public:
        Model(Shape *shape, Color color, string textureFile): shape(shape), color(color), textureFile(textureFile) {textureLoader = NULL;}
};
