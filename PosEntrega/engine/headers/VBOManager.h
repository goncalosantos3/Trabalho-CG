#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "../../common/headers/Shape.h"
#include <map>
#include <string>

using namespace std;

class VBOManager
{
    private:
        map<string, GLuint> points, normals, tex;
        void init();

    public:
        VBOManager(): points(), normals(), tex() {init();};
        VBOManager(VBOManager& vbom): points(vbom.points), normals(vbom.normals), tex(vbom.tex) {init();};
        ~VBOManager(){};

        void addModel(Shape *shape);
        GLuint getPointsVBOID(string shapeFile);
        GLuint getNormalsVBOID(string shapeFile);
        GLuint getTexCoordsVBOID(string shapeFile);
};
