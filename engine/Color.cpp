#include "headers/Color.h"

static void initializeLighting()
{
    glEnable(GL_LIGHTING);
    initialized = true;
}

void Color::applyColor()
{
    if (!initialized)
        initializeLighting();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.asFloatArray());
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.asFloatArray());
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular.asFloatArray());
    glMaterialfv(GL_FRONT, GL_EMISSION, ambient.asFloatArray());
    glMaterialf(GL_FRONT, GL_SPECULAR, shininess);
}
