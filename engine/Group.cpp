#include "headers/Group.h"

Group::Group()
{
	this->transformations = std::vector<Transformation*>();
	this->models = std::vector<Model*>();
	this->groups = std::vector<Group*>();
	transformations.reserve(3);
	models.reserve(5);
	groups.reserve(5);
	oldYaxis[0] = 0.0f;
	oldYaxis[1] = 1.0f;
	oldYaxis[2] = 0.0f;
}

Group::Group(const Group& group)
{
	transformations.reserve(group.transformations.size());
	for (auto& t: group.transformations)
		transformations.push_back(t);
	models.reserve(group.models.size());
	for (auto& m: group.models)
		models.push_back(m);
	groups.reserve(group.groups.size());
	for (auto& g: group.groups)
		groups.push_back(g);
	oldYaxis[0] = 0.0f;
	oldYaxis[1] = 1.0f;
	oldYaxis[2] = 0.0f;
}

Group::~Group()
{
	for (Transformation *t : this->transformations)
		delete t;
	this->transformations.clear();
	this->models.clear();
	for (Group *g : this->groups)
		delete g;
	this->groups.clear();
}

void Group::addTransformation(Transformation* t)
{
	this->transformations.push_back(t);
}

void Group::addTransformations(std::vector<Transformation*>& t)
{
	for (Transformation *tr : t)
		this->addTransformation(tr);
}

void Group::addModel(Model* m)
{
	this->models.push_back(m);
}

void Group::addModels(std::vector<Model*>& m)
{
	for (Model *model : m)
		this->addModel(model);
}

void Group::addGroup(Group* g)
{
	this->groups.push_back(g);
}

void Group::addGroups(std::vector<Group*>& g)
{
	for (Group *gr : g)
		this->addGroup(gr);
}

void Group::setOldYAxis(float *newOldYAxis)
{
	oldYaxis[0] = newOldYAxis[0];
	oldYaxis[1] = newOldYAxis[1];
	oldYaxis[2] = newOldYAxis[2];
}

void Group::generateVBOs()
{
    for (Model* model : models)
        model->generateVBOs();
}

void renderCatmullRomCurve(Curve *c) 
{
	int NUM_SEG = 100;
	float t = 0.0f, inc = 1.0f/NUM_SEG;
	float pos[3], deriv[3];

	c->getGlobalCatmullRomPoint(t, pos, deriv);
// draw curve using line segments with GL_LINE_LOOP 

	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR,currentColor);
	glColor3f(0.4f, 0.4f, 0.4f);
	glBegin(GL_LINE_LOOP);
		for (int i=0 ; i<NUM_SEG ; i++, t+=inc,c->getGlobalCatmullRomPoint(t*c->getTime(), pos, deriv))
			glVertex3f(pos[0], pos[1], pos[2]);
	glEnd();
	glColor3f(currentColor[0], currentColor[1], currentColor[2]);
}


void buildRotMatrix(float *x, float *y, float *z, float *m) 
{
	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void Group::applyTransformations(float transfTime)
{
	for (Transformation *t : transformations)
	{
		float x = t->getX(),
					y = t->getY(),
					z = t->getZ(),
					angle = t->getAngle();
		switch(t->getType())
		{
			case Translate:
				glTranslatef(x,y,z);
				break;
			case TimeTranslate:{
				Curve *c = (Curve*)t;
				float pos[3], deriv[3];
				c->getGlobalCatmullRomPoint(transfTime, pos, deriv);
				renderCatmullRomCurve(c);
				glTranslatef(pos[0], pos[1], pos[2]);
				if (c->getAlign())
				{
					float *xAxis=deriv, yAxis[3],zAxis[3], *y0 = oldYaxis;
					normalize(xAxis);
					cross(xAxis, y0, zAxis);
					normalize(zAxis);
					cross(zAxis,xAxis,yAxis);
					normalize(yAxis);
					setOldYAxis(yAxis);
					float m[16];
					buildRotMatrix(xAxis, yAxis, zAxis, m);
					glMultMatrixf(m);
				}
				break;
			}
			case Rotate:
				glRotatef(angle, x,y,z);
				break;
			case TimeRotate:{
				float totalRotTime = t->getTime();
				float baseAngle = (360) / totalRotTime;
				float rotTime = fmod(transfTime, totalRotTime);
				glRotatef(rotTime*baseAngle, x, y, z);
				break;
			}
			case Scale:
				glScalef(x,y,z);
				break;
			case Color:
				glColor3f(x,y,z);
				break;
		}
	}
}

void Group::draw(float transfTime)
{
    glPushMatrix();
        applyTransformations(transfTime);
        for (Model* model : models)
            model->draw();
        for (Group* group : groups)
            group->draw(transfTime);
    glPopMatrix();
}

std::vector<Transformation*> Group::getTransformations()
{
	return this->transformations;
}

std::vector<Model*> Group::getModels()
{
	return models;
}

std::vector<Group*> Group::getGroups()
{
	return this->groups;
}

float *Group::getOldYAxis()
{
	return oldYaxis;
}
