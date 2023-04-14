#include "headers/Group.h"

Group::Group()
{
	this->transformations = std::vector<Transformation*>();
	this->models = std::vector<Shape*>();
	this->groups = std::vector<Group*>();
	transformations.reserve(3);
	models.reserve(5);
	groups.reserve(5);
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

void Group::addModel(Shape* s)
{
	this->models.push_back(s);
}

void Group::addModels(std::vector<Shape*>& s)
{
	for (Shape *sh : s)
		this->addModel(sh);
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

std::vector<Transformation*> Group::getTransformations()
{
	return this->transformations;
}

std::vector<Shape*> Group::getModels()
{
	std::vector<Shape*> dup;
	for (Shape* s : this->models)
		dup.push_back(new Shape(*s));
	return dup;
}

std::vector<Group*> Group::getGroups()
{
	std::vector<Group*> dup;
	for (Group* g : this->groups)
		dup.push_back(new Group(*g));
	return dup;
}
