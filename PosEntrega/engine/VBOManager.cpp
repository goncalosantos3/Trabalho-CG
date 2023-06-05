#include "headers/VBOManager.h"


void VBOManager::init()
{
	#ifndef __APPLE__
	glewInit();
	#endif

    glEnableClientState(GL_ARRAY_BUFFER);
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void VBOManager::addModel(Shape *shape)
{
    if (points.count(shape->getFile()) != 0)
        return;

    int count = shape->getPoints().size();

    vector<float> points, normals, texCoords;
    points.reserve(count*3);
    normals.reserve(count*3);
    texCoords.reserve(count*2);

    for (Point p : shape->getPoints())
    {
        points.push_back(p.getX());
        points.push_back(p.getY());
        points.push_back(p.getZ());
    }

    for (Point p : shape->getNormals())
    {
        normals.push_back(p.getX());
        normals.push_back(p.getY());
        normals.push_back(p.getZ());
    }

    for (Point2D p : shape->getTextCoords())
    {
        texCoords.push_back(p.getX());
        texCoords.push_back(p.getY());
    }

    GLuint pointsVBO, normalsVBO, texCoordsVBO;

    // criar o VBO
    glGenBuffers(1, &pointsVBO);
    glGenBuffers(1, &normalsVBO);
    glGenBuffers(1, &texCoordsVBO);

    // bind ao VBO, alocar memoria e preencher
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // guardar no mapa
    this->points[shape->getFile()] = pointsVBO;
    this->normals[shape->getFile()] = normalsVBO;
    this->tex[shape->getFile()] = texCoordsVBO;

}

GLuint VBOManager::getPointsVBOID(string shapeFile)
{
    return points[shapeFile];
}

GLuint VBOManager::getNormalsVBOID(string shapeFile)
{
    return normals[shapeFile];
}

GLuint VBOManager::getTexCoordsVBOID(string shapeFile)
{
    return tex[shapeFile];
}
