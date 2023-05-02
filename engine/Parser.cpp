#include "headers/Parser.h"
#include "headers/Transformation.h"
#include "headers/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

/** 
 * @brief Parse the window section of the xml input file
 * @return int indicating if an error occured (0==ok, !=0 == not ok)
 */
int Parser::parseWindow (xml_node<>* windowNode)
{
	xml_attribute<>* width = windowNode->first_attribute("width");
	xml_attribute<>* height = windowNode->first_attribute("height");
	
	if (!width || !height) return 2;

	this->window = pair<float, float>(strtod(width->value(), NULL), strtod(height->value(),NULL));
	
	return 0;
}

int Parser::parseCamera (xml_node<>* cameraNode)
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

Transformation* parseTranslation(xml_node<>* transformationNode)
{
	TransfType type;
	xml_attribute<>* timeAttr = transformationNode->first_attribute("time");
	if (timeAttr)
	{
		type = TimeTranslate;
		float time = strtof(timeAttr->value(), NULL);
		xml_attribute<>* alignAttr = transformationNode->first_attribute("align");
		string alignStr = (alignAttr) ?string(alignAttr->value()) :"false";
		bool align = false;
		if (alignStr == "true")
			align = true;
		
		vector<Point> curvePoints;

		for (xml_node<>* point = transformationNode->first_node("point") ; point ; point = point->next_sibling("point"))
		{
			xml_attribute<> *x = point->first_attribute("x"), 
											*y = point->first_attribute("y"), 
											*z = point->first_attribute("z");

			if (!x || !y || !z)
			{
				cerr << "ERROR: Couldn't read point on translation definition!!" << endl;
				exit(1);
			}
			curvePoints.emplace_back(strtof(x->value(), NULL),
															 strtof(y->value(),NULL),
															 strtof(z->value(),NULL));
		}

		if (curvePoints.size() < 4)
		{
			cerr << "ERROR: Catmull-Rom curve should have at least 4 points!!" << endl;
			exit(1);
		}

		return new Curve(time, align, curvePoints);
	}
	float x = strtof(transformationNode->first_attribute("x")->value(), NULL);
	float y = strtof(transformationNode->first_attribute("y")->value(), NULL);
	float z = strtof(transformationNode->first_attribute("z")->value(), NULL);
	return new Transformation(Translate, x, y, z);
}

Transformation* parseRotation (xml_node<>* transformationNode)
{
	float x = strtof(transformationNode->first_attribute("x")->value(), NULL);
	float y = strtof(transformationNode->first_attribute("y")->value(), NULL);
	float z = strtof(transformationNode->first_attribute("z")->value(), NULL);
	xml_attribute<>* time = transformationNode->first_attribute("time");
	if (time) // caso seja uma rotacao temporal
		return new Transformation(x,y,z, strtof(time->value(), NULL));
	return new Transformation(Rotate,x,y,z, strtof(transformationNode->first_attribute("angle")->value(), NULL));
}

Transformation* parseTransformation(xml_node<>* transformationNode)
{
	if (transformationNode->type() == node_element)
	{
		TransfType type;
		if (strcmp(transformationNode->name(),"color")==0)
		{	
			type = Color;
			float r = strtof(transformationNode->first_attribute("r")->value(), NULL)/255.0f;
			float g = strtof(transformationNode->first_attribute("g")->value(), NULL)/255.0f;
			float b = strtof(transformationNode->first_attribute("b")->value(), NULL)/255.0f;
			return new Transformation(type, r, g, b, -1);
		}
		int aux = strcmp(transformationNode->name(),"scale");
		if (aux == 0)
		{
			type = Scale;
			float x = strtof(transformationNode->first_attribute("x")->value(), NULL);
			float y = strtof(transformationNode->first_attribute("y")->value(), NULL);
			float z = strtof(transformationNode->first_attribute("z")->value(), NULL);
			return new Transformation(type, x, y, z);
		}
		else if (aux < 0)
			return parseRotation(transformationNode);
		return parseTranslation(transformationNode);
	}
	return NULL;
}

int Parser::parseTransformations(vector<Transformation*>* transformations, xml_node<>* transformNode)
{
	if (!transformNode)
		return 0;
	xml_node<>* transformationNode;

	bool transl=false, rot=false, scale=false, color=false;

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
				case Color: verify = &color; break;
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

int Parser::parseModel(Shape* model, string filename)
{
	Shape* aux;
	if ((aux = this->getModelFromModels(model->getName(), filename)))
	{
		if (aux->getFile() == "")
		{
			cerr << "Error: Only one Model can have the same \"name\" attribute!" << endl;
			return 1;
		}
		if (model->getName() == aux->getName() && aux->getPoints().size() > 0)
		{
			for (Point p : aux->getPoints())
				model->addPoint(p);
			return 0;
		}
	}

	string path = "../files/";
	path.append(filename);
	ifstream modelFile(path);

	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
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


int Parser::parseModels(vector<Shape*>* models, xml_node<>* modelsNode)
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
			string filename = string(file->value()), name;
			xml_attribute<>* nameAttr = model->first_attribute("name");
			if (nameAttr)
				name = nameAttr->value();
			else
				name = "";
			Shape* model = new Shape(string(name), string(filename));

			int error = this->parseModel(model, filename);
			if (error)
			{
				cerr << "Error parsing model" << endl;
				return error;
			}
			models->push_back(model);
			this->allModels.push_back(model);;
	}
	return 0;
}

int Parser::parseGroup(Group* group, xml_node<>* groupNode)
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

int Parser::parseGroups(vector<Group*>* groups, xml_node<>* node)
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
		cerr << "Error reading file" << endl;
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

Shape* Parser::getModelFromModels(std::string name, std::string file)
{
	if (name == "")
	{
		for (Shape* s : allModels)
			if (s->getFile() == file)
				return s;
		return new Shape(name, file);
	}
	Shape* found = NULL;
	bool sameName = false;
	for (Shape* s : allModels)
	{
		if (s->getName() == name)
			return new Shape();
		else if (s->getFile()==file)
			found = s;
	}
	return found;
}

pair<float, float> Parser::getWindow()
{
	return this->window;
}

Camera* Parser::getCamera()
{
	return this->camera;
}

vector<Group*> Parser::getGroups()
{
	return groups;
}

vector<Shape*> Parser::getModels()
{
	return allModels;
}
