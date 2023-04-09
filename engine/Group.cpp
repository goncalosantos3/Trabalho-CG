#include "headers/Group.h"


Group::Group()
{
	this->transformations = std::vector<Transformation>();
	this->models = std::vector<Shape>();
	this->groups = std::vector<Group>();
}

Group::Group(Group& group)
{
	this->transformations = group.getTransformations();
	this->models = group.getModels();
	this->groups = group.getGroups();
}

Group::~Group()
{
	for (Transformation t : this->transformations)
		t.~Transformation();
	this->transformations.clear();
	for (Shape s : this->models)
		s.~Shape();
	this->models.clear();
	for (Group g : this->groups)
		g.~Group();
	this->groups.clear();
}

void Group::addTransformation(Transformation t)
{
	this->transformations.push_back(t);
}

void Group::addModel(Shape s)
{
	this->models.push_back(s);
}

void Group::addGroup(Group g)
{
	this->groups.push_back(g);
}


std::vector<Transformation> Group::getTransformations()
{
	return this->transformations;
}

std::vector<Shape> Group::getModels()
{
	std::vector<Shape> dup;
	for (Shape s : this->models)
		dup.push_back(Shape(s));
	return dup;
}

std::vector<Group> Group::getGroups()
{
	std::vector<Group> dup;
	for (Group g : this->groups)
		dup.push_back(Group(g));
	return dup;
}
