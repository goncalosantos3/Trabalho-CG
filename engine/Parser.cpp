#include "headers/Parser.h"
#include "headers/Transformation.h"

using namespace std;
using namespace rapidxml;

int Parser::parseWindow (rapidxml::xml_node<>* windowNode)
{
	xml_attribute<>* width = windowNode->first_attribute("width");
	xml_attribute<>* height = windowNode->first_attribute("height");
	
	if (!width || !height) return 2;

	this->window = pair<float, float>(strtod(width->value(), NULL), strtod(height->value(),NULL));
	
	return 0;
}

int Parser::parseCamera (rapidxml::xml_node<>* cameraNode)
{
	xml_attribute<> *x, *y, *z;

	// Parsing camera position
	xml_node<>* node = cameraNode->first_node("position");

	if (node)
	{
		x = node->first_attribute("x");
		y = node->first_attribute("y");
		z = node->first_attribute("z");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera Position" << endl;
			return 2;
		}

		this->camera->setPosition(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
	}

	node = cameraNode->first_node("lookAt");

	if (node)
	{
		x = node->first_attribute("x");
		y = node->first_attribute("y");
		z = node->first_attribute("z");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera LookAt" << endl;
			return 2;
		}
			
		this->camera->setLookAt(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
	}
	
	node = cameraNode->first_node("up");

	if (node)
	{
		x = node->first_attribute("x");
		y = node->first_attribute("y");
		z = node->first_attribute("z");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera Up" << endl;
			return 2;
		}
			
		this->camera->setUp(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
	}

	node = cameraNode->first_node("projection");

	if (node)
	{
		x = node->first_attribute("fov");
		y = node->first_attribute("near");
		z = node->first_attribute("far");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera Projection" << endl;
			return 2;
		}

		this->camera->setFov(strtod(x->value(), NULL));
		this->camera->setNear(strtod(y->value(), NULL));
		this->camera->setFar(strtod(z->value(), NULL));
	}

	return 0;
}

Transformation* parseTransformation(rapidxml::xml_node<>* transformationNode)
{
	if (transformationNode->type() == rapidxml::node_element)
	{
		int aux = strcmp(transformationNode->name(),"scale");
		float x = strtof(transformationNode->first_attribute("x")->value(), NULL);
		float y = strtof(transformationNode->first_attribute("y")->value(), NULL);
		float z = strtof(transformationNode->first_attribute("z")->value(), NULL);
		float angle = -1;
		TransfType type;
		if (aux == 0)
			type = Scale;
		else if (aux < 0)
		{
			angle = strtof(transformationNode->first_attribute("angle")->value(), NULL);
			type = Rotate;
		}
		else
			type = Translate;
		return new Transformation(type, x, y, z, angle);
	}
	return NULL;
}

int Parser::parseTransformations(vector<Transformation*>* transformations, rapidxml::xml_node<>* transformNode)
{
	if (!transformNode)
		return 0;
	xml_node<>* transformationNode;

	bool transl=false, rot=false, scale=false;

	for (transformationNode = transformNode->first_node() ; transformationNode ; transformationNode = transformationNode->next_sibling())
	{
		Transformation *transf = parseTransformation(transformationNode);
		int error = transf == NULL;
		if (!error)
		{
			bool *verify;
			switch (transf->getType())
			{
				case Translate: verify = &transl; break;
				case Rotate: verify = &rot; break;
				default: verify = &scale; break;
			}
			if (!(*verify))
				*verify = true;
			else
			{
				cerr << "Error: Can't have more than 1 transformation of the same type!!" << endl;
				return 1;
			}
			transformations->push_back(transf);
		
		}
	}

	return 0;
}

int Parser::parseModels(vector<Shape*>* models, rapidxml::xml_node<>* modelsNode)
{
	if (!modelsNode)
		return 0;
	xml_node<>* model;
	for (model = modelsNode->first_node("model"); model; model = model->next_sibling("model"))
	{
			xml_attribute<>* file = model->first_attribute("file");
			if (!file)
			{
				cerr << "Error: No file attribute is specified no model" << endl;
				return 1;
			}
			char* filename = file->value();
			Shape* model = new Shape();
			int error = this->parseModel(model, filename);
			if (error)
			{
				cerr << "Error parsing model" << endl;
				return error;
			}
			models->push_back(model);
	}
	return 0;
}

int Parser::parseGroup(Group* group, rapidxml::xml_node<>* groupNode)
{
	xml_node<>* transfNode = groupNode->first_node("transform");
	vector<Transformation*> transformations;
	int error = this->parseTransformations(&transformations, transfNode);

	if (error) // if transmodeltions is NULL that means that an exception occured during the parse
	{
		cerr << "Error parsing Transformations" << endl;
		return error;
	}
	group->addTransformations(transformations);

	xml_node<>* modelsNode = groupNode->first_node("models");
	vector<Shape*> models;
	error = this->parseModels(&models, modelsNode);
	if (error)
	{
		cerr << "Error parsing Models!!" << endl;
		return error;
	}
	group->addModels(models);

	vector<Group*> groups;
	error = this->parseGroups(&groups, groupNode);
	if (error)
	{
		cerr << "Error Parsing Groups" << endl;
		return error;
	}
	group->addGroups(groups);

	return 0;
}

int Parser::parseGroups(vector<Group*>* groups, rapidxml::xml_node<>* node)
{
	if (!node)
		return 0;
	xml_node<>* groupNode;
	for (groupNode = node->first_node("group") ; groupNode ; groupNode = groupNode->next_sibling("group"))
	{
		Group *group = new Group();
		int error = this->parseGroup(group, groupNode);
		if (error)
			return error;
		groups->push_back(group);
	}

	return 0;
}

int Parser::parseXML(char *filePath)
{
	xml_document<> document;
	xml_node<> * root_node = NULL;

	string path = "../files/";
	path.append(filePath);
	ifstream file(path);
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	document.parse<0>(&buffer[0]);

	root_node = document.first_node("world");

	if (!root_node)
	{
		cerr << "Error reading file" << std::endl;
		return 1;
	}

	int rv = this->parseWindow(root_node->first_node("window"));
	rv = rv | this->parseCamera(root_node->first_node("camera"));

	vector<Group*> groups;
	rv = rv | this->parseGroups(&groups, root_node);
	this->groups = groups;

	file.close();

	if (rv)
		return rv;

	return 0;
}

int Parser::parseModel(Shape* model, char* filename)
{
	string path = "../files/";
	path.append(filename);
	ifstream modelFile(path);

	if (modelFile.bad())
	{
		cerr << "Error opening file \"" << filename << "\".\nMake sure the file exists...";
		return 1;
	}

	string line;

	while(getline(modelFile, line))
	{
		float x,y,z;
		if (sscanf(line.c_str(), "%f, %f, %f", &x, &y, &z) != 3)
		{
			cerr << "Error: model line has wrong syntax...\n Make sure the file was generated correctly..";
			return 1;
		}

		model->addPoint(Point(x,y,z));
	}

	return 0;
}

std::pair<float, float> Parser::getWindow()
{
	return this->window;
}

Camera* Parser::getCamera()
{
	return this->camera;
}

std::vector<Group*> Parser::getGroups()
{
	std::vector<Group*> dup;
	for (Group *g : this->groups)
		dup.push_back(new Group(*g));
	return dup;
}
