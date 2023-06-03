#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include "../../common/headers/Point.h"


enum LightType {point=0, directional=1, spotlight=2};

class Light
{
    private:
        LightType type;
        Point pos, dir;
        float cutoff;
        GLenum light;
    
        void init();

    public:
        Light(): type(point), pos(0,0,0), dir(0,0,0), cutoff(0.0f) {init();};
        Light(LightType type, Point point);
        Light(LightType type, Point pos, Point dir, float cutoff): type(type), pos(pos), dir(dir), cutoff(cutoff) {init();};
        Light(LightType type, Point pos, Point dir, float cutoff, GLenum light): type(type), pos(pos), dir(dir), cutoff(cutoff), light(light) {init();};
        ~Light(){}

        void apply();

        void setType(LightType type);
        void setPos(Point pos);
        void setDir(Point dir);
        void SetCutoff(float cutoff);
        void setLight(GLenum light);

        LightType getType();
        Point getPos();
        Point getDir();
        float getCutoff();
        GLenum getLight();

};
