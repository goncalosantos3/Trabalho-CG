#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Model.h"
#include "Transformation.h"
#include "Curve.h"

#include <vector>

class Group
{
	private:
		std::vector<Transformation*> transformations;
		std::vector<Model*> models;
		std::vector<Group*> groups;
		float oldYaxis[3];

	public:
		Group();
		Group(const Group& group);
		~Group();

		void addTransformation(Transformation* t);
		void addTransformations(std::vector<Transformation*>& t);
		void addModel(Model* m);
		void addModels(std::vector<Model*>& s);
		void addGroup(Group* g);
		void addGroups(std::vector<Group*>& g);
		void setOldYAxis(float *newOldYAxis);

        void applyTransformations(float transfTime);

        void generateVBOs();
        void draw(float transfTime);

		std::vector<Transformation*> getTransformations();
		std::vector<Model*> getModels();
		std::vector<Group*> getGroups();
		float *getOldYAxis();
};
