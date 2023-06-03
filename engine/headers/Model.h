#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "../../common/headers/Shape.h"
#include "Color.h"
#include "TextureLoader.h"
#include "VBOManager.h"
#include <string>

using namespace std;

class Model
{
    private:
        Shape *shape;
        Color color;
        string textureFile;
        TextureLoader *textureLoader;
        VBOManager *vboManager;
    public:
        Model(Shape *shape): shape(shape), color(), textureFile("") {textureLoader=NULL; vboManager=NULL;}
        Model(Shape *shape, string textureFile): shape(shape), color(), textureFile(textureFile) {textureLoader=NULL;vboManager=NULL;}
        Model(Shape *shape, Color color, string textureFile): shape(shape), color(color), textureFile(textureFile) {textureLoader = NULL;vboManager=NULL;}
        Model(Shape *shape, Color color, string textureFile, TextureLoader *textureLoader, VBOManager *vboManager)
            :shape(shape), color(color), textureFile(textureFile)
            {this->textureLoader=textureLoader;this->vboManager=vboManager;}
        ~Model()
        {
            delete shape;
        }

        void generateVBOs();
        void drawPicking(vector<Shape*> codes);
        void draw();

        Shape *getShape();
        Color getColor();
        string getTextureFile();
        TextureLoader *getTextureLoader();
        VBOManager *getVBOManager();

        void setShape(Shape *shape);
        void setColor(Color color);
        void setTextureFile(string textureFile);
        void setTextureLoader(TextureLoader *textureLoader);
        void setVBOManager(VBOManager *vboManager);
};
