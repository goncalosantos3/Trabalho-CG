include "headers/Light.h"


void Light::init()
{
    glEnable(GL_LIGHTING);
    glEnable(light);

    float white[4] = {1.0f, 1.0f, 1.0f, 1.0f},
          specular[4] = {1.0f, 1.0f, 1.0f, 1.0f},
          diffuse[4] = {1.0f, 1.0f ,1.0f, 0.0f},
          ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    glLightfv(light, GL_SPECULAR, specular);
    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
	glEnable(GL_RESCALE_NORMAL);
}

void Light::apply()
{
    glEnable(GL_LIGHTING);
    glEnable(light);
    if (type == point)
    {
        GLfloat position[4] = {pos.getX(), pos.getY(), pos.getZ(), 1.0f};
        glLightfv(light, GL_POSITION, position);
    }
    else if (type == directional)
    {
        GLfloat direction[4] = {dir.getX(), dir.getY(), dir.getZ(), 0.0f};
        glLightfv(light, GL_POSITION, direction);
    }
    else
    {
        GLfloat position[4] = {pos.getX(), pos.getY(), pos.getZ(), 1.0f};
        GLfloat direction[4] = {dir.getX(), dir.getY(), dir.getZ(), 0.0f};
        glLightfv(light, GL_POSITION, position);
        glLightfv(light, GL_SPOT_DIRECTION, direction);
        glLightf(light, GL_SPOT_CUTOFF, cutoff);
    }
}

Light::Light(LightType type, Point point)
{
    this->type = type;
    if (type == 0)
        pos = point;
    else
        dir = point;
    init();
}

void Light::setType(LightType type)
{
    this->type = type;
}

void Light::setPos(Point pos)
{
    this->pos = pos;
}

void Light::setDir(Point dir)
{
    this->dir = dir;
}

void Light::SetCutoff(float cutoff)
{
    this->cutoff = cutoff;
}

void Light::setLight(GLenum light)
{
    this->light = light;
}

LightType Light::getType()
{
    return type;
}

Point Light::getPos()
{
    return pos;
}

Point Light::getDir()
{
    return dir;
}

float Light::getCutoff()
{
    return cutoff;
}

GLenum Light::getLight()
{
    return light;
}
