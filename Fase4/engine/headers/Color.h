#pragma once

#include "../../common/headers/Point.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

static bool initialized = false;

static void initializeLighing();

class Color
{
    private:
        Point diffuse, ambient, specular, emissive;
        int shininess;

    public:
        Color(): diffuse(200.0f/255.0f,200.0f/255.0f,200.0f/255.0f), ambient(50.0f/255.0f,50.0f/255.0f,50.0f/255.0f), specular(0,0,0), emissive(0,0,0), shininess(0.0f){};
        Color(Point d, Point a, Point s, Point e, float sh): diffuse(d), ambient(a), specular(s), emissive(e), shininess((sh<0) ?0 :(sh>128) ?128 :sh) {};
        Color(Color& color): diffuse(color.diffuse), ambient(color.ambient), specular(color.specular), emissive(color.emissive), shininess(color.shininess) {};

        void apply();
        
        void setDiffuse(Point diffuse);
        void setAmbient(Point ambient);
        void setSpecular(Point specular);
        void setEmissive(Point emissive);
        void setShininess(float shininess);
};
