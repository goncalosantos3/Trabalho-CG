#pragma once

#include <map>
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

using namespace std;

class TextureLoader
{
    private:
        map<string, GLuint> textureIDs;
        const string textureFolder;
        void init();

    public:
        TextureLoader(): textureFolder("../files/"), textureIDs(){init();};
        TextureLoader(TextureLoader& tl): textureFolder(tl.textureFolder), textureIDs(tl.textureIDs){init();};
        ~TextureLoader();

        void addTexture(string textureFile);
        GLuint getTextureID(string textureFile);
};
