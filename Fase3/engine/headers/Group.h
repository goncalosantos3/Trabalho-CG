#pragma once

#include "../../common/headers/Shape.h"
#include "Transformation.h"

#include <vector>

class Group
{
	private:
		std::vector<Transformation*> transformations;
		std::vector<Shape*> models;
		std::vector<Group*> groups;
		float oldYaxis[3];

	public:
		Group();
		Group(const Group& group);
		~Group();

		void addTransformation(Transformation* t);
		void addTransformations(std::vector<Transformation*>& t);
		void addModel(Shape* s);
		void addModels(std::vector<Shape*>& s);
		void addGroup(Group* g);
		void addGroups(std::vector<Group*>& g);
		void setOldYAxis(float *newOldYAxis);

		std::vector<Transformation*> getTransformations();
		std::vector<Shape*> getModels();
		std::vector<Group*> getGroups();
		float *getOldYAxis();
};
