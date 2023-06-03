#include "headers/Color.h"
#include <iostream>

static void initializeLighting()
{
    glEnable(GL_LIGHTING);
    initialized = true;
}

void Color::apply()
{
    if (!initialized)
        initializeLighting();
   
    float dif[] = {diffuse.getX(), diffuse.getY(), diffuse.getZ(), 1.0f},
            amb[] = {ambient.getX(), ambient.getY(), ambient.getZ(), 1.0f},
            spec[] = {specular.getX(), specular.getY(), specular.getZ(), 1.0f},
            emis[] = {emissive.getX(), emissive.getY(), emissive.getZ(), 1.0f};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void Color::setDiffuse(Point diffuse)
{
    this->diffuse = diffuse;
}

void Color::setAmbient(Point ambient)
{
    this->ambient = ambient;
}

void Color::setSpecular(Point specular)
{
    this->specular = specular;
}

void Color::setEmissive(Point emissive)
{
    this->emissive = emissive;
}

void Color::setShininess(float shininess)
{
    this->shininess = (shininess < 0) ?0 :(shininess>128) ?128 :shininess;
}
