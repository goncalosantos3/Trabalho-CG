#include "headers/Model.h"

void Model::generateVBOs()
{
    if (textureLoader && textureFile != "")
        textureLoader->addTexture(textureFile);
    if (vboManager && shape)
        vboManager->addModel(shape);
}

void Model::draw()
{
    string shapeFile = shape->getFile();

    // bind dos pontos
    glBindBuffer(GL_ARRAY_BUFFER, vboManager->getPointsVBOID(shapeFile));
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // bind das normais
    glBindBuffer(GL_ARRAY_BUFFER, vboManager->getNormalsVBOID(shapeFile));
    glNormalPointer(GL_FLOAT, 0, 0);

    // bind das coordenadas de textura
    glBindBuffer(GL_ARRAY_BUFFER, vboManager->getTexCoordsVBOID(shapeFile));
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    // se houver textura, dar bind a ela
    if (textureFile != "")
    {
        int texID = textureLoader->getTextureID(textureFile);
        glBindTexture(GL_TEXTURE_2D, texID);
    }
    else
        color.apply();

    // desenhar os triangulos
    glDrawArrays(GL_TRIANGLES, 0, shape->getPoints().size());

    if (textureFile != "")
        glBindTexture(GL_TEXTURE_2D, 0);
}

Shape *Model::getShape() 
{
    return shape;
}

Color Model::getColor() 
{
    return color;
}

string Model::getTextureFile() 
{
    return textureFile;
}

TextureLoader *Model::getTextureLoader() 
{
    return textureLoader;
}

VBOManager *Model::getVBOManager() 
{
    return vboManager;
}

void Model::setShape(Shape *shape)
{
    this->shape = shape;
}

void Model::setColor(Color color)
{
    this->color = color;
}

void Model::setTextureFile(string textureFile)
{
    this->textureFile = textureFile;
}

void Model::setTextureLoader(TextureLoader *textureLoader)
{
    this->textureLoader = textureLoader;
}

void Model::setVBOManager(VBOManager *vboManager)
{
    this->vboManager = vboManager;
}
